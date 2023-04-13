C> \file       PlotHDC.for
C> \brief      Utility: Plot Journalfiles
C> \version    1.0-GCC
C> \author     (C) 2023 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> Hilfsprogramm zur Anzeige von Journal-Hardcopies von SDL2 und wX-Programmen.
C> Diese koennen dann ueber Cut/Paste in andere Windowsprogramme uebernommen werden.
C> Die Abfrage der Programmparameter erfolgt durch gfortran spezifische Erweiterungen.
C> \note \verbatim
C>    Aufruf durch:
C>     $> plothdc FileName
C> \endverbatim
C>
C> \~english
C> Utility to draw journal-hardcopies from SDL2 and wX programs.
C> With cut/paste they could be used by other MS-win programs.
C> Program parameters are optained by calling gfortran extensions.
C> \note \verbatim
C>    Invoke by:
C>     $> plothdc FileName
C> \endverbatim
C> \~
C>
      program PlotHDC
      implicit none
      integer iTrimLen
      integer ipar
      character * 128 filnam

      call initt (0)
      ipar = IARGC() ! Version for GCC compiler
	  call GETARG(1,filnam)

      if (ipar.gt.0) then
        call GetHDC (filnam(1:iTrimLen(filnam))//char(0))
      else
        call GraphicError (9, 'Please invoke by: PlotHDC FileName')
      end if
      call finitt
      end
