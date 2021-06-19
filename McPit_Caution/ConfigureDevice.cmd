@echo off & setlocal
set batchPath=%~dp0
powershell.exe -noexit -file "%batchPath%..\Arduino_Configurator\Set-Arduino-DeviceInfo.ps1" "Caution" "0x0496"
pause