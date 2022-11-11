# Makefile zur Compilation mit den GCC-Compilern
# 
# Aufruf: C:\ProgWin32\MinGW\msys\1.0\bin\make d1.exe (...d6.exe, clean, all)
# Anmerkung: Es wurde auf Folgezeilen verzichtet, um das Makefile sowohl
#  mit GNU-Make (Folgezeile durch \ ankündigen) als auch mit Watcom-Make
#  verwenden zu können. Make clean und make all ist nicht mit WMAKE aufrufbar,
#  da die (für GNU-Make unbekannte Abhängigkeit) .SYMBOLIC feht 

FTNLIBDIR = C:\UsrProg\Dev-Cpp\MinGW32\lib\gcc\mingw32\4.6.1
FTNLIB = gfortran

FTNOPT = -x f77
COPT= -Wno-pointer-to-int-cast
LNKOPT = -mwindows -static-libgcc -static-libgfortran

.SUFFIXES:
.SUFFIXES: .o .c .for .coff .rc

# 
# Implicit Rules
#
.c.o:
	gcc -c $(COPT) $<
.for.o:
	gfortran -c $(FTNOPT)  $<
.rc.coff:
	windres $< $*.coff
 
#
# Targets
#
TCSLIB = createmainwindow.o getmaininstance.o tcs.o tcsdwinc.o tcsdrwin.o tcsinitt.o strings.o 
AG2LIB = AG2.o AG2Holerith.o
AG2USRLIB = AG2uline.o AG2umnmx.o AG2upoint.o AG2users.o AG2useset.o AG2usesetC.o AG2UsrSoftek.o

libteklib.a : $(TCSLIB) $(AG2LIB) $(AG2USRLIB)
	ar crs $@ $?
	ranlib $@
 
d1.exe: ag2demo1.o ag2dem1r.coff libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 

d2.exe: ag2demo2.o libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 

d3.exe: ag2demo3.o libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 

d4.exe: ag2demo4.o libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 

d5.exe: ag2demo5.o libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 

d6.exe: ag2demo6.o libteklib.a
	gcc -o $@ $(LNKOPT) $? -L$(FTNLIBDIR) -l$(FTNLIB) 
	
clean:
	rm -f *.o
	rm -f *.coff
	rm -f *.a
	rm -f *.exe

.CONTINUE:
all: d1.exe d2.exe d3.exe d4.exe d5.exe d6.exe
