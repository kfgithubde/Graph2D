       title   'TCS: Hardcopy'
;  Abspeichern des Graphikbildschirms in Files 'BILDx.HDC'
;
;  Version 1.0
;      13.5.88         Dr.-Ing. K. Friedewald
;
$*Macro

       maclib  z80
       public  hdcopy
       name    'TCSHDC'

tcstab equ     4000h
jmpfar equ     0FFF1h

sfirst equ     17
makef  equ     22
setdma equ     26
multio equ     44
writes equ     21
reads  equ     20
closef equ     16

filsiz equ     640/8*200/128   ; Anzahl Records

bdos   macro   function, parameter
       mvi     c,function
       lxi     d,parameter
       call    5
       endm

hdcopy mvi     a,'A'           ; Konstruktion des Filenamens
       sta     fcbpar
       mvi     a,0
       sta     fcbex
       sta     fcbcr
sealp  bdos    sfirst, fcb
       cpi     255
       jrz     notfnd
       lda     fcbpar
       inr     a               ; Schon vorhanden
       sta     fcbpar          ; also naechster Versuch
       jr      sealp
notfnd bdos    makef,fcb
       bdos    setdma,iobuf
       bdos    multio,1

       lxi     d,0-1           ; Bildschirmadresse-1
       mvi     b,Filsiz        ; Anzahl Records

Getlp  push    b
       mvi     b,128           ; Laenge log. Record
       lxi     h,iobuf
       shld    bufptr
getlp1 inx     d               ; naechste VDC-Adresse
       push    d
       push    b               ; restliche Blocklaenge
       lxi     h,tcstab+27h    ; Peek VDC
       call    jmpfar
       lhld    bufptr
       mov     m,e
       inx     h
       shld    bufptr
       pop     b
       pop     d
       djnz    getlp1
       push    d
       bdos    writes,fcb
       pop     d
       pop     b
       djnz    getlp

       bdos    closef,fcb
       ret

       dseg
fcb    db      0               ; Default Drive
       db      'BILD?   '      ; Filename
fcbpar equ     $-4
       db      'HDC'           ; extension
fcbex  db      0
       ds      19
fcbcr  db      0

iobuf  ds      128
bufptr dw      0
       end

