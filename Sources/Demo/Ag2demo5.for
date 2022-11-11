C Demo Kalenderplotten Seite 2-19 aus dem AG2-User-Manual
C   Link durch L80 ag2demo5,calendar,tek/s,forlib/s
C
       dimension xdata(5),ydata(7)
       data xdata/-2.,6.,12.,1969.,7./
       data ydata/6.,328.,632.,421.,780.,632.,472./
       call initt(120)
       call binitt
       call xwdth(3)
       call check(xdata,ydata)
       call dsplay(xdata,ydata)
       call finitt (0,780)
       end
