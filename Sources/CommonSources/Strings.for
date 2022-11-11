C> \file       Strings.for
C> \brief      TCS: String functions
C> \version    1.26
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C> \~german
C> Hilfsfunktionen zur Fortran Stringverarbeitung
C> \~english
C> Fortran utility functions for string processing
C> \~
C>
C
Cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc
C
C  Unterprogramme zur Behandlung von Fortran-Strings.
C  Die Stringenden werden entweder durch CHAR(0) markiert oder
C  ueber die Deklaration ermittelt.
C
C     9.11.88     K. Friedewald
C
C  Ergaenzungen:
C     iTrimLen
C
C     7.12.01     K. Friedewald
C
C  Version: 1.26
C
Cccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc

      subroutine Substitute (Source, Destination, Old1, New1)
C
C  Durchsucht SOURCE nach den Substrings OLD, ersetzt sie durch NEW
C  und uebergibt das Ergebniss in DESTINATION. Wenn New=CHAR(0), werden
C  die vorkommenden OLD nur geloescht.
C
C  Stringenden koennen durch CHAR(0) markiert werden.
C
      implicit none
      integer iNext, iNext2, TempLen
      integer iStringLen
      character *(*) Source, Destination, Old1, New1
      character*255 temp, old, new

      if (iStringLen(old1).le.0) return
      if (iStringlen(Source) .le. 0) then
       Destination= char(0)
       return
      end if

      old= old1 // char(0)          ! old evtl. = Destination
      new= new1 // char(0)          ! => retten!

      temp= Source(1:iStringLen(Source)) // char(0) ! evtl. Ueberlappung!
      Destination= temp
      iNext= index( Destination(:iStringLen(Destination)),
     1                                         Old(:iStringLen(Old)) )
      do while (iNext.gt.0)
       if (iNext.eq.1) then
        temp= Destination
        if (New.eq.char(0)) then
         Destination= temp (iStringLen(old)+1:)
        else
         Destination= new(:iStringLen(New)) // temp(iStringLen(old)+1:)
        end if
       else
        Temp= Destination(1:iNext-1)
        TempLen= iNext-1
        if (New.ne.char(0)) then
         temp= temp(1:TempLen)//new
         TempLen= TempLen+iStringLen(new)
        end if
        if (iNext+iStringLen(old).lt.len(Destination)) then
         temp= temp(1:templen)//Destination(iNext+iStringLen(old):)
        end if
        Destination= temp
       end if
       iNext2= iNext+iStringLen(new)
       if (iNext2.lt.len(Destination)) then
        iNext2= index (Destination(iNext2:), Old(:iStringLen(old)) )
       else
        iNext2=0
       end if
       if (iNext2.gt.0) then
        iNext= iNext+iStringLen(new)+iNext2-1
       else
        iNext=0
       end if
      end do
      return
      end



      function iStringLen (String)
C
C Ermittelt die Stringlänge bei durch char(0) abgeschlossenen STRINGs.
C Falls kein char(0) vorhanden ist, wird die Gesamtlänge übergeben.
C
      implicit none
      character *(*) string
      integer iStringLen, i

      i= index (String,char(0))-1
      if (i.ge.0) then
       iStringLen=i
      else
       iStringLen= len(String)
      end if
      return
      end



      character*(*) function PrintString (String)
C
C  Kopiert STRING in einen variabel langen PRINTSTRING. Hierdurch wird
C  der Ausdruck von Nullstrings (Fortran-Fehler!) vermieden.
C
      implicit none
      character String *(*)
      integer iStringLen

      if (iStringlen(String).gt.0) then
       PrintString= String(1:iStringLen(String))
      else
       PrintString= ' '
      end if
      return
      end



      integer function iTrimLen (string)
C
C  Bestimmt die Länge des Strings ohne angehängte Leerzeichen.
C  Bei Bedarf wird ein Char(0) angehaengt. Es darf in Ftn77 nie ein
C  Nullstring erzeugt werden, da sonst die RTL-Library abstuerzt. Deswegen
C  ist der kleinste erzeugte String ein Blank ' '.
C
      implicit none
      character *(*) string
      integer i, iStringLen

      i=iStringLen (string) +1

 10   continue
       i= i-1
       if (i.ge.1) then
        if (string(i:i).eq.' ') goto 10
      end if
      iTrimLen=i
      if ((i.lt.len(string)).and.(len(string).gt.1)) then
       String (i+1:i+1)= char(0) ! .gt.1: Achtung, nie Nullstring erzeugen!
      end if
      return
      end
	  