C> \file       TCSinitt.for
C> \version    1.4
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C> \~german
C> \brief   MS Windows Port: Initialisierung
C> \~english
C> \brief   MS Windows Port: initialization
C> \~
C
C
C> \~german
C> MS Windows-spezifische TCS-Routinen
C> \note
C> Initialisierung der DLL: Das Unterprogramm INITT darf sich nicht
C> in der DLL befinden, sondern muss statisch zu dem Anwenderprogramm
C> gelinkt werden, da sonst die Instanz der DLL und nicht die des
C> Anwenderprogramms ermittelt wird.
C>
C> \note
C> Achtung bei 64bit Betriebssystemen: Die Übergabe von Pointern erfolgt
C> durch Fortran77 Integer-Variablen. Bei Win64 beträgt die Pointerlänge
C> 8 Bytes entsprechend 2 StorageUnits (integer*4). Entsprechend muss der
C> Parameter nPtrStorageUnits angepasst werden.
C>
C> \note
C> Die Routine kann auch zur Initialisierung von Windows NT
C> Konsolenprogrammen verwendet werden.
C>
C
C
C> \~english
C> MS Windows specific subroutines
C> \note
C> Initialization of the DLL: The subroutine INITT must not be
C> placed inside the DLL, but must be linked statically to the user 
C> program. Otherwise the instance of the DLL and not the instance 
C> of the main programm will be optained.
C>
C> \note
C> Attention with 64bit operating systems: The passing of pointers is done
C> by Fortran77 integer variables. With Win64 the pointer length is
C> 8 bytes, corresponding to 2 StorageUnits (integer*4). In consequence the 
C> parameter nPtrStorageUnits must be set >= 2.
C>
C> \note
C> This routine can also be used for initializing Windows NT console programs.
C>\~
C>
C CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   Changelog   CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C
C  Version 1.4, 30.4.2021, K. Friedewald
C     Anpassung an Windows64: Pointerlänge 8 Byte > int*4 bei win32
C
C  Version 1.3, 17.8.2020, K. Friedewald
C     Reaktivierung KHOMEY fuer HOME()
C
C  Version 1.2, 29.9.2004, K. Friedewald
C     Zusammenfassung der DLL-Initialisierung mit der LIB-Version. INITT
C     wird zusammen mit GetMainInstance.c in der LIB gehalten, die rest-
C     lichen Programme können sich in einer DLL befinden.
C
C  Version 1.1, 22.6.2004, K. Friedewald
C     Falls initt1 von dem Hauptprogramm ohne ein aktives Fenster aufgerufen
C     wird treten schwer reproduzierbare Fehler auf, da die Rueckmeldungen
C     auf die anfänglichen Windowsabfragen nicht eindeutig zugeordnet werden.
C
C     Abhilfe: Es wird jetzt bei Bedarf vor der Initialisierung ein eigenes
C     Hauptprogrammfenster erstellt.
C
C  Version 1.0, 19.3.2003, K. Friedewald
C


C
C>  Init Hardware & Software
C


      subroutine INITT (iDummy)
C 
      parameter (nPtrStorageUnits=2) ! max.Laenge Pointer in StorageUnits (2=64bit)
      integer iInstance(nPtrStorageUnits), iWindow(nPtrStorageUnits)
      call GetMainInstAndWin (iInstance, iWindow)
      call initt1 (iInstance, iWindow)
      call SaveMainInstAndWin (iInstance, iWindow)

C> initt2() -> Reset Software
      entry initt2
      call lintrn
      call swindo (0,1023,0,780)
      call vwindo (0.,1023.,0.,780.)
      call rrotat (0.)
      call rscale (1.)
      call setmrg (0,1023)
      call nrmsiz
      call italir
      call home
      return
      end
