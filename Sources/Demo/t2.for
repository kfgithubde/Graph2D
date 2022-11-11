      program TekWin10
C
C Treiberprogramm fuer die Tektronix-Basissoftware
C       Nur mit der statischen Library linkbar, aus der DLL werden
C       nicht alle benoetigten Symbole exportiert!
C
C Zugehörige Module:
C      TCSdrWIN.c identisch mit  TCSdrWin.cpp der MS-Visual C++ Umgebung
C      TCSdrWin.h
C      TKTRNX.h   - Definition des Common-Blocks Tektronix für den C-Teil
C      TKTRNX.fd - dito für den Fortran-Teil
C
C      noch: StdAfx.h
C
       include 'TKTRNX.FD'
       character*1 InChr, Buf*120, PrintString *90
C
C  Ersatz Swindo von INITT
C
       kminsx= 0
       kmaxsx= 1023
       kminsy=0
       kmaxsy=780


       call Winlbl ('Bildfenster', 'Meckerfenster','%:t2.xml')
       call initt (0)
       Call movabs (200,400)
       CALL DRWABS (600,400)
       call dshabs (600, 700, 1)

       call GraphicError (9, 'Hallo9')
       call GraphicError (23, 'Hallo23')

       call outgtext ('Text')


       call finitt ()

       write (6,*) 'STOP'
 500   format (1x, a1)
       stop
       end
