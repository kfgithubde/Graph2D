echo off
if /I "%1%" neq "" goto ParExists
 echo Zu veroeffentlichende Library nicht angegeben:
 echo  CPM   - Doxygen Hilfedateien
 echo  DOS   - stat. Library fuer Watcom-Compiler ab XT
 echo  Win16 - stat. Library Windows 3.1x ab 386 ohne Arithmetikprozessor
 echo  Win32 - stat. Library Win95 ab Pentium Pro
 echo  DLL32 - dyn. und Importlibrary Win95 ab Pentium Pro
 echo  GNU32 - stat. Win95-Library fuer GNU-Compiler
 echo  GNU64 - stat. 64bit Windows Library fuer GNU-Compiler
 echo  SDL32 - stat. 32bit-Library fuer SDL2 unter MinGW/GCC und Hilfedateien
 echo  SDL64 - stat. 64bit-Library fuer SDL2 unter MinGW/GCC und Hilfedateien
 echo  wX    - stat. Library fuer wX, t.b.d.
 goto done

:ParExists

if /I "%1%" neq "CPM" goto NoCpm
 echo Copying Helpfiles for CP/M
 echo on
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\CPMlatex-en\refman.pdf  ..\..\Doc\DoxyGen\CPMdox-en.pdf
 goto done
:NoCpm

if /I "%1%" neq "Dos" goto NoDos
 echo Copying Files for Watcom DOS-Library
 echo on
 copy wttekdos.lib                   ..\..\Binaries\dos\Graph2Ddos.lib
 copy GraphLib.lng                   ..\..\Binaries\dos\GraphLib.lng
 copy GRAPHLIB.FON                   ..\..\Binaries\dos\GRAPHLIB.FON
 copy GraphLib.lng                   ..\..\Binaries\dos\Demo\GraphLib.lng
 copy GRAPHLIB.FON                   ..\..\Binaries\dos\Demo\GRAPHLIB.FON
 copy ColourDemo.exe                 ..\..\Binaries\dos\Demo\*.*
 copy D?w.exe                        ..\..\Binaries\dos\Demo\D?.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\DOSlatex-en\refman.pdf  ..\..\Doc\DoxyGen\DOSdox-en.pdf
 goto done
:NoDos

if /I "%1%" neq "Win16" goto NoWin16
 echo Copying Files for 16bit Windows static Library
 echo on
 copy tekwin16.lib                   ..\..\Binaries\win16\Graph2D.lib
 copy Graph2DGerman.ini              ..\..\Binaries\win16\Graph2D.ini
 copy Graph2DGerman.ini              ..\..\Binaries\win16\Demo\Graph2D.ini
 copy ColourDemo.exe                 ..\..\Binaries\win16\Demo\*.*
 copy PlotHDC.exe                    ..\..\Binaries\win16\Demo\*.*
 copy D?.exe                         ..\..\Binaries\win16\Demo\*.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\WINlatex-en\refman.pdf  ..\..\Doc\DoxyGen\WINdox-en.pdf
 goto done
:NoWin16

if /I "%1%" neq "Win32" goto NoWin32
 echo Copying Files for 32bit Windows static Library
 echo on
 copy graph2ds.lib                   ..\..\Binaries\win32\WatcomStatic\graph2ds.lib
 copy Graph2DGerman.ini              ..\..\Binaries\win32\WatcomStatic\Graph2D.ini
 copy ColourDemo.exe                 ..\..\Binaries\win32\WatcomStatic\Demo\*.*
 copy PlotHDC.exe                    ..\..\Binaries\win32\WatcomStatic\Demo\*.*
 copy D?.exe                         ..\..\Binaries\win32\WatcomStatic\Demo\*.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\WINlatex-en\refman.pdf  ..\..\Doc\DoxyGen\WINdox-en.pdf
 goto done
:NoWin32

if /I "%1%" neq "DLL32" goto NoDll32
 echo Copying Files for 32bit Windows dynamic Library
 echo on
 copy graph2d.dll                    ..\..\Binaries\win32\WatcomDLL\graph2d.dll
 copy graph2dd.lib                   ..\..\Binaries\win32\WatcomDLL\graph2dd.lib
 copy graph2du.dll                   ..\..\Binaries\win32\WatcomDLL\graph2du.dll
 copy Graph2DGerman.ini              ..\..\Binaries\win32\WatcomDLL\Graph2D.ini
 copy graph2d.dll                    ..\..\Binaries\win32\WatcomDLL\Demo\graph2d.dll
 copy graph2du.dll                   ..\..\Binaries\win32\WatcomDLL\Demo\graph2du.dll
 copy d4user.dll                     ..\..\Binaries\win32\WatcomDLL\Demo\d4user.dll
 copy ColourDemo.exe                 ..\..\Binaries\win32\WatcomDLL\Demo\*.*
 copy PlotHDC.exe                    ..\..\Binaries\win32\WatcomDLL\Demo\*.*
 copy D?.exe                         ..\..\Binaries\win32\WatcomDLL\Demo\*.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\WINlatex-en\refman.pdf  ..\..\Doc\DoxyGen\WINdox-en.pdf
 goto done
:NoDll32

if /I "%1%" neq "GNU32" goto NoGnu32
 echo Copying Files for 32bit Windows GNU-Compiler Library
 echo on
 copy libgraph2d.a                   ..\..\Binaries\win32\Gnu\graph2d.a
 copy Graph2DGerman.ini              ..\..\Binaries\win32\Gnu\Graph2D.ini
 copy ColourDemo.exe                 ..\..\Binaries\win32\Gnu\Demo\*.*
 copy PlotHDC.exe                    ..\..\Binaries\win32\Gnu\Demo\*.*
 copy D?.exe                         ..\..\Binaries\win32\Gnu\Demo\*.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\WINlatex-en\refman.pdf  ..\..\Doc\DoxyGen\WINdox-en.pdf
 goto done
:NoGnu32

if /I "%1%" neq "GNU64" goto NoGnu64
 echo Copying Files for 64bit Windows GNU-Compiler Library
 echo on
 copy libgraph2d.a                   ..\..\Binaries\win64\*.*
 copy Graph2DGerman.ini              ..\..\Binaries\win64\Graph2D.ini
 copy ColourDemo.exe                 ..\..\Binaries\win64\Demo\*.*
 copy PlotHDC.exe                    ..\..\Binaries\win64\Demo\*.*
 copy D?.exe                         ..\..\Binaries\win64\Demo\*.*
 copy DoxOut\*.chm                   ..\..\Doc\DoxyGen
 copy DoxOut\WINlatex-en\refman.pdf  ..\..\Doc\DoxyGen\WINdox-en.pdf
 goto done
:NoGnu64

if /I "%1%" neq "SDL32" goto NoSDL32
 echo Copying Files for 32bit Windows SDL2 Library
 echo on
 copy libgraph2d.a                      ..\..\Binaries\SDL2\32bit\*.*
 copy SDL2.dll                          ..\..\Binaries\SDL2\32bit\*.*
 copy SDL2_ttf.dll                      ..\..\Binaries\SDL2\32bit\*.*
 copy Graph2DGerman.xml                 ..\..\Binaries\SDL2\32bit\*.*
 copy graph2d.ttf                       ..\..\Binaries\SDL2\32bit\*.*
 copy ..\..\Binaries\SDL2\32bit\*.dll   ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy ..\..\Binaries\SDL2\32bit\*.ttf   ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy alternative.ttf                   ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy ColourDemo.exe                    ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy PlotHDC.exe                       ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy D?.exe                            ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy D1.xml                            ..\..\Binaries\SDL2\32bit\Demo\*.*
 copy DoxOut\*.chm                      ..\..\Doc\DoxyGen
 copy DoxOut\SDLlatex-en\refman.pdf     ..\..\Doc\DoxyGen\SDLdox-en.pdf
 goto done
:NoSDL32

if /I "%1%" neq "SDL64" goto NoSDL64
 echo Copying Files for 64bit Windows SDL2 Library
 echo on
 copy libgraph2d.a                      ..\..\Binaries\SDL2\64bit\*.*
 copy SDL2.dll                          ..\..\Binaries\SDL2\64bit\*.*
 copy SDL2_ttf.dll                      ..\..\Binaries\SDL2\64bit\*.*
 copy Graph2DGerman.xml                 ..\..\Binaries\SDL2\64bit\*.*
 copy graph2d.ttf                       ..\..\Binaries\SDL2\64bit\*.*
 copy ..\..\Binaries\SDL2\64bit\*.dll   ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy ..\..\Binaries\SDL2\64bit\*.ttf   ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy alternative.ttf                   ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy ColourDemo.exe                    ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy PlotHDC.exe                       ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy D?.exe                            ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy D1.xml                            ..\..\Binaries\SDL2\64bit\Demo\*.*
 copy DoxOut\*.chm                      ..\..\Doc\DoxyGen
 copy DoxOut\SDLlatex-en\refman.pdf     ..\..\Doc\DoxyGen\SDLdox-en.pdf
 copy DoxOut\SDLdox-en\*.*              ..\..\Doc\DoxyGen\HTML\SDLdox-en
:NoSDL64

if /I "%1%" neq "wX" goto NowX
 echo Copying Files for wxWidgets Library
 echo Not Yet Implemented
 echo on
 goto done
:NowX

echo Unknown Option

:done
