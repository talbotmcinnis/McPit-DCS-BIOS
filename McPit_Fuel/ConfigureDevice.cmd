@echo off & setlocal
set batchPath=%~dp0
powershell.exe -noexit -file "%batchPath%..\Arduino_Configurator\Set-Arduino-DeviceInfo.ps1" "FUEL" "0x0489"
pause