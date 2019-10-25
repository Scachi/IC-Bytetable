SETLOCAL

set PROJECTNAME=%1
set APPVERSION=%2
set OUTDIR=%PROJECTNAME%_%APPVERSION%
rem windeployqt.exe %PROJECTNAME%.exe --release --dir %OUTDIR% --libdir %OUTDIR%\lib --no-plugins --no-quick-import --no-system-d3d-compiler --no-compiler-runtime --no-webkit2
rem copy %PROJECTNAME%.exe %OUTDIR%\%PROJECTNAME%.exe
copy %PROJECTNAME%.exe %OUTDIR%\%PROJECTNAME%.exe
windeployqt.exe %OUTDIR% --libdir %OUTDIR%\lib --no-webkit2
rem pause
