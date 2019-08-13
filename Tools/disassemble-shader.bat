for %%a in (*.bin) do orbis-cu-as.exe --disassemble-raw %%a > %%~na.txt
 