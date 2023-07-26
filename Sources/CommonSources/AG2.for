C> \file       AG2.for
C> \brief      Graph2D: Tektronix Advanced Graphing II Emulation
C> \version    (2023,135, x)
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C>  Schicht 2: Unterprogramme zur Erzeugung wissenschaftlicher 2-D Graphiken
C> \note
C>     Die Sonderzeichen Hochindex (alt: -1) und Index (alt: -2) sind jetzt
C>     SOH=char(1) (Hochindex) bzw. STX=char(2) (Index).
C>
C> \~english
C> Layer 2: scientific 2-D graphic subroutines
C> \note
C>     The control character for exponent (originally -1) is now SOH=char(1)
C>     and for index (originally -2) STX=char(2).
C>
C> \~
C> \note \verbatim
C>   Package:
C>    - AG2.for:         chart plotting routines
C>    - AG2Holerith.for: deprecated routines
C>    - AG2USR.for:      default userroutines
C>    - G2dAG2.fd:       commonblock
C> \endverbatim
C
C
C  Tektronix Advanced Graphics 2 - Version 2.x
C
C
C     Neuer Code in Fortran 77. Die Verwendung der im Manual dokumentierten
C     Unterprogramme bleibt unveraendert, die direkte Manipulation von
C     Variablen des zugrundeliegenden Commonblockes ist jedoch nicht mehr
C     empfehlenswert. IBASEX (iPar) und IBASEY(iPar) mit ipar <>0,
C     IBASEC, COMGET und COMSET sollten in neuen Programmen nicht verwendet
C     werden.
C
C     Die Zwischenspeicherung der Statusvariablen ueber
C           SAVCOM und RESCOM
C     und die Achsensteuerung ueber
C           IBASEX(0), IBASEY(0) und IOTHER
C     werden weiterhin unterstuetzt.
C
C     Die Implementation der Unterprogramme COMGET und COMSET setzt die gleiche
C     Laenge von REAL und INTEGER-Variablen voraus.
C
C     Da Holerithvariablen von modernen Compilern uneinheitlich unterstuetzt
C     werden (4Habcd entweder als gepackte Integervariable oder als Character-
C     variable interpretiert), wurden die folgenden Routinen angepasst:
C      - subroutine PLACE (Lit): Lit wird nur noch als Ordnungszahl (1..13)
C        und nicht mehr alternativ als Literal ('STD', 'UPH') interpretiert.
C
C     subroutine LEAP (iyear): Die Schaltjahrkorrektur erfolgt nicht mehr
C     als SUBROUTINE ueber einen Common-Block, sondern direkt als
C     integer function LEAP (iyear) ! = 1: Schaltjahr, sonst 0
C
C     Die Sonderzeichen Hochindex (alt: -1) und Index (alt: -2) sind jetzt
C     SOH=char(1) (Hochindex) bzw. STX=char(2) (Index).
C
C     Intern erfolgt die Stringverarbeitung ueber Charactervariablen als
C     nullterminierte C-Strings.
C
C     Der User-API wurden die folgenden Unterprogramme als Charactervarianten
C     der Original-Holerithroutinen hinzugefuegt:
C      - subroutine NUMSETC (fnum,nbase, outstr,fillstr)
C      - subroutine FONLYC (fnum,iwidth,idec, outstr,fillstr)
C      - subroutine EFORMC (fnum,iwidth,idec, outstr,fillstr)
C      - subroutine EXPOUTC (nbase,iexp, outstr,fillstr)
C      - subroutine ALFSETC (fnum,iwidth,labtyp,outstr)
C      - subroutine NOTATEC (IX,IY,LENCHR,IARRAY)
C      - subroutine JUSTERC
C
C      - subroutine USESETC (fnum, iwidth, nbase, labstr)
C
C      subroutine MONPOS (nbase,iy1,dpos, spos) ! spos ist INTEGER
C      subroutine GLINE (nbase,datapt,spos) ! spos ist INTEGER
C
C     Der Code ab Version 2.0 wird nicht mehr fuer CP/M entwickelt. Letzte
C     unter CP/M compilierbare Version: (2006, 013, 1)
C
C     Zugehoerige Module:
C      - AG2.FOR:     Basisfunktionen
C      - AG2Holerith: Veraltete Unterprogramme zur Wahrung der Kompatibilitaet
C                     (Unterstuetzung Holerithvariablen und vektorisierter Zu-
C                     griff auf den Commonblock)
C      - AG2USR.FOR:  Userroutinen
C      - G2dAG2.fd:   Commonblockdefinition
C

C
C  Ausgabe der Softwareversion
C
      subroutine AG2LEV (ilevel)
      implicit none
      integer ilevel(3)

      call TCSLEV (ilevel) ! level(3)= System aus TCS
      ilevel(1)=2023       ! Aenderungsjahr
      ilevel(2)= 135       ! Aenderungstag
      return
      end



C
C  Setzen allgemeiner Commonvariablen
C
      subroutine LINE (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cline= ipar
      return
      end



      subroutine SYMBL (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      csymbl= ipar
      return
      end



      subroutine STEPS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      csteps= ipar
      return
      end



      subroutine INFIN (par)
      implicit none
      real par
      include 'G2dAG2.fd'

      if (par .gt. 0.) then
       cinfin= par
      end if
      return
      end



      subroutine NPTS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cnpts= ipar
      return
      end



      subroutine STEPL (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cstepl= ipar
      return
      end



      subroutine SIZES (par)
      implicit none
      real par
      include 'G2dAG2.fd'

      csizes= par
      return
      end



      subroutine SIZEL (par)
      implicit none
      real par
      include 'G2dAG2.fd'

      csizel= par
      return
      end



C
C  Setzen der achsenbezogenen Commonvariablen
C
      subroutine XNEAT (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyneat(1) = ipar .ne. 0
      return
      end



      subroutine YNEAT (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyneat(2) = ipar .ne. 0
      return
      end



      subroutine XZERO (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyzero(1) = ipar .ne. 0
      return
      end



      subroutine YZERO (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyzero(2) = ipar .ne. 0
      return
      end



      subroutine XLOC (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyloc(1)= ipar
      return
      end



      subroutine YLOC (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyloc(2)= ipar
      return
      end



      subroutine XLOCTP (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyloc(1)= ipar+abs(cxysmax(2)-cxysmin(2))
      return
      end



      subroutine YLOCRT (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxyloc(2)= ipar + abs(cxysmax(1)-cxysmin(1))
      return
      end



      subroutine XLAB (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxylab(1)= ipar
      return
      end



      subroutine YLAB (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxylab(2)= ipar
      return
      end



      subroutine XDEN (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 10)) then
       cxyden(1)= ipar
       cxytics(1)= 0
       cxymtcs(1)= 0
      end if
      return
      end



      subroutine YDEN (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 10)) then
       cxyden(2)= ipar
       cxytics(2)= 0
       cxymtcs(2)= 0
      end if
      return
      end



      subroutine XTICS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxytics(1)= abs(ipar)
      return
      end



      subroutine YTICS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxytics(2)= abs(ipar)
      return
      end



      subroutine XLEN (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if (ipar .ge. 0) then
       cxylen(1)= ipar
      end if
      return
      end



      subroutine YLEN (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if (ipar .ge. 0) then
       cxylen(2)= ipar
      end if
      return
      end



      subroutine XFRM (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 6)) then
       cxyfrm(1)= ipar
      end if
      return
      end



      subroutine YFRM (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 6)) then
       cxyfrm(2)= ipar
      end if
      return
      end



      subroutine XMTCS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxymtcs(1)= abs(ipar)
      return
      end



      subroutine YMTCS (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      cxymtcs(2)= abs(ipar)
      return
      end



      subroutine XMFRM (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 6)) then
       cxymfrm(1)= ipar
      end if
      return
      end



      subroutine YMFRM (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 6)) then
       cxymfrm(2)= ipar
      end if
      return
      end



      subroutine DLIMX (xmin,xmax)
      implicit none
      real xmin,xmax
      include 'G2dAG2.fd'

      cxydmin(1)= xmin
      cxydmax(1)= xmax
      return
      end



      subroutine DLIMY (ymin,ymax)
      implicit none
      real ymin,ymax
      include 'G2dAG2.fd'

      cxydmin(2)= ymin
      cxydmax(2)= ymax
      return
      end



      subroutine SLIMX (ixmin,ixmax)
      implicit none
      integer ixmin,ixmax
      include 'G2dAG2.fd'

      cxysmin(1)= ixmin
      cxysmax(1)= ixmax
      return
      end



      subroutine SLIMY (iymin,iymax)
      implicit none
      integer iymin,iymax
      include 'G2dAG2.fd'

      cxysmin(2)= iymin
      cxysmax(2)= iymax
      return
      end



      subroutine PLACE (ipar)
      implicit none
      include 'G2dAG2.fd'
      integer ipar

      integer postab (4,13)        ! Koordinaten des Zeichenbereiches
      data postab /150,900, 125,700,
     2             150,850, 525,700,
     3             150,850, 150,325,
     4             150,450, 525,700,
     5             650,950, 525,700,
     6             150,450, 150,325,
     7             650,950, 150,325,
     8             150,325, 525,700,
     9             475,650, 525,700,
     A             800,975, 525,700,
     1             150,325, 150,325,
     2             475,650, 150,325,
     3             800,975, 150,325/
      save postab

      if ((ipar .ge. 1) .and. (ipar.le.13)) then
       cxysmin(1)= postab(1,ipar)
       cxysmax(1)= postab(2,ipar)
       cxysmin(2)= postab(3,ipar)
       cxysmax(2)= postab(4,ipar)
      end if
      return
      end



      subroutine XTYPE (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 1) .and. (ipar .le. 8)) then
       cxytype(1)= ipar
      end if
      return
      end



      subroutine YTYPE (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 1) .and. (ipar .le. 8)) then
       cxytype(2)= ipar
      end if
      return
      end



      subroutine XWDTH (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if (ipar .ge. 0) then
       cxywdth(1)= ipar
      end if
      return
      end



      subroutine YWDTH (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if (ipar .ge. 0) then
       cxywdth(2)= ipar
      end if
      return
      end



      subroutine XETYP (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 4)) then
       cxyetyp(1)= ipar
      end if
      return
      end



      subroutine YETYP (ipar)
      implicit none
      integer ipar
      include 'G2dAG2.fd'

      if ((ipar .ge. 0) .and. (ipar .le. 4)) then
       cxyetyp(2)= ipar
      end if
      return
      end



      subroutine SETWIN
      implicit none
      include 'G2dAG2.fd'

      call TWINDO (cxysmin(1),cxysmax(1), cxysmin(2),cxysmax(2))
      call DWINDO (cxydmin(1),cxydmax(1), cxydmin(2),cxydmax(2))
      if (cxytype(1) .eq. 2) then
       if (cxytype(2) .eq. 2) then
        call LOGTRN (3)
       else
        call LOGTRN (1)
       end if
      else if (cxytype(2) .eq. 2) then
        call LOGTRN (2)
      else
       call LINTRN
      end if
      return
      end



      subroutine DINITX
      implicit none
      include 'G2dAG2.fd'

      cxydmin(1)= 0.        ! Datenbereich
      cxydmax(1)= 0.
      cxywdth(1)= 0         ! Dezimalstellen
      cxydec(1)=  0         ! Dezimalstellen
      cxyepon(1)= 0         ! Exponent Label
      return
      end



      subroutine DINITY
      implicit none
      include 'G2dAG2.fd'

      cxydmin(2)= 0.        ! Datenbereich
      cxydmax(2)= 0.
      cxywdth(2)= 0         ! Dezimalstellen
      cxydec(2)=  0         ! Dezimalstellen
      cxyepon(2)= 0         ! Exponent Label
      return
      end



      subroutine HBARST (ishade,iwbar,idbar)
      implicit none
      integer ishade,iwbar,idbar
      include 'G2dAG2.fd'

      cline= -3
      if ((ishade .ge. 0).and. (ishade .le. 15)) csymbl= ishade
      csizes= real (idbar)
      csizel= real (iwbar)

      if (cxyfrm(2) .eq. 5) then
       cxyfrm(2)= 2
      else if (cxyfrm(2) .eq. 6) then
       cxyfrm(2)= 1
      end if
      return
      end



      subroutine VBARST (ishade,iwbar,idbar)
      implicit none
      integer ishade,iwbar,idbar
      include 'G2dAG2.fd'

      cline=  -2
      if ((ishade .ge. 0) .and. (ishade .le. 15)) csymbl= ishade
      csizes= real (idbar)
      csizel= real (iwbar)
      if (cxyfrm(1) .eq. 5) then
       cxyfrm(1)= 2
      else if (cxyfrm(1) .eq. 6) then
       cxyfrm(1)= 1
      end if
      return
      end



C
C  Berechnung der Commonvariablen
C
      subroutine BINITT
      implicit none
      integer ih
      include 'G2dAG2.fd'

      cline= 0
      csymbl= 0
      csteps= 1
      cinfin= 1.E30
      cnpts= 0
      cstepl= 1
      cnumbr= 0
      csizes= 1.
      csizel= 1.

      cxyneat(1)= .true.
      cxyneat(2)= .true.
      cxyzero(1)= .true.
      cxyzero(2)= .true.
      cxyloc(1)= 0
      cxyloc(2)= 0
      cxylab(1)= 1
      cxylab(2)= 1
      cxyden(1)= 8
      cxyden(2)= 8
      cxytics(2)= 0
      cxytics(2)= 0

      call CSIZE (ih,cxylen(1))
      cxylen(2)= cxylen(1)

      cxyfrm(1)= 5
      cxyfrm(2)= 5
      cxymtcs(1)= 0
      cxymtcs(2)= 0
      cxymfrm(1)= 2
      cxymfrm(2)= 2
      cxydec(1)= 0
      cxydec(2)= 0
      cxydmin(1)= 0.
      cxydmin(2)= 0.
      cxydmax(1)= 0.
      cxydmax(2)= 0.

      cxysmin(1)= 150
      cxysmin(2)= 125
      cxysmax(1)= 900
      cxysmax(2)= 700

      cxytype(1)= 1
      cxytype(2)= 1
      cxylsig(1)= 0
      cxylsig(2)= 0
      cxywdth(1)= 0
      cxywdth(2)= 0
      cxyepon(1)= 0
      cxyepon(2)= 0
      cxystep(1)= 1
      cxystep(2)= 1
      cxystag(1)= 1
      cxystag(2)= 1
      cxyetyp(1)= 0
      cxyetyp(2)= 0
      cxybeg(1)= 0
      cxybeg(2)= 0
      cxyend(1)= 0
      cxyend(2)= 0
      cxymbeg(1)= 0
      cxymbeg(2)= 0
      cxymend(1)= 0
      cxymend(2)= 0
      cxyamin(1)= 0.
      cxyamin(2)= 0.
      cxyamax(1)= 0.
      cxyamax(2)= 0.
      return
      end



C
C  Datenanalyse
C

      subroutine CHECK (x,y)
      implicit none
      real  x(5),y(5)
      include 'G2dAG2.fd'

      external SPREAD ! External wg. Namenskonflikt FTN90-Intrinsic

      call TYPCK (1,x)
      call RGCHEK(1,x)
      call OPTIM (1)
      call WIDTH (1)
      if (cxystag(1) .eq. 1) call SPREAD (1)
      call TSET (1)

      call TYPCK (2,y)
      call RGCHEK(2,y)
      call OPTIM(2)
      call WIDTH(2)
      if (cxystag(2) .eq. 1) call SPREAD (2)
      call TSET (2)
      return
      end



      subroutine TYPCK (ixy, arr)
      implicit none
      integer ixy
      real arr(5)
      integer i
      include 'G2dAG2.fd'

      if ((cxytype(ixy) .lt. 3) .or. (nint(arr(1)) .lt. -1 )) then
       if ((cnpts .ne. 0) .or. (nint(arr(1)) .ne. -2) ) return
       i= nint(arr(3))
       if ( i .eq. 1) then
        cxytype(ixy)= 8
       else if ( i .eq. 4) then
        cxytype(ixy)= 7
       else if ( i .eq. 12) then
        cxytype(ixy)= 6
       else if ( i .eq. 13) then
        cxytype(ixy)= 5
       else if ( i .eq. 52) then
        cxytype(ixy)= 4
       else if ( i .eq. 365) then
        cxytype(ixy)= 3
       end if
      else
       cxytype(ixy)= 1
      end if
      return
      end



      subroutine RGCHEK (ixy,arr)
      implicit none
      integer ixy
      real arr(5)
      real amin, amax
      include 'G2dAG2.fd'

      if (cxydmax(ixy) .eq. cxydmin(ixy)) then ! Bereich schon bestimmt?
       if (cxyzero(ixy)) then ! Nullpunktunterdrueckung?
        amin= cinfin
       else
        amin= 0.
       end if
       amax= -amin
       call MNMX (arr, amin, amax)
       if (amax .eq. amin) then
        amin= amin - 0.5
        amax= amax + 0.5
       end if
       cxydmin(ixy)= amin
       cxydmax(ixy)= amax
      end if
      return
      end



      subroutine MNMX (arr,amin,amax)
      implicit none
      real arr(5), amin,amax, aminmax
      integer i, itype, nstart,nlim
      include 'G2dAG2.fd'

      if (cnpts .eq. 0) then                           ! Tek Standard-Format
       nlim= nint(arr(1)) + 1
       nstart= 2
      else
       nlim= cnpts
       nstart= 1
      end if
      if ((arr(1) .lt. 0.) .and. (cnpts .eq. 0))  then ! Kurzformate
       itype= abs(arr(1))
       if (itype .eq. 1) then
        aminmax= arr(3) + (arr(2)-1.) * arr(4)
        amin= amin1(arr(3),aminmax,amin)
        amax= amax1(arr(3),aminmax,amax)
       else if (itype .eq. 2) then
        call CMNMX (arr,amin,amax)
       else
        call UMNMX (arr,amin,amax)
       end if
      else                                             ! Langformate
       if (nstart .le. nlim) then
        do 100 i= nstart, nlim
         if (arr(i) .lt. cinfin) then
          if (arr(i).lt. amin) amin= arr(i)
          if (arr(i).gt. amax) amax= arr(i)
         end if
100     continue
       end if
      end if
      return
      end



      subroutine CMNMX (arr,amin,amax)
      implicit none
      real arr(5), amin, amax
      integer nTage, iStUBGC, nIntv, iadj, imin,imax
      integer minTg,minJr, maxTg,maxJr


      nIntv= nint(arr(3))
      if ((nIntv .eq. 52).or.(nIntv .eq. 13).or.(nIntv .eq. 4)) then
       if (nIntv .eq. 52) then          ! Wochen
        nTage=7
       else if (nIntv .eq. 13) then     ! 28 Tagemonat
        nTage= 28
       else if (nIntv .eq. 4) then      ! Quartal
        nTage=91
       end if
       call IUBGC (nint(arr(4)),1, iStUBGC)    ! Start: Jahr=arr(4), Tag=1
       iadj= mod(iStUBGC,7)
       if (iadj .gt. 3) iadj=iadj-7
       imin= iStUBGC-iadj + nint(arr(5))*nTage ! Min= f(Startjahr,StartIntervall)
       imax= imin + nint(arr(2))*nTage

      else
       if (nIntv .eq. 1) then ! Jahre
        minTg= 1
        maxTg= 1
        minJr= nint(arr(4))+1
        maxJr= nint(arr(4)+arr(2))
       else if ( nIntv .eq. 12) then ! Monate
        call YMDYD (minJr,minTg, nint(arr(4)),nint(arr(5))+1,1)
        call YMDYD (maxJr,maxTg, nint(arr(4)),nint(arr(5)+arr(2)),1)
       else if ( nIntv .eq. 365) then ! Tage
        minJr= nint(arr(4))
        minTg= nint(arr(5))
        maxJr= nint(arr(4))
        maxTg= nint(arr(5)+arr(2)) -1
       end if
       call IUBGC (minJr,minTg, imin)
       call IUBGC (maxJr,maxTg, imax)
      end if
      if (real(imax) .gt. amax) amax= real(imax)
      if (real(imin) .lt. amin) amin= real(imin)
      return
      end



C
C  Ticmarkoptimierung
C

      subroutine OPTIM (ixy)
      implicit none
      integer ixy
      include 'G2dAG2.fd'

      if (cxytype(ixy) .eq. 2) cxylab(ixy)= 2
      if (cxylab(ixy) .eq. 2) cxylab(ixy)= cxytype(ixy)
      if (cxytype(ixy) .le. 2) then
       call LOPTIM (ixy) ! Tic-Mark Optimierung fuer lineare und log. Daten
      else
       call COPTIM (ixy) ! Tic-Mark Optimierung fuer Kalenderdaten
      end if
      return
      end



      subroutine LOPTIM (ixy)
      implicit none
      integer ixy ,i, labtyp, ntics, lsig, mtcs
      real dataint, amin,amax, aminor,amaxor, sigfac
      integer idataint
      integer mintic
      integer LINWDT, LINHGT
      real ROUNDD, ROUNDU
      include 'G2dAG2.fd'

      labtyp=abs( cxylab(ixy)) ! <0: Userlabel
      if (labtyp .le. 1) labtyp= cxytype(ixy) ! Default: Achsentyp = Datentyp

      amin= cxydmin(ixy)
      amax= cxydmax (ixy)
      ntics= abs(cxytics (ixy)) ! Anzahl >=1, 0= Flag fuer autoscale
      mintic= 0

      if (labtyp .eq. 2) then ! logarithmische Achsen
       amin= log10(max(amin,1./cinfin)) + 1.e-7  ! !> 0 => log10 definiert
       amax= log10(amax)
      end if

      aminor= amin
      amaxor= amax

      if (ntics .eq. 0) then ! = F( X-Achsenlaenge,Buchstabengroesse)
       if (ixy.eq.1) then
        i= LINWDT(8) ! 100 + LINWDT(3)
       else
        i= LINHGT(3) ! 50 + LINHGT(3)
       end if
       ntics= (cxysmax(ixy) - cxysmin(ixy)) / i
       if (ntics .lt. 1) ntics= 1
      end if
      dataint= abs(amax-amin) / real(ntics)

310   continue ! repeat...
       if (labtyp .eq. 2) dataint= ROUNDU (dataint,1.) ! logarithmische Achsen
       lsig= ROUNDD (log10(dataint),1.) ! Anzahl signifikanter Nachkommastellen
       sigfac=10.**(lsig)
       if (cxyneat(ixy)) then ! Achsenteilung aus Tabelle
        if(labtyp .ne. 2) then ! nicht bei log. Achsen
         if ((dataint/sigfac) .le. 1.) then
          dataint= 1. * sigfac
          mintic= 10
         else if ((dataint/sigfac) .le. 2.) then
          dataint= 2. * sigfac
          mintic= 2
         else if ((dataint/sigfac) .le. 2.5) then
          dataint= 2.5 * sigfac
          mintic= 5
          lsig=lsig-1
         else if ((dataint/sigfac) .le. 5.) then
          dataint= 5. * sigfac
          mintic=  5
         else if ((dataint/sigfac) .le. 10.) then
          dataint= 10. * sigfac
          mintic= 10
          lsig=lsig+1
         else
          dataint= cinfin
          mintic= 0
         end if
        end if ! log. Achse
       else ! .not. neat
        lsig=lsig-2
       end if
       if (lsig .ge. 0) lsig=lsig+1
      if (cxyneat(ixy) .or. (labtyp .eq. 2) ) then ! ... until
       amin= ROUNDD (amin+.01*sigfac,dataint) !  runde auf TicIntervall
       amax= ROUNDU (amax-.01*sigfac,dataint) ! .01*sigfac= Genauigkeit Plot
       ntics= int(abs(amax-amin)/dataint+.0001)
       if(cxytics(ixy) .ne. 0) then ! until: ntics nicht vorbesetzt oder = vorbesetzt
        if(abs(cxytics(ixy)) .lt. ntics) then
         dataint= dataint * 1.1
         amin=aminor
         amax=amaxor
         goto 310 ! noch eine Iterationsschleife
        else if (abs(cxytics(ixy)) .gt. ntics) then
         ntics= abs(cxytics(ixy))
         amax= amin + real(ntics) * dataint
        end if ! abs(cxytics(ixy)) .eq. ntics: no action
       end if
      end if
      cxytics(ixy)= ntics

      if ((cxymtcs(ixy) .eq. 0) .and. (cxyden(ixy) .ge. 6)) then ! unbesetzt oder wenig TICS
       mtcs= mintic ! Bestimmung Minor TicMarcs
       if((mtcs .eq. 10) .or. (labtyp .eq. 2)) then
        if(cxyden(ixy) .lt. 9) mtcs=5
        if(cxyden(ixy) .lt. 7) mtcs=2
        if(labtyp .eq. 2) then ! log. Achsen
         idataint= nint(dataint)
         if (idataint .ne. 1) then ! mehrere Achsenintervalle
          i= 1
320       continue ! repeat...
           mtcs= idataint/i
          if ((mtcs*i .ne. idataint) .and. (i .lt. (idataint-1))) then ! ...until
           i= i+1
           goto 320
          else if (mtcs .gt. 10 ) then
           mtcs= 0  ! Failure
          end if
         else ! einzelne logarithmische Dekade
          if ((cxysmax(ixy) - cxysmin(ixy)) .ge. 100* ntics) mtcs=-1 ! logarithm. Tics
          if ((cxysmax(ixy) - cxysmin(ixy)) .ge. 20* LINHGT(1)) mtcs=-2 ! Label
         end if
        end if
       end if
       cxymtcs(ixy)= mtcs
      end if

      cxylsig(ixy)= lsig
      cxyamin(ixy)= amin
      cxyamax(ixy)= amax
      if (labtyp .eq. 2) then ! logarithmische Achsen: Wiederherstellung der Originalwerte
       amax=10.**amax
       amin=10.**amin
      end if
      cxydmin(ixy)= amin
      cxydmax(ixy)= amax
      return
      end



      subroutine COPTIM (ixy)
      implicit none
      integer ixy , labtyp, ntics
      real dataint, amin,amax, aminor,amaxor
      integer LINWDT
      real ROUNDD, ROUNDU
      include 'G2dAG2.fd'

      if (cxytics(ixy) .eq. 1) cxytics(ixy)= 2 ! Minimum manuelle Ticwahl: 2
      labtyp=abs( cxylab(ixy)) ! <0: Userlabel
      if (labtyp .le. 1) labtyp= cxytype(ixy) ! Default: Achsentyp = Datentyp
      amin= cxydmin(ixy)
      amax= cxydmax (ixy)
      call CALCON (amin,amax,labtyp,.true.) ! Konvertiere UBGC -> Labelzeiteinheit
      ntics= cxytics(ixy)
      aminor=amin
      amaxor=amax
      if (ntics .eq. 0) then ! = F( X-Achsenlaenge,Buchstabengroesse)
       ntics= (cxysmax(ixy) - cxysmin(ixy)) / (25 + LINWDT(1))
       if (ntics .lt. 2) ntics= 2
      end if
      dataint= abs(amax-amin) / real(ntics)

      if (cxyneat(ixy)) then ! Achsenteilung aus Tabelle
310    continue ! repeat...
        if (cxytics(ixy) .eq. 0) then ! keine manuelle Belegung erfolgt
         if (labtyp.eq.3) then ! Labeltyp: Tage
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 7.) then
           dataint= 7.
          else if (dataint .le. 14.) then
           dataint= 14.
          else if (dataint .le. 28.) then
           dataint= 28.
          else if (dataint .le. 56.) then
           dataint= 56.
          else if (dataint .le. 128.) then
           dataint= 128.
          end if ! dataint > 128 -> unveraendert
         else if (labtyp.eq.4) then ! Labeltyp: Wochen
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 2.) then
           dataint= 2.
          else if (dataint .le. 4.) then
           dataint= 4.
          else if (dataint .le. 8.) then
           dataint= 8.
          else if (dataint .le. 16.) then
           dataint= 16.
          else if (dataint .le. 26.) then
           dataint= 26.
          else if (dataint .le. 52.) then
           dataint= 52.
          else if (dataint .le. 104.) then
           dataint= 104.
          end if ! dataint -> unveraendert
         else if (labtyp.eq.5) then ! Labeltyp: Kalenderabschnitte
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 2.) then
           dataint= 2.
          else if (dataint .le. 13.) then
           dataint= 13.
          else if (dataint .le. 26.) then
           dataint= 26.
          else if (dataint .le. 52.) then
           dataint= 52.
          end if ! dataint -> unveraendert
         else if (labtyp.eq.6) then ! Labeltyp: Monate
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 2.) then
           dataint= 2.
          else if (dataint .le. 3.) then
           dataint= 3.
          else if (dataint .le. 4.) then
           dataint= 4.
          else if (dataint .le. 6.) then
           dataint= 6.
          else if (dataint .le. 12.) then
           dataint= 12.
          else if (dataint .le. 24.) then
           dataint= 24.
          else if (dataint .le. 36.) then
           dataint= 36.
          end if ! dataint -> unveraendert
         else if (labtyp.eq.7) then ! Labeltyp: Quartale
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 2.) then
           dataint= 2.
          else if (dataint .le. 4.) then
           dataint= 4.
          else if (dataint .le. 8.) then
           dataint= 8.
          else if (dataint .le. 12.) then
           dataint= 12.
          else if (dataint .le. 16.) then
           dataint= 16.
          else if (dataint .le. 24.) then
           dataint= 24.
          end if ! dataint -> unveraendert
         else if (labtyp.eq.8) then ! Labeltyp: Jahre
          if (dataint .le. 1.) then
           dataint= 1.
          else if (dataint .le. 2.) then
           dataint= 2.
          else if (dataint .le. 5.) then
           dataint= 5.
          else if (dataint .le. 10.) then
           dataint= 10.
          else if (dataint .le. 20.) then
           dataint= 20.
          else if (dataint .le. 50.) then
           dataint= 50.
          else if (dataint .le. 100.) then
           dataint= 100.
          end if ! dataint -> unveraendert
         end if ! labtyp 3..8
        end if ! manuelle Vorbesetzung
        amin= ROUNDD (amin,dataint) !  runde auf TicIntervall
        amax= ROUNDU (amax,dataint)
        ntics= ifix(abs(amax-amin)/dataint+.0001)
        if (ntics .eq. 0) ntics = 2
       if(cxytics(ixy) .ne. 0) then ! until: ntics nicht oder = vorbesetzt
        if(abs(cxytics(ixy)) .lt. ntics) then ! Verringere Ticanzahl
         dataint= dataint * 1.1
         amin=aminor
         amax=amaxor
         goto 310 ! noch eine Iterationsschleife
        else if (abs(cxytics(ixy)) .gt. ntics) then ! Vergroessere Ticanzahl
         ntics= abs(cxytics(ixy))
         amax= amin + real(ntics) * dataint
        end if ! abs(cxytics(ixy)) .eq. ntics: no action
       end if ! Ende der Schleife
      end if ! neat
      cxytics(ixy)= ntics
      cxylsig(ixy)= 0
      cxyamin(ixy)= amin
      cxyamax(ixy)= amax
      call CALCON (amin,amax,labtyp,.false.) ! Labelzeiteinheit -> UBGC
      cxydmin(ixy)= amin
      cxydmax(ixy)= amax
      return
      end



C
C  Kalenderroutinen
C



      real function CALPNT (arr,i)
      implicit none
      integer i
      real arr(5)
      integer iy,idays, itmp
      integer icltyp, istyr, istper, iubg1, iweek1, nodays
      save icltyp, istyr, istper, iubg1, iweek1, nodays

      if (i .eq. 1) then ! 1. Datenpunkt: Formatanalyse, Parameterberechnung
       istyr= nint (arr(4))
       istper= nint(arr(5))
       itmp= nint (arr(3)) ! Laenge Intervall in Tagen
       if (itmp .eq. 12) then ! Zeitintervall Monat
        icltyp= 2
       else if (itmp .eq. 365) then ! Zeitintervall Tage
        icltyp=3
        call IUBGC (istyr,istper,iubg1)
       else if (itmp .eq. 52) then ! Zeitintervall Wochen
        icltyp= 4
        nodays= 7
       else if (itmp .eq. 13) then ! Zeitintervall 4 Wochen
        icltyp= 5
        nodays= 28
       else if (itmp .eq. 4) then  ! Zeitintervall Quartal
        icltyp= 6
        nodays= 91
       else ! Zeitintervall Jahre
        icltyp= 1
       end if
       if (icltyp .ge. 4) then
        call IUBGC (istyr,1,iubg1)
        itmp= mod(iubg1+1,7)
        if(itmp .gt. 3) itmp= itmp-7
        iweek1= iubg1-itmp
        iubg1= iweek1+(istper-1)*nodays
       end if
      end if ! Ende Initialisierung, jetzt Berechnung

      if (icltyp .eq. 1) then ! Zeitintervall Jahr
       call IUBGC (istyr+i,1,iubg1)
       CALPNT= iubg1
      else if (icltyp .eq. 2) then ! Zeitintervall Monat
       call YMDYD (iy,idays,istyr,istper+i,1)
       call IUBGC (iy,idays,iubg1)
       CALPNT= iubg1 ! Zeitintervall Tage
      else if (icltyp .eq. 3) then
       CALPNT= iubg1+i-1
      else ! Zeitintervall Wochen oder 4 Wochen
       calpnt= iweek1+(istper-1+i)*nodays
      end if
      return
      end



      subroutine CALCON (amin,amax,labtyp,ubgc)
      implicit none
      real amin, amax
      integer labtyp
      logical ubgc
      integer iubg1, iubg2, iday1, iadj, id, month1,month2 , imin,imax
      real dimin, dimax
      integer iweek1
      real fnoday
      integer iy1,iy2, iy3,iy4, idays
      save iweek1, fnoday
      save iy1,iy2, iy3, iy4, idays

      real ROUNDD, ROUNDU

      if (labtyp .le. 3) return ! nicht Kalender, bzw.Tage: keine Transformation

      if (ubgc) then ! Konvertierung UBGC in Labeltype
       if ( (labtyp .eq. 4).or.(labtyp .eq. 5).or.(labtyp .eq. 7) ) then
        if (labtyp .eq. 4) fnoday= 7.
        if (labtyp .eq. 5) fnoday= 28.
        if (labtyp .eq. 7) fnoday= 91.
        iubg1=amin
        iubg2=amax
        call OUBGC (iy1,idays,iubg1) ! Wochenanfang der 1.KW Startjahr
        iday1=iubg1-idays+1
        iadj=mod(iday1+1,7)
        if(iadj .gt. 3) iadj=iadj-7
        iweek1= iday1-iadj           ! Merken in iweek1
        dimin= ROUNDD (real(iubg1-iweek1),fnoday)
        dimin= dimin/fnoday+1.
        call OUBGC (iy2,idays,iubg2)
        dimax= ROUNDU (real(iubg2-iweek1),fnoday)
        dimax= dimax/fnoday
       else if (labtyp .eq. 6) then
        call OUBGC (iy1,idays,nint(amin))
        call YDYMD (iy1,idays,iy3,month1,id)
        dimin= month1
        call OUBGC (iy2,idays,nint(amax))
        call YDYMD (iy2,idays,iy4,month2,id)
        dimax= (iy4-iy3)*12+month2
        if(id .gt. 1) dimax=dimax+1.
       else if (labtyp .eq. 8) then
        call OUBGC (iy1,idays,nint(amin))
        dimin= iy1
        call OUBGC(iy2,idays,nint(amax))
        dimax= iy2
        if(idays .gt. 1) dimax=dimax+1.
       end if
       amin= dimin-1.
       amax= dimax-1.
       return

      else ! Konvertierung Labeltype in UBGC
       amin=amin+1.
       amax=amax+1.
       if ((labtyp .eq. 4).or.(labtyp .eq. 5).or.(labtyp .eq. 7)) then
        amin= iweek1 + (nint(amin)-1) * nint(fnoday)
        amax= iweek1+(nint(amax)-1)*nint(fnoday)
       else if (labtyp .eq. 6)then
        iy4= iy3
        call YMDYD (iy1,idays,iy3,nint(amin),1)
        call IUBGC (iy1,idays,imin)
        amin= imin
        call YMDYD (iy2,idays,iy4,nint(amax),1)
        call IUBGC (iy2,idays,imax)
        amax= imax
       else if (labtyp .eq. 8) then
        call IUBGC (nint(amin),1,imin)
        amin= imin
        call IUBGC (nint(amax),1,imax)
        amax= imax
       end if
      endif
      return
      end


      subroutine YMDYD (iJulYrOut,iJulDayOut,
     1                               iGregYrIn,iGregMonIn,iGregDayIn)
      implicit none
      integer iJulYrOut,iJulDayOut, iGregYrIn,iGregMonIn,iGregDayIn
      integer iJulYrIn,iJulDayIn, iGregYrOut,iGregMonOut,iGregDayOut
      integer iMon, LEAP
      integer iDatTab(12)
      save iDatTab
      data iDatTab /0,31,59,90,120,151,181,212,243,273,304,334/

      iJulYrOut= iGregYrIn
      iMon= iGregMonIn
100   if (iMon .lt. 1) then ! while iMon .not. in [1..12]
       iMon= iMon + 12
       iJulYrOut= iJulYrOut-1
       goto 100
      else if (iMon .gt. 12) then
       iMon= iMon -12
       iJulYrOut= iJulYrOut+1
       goto 100
      end if
      iJulDayOut= iGregDayIn + iDatTab(iMon)
      if (iMon .gt.2) iJulDayOut= iJulDayOut + LEAP(iJulYrOut)
      return

C> entry subroutine YMDYD (iJulYrIn,iJulDayIn,iGregYrOut,iGregMonOut,iGregDayOut)
      entry YDYMD (iJulYrIn,iJulDayIn,
     1                        iGregYrOut,iGregMonOut,iGregDayOut)

      iGregDayOut= iJulDayIn
      iGregYrOut= iJulYrIn
110   if (iGregDayOut .lt. 1) then ! while iGregDayOut .not. in [1..365(366)]
       iGregYrOut= iGregYrOut-1
       iGregDayOut= iGregDayOut + 365 + LEAP (iGregYrOut)
       goto 110
      else if (iGregDayOut .gt. 365+ LEAP(iGregYrOut)) then
       iGregYrOut= iGregYrOut+1
       iGregDayOut= iGregDayOut - 365 - LEAP (iGregYrOut)
       goto 110
      end if

      iGregMonOut= int( real(iGregDayOut)/29.5+1.)
      if (iGregDayOut .le. iDatTab(iGregMonOut)) then
       if ((iGregMonOut .le. 2) .or.
     1   (iGregDayOut.le.(iDatTab(iGregMonOut)+LEAP(iGregYrOut)))) then
        iGregMonOut= iGregMonOut-1
       end if
      end if
      iGregDayOut= iGregDayOut- iDatTab(iGregMonOut)
      if (iGregMonOut .gt. 2) iGregDayOut= iGregDayOut -LEAP(iGregYrOut)
      return
      end



      integer function  LEAP (iyear)
      implicit none
      integer iyear
      if (  (mod(iyear,4) .eq. 0) .and.
     1     ((mod(iyear,100).ne.0) .or. (mod(iyear,400).eq.0))  ) then
       LEAP= 1
      else
       LEAP= 0
      end if
      return
      end



      subroutine IUBGC(iyear,iday, iubgcO)
      implicit none
      integer iyear,iday,iubgcO
      integer iYr1

      iYr1= iYear-1 ! Schaltjahreskorrektur erst nach Jahresabschluss
      iubgcO= 365* (iyear-1901) ! Verhinderung Overflow: Offset im Faktor
      iubgcO= iubgcO + int(iYr1/4) - int(iYr1/100) + int(iYr1/400)
      iubgcO= iubgcO + iday -460 ! Bezugsdatum 1.1.1901= 365*1901 + 460 Schalttage
      return
      end



      subroutine OUBGC(iyear,iday,iubgcI)
      implicit none
      integer iyear,iday,iubgcI
      integer iYr1

      iyear= int( (real(iubgcI) + 694325.99) / 365.2425 )
100   continue ! Schleife der evtl. Nachiteration
       iYr1= iYear-1 ! Schaltjahreskorrektur erst nach Jahresabschluss
       iday= iubgcI + 460 - 365*(iyear-1901)
       iday= iday + int(iYr1/100) - int(iYr1/4) - int(iYr1/400)
      if (iday .lt. 1) then ! Nachiteration?
       iyear= iyear-1
       goto 100
      end if
      return
      end



C
C  Zeichenroutinen
C

      subroutine FRAME
      implicit none
      include 'G2dAG2.fd'

      call MOVABS (cxysmax(1),cxysmin(2))
      call DRWABS (cxysmax(1),cxysmax(2))
      call DRWABS (cxysmin(1),cxysmax(2))
      call DRWABS (cxysmin(1),cxysmin(2))
      call DRWABS (cxysmax(1),cxysmin(2))
      return
      end



      subroutine DSPLAY (x,y)
      implicit none
      real x(5),y(5)

      call SETWIN
      call CPLOT (x,y)
      call GRID
      call LABEL (1)
      call LABEL (2)
      return
      end



      subroutine CPLOT (x,y)
      implicit none
      real x(5),y(5)
      logical symbol
      integer i,i1, keyx, keyy, lines, linsav, icount, imax
      real xpoint(1), ypoint(1)
      real DATGET
      include 'G2dAG2.fd'

      call KEYSET (x,keyx)
      call KEYSET (y,keyy)
      if (keyx .eq. 1) then ! standard long
       imax= x(1)
      else if ((keyx .ge. 2) .and. (keyx .le. 4)) then ! short
       imax= x(2)
      else ! nonstandard
       imax= cnpts
      end if
      if (keyy .eq. 1) then ! standard long
       if (imax .lt. y(1)) imax= y(1)
      else if ((keyx .ge. 2) .and. (keyx .le. 4)) then ! short
       if (imax .lt. y(2)) imax= y(2)
      else ! nonstandard
       if (imax .lt. cnpts) imax= cnpts
      end if

      symbol= (csymbl .ne. 0) .and.(cline .ne.-2) .and.(cline .ne.-3)

      i= 1 ! Suche Startpunkt
100   continue ! repeat
       if (i .gt. imax) return ! kein Punkt zu zeichnen
       xpoint(1)= DATGET (x,i,keyx)
       ypoint(1)= DATGET (y,i,keyy)
      if ((xpoint(1) .ge. cinfin) .or. (ypoint(1) .ge. cinfin)) then ! while
       i= i+cstepl
       goto 100
      end if

      call MOVEA (xpoint(1),ypoint(1))
      if (cline .eq. -4) call POINTA (xpoint(1),ypoint(1))
      if (cline .lt. -10) call ULINE (xpoint(1),ypoint(1),1)
      if (cline .eq.-2 .or. cline .eq.-3) then
       call BAR (xpoint(1),ypoint(1),cline)
      end if
      if (symbol) call BSYMS (xpoint(1),ypoint(1),csymbl)

      if (cline .eq. -1) then
       lines= 2
      else if ((cline .eq. -2) .or. (cline .eq. -3)) then
       lines= 3
      else if (cline .eq. -4) then
       lines=4
      else if (cline .lt. -10) then
       lines=5
      else
       lines=1 ! bei cline = 0: dash ergibt durchgezogene Linie
      end if

      i1= i+cstepl
      if (i1 .ge. imax) return
      icount= csteps
      linsav= lines

      do 900 i=i1,imax,cstepl
       xpoint(1)= DATGET (x,i,keyx)
       ypoint(1)= DATGET (y,i,keyy)
       if ((xpoint(1) .ge. cinfin) .or. (ypoint(1) .ge. cinfin)) then
        if (i.gt.imax-cstepl) return ! Der letzte Punkt ist ungueltig -> done
        if ((cline .ne. -2) .and. (cline .ne. 3)) lines= 2
       else
        if (lines .eq. 1 ) then
         call DASHA (xpoint(1),ypoint(1), cline) ! dashed or solid
        else if (lines .eq. 2 ) then
         call MOVEA (xpoint(1),ypoint(1))
         lines=linsav ! restore after missing data
        else if (lines .eq. 3 ) then
         call BAR (xpoint(1),ypoint(1),0)
        else if (lines .eq. 4 ) then
         call POINTA (xpoint(1),ypoint(1))
        else
         call ULINE (xpoint(1),ypoint(1),i)
        end if
        if (symbol) then
         icount=icount-1
         if(icount .le. 0) then
          icount= csteps
          call BSYMS (xpoint(1),ypoint(1),csymbl)
         end if
        end if
       end if
900   continue
      return
      end



      subroutine KEYSET (array,key)
      implicit none
      integer key
      integer npts
      real array(1)
      include 'G2dAG2.fd'

      if (cnpts .ne. 0) then       ! nonstandard array
       Key= 5
      else
       npts= nint (array(1))
       if (npts .ge. 0) then       ! standard long
        key= 1
       else if (npts .eq. -1) then ! short
        key= 2
       else if (npts .eq. -2) then ! short calendar
        key= 3
       else                        ! short user
        key= 4
       end if
      end if
      return
      end



      real function DATGET (arr,i,key)
      implicit none
      integer i, key
      real CALPNT, UPOINT
      real arr(5) ! Dimension 5 sonst GNU-Compilerwarnung bei dat= ...arr(5)...
      real dat, olddat
      save olddat

      if (key.eq.1) then ! standard long
       dat= arr(i+1)
      else if (key.eq.2) then ! standard short
       dat= arr(3) + arr(4)*real(i-1)
      else if (key.eq.3) then ! short calendar
       dat= CALPNT (arr,i)
      else if (key.eq.4) then ! user
       dat= UPOINT (arr,i,olddat)
      else if (key.eq.5) then ! non standard
       dat= arr(i)
      endif
      olddat= dat
      DATGET= dat
      return
      end



C  Balkendiagramme

      subroutine BAR (x,y,line)
      implicit none
      real x, y
      integer line
      integer key, ix,iy, ixl,iyl,ixh,iyh
      real xfac, yfac
      logical VerticalBar
      integer isymb, ihalf, lspace, minx,maxx,miny,maxy, ibegx,ibegy
      SAVE isymb, ihalf, lspace, minx,maxx,miny,maxy, ibegx,ibegy
      SAVE VerticalBar
      include 'G2dAG2.fd'

      if (line .ne. 0) then ! Erster Aufruf -> Parameterbestimmung
       VerticalBar= line .ne. -3
       isymb= csymbl
       ihalf= .5 * csizel
       lspace= csizes
       if (lspace .le. 1) lspace=20 ! Default: 20 Pixel Schraffur
       if (ihalf .lt. 2) ihalf=20 ! Default: 40 Pixel Balkenbreite
       if (cxysmin(1) .le. cxysmax(1)) then
        minx= cxysmin(1)
        maxx= cxysmax(1)
       else
        minx= cxysmax(1)
        maxx= cxysmin(1)
       end if
       if (cxysmin(2) .le. cxysmax(2)) then
        miny= cxysmin(2)
        maxy= cxysmax(2)
       else
        miny= cxysmax(2)
        maxy= cxysmin(2)
       end if

       call seetrn(xfac,yfac, key)
       if (key .eq. 2) then ! logarithmische Werte
        ibegx= cxysmin(1)
        ibegy= cxysmin(2)
       else
        call WINCOT (0.,0.,ibegx,ibegy)
       end if
      end if

      call WINCOT (x,y,ix,iy)
      if (VerticalBar) then ! vertikale Balken
       iyl= min0(ibegy,iy)
       iyh= max0(ibegy,iy)
       ixl= min0(ix-ihalf,ix+ihalf)
       ixh= max0(ix-ihalf,ix+ihalf)
      else ! horizontale Balken
       iyl= min0(iy-ihalf,iy+ihalf)
       iyh= max0(iy-ihalf,iy+ihalf)
       ixl= min0(ibegx,ix)
       ixh= max0(ibegx,ix)
      end if
      ixl=max0(ixl,minx)
      ixh=min0(ixh,maxx)
      iyl=max0(iyl,miny)
      iyh=min0(iyh,maxy)
      if ((ixh-ixl .ge. 2) .and. (iyh-iyl .ge. 2)) then ! mindestens 2x2 Pxl
       call filbox(ixl,iyl,ixh,iyh,isymb,lspace)
      end if
      return
      end



      subroutine FILBOX (minx,miny,maxx,maxy,ishade,lspace)
      implicit none
      integer minx,miny,maxx,maxy,ishade,lspace
      integer iminx,imaxx,iminy,imaxy
      integer i, ishift, idely, iymax
      real ximin, ximax
      real savcom (60)

      iminx= min0(minx,maxx)        ! zeichne Rechteck
      iminy= min0(miny,maxy)
      imaxx= max0(minx,maxx)
      imaxy= max0(miny,maxy)

      call MOVABS (iminx,iminy)
      call DRWABS (imaxx,iminy)
      call DRWABS (imaxx,imaxy)
      call DRWABS (iminx,imaxy)
      call DRWABS (iminx,iminy)

      if ((ishade .le.0) .or. (ishade .gt. 15)) return ! ohne Schraffur

      ishift= ishade / 2
      if ((ishade-ishift*2) .ne. 0) then ! Bit0: horizontale Schraffur
       i= iminy
100    continue ! repeat...
        i= i+lspace
       if (i .lt. imaxy) then
        call MOVABS (iminx,i)
        call DRWABS (imaxx,i)
        goto 100 ! ... until
       end if
      end if ! horizontale Schraffur gezeichnet

      if (mod (ishift,2) .ne. 0) then ! Bit1: vertikale Schraffur
       i= iminx
110    continue ! repeat
        i= i+lspace
       if(i .lt. imaxx) then
        call MOVABS (i,iminy)
        call DRWABS (i,imaxy)
        goto 110
       end if ! vertikale Schraffur gezeichnet
      end if

      if (ishade .ge. 4) then ! diagonale Schraffuren
       ximin= real(iminx)
       ximax= real(imaxx)
       call SVSTAT (savcom) ! verwende TCS-Clipping
       call LINTRN
       call DWINDO (ximin,ximax,real(iminy),real(imaxy))
       call TWINDO (iminx,imaxx,iminy,imaxy)

       if (ishade .ge. 8) then ! Bit3: diagonal fallend
        idely= iminx-imaxx
        iymax= imaxy+imaxx-iminx
        i= iminy+lspace
120     continue ! repeat ...
         call MOVEA (ximin,real(i))
         call DRAWA (ximax,real(i+idely))
         i= i+lspace
        if (i .lt. iymax) goto 120 ! ... until
        ishift= ishade -8
       else
        ishift= ishade
       end if

       if (ishift .ge. 4) then ! Bit2: diagonal steigend
        idely= imaxx-iminx
        iymax= real(imaxy)
        i= iminy - idely + lspace
130     continue ! repeat...
         call MOVEA (ximin,real(i))
         call DRAWA (ximax,real(i+idely))
         i= i+lspace
        if (i .lt. iymax) goto 130 ! ...until
       end if
       call RESTAT (savcom)
      end if ! Diagonalen
      return
      end



C  Zeichnen von Symbolen

      subroutine BSYMS (x,y,isym)
      implicit none
      real x,y
      integer isym
      include 'G2dAG2.fd'

      if (isym .ge. 0) then
       call SYMOUT (isym, csizes)
      else
       call USERS (x,y,isym)
      end if
      call MOVEA (x,y)
      return
      end



      subroutine SYMOUT (isym,fac)
      implicit none
      integer isym
      real fac
      integer ix,iy, ihorz,ivert

      call SEELOC (ix,iy)
      if (isym .gt. 127) then
       call SOFTEK (isym)
      else if (isym .ge. 33) then
       call CSIZE (ihorz,ivert)
       ihorz= int( real(ihorz)*.3572)
       ivert= int( real(ivert)*.3182)
       call MOVREL (-ihorz,-ivert)
       call ALFMOD
       call TOUTPT (isym)
      else if (isym .le. 11) then
       call TEKSYM (isym,fac)
      end if
      call MOVABS (ix,iy)
      return
      end



      subroutine TEKSYM (isym,amult)
      implicit none
      integer isym
      real amult
      integer ihalf, ifull

      ihalf= nint(8.* amult)
      ifull=ihalf * 2
      if (isym .eq. 1) then ! Kreis
       call TEKSYM1 (0, 360, 30, 8.*amult)
      else if (isym .eq. 2) then ! X
       call MOVREL (ihalf,ihalf)
       call DRWREL (-ifull,-ifull)
       call MOVREL (0,ifull)
       call DRWREL (ifull,-ifull)
      else if (isym .eq. 3) then ! Dreieck
       call TEKSYM1 (90, 450, 120, 8.*amult)
      else if (isym .eq. 4) then ! Quadrat
       call TEKSYM1 (45, 405, 90, 8.*amult)
      else if (isym .eq. 5) then ! Stern
       call TEKSYM1 (90, 810, 144, 8.*amult)
      else if (isym .eq. 6) then ! Raute
       call TEKSYM1 (90, 450, 90, 8.*amult)
      else if (isym .eq. 7) then ! vertikaler Balken
       call TEKSYM1 (90, 270, 180, 8.*amult)
      else if (isym .eq. 8) then ! Kreuz
       call MOVREL (0,ihalf)
       call DRWREL (0,-ifull)
       call MOVREL (-ihalf,ihalf)
       call DRWREL (ifull,0)
      else if (isym .eq. 9) then ! Pfeil nach oben
       call DRWREL (-2,-6)
       call DRWREL (4,0)
       call DRWREL (-2,6)
       call DRWREL (0,-ifull)
      else if (isym .eq. 10) then ! Pfeil nach unten
       call DRWREL (-2,6)
       call DRWREL (4,0)
       call DRWREL (-2,-6)
       call DRWREL (0,ifull)
      else if (isym .eq. 11) then ! Durchstreichung
       call TEKSYM1 (270, 630, 120, 8.*amult)
      end if
      return
      end



      subroutine TEKSYM1 (istart, iend, incr, siz)
      implicit none
      integer istart, iend, incr
      real siz
      integer i, mx,my,mix,miy
      real b

      b= real(istart)*.01745
      mx= nint(siz*cos(b))
      my= nint(siz*sin(b))
      call MOVREL (mx,my)
      do 100 i= istart+incr, iend, incr
       b= real(i)*.01745
       mix= nint(siz*cos(b))
       miy= nint(siz*sin(b))
       call DRWREL (mix-mx,miy-my)
       mx= mix
       my= miy
100   continue
      return
      end



C Netz und Ticmarks

      subroutine GRID
      implicit none
      integer i, mlim
      real xyext,xyextm, tintvl,tmntvl
      include 'G2dAG2.fd'

      if (cxyfrm(2) .ne. 0) then ! Zeichnen der y-Achse
       i= min0(cxysmin(1),cxysmax(1)) + cxyloc(2)
       call MOVABS (i, cxysmax(2))
       call DRWABS (i, cxysmin(2))
       if (cxybeg(2) .ne. cxyend(2)) then ! Zeichnen y-Ticmarks
        i= cxylab(2) ! Labeltyp
        if (i .eq. 1) i= cxytype(2) ! =1: Typ entsprechend Daten
        if (i .ne. 6) then ! =6 (Monate): Tics durch GLINE zeichnen lassen
         if(cxytics(2) .ne. 0) then
          tintvl= real(cxysmax(2)-cxysmin(2)) / real( cxytics(2))
         end if
         if (cxymtcs(2) .gt. 0) tmntvl= tintvl / real(cxymtcs(2))
         call movabs(cxybeg(2),cxysmin(2))
         call drwabs(cxyend(2),cxysmin(2))
         xyext= real(cxysmin(2))
         do 100, i=1,cxytics(2)
          if (cxymbeg(2) .ne. cxymend(2)) then ! Zeichnen Minor Ticmarks
           mlim= cxymtcs(2)-1
           xyextm= xyext
110        continue ! repeat...
           if (mlim.gt.0) then ! ...until mlim <= 0
            xyextm= xyextm+tmntvl
            call MOVABS (cxymbeg(2), nint(xyextm))
            call DRWABS (cxymend(2), nint(xyextm))
            mlim=mlim-1
            goto 110
           else if (mlim. lt. 0) then
            call LOGTIX (2,xyext,tintvl,cxymbeg(2),cxymend(2))
           end if
          end if
          xyext= xyext+tintvl
          call MOVABS (cxybeg(2), nint(xyext))
          call DRWABS (cxyend(2), nint(xyext))
100      continue
        end if ! Labtyp=6: Monate
       end if ! Ende Zeichnen Ticmarks
      end if ! Ende Zeichnen der Achse

      if (cxyfrm(1) .ne. 0) then ! Zeichnen der x-Achse
       i= min0(cxysmin(2),cxysmax(2)) + cxyloc(1)
       call MOVABS (cxysmin(1), i)
       call DRWABS (cxysmax(1), i)
       if (cxybeg(1) .ne. cxyend(1)) then ! Zeichnen y-Ticmarks
        i= cxylab(1) ! Labeltyp
        if (i .eq. 1) i= cxytype(1) ! =1: Typ entsprechend Daten
        if (i .ne. 6) then ! =6 (Monate): Tics durch GLINE zeichnen lassen
         if(cxytics(1) .ne. 0) then
          tintvl= real(cxysmax(1)-cxysmin(1)) / real( cxytics(1))
         end if
         if (cxymtcs(1) .gt. 0) tmntvl= tintvl / real(cxymtcs(1))
         call movabs(cxysmin(1), cxybeg(1))
         call drwabs(cxysmin(1), cxyend(1))
         xyext= real(cxysmin(1))
         do 120, i=1,cxytics(1)
          if (cxymbeg(1) .ne. cxymend(1)) then ! Zeichnen Minor Ticmarks
           mlim= cxymtcs(1)-1
           xyextm= xyext
130        continue ! repeat...
           if (mlim.gt.0) then ! ...until mlim <= 0
            xyextm= xyextm+tmntvl
            call MOVABS (nint(xyextm), cxymbeg(1))
            call DRWABS (nint(xyextm), cxymend(1))
            mlim=mlim-1
            goto 130
           else if (mlim. lt. 0) then
            call LOGTIX (1,xyext,tintvl,cxymbeg(1),cxymend(1))
           end if
          end if
          xyext= xyext+tintvl
          call MOVABS (nint(xyext), cxybeg(1))
          call DRWABS (nint(xyext), cxyend(1))
120      continue
        end if ! Labtyp=6: Monate
       end if ! Ende Zeichnen Ticmarks
      end if ! Ende Zeichnen der Achse
      return
      end



      subroutine LOGTIX (nbase,start,tintvl,mstart,mend)
      implicit none
      integer nbase,mstart,mend
      real start, tintvl
      integer i, logtic, ihorz, ivert, idx,idy
      character*1 loglab
      include 'G2dAG2.fd'

      call CSIZE (ihorz,ivert)
      do 100 i=2,9
       write (unit=loglab, fmt='(i1)') i ! Unicodefaehig durch Compilerfeature
       logtic= nint(log10(real(i))*tintvl + start)
       if (nbase .eq. 1) then ! x-Achse
        idx=  -ihorz/3
        if  (mstart .gt. mend) then
         idy= ivert
        else
         idy= -ivert
        end if
        call MOVABS (logtic,mend)
        call DRWABS (logtic,mstart)
        if (cxymtcs(nBase) .eq. -2) then ! numerisches Ticmarklabel
         call MOVREL (idx,idy)
         call TOUTSTC (loglab)
        end if

       else if (nbase .eq. 2) then ! y-Achse
        if  (mstart .gt. mend) then
         idx= ihorz
        else
         idx= -ihorz
        end if
        idy=  -ivert / 3
        call MOVABS (mend,logtic)
        call DRWABS (mstart,logtic)
       end if

       if (cxymtcs(nBase) .eq. -2) then ! numerisches Ticmarklabel
        call movrel (idx,idy)
        call TOUTSTC (loglab)
       end if
100   continue
      return
      end



      subroutine TSET (nbase)
      implicit none
      integer nbase
      integer IOTHER
      integer otherbase, near, nfar, newloc, nlen
      include 'G2dAG2.fd'

      otherbase= IOTHER (nbase)
      near= min0 (cxysmin(otherbase), cxysmax(otherbase))
      nfar= max0 (cxysmin(otherbase), cxysmax(otherbase))
      newloc= near + cxyloc(nbase)
      if (cxyfrm(nbase) .ne. 1) then
       if (newloc .lt. ((nfar+near)/2)) then
        nlen= cxylen(nbase)
       else
        nlen= -cxylen(nbase)
        nfar= near
       end if
       call TSET2 (newloc,nfar,nlen,cxyfrm(nbase),
     1                              cxybeg(nbase),cxyend(nbase))
      else
       cxybeg(nbase)= 0
       cxyend(nbase)= 0
      end if

      if ((cxymfrm(nbase) .ne. 1) .and. (cxymtcs(nbase) .ne. 0)) then
       nlen= nlen / 2
       call TSET2 (newloc,nfar,nlen,cxymfrm(nbase),
     1                              cxymbeg(nbase),cxymend(nbase))
      else
       cxymbeg(nbase)= 0
       cxymend(nbase)= 0
      end if
      return
      end



      subroutine TSET2 (newloc,nfar,nlen,nfrm,kstart,kend)
      implicit none
      integer newloc,nfar,nlen,nfrm,kstart,kend

      if (nfrm .eq. 3 .or. nfrm .eq. 6) then
       kstart= newloc
      else
       kstart=newloc-nlen
      end if
      if (kstart .lt. 0) then
       kstart= 0
      else if (kend .gt. 1023) then
       kstart= 1023
      end if

      if (nfrm .eq. 2) then
       kend= newloc
      else if (nfrm .eq. 5 .or. nfrm .eq. 6) then
       kend = nfar
      else
       kend=newloc+nlen
      end if
      if (kend .lt. 0) then
       kend= 0
      else if (kend .gt. 1023) then
       kend= 1023
      end if
      return
      end



      subroutine MONPOS (nbase,iy1,dpos, spos)
      implicit none
      integer nbase, iy1, spos
      integer iy,idays,iubgc1
      real dpos

      call YMDYD (iy,idays,iy1, nint(dpos)+1,1)
      call IUBGC (iy,idays, iubgc1)
      call GLINE (nbase, real(iubgc1), spos)
      return
      end



      subroutine GLINE (nbase,datapt,spos)
      implicit none
      integer nbase, spos
      real datapt
      integer i
      include 'G2dAG2.fd'

      if (nbase .eq. 1) then ! x-Achsengrid
       call WINCOT (datapt,1., spos,i)
       if (iabs(cxyend(1)-cxybeg(1)) .ge. 2) then
        call movabs(spos,cxybeg(1))
        call drwabs(spos,cxyend(1))
       end if
      else ! y-Achsengrid
       call wincot (1.,datapt, i,spos)
       if (iabs(cxyend(2)-cxybeg(2)) .ge. 2) then
        call movabs(cxybeg(2),spos)
        call drwabs(cxyend(2),spos)
       end if
      end if
      return
      end



C Label

      subroutine LABEL (nbase)
      implicit none
      integer nbase
      logical even, stag
      integer i, icv, igap, iquadrant, labtyp, ilim, iposflag, ioff, iy
      integer ispos,isintv, iyear
      integer level1, level2
      real fnum, fac, dpos, dintv
      character *(255) labstr
      integer IOTHER
      include 'G2dAG2.fd'

      labtyp= cxylab(nbase)
      if(labtyp .eq. 1) labtyp= cxytype(nbase) ! LabTyp=1: = dataType
      if (labtyp .eq. 0) return ! LabTyp=0: keine Label

      fac= 10.**(-cxyepon(nbase))

      dintv= real(cxystep(nbase)) / real(cxytics(nbase)) ! Zwischenergebnis
      isintv= nint(real(cxysmax(nbase)-cxysmin(nbase)) * dintv)
      dintv= (cxyamax(nbase)-cxyamin(nbase)) * dintv

      call csize (i,icv) ! nur icv = vertikale Hoehe benoetigt
      igap= icv / 3
      if (nbase.eq.1) igap= 2*igap
      if (iabs (cxysmax(IOTHER(nbase))-cxysmin(IOTHER(nbase)))
     1                                    .gt. 2* cxyloc(nbase)) then
       iQuadrant= -1 ! untere Haelfte
      else
       iQuadrant= +1
      end if
      level1= min0(cxysmax (IOTHER(nbase)),cxysmin (IOTHER(nbase)))
     1                              - (igap-icv/3 ) + cxyloc(nbase)
     2                       + isign(igap+cxylen (nbase),iQuadrant)
      level2= level1 + isign(icv+igap, iquadrant)

      if (nbase .eq. 1) then ! Label links/zentriert/rechts?
       iPosFlag= 0 ! x-Achse: zentriert
      else
       iPosFlag= -iQuadrant
      end if

      stag= cxystag (nbase) .eq. 2 ! Verwendung in Schleife
      even= .false.
      ilim= cxytics(nbase) + 1

      dpos= cxyamin (nbase)
      ispos= cxysmin (nbase)

      if (iabs(labtyp) .ge. 3 .and. iabs(labtyp) .le. 8) then ! Kalenderdaten
       call OUBGC (iYear,i,ifix(cxydmin(nbase))) ! i: Tag nicht benoetigt
       dpos= dpos+dintv ! 1. Tic ungelabelt
       ispos= ispos+isintv
       ilim=ilim-1
       if (nbase .eq. 1) iPosFlag= 1 ! x-Achse Kalender: rechtsbuendig
      end if

      do 100 i=1,ilim, cxystep(nbase)
       if ((labtyp .le. 2) .or. (labtyp .ge. 8)) then
        fnum= dpos
       else ! Kalendertyp ohne Jahr
        if (labtyp.eq.3) then ! Tage
         fnum= 7.
        else if (labtyp.eq.4) then ! Wochen
         fnum= 52.
        else if (labtyp.eq.5) then ! Periods
         fnum= 13.
        else if (labtyp.eq.6) then ! Monate
         fnum= 12.
        else if (labtyp.eq.7) then ! Quartal
         fnum= 4.
        end if ! Jahr wird wie linear behandelt
        fnum= amod(dpos-1.,fnum)+1.
       end if

       if (labtyp .lt. 0) then
        call USESETC (fnum, cxywdth(nbase), nbase, labstr)
       else if ((labtyp .eq. 6) .OR. (labtyp .eq. 3)) then
        call ALFSETC (fnum, labtyp, labstr)
        if (cxywdth(nbase) .lt. len(labstr)) then
         labstr(cxywdth(nbase)+1:cxywdth(nbase)+1)= char(0)
        end if
        if (labtyp .eq. 6) call MONPOS (nbase,iYear,dpos,ispos)
       else
        call NUMSETC (fnum*fac,cxywdth(nbase),nbase,labstr)
       end if
       call JUSTERC (labstr, iPosFlag, iOff)

       if (nbase .eq. 1) then ! x-Achse
        iy= level1
        if(stag .and. even) iy= level2
        even= .not. even
        call NOTATEC (ispos+ioff,iy, labstr)
       else ! y-Achse
        call NOTATEC (level1+ioff,ispos-igap,labstr)
       end if
       dpos= dpos+dintv
       ispos= ispos+isintv
100   continue ! end do

      if ((labtyp .ne. 2) .and. (cxyetyp(2) .ge. 0)) then ! nicht logarithm.
       if (nbase .eq. 1) then ! x-Achse
        if (stag) level2= level2 + isign (icv+igap,iquadrant)
        i=(cxysmin (nbase)+cxysmax (nbase))/2.
        iy=level2
       else
        i= level1
        iy= max0(cxysmin (nbase),cxysmax (nbase)) +icv+igap
       end if
       call REMLAB (nbase,cxyloc(nbase),labtyp,i,iy)
      end if
      return
      end



      subroutine NUMSETC (fnum,iwidth,nbase, outstr)
      implicit none
      real fnum
      integer iwidth,nbase
      character outstr *(*)
      integer iexp
      include 'G2dAG2.fd'

      if (cxytype(nbase) .eq. 2) then
       if (fnum .gt. 0.) then
        iexp= fnum + .00005
       else if (fnum .lt. 0.) then
        iexp= fnum - .00005
       else
        iexp= 0
       end if
       call EXPOUTC (nbase,iexp, outstr)
      else if ((cxytype(nbase).eq.1) .and. (cxydec(nbase).gt.0)) then
       call FFORMC (fnum,iwidth, cxydec(nbase), outstr)
      else
       call IFORMC (fnum,iwidth, outstr)
      end if
      return
      end



      subroutine IFORMC (fnum,iwidth, outstr)
      implicit none
      real fnum
      integer iwidth
      character outstr *(*)
      character fmtstr *(11)

      if (iwidth .le. 0) then ! iwidth=0: ohne Label
       outstr= char(0)
       return
      end if

      if (iwidth .gt. 99) goto 200 ! Errorhandler
      write (unit=fmtstr,fmt=100, err=200) iwidth
      if (len(outstr) .gt. iwidth) then
       write (unit= outstr, fmt=fmtstr, err=200) nint(fnum),0 ! 0: End of String
      else
       write (unit= outstr, fmt=fmtstr, err=200) nint(fnum) ! evtl. ohne EoS?
      end if

      return

200   continue ! Error Handler
      outstr= '???'
      if (iwidth.lt.len(outstr)) outstr(iwidth+1:iwidth+1)= char(0)
      return

100   format ('(SS,I' ,I2.2, ',A1)')
      end



      subroutine FFORMC (fnum,iwidth,idec, outstr)
      implicit none
      real fnum
      integer iwidth,idec
      character outstr *(*)
      integer nDgtM
      real fa
      include 'G2dAG2.fd'

      nDgtM= iwidth-idec
      if (fnum .ge. 0.) then
       nDgtM= nDgtM -1  ! Ziffern Mantisse
      else
       nDgtM= nDgtM-2   ! 1 Ziffer Vorzeichen
      end if
      fa= abs(fnum) ! Skalierung mindestens 2 signfikante Stellen: .1*abs(fnum)

      if ( ((fa .lt. 10./cinfin) .or. (fa .gt. .1**idec))
     1                        .and.(fa .lt. 10.**nDgtM)) then
       call FONLYC (fnum,iwidth,idec, outstr)
      else
       call EFORMC (fnum,iwidth,idec, outstr)
      end if
      return
      end



      subroutine FONLYC (fnum,iwidth,idec, outstr)
      implicit none
      real fnum
      integer iwidth,idec
      character outstr *(*)
      character fmtstr *(14)

      if (iwidth .le. 0) then ! iwidth=0: ohne Label
       outstr= char(0)
       return
      end if

      if ((idec .gt. iwidth-1) .or. (iwidth .gt. 99)) goto 200 ! Errorhandler
      write (unit=fmtstr,fmt=100, err=200) iwidth,idec
      if (len(outstr) .gt. iwidth) then
       write (unit= outstr, fmt=fmtstr, err=200) fnum,0 ! 0: End of String
      else
       write (unit= outstr, fmt=fmtstr, err=200) fnum ! evtl. ohne EoS?
      end if
      return

200   continue ! Error Handler
      outstr= '???'
      if (iwidth.lt.len(outstr)) outstr(iwidth+1:iwidth+1)= char(0)
      return

100   format ('(SS,F' ,I2.2,'.', I2.2,',A1)')
      end



      subroutine EFORMC (fnum,iwidth,idec, outstr)
      implicit none
      real fnum
      integer iwidth,idec
      character outstr *(*)
      integer iexpon
      character fmtstr *(18)

      if (iwidth .le. 0) then ! iwidth=0: ohne Label
       outstr= char(0)
       return
      end if

      call ESPLIT (fnum,iwidth,idec,iexpon)
      if ((idec .gt. iwidth-7) .or. (iwidth .gt. 99)) goto 200 ! Errorhandler
      write (unit=fmtstr,fmt=100, err=200) iwidth-idec-6,iwidth,iwidth-7
      if (len(outstr) .gt. iwidth) then
       write (unit= outstr, fmt=fmtstr, err=200) fnum,0 ! 0: End of String
      else
       write (unit= outstr, fmt=fmtstr, err=200) fnum ! evtl. ohne EoS?
      end if
      return

200   continue ! Error Handler
      outstr= '???'
      if (iwidth.lt.len(outstr)) outstr(iwidth+1:iwidth+1)= char(0)
      return

100   format ('(SS,' ,I2.2,'P,E' ,I2.2,'.', I2.2,',A1)')
      end



      subroutine ESPLIT (fnum,iwidth,idec,iexpon)
      implicit none
      real fnum
      integer iwidth,idec,iexpon
      real fabs
      include 'G2dAG2.fd'

      fabs= abs(fnum)
      if (fabs .ge. 1.) then
       iexpon= ifix( alog10(fabs)+1.000005) - iwidth+idec+6 ! 6: Vorz.-Pkt-Exp(4)
      else if (fabs .ge. 10./cinfin) then
       iexpon= alog10(fabs)
      else
       iexpon= -alog10(cinfin)
      end if
      return
      end



      subroutine EXPOUTC (nbase,iexp, outstr)
      implicit none
      integer nbase,iexp, i, iL, nexp
      character outstr *(*), tmpstr *(4)
      include 'G2dAG2.fd'

      iL= len(outstr)
      nexp= abs(iexp)

      if ( (cxyetyp(nbase).eq.2) .and. (iL.gt. 5)
     1             .and. (mod(nexp,3) .eq. 0)
     2             .and. (iexp.ge.1)  .and. (iexp.le.9) ) then ! MMMs
       do 20 i=3,nexp,3
        outstr(i/3:i/3)= 'M'
20     continue
       outstr(nexp/3+1:)= char(39) // 'S' // char(0)

      else if ( (cxyetyp(nbase).eq.3) .and. (iL.gt.17)
     1                    .and. (iexp.ge.1) .and. (iexp.le.6)) then ! TENS
       if (nexp .eq. 1) then
        outstr= 'TENS' // char(0)
       else if (nexp .eq. 2) then
        outstr= 'HUNDREDS' // char(0)
       else if (nexp .eq. 3) then
        outstr= 'THOUSANDS' // char(0)
       else if (nexp .eq. 4) then
        outstr= 'TEN THOUSANDS' // char(0)
       else if (nexp .eq. 5) then
        outstr= 'HUNDRED THOUSANDS' // char(0)
       else if (nexp .eq. 6) then
        outstr= 'MILLIONS' // char(0)
       end if
      else if( (cxyetyp(nbase).eq.4) ! 10000
     1    .and. (iexp.ge.1)  .and. (iexp.le.9)
     2                   .and. (iL.ge.nexp+2)) then
       do 30 i=2,nexp+1
        outstr(i:i)= '0'
 30    continue
       outstr(1:1)= '1'
       outstr(nexp+2:)= char(0)

      else if (iL .gt. 7) then ! Default: Superscript EXP
       if (iexp .ne. 1) then
        if (nexp .lt. 10) then
         i=1
        else
         i=2
        end if
        if (iexp .lt. 0) then
         i= i+1
        end if
        call IFORMC (real(iexp), i, tmpstr)
       else
        tmpstr= char(0) ! 10 wird ohne Exponenten 1 ausgegeben
       end if
       if (iexp .ne. 0) then
        if (cxytype(nbase) .ne. 2) then
         outstr(1:1)= 'x'
         i= 2
        else
         i= 1
        end if
        outstr(i:)= '10' // char(1) ! Index UP
        outstr(i+3:)= tmpstr ! char(0) wird bei IFORMC angehaengt
       else
        outstr(1:)= '1' // char(0) ! 1 wird nicht als 10**0 ausgegeben
       end if
      else ! outstr zu kurz
       outstr= '???'
      end if

      return
      end



      subroutine ALFSETC (fnum, labtyp, string)
      implicit none
      integer inum, labtyp
      real fnum
      character *(*) string

      inum= fnum + .001 ! truncate real to integer
      if (labtyp .eq. 3) then ! Tage
       if ((inum .eq. 0) .or. (inum .eq. 7)) then
        string= 'MONDAY' // char(0)
       else if (inum .eq. 1) then
        string= 'TUESDAY' // char(0)
       else if (inum .eq. 2) then
        string= 'WEDNESDAY' // char(0)
       else if (inum .eq. 3) then
        string= 'THURSDAY' // char(0)
       else if (inum .eq. 4) then
        string= 'FRIDAY' // char(0)
       else if (inum .eq. 5) then
        string= 'SATURDAY' // char(0)
       else if (inum .eq. 6) then
        string= 'SUNDAY' // char(0)
       end if
      else if (labtyp .eq. 6) then ! Monate
       if (inum .eq. 1) then
        string= 'JANUARY' // char(0)
       else if (inum .eq. 2) then
        string= 'FEBRUARY' // char(0)
       else if (inum .eq. 3) then
        string= 'MARCH' // char(0)
       else if (inum .eq. 4) then
        string= 'APRIL' // char(0)
       else if (inum .eq. 5) then
        string= 'MAY' // char(0)
       else if (inum .eq. 6) then
        string= 'JUNE' // char(0)
       else if (inum .eq. 7) then
        string= 'JULY' // char(0)
       else if (inum .eq. 8) then
        string= 'AUGUST' // char(0)
       else if (inum .eq. 9) then
        string= 'SEPTEMBER' // char(0)
       else if (inum .eq. 10) then
        string= 'OCTOBER' // char(0)
       else if (inum .eq. 11) then
        string= 'NOVEMBER' // char(0)
       else if (inum .eq. 12) then
        string= 'DECEMBER' // char(0)
       end if
      end if
      return
      end



      subroutine NOTATEC (ix,iy, string)
      implicit none
      integer ix, iy
      character *(*) string
      integer i, iv, is
      integer ISTRINGLEN

      call csize(i,iv)        ! nur iv benoetigt
      call movabs(ix,iy)

      is= 1
      do 100 i=1, ISTRINGLEN (string)
       if (string(i:i) .lt. char(31) ) then
        if (i.gt.is) call TOUTSTC (string(is:i-is))
        if (string(i:i) .eq. char(1)) call MOVREL (0, iv/2)  ! Hochindex
        if (string(i:i) .eq. char(2)) call MOVREL (0, -iv/2) ! Index
        is= i+1
       end if
100   continue
      if (is .le. ISTRINGLEN(string)) call TOUTSTC (string(is:))
      return
      end



      subroutine VLABLC (string)
C
C  Sollte in das TCS verlagert werden, um vertikale Schrift zu erzeugen
C
      implicit none
      character string*(*)
      integer i, icy, ix,iy
      integer ISTRINGLEN

      if (ISTRINGLEN(string) .le. 0) return
      call csize (i,icy)
      call seeloc (ix,iy)
      do 100 i=1,ISTRINGLEN(string)
       iy= iy-icy
       if (iy .lt. 0) return
       call movabs (ix,iy)
       call toutpt (ichar(string(i:i)))
100   continue
      return
      end



      subroutine JUSTERC (string, iPosFlag, iOff)
      implicit none
      integer iPosFlag, iOff
      character string*(*)
      integer i, iLen, nCtrl
      integer ISTRINGLEN, LINWDT

      iLen= ISTRINGLEN(string)
      nCtrl= 0    ! Zaehlen der Ctrlcharacter
      do 100 i=1, iLen
       if (string(i:i) .lt. char(31) ) nCtrl= nCtrl+1
100   continue

      if (iPosFlag .lt. 0) then ! linksbuendig
       iOff= 0
      else ! rechtsbuendig und zentriert
       ioff= -LINWDT ((iLen-nCtrl)*8-2)/8       ! rechtsbuendig
       if (iPosFlag.eq.0) iOff= iOff / 2        ! zentriert
      end if

      return
      end



      subroutine WIDTH (nbase)
      implicit none
      integer nbase
      integer labtyp
      include 'G2dAG2.fd'

      labtyp= cxylab(nbase)
      if(labtyp .eq. 1) labtyp= cxytype(nbase) ! LabTyp=1: = dataType

      if ((cxywdth(nbase).ne.0) .and. (labtyp.ne.1)) return ! Manuelle Vorgabe nichtlinear

      if (labtyp.le.1) then ! lineare Achsen und anwenderdefinierte Label
       call LWIDTH (nbase)

      else if (labtyp .eq. 2) then ! logarithmische Achsen
       if (cxyetyp(nbase) .le. 1) then ! 10 mit Exponent
        cxywdth(nbase)= 6
       else if (cxyetyp(nbase) .eq. 2) then ! M, MM...
        cxywdth(nbase)= int(alog10(abs(cxydmax(nbase)))/3. ) + 6
       else if (cxyetyp(nbase) .eq. 3) then ! Ausgeschriebene Worte
        cxywdth(nbase)= 20
        cxystep(nbase)= 1
        cxystag(nbase)= 2
       else if (cxyetyp(nbase) .eq. 4) then ! 1 mit 0
        cxywdth(nbase)= max (abs(alog10(abs(cxydmin(nbase)))),
     1                       abs(alog10(abs(cxydmin(nbase)))) ) + 2
       end if

      else if (labtyp .gt. 2) then ! Kalenderachsen
       if ((labtyp .eq. 3) .or. (labtyp .eq. 6)) then ! Tage oder Monate
        cxywdth(nbase)= 9
       else
        cxywdth(nbase)= 4
       end if
      end if

      return
      end



      subroutine LWIDTH (nbase)
      implicit none
      integer nbase
      integer iadj, most, least, isign,iwidth, idelta, ndec, iexp
      real xmax
      real ROUNDD
      include 'G2dAG2.fd'

      iadj= 0
      xmax= amax1(abs(cxydmin(nbase)),abs(cxydmax(nbase)))
      if (xmax .gt. 1.) then
       most= int(alog10(xmax) + 1.00005) ! Position Most Significant Digit
       iadj= 1
      else if (xmax .eq. 1.) then
       most= 0
      else
       most= int(alog10(xmax) - 0.00005)
      end if

      ndec= cxydec(nbase)
      if (cxydec(nbase) .ne. 0) then ! Anzahl Dezimalstellen vorgegeben
       least= -ndec ! Entspricht Position LeastSignificant Digit
      else
       least= cxylsig(nbase)
      end if

      if (cxydmin(nbase) .lt. 0.) then
       isign=1    ! 1 Buchstabe Vorzeichen
      else
       isign=0
      end if

      if ((most .lt. 0) .or. (least .ge. 0)) then
       iwidth= max0(1,most)- min0(0,least) + isign
       if (most .lt. 0) iwidth= iwidth+1 ! 1 Dezimalpunkt
       if ((iwidth .gt. 5 ) .and. (cxyetyp(nbase) .ge. 0)) then
        if (cxyetyp(nbase).eq.2) then
         iexp= int( ROUNDD (real(most-iadj),3.))
        else
         iexp= int( ROUNDD (real(most-iadj),1.))
        end if
        iwidth= most-least+isign+ 2
        ndec= max0(0,iexp-least+iadj)
       else
        ndec= max(0,-least)
        iexp= 0
       end if
      else
       iexp= 0
       ndec= max(0,-least)
       iwidth= most-least+isign+1
       if (most .eq. 0) iwidth= iwidth+1 ! Einbezug fuehrende Null
      end if

      if ((cxywdth(nbase) .ne. 0).and.(cxywdth(nbase).lt. iwidth)) then
       idelta= iwidth - cxywdth(nbase) - ndec
       if ((ndec .gt. 0) .and. (idelta .lt. 1) ) then
        ndec= max0(0,-idelta)
        iwidth= cxywdth(nbase)
       else
        iexp= iexp+idelta
        if(ndec .gt. 0) iexp=iexp-1
        iwidth= cxywdth(nbase)
        ndec=0
       end if
      end if

      cxywdth(nbase)= iwidth
      cxydec(nbase)= ndec
      cxyepon(nbase)= iexp
      return
      end



      subroutine REMLAB (nbase,iloc,labtyp,ix,iy)
      implicit none
      integer nbase, iloc, labtyp, ix, iy
      integer iyear1,iday1, iyear2,iday2
      integer iyear,imon,iday, ioff, iposflag
      character label *(25)
      include 'G2dAG2.fd'

      if (iabs(labtyp) .eq. 1) then ! lineare Daten
       if (cxyepon(nbase) .eq. 0) return ! kein Exponent
       call EXPOUTC (nbase,cxyepon(nbase), label)
      else ! Kalenderdaten
       if ((labtyp .ge. 4) .and. (labtyp.ne.6)) then ! Wochen, Quartale, Jahre
        ioff= 4 ! Überlappung der Jahre vermeiden
       else
        ioff= 0
       end if
       call OUBGC (iyear1,iday1, nint(cxydmin(nbase))+ioff)
       call OUBGC (iyear2,iday2, nint(cxydmax(nbase))-ioff)
       if (iday2 .le. 1) iyear2=iyear2-1
       iday2=iday2-1
       call ydymd(iyear1,iday1,iyear,imon,iday)

       if (iabs(labtyp).eq. 3) then
        call IFORMC (real(iday), 2, label(1:2))
        label(3:3)= ' ' ! 'dd '
        call ALFSETC (real(imon), 6, label(4:6)) ! labtyp 6= Monate, Laenge 3
        label(7:7)= ' ' ! 'dd mmm '
        call IFORMC (real(iyear), 4, label(7:10)) ! 'dd mm yyyy'
        label(11:11)= char(0) ! evtl. Labelende
        if (iyear1 .lt. iyear2) then ! bei Bedarf Start und Endjahr
         label(11:11)= '-' ! 'dd mm yyyy-'
         call ydymd(iyear2,iday2,iyear,imon,iday)
         call IFORMC (real(iday), 2, label(12:13)) ! 'dd'
         label(14:14)= ' ' ! 'dd mm yyyy-dd '
         call ALFSETC (real(imon), 6, label(15:17)) ! 'dd mmm'
         label(18:18)= ' ' ! 'dd mm yyyy-dd mmm '
         call IFORMC (real(iyear), 4, label(19:22)) ! 'dd mm yyyy-'
         label(23:23)= char(0)
        end if
       else
        call IFORMC (real(iyear), 4, label(1:4)) ! 'yyyy'
        label(5:5)= char(0)
        if (iyear1 .lt. iyear2) then ! bei Bedarf Start und Endjahr
         label(5:5)= '-' ! 'yyyy-'
         call IFORMC (real(iyear2), 4, label(6:9)) ! 'yyyy-yyyy'
         label(10:10)= char(0)
        end if
       end if
      end if

      if ((nbase.eq.1) .or. (iLoc.eq.1)) then ! X-Achse oder y Zentriert
       iPosFlag= 0
      else
       iPosFlag= isign(1,1-iloc)
      end if
      call JUSTERC (label, iPosFlag, iOff)
      call NOTATEC (ix+ioff, iy,label)
      return
      end



      subroutine SPREAD (nbase)
      implicit none
      integer nbase
      integer ih, labtyp, iwidth, iMaxWid
      integer LINWDT
      include 'G2dAG2.fd'

      if (cxystag(nbase) .ne. 1) return

      labtyp= cxylab(nbase)
      if ((labtyp .eq. 1) .or. (labtyp .eq. 0)) labtyp= cxytype(nbase)

100   continue ! outer loop
       if (nbase .eq. 1) then ! x-Achse
        iwidth= LINWDT (cxywdth(nbase))
       else
        call csize(ih, iwidth)
       end if

       iMaxWid= iabs(cxysmax(nbase)-cxysmin(nbase))- 2*iwidth
       iMaxWid= iMaxWid* cxystep(nbase)* cxystag(nbase) / cxytics(nbase)

       cxystep(nbase)= 1
       cxystag(nbase)= 1

       if (iwidth .lt. iMaxWid) return ! exit loop

       if (nbase .eq. 1) then ! x-Achse
        cxystag(nbase)= 2
       else
        cxystep(nbase)= cxystep(nbase) + 1
       end if

110    continue ! inner loop
        if(iwidth .lt. iMaxWid) return ! exit loop
        if(cxystep(nbase) .gt. cxytics(nbase)) return ! exit loop
       if (labtyp .ne. 3 .and. labtyp .ne. 6) then ! cycle inner loop
        cxystep(nbase)= cxystep(nbase)+1
        goto 110
      else ! cycle outer loop
       if (cxywdth(nbase) .eq. 3) return
       cxywdth(nbase)=3
       goto 100
      end if ! cycle until force exit
      end



C
C  Tabellensuche und Rundungen
C

      real function FINDGE (val,tab,iN)
      implicit none
      integer iN
      real val, tab(1)

100   if (tab(in) .lt. val) goto 110 ! while
       iN= iN-1
       goto 100
110   continue ! endwhile

120   continue ! repeat
       iN= iN+1
      if (tab(iN) .lt. val) goto 120 ! end repeat
      FINDGE= tab(iN)
      return
      end



      real function FINDLE (val,tab,iN)
      implicit none
      integer iN
      real val, tab(1)
      real valeps

      valeps= val+ 1.e-7 ! Vergleich um 0 ermoeglichen (Rechengenauigkeit!)

100   if (tab(in) .le. valeps) goto 110 ! while
       iN= iN-1
       goto 100
110   continue ! endwhile

120   continue ! repeat
       iN= iN+1
      if (tab(iN) .lt. valeps) goto 120 ! end repeat
      FINDLE= tab(iN-1)
      return
      end



      integer function LOCGE (ival,itab,iN)
      implicit none
      integer ival, itab(1), iN

100   if (itab(iN) .lt. ival) goto 110 ! while
       iN= iN-1
       goto 100
110   continue ! endwhile

120   continue ! repeat
       iN= iN+1
      if (itab(iN) .lt. ival) goto 120 ! end repeat
      LOCGE= itab(iN)
      return
      end



      integer function LOCLE (ival,itab,iN)
      implicit none
      integer ival, itab(1), iN

100   if (itab(in) .le. ival) goto 110 ! while
       iN= iN-1
       goto 100
110   continue ! endwhile

120   continue ! repeat
       iN= iN+1
      if (itab(iN) .le. ival) goto 120 ! end repeat
      LOCLE= itab(iN-1)
      return
      end



      real function ROUNDD (value,finterval)
      implicit none
      real value,finterval
      integer ifrac
      real frac

      frac= value/finterval
      ifrac= int(frac)
      if (real(ifrac) .gt. frac) ifrac= ifrac-1 ! Abrunden bei frac neg.
      ROUNDD = real(ifrac) * finterval
      if (ROUNDD .gt. value) ROUNDD= value
      return
      end



      real function ROUNDU (value,finterval)
      implicit none
      real value,finterval
      integer ifrac
      real frac

      frac= value/finterval
      ifrac= int(frac)
      if (real(ifrac) .lt. frac) ifrac= ifrac+1 ! Aufrunden bei frac pos.
      ROUNDU = real(ifrac) * finterval
      if (ROUNDU .lt. value) ROUNDU= value
      return
      end



C
C  Generelle Manipulationen der Commonvariablen
C
      subroutine SAVCOM (Array)
      implicit none
      integer array(1)
      include 'G2dAG2.fd'

      integer i
      integer arr(1)
      equivalence (arr(1),cline)
      do 10 i=1,G2dAG2L
       array(i)= arr(i)
10    continue
      return
      end



      subroutine RESCOM (Array)
      implicit none
      integer array(1)
      include 'G2dAG2.fd'

      integer i
      integer arr(1)
      equivalence (arr(1),cline)
      do 10 i=1,G2dAG2L
       arr(i)= array(i)
10    continue
      return
      end



      integer function IOTHER (ipar)
      implicit none
      integer ipar

      if (mod(ipar,2) .eq. 1) then ! ungerader Parameter=x-Achse
       IOTHER= ipar+1
      else
       IOTHER= ipar-1
      end if
      return
      end
