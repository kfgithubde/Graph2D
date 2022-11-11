#!/bin/bash
mkdir obj
gfortran -c AG2.for -o obj/AG2.o
gfortran -c AG2Holerith.for -o obj/AG2Holerith.o
gfortran -c AG2uline.for -o obj/AG2uline.o
gfortran -c AG2umnmx.for -o obj/AG2umnmx.o
gfortran -c AG2upoint.for -o obj/AG2upoint.o
gfortran -c AG2users.for -o obj/AG2users.o
gfortran -c AG2useset.for -o obj/AG2useset.o
gfortran -c AG2usesetC.for -o obj/AG2usesetC.o
gfortran -c AG2UsrSoftek.for -o obj/AG2UsrSoftek.o

gfortran -c AG2Holerith.for -o obj/AG2Holerith.o

gfortran -c Strings.for -o obj/Strings.o
gfortran -c TCS.for -o obj/TCS.o
gfortran -c TCSdrSDL.for -o obj/TCSdrSDL.o

gcc -c TCSdSDLc.c -o obj/TCSdSDLc.o

ar -r libgraph2d.a obj/AG2.o obj/AG2Holerith.o obj/AG2uline.o obj/AG2umnmx.o obj/AG2upoint.o obj/AG2users.o obj/AG2useset.o obj/AG2usesetC.o obj/AG2UsrSoftek.o obj/Strings.o obj/TCS.o obj/TCSdrSDL.o obj/TCSdSDLc.o
ranlib libgraph2d.a

# Examples

gfortran -c t1.for -o obj/t1.o
gfortran -L./ -o t1.exe obj/t1.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo1.for -o obj/Ag2demo1.o
gfortran -L./ -o D1.exe obj/Ag2demo1.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo2.for -o obj/Ag2demo2.o
gfortran -L./ -o D2.exe obj/Ag2demo2.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo3.for -o obj/Ag2demo3.o
gfortran -L./ -o D3.exe obj/Ag2demo3.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo4.for -o obj/Ag2demo4.o
gfortran -L./ -o D4.exe obj/Ag2demo4.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo5.for -o obj/Ag2demo5.o
gfortran -L./ -o D5.exe obj/Ag2demo5.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c Ag2demo6.for -o obj/Ag2demo6.o
gfortran -L./ -o D6.exe obj/Ag2demo6.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf

gfortran -c ColourDemo.for -o obj/ColourDemo.o
gfortran -L./ -o ColourDemo.exe obj/ColourDemo.o  -s -lgraph2d -lmxml -lSDL2 -lSDL2_ttf