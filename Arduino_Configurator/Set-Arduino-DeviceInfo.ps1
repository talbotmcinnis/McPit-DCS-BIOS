Param(
    $DeviceName,
    $ProductId = "0x0487",
    $usb_private_path = "C:\Program Files (x86)\Arduino\hardware\teensy\avr\cores\usb_serial_hid\usb_private.h"
)

# Debug command line: .\Set-Arduino-DeviceInfo.ps1 "IFF" "0x0488" .\usb_private.h

Write-Output "DeviceName: $DeviceName"
Write-Output "usb_private_path: $usb_private_path"

# Make a backup of the item first:
$backup_count = 0
do{
    $backup_count++
    #$BackupFileName = Join-Path (Split-Path -Path $usb_private_path) 'usb_private.h.$backup_count.bak'
    $BackupFileName = "$usb_private_path.$backup_count.bak"
    
}while(Test-Path $BackupFileName -PathType Leaf)
Write-Output "Created backup file: $BackupFileName"

Copy-Item $usb_private_path -Destination $BackupFileName

$usb_private_content = ""
foreach($line in Get-Content $usb_private_path) {
    if($line.StartsWith('#define STR_PRODUCT')){
        $line = "#define STR_PRODUCT             L`"McPit-$DeviceName`""
    }
    elseif($line.StartsWith('#define PRODUCT_ID')){
        $line = "#define PRODUCT_ID              $ProductId"
    }
    elseif($line.StartsWith('#define STR_MANUFACTURER')){
        $line = "#define STR_MANUFACTURER        L`"McPit`""
    }elseif($line.StartsWith('#define STR_SERIAL_NUMBER')){
        $line = "#define STR_SERIAL_NUMBER       L`"$ProductId`""
    }

    $usb_private_content += $line + [Environment]::NewLine
}

Set-Content -Path $usb_private_path -Value $usb_private_content
#Set-Content -Path "./debug_outfile.h" -Value $usb_private_content