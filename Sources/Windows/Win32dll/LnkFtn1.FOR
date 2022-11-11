C> \file    LnkFtn1.for
C> \brief   MS Windows Port: Fortran Initialisierung der DLL
C> \version 1.0
C> \~german
C> Dummyroutine zur DLL-Erzeugung: Erzwingen Fortran Initialisierung
C> \~english
C> Dummyroutine to enforce initialization of the FORTRAN system
C> \~
C> \author  Dr.-Ing. Klaus Friedewald
C>
C> \note 
C> Zum Linken der DLL muß ein Fortran-File als erstes File angegeben werden.
C> Da die IDE diese Randbedingung nicht beruecksichtigt, wird das Objectfile
C> dieser Dummyroutine dem Linker als Linkparameter uebergeben.
C>
C> \note
C> Die Compilation muss (nur einmal) von Hand erfolgen!
C>
C Dummyroutine zur DLL-Erzeugung: Erzwingen Fortran Initialisierung
C
      subroutine Link_FTN_Init_First ()
      return
      end
