SETLOCAL

set PROJECTNAME=%1
set APPVERSION=%2
set OUTDIR=%PROJECTNAME%_%APPVERSION%
windeployqt.exe %PROJECTNAME%.exe --dir %OUTDIR% --libdir %OUTDIR%\lib --no-plugins --no-quick-import --no-system-d3d-compiler --no-compiler-runtime --no-webkit2
copy %PROJECTNAME%.exe %OUTDIR%\%PROJECTNAME%.exe
rem pause
