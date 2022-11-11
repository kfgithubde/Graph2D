echo off
if /I "%1%" neq "" goto ParExists
 echo Zielsystem nicht angegeben:
 echo  CPM   - Paketierung der CP/M-Files fuer C128
 echo  DOS   - stat. Library fuer Watcom-Compiler ab XT
 echo  Win16 - stat. Library Windows 3.1x ab 386 ohne Arithmetikprozessor
 echo  Win32 - stat. Library Win95 ab Pentium Pro
 echo  DLL32 - dyn. und Importlibrary Win95 ab Pentium Pro
 echo  GNU32 - stat. Win95-Library fuer GNU-Compiler
 echo  GNU64 - stat. Win64-Library fuer GNU-Compiler
 echo  SDL32 - stat. Library fuer 32bit SDL
 echo  SDL64 - stat. Library fuer 64bit SDL
 echo  SDLPI - stat. Library fuer SDL Raspberry Pi
 echo  WX    - stat. Library fuer wxWidgets
 goto done

:ParExists
if /I "%1%" neq "CPM" goto NoCpm
 echo Copying Files for Commodore CP/M
 copy ..\CPM\*.* *.*
 copy ..\CPM\AG2NoSav *.*
 copy ..\CPM\Make *.*
 copy ..\CPM\Util *.*
 copy ..\CPM\Pascal *.*
 goto finish
:NoCpm

if /I "%1%" neq "Dos" goto NoDos
 echo Copying Files for Watcom DOS-Library
 copy ..\dos\*.* *.*
 copy ..\dos\Watcom *.*
 copy ..\..\OpenContent\Fonts\Normal.fon Graphlib.fon
 goto finish
:NoDos

if /I "%1%" neq "Win16" goto NoWin16
 echo Copying Files for 16bit Windows static Library
 copy ..\windows\*.* *.*
 copy ..\windows\Win16 *.*
 goto finish
:NoWin16

if /I "%1%" neq "Win32" goto NoWin32
 echo Copying Files for 32bit Windows static Library
 copy ..\windows\*.* *.*
 copy ..\windows\Win32lib *.*
 copy ..\..\OpenContent\binaries\Watcom\include\*.*
 copy ..\..\OpenContent\binaries\Watcom\lib\*.*
 goto finish
:NoWin32

if /I "%1%" neq "DLL32" goto NoDll32
 echo Copying Files for 32bit Windows dynamic Library
 copy ..\windows\*.* *.*
 copy ..\windows\Win32dll *.*
 copy ..\..\OpenContent\binaries\Watcom\include\*.*
 copy ..\..\OpenContent\binaries\Watcom\lib\*.*
 goto finish
:NoDll32

if /I "%1%" neq "GNU32" goto NoGnu32
 echo Copying Files for 32bit Windows GNU-Compiler Library
 copy ..\windows\*.* *.*
 copy ..\windows\Gnu32lib\ *.*
 copy ..\..\OpenContent\binaries\gcc\include\mxml.h
 copy ..\..\OpenContent\binaries\gcc\include\sglib.h
 copy ..\..\OpenContent\binaries\gcc\lib\32bit\*.*
 copy ..\windows\Gnu32lib\CodeBlocks *.*
 goto finish
:NoGnu32

if /I "%1%" neq "GNU64" goto NoGnu64
 echo Copying Files for 64bit Windows GNU-Compiler Library
 copy ..\windows\*.* *.*
 copy ..\windows\Gnu64lib *.*
 copy ..\..\OpenContent\binaries\gcc\include\mxml.h
 copy ..\..\OpenContent\binaries\gcc\include\sglib.h
 copy ..\..\OpenContent\binaries\gcc\lib\64bit\*.*
 goto finish
:NoGnu64

if /I "%1%" neq "SDL32" goto NoSDL32
 echo Copying Files for SDL 32bit Library
 copy ..\SDL\*.* *.*
 copy ..\SDL\MinGW32\ *.*
 copy ..\..\OpenContent\binaries\gcc\include\*.*
 copy ..\..\OpenContent\binaries\gcc\lib\32bit\*.*
 copy ..\..\OpenContent\Fonts\RobotoMono-VariableFont_wght.ttf graph2d.ttf
 copy ..\..\OpenContent\Fonts\bitstream_vera_seri.ttf alternative.ttf
 goto finish
:NoSDL32

if /I "%1%" neq "SDL64" goto NoSDL64
 echo Copying Files for SDL 64bit Library
 copy ..\SDL\*.* *.*
 copy ..\SDL\MinGW64\ *.*
 copy ..\..\OpenContent\binaries\gcc\include\*.*
 copy ..\..\OpenContent\binaries\gcc\lib\64bit\*.*
 copy ..\..\OpenContent\Fonts\RobotoMono-VariableFont_wght.ttf graph2d.ttf
 copy ..\..\OpenContent\Fonts\bitstream_vera_seri.ttf alternative.ttf
 goto finish
:NoSDL64

if /I "%1%" neq "SDLPI" goto NoSDLPI
 echo Copying Files for SDL Linux
 copy ..\SDL\*.* *.*
 copy ..\SDL\RaspberryPi32\ *.*
 copy ..\..\OpenContent\binaries\gcc\include\*.*
 copy ..\..\OpenContent\Fonts\RobotoMono-VariableFont_wght.ttf graph2d.ttf
 copy ..\..\OpenContent\Fonts\bitstream_vera_seri.ttf alternative.ttf
 goto finish
:NoSDLPI

if /I "%1%" neq "WX" goto NoWX
 echo Copying Files for wxWidgets Library
 echo Not Yet Implemented
   goto done
 goto finish
:NoWX

echo Unknown Option
goto done

:finish
copy ..\CommonSources\*.* *.*
copy ..\demo\*.* *.*

:done
 echo Done.
