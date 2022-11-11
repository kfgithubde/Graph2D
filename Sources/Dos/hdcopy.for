C> \file       hdcopy.for
C> \brief      DOS Port: Hardcopy
C> \version    1.35
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> TCS Bildschirmhardcopy
C> \~english
C> TCS Hardcopy from Screen
C> \~
C>
C
C CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   Changelog   CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  TCS Graphik Hardcopy für DOS
C
C     Version 1.1
C
C           subroutine HDCOPY: Erzeugt Windows-Bitmapfile der Form HDCxxx.bmp
C
C      21.11.01         Dr.-Ing. K. Friedewald
C
C     08.02.02 Version 1.2
C           Implementierung multilinguale Meldungen
C
C     31.05.02 Version 1.3:
C           Ersatz Hex-Konstante durch Dezimalkonstante zur Erzielung Kompatibilität mit WATCOM-Kompiler
C           INCLUDE Interface TCSDOSA.FI zur Anpassung an den WATCOM-Compiler
C
C     19.10.02 Version 1.34
C           Umbenennung TKTRNX.FOR in TKTRNX.FD zur Kompatibilität CP/M
C
C     06.02.03 Version 1.35
C           Interne Umbenennung lib$movc3 in lib_movc3
C
      INCLUDE 'FGRAPH.FI'
      INCLUDE 'TCSdDOSa.FI'

      subroutine HDCOPY
      include 'TKTRNX.FD'
      INCLUDE  'FGRAPH.FD'
      structure /BitMapFileHeader/
            integer*2   DatKennung  ! = $4d42
            integer*4   DatSize     ! Bilddateigroesse in Byte
            integer*2   Reserved1
            integer*2   Reserved2
            integer*4   GraphDatDst ! Entfernung BITMAPFILEHEADER zu Graphikdaten (Byte)
      end structure
      structure /BitMapInfoHeader/
            integer*4   BMpInfHdSiz ! Größe Bitmapinfoheader in Byte
            integer*4   PicWidth    ! Bildbreite Pixel, abgespeicherte Bytes durch 4 teilbar!
            integer*4   PicHeight   ! Bildhöhe in Pixel
            integer*2   iLayer      ! = 1
            integer*2   iBitPix     ! Bits per Pixel (1,4,8,24)
            integer*4   Kompr       ! Komprimierung =0(ohne),1(RLE8),2(RLE4)
            integer*4   PicSiz      ! Bildgroesse in Byte
            integer*4   HorPixDen   ! Horizontale Auflösung Pixel/ Meter
            integer*4   VerPixDen   ! Vertikale Auflösung Pixel/ Meter
            integer*4   iCol        ! Anzahl benutzte Farben
            integer*4   iVIPCol     ! Anzahl wichtige Farben =0(alle)
      end structure
      structure /RGBQuad/
            integer*1   Blue
            integer*1   Green
            integer*1   Red
            integer*1   Reserved    ! =0
      end structure
      structure /FileHeader/
            record /BitMapFileHeader/  BFH
            record /BitMapInfoHeader/  BIH
            record /RGBQuad/           Palette(16)
      end structure

      record /FileHeader/ FilHead

      integer iWrtBuf
      parameter (iWrtBuf=650)
      integer*1 Buf(iWrtBuf)            ! > 2* (VGA-Auflösung/2)
      equivalence (buf,FilHead)


      integer nByteRow
      integer iPtr, iPathlen
      integer*2 iHandle, ierr
      character*10 FilNam, Path*80

      call GraphicError (10,' ') ! Hardcopy in progress
c
c  Initialisierung Fileheader
c
      nByteRow=(kScrX+7-mod(kScrX-1,8))/2 ! Byte pro Zeile durch 4 teilbar
      if (2*nByteRow.gt.iWrtBuf) then
       call GRAPHICERROR (8, ' ') ! Hardcopy: Write Buffer Overflow
      end if

      FilHead.BFH.DatKennung= 19778 ! = 4d42h

      FilHead.BFH.Reserved1= 0
      FilHead.BFH.Reserved2= 0

      FilHead.BFH.GraphDatDst= 118 ! = 76h
      FilHead.BFH.DatSize=nByteRow*(kScrY+1) + FilHead.BFH.GraphDatDst

      FilHead.BIH.BmpInfHdSiz= 40 ! = 28h
      FilHead.BIH.PicWidth= kScrX+1
      FilHead.BIH.PicHeight= kScrY+1

      FilHead.BIH.iLayer= 1
      FilHead.BIH.iBitPix=4         ! Auch bei Monochrom???
      FilHead.BIH.Kompr= 0
      FilHead.BIH.PicSiz= 0         ! nicht verwendet
      FilHead.BIH.HorPixDen= 0
      FilHead.BIH.VerPixDen= 0
      FilHead.BIH.iCol= 0
      FilHead.BIH.iVipCol= 0

      FilHead.Palette(1).red= 0
      FilHead.Palette(1).green= 0
      FilHead.Palette(1).blue= 0

      FilHead.Palette(2).red= 0
      FilHead.Palette(2).green= 0
      FilHead.Palette(2).blue= 160

      FilHead.Palette(3).red= 0
      FilHead.Palette(3).green= 160
      FilHead.Palette(3).blue= 0

      FilHead.Palette(4).red= 0
      FilHead.Palette(4).green= 160
      FilHead.Palette(4).blue=160

      FilHead.Palette(5).red= 160
      FilHead.Palette(5).green= 0
      FilHead.Palette(5).blue= 0

      FilHead.Palette(6).red= 160
      FilHead.Palette(6).green= 0
      FilHead.Palette(6).blue= 160

      FilHead.Palette(7).red= 160
      FilHead.Palette(7).green= 80
      FilHead.Palette(7).blue= 0

      FilHead.Palette(8).red= 160
      FilHead.Palette(8).green= 160
      FilHead.Palette(8).blue= 160

      FilHead.Palette(9).red= 80
      FilHead.Palette(9).green= 80
      FilHead.Palette(9).blue= 80

      FilHead.Palette(10).red= 80
      FilHead.Palette(10).green= 80
      FilHead.Palette(10).blue= 240

      FilHead.Palette(11).red= 80
      FilHead.Palette(11).green= 240
      FilHead.Palette(11).blue= 80

      FilHead.Palette(12).red= 80
      FilHead.Palette(12).green= 240
      FilHead.Palette(12).blue= 240

      FilHead.Palette(13).red= 240
      FilHead.Palette(13).green= 80
      FilHead.Palette(13).blue= 80

      FilHead.Palette(14).red= 240
      FilHead.Palette(14).green= 80
      FilHead.Palette(14).blue= 240

      FilHead.Palette(15).red= 240
      FilHead.Palette(15).green= 240
      FilHead.Palette(15).blue= 80

      FilHead.Palette(16).red= 240
      FilHead.Palette(16).green= 240
      FilHead.Palette(16).blue= 240

      do 3 i=1,16
 3     FilHead.Palette(i).reserved= 0
c
c Create Filename and open
c
      Path= 'SPL='//char(0)
      call GetEnv (Path, len(Path))
      iPathLen=iStringLen (Path)

      i=0
5     continue
       i= i+1
       write (Filnam,FMT=300) i
       if (iPathLen.gt.0) then
        call OpenBytFil(iErr,iHandle,
     1                 Path(:iPathLen)//'\'//FilNam//char(0))
       else
        call OpenBytFil(iErr,iHandle, FilNam//char(0))
       end if
      if (iErr.eq.80) goto 5  ! File exists - increase FilNam
      if (iErr.ne.0) call GRAPHICERROR (6, ' ') ! Hardcopy: Error during OPEN
c
c Zeilenweises Auslesen Bildschirmspeicher, Puffern und Fileausgabe
c
      iPtr= FilHead.BFH.GraphDatDst +1

      do 20 iy=kScrY,0,-1      ! oder 1?
       ix=0
 10    continue                     ! repeat
        Buf(iPtr)= ISHL (GetPixel(ix,iy),4)
        ix= ix+1
        if(ix.le.kScrX)Buf(iPtr)=Buf(iPtr).or.(GetPixel(ix,iy).and.15)
        iPtr= iPtr+1
        ix=ix+1
       if (ix.le.kScrX) goto 10
       ix=ix                        ! Anzahl belegter Halfbytes
 15    if (ix.lt.2*nByteRow) then   ! do while
        Buf (iPtr)= 0
        iPtr= iPtr+1
        ix=ix+2
        goto 15
       end if                       ! end while
       call WriteBuf (iHandle, Buf(1),iPtr, 256)
 20   continue
c
c Empty Buffer and Close File
c
      call WrtBytFil (iErr, iHandle, Buf(1), iPtr)
      if (iErr.ne.0) call GRAPHICERROR (7, ' ') ! Hardcopy: Error during WRITE

      call CloseBytFil (iHandle)
      call StatSt (' ')
      return

300   format ('HDC',I3.3,'.BMP')
      end



      subroutine WriteBuf (iHandle, Buf, iPtr, iWrite)
      integer*1 Buf(1)
      integer iPtr, iWrite
      integer*2 iHandle
      integer*2 iErr
 10   continue
       if (iPtr.le.iWrite) return
       call WrtBytFil (iErr, iHandle, Buf(1), iWrite)
       if (iErr.ne.0) call GRAPHICERROR (7, ' ') ! Hardcopy: Error during WRITE
       call lib_movC3 (iPtr-iWrite,Buf(iWrite+1), Buf(1))
       iPtr= iPtr-iWrite
      goto 10
      end


