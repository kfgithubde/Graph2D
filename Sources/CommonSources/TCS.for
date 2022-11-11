C> \file       TCS.for
C> \brief      TCS: Tektronix Plot 10 Emulation
C> \version    4.0
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C> \~german
C> Systemübergreifende TCS-Routinen
C> \~english
C> System independent subroutines
C> \~
C
C CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   Changelog   CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C      27.11.20 Version 4.0:
C               Einheitliche Version CPM/DOS/Windows/SDL2
C
C      17.08.20 Version 3.2
C               Harmonisierung der Verwendung des Commonblocks TKTRNX
C               Variable KHOMEY wird jetzt (analog alter DOS-Version) verwendet.
C               Da KHOMEY nicht in der CP/M Version vorhanden ist, muss ab dieser
C               Version fuer eine Complilation unter CP/M die entsprechende Zeile
C               in der SUBROUTINE HOME geändert werden.
C
C      13.11.17 Version 3.1
C               Anpassung an OpenWatcom 2.0
C               Bugfix: Unterscheidung Aufrufe ueber windowsx.h (win16) und GDI (win32)
C                - SelectPen -> SelectObject
C                - DeletePen -> DeleteObject
C                - DeleteBrush -> DeleteObject
C                - GetStockBrush -> GetStockObject
C                - DeleteRgn -> DeleteObject
C                - SelectFont -> SelectObject
C                - DeleteFont -> DeleteObject
C
C      27.03.13 Version 3.0
C               Anpassung an Windows 7 und OpenWatcom 1.9
C               Anpassung an gfortran anstelle von g77 der GCC
C
C      22.12.05 Version 2.19
C               Elimination berechnetes GOTO in LOGTRN
C
C      18.10.05 Version 2.18
C               Anpassung der Windowsversionen zur gemeinsamen Verwendung SDL2:
C                 TCSdrWIN.for
C                 TCSdWINc.h
C                 - Überfuehrung der Deklaration aus TCSdWIN.c nach *.h:
C                   GraphicError und CreateMainWindow_IfNecessary
C                 - Definition der Fehlernummern als Konstante statt enum
C               Abhaengigkeit Watcom-Defaultwindowsystem eliminiert
C               - TCSdWINc.c: Kein Abbruch bei OpenWatcom > 1.3 und
C                 definiertem Symbol trace_calls
C
C      26.10.04 Version 2.17
C               Bugfix Windows-System: Größe und Defaultposition des Status-
C                fensters wird bei der Erzeugung berechnet -> 1. RESTORE nach
C                Verkleinern des Graphikfensters entspricht dem vorherigen
C                Bild. 2. Angleichung des Verhaltens von 16- und 32bit Windows
C               Bei Definition des Symbols STAT_WINDOW_PRIVATE erhält das
C                Statusfenster einen privaten Devicekontext.
C               Zusammenfuehrung Initialisierung der Windows-Library und
C                Windows-DLL -> zusaetzliche Sourcefiles
C                TCSinitt.for, CreateMainWindow.c, GetMainInstance.c
C
C      23.06.04 Version 2.16:
C               Anpassungen an GNU-Compiler fuer Win32. Zusätzliches Sourcefile
C                fuer die GNU-Version: WinMain.c
C               CSIZE in Windows-Version: Korrektur Rundungsfehler
C
C      08.06.04 Version 2.15:
C               Umbenennung lib$movc3 in lib_movc3 (entsprechend ANSI-Fortran)
C               Modul STRINGS.FOR: Version 1.24
C
C      27.06.03 Version 2.14:
C               Verarbeitung Steuerzeichen in ANCHO
C
C      21.10.02 Version 2.13:
C               Einheitliche Version CPM/DOS/Windows
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Grundversion fuer C128 / Version 1.0:
C
C      Zugehoerige Module:
C              TKTRNX.FOR    Common-Block TKTRNX
C              TCSBASIC.ASM  Low-Level Routinen in Bank 0, C128 spezifisch
C              TCSDRIVR.ASM  Treiber fuer TCSBASIC
C              TCSGIN.ASM    Treiber des Gin-Cursors
C
C      20.4.88         Dr.-Ing. K. Friedewald
C                      4000 Duesseldorf 1
C                      Gerresheimerstr. 84
C
C      21.10.02 Version 2.13:
C               Vereinheitlichung CPM/DOS/Windowsversion
C               Zusätzliches Modul: TCSdrCPM.FOR: früher Teil von TCS.FOR
C               Ausschließliche Verwendung von durch grosses "C" eingeleiteten
C                Kommentaren zur Kompatibilität mit FORTRAN 4
C               Umbenennung des Includefiles in Tktrnx.fd. So kann unter CP/M
C                das als Teil des Filenamens interpretierte "'" der INCLUDE-
C                Anweisung entsprechend der 8.3 Filenamen umgesetzt werden.
C               Implementierung Unterprogramm TCSLEV
C               Bugfix: Kommentar in Tktrnx.fd wurde falsch gekennzeichnet
C                       (c statt C) -> SVSTAT und RESTAT fehlerhaft, da nicht
C                       erkannte Kommentare zusaetzliche Variablen erzeugten.
C
C      TBD: Implementierung vertikale Auflösung von 400 Pixeln
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Anpassung an DOS:
C
C      Änderungen gegenüber CP/M-Version:
C               SEELOC, DCURSR, SVSTAT, RESTAT, CSIZE in TCSdrDOS.FOR
C      Bugfix:  DASHA, DASHR - Korrektur Parameterliste
C               SEETRM - ibaud statt ibaudr
C
C      Zugehörige Module:
C               TKTRNX.FOR    Common-Block TKTRNX
C               TCSdrDOS.FOR  Bildschirmtreiber
C               TCSdDOSa.ASM  Betriebssystemspezifische Low-Level Routinen
C               HDCOPY.FOR    Hardcopyroutine
C               STRINGS.FOR   Hilfsroutinen zur Stringverarbeitung
C               OUTTEXT.FOR   nur für WATCOM-Compiler
C
C      25.10.01 Version 2.00:  Dr.-Ing. K. Friedewald
C
C      07.02.02 Version 2.10:
C               Implementierung multilinguale Fehlermeldungen
C
C      11.10.02 Version 2.12:
C               Vereinheitlichung DOS/Windowsversion
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Anpassungen an Microsoft-Windows:
C
C      Änderungen gegenüber DOS-Version:
C               INITT befinden sich jetzt in TCSdrWIN.FOR bzw. TCSinitt.FOR
C
C      Zugehörige Module:
C               TKTRNX.FOR    Common-Block TKTRNX
C               TKTRNX.h      Common-Block TKTRNX für Zugriff durch C
C               TCSdrWIN.FOR  Bildschirmtreiber
C               TCSdWINc.c    Windowspezifische API-Routinen
C               TCSdWINc.h    Compiler- und systemspezifische Deklarationen
C               STRINGS.FOR   Hilfsroutinen zur Stringverarbeitung
C
C      27.10.01 Version 2.11: Dr.-Ing. K. Friedewald
C
C      11.10.02 Version 2.12:
C               Vereinheitlichung DOS/Windowsversion
C
C
CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Anpassungen an SDL2:
C
C      Änderungen gegenüber Windows-Version:
C               Fehlerausgabe in den Windows-Debug-Channel (bzw. *ix Fehlerkanal)
C               Statusfenster analog DOS nur einzeilig ohne Scrollmöglichkeit
C
C      Zugehörige Module:
C               TKTRNX.FOR    identisch mit Windows-Version
C               TKTRNX.h      identisch mit Windows-Version
C               TCSdrSDL.FOR  SDL2-spezifische API-Routinen
C               TCSdSDLc.c    SDL2-spezifische API-Routinen
C               TCSdSDLc.h    Compiler- und systemspezifische Deklarationen
C               STRINGS.FOR   identisch mit Windows-Version
C
C      27.11.20 Version 4.00: Dr.-Ing. K. Friedewald
C


C
C Graphic Input
C

      subroutine VCURSR (IC,X,Y)
      call dcursr (ic,ix,iy)
      call revcot (ix,iy,x,y)
      return
      end

C
C  Virtuelle Graphik, relativ
C

      subroutine DRAWR (X,Y)
      call rel2ab (x,y,xabs,yabs)
      call drawa (xabs,yabs)
      return
      end



      subroutine MOVER (X,Y)
      call rel2ab (x,y,xabs,yabs)
      call movea (xabs,yabs)
      return
      end



      subroutine POINTR (X,Y)
      call rel2ab (x,y,xabs,yabs)
      call pointa (xabs,yabs)
      return
      end



      subroutine DASHR (X,Y, iL)
      call rel2ab (x,y,xabs,yabs)
      call dasha (xabs,yabs, iL)
      return
      end



      subroutine REL2AB (Xrel, Yrel, Xabs, Yabs)
      include 'Tktrnx.fd'
      call seeloc (ix,iy)
      call revcot (ix,iy,Xabs,Yabs)
      xabs= (( xrel*trcosf - yrel*trsinf)*trscal)+Xabs
      yabs= (( xrel*trsinf + yrel*trcosf)*trscal)+Yabs
      return
      end

C
C  Virtuelles Zeichnen, absolut
C

      subroutine DRAWA (X,Y)
      include 'Tktrnx.fd'
      call wincot (x,y,ix,iy)
      call swind1 (kminsx,kminsy,kmaxsx,kmaxsy)
      call drwabs (ix,iy)
      call swind1 (0,0,1023,780)
      return
      end



      subroutine MOVEA (X,Y)
      include 'Tktrnx.fd'
      call wincot (x,y,ix,iy)
      call swind1 (kminsx,kminsy,kmaxsx,kmaxsy)
      call movabs (ix,iy)
      call swind1 (0,0,1023,780)
      return
      end



      subroutine POINTA (X,Y)
      include 'Tktrnx.fd'
      call wincot (x,y,ix,iy)
      call swind1 (kminsx,kminsy,kmaxsx,kmaxsy)
      call pntabs (ix,iy)
      call swind1 (0,0,1023,780)
      return
      end



      subroutine DASHA (X,Y, iL)
      include 'Tktrnx.fd'
      call wincot (x,y,ix,iy)
      call swind1 (kminsx,kminsy,kmaxsx,kmaxsy)
      call dshabs (ix,iy, iL)
      call swind1 (0,0,1023,780)
      return
      end



      subroutine WINCOT (X,Y,IX,IY)
      include 'Tktrnx.fd'
      dx= x-tminvx
      dy= y-tminvy
      if ((xlog.lt.255.).and.(x.gt.0.)) dx= alog(x)-xlog
      if ((ylog.lt.255.).and.(y.gt.0.)) dy= alog(y)-ylog
      ix= ifix (dx*xfac+.5)+kminsx
      iy= ifix (dy*yfac+.5)+kminsy
      return
      end



      subroutine REVCOT (IX,IY,X,Y)
      include 'Tktrnx.fd'
      dx= float (ix-kminsx) / xfac
      dy= float (iy-kminsy) / yfac
      x= dx + tminvx
      y= dy + tminvy
      if (xlog.lt.255.) x= 2.718282**(dx+xlog)
      if (ylog.lt.255.) y= 2.718282**(dy+ylog)
      return
      end

C
C  Alphanumerische Ausgabe
C

      subroutine ANSTR (NChar, IStrin)
      dimension istrin(1)
      do 10 i=1,NChar
       call ancho (istrin(i))
10    continue
      return
      end



      subroutine ANCHO (ichar)
      include 'Tktrnx.fd'

      if (ichar.gt.31) goto 10
      if (ichar.eq.7) call bell
      if (ichar.eq.10) call linef
      if (ichar.eq.13) call cartn
      return

 10   call seeloc (ix,k)
      call CSize (ixlen,k)
      if (ix.gt.krmrgn-ixlen) call newlin
      call toutpt (ichar)
      return
      end



      subroutine NEWLIN
      call cartn
      call linef
      return
      end



      subroutine CARTN
      include 'Tktrnx.fd'
      call seeloc (ix,iy)
      call movabs (klmrgn,iy)
      return
      end



      subroutine LINEF
      call seeloc (j,iy)
      call CSize (j,iylen)
      if (iy.lt.iylen) call home
      call movrel (0,-iylen)
      return
      end



      subroutine BAKSP
      call csize (ix,iy)
      call movrel (-ix,0)
      return
      end



      subroutine NEWPAG
      call erase
      call home
      return
      end



      function LINHGT (Numlin)
      call Csize (ix,iy)
      linhgt= Numlin*iy
      return
      end



      function LINWDT (NumChr)
      call Csize (ix,iy)
      linwdt= NumChr*ix
      return
      end

C
C  Initialisierungsroutinen
C

      subroutine LINTRN
      include 'Tktrnx.fd'
      xlog= 255.
      ylog= 255.
      call rescal
      return
      end



      subroutine LOGTRN (IMODE)
      include 'Tktrnx.fd'
      call lintrn
      if ((imode .eq. 1) .or. (imode .eq. 3)) then
       xlog= 0.
      end if
      if ((imode .eq. 2) .or. (imode .eq. 3)) then
       ylog= 0.
      end if
      call rescal
      return
      end



      subroutine TWINDO (IX1,IX2,IY1,IY2)
      call swindo (ix1,ix2-ix1,iy1,iy2-iy1)
      return
      end



      subroutine SWINDO (IX,LX,IY,LY)
      include 'Tktrnx.fd'
      kminsx= ix
      kmaxsx= ix+lx
      kminsy= iy
      kmaxsy= iy+ly
      call rescal
      return
      end



      subroutine DWINDO (X1,X2,Y1,Y2)
      call vwindo (x1,x2-x1,y1,y2-y1)
      return
      end



      subroutine VWINDO (X,XL,Y,YL)
      include 'Tktrnx.fd'
      tminvx= x
      tmaxvx= x+xl
      tminvy= y
      tmaxvy= y+yl
      call rescal
      return
      end



      subroutine RESCAL
      include 'Tktrnx.fd'
      xfac= 0.
      yfac= 0.
      if ((tmaxvx.eq.tminvx) .or. (tmaxvy.eq.tminvy)) return
      dx= tmaxvx-tminvx
      dy= tmaxvy-tminvy
      if ((xlog.eq.255.).or.(amin1(tminvx,tmaxvx).le.0.)) goto 10
       xlog= alog(tminvx)
       dx= alog(tmaxvx)-xlog
10    if ((ylog.eq.255.).or.(amin1(tminvy,tmaxvy).le.0.)) goto 20
       ylog= alog(tminvy)
       dy= alog(tmaxvy)-ylog
20    xfac= float (kmaxsx-kminsx) / dx
      yfac= float (kmaxsy-kminsy) / dy
      return
      end



      subroutine RROTAT (Grad)
      include 'Tktrnx.fd'
      trsinf= sin(Grad/57.29578)
      trcosf= cos(Grad/57.29578)
      return
      end



      subroutine RSCALE (Faktor)
      include 'Tktrnx.fd'
      trscal= Faktor
      return
      end



      subroutine HOME
      include 'Tktrnx.fd'
C      call movabs(klmrgn,750) Fuer CP/M (kein khomey verfuegbar, -> !=750)
      call movabs(klmrgn,khomey)
      return
      end



      subroutine SETMRG (Mlinks, Mrecht)
      include 'Tktrnx.fd'
      klmrgn= Mlinks
      krmrgn= Mrecht
      return
      end



      subroutine SEETRM (IBaud,Iterm,ICSize,MaxScr)
      include 'Tktrnx.fd'
      ibaud= 0
      iterm= 1
      icsize= 1
      MaxScr= 1023
      return
      end



      subroutine SEETRN (xf,yf,key)
      include 'Tktrnx.fd'
      xf= xfac
      yf= yfac
      key= 1
      if ((xlog.lt.255.).or.(ylog.lt.255.)) key=2
      return
      end



      logical function GENFLG (ITEM)
      genflg= item.eq.0
      return
      end
	  