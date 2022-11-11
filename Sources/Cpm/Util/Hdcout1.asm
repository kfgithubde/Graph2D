       title   'TCS: Hilfsroutinen zur Druckerausgabe'
;  Lesen des VDC-RAMS:
;     LOGICAL P,PEEKV
;     p= peekv(adr)
;
;      26.5.88         Dr.-Ing. K. Friedewald
;
$*Macro

       maclib  z80
       public  peekv
       name    'HDCou1'

tcstab equ     4000h
jmpfar equ     0FFF1h

peekv  mov     e,m
       inx     h
       mov     d,m
       lxi     h,4027h
       call    jmpfar
       mov     a,e
       ret

       end

