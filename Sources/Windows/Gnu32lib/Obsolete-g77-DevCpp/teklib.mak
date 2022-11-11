# Makefile zur Compilation mit den GCC-Compilern
#  für die Verwendung durch die Watcom Makeutility
# Aufruf: wmake -f teklib.mak d1.exe (...d6.exe)

.EXTENSIONS:
.EXTENSIONS: .o .c .for .coff .rc

# 
# Implicit Rules
#
.c.o:
 gcc -c -Wno-pointer-to-int-cast $<
.for.o:
 gfortran -c -x f77  $<
.rc.coff:
 windres $< $*.coff
 
#
# Targets
#
libteklib.a : createmainwindow.o getmaininstance.o tcs.o tcsdwinc.o &
		      AG2.o AG2Holerith.o  &
		      AG2uline.o AG2umnmx.o AG2upoint.o AG2users.o &
			  AG2useset.o AG2usesetC.o AG2UsrSoftek.o &
		      strings.o tcsdrwin.o tcsinitt.o
 ar crs $@ $?
 ranlib $@
 
d1.exe: ag2demo1.o ag2dem1r.coff libteklib.a
 gcc -o d1.exe -mwindows ag2demo1.o ag2dem1r.coff libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   

d2.exe: ag2demo2.o libteklib.a
 gcc -o d2.exe -mwindows ag2demo2.o libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   

d3.exe: ag2demo3.o libteklib.a
 gcc -o d3.exe -mwindows ag2demo3.o libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   

d4.exe: ag2demo4.o libteklib.a
 gcc -o d4.exe -mwindows ag2demo4.o libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   

d5.exe: ag2demo5.o libteklib.a
 gcc -o d5.exe -mwindows ag2demo5.o libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   

d6.exe: ag2demo6.o libteklib.a
 gcc -o d6.exe -mwindows ag2demo6.o libteklib.a -LC:\ProgWin32\MinGW\lib\gcc\mingw32\4.6.2 -lgfortran   
