!> \file       Graph2D.f08
!> \brief      wX Port: Interfacemodul
!> \version    (2022,305,6)
!> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
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

module Graph2D
    implicit none

    interface
        subroutine tcslev (level)
        use, intrinsic  :: iso_c_binding, only: c_int
        integer (c_int) :: level (3)
        end subroutine tcslev
    end interface


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
        subroutine swind1 (ix1,iy1, ix2,iy2) bind(C, name='swind1_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix1,iy1, ix2,iy2
        end subroutine swind1
    end interface


    interface
        subroutine erase () bind(C, name='erase_')
        end subroutine erase
    end interface


    interface
        subroutine movabs (ix, iy) bind(C, name='movabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine movabs
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
        subroutine pntabs (ix, iy) bind(C, name='pntabs_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: ix, iy
        end subroutine pntabs
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
        subroutine bckcol (iCol) bind(C, name='bckcol_')
        use, intrinsic           :: iso_c_binding, only: c_int
        integer (c_int)          :: iCol
        end subroutine bckcol
    end interface


    interface
        subroutine DefaultColour () bind(C, name='defaultcolour_')
        end subroutine DefaultColour
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
        subroutine dblsiz () bind(C, name='dblsiz_')
        end subroutine dblsiz
    end interface


    interface
        subroutine bell () bind(C, name='bell_')
        end subroutine bell
    end interface


    interface
        subroutine hdcopy () bind(C, name='hdcopy_')
        end subroutine hdcopy
    end interface

end module Graph2D
