       real x(7),y(7),y1(7)
       data x/6.,1.,2.,3.,4.,5.,6./
       data y/6.,211.,114.,306.,354.,291.,325./
       data y1/6.,367.,300.,179.,238.,320.,210./

       call winlbl (char(0), 'Statusfenster', '%:d1.%')
       call initt (0)
       call binitt

       call bell ()
       call hdcopy
       
       call bckcol (2)
       call erase
       call hdcopy
       

       call lincol (5)
       call movabs (10,10)
       call drwabs (300,200)
       call movabs (300,550)
       call tOutStc ('Hein'//char(0)//'rich') 
       call toutstc ('rich der Achte')     
       call pntabs (15,10)

       call hdcopy
       
       call movabs (17,12)
       call drwabs (299,12)
       call movabs (300,13)
       call drwabs  (300,100)
       call pntabs (301,100)
       
       call pntabs (300,12)
       
       call movabs (300, 15)
       call dshabs (200, 100, 3)
       call drwabs (200,200)
       call dshabs (220, 700, 1)
       call dshabs (300,700,2)
       call dshabs (0,0,0)

       call statst ('Ein langer Ausgabetext')
       call statst ('Zweiter ') 
       i= 8
       call tinput (i)
       call GraphicError (3,'Hallo')
       call tinput (i)

       call hdcopy

C       call check(x,y)
C       call dsplay(x,Y)
              
C       call symbl(6)
C       call cplot(x,y1)
       call finitt (0,700)
       stop
       end
