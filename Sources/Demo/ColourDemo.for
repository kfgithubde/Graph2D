      program ColourDemo
      implicit none
      integer iPkt
      real xStart, step, Amp, xmin, xmax
      parameter (iPkt=100, xStart= 1.5,step= .3, Amp=5.)
      integer i
      real f1
      real x1(0:iPkt),x2(0:iPkt),y1(0:iPkt),y2(0:iPkt)
C
C  Make Datavectors
C
      x1(0)= iPkt  ! Standard long form
      y1(0)= iPkt
      do 10, i=1,iPkt
       x1(i)= xStart+(i-1)*step
       y1(i)= f1(x1(i),Amp)
10    continue

      x2(0)= -1.  ! Short linear form
      y2(0)= -1.
      x2(1)= 3.
      y2(1)= 3.
      x2(2)= xStart
      y2(2)= xStart
      x2(3)= iPkt*step/2.
      y2(3)= iPkt*step/2.
C
C Initialization
C
      call winlbl ('Graphik: %:', 'Status:%:', char (0))
      call initt (0)
      call binitt
      call bckcol (6)
      call erase ! Show yellow background

      xmin= +1.e20 ! Initialization of Rangecheckingroutine
      xmax= -1.e20
      call mnmx (x1,xmin,xmax)   ! Min/Max of x1
      call mnmx (x2, xmin,xmax)  ! Min/Max of x1 and x2
      call dlimx (xmin-10*step, xmax+20*step) ! Set own x-axis limits
      call check(x1,y1)

      call dsplay(0.,0.) ! Plot only Grid with color 1 without curve

      call lincol (2)    ! now plot curve in color 2
      call cplot(x1,y1)

      call lincol (3) ! and plot curve in color 3
      call line (2)   ! dash-dotted
      call symbl(6)   ! with symbols
      call cplot(x2,y2)

      call finitt (0,700)
      end

      real function f1 (x, fac)
      implicit none
      real x,fac
      f1= x+sin(x)*fac
      return
      end
