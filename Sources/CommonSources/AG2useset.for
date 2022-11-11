C> \file    AG2useset.for
C> \brief   Graph2D: Dummy User Routine
C
C  Tektronix Advanced Graphics 2 - Version 2.0
C
C     User Subroutinen
C

      subroutine USESET (fnum,iwidth,nbase,labeli)
      implicit none
      real fnum
      integer iwidth, nbase
      integer labeli(1)
      integer i

      do 100 i=1, iwidth
       labeli(i)= 32 ! Blank
100   continue
      return
      end

