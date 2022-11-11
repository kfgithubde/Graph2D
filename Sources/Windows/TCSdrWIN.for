C> \file       TCSdrWIN.for
C> \brief      MS Windows Port: High-Level Driver
C> \version    (2022, 88,x)
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> MS Windows-spezifische TCS-Routinen
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
C> MS Windows specific subroutines
C> \note \verbatim
C>    Supplement to Tektronix:
C>     subroutine TOUTSTC (String): Print Fortran-String
C>     subroutine LINCOL (iCol): Set line color (iCol=0..15)
C>     subroutine TXTCOL (iCol): Set text color
C>     subroutine BCKCOL (iCol): Set background color (shows after ERASE)
C>     subroutine DefaultColour: Reset default colors
C> \endverbatim
C> \~
C>
C
C
C  TCS Graphik Grundfunktionen für Windows
C
C     Version 1.95 bzw. (2022,88,x)
C     - Anpassung 64bit Windows 10 und kleinere Bugfixes
C
C     Version 1.94 bzw. (2021,123,x)
C     - Ergaenzung englische Dokumentation
C
C     Version 1.93 bzw. (2020,332,x)
C     - Fehlerbehandlung analog SDL-Version
C
C     Version 1.92 bzw. (2020,230,x)
C     - Harmonisierung Commonblock TKTRNX
C     - Verwendung von khorsz, kversz, khomey in Abhängigkeit vom Zeichensatz
C
C     Version 1.91 bzw. (2017,317,x)
C     - Bugfix
C
C     Version 1.9
C     - Anpassung Windows7
C
C     Version 1.8 bzw. (2008,134,x)
C     - Hardcopy fuer Journal=3 in Form von Postscriptfiles. TBD.
C     - Ergaenzung Journal=3: Implementation Schriftarten.
C     - DRWABS bei Journal=3: Der Endpunkt wird erst beim Neuzeichnen ge-
C       setzt, im Journal steht nur die Linie mit Endpunkt. Vorteil: UNIX
C       muss den Endpunkt so nicht zweimal setzen.
C     - Fehlermeldungen der Listenverwaltung fuer Journal=3 erfolgen durch
C       GraphError bzw. Unterprogramm TCSJouListError.
C     - Bugfix TCSdWINc.h: Eintrag von TCSLEV3 in C++ Klassendefinition.
C     - Bugfix OUTGTEXT: Prüfung auf freien Platz erfolgt mit gesamtem String.
C
C     Version 1.7 bzw. (2005,291,x)
C     - Einfuehrung des Windows-unabhaengigen Journals zur Vorbereitung
C       der X11-Version. Wahl des Journaltyps (Metafile oder Liste) durch
C       bedingte Kompilation, gesteuert von der Konstante JOURNALTYP
C       im File TCSdWINc.c
C     - Bugfix GraphicError: ErrSeverity=0 entspricht jetzt NO ACTION.
C     - Das System wird nicht mehr durch Fortran-Pragmas in TCSLEV, sondern
C       durch das neue Unterprogramm TCSLEV3 in TCSdWINc.c ermittelt.
C
C     Version 1.6 bzw. (2004,302,x)
C     - Auslagern der Subroutine INITT in ein eigenes File. So kann sicher-
C       gestellt werden, dass sich INITT stets im *.exe des Hauptprogrammes
C       und nicht in einer DLL befindet und eine Ermittlung der Programm-
C       instanz und nicht der DLL-Instanz erfolgt.
C     - Sources der LIB- und DLL-Version zusammengefasst
C
C     Version 1.5 bzw. (2004,167,x)
C     - Anpassung TCSLEV: 5= Alternative Win32-Version für GCC
C
C     Version 1.4 bzw. (2004, 22,x)
C     - Bugfix OUTGTEXT: Bei c-Strings auch char(0) als Stringende erkennen
C     - Bugfix INITT1: Wiederherstellung Charakterdefinitionsblock nach
C       Erzeugung des Statusfensterfonts -> Buchstabengroesse bei ITALIC,
C       ITALIR, DBLSIZ, NRMSIZ wird jetzt richtig gesetzt.
C     - Verschieben und Scrollen Statusfenster auch bei Eingabe möglich
C
C     Version 1.3 bzw. (2003, 78,x)
C     - Falls die eigene Applikation in einem anderen Fenster aktiv ist, setzt
C       TINPUT den Fokus wieder in dieses Fenster zurück
C     - Icon für das Graphikfenster
C     - Instanzermittlung ueber Programmnamen fuer die DLL-Version
C
C     Version 1.2 bzw. (2003, 36,x)
C     - Ergänzung lib$movc3 zur Kompatibilität DOS
C     - Verwirrendes Bildschirmverhalten bei sehr langsamen Rechnern nach Erase
C       -> Einfügen UpdateWindow
C
C     Version 1.1 bzw. (2002,292,x)
C     - Umbenennung TKTRNX.FOR in TKTRNX.FD zur Kompatibilität CP/M
C
C     Version 1.0
C     - Erweiterungen gegenüber Tektronix:
C           subroutine TOUTSTC (String): Ausgabe Fortran-String
C           subroutine STATST (String) : Ausgabe String in Statusfenster
C           subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15)
C           subroutine TXTCOL (iCol): Setzen Textfarbe
C           subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar)
C           subroutine DefaultColour: Wiederherstellung Defaultfarben
C
C
C     27.09.02         Dr.-Ing. K. Friedewald
C



C
C  Ausgabe der Softwareversion
C
      subroutine TCSLEV(LEVEL)
      integer LEVEL(3)
      LEVEL(1)=2022     ! Aenderungsjahr
      LEVEL(2)=  88     ! Aenderungstag
C Kennzeichnung des Systems, wird im systemabhaengigem Code gesetzt
C     3=Watcom && MS-Win16  4=Watcom && MS-Win32  5=GNU-Win32  7=GNU-Win64
      call TCSLEV3 (LEVEL(3))

      return
      end



C
C  Abspeichern Terminal Status Area (wie DOS)
C

      subroutine SVSTAT (Array)
      integer array(1)
      include 'TKTRNX.FD'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
       array(i)= arr(i)
10    continue
      return
      end



      subroutine RESTAT (Array)
      integer array(1)
      include 'TKTRNX.FD'
      integer arr(1)
      equivalence (arr(1),khomey)
      do 10 i=1,iTktrnxL
       arr(i)= array(i)
10    continue
      call movabs (kBeamX, kBeamY)
      return
      end



C
C  Relative Zeichenbefehle (wie DOS)
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
C   Ersatz SEELOC der CP/M-Version, SEELOC1 unnötig (wie DOS)
C

      subroutine SEELOC (IX,IY)
      include 'TKTRNX.FD'
      ix= kBeamX
      iy= kBeamY
      return
      end



C
C  Textausgabe, geändert zu DOS-Version
C



      subroutine ToutPt (iChr)
      include 'TKTRNX.FD'
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
C  Dummyroutinen (WINLBL keine Dummyroutine, ALPHA zusätzlich)
C

      subroutine    AnMode
      entry         AlfMod
      entry         pClipt
      entry         ioWait
      entry         alpha
      return
      end
