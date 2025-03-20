!> \file       Graph2D.f08
!> \brief      wX Port: Fortran Interfacemodul
!> \version    1.1
!> \author     (C) 2025 Dr.-Ing. Klaus Friedewald
!> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
!>
!> \~german
!> Interfacemodul für Fortran 2003 Programme
!>
!> \~english
!> Interfacemodule for programs written in Fortran 2003
!> \~
!>

module Graph2D
    implicit none


! ***************** TCS/AG2 Routines with string parameters ******************


    interface
        subroutine winlbl (PloWinNam, StatWinNam, IniFilNam)
        character*(*) :: PloWinNam, StatWinNam, IniFilNam
        end subroutine winlbl
    end interface



    interface ! TCS: added character output
        subroutine toutstc (String)
        character*(*) :: String
        end subroutine toutstc
    end interface

    interface
        subroutine statst (String)
        character*(*) :: String
        end subroutine statst
    end interface

    interface
        subroutine graphicerror (i, String)
		integer i
        character*(*) :: String
        end subroutine graphicerror
    end interface



    interface ! TCS add-on
        function gethdc (Filnam)
        character*(*) :: Filnam
		logical       :: gethdc
        end function gethdc
    end interface



    interface ! AG2:  added character labeling  routines
        subroutine numsetc (fnum,iwidth,nbase, outstr)
		real          :: fnum
		integer       :: iwidth,nbase
        character*(*) :: outstr
        end subroutine numsetc
    end interface

    interface
        subroutine iformc (fnum,iwidth, outstr)
		real          :: fnum
		integer       :: iwidth
        character*(*) :: outstr
        end subroutine iformc
    end interface

    interface
        subroutine fformc (fnum,iwidth,idec, outstr)
		real          :: fnum
		integer       :: iwidth,idec
        character*(*) :: outstr
        end subroutine fformc
    end interface

    interface
        subroutine fonlyc (fnum,iwidth,idec, outstr)
		real          :: fnum
		integer       :: iwidth,idec
        character*(*) :: outstr
        end subroutine fonlyc
    end interface

    interface
        subroutine eformc (fnum,iwidth,idec, outstr)
		real          :: fnum
		integer       :: iwidth,idec
        character*(*) :: outstr
        end subroutine eformc
    end interface

    interface
        subroutine expoutc (nbase,iexp, outstr)
		integer       :: nbase,iexp
        character*(*) :: outstr
        end subroutine expoutc
    end interface

    interface
        subroutine alfsetc (fnum, labtyp, string)
		real          :: fnum
		integer       :: labtyp
        character*(*) :: string
        end subroutine alfsetc
    end interface

    interface
        subroutine notatec (ix,iy, string)
		integer       :: ix,iy
        character*(*) :: string
        end subroutine notatec
    end interface

    interface
        subroutine vlablc (string)
        character*(*) :: string
        end subroutine vlablc
    end interface

    interface
        subroutine justerc (string, iPosFlag, iOff)
		integer       :: iPosFlag, iOff
        character*(*) :: string
        end subroutine justerc
    end interface


! ************************** AG2 API *********************************


    interface ! System
        subroutine ag2lev (level)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: level (3)
        end subroutine ag2lev
    end interface

    interface
        subroutine  binitt ()
        end subroutine binitt
    end interface


    interface ! Table and data handling
        subroutine comdmp ()
        end subroutine comdmp
    end interface

    interface
        function comget (iPar)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: iPar
        real (c_float)  :: comget
        end function comget
    end interface

    interface
        subroutine comset (iPar, rPar)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: iPar
        real (c_float)  :: rPar
        end subroutine comset
    end interface

    interface
        function findge (v, t, i)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        real (c_float)  :: v, t
		integer (c_int) :: i
        real (c_float)  :: findge
        end function findge
    end interface

    interface
        function findle (v, t, i)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        real (c_float)  :: v, t
		integer (c_int) :: i
        real (c_float)  :: findle
        end function findle
    end interface

    interface
        function ibasec (i)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i, ibasec
        end function ibasec
    end interface

    interface
        function ibasex (i)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i, ibasex
        end function ibasex
    end interface

    interface
        function ibasey (i)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i, ibasey
        end function ibasey
    end interface

    interface
        subroutine iubgc (i1,i2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1, i2, i3
        end subroutine iubgc
    end interface

    interface
        subroutine oubgc (i1,i2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1, i2, i3
        end subroutine oubgc
    end interface

    interface
        function leap (i)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i, leap
        end function leap
    end interface

    interface
        function locge (i1, i2, i3)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i1, i2, i3
		integer (c_int) :: locge
        end function locge
    end interface

    interface
        function locle (i1, i2, i3)
        use, intrinsic  :: iso_c_binding, only: c_int
		integer (c_int) :: i1, i2, i3
		integer (c_int) :: locle
        end function locle
    end interface

    interface
        function roundd (f1,f2)
        use, intrinsic  :: iso_c_binding, only: c_float
        real (c_float)  :: f1, f2
        real (c_float)  :: roundd
        end function roundd
    end interface

    interface
        function roundu (f1,f2)
        use, intrinsic  :: iso_c_binding, only: c_float
        real (c_float)  :: f1, f2
        real (c_float)  :: roundu
        end function roundu
    end interface

    interface
        subroutine savcom (iArr)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iArr (*)
        end subroutine savcom
    end interface

    interface
        subroutine rescom (iArr)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iArr (*)
        end subroutine rescom
    end interface



    interface ! Axis and labeling
        subroutine alfset (f1, i1,i2,iString)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,iString(*)
		real (c_float)  :: f1
        end subroutine alfset
    end interface

    interface
        subroutine numset (f1, i1,i2,iString,i4)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,iString(*),i4
		real (c_float)  :: f1
        end subroutine numset
    end interface

    interface
        subroutine eform (f1, i1,i2,iString,i4)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,iString(*),i4
		real (c_float)  :: f1
        end subroutine eform
    end interface

    interface
        subroutine esplit (f1, i1,i2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,i3
		real (c_float)  :: f1
        end subroutine esplit
    end interface

    interface
        subroutine expout (i1,i2,iString,i4,i5)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,iString(*),i4,i5
        end subroutine expout
    end interface

    interface
        subroutine fform (f1, i1,i2,iString,i4)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,iString(*),i4
		real (c_float)  :: f1
        end subroutine fform
    end interface

    interface
        subroutine fonly (f1, i1,i2,iString,i4)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,iString(*),i4
		real (c_float)  :: f1
        end subroutine fonly
    end interface

    interface
        subroutine gline (i1,f2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1,i3
		real (c_float)  :: f2
        end subroutine gline
    end interface

    interface
        subroutine hlabel (iLen,iString)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iLen,iString(*)
        end subroutine hlabel
    end interface

    interface
        subroutine hstrin (iString)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iString(*)
        end subroutine hstrin
    end interface

    interface
        subroutine iform (i1,i2,iString,i4)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,iString(*),i4
        end subroutine iform
    end interface

    interface
        function iother (i)
        use, intrinsic           :: iso_c_binding, only: c_int
		integer (c_int)          :: i, iother
        end function iother
    end interface

    interface
        subroutine juster (iLen,iString,i3,i4,i5,i6)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iLen,iString(*),i3,i4,i5,i6
        end subroutine juster
    end interface

    interface
        subroutine lwidth (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine lwidth
    end interface

    interface
        subroutine monpos (i1,i2,f3,i4)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1,i2,i4
		real (c_float)  :: f3
        end subroutine monpos
    end interface

    interface
        subroutine notate (i1,i2,iLen,iString)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,iLen,iString(*)
        end subroutine notate
    end interface

    interface
        subroutine place (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine place
    end interface

    interface
        subroutine remlab (i1,i2,i3,i4,i5)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3,i4,i5
        end subroutine remlab
    end interface

    interface
        subroutine spread (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine spread
    end interface

    interface
        subroutine vlabel (iLen,iString)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iLen,iString(*)
        end subroutine vlabel
    end interface

    interface
        subroutine vstrin (iString)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: iString(*)
        end subroutine vstrin
    end interface

    interface
        subroutine ydymd (i1,i2,i3,i4,i5)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3,i4,i5
        end subroutine ydymd
    end interface

    interface
        subroutine ymdyd (i1,i2,i3,i4,i5)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3,i4,i5
        end subroutine ymdyd
    end interface



    interface !  Symbols
        subroutine bsyms (f1,f2, i3)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i3
		real (c_float)  :: f1,f2
        end subroutine bsyms
    end interface

    interface
        subroutine calpnt (fArr,i1)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1
		real (c_float)  :: fArr(*)
        end subroutine calpnt
    end interface

    interface
        subroutine filbox (i1,i2,i3,i4,i5,i6)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3,i4,i5,i6
        end subroutine filbox
    end interface

    interface
        subroutine grid ()
        end subroutine grid
    end interface

    interface
        subroutine keyset (fArr,i1)
        use, intrinsic  :: iso_c_binding, only: c_int, c_float
        integer (c_int) :: i1
		real (c_float)  :: fArr(*)
        end subroutine keyset
    end interface

    interface
        subroutine label (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine label
    end interface

    interface
        subroutine logtix (i1,f2,f3,i4,i5)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1,i4,i5
		real (c_float)  :: f2,f3
        end subroutine logtix
    end interface

    interface
        subroutine setwin ()
        end subroutine setwin
    end interface

    interface
        subroutine symout (i1,f2)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1
		real (c_float)  :: f2
        end subroutine symout
    end interface

    interface
        subroutine teksym (i1,f2)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1
		real (c_float)  :: f2
        end subroutine teksym
    end interface

    interface
        subroutine tset (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine tset
    end interface

    interface
        subroutine tset2 (i1,i2,i3,i4,i5,i6)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3,i4,i5,i6
        end subroutine tset2
    end interface

    interface
        subroutine width (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine width
    end interface



    interface ! Display
        subroutine bar (f1,f2,i1)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1
		real (c_float)  :: f1,f2
        end subroutine bar
    end interface

    interface
        subroutine calcon (f1,f2,i1,i2)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1,i2
		real (c_float)  :: f1,f2
        end subroutine calcon
    end interface

    interface
        subroutine cmnmx (fArr,f2,f3)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: fArr(*),f2,f3
        end subroutine cmnmx
    end interface

    interface
        subroutine coptim (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine coptim
    end interface

    interface
        subroutine cplot (xArr,yArr)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: xArr(*),yArr(*)
        end subroutine cplot
    end interface

    interface
        subroutine datget (fArr,i1,i2)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1,i2
		real (c_float)  :: fArr(*)
        end subroutine datget
    end interface

    interface
        subroutine dsplay (xArr,yArr)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: xArr(*),yArr(*)
        end subroutine dsplay
    end interface

    interface
        subroutine frame ()
        end subroutine frame
    end interface

    interface
        subroutine loptim (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine loptim
    end interface

    interface
        subroutine mnmx (fArr,f2,f3)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: fArr(*),f2,f3
        end subroutine mnmx
    end interface

    interface
        subroutine optim (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine optim
    end interface



    interface ! Curve plotting
        subroutine check (xArr,yArr)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: xArr(*),yArr(*)
        end subroutine check
    end interface

    interface
        subroutine dinitx ()
        end subroutine dinitx
    end interface

    interface
        subroutine dinity ()
        end subroutine dinity
    end interface

    interface
        subroutine dlimx (f1,f2)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: f1,f2
        end subroutine dlimx
    end interface

    interface
        subroutine dlimy (f1,f2)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: f1,f2
        end subroutine dlimy
    end interface

    interface
        subroutine hbarst (i1,i2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3
        end subroutine hbarst
    end interface

    interface
        subroutine infin (f1)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: f1
        end subroutine infin
    end interface

    interface
        subroutine line (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine line
    end interface

    interface
        subroutine npts (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine npts
    end interface

    interface
        subroutine rgchek (i1,f1)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1
		real (c_float)  :: f1
        end subroutine rgchek
    end interface

    interface
        subroutine sizel (f1)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: f1
        end subroutine sizel
    end interface

    interface
        subroutine sizes (f1)
        use, intrinsic  :: iso_c_binding, only: c_float
		real (c_float)  :: f1
        end subroutine sizes
    end interface

    interface
        subroutine slimx (i1,i2)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2
        end subroutine slimx
    end interface

    interface
        subroutine slimy (i1,i2)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2
        end subroutine slimy
    end interface

    interface
        subroutine stepl (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine stepl
    end interface

    interface
        subroutine steps (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine steps
    end interface

    interface
        subroutine symbl (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine symbl
    end interface

    interface
        subroutine typck (i1,f1)
        use, intrinsic  :: iso_c_binding, only: c_int,c_float
        integer (c_int) :: i1
		real (c_float)  :: f1
        end subroutine typck
    end interface

    interface
        subroutine vbarst (i1,i2,i3)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1,i2,i3
        end subroutine vbarst
    end interface

    interface
        subroutine xden (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xden
    end interface

    interface
        subroutine xetyp (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xetyp
    end interface

    interface
        subroutine xfrm (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xfrm
    end interface

    interface
        subroutine xlab (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xlab
    end interface

    interface
        subroutine xlen (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xlen
    end interface

    interface
        subroutine xloc (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xloc
    end interface

    interface
        subroutine xloctp (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xloctp
    end interface

    interface
        subroutine xmfrm (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xmfrm
    end interface

    interface
        subroutine xmtcs (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xmtcs
    end interface

    interface
        subroutine xneat (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xneat
    end interface

    interface
        subroutine xtics (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xtics
    end interface

    interface
        subroutine xtype (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xtype
    end interface

    interface
        subroutine xwdth (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xwdth
    end interface

    interface
        subroutine xzero (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine xzero
    end interface

    interface
        subroutine yden (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yden
    end interface

    interface
        subroutine yetyp (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yetyp
    end interface

    interface
        subroutine yfrm (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yfrm
    end interface

    interface
        subroutine ylab (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ylab
    end interface

    interface
        subroutine ylen (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ylen
    end interface

    interface
        subroutine yloc (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yloc
    end interface

    interface
        subroutine ylocrt (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ylocrt
    end interface

    interface
        subroutine ymfrm (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ymfrm
    end interface

    interface
        subroutine ymtcs (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ymtcs
    end interface

    interface
        subroutine yneat (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yneat
    end interface

    interface
        subroutine ytics (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ytics
    end interface

    interface
        subroutine ytype (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ytype
    end interface

    interface
        subroutine ywdth (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine ywdth
    end interface

    interface
        subroutine yzero (i1)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: i1
        end subroutine yzero
    end interface


! ************************** TCS API *********************************


    interface
        subroutine initt (i)
        use, intrinsic            :: iso_c_binding, only: c_int
        integer (c_int), optional :: i ! Dummy: Baudrate
        end subroutine initt
    end interface

    interface
        subroutine finitt (ix, iy) bind(C, name='finitt_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int),optional :: ix,iy ! Dummy: position of cursor at exit
        end subroutine finitt
    end interface

    interface
        subroutine erase () bind(C, name='erase_')
        end subroutine erase
    end interface

    interface
        subroutine iowait (i) bind(C, name='iowait_')
        use, intrinsic            :: iso_c_binding, only: c_int
        integer (c_int), optional :: i ! Dummy: Baudrate
        end subroutine iowait
    end interface

    interface
        subroutine lintrn ()
        end subroutine lintrn
    end interface



    interface
        subroutine tcslev (level)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: level (3)
        end subroutine tcslev
    end interface

    interface
        subroutine svstat (array) bind(C, name='svstat_')
        use, intrinsic  :: iso_c_binding, only: c_float
        real (c_float) :: array (*) ! see TCS User Manual p. 44
        end subroutine svstat
    end interface

    interface
        subroutine restat (array) bind(C, name='restat_')
        use, intrinsic  :: iso_c_binding, only: c_float
        real (c_float) :: array (*) ! see TCS User Manual p. 44
        end subroutine restat
    end interface

    interface
        function genflg (item)
        use, intrinsic           :: iso_c_binding, only: c_int
		integer (c_int)          :: item
        logical (c_int)          :: genflg ! see TCS System Manual p. 3-5
        end function genflg
    end interface



    interface
        subroutine hdcopy () bind(C, name='hdcopy_')
        end subroutine hdcopy
    end interface



    interface
        subroutine movrel (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine movrel
    end interface

    interface
        subroutine pntrel (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine pntrel
    end interface

    interface
        subroutine drwrel (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine drwrel
    end interface

    interface
        subroutine dshrel (ix, iy, iMask)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy, iMask
        end subroutine dshrel
    end interface



    interface
        subroutine movabs (ix, iy) bind(C, name='movabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine movabs
    end interface

    interface
        subroutine pntabs (ix, iy) bind(C, name='pntabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine pntabs
    end interface

    interface
        subroutine drwabs (ix, iy) bind(C, name='drwabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine drwabs
    end interface

    interface
        subroutine dshabs (ix, iy, iMask) bind(C, name='dshabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy, iMask
        end subroutine dshabs
    end interface



    interface
        subroutine movea (x, y)
        use, intrinsic          :: iso_c_binding, only: c_float
        real (c_float)          :: x, y
        end subroutine movea
    end interface

    interface
        subroutine pointa (x, y)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x, y
        end subroutine pointa
    end interface

    interface
        subroutine drawa (x, y)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x, y
        end subroutine drawa
    end interface

    interface
        subroutine dasha (x, y, iMask)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        real (c_float)           :: x, y
        integer (c_int)          :: iMask
        end subroutine dasha
    end interface



    interface
        subroutine mover (x, y)
        use, intrinsic          :: iso_c_binding, only: c_float
        real (c_float)          :: x, y
        end subroutine mover
    end interface

    interface
        subroutine pointr (x, y)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x, y
        end subroutine pointr
    end interface

    interface
        subroutine drawr (x, y)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x, y
        end subroutine drawr
    end interface

    interface
        subroutine dashr (x, y, iMask)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        real (c_float)           :: x, y
        integer (c_int)          :: iMask
        end subroutine dashr
    end interface



    interface
        subroutine toutpt (ic)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ic
        end subroutine toutpt
    end interface

    interface
        subroutine toutst (nchr, ic)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: nchr, ic(*)
        end subroutine toutst
    end interface

    interface
        subroutine nrmsiz () bind(C, name='nrmsiz_')
        end subroutine nrmsiz
    end interface

    interface
        subroutine dblsiz () bind(C, name='dblsiz_')
        end subroutine dblsiz
    end interface

    interface
        subroutine italic () bind(C, name='italic_')
        end subroutine italic
    end interface

    interface
        subroutine italir () bind(C, name='italir_')
        end subroutine italir
    end interface

    interface
        subroutine csize (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine csize
    end interface

    interface
        subroutine setmrg (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine setmrg
    end interface

    interface
        subroutine linhgt (n)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: n
        end subroutine linhgt
    end interface

    interface
        subroutine linwdt (n)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: n
        end subroutine linwdt
    end interface



    interface
        subroutine ancho (iC)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iC
        end subroutine ancho
    end interface

    interface
        subroutine anstr (n, iC)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: n, iC(*)
        end subroutine anstr
    end interface

    interface
        subroutine baksp ()
        end subroutine baksp
    end interface

    interface
        subroutine cartn ()
        end subroutine cartn
    end interface

    interface
        subroutine linef ()
        end subroutine linef
    end interface

    interface
        subroutine newlin ()
        end subroutine newlin
    end interface



    interface
        subroutine bell () bind(C, name='bell_')
        end subroutine bell
    end interface



    interface
        subroutine home ()
        end subroutine home
    end interface

    interface
        subroutine newpag ()
        end subroutine newpag
    end interface



    interface
        subroutine tinput (iC) bind(C, name='tinput_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iC
        end subroutine tinput
    end interface

    interface
        subroutine dcursr (iC, ix, iy) bind(C, name='dcursr_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iC, ix, iy
        end subroutine dcursr
    end interface

    interface
        subroutine vcursr (ic, x,y)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        integer (c_int)          :: ic
        real (c_float)           :: x, y
        end subroutine vcursr
    end interface

    interface
        subroutine seeloc (ix, iy)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine seeloc
    end interface



    interface
        subroutine swindo (ix,ilx, iy,ily)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix,ilx, iy,ily
        end subroutine swindo
    end interface

    interface
        subroutine twindo (ix1,ix2, iy1,iy2)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix1,ix2, iy1,iy2
        end subroutine twindo
    end interface

    interface
        subroutine dwindo (x1,x2, y1,y2)
        use, intrinsic        :: iso_c_binding, only: c_float
        real (c_float)        :: x1,x2, y1,y2
        end subroutine dwindo
    end interface

    interface
        subroutine vwindo (x,lx, y,ly)
        use, intrinsic        :: iso_c_binding, only: c_float
        real (c_float)        :: x,lx, y,ly
        end subroutine vwindo
    end interface

    interface
        subroutine rel2ab (x1,x2, y1,y2)
        use, intrinsic        :: iso_c_binding, only: c_float
        real (c_float)        :: x1,x2, y1,y2
        end subroutine rel2ab
    end interface

    interface
        subroutine wincot (x,y, ix,iy)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        integer (c_int)          :: ix,iy
        real (c_float)           :: x, y
        end subroutine wincot
    end interface

    interface
        subroutine revcot (ix,iy, x,y)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        integer (c_int)          :: ix,iy
        real (c_float)           :: x, y
        end subroutine revcot
    end interface

    interface
        subroutine rescal ()
        end subroutine rescal
    end interface

    interface
        subroutine logtrn (i)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: i
        end subroutine logtrn
    end interface

    interface
        subroutine rrotat (x)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x
        end subroutine rrotat
    end interface

    interface
        subroutine rscale (x)
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: x
        end subroutine rscale
    end interface


    interface
        subroutine seetrm (i1,i2, i3,i4)
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: i1,i2, i3,i4
        end subroutine seetrm
    end interface


    interface
        subroutine seetrn (x,y, i)
        use, intrinsic           :: iso_c_binding, only: c_int, c_float
        integer (c_int)          :: i
        real (c_float)           :: x, y
        end subroutine seetrn
    end interface



    interface
        subroutine alfmod ()
        end subroutine alfmod
    end interface

    interface
        subroutine alpha ()
        end subroutine alpha
    end interface

    interface
        subroutine anmode ()
        end subroutine anmode
    end interface

    interface
        subroutine pclipt ()
        end subroutine pclipt
    end interface


! ************************** AG2: extensions *********************************


    interface
        function ag2infin () bind(C, name='ag2infin_')
        use, intrinsic           :: iso_c_binding, only: c_float
        real (c_float)           :: ag2infin
        end function ag2infin
    end interface


! ************************** TCS: extensions *********************************


    interface
        subroutine bckcol (iCol) bind(C, name='bckcol_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iCol
        end subroutine bckcol
    end interface

    interface
        subroutine lincol (iCol) bind(C, name='lincol_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iCol
        end subroutine lincol
    end interface

    interface
        subroutine txtcol (iCol) bind(C, name='txtcol_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iCol
        end subroutine txtcol
    end interface

    interface
        subroutine DefaultColour () bind(C, name='defaultcolour_')
        end subroutine DefaultColour
    end interface


! ************************** String Utilities ********************************


    interface
      function LIB_MOVC3 (iByte, Source, Destination)
        integer       :: iByte
        character*(*) :: Source, Destination
      end function
    end interface

    interface
      function Substitute (Source, Destination, Old1, New1)
        character*(*) :: Source, Destination, Old1, New1
      end function
    end interface

    interface
      function iStringLen (string)
        integer iStringLen
        character*(*) :: string
      end function
    end interface

    interface
      function iTrimLen (string)
        integer iTrimLen
        character*(*) :: string
      end function
    end interface

    interface
      function PrintString (buf)
        character*(*) :: PrintString, buf
      end function
    end interface

end module Graph2D
