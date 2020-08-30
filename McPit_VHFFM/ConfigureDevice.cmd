@echo off & setlocal
set batchPath=%~dp0
powershell.exe -noexit -file "%batchPath%..\Arduino_Configurator\Set-Arduino-DeviceInfo.ps1" "VHFFM" "0x0493"
REM %userprofile%\documents\GitHub\McPit-DCS-BIOS\Arduino_Configurator\Set-Arduino-DeviceInfo.ps1 "VHFFM" "0x0493"
pause