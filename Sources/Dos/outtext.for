C> \file       outtext.for
C> \version    1.0
C> \author     (C) 2022 Dr.-Ing. Klaus Friedewald
C> \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
C>
C> \~german
C> \brief   DOS Port: Textausgabe in den Grafikbereich
C> \~english
C> \brief   DOS Port: alphanumeric output to the graphic screen
C> \~
C> \version 1.0
C> \~german
C> Angleichung der Watcom-Graphikroutine an die MS-Version
C> \~english
C> Unification of the Watcom and Microsoft version
C> \~
C>
C CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC   Changelog   CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
C OUTTEXT.FOR - Angleichung der Watcom-Graphikroutine an die MS-Version
C
      include 'FGRAPH.FI'

      subroutine outtext (text) ! Angleichung an MS-Version
      character *(*) text
      character *(81) TextBuf
      TextBuf= text//char(0)
      call _outtext (TextBuf)
      return
      end
 