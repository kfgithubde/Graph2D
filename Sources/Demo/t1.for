
      integer i, Version (3)
      integer  ic,ix,iy

      real x(7),y(7),y1(7)
      real arr (100)
      character*50 buf
      character*80 PrintString
      data x/6.,1.,2.,3.,4.,5.,6./
      data y/6.,211.,114.,306.,354.,291.,325./
      data y1/6.,367.,300.,179.,238.,320.,210./

      open (9,FILE='OTTO.DAT')
      call TCSLEV (Version)
      write (9,*) 'TCS:',Version(1),'/',Version(2),'/',Version(3)
      call AG2LEV (Version)
      write (9,*) 'AG2:',Version(1),'/',Version(2),'/',Version(3)


      buf='Teststring1234567890weiter'
      call lib_movc3 (4,buf(11:11),buf(2:2))
      write (9,*) 'lib$movc3:',buf
      write (9,*) PrintString (Buf)

      call bell
      write (9,*) 'Gebe beliebige Zahl ein'

      call initt (120)


C
C Test HOME
C
      call home
      call txtcol (1)
      call cSize (iCx, iCy)
      write (Buf,212) iCx,iCy
212   FORMAT ('Defaultzustand, iCx:',i4,' iCy:',i4)
      call toutstc (Buf)
      
      Buf= ' Test char(0)' // char(0)
      call toutstc (Buf)

C
C  Farbpalette und kursiv
C
      call italic
      i=1
      do 10 i=1,15
       call newlin
       call txtcol (i)
       write (Buf,200) i
200    format ('Farbe: ',i4)
       call toutstc (Buf)
 10   continue

      call newlin
      call italir
      call toutstc ('Jetzt nicht mehr kursiv')
      call cSize (iCx, iCy)
      write (Buf,215) iCx,iCy
215   FORMAT (' iCx:',i4,' iCy:',i4)
      call toutstc (Buf)
C
C Test Dash
C
      call movabs (0,0)
      call lincol (1)
      call dshabs (100,100,1)
      call lincol (2)
      call dshrel (100,100,2)
      call lincol (3)
      call dshrel (100,100,3)
      call lincol (4)
      call dshrel (100,100,0)

      call bell
      call HDCOPY
C
C Farbe bei AG2
C
      call Statst ('Druecke Taste')

      call NEWLIN
      call toutstc ('abcdefg')
      call LINEF
      call toutstc ('h')
      call CARTN
      call toutstc ('1')
      call NEWLIN

      call toutstc ('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij'//
     1 'klmnopqrstuvwxyz1234567890ß!"§$%&/()=?´`,.-;:_')
      call NEWLIN
      call ITALIC
      call toutstc ('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij'//
     1 'klmnopqrstuvwxyz1234567890ß!"§$%&/()=?´`,.-;:_')

      call DBLSIZ
      call CARTN
      call LINEF
      call cSize (iCx, iCy)
      write (Buf,220) iCx,iCy
220   FORMAT ('iCx:',i4,' iCy:',i4)
      call toutstc (Buf)

      call toutstc ('ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghij'//
     1 'klmnopqrstuvwxyz1234567890ß!"§$%&/()=?´`,.-;:_')


      call tinput (i)

      call bckcol (2)
      call erase
      call lincol (3)
      call txtcol (4)
      call binitt

      call check(x,y)
      call dsplay(x,Y)

      call LinCol (5)
      call TxtCol (6)

      call symbl(6)
      call cplot(x,y1)

      call bell
      call HDCOPY

      call Statst ('Druecke Taste')
      call tinput (i)

      call NRMSIZ

C
C Test Gincursor, Sichtbereich Zeichenfläche
C
      call DefaultColour
      call erase

      call PntAbs (0,0)
      call DrwAbs (1023,780)
      call DrwAbs (1023,0)
      call DrwAbs (0,0)
      call DrwAbs (0,780)
      call DrwAbs (1023,780)

20    continue
       call DCURSR (iC, iX, iY)
       write (Buf,210) iC, iX, iY
       call StatSt (Buf)
      if (iC.ne.113) goto 20  ! iC=q

      call bell
      call HDCOPY

C
C Test SVSTAT / RESTAT
C

      call BckCol (7)
      call erase
      call MovAbs (100,100)
      call DrwRel (500,0)
      call LinCol (2)

      call svstat (ARR)
      call DrwRel (0,500)
      call Lincol (3)
      call Drwrel (-500,0)


      call restat (arr) ! Verhindert das Schliessen des Quadrats!
      call LinCol (4)
      call DrwRel (0,-500)

      call finitt (0,700)

210   format ('iC:',i4,' iX:',i4,' iY:',i4,' Press q to quit')


      end
