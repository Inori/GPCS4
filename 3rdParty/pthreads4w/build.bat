@echo off

call "%SystemDrive%\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"

nmake %1 %2