       title   'TCS: Kursivschrift'
; TCS-Subroutinen ITALIC und ITALIR
;
; Version 1.0
;      14.5.88         Dr.-Ing. K. Friedewald
$*Macro
       maclib  z80
       public  italic,italir
       name    'TCSITL'
jmpfar equ     0FFF1h
TcsTab equ     4000h

italic lxix    -8
ital1  lxiy    1
       lxi     d,7
       lxi     b,0
       lxi     h,tcstab+21h    ; SetChar
       jmp     jmpfar          ; und fertig

italir lxix    -7
       jmp     ital1           ; Rest s.o.
       end


