@echo off & setlocal
set batchPath=%~dp0
powershell.exe -noexit -file "%batchPath%..\Arduino_Configurator\Set-Arduino-DeviceInfo.ps1" "NMSP" "0x0490"
pause