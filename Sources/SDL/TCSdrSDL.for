C> \file       TCSdrSDL.for
C> \brief      SDL Port: High-Level Driver
C> \version    (2022,305,6)
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> SDL2-spezifische TCS-Routinen
C> \note \verbatim
C>    Erweiterungen gegenüber Tektronix:
C>     subroutine TOUTSTC (String): Ausgabe Fortran-String
C>     subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15)
C>     subroutine TXTCOL (iCol): Setzen Textfarbe
C>     subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar)
C>     subroutine DefaultColour: Wiederherstellung Defaultfarben
C> \endverbatim
C>
C>
C> \~english
C> SDL2 specific subroutines
C> \note \verbatim
C>    Supplement to Tektronix:
C>     subroutine TOUTSTC (String): Ausgabe Fortran-String
C>     subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15)
C>     subroutine TXTCOL (iCol): Setzen Textfarbe
C>     subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar)
C>     subroutine DefaultColour: Wiederherstellung Defaultfarben
C> \endverbatim
C> \~
C>



C
C  Ausgabe der Softwareversion
C
      subroutine TCSLEV(LEVEL)
      integer LEVEL(3)
      LEVEL(1)=2022     ! Aenderungsjahr
      LEVEL(2)= 305     ! Aenderungstag
      LEVEL(3)=   6     ! System= SDL
      return
      end



C
C>  Initialisierung Hard- und Software
C
      subroutine INITT (iDummy)
      include 'Tktrnx.fd'
      call initt1 ! Init Hardware
      call initt2 ! Reset Common TKTRNX ohne Einfluss auf das Journal
      call nrmsiz
      call italir
      call home
      return
      end



      subroutine INITT2
C INITT2 auch durch RepaintBuffer aufgerufen -> Schreiben Journal unmoeglich!
      include 'Tktrnx.fd'
      call lintrn
      call swindo (0,1023,0,780)
      call vwindo (0.,1023.,0.,780.)
      call rrotat (0.)
      call rscale (1.)
      call setmrg (0,1023)
      return
      end




C
C  Abspeichern Terminal Status Area (wie MS Windows und DOS)
C

      subroutine SVSTAT (Array)
      integer array(1)
      include 'Tktrnx.fd'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
       array(i)= arr(i)
10    continue
      return
      end



      subroutine RESTAT (Array)
      integer array(1)
      include 'Tktrnx.fd'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
       arr(i)= array(i)
10    continue
      call movabs (kBeamX, kBeamY)
      return
      end



C
C  Relative Zeichenbefehle (wie MS Windows und DOS)
C

      subroutine MovRel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call MovAbs (iXx, iYy)
      return
      end



      subroutine PntRel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call PntAbs (iXx, iYy)
      return
      end



      subroutine DrwRel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call DrwAbs (iXx, iYy)
      return
      end



      subroutine DshRel (iX, iY, iMask)
      include 'Tktrnx.fd'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call DshAbs (iXx, iYy, iMask)
      return
      end



C
C   Ersatz SEELOC der CP/M-Version (wie MS Windows, DOS)
C

      subroutine SEELOC (IX,IY)
      include 'Tktrnx.fd'
      ix= kBeamX
      iy= kBeamY
      return
      end



C
C  Textausgabe
C

      subroutine ToutPt (iChr)
      include 'Tktrnx.fd'
      call outgtext (char(iChr))
      return
      end



      subroutine tOutSt (nChr, iChrArr)
      integer iChrArr (1)
      if (nChr.eq.0) return
      do 10 i=1,nChr
       call toutpt (iChrArr(i))
10    continue
      return
      end



      subroutine tOutStc (String)
      character *(*) String
      call outgtext (String)
      return
      end



      subroutine StatSt (String)
      character *(*) String
      call outtext (String)
      return
      end



C
C  Eingabe
C

      subroutine tinput (iChr)
      call DCURSR (iChr, iChr,iChr)
C     Aufruf von DCURSR mit ix=iy: Maustasten ausser Funktion
      return
      end



C
C>  Entry Dummyroutinen
C
      subroutine  AnMode
C> AlfMod
      entry       AlfMod
C> pClipt
      entry       pClipt
C> alpha
      entry       alpha
      return
      end
