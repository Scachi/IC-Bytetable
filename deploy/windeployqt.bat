SETLOCAL
set PROJECTNAME=%1
set APPVERSION=%2
set OUTDIR=%PROJECTNAME%_%APPVERSION%
echo d | xcopy %PROJECTNAME%.exe %OUTDIR%\%PROJECTNAME%.exe
windeployqt.exe %OUTDIR%\%PROJECTNAME%.exe --release --dir %OUTDIR%
echo "outdir:"
echo %OUTDIR%
rem pause
