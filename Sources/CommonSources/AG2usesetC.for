C> \file    AG2usesetC.for
C> \brief   Graph2D: Dummy User Routine
C
C  Tektronix Advanced Graphics 2 - Version 2.0
C
C     User Subroutinen
C

      subroutine USESETC (fnum,iwidth, nbase, labstr)
      implicit none
      real fnum
      integer iwidth, nbase
      character *(*) labstr
      integer labeli(20)
      integer i, i1, iw, ISTRINGLEN

      iw= min (20, iwidth, ISTRINGLEN(labstr))
      call USESET (fnum,iw,nbase,labeli)

      i1= 0
      do 100 i=1,iw
       i1= i1+1
       labstr(i1:i1)= char(labeli(i))
100   continue
      if (i1 .lt. iw) labstr(i1+1:i1+1)= char(0)
      return
      end

