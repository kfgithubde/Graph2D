REM Adapt GCCbase to your individual wX Installation
REM set GCCbase="C:\ProgWin32\TDM-GCC-64\bin\" if not already in the path

mkdir obj

REM  -------------- Build: Release in graph2d (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c AG2.for -o obj\AG2.o
gfortran.exe   -Wall  -m64    -c AG2Holerith.for -o obj\AG2Holerith.o
gfortran.exe   -Wall  -m64    -c AG2uline.for -o obj\AG2uline.o
gfortran.exe   -Wall  -m64    -c AG2umnmx.for -o obj\AG2umnmx.o
gfortran.exe   -Wall  -m64    -c AG2upoint.FOR -o obj\AG2upoint.o
gfortran.exe   -Wall  -m64    -c AG2users.for -o obj\AG2users.o
gfortran.exe   -Wall  -m64    -c AG2useset.FOR -o obj\AG2useset.o
gfortran.exe   -Wall  -m64    -c AG2usesetC.FOR -o obj\AG2usesetC.o
gfortran.exe   -Wall  -m64    -c AG2UsrSoftek.FOR -o obj\AG2UsrSoftek.o
gfortran.exe   -Wall  -m64    -c GetHDC.for -o obj\GetHDC.o
gfortran.exe   -Wall  -m64    -c Strings.for -o obj\Strings.o
gfortran.exe   -Wall  -m64    -c TCS.for -o obj\TCS.o
gfortran.exe   -Wall  -m64    -c TCSdrWIN.for -o obj\TCSdrWIN.o
gfortran.exe   -Wall  -m64    -c TCSinitt.for -o obj\TCSinitt.o

gfortran.exe   -Wall  -m64    -c CreateMainWindow.c -o obj\CreateMainWindow.o
gfortran.exe   -Wall  -m64    -c GetMainInstance.c -o obj\GetMainInstance.o
gfortran.exe   -Wall  -m64    -c TCSdWINc.c -o obj\TCSdWINc.o
ar.exe -r libgraph2d.a obj\AG2.o obj\AG2Holerith.o obj\AG2uline.o obj\AG2umnmx.o obj\AG2upoint.o obj\AG2users.o obj\AG2useset.o obj\AG2usesetC.o obj\AG2UsrSoftek.o obj\GetHDC.o obj\Strings.o obj\TCS.o obj\TCSdrWIN.o obj\TCSinitt.o obj\CreateMainWindow.o obj\GetMainInstance.o obj\TCSdWINc.o
ranlib libgraph2d.a

REM -------------- Build: Release in D1 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo1.for -o obj\Ag2demo1.o
windres.exe   -J rc -O coff -i C:\Users\ebb3494\DOCUME~1\Sources\Teklib\Sources\Build\Ag2dem1r.RC -o obj\Ag2dem1r.res
gfortran.exe  -o D1.exe obj\Ag2demo1.o  obj\Ag2dem1r.res -O3 -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in D2 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo2.for -o obj\Ag2demo2.o
gfortran.exe  -o D2.exe obj\Ag2demo2.o  -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in D3 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo3.for -o obj\Ag2demo3.o
gfortran.exe  -o D3.exe obj\Ag2demo3.o  -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in D4 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo4.for -o obj\Ag2demo4.o
gfortran.exe  -o D4.exe obj\Ag2demo4.o  -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in D5 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo5.for -o obj\Ag2demo5.o
gfortran.exe  -o D5.exe obj\Ag2demo5.o  -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in D6 (compiler: GNU Fortran Compiler)---------------

gfortran.exe   -Wall  -m64    -c Ag2demo6.for -o obj\Ag2demo6.o
gfortran.exe  -o D6.exe obj\Ag2demo6.o  -static  .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in ColourDemo (compiler: GNU Fortran Compiler)---------------

gfortran.exe  -Wall  -O2  -m64    -c ColourDemo.for -o obj\ColourDemo.o
gfortran.exe  -o .\ColourDemo.exe obj\ColourDemo.o  -static -s  -lgdi32 -luser32 -lkernel32 -lcomctl32 .\libgraph2d.a .\libmxml.a -mwindows

REM -------------- Build: Release in PlotHDC (compiler: GNU Fortran Compiler)---------------

gfortran.exe  -Wall  -O2  -m64    -c PlotHDC.for -o obj\PlotHDC.o
gfortran.exe  -o .\PlotHDC.exe obj\PlotHDC.o  -static -s  -lgdi32 -luser32 -lkernel32 -lcomctl32 .\libgraph2d.a .\libmxml.a -mwindows
