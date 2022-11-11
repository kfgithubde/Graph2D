       title   'TCS: Wiedereinlesen von Hardcopyfiles'
;  Einlesen eines *.HDC-Files. Aufruf aus Fortran:
;
;     LOGICAL GETHDC
;       ...
;     IF (GETHDC (Filename)) goto error
;
;      26.5.88         Dr.-Ing. K. Friedewald
;
$*Macro

       maclib  z80
       public  gethdc
       name    'GETHDC'

tcstab equ     4000h
jmpfar equ     0FFF1h

openf  equ     15
setdma equ     26
multio equ     44
reads  equ     20
closef equ     16

filsiz equ     640/8*200/128   ; Anzahl Records

bdos   macro   function, parameter
       mvi     c,function
       lxi     d,parameter
       call    5
       endm

gethdc lxi     d,fcbnam        ; Kopieren Filename
       lxi     b,8             ; 8 Buchstaben
       ldir
       mvi     a,0             ; Loeschen FCB-Felder
       sta     fcbcr
       sta     fcbex

       bdos    openf,fcb
       cpi     255             ; File vorhanden?
       rz                      ; nein => false
       bdos    setdma,iobuf
       bdos    multio,1

       lxi     d,0-1           ; Bildschirmadresse-1
       mvi     b,Filsiz        ; Anzahl Records

Getlp  push    b
       push    d
       bdos    reads,fcb
       pop     d
       mvi     b,128           ; Laenge log. Record
       lxi     h,iobuf
       shld    bufptr
getlp1 inx     d               ; naechste VDC-Adresse
       push    d
       push    b               ; restliche Blocklaenge
       lhld    bufptr
       mov     c,m
       inx     h
       shld    bufptr
       lxi     h,tcstab+2Ah    ; Poke VDC
       call    jmpfar
       pop     b
       pop     d
       djnz    getlp1
       pop     b
       djnz    getlp

       bdos    closef,fcb
       ret

       dseg
fcb    db      0               ; Default Drive
fcbnam db      '????????'      ; Filename
       db      'HDC'           ; extension
fcbex  db      0
       ds      19
fcbcr  db      0

iobuf  ds      128
bufptr dw      0
       end

