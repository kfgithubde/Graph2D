REM Adapt GCCbase to your individual wX Installation
REM set GCCbase="C:\ProgWin32\TDM-GCC-64\bin\" if not already in the path

mkdir obj

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
