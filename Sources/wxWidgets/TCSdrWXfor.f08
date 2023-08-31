!> \file       TCSdrWXfor.f08
!> \brief      wX Port: High-Level Driver
!> \version    (2023,243,8)
!> \author     (C) 2023 Dr.-Ing. Klaus Friedewald
!> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
!>
!> \~german
!> wX-spezifische TCS-Routinen
!> \note \verbatim
!>    Erweiterungen gegenüber Tektronix:
!>     subroutine TOUTSTC (String): Ausgabe Fortran-String
!>     subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15)
!>     subroutine TXTCOL (iCol): Setzen Textfarbe
!>     subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar)
!>     subroutine DefaultColour: Wiederherstellung Defaultfarben
!> \endverbatim
!>
!>
!> \~english
!> wX specific subroutines
!> \note \verbatim
!>    Supplement to Tektronix:
!>     subroutine TOUTSTC (String): Ausgabe Fortran-String
!>     subroutine LINCOL (iCol): Setzen Linienfarbe (iCol=0..15)
!>     subroutine TXTCOL (iCol): Setzen Textfarbe
!>     subroutine BCKCOL (iCol): Hintergrundfarbe (nach ERASE sichtbar)
!>     subroutine DefaultColour: Wiederherstellung Defaultfarben
!> \endverbatim
!> \~
!>


! FTN 77 linkbare Unterprogramme / Wrapper

!
!  Ausgabe der Softwareversion
!

      subroutine TCSLEV(LEVEL)
      integer LEVEL(3)
      LEVEL(1)=2023     ! Aenderungsjahr
      LEVEL(2)= 243     ! Aenderungstag
      LEVEL(3)=   8     ! System= wX
      return
      end



!
!  Initialization
!

      subroutine winlbl (PloWinNam, StatWinNam, IniFilNam)
      use, intrinsic :: iso_c_binding
      implicit none

      character*(*) PloWinNam, StatWinNam, IniFilNam
      interface
        subroutine winlbl0 (PloWinNam0, StatWinNam0, IniFilNam0) bind(C, name='winlbl0')
        use, intrinsic                        :: iso_c_binding, only: c_char
        character(kind= c_char), dimension(*) :: PloWinNam0, StatWinNam0, IniFilNam0
        end subroutine winlbl0
      end interface

      call winlbl0 (PloWinNam//c_null_char, StatWinNam//c_null_char, IniFilNam//c_null_char)
      end



      subroutine INITT (iDummy)
      use, intrinsic :: iso_c_binding
      implicit none

      integer iDummy
      integer (c_intptr_t), parameter :: NULLPTR = 0
      interface
        subroutine initt1 (iMode, iParent, iFrame, iStatus) bind(C)
          use, intrinsic              :: iso_c_binding
          integer (c_int), value      :: iMode
          integer (c_intptr_t), value :: iParent, iFrame, iStatus
        end subroutine initt1
      end interface

      call initt1 (0, NULLPTR, NULLPTR, NULLPTR) ! 0 => no Parent Window
      return
      end



!
!  Relative drawing
!

      subroutine movrel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kbeamx + ix
      iyy= kbeamy + iy
      call movabs (ixx, iyy)
      return
      end



      subroutine pntrel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kbeamx + ix
      iyy= kbeamy + iy
      call pntabs (ixx, iyy)
      return
      end



      subroutine drwrel (iX, iY)
      include 'Tktrnx.fd'
      ixx= kbeamx + ix
      iyy= kbeamy + iy
      call drwabs (ixx, iyy)
      return
      end



      subroutine dshrel (iX, iY, iMask)
      include 'Tktrnx.fd'
      ixx= kbeamx + ix
      iyy= kbeamy + iy
      call dshabs (ixx, iyy, imask)
      return
      end



 !
 !  Ersatz SEELOC der CP/M-Version (wie MS Windows, DOS)
 !

       subroutine seeloc (IX,IY)
       include 'Tktrnx.fd'
       ix= kbeamx
       iy= kbeamy
       return
       end



!
!  Graphic text output
!

      subroutine ToutPt (iChr)
      use, intrinsic :: iso_c_binding
      implicit none
      integer iChr

      interface
        subroutine outgtext (strng) bind(C, name='outgtext_')
        use, intrinsic                        :: iso_c_binding, only: c_char
        character(kind= c_char), dimension(*) :: strng
        end subroutine outgtext
      end interface

      call outgtext (char(iChr)//c_null_char)
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
      implicit none

      character *(*) String
      interface
        subroutine outgtext (strng) bind(C, name='outgtext_')
        use, intrinsic                        :: iso_c_binding, only: c_char
        character(kind= c_char), dimension(*) :: strng
        end subroutine outgtext
      end interface

      call outgtext (String//char(0))
      return
      end



      subroutine CSIZE (ixlen,iylen)
      include 'Tktrnx.fd'
      ixlen= khorsz
      iylen= kversz
      return
      end



      subroutine statst (String)
      use, intrinsic :: iso_c_binding
      implicit none

      character *(*) String
      interface
        subroutine outtext (cString) bind(C, name='outtext_')
        use, intrinsic                        :: iso_c_binding, only: c_char
        character(kind= c_char), dimension(*) :: cString
        end subroutine outtext
      end interface

      call outtext (string//c_null_char)
      return
      end



      subroutine GraphicError (iErr,Mssg) ! Bis jetzt genutzt: TCSGraphicError in Cpp
      use, intrinsic :: iso_c_binding
      implicit none

      integer iErr
      character *(*) Mssg
      interface
        subroutine TCSGraphicError (i, cString) bind(C, name='TCSGraphicError')
        use, intrinsic                        :: iso_c_binding
        integer(kind=c_int), value            :: i
        character(kind= c_char), dimension(*) :: cString
        end subroutine TCSGraphicError
      end interface

      call TCSGraphicError (iErr,Mssg//c_null_char)
      return
      end



!
!>  Entry dummy routines
!
      subroutine  anmode
!> AlfMod
      entry       alfmod
!> pClipt
      entry       pclipt
!> alpha
      entry       alpha
      return
      end
