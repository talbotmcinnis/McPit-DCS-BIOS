#include <EOS.h> // includes the core eos libs
#include <EEPROM.h> // required for eos to store config in eeprom

// REGION global vars 
// create a bank of 7 inputs on pins 2-8
EOSArduinoDigital gDigitalInputBank(10,11,12,13,14,15,16); // 0,1 are reserved for serial comms

boolean gPollDataChanged = false; // will be used to detect if data has changed

//Config variables
byte gName[9] = "T01Board"; // Board name
byte gFWVersion[5] = "T001"; // Firmware version

//ENDREGION global vars

//REGION helper functions

/* ReadEPROM()
 read config from EEPROM
 checks if the EEPROM is "stamped" properly from a previous write
 if so it reads the name and firmware from EEPROM
 EEPROM indexes
 0  version?  not sure exactly what this is used for
 1  first byte of written "stamp"
 2  second byte of written "stamp"
 3  EOS node address
 4  EOS group id
 5  start index of name (currently limited to 9 bytes including string terminator
 */
void ReadEPROM()
{
  byte ver = EEPROM.read(0); // 
  if (EEPROM.read(1) == 0xbb && EEPROM.read(2) == 0x88)
  {
    EosBus.setNodeAddress(EEPROM.read(3));
    EosBus.setGroup(EEPROM.read(4));
    if (ver == 3)
    {
      // name limited to 8 bytes plus terminator byte
      for(int i=0; i<8; i++) 
      {
        gName[i] = EEPROM.read(5+i);
      }
    }
  }
}

/* UpdateEPROM()
 write config from EEPROM and "stamp" it
 EEPROM indexes
 0  version?  not sure exactly what this is used for
 1  first byte of written "stamp"
 2  second byte of written "stamp"
 3  EOS node address
 4  EOS group id
 5  start index of name (currently limited to 9 bytes including string terminator
 */
void UpdateEPROM()
{
  // not sure what is used for
  EEPROM.write(0, 3);

  EEPROM.write(1, 0xbb); 
  EEPROM.write(2, 0x88); 

  //write node address
  EEPROM.write(3, EosBus.getNodeAddress()); 

  //write group id
  EEPROM.write(4, EosBus.getGroup()); 

  // name is limited to 8 bytes plus string terminator
  for(int i=0; i<8; i++) 
  {
    EEPROM.write(5+i, gName[i]);
  }
}
//ENDREGION helper functions

void setup() 
{
  Serial.begin(9600);
  Serial.println("setup...");
  // setup our inputs
  gDigitalInputBank.begin(); 

  // get config values from EEPROM
  ReadEPROM(); 

  // initialize EOS Bus using pin 13 for rs-485 tx at 250000 baud
  // gardoc recomends leaving baud at 250000
  EosBus.begin(6, 250000); 

  // flag for update to send initial state
  gPollDataChanged = true;
}

void loop()
{
  // check if our inputs have changed state
  gPollDataChanged |= gDigitalInputBank.process();

  // Determine EosBus State on each iteration of loop
  // EosBus utilizes a state machine
  byte state = EosBus.process();

  /* Handle each state in EosBus State Machine
   These are documented on gadrocs website
   http://www.gadrocsworkshop.com/eos/protocol
   */
  switch(state)
  {
  case BUS_TIMEOUT:
    break;

  case BUS_ERROR:
    break;

  case BUS_PACKETREADY:

    /* Determine EOS Command
     These are documented on gadrocs website
     http://www.gadrocsworkshop.com/eos/protocol
     */
    switch (EosBus.getCommand())
    {

      // CORE COMMAND..REQUIRED
      // Handle set address command
    case PACKET_COMMAND_SET_ADDRESS:
      {

        // read the address then update the EEPROM
        byte newAddress = EosBus.readByte();
        EosBus.setNodeAddress(newAddress);
        UpdateEPROM();
        break;
      }

      // CORE COMMAND..REQUIRED
      // Handle groupid set request
    case PACKET_COMMAND_SET_GROUP:
      {

        // read the groupid then call update to EEPROM
        byte newGroup = EosBus.readByte();
        EosBus.setGroup(newGroup);
        UpdateEPROM();
        break;
      }

      // CORE COMMAND..REQUIRED
      // Handle set name request
    case PACKET_COMMAND_SET_NAME:
      {
        // Read the name then call update to EEPROM
        for(int i=0; i<8; i++)
        {
          gName[i] = EosBus.readByte();
        }
        UpdateEPROM();
        break;
      }

      // CORE COMMAND..REQUIRED
      // handle a poll request
    case PACKET_COMMAND_POLL:
      {
        // setup our response packet
        EosBus.beginResponse();     

        // Add input data to poll response if it has changed    
        if (gPollDataChanged)
        {
          EosBus.addData(gDigitalInputBank.getState());
          gPollDataChanged = false;
        }

        // send data and handle it if it fails
        if (!EosBus.send())
        {
          // handle failure here
        }
        break;
      }

      // CORE COMMAND..REQUIRED
      // handle an info request
    case PACKET_COMMAND_INFO:
      {
        // setup our response packet
        EosBus.beginResponse();

        // add our name to the packet
        // name limited to 8 bytes plus terminator byte
        for(int i=0;i<8;i++)  
        {
          EosBus.addData(gName[i]);
        }
        
        // add firmware version to response
        for(int i=0;i<4;i++)
        {
          EosBus.addData(gFWVersion[i]);
        }

        // add our capabilities to the response
        // we only have 7 digital inputs
        EosBus.addData((byte)7); // No digital inputs
        EosBus.addData((byte)0); // analog inputs
        EosBus.addData((byte)0); // encoder inputs
        EosBus.addData((byte)0); // LED channels
        EosBus.addData((byte)0); // Steppers
        EosBus.addData((byte)0); // Servos
        EosBus.addData((byte)0); // Alpha numeric Display

        // add our group to the response
        EosBus.addData(EosBus.getGroup());

        //  send the data and handle it if it fails
        if (!EosBus.send())
        {
          // handle failure here
        }
        break;
      }

    // Not used in this example
    case PACKET_COMMAND_BACKLIGHT_POWER:
      {
        boolean power = EosBus.readBoolean();
      }

    // Not used in this example
    case PACKET_COMMAND_BACKLIGHT_SET_LEVEL:
      {
        byte level = EosBus.readByte();
      } 

    // Not used in this example
    case PACKET_COMMAND_LED_POWER:
      {
        byte led = EosBus.readByte();
        boolean power = EosBus.readBoolean();
        break;
      }

    // Not used in this example
    case PACKET_COMMAND_LED_SET_LEVEL:
      {
        byte led = EosBus.readByte();
        break;    
      }

      // CORE COMMAND..REQUIRED
      // handle request to reset
    case PACKET_COMMAND_RESET:
      {
        // force a refresh of data
        gPollDataChanged = true;
        break;
      }

    // Not used in this example
    case PACKET_COMMAND_DISPLAY_SETTEXT:
      {
        byte displayId = EosBus.readByte();
        break;
      }
    }
    break; 
  }
}















