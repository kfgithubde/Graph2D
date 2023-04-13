C> \file       GetHDC.for
C> \brief      Restore Hardcopies
C> \version    1.2
C> \author     (C) 2023 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C> \~german
C> Einlesen und Zeichnen von Hardcopydateien\n
C> Verwendete temporaeres Ein/Ausgabeunit: 41. Falls bereits belegt, wird ein freier Kanal gesucht
C> \~english
C> Read and plot hardcopies\n
C> Temporary input unit: 41. If already used, an other channel will be searched.
C> \~
C

      logical function GetHDC (Filnam)
C> \param FilNam: Hardcopyfie
C> \result (optional) .true. -> Error
      include 'Tktrnx.fd'
      integer TCS_MESSAGELEN, iUnit
      parameter (TCS_MESSAGELEN=132)
      character *(*) Filnam
      logical iUnitUsed
      character *(TCS_MESSAGELEN+1) TxtString

      integer ios, iDash, iPrntLen, iActLen
      integer Action, i1, i2

      iUnit= 40
      GetHDC= .true.

 5    continue ! repeat
        iUnit= iUnit+1
        inquire (unit=iUnit, opened= iUnitUsed)
      if (iUnitUsed) goto 5

      open (iUnit,file=Filnam,status='old',iostat=ios,form='formatted')
      if (ios.ne.0) then
        call GraphicError (6, ' ')
        return
      end if

 10   continue ! repeat
        read (iUnit, fmt='(i2,1x,i4,1x,i3)', iostat=ios)Action, i1, i2
        if (ios.gt.0) then ! Error, not EOF
         call GraphicError (8, ' ')
         return
        end if
        if (Action.eq.1) then ! XACTION_INITT
          call DefaultColour()
          call erase ()
        else if (Action.eq.2) then ! XACTION_ERASE
          call erase ()
        else if (Action.eq.3) then ! XACTION_MOVABS
          call movabs (i1,i2)
        else if (Action.eq.4) then ! XACTION_DRWABS
          call drwabs (i1,i2)
        else if (Action.eq.5) then ! XACTION_DSHSTYLE
          iDash= i1
        else if (Action.eq.6) then ! XACTION_DSHABS
          call dshabs (i1,i2,iDash)
        else if (Action.eq.7) then ! XACTION_PNTABS
          call pntabs (i1,i2)
        else if (Action.eq.8) then ! XACTION_GTEXT
          iPrntLen= i1
          if (iPrntLen.gt.TCS_MESSAGELEN) iPrntLen= TCS_MESSAGELEN
          TxtString(1:1)= char(i2)
          if (iPrntLen.eq.1) then
            TxtString= TxtString (1:1) // char(0)
            call ToutstC (TxtString)
          else
            iActLen= 1
          end if
        else if (Action.eq.9) then ! XACTION_ASCII
          if (iActLen.lt.iPrntLen) then
            iActLen= iActLen+1
            TxtString(iActLen:iActLen)= char (i1)
          end if
          if (iActLen.lt.iPrntLen) then
            iActLen= iActLen+1
            TxtString(iActLen:iActLen)= char (i2)
          end if
          if (iActLen.ge.iPrntLen) then
            TxtString (iActLen+1:iActLen+1) = char(0)
            call ToutstC (TxtString)
          end if
        else if (Action.eq.10) then ! XACTION_BCKCOL
          call bckcol(i1)
        else if (Action.eq.11) then ! XACTION_LINCOL
          call lincol (i1)
        else if (Action.eq.12) then ! XACTION_TXTCOL
          call txtcol (i1)
        else if (Action.eq.13) then ! XACTION_FONTATTR
          if (i1.eq.0) call italir()
          if (i1.eq.1) call italic()
          if (i2.eq.0) call nrmsiz()
          if (i2.eq.1) call dblsiz()
        else if (Action.eq.14) then ! XACTION_NOOP
          continue
        else if (Action.eq.15) then ! XACTION_CLIP
          if (i1.eq.0) then ! clipping not active
            kminsx= 0
            kminsy= 0
            kmaxsx= 1023 ! TEK_XMAX
            kmaxsy= 780 ! TEK_YMAX
            call swind1(kminsx,kminsy,kmaxsx,kmaxsy) ! Set bool ClippingNotActive
          end if
        else if (Action.eq.16) then ! XACTION_CLIP1
          kminsx= i1
          kminsy= i2
          call swind1(kminsx,kminsy,kmaxsx,kmaxsy)
        else if (Action.eq.17) then ! XACTION_CLIP2
          kmaxsx= i1
          kmaxsy= i2
          call swind1(kminsx,kminsy,kmaxsx,kmaxsy)
        else ! unknown
          continue
        end if
      if (ios.eq.0) goto 10 ! until EOF

      close (iUnit)
      GetHDC= .false.
      return
      end
