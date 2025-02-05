REM Adapt wxBase and GCCbase to your individual wX Installation
set wxBase="C:\ProgWin32\wx3-1-5Us64bit"
set GCCbase="C:\ProgWin32\TDM-GCC-64"


mkdir obj

REM ---------- Build: Release in D1 (compiler: GNU GCC Compiler)---------------

gcc.exe -c -m64 -Wall Ag2demo1.for -o obj\Ag2demo1.o
objcopy  --strip-symbol=main --globalize-symbol=MAIN__ obj/ag2demo1.o
windres.exe   -J rc -O coff -i Ag2dem1r.RC -o obj\Ag2dem1r.res
gcc.exe -Wall -m64 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c wxTCSmain.cpp -o obj\wxTCSmain.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D1.exe  obj\wxTCSmain.o obj\Ag2dem1r.res obj\Ag2demo1.o -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows

REM ----------- Build: Release in D2 (compiler: GNU GCC Compiler)---------------

gcc.exe -Wall -m64 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c Ag2demo2.for -o obj\Ag2demo2.o
gcc.exe -Wall -m64 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c wxTCSmain2sub.cpp -o obj\wxTCSmain2sub.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D2.exe obj\Ag2demo2.o obj\wxTCSmain2sub.o  -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows

REM -------------- Build: Release in D3 (compiler: GNU GCC Compiler)---------------

gcc.exe -Wall -m64 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c ag2demo3changed.s -o obj\ag2demo3changed.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D3.exe obj\ag2demo3changed.o obj\wxTCSmain.o  -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows

REM -------------- Build: Release in D4 (compiler: GNU GCC Compiler)---------------

gcc.exe -c -m64 -Wall Ag2demo4.for -o obj\Ag2demo4.o
objcopy  --strip-symbol=main --globalize-symbol=MAIN__ obj/ag2demo4.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D4.exe obj\wxTCSmain.o  obj\Ag2demo4.o -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows

REM -------------- Build: Release in D5 (compiler: GNU GCC Compiler)---------------

gcc.exe -c -m64 -Wall Ag2demo5.for -o obj\Ag2demo5.o
objcopy  --strip-symbol=main --globalize-symbol=MAIN__ obj/ag2demo5.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D5.exe obj\wxTCSmain.o  obj\Ag2demo5.o -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows


REM -------------- Build: Release in D6 (compiler: GNU GCC Compiler)---------------

gcc.exe -c -m64 -Wall Ag2demo6.for -o obj\Ag2demo6.o
objcopy  --strip-symbol=main --globalize-symbol=MAIN__ obj/ag2demo6.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o D6.exe obj\wxTCSmain.o  obj\Ag2demo6.o -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows


REM -------------- Build: Release in ColourDemo (compiler: GNU GCC Compiler)---------------

gcc.exe -c -m64 -Wall ColourDemo.for -o obj\ColourDemo.o
objcopy  --strip-symbol=main --globalize-symbol=MAIN__ obj/ColourDemo.o
g++.exe -L%wxBase%\lib\gcc_lib -L.\ -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o ColourDemo.exe obj\wxTCSmain.o  obj\ColourDemo.o -s -m64 -m64  -lgraph2d -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows

REM -------------- Build: Release in wxDemo (compiler: GNU GCC Compiler)---------------

gcc.exe -Wall -pipe -mthreads -J..\ -D__GNUWIN32__ -D__WXMSW__ -DwxUSE_UNICODE -O2 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c wxDemo\ftnsub.f08 -o obj\ftnsub.o
windres.exe -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu  -J rc -O coff -i C:\Users\ebb3494\DOCUME~1\Sources\Teklib\Sources\Build\wxDemo\resource.rc -o obj\resource.res
gcc.exe -Wall -pipe -mthreads -J..\ -D__GNUWIN32__ -D__WXMSW__ -DwxUSE_UNICODE -O2 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c wxDemo\wxDemoApp.cpp -o obj\wxDemoApp.o
gcc.exe -Wall -pipe -mthreads -J..\ -D__GNUWIN32__ -D__WXMSW__ -DwxUSE_UNICODE -O2 -m64 -I%wxBase%\include -I%wxBase%\lib\gcc_lib\mswu -c wxDemo\wxDemoMain.cpp -o obj\wxDemoMain.o
g++.exe -L%wxBase%\lib\gcc_lib -L%GCCbase%\lib\gcc\mingw32\10.3.0 -o wxDemo\wxDemo.exe obj\ftnsub.o obj\wxDemoApp.o obj\wxDemoMain.o  obj\resource.res -s -mthreads -m64  libgraph2d.a -lwxmsw31u -lwxpng -lwxjpeg -lwxtiff -lwxzlib -lwxexpat -lkernel32 -luser32 -lgdi32 -lwinspool -lcomdlg32 -ladvapi32 -lshell32 -lole32 -loleaut32 -luuid -lcomctl32 -lwsock32 -lodbc32 -lshlwapi -lversion -loleacc -luxtheme -lgfortran -mwindows
