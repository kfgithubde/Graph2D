C Additional SUBROUTINE for demonstration Method 1 with wxTCSmain2sub
       subroutine FtnMain2sub ()
C Beispiel AG2 User Manual Seite A2
C
       dimension xpts(15),ypts(15)
       data xpts/14.,-1.52,-1.21,-1.05,-.81,-.62,-.6,-.57,-.41,-.32,
     &   -.09,.1,.3,.5,.7/
       data ypts/14.,-.7,-.27,-.12,.17,.28,.31,.32,.2,.18,.1,-.08,
     &   -.35,-.71,-1.22/
       call initt (120)
       call binitt
       call slimx (100,400)
       call slimy(200,600)
       call check (xpts,ypts)
       call dsplay(xpts,ypts)
       call movea (-.8,.2)
       call drawa (-.2,.2)
       call drawa (-.2,.4)
       call drawa (-.8,.4)
       call drawa (-.8,.2)
       call anmode
       call binitt
       call dlimx (-.8,-.2001)
       call dlimy(.2,.4)
       call slimx(500,975)
       call slimy(200,700)
       call check (xpts,ypts)
       call dsplay (xpts,ypts)
       call finitt (0,700)

C Additional RETURN for demonstration Method 1 with wxTCSmain2sub
       return
       end
