C
C  TCS Graphik Grundfunktionen für C128 unter CP/M
C
C     Version (2002,312,1)
C
C               Vereinheitlichung DOS/Windowsversion
C               Zusätzliches Modul: TCSdrCPM.FOR, früher Teil von TCS.FOR.
C                Muss vor TCS.FOR in die Library aufgenommen werden, da sonst
C                der 1-pass Linker von CP/M rscale,setmrg,rrotat nicht findet.
C               Implementierung Unterprogramm TCSLEV
C               Umbenennung TKTRNX.FOR in 'TKTRNX.FD'
C
C
C     15.10.02         Dr.-Ing. K. Friedewald
C
C              Bugfix SVSTAT, RESTAT: Beginn Kommentarzeilen "C" statt "c"
C                                     Länge Commonblock korrigiert
C
C     08.11.02         Dr.-Ing. K. Friedewald
C



C
C  Ausgabe der Softwareversion
C
      subroutine TCSLEV(LEVEL)
      dimension LEVEL(3)
C    Aenderungsjahr
      LEVEL(1)=2002
C    Aenderungstag
      LEVEL(2)= 312
C    System= CP/M
      LEVEL(3)=   1

      return
      end



C
C  Abspeichern Terminal Status Area
C

      subroutine SVSTAT (Array)
      real array(1)
      common /TKTRNX/ arr(14)
      do 10 i=1,14
10     array(i)= arr(i)
      call seeloc (i,j)
      array(15)= float(i)
      array(16)= float(j)
      return
      end



      subroutine RESTAT (Array)
      real array(1)
      common /TKTRNX/ arr(14)
      do 10 i=1,14
10     arr(i)= array(i)
      call movabs (ifix(array(15)), ifix(array(16)))
      return
      end

C
C Graphic Input
C

      subroutine DCURSR (IC,IX,IY)
      call gincrs (ic,ix,iy)
      ix= ix*8/5
      iy= (199-iy)*4
      return
      end

C
C  Alphanumerische Ausgabe
C

      subroutine CSIZE (Ixlen,iylen)
      Ixlen= 11
      Iylen= 32
      return
      end

C
C  Initialisierungsroutinen
C

      subroutine INITT
      call lintrn
      call swindo (0,1023,0,780)
      call vwindo (0.,1023.,0.,780.)
      call rrotat (0.)
      call rscale (1.)
      call setmrg (0,1023)
      call initt1
      call home
      return
      end



      subroutine SEELOC (IX,IY)
      call seelc1 (ix,iy)
      ix= ix*8/5
      iy= (199-iy)*4
      return
      end

