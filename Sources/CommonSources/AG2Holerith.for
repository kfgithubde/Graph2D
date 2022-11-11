C> \file       AG2Holerith.for
C> \version    2.2
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C> \~german
C> \brief   Graph2D: obsolete AG2 Routinen
C> \~english
C> \brief   Graph2D: deprecated AG2 routines
C> \~
C>
C> \~german
C>     Unterprogramme zur Behandlung von Holerithvariablen und direkter 
C>     Manipulation des Commonblocks
C>
C> \~english
C>     Compatibility routines dealing with holerith characters
C>     and direct manipulation of common variables.
C>
C
C
C  Tektronix Advanced Graphics 2 - Version 2.x
C
C     Optionale Unterprogramme
C

C
C Stringfunktionen fuer Holerithvariablen
C

      subroutine NOTATE (ix,iy,lenchr,iarray)
      implicit none
      integer ix,iy,lenchr, iarray(lenchr)
      integer i
      character *(255) buf

      do 100 i=1,lenchr
       buf(i:i)= char(iarray(i))
100   continue
      call NOTATEC (ix,iy,buf(1:lenchr))
      return
      end



      subroutine ALFSET (fnum,kwidth,labtyp,ilabel)
      implicit none
      integer kwidth,labtyp, ilabel(kwidth)
      real fnum
      integer i, buflen
      character *(255) buf
      integer ISTRINGLEN

      call ALFSETC (fnum, labtyp, buf)
      buflen= ISTRINGLEN (buf)
      do 100 i=1,kwidth
       if (i .le. buflen) then
        ilabel(i)= ichar(buf(i:i))
       else
        ilabel(i)= ichar(' ')
       end if
100   continue
      return
      end



      subroutine NUMSET (fnum,iwidth,nbase,ilabel,ifill)
      implicit none
      integer iwidth,nbase,ilabel(iwidth),ifill
      real fnum
      integer i, iLeadFill
      character *(255) buf
      integer ISTRINGLEN

      call NUMSETC (fnum,iwidth,nbase, buf)
      iLeadFill= max(0,iwidth-ISTRINGLEN (buf))
      do 100 i=1,iwidth
       ilabel(iLeadFill+i)= ichar(buf(i:i))
100   continue
      i=1 ! iLabel ist rechtsjustiert!
      if (i.gt.iLeadFill) goto 110 ! while
       ilabel(i)= ifill
       i= i+1
110   continue ! endwhile
      return
      end



      subroutine EXPOUT (nbase,iexp,ilabel,nchars,ifill)
      implicit none
      integer nbase,iexp, nchars, ilabel(nchars), ifill
      integer i, iLeadFill
      character *(255) buf
      integer ISTRINGLEN

      call EXPOUTC (nbase,iexp, buf(1:nchars))
      iLeadFill= max(0,nchars-ISTRINGLEN (buf))
      do 100 i=1,nchars
       ilabel(iLeadFill+i)= ichar(buf(i:i))
100   continue
      i=1 ! iLabel ist rechtsjustiert!
      if (i.gt.iLeadFill) goto 110 ! while
       ilabel(i)= ifill
       i= i+1
110   continue ! endwhile
      return
      end



      subroutine HSTRIN (iString)
      implicit none
      integer iString(2)
      call ANSTR (iString(1),iString(2))
      return
      end



      subroutine HLABEL (iLen, iString)
      implicit none
      integer iLen, iString(iLen)
      call ANSTR (iLen, iString)
      return
      end



      subroutine VSTRIN (iarray)
      implicit none
      integer iarray(2)
      call VLABEL (iarray(1),iarray(2))
      return
      end



      subroutine VLABEL (iLen,iString)
      implicit none
      integer iLen, iString(iLen)
      integer i
      character *(255) buf
      integer ISTRINGLEN
      do 100 i=1, iLen
       buf(i:i)= char(iString(i))
100   continue
      call VLABLC (buf(:iLen))
      return
      end



      subroutine JUSTER (iLen,iString,iposflag,ifill,lenchr, ioff)
      implicit none
      integer iLen,iString(iLen), iposflag,ifill, lenchr, ioff
      integer i
      character *(255) buf

      lenchr= 0
      do 100 i=1, iLen
       if ( (i .gt. 1) .or. (iString(i) .ne. iFill) ) then ! Ueberlese Startfillchars
        lenchr= lenchr+1
        buf(lenchr:lenchr)= char (abs(iString(i))) ! Tek Index -1,-2 -> char(1),char(2)
       end if
100   continue
      call JUSTERC (buf, iPosFlag, iOff)
      return
      end



      subroutine EFORM (fnum,iwidth,idec,ilabel,ifill)
      implicit none
      integer iwidth,idec, ilabel(iwidth), ifill
      real fnum
      integer i
      character *(255) buf

      call EFORMC (fnum,iwidth,idec, buf)
      do 100 i=1,iwidth
       ilabel(i)= ichar(buf(i:i))
100   continue
      return
      end



      subroutine FFORM (fnum,iwidth,idec,ilabel,ifill)
      implicit none
      integer iwidth,idec, ilabel(255), ifill
      real fnum
      integer i
      character *(255) buf

      call FFORMC (fnum,iwidth,idec, buf)
      do 100 i=1,iwidth
       ilabel(i)= ichar(buf(i:i))
100   continue
      return
      end



      subroutine FONLY (fnum,iwidth,idec,ilabel,ifill)
      implicit none
      integer iwidth,idec, ilabel(iwidth), ifill
      real fnum
      integer i
      character *(255) buf

      call FONLYC (fnum,iwidth,idec, buf)
      do 100 i=1,iwidth
       ilabel(i)= ichar(buf(i:i))
100   continue
      return
      end



      subroutine IFORM (fnum,iwidth,ilabel,ifill)
      implicit none
      integer iwidth,idec, ilabel(iwidth), ifill
      real fnum
      integer i
      character *(255) buf

      call IFORMC (fnum,iwidth,idec, buf)
      do 100 i=1,iwidth
       ilabel(i)= ichar(buf(i:i))
100   continue
      return
      end



C
C  Direkte Manipulation des Commonblocks
C

      integer function IBASEC (iPar)
      implicit none
      integer iPar

      IBASEC= -1-ipar
      return
      end



      integer function IBASEX (ipar)
      implicit none
      integer ipar

      ibasex= 1 + 2*ipar
      return
      end



      integer function IBASEY (ipar)
      implicit none
      integer ipar

      ibasey= 2 + 2*ipar
      return
      end



      real function COMGET (iPar)
      implicit none
      integer iPar
      include 'G2dAG2.fd'

      integer iarr(1), iarr2(1)
      real arr(1), arr2(1)
      equivalence (iarr(1),cline), (iarr2(1),cxyneat)
      equivalence (arr(1),cline), (arr2(1),cxyneat)

      if ((iPar.lt.0) .and. (iPar.ge. -9))then
       if ((iPar .eq. -4) .or. (iPar .le. -8)) then
        COMGET= arr (-iPar)
       else
        COMGET= real(iarr(-iPar))
       end if
      else if ((iPar.gt.0) .and. (iPar.le.56)) then
       if ((iPar.le.22) .or. ((iPar .ge. 27).and.(iPar.le.52))) then
        COMGET= real (iarr2(iPar))
       else
        COMGET= arr2(iPar)
       end if
      end if
      return
      end



      subroutine COMSET (iPar,val)
      implicit none
      integer iPar
      real val
      include 'G2dAG2.fd'

      integer iarr(1), iarr2(1)
      real arr(1), arr2(1)
      equivalence (iarr(1),cline), (iarr2(1),cxyneat)
      equivalence (arr(1),cline), (arr2(1),cxyneat)

      if ((iPar.lt.0) .and. (iPar.ge. -9))then
       if ((iPar.eq.-4) .or. (iPar .le. -8)) then
        arr (-iPar)= val
       else
        iarr(-iPar)= int(val)
       end if
      else if ((iPar.gt.0) .and. (iPar.le.56)) then
       if ((iPar.le.22) .or. ((iPar .ge. 27).and.(iPar.le.52))) then
        iarr2(iPar)= int(val)
       else
        arr2(iPar)= val
       end if
      end if
      return
      end



      subroutine COMDMP
      implicit none
      integer i
      character *80 buf
      include 'G2dAG2.fd'

      call ERASE
      call HOME

      write (unit= buf,fmt=600, err=200) (cxyneat(i),i=1,2), cline
600   format (1x,' 0:  cxneat(1)=',L14,', (2)=',L14,',   cline=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=601, err=200) (cxyzero(i),i=1,2), csymbl
601   format (1x,' 1: cxyzero(1)=',L14,', (2)=',L14,',  csymbl=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=602, err=200) (cxyloc(i),i=1,2), csteps
602   format (1x,' 2:  cxyloc(1)=',I14,', (2)=',I14,',  csteps=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=603, err=200) (cxylab(i),i=1,2), cinfin
603   format (1x,' 3:  cxylab(1)=',I14,', (2)=',I14,',  cinfin=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=604, err=200) (cxyden(i),i=1,2), cnpts
604   format (1x,' 4:  cxyden(1)=',I14,', (2)=',I14,',   cnpts=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=605, err=200) (cxytics(i),i=1,2), cstepl
605   format (1x,' 5: cxytics(1)=',I14,', (2)=',I14,',  cstepl=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=606, err=200) (cxylen(i),i=1,2), cnumbr
606   format (1x,' 6:  cxylen(1)=',I14,', (2)=',I14,',  cnumbr=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=607, err=200) (cxyfrm(i),i=1,2), csizes
607   format (1x,' 7:  cxyfrm(1)=',I14,', (2)=',I14,',  csizes=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=608, err=200) (cxymtcs(i),i=1,2), csizel
608   format (1x,' 8: cxymtcs(1)=',I14,', (2)=',I14,',  csizel=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=609, err=200) (cxymfrm(i),i=1,2)
609   format (1x,' 9: cxymfrm(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=610, err=200) (cxydec(i),i=1,2)
610   format (1x,'10:  cxydec(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=611, err=200) (cxydmin(i),i=1,2)
611   format (1x,'11: cxydmin(1)=',E14.7,', (2)=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=612, err=200) (cxydmax(i),i=1,2)
612   format (1x,'12: cxydmax(1)=',E14.7,', (2)=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=613, err=200) (cxysmin(i),i=1,2)
613   format (1x,'13: cxysmin(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=614, err=200) (cxysmax(i),i=1,2)
614   format (1x,'14: cxysmax(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=615, err=200) (cxytype(i),i=1,2)
615   format (1x,'15: cxytype(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=616, err=200) (cxylsig(i),i=1,2)
616   format (1x,'16: cxylsig(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=617, err=200) (cxywdth(i),i=1,2)
617   format (1x,'17: cxywdth(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=618, err=200) (cxyepon(i),i=1,2)
618   format (1x,'18: cxyepon(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=619, err=200) (cxystep(i),i=1,2)
619   format (1x,'19: cxystep(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=620, err=200) (cxystag(i),i=1,2)
620   format (1x,'20: cxystag(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=621, err=200) (cxyetyp(i),i=1,2)
621   format (1x,'21: cxyetyp(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=622, err=200) (cxybeg(i),i=1,2)
622   format (1x,'22:  cxybeg(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=623, err=200) (cxyend(i),i=1,2)
623   format (1x,'23:  cxyend(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=624, err=200) (cxymbeg(i),i=1,2)
624   format (1x,'24: cxymbeg(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=625, err=200) (cxymend(i),i=1,2)
625   format (1x,'25: cxymend(1)=',I14,', (2)=',I14)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=626, err=200) (cxyamin(i),i=1,2)
626   format (1x,'26: cxyamin(1)=',E14.7,', (2)=',E14.7)
      call TOUTSTC (buf)
      call NEWLIN
      write (unit= buf,fmt=627, err=200) (cxyamax(i),i=1,2)
627   format (1x,'27: cxyamax(1)=',E14.7,', (2)=',E14.7)
      call TOUTSTC (buf)

      call GRAPHICERROR (11,char(0))
      call ERASE

200   continue
      return
      end
