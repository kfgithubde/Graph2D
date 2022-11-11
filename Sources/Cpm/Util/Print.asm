; Subroutine PRINT (Adr+256*SecAdr, Char)
;   Ausgabe eines Buchstabens ohne Codewandlung auf dem Geraet
;   26.5.88            Dr.-Ing. K. Friedewald
       title   'Print# fuer CP/M'
       name    'Print'
       public  print

bdos   equ     5
bios   equ     50

parblk equ     0fd00h
bufptr equ     0fe00h
vicdrv equ     parblk+2
victrk equ     parblk+3
viccnt equ     parblk+5
vicdat equ     parblk+6

       cseg
print  mov     a,m             ; Geraeteadresse
       sta     vicdrv
       inx     h
       mov     a,m             ; Sek. Adr.
       sta     victrk
       ldax    d               ; auszugebender Buchstabe
       sta     vicdat

       mvi     a,1             ; Anzahl Buchstaben
       sta     viccnt

       lxi     h,vicdat        ; Bufferadresse, in Bank 0
       shld    BufPtr

       lxi     d,BiosP
       mvi     c,Bios
       jmp     bdos            ; und Rueckkehr

       dseg
BiosP  db      30              ; Bios: Userfunction
       db      4               ; 6502 Job
       dw      0               ; Dummy-Parameter
       dw      0
       dw      7               ; Jobnr: Print
       end

