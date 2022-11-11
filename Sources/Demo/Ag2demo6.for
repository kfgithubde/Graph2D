C Demo Seite A-14 aus dem AG2 User-Manual: Unterschiedliche Linienarten und Farben
C    Link durch L80 ag2demo5,calendar,tek/s,forlib/s
C
      DIMENSION RPM(4),POW(20),SPC(20),TOR(20),LAB1(18),LAB2(10),
     & LAB3(16),LAB4(6)
C * Graph Title: E  N  G  I  N  E     P  E  R  F  O  R  M  A  N  C  E
      data lab1/69,78,71,73,78,69,32,80,69,82,70,79,82,77,65,78,67,69/
C * Scale Label: H  O  R  S  E  P  O  W  E  R
      data lab2/72,79,82,83,69,80,79,87,69,82/
C * Scale Label: F  U  E  L     C  O  N  S  U  M  P  T  I  O  N
      data lab3/70,85,69,76,32,67,79,78,83,85,77,80,84,73,79,78/
C * Scale Label: T  O  R  Q  U  E
      data lab4/84,79,82,81,85,69/
      data rpm /-1.,19.,1000.,175./
      data pow/19.,14.,20.,23.,27.,31.,34.,37.,40.,42.,44.,46.,47.5,
     &48.,49.,49.5,49.9,50.3,50.5,50.6/
      data spc/19.,13.7,13.5,13.17,12.77,12.3,11.8,11.25,10.65,10.0,
     &9.35,8.8,8.35,8.0,7.8,7.7,7.65,7.66,7.8,8.1/
      data tor/19.,60.,64.,68.,71.5,74.9,77.9,80.4,82.3,
     &84.,86.,87.2,88.,88.6,87.9,87.2,86.,84.8,83.,81./
C * Label Length
      l1= 18
      l2= 10
      l3= 16
      l4= 6
      call initt (120)
      call binitt
      call slimx(200,800)
C * Each y-axis tic intervall will span 40 raster units
      call slimy(100,100+16*40)
      call ytics (16)
      call check(rpm,pow)
      call dsplay (rpm,pow)
C * y-data differs in the second curve, so reinitialization is needed
      call dinity
C second curve spans 6 tic intervalls
      call slimy (140,140+6*40)
      call ytics (6)
      call line (54)
C line (54) bei Windows Default: dotted
      call ylocrt(0)
      call xfrm(0)
      call xlab(0)
      call yfrm(2)
      call check (rpm,spc)
      call dsplay(rpm,spc)
C * third curve is handled sililarily to second curve
      call dinity
      call line (7494)
C line (7494) = Test auf Indexoverflow!!!
      call line (3)
      call ytics (4)
      call slimy (460,460+4*40)
      call check (rpm,tor)
      call dsplay (rpm,tor)
C * print labels interactively
      call dcursr(ic,ix,iy)
      call movabs(ix,iy)
      call hlabel(L1,lab1)
      call dcursr (ic,ix,iy)
      call movabs(ix,iy)
      call vlabel (l2,lab2)
      call dcursr (ic,ix,iy)
      call movabs(ix,iy)
      call vlabel (l3,lab3)
      call dcursr (ic,ix,iy)
      call movabs (ix,iy)
      call vlabel (l4,lab4)
      call finitt (0,700)
      end
