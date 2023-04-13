C Beispiel AG2-User Manual Seite A-11
C
       real xpts(11),ypts(11)
       xpts(1)=10.
       ypts(1)=10.
       do 100 i=2,11
       xpts(i)=float(i-1)
100    ypts(i)=float(i-1)**3
       call initt(120)
       call binitt
       call check(xpts,ypts)
       call dsplay(xpts,ypts)
       limz=0
       call symbl(1)
       call ylab (2)
       call ytype(2)
       call ylocrt(limz)
       call xlab(0)
       call xfrm(0)
       call yfrm(2)
       call dlimy(0,0)
       call check(xpts,ypts)
       call dsplay(xpts,ypts)
       call finitt
	   stop
       end

