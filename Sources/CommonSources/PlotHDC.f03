!> \file       PlotHDC.f03
!> \brief      Utility: Plot Journalfiles
!> \version    1.0-GCC
!> \author     (C) 2023 Dr.-Ing. Klaus Friedewald
!> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
!>
!> \~german
!> Hilfsprogramm zur Anzeige von Journal-Hardcopies von SDL2 und wX-Programmen.
!> Diese koennen dann ueber Cut/Paste in andere Windowsprogramme uebernommen werden.
!> Die Abfrage der Programmparameter erfolgt durch ISO-Fortran 2003 Intrinsics.
!> \note \verbatim
!>    Aufruf durch:
!>     $> plothdc FileName
!> \endverbatim
!>
!> \~english
!> Utility to draw journal-hardcopies from SDL2 and wX programs.
!> With cut/paste they could be used by other MS-win programs.
!> Program parameters are optained by calling ISO Fortran 2003 intrinsic procedures.
!> \note \verbatim
!>    Invoke by:
!>     $> plothdc FileName
!> \endverbatim
!> \~
!>
      program PlotHDC
      implicit none
      integer iTrimLen
      integer ipar
      character * 128 filnam

      call initt (0)
      ipar = COMMAND_ARGUMENT_COUNT() ! FTN03 Standard
	  call GET_COMMAND_ARGUMENT (1,filnam)
      if (ipar.gt.0) then
        call GetHDC (filnam(1:iTrimLen(filnam))//char(0))
      else
        call GraphicError (9, 'Please invoke by: PlotHDC FileName')
      end if
      call finitt
      end
