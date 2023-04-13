
program T4
! Test command line parameters according to Fortran 2003 Standard
    implicit none
    integer i1,i2
    character*132 line

    call get_command (line, i1, i2)
    print *, "call get_command (line, i1, i2) -> Line:"
    print *, line
    print *, "i1:", i1, "i2:", i2

    call get_command_argument (0, line, i1, i2)
    print *, "call get_command_argument (0, line, i1, i2) -> Line:"
    print *, line
    print *, "i1:", i1, "i2:", i2

    i1= command_argument_count()
    call get_command_argument (i1,line, i1, i2)
    print *, "call get_command_argument (command_argument_count(), line, i1, i2) -> Line:"
    print *, line
    print *, "i1:", i1, "i2:", i2

end program

