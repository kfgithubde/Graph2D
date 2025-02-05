function f1ftn (x) bind (C)
    use iso_c_binding, only: c_float
    implicit none
    real (c_float), intent (in) :: x
    real (c_float) f1ftn

    f1ftn= sin(x) * x
    return
end function f1ftn



subroutine f1pltftn (f1, xmin, xmax) bind (C)

    use Graph2D
    use iso_c_binding, only: c_float

    implicit none

    real (c_float), intent (in) :: xmin,xmax
    interface
        function f1 (x) bind(C)
        use, intrinsic :: iso_c_binding
        real (c_float) :: f1
        real (c_float), intent (in) :: x
        end function f1
    end interface

    integer nPoints
    parameter (nPoints=100)
    real x(0:nPoints), y(0:nPoints)
    integer i
    real dx


    x(0)= nPoints  ! Fill TekLong Data Arrays
    y(0)= nPoints
    dx= (xmax-xmin) / (nPoints-1)
    do i=1,nPoints
     x(i)= xmin+ (i-1)* dx
     y(i)= f1(x(i))
    end do

    call winlbl ("Plowin"//char(0), "Statwin"//char(0), char(0))
    call initt (0)
    call binitt
    call DefaultColour
    call lincol(2)
    call bckcol (6)
    call erase
    call check(x,y)
    call dsplay(x,Y)
    call iowait (1)

    continue
    return
end subroutine f1pltftn
