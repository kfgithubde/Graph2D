C Beispiel AG2 User Manual Seite 1-7
C
       real x(7),y(7),y1(7)
       data x/6.,1.,2.,3.,4.,5.,6./
       data y/6.,211.,114.,306.,354.,291.,325./
       data y1/6.,367.,300.,179.,238.,320.,210./

       call winlbl (char(0), 'Statusfenster', '%:D1.%')
       call initt (0)
       call binitt

       call check(x,y)
       call dsplay(x,Y)

       call symbl(1)
       call cplot(x,y1)
       call finitt (0,700)
       stop
       end
