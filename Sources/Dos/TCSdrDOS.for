C> \file       TCSdrDOS.for
C> \brief      DOS Port: High-Level Driver
C> \version    (2005, 45,2)
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> \note \verbatim
C>    Erweiterungen gegenüber Tektronix:
C>     subroutine TOUTSTC (String): Ausgabe Fortran-String
C>     subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15) 
C>     subroutine TXTCOL (iCol): Setzen Textfarbe 
C>     subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar) 
C>     subroutine DefaultColour: Wiederherstellung Defaultfarben 
C> \endverbatim
C> \~english
C> \note \verbatim
C>    Extensions of the Tektronix TCS:
C>     subroutine TOUTSTC (String): Output Fortran-String
C>     subroutine LINCOL (iCol): Set line color (iCol=0..15) 
C>     subroutine TXTCOL (iCol): Set text color 
C>     subroutine BCKCOL (iCol): Set background color (visible after ERASE) 
C>     subroutine DefaultColour: Reset default colors
C> \endverbatim
C> \~
C>
C
C CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   Changelog   CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C     07.02.02 Version 1.2:
C           Implementierung multilinguale Fehlermeldungen
C
C     24.05.02 Version 1.3:
C           Umgebungsvariablen werten auch mit ";" getrennte Pfade aus
C           INCLUDE Interface TCSDOSA.FI zur Anpassung an den WATCOM-Compiler
C           TKTRNX.for: geänderte Common-Blocklänge aufgrund INTEGER*4
C                       bei WATCOM (MS: INTEGER*2)
C           DSHABS:     Ersatz Hex-Konstante durch Dezimalkonstante zur
C                       Erzielung Kompatibilität mit WATCOM-Compiler
C           INITT1:     Anpassung WATCOM-Compiler:
C                       - Apostrophe innerhalb von Strings durch 2 Apostrophe
C                       - Strings muessen mit CHAR(0) abgeschlossen werden
C                       BugFix: SETVIEWPORT erwartet INTEGER*2 statt REAL*4!
C           TOUTPT:     Anpassung WATCOM: auszugebender Character mit CHAR (0)
C           GraphicError: Format 900 ist bei den *.lng-Files streng zu be-
C                       folgen, d.h. "_12,Text" . Ausgabe OUTTEXT mit char(0).
C
C     23.07.02 Version 1.31:
C           Erweiterung: subroutine WINLBL (GraphicWinLbl, StatusWindLbl):
C                       Kompatibilität zu Windowsversion.
C           Eigenes Modul OUTTEXT zur Kompatibilität MS-WATCOM
C                       (Watcom benötigt char(0), dann MS Zeilenüberlauf)
C
C     24.08.02 Version 1.32:
C           ITALIC, ITALIR, DBLSIZ, NRMSIZ: Schriftarten Graphikausgabe.
C                       Kombination groß/kursiv nicht vorgesehen.
C                       Implementiert durch Fontfile GraphLib.FON
C                       (Quelle: Programm SOFTY und Arial Terminal TTF-Basis)
C           DSHABS:     Standardisierung Dash-Linestyles DOS-Windows:
C                       0: solid, 1: dotted, 2: dash-dotted, 3:dashed
C           DEFAULTCOLOUR: Bugfix Namensgebung, nicht DEFAULTCOLOURS
C
C     10.10.02 Version 1.33:
C           INITT:      Zur Vereinheitlichung DOS/Windows jetzt in diesem File
C           TCSLEV:     neu, zur Angleichung mit AG2LEV und Systemerkennung
C
C     19.10.02 Version 1.34 bzw. (2002,292,2)
C           Umbenennung TKTRNX.FOR in TKTRNX.FD zur Kompatibilität CP/M
C
C     25.10.02 Version (2002,298,2)
C           Entprellen Mousetaste bei GIN-Cursoreingabe
C
C     06.02.03 Version (2003, 37,2)
C           Vereinheitlichtes Interface lib$movc3 (Kompatibilitaet Windows)
C
C     12.01.04 Version (2004, 12,2)
C           INITT1:     Bugfix Endlosschleife bei fehlerhaftes Fontfile und
C                       Severity 5
C           GRAPHICERRORINIT: Defaultseverity 10 bei EXIT (FINITT, iErr=12)
C           Anmerkung: Die Subroutine GRAPHICERROR ruft sich bei Programm-
C                 abbruch über FINITT implizit selber rekursiv auf (nicht
C                 FORTRAN-konform!). Da jedoch keine lokalen Variablen ver-
C                 wendet werden, ist dies in der Regeln nicht kritisch.
C
C     25.10.04 Version (2004,299,2)
C           WINLBL:     Wertet jetzt den 3. Parameter (Initilisierungsfile)
C                       analog zur Windowsversion aus (einschliesslich Ueber-
C                       setzung '%:' und '.%'
C           LIB$MOVC3:  Umbenannt in LIB_MOVC3. Alte Assemblerroutine heisst
C                       jetzt LIB_MOVC3_.
C
C     15.02.05 Version (2005, 45,2)
C           GRAPHICERROR: Bugfix ErrSeverity=0 entspricht jetzt NO ACTION.
C

      include 'FGRAPH.FI'
      include 'TCSdDOSa.FI'



C
C  Ausgabe der Softwareversion
C
      subroutine TCSLEV(LEVEL)
      integer LEVEL(3)
      LEVEL(1)=2005     ! Aenderungsjahr
      LEVEL(2)=  45     ! Aenderungstag
      LEVEL(3)=   2     ! System= DOS

      return
      end



C
C  Bildschirm Verwaltung
C



      subroutine INITT (iDummy)
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



      subroutine initt1
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer*2 iErr, iAvail, iButton, kScrX2, kScrY2
      integer iLen, iTrimLen, iParse

      character*80 cBuf, cBuf1*80
      record /VideoConfig/ MyScreen
      record /FontInfo/ MyFont

      character *13 cFontFile           ! Graphikfontfile
      parameter (cFontFile='GRAPHLIB.FON'//char(0))

      character*5 cEnv                  ! Logischer Name für den Fontfilepfad
      parameter (cEnv='LIB='//char(0))

      call GraphicErrorInit

      iErr= SetVideoMode ($MAXRESMODE)

      if (iErr .eq. 0) then
       call GraphicError (2,' ') ! TCS-Initt: unknown graphic adapter
      end if

      call GetVideoConfig (MyScreen)
      kScrX= MyScreen.NumXPixels-1
      kScrY= MyScreen.NumYPixels-1-
     1   (MyScreen.NumYPixels/MyScreen.NumTextRows)      ! Höhe Statuszeile

      call SetViewPort (0,0, kScrX, kScrY)

      call SetTextWindow (MyScreen.NumTextRows,1,MyScreen.NumTextRows,
     1  MyScreen.NumTextCols)                            ! Statuszeile
      kStCol= MyScreen.NumTextCols - 1 ! Verhindere Scrollen durch -1

      if (RegisterFonts (cFontFile).lt.0) then
       cBuf= cEnv                              ! Abfrage Enviroment
       call GetEnv (cBuf, len(cBuf))
       iLenPath= iTrimLen (cBuf)
       iParse=1
   10  continue ! while
        if (iParse.le.iLenPath) then
         iLen= index (cBuf(iParse:iLenPath), ';')-1
         if (iLen.le.0) iLen=iLenPath-iParse+1
        else
         iLen= -1
        end if
        if ((iLen.lt.1).or.(iParse.gt.iLenPath)) then
         cBuf1= cEnv    ! Notwendig zur Bildung des Substrings aus PARAMETER
         cBuf1=cBuf1(1:iStringLen(cBuf1))//':'//cFontFile
         call GraphicError (3,cBuf1(1:iStringLen(cBuf1))) !openerror fontfile
         goto 15 ! ENDWHILE falls Errorseverity(3) < 10 (STOP)
        else
         cBuf1= cBuf(iParse:iParse+iLen-1)//'\'//cFontFile ! Chr0 in cFontFile
         call substitute (cBuf1,cBuf1, '\\', '\') ! kein doppelter Backslash!
        end if
       if (RegisterFonts (cBuf1(1:iStringLen(cBuf1))).lt.0) then ! end while
        if (iLen.lt.iLenPath) then
         iParse= iParse+iLen+1
         goto 10        ! nächster Eintrag im Pfad
        else
         call GraphicError (3,cBuf1(1:iStringLen(cBuf1)))
        end if
   15  end if
      end if

      call NRMSIZ               ! Standardschrift: normalgroß, nicht kursiv

      kScrX2= kScrX             ! Konvertierung in int*2 durch WATCOM-Compiler
      kScrY2= kScrY
      call GinCrsIn (iAvail, iButton, 0, kScrX2, 0, kScrY2)
      if (iAvail.eq.-1) then
       iMouse= iButton
      else
       iMouse= 0
      end if
      call DefaultColour
      call Erase

      return
      end



      subroutine ITALIC
C
C Verändern des Graphik-Fonts
C
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer*2 iErr
      record /FontInfo/ MyFont

      iErr= SetFont ('t''Italic'''//char(0))
      goto 10

      entry DBLSIZ
      ierr= SetFont ('t''Double'''//char(0))
      goto 10

      entry ITALIR
      entry NRMSIZ
      iErr= SetFont ('t''Normal'''//char(0))

 10   continue          ! identischer Code für ITALIC und ITALIR
      if (iErr.lt.0) then
       call GraphicError (4,'Normal/Italic/Double') ! TCS-Initt: unknown font
      end if
      iErr= getfontinfo(MyFont)
      khorsz= iSign (iRevScreenXCoord(int(MyFont.pixwidth))
     1  - iRevScreenXCoord(0),1)
      kversz= iSign (iRevScreenYCoord(int(MyFont.pixheight))
     1  - iRevScreenYCoord(0),1)
      kHomeY= 780-(1.1*kVersZ)
      return
      end



      subroutine GraphicErrorInit
C
C  SUBROUTINE GraphicErrorInit, ENTRIES WinLbl, GraphicError
C  Internationalisierung der Fehlermeldungen
C
      implicit none
      include 'FGRAPH.FD'
      save ErrSeverity, ErrMsg, FilNam
      
      integer MaxErr
      parameter (MaxErr=12)
      character *(*) Mssg
      character *(*) WinLblDummy, StatLblDummy, MessageFile
      integer iErr, i, iTrimLen,iStringLen, iErrSev
      integer iLenPath, iParse, iLen

      character*132 cEnv, FilNam, cBuf
      integer ErrSeverity (MaxErr)
      character*80 ErrMsg (MaxErr)
      data cEnv,FilNam /'LIB=','GRAPHLIB.LNG'/
      data ErrMsg/'GRAPHLIB %%% INITT: Incompatible message file - Press
     1 any key',
     2      'GRAPHLIB %%% INIT: Unknown graphic adapter',
     3      'GRAPHLIB %%% INIT: Error opening fontfile $$',
     4      'GRAPHLIB %%% INIT: Unknown font $$',
     5      'GRAPHLIB %%% INPUT: No mousedriver available, use keyboard'
     6     ,'GRAPHLIB %%% HARDCOPY: Error during OPEN',
     7      'GRAPHLIB %%% HARDCOPY: Error during WRITE',
     8      'GRAPHLIB %%% HARDCOPY: Internal error (buffer overflow)',
     9      '$$','Hardcopy in progress','Press any key to continue',
     2      'Press any key to exit program'/

      data ErrSeverity /5,10,10,10, 1, 5, 5, 5, 1, 1, 5, 10/
      
      external iGetArg           ! Watcom Library-Funktion
      integer iGetArg

      cEnv=cEnv(1:iTrimLen(cEnv))//char(0)
      FilNam= FilNam(1:iTrimLen(FilNam))//char(0)

C
C  1.Priorität: Message-File durch WinLbl spezifiziert
C  2.Priorotät: GRAPHLIB.LNG im Arbeitsdirectory
C

      open (unit=9,form='FORMATTED', err=5, status='OLD', File=
     1                        FilNam(1:iStringLen(FilNam)))
      goto 7      ! File gefunden -> Einlesen

C
C  3.Priorität: Message-File GRAPHLIB.LNG in LIB:
C

5     call GetEnv (cEnv, Len(cEnv))
      iLenPath= iTrimLen (cEnv)
      iParse=1
   10 continue ! while
       if (iParse.le.iLenPath) then
        iLen= index (cEnv(iParse:iLenPath), ';')-1
        if (iLen.le.0) iLen=iLenPath-iParse+1
       else
        goto 99         ! benutze Default
       end if
       if ((iLen.ge.1).and.(iParse.le.iLenPath)) then
        cBuf= cEnv(iParse:iParse+iLen-1)//'\'//FilNam ! Chr0 bereits in FilNam
        call substitute (cBuf,cBuf, '\\', '\') ! kein doppelter Backslash !
       end if
       open (unit=9,form='FORMATTED', err=6, status='OLD', File=
     1                        cBuf(1:iStringLen(cBuf)))
      goto 7      ! File gefunden -> Einlesen
6     if (iLen.lt.iLenPath) then ! end while
       iParse= iParse+iLen+1
       goto 10  ! nächster Eintrag im Pfad
      else
       goto 99 ! kein File vorhanden - > benutze Default
      end if

7     do 20 i=1,MaxErr
       read (unit=9, err=90, fmt=900) ErrSeverity(i),ErrMsg (i)
20    continue

      close (unit=9)

99    return
C
C Ausgabe Fehlermeldung Messagefile
C
90    call outtext (ErrMsg(1)) ! Graphiksystem wurde noch nicht initialisiert!
      call tinput (i)
      return



      entry WinLbl (WinLblDummy, StatLblDummy, MessageFile)
C
C  Setzen des Messagefiles und Uebersetzung '%:' bzw. '.%'
C
      if (iStringLen(Messagefile).le.0) return
      FilNam= MessageFile
      i= iGetArg (0, cBuf) ! Arg. 0: Programmname mit Directory
      if (i.gt.1) then
   30  continue ! repeat
        i= i-1
       if ((cBuf(i:i).ne.'\').and.(i.gt.1)) goto 30
       cBuf(i+1:i+1)= char(0)
       call substitute (FilNam, Filnam,'%:',cBuf)
      end if
      call substitute (FilNam, FilNam,'.%','.lng')
      return
      
      
      
      entry GraphicError (iErr,Mssg)
C
C  Ausgabe der Fehlermeldung
C
      if (iErr.eq.99) then             ! Programmabbruch aus FINITT (2. Aufruf)
       if (ErrSeverity(12).eq.10) then
        iErrSev= 99                             ! STOP
       else if (ErrSeverity(12).eq.5) then
        iErrSev= 1                              ! TINPUT bereits durchgeführt
       else
        iErrSev= ErrSeverity(12)
       end if
      else
       iErrSev= ErrSeverity(iErr)
       if (iErrSev.gt.0) then
        call bell
        call Substitute (ErrMsg(iErr),cBuf, '$$', Mssg)
        call StatSt (cBuf)
       end if
      end if

      if (iErrSev.le.1) then                ! =1: Statusmeldung
       return
      else if (iErrSev.eq.99) then
       stop                                 ! =99: aus FINITT
      else
       call tinput (i)
       if (iErrSev.eq.5) then               ! =5: Warnung
        return
       else if (iErrSev.eq.10) then         ! =10: Abbruch
        if (iErr.ne.12) call finitt ()      ! Rekursion iErr=12 verhindern
       end if
      end if

      return
900   format (1x,i2,1x,a)
      end



      subroutine LinCol (iCol)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer iColCode, iCol
      integer *2 iErr
      iLinCol= iColCode (iCol)
      iErr= setcolor (iLinCol)
      return
      end



      subroutine TxtCol (iCol)
      include 'TKTRNX.FD'
      integer iColCode, iCol
      iTxtCol= iColCode (iCol)
      return
      end



      subroutine BckCol (iCol)
      include 'TKTRNX.FD'
      integer iColCode, iCol
      iBckCol= iColCode (iCol)
      return
      end



      Subroutine DefaultColour
      call BckCol (0)
      call LinCol (1)
      call TxtCol (1)
      return
      end


      integer function iColCode (iCol)
      include 'FGRAPH.FD'
      integer iColTab (15)    ! Anpassung Farbindex an VGA-Palette
      data iColTab/  15       ,12         ,10         ,11         ,9
C        iCol=       1        2           3           4           5
C        entspricht: weiss    rot         gruen       blau        lila
     1               ,14      ,7          ,13         ,4          ,2
C        iCol=       6        7           8           9           10
C        entspricht: gelb     grau        violett     mattrot     mattgruen
     2               ,1       ,3          ,6          ,8          ,5/
C        iCol=       11       12          13          14          15
C        entspricht: mattblau mattlila    orange      mattgrau    mattviolett
      if (iCol.le.0) then
       iColCode= 0
      else if (iCol.gt.15) then
       iColCode= iColTab (1)
      else
       iColCode= iColTab(iCol)
      end if
      return
      end



      integer function iScreenXCoord (iX)
      include 'TKTRNX.FD'
      iScreenXCoord= (iX*kScrX)/1023
      return
      end



      integer function iScreenYCoord (iY)
      include 'TKTRNX.FD'
      iScreenYCoord= kScrY-(kScrY*iY)/780
      return
      end



      integer function iRevScreenXCoord (iX)
      include 'TKTRNX.FD'
      iRevScreenXCoord= (iX*1023)/kScrX
      return
      end



      integer function iRevScreenYCoord (iY)
      include 'TKTRNX.FD'
      iRevScreenYCoord= 780-(780*iY)/kScrY
      return
      end



      subroutine Erase
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      call ClearScreen ($GCLEARSCREEN)
      iErr= setcolor (iBckCol)
      iErr= rectangle( $GFILLINTERIOR, 0, 0, kScrX, kScrY)
      iErr= setcolor (iLinCol)
      call MovAbs (kBeamX, kBeamY)      ! Cursorposition wiederherstellen
      return
      end



      subroutine finitt
      implicit none
      include 'FGRAPH.FD'
      integer*2 iErr
      call GraphicError (12,' ')        ! Press any key to exit program
      call unregisterfonts ()
      iErr= SetVideoMode ($Defaultmode)
      call GinCrsEx
      call GraphicError (99,' ')        ! Jetzt auch STOP möglich
      return
      end

C
C  Abspeichern Terminal Status Area
C

      subroutine SVSTAT (Array)
      integer array(1)
      include 'TKTRNX.FD'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
10     array(i)= arr(i)
      return
      end



      subroutine RESTAT (Array)
      integer array(1)
      include 'TKTRNX.FD'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
10     arr(i)= array(i)
      call movabs (kBeamX, kBeamY)
      return
      end


C
C  Absolute Zeichenbefehle
C

      subroutine movabs (ix,iy)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      record /xycoord/ oldxy
      integer iScreenXcoord, iScreenYcoord
      call moveto (iScreenXcoord(ix),iScreenYcoord(iy), oldxy)
      kBeamX= ix
      kBeamY= iY
      return
      end



      subroutine pntabs (ix,iy)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer iScreenXcoord, iScreenYcoord
      integer oldPixel,ixs,iys
      record /xycoord/ oldxy
      ixs= iScreenXCoord (iX)
      iys= iScreenYCoord (iY)
      call moveto (iXs,iYs, oldxy)
      oldPixel= SetPixel (iXs,iYs)
      kBeamX= ix
      kBeamY= iY
      return
      end



      subroutine drwabs (ix,iy)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer iScreenXcoord, iScreenYcoord
      iErr= Lineto (iScreenXCoord (ix), iScreenYCoord (iY))
      kBeamX= ix
      kBeamY= iY
      return
      end



      subroutine dshabs (ix,iy, iMask)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      integer iScreenXcoord, iScreenYcoord
      integer*2 iErr
      if (iMask.eq.0) then      ! solid line
       iMask= 65535             ! 1111 1111 1111 1111
      else if (iMask.eq.1) then ! dotted line
       iMask= 43690             ! 1010 1010 1010 1010
      else if (iMask.eq.2) then ! dash-dotted line
       iMask= 58596             ! 1110 0100 1110 0100
      else if (iMask.eq.3) then ! dashed line
       iMask= 61680             ! 1111 0000 1111 0000
      end if
      call SetLineStyle (iMask)
      iErr= Lineto (iScreenXCoord (ix), iScreenYCoord (iY))
      call SetLineStyle (65535) ! =#ffff, so zu WATCOM-Compiler kompatibel
      kBeamX= ix
      kBeamY= iY
      return
      end

C
C  Relative Zeichenbefehle
C

      subroutine MovRel (iX, iY)
      include 'TKTRNX.FD'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call MovAbs (iXx, iYy)
      return
      end



      subroutine PntRel (iX, iY)
      include 'TKTRNX.FD'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call PntAbs (iXx, iYy)
      return
      end



      subroutine DrwRel (iX, iY)
      include 'TKTRNX.FD'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call DrwAbs (iXx, iYy)
      return
      end



      subroutine DshRel (iX, iY, iMask)
      include 'TKTRNX.FD'
      ixx= kBeamX + iX
      iyy= kBeamY + iY
      call DshAbs (iXx, iYy, iMask)
      return
      end

C
C   Ersatz SEELOC der CP/M-Version, SEELOC1 unnötig
C

      subroutine SEELOC (IX,IY)
      include 'TKTRNX.FD'
      ix= kBeamX
      iy= kBeamY
      return
      end



      Subroutine Swind1 (ix1,iy1, ix2,iy2)
      include 'FGRAPH.FD'
      integer iScreenXcoord, iScreenYcoord
      call setcliprgn (iScreenXcoord(ix1),iScreenYcoord(iy1),
     1                     iScreenXcoord(ix2),iScreenYcoord(iy2))
      return
      end



      Subroutine alpha
      implicit none
      include 'FGRAPH.FD'
      integer*2 iErr
      iErr= SetVideoMode ($Defaultmode)
      return
      end

C
C  Textausgabe
C

      subroutine CSIZE (Ixlen,iylen)
      include 'TKTRNX.FD'
      Ixlen= khorsz
      Iylen= kversz
      return
      end



      subroutine ToutPt (iChr)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      record /xycoord/ oldxy
      integer iScreenXcoord, iScreenYcoord
      integer*2 iErr
      call moveto (iScreenXcoord(kBeamX),iScreenYcoord(kBeamY+kVerSz)
     1      , oldxy)
      iErr= setcolor (iTxtCol)
      call outgtext (char(iChr)//char(0))
      iErr= setcolor (iLinCol)
      kBeamX= kBeamX+kHorSz
      call moveto (iScreenXcoord(kBeamX), iScreenYcoord(kBeamY), oldxy)
      return
      end



      subroutine tOutSt (nChr, iChrArr)
      integer iChrArr (1)
      if (nChr.eq.0) return
      do 10 i=1,nChr
10     call toutpt (iChrArr(i))
      return
      end



      subroutine tOutStc (String)
      character *(*) String
      do 10 i=1,iStringLen(String)
 10    call tOutPt (iChar(String(i:i)))
      return
      end



      subroutine StatSt (String)
      include 'FGRAPH.FD'
      include 'TKTRNX.FD'
      record /rccoord/ s
      character *(*) String
      character *80 Buf
      Buf= String(1:iStringLen(String)) ! Mit Blanks auf 80 Zeichen aufgefüllt
      call SetTextPosition (1,1,s)
      call outtext (Buf(1:Min(80,kStCol)))
      return
      end

C
C  Eingabe
C

      subroutine tinput (iChr)
      integer *2 kTinput
      iChr= kTinput ()  ! Konversion Integer*2 nach *4 durch Compiler
      return
      end


      subroutine DCURSR (IC,IX,IY)
      include 'TKTRNX.FD'
      integer ic, ix, iy
      integer*2 ic2, ix2, iy2
      if (iMouse.ne.0) then
       call gincrs (ic2,ix2,iy2)
       ix= ix2                    ! Watcom: Konvertierung int*2 in int*4
       iy= iy2
       ic= ic2
      else
       call GraphicError (5, ' ') ! No Mousedriver available, use Keyboard
       call tinput (ic)
       ix= 0
       iy= 0
      end if
      ix= iRevScreenXCoord (iX)
      iy= iRevScreenYCoord (iY)
      return
      end

C
C  Interface lib$movc3 (Anpassung Parameterübergabe durch "TcsDDosA.FI"
C
      subroutine lib_movc3 (iLen, sou, dst)
      integer iLen
      character *(*) sou,dst
      call lib_movc3_ (iLen, sou, dst)
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
C> ioWait
      entry       ioWait
      return
      end


	  
      logical function WINselect (iDummy)
	  WINselect= .false.
	  return
	  end
