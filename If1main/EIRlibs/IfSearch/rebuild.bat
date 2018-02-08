qmake.exe D:/SVN2/trunk/trunk/EclipseIR/IfSearch/IfSearch.pro -r -spec win32-g++ CONFIG+=release
cd ..\IfSearch-build-desktop-Qt_4_8_0_for_Desktop_-_MinGW__Qt_SDK__Release
DEL -Y .\release\*.*
mingw32-make.exe -f Makefile.Release 
cd ..\IfSearch