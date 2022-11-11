        title 'TCS: Graphic Input'
$*Macro
; Tektronix Plot 10 Emulation fuer C128
;
;      Assemblerteil der Gin-Cursorroutinen
;
; Version 1.0
;              13.5.88         Dr.-Ing. K. Friedewald
        maclib z80

        public  GinCrs
        name    'TCSGIN'

tcstab  equ     04000h
JmpFar  equ     0FFF1h
Cia1    equ     0DC00h

CLinks  equ     1      ; Tastaturcode der Cursortasten
CRecht  equ     6
CHoch   equ     5
CTief   equ     24

bdos    equ     5


GinCrs  push    h               ; Parameter retten
        push    d
        push    b

        lxi     h,tcstab+36h    ; Zeichnen Ausgansposition
        call    jmpfar
        push    psw             ; Dummy

CrsLp   pop     psw             ; Alter Terminator
        lxi     h,tcstab+36h    ; Loeschen alte Position
        call    jmpfar

        mvi     e,0ffh          ; Input/Status
        mvi     c,6             ; Direct Console i/o
        call    bdos
        push    psw             ; Terminator
        cpi     0
        jrz     Joy

        lxi     d,0
        lxi     b,0
        cpi     CLinks
        jrnz    NCl
        lxi     d,-1
        mvi     a,0ffh          ; kein Joystick
        jr      MovGin
nCl     cpi     CRecht
        jrnz    nCr
        lxi     d,1
        mvi     a,0ffh          ; kein Joystick
        jr      MovGin
ncr     cpi     CHoch
        jrnz    nch
        lxi     b,-1
        mvi     a,0ffh          ; kein Joystick
        jr      MovGin
nch     cpi     CTief
        jrnz    nct
        lxi     b,1
        mvi     a,0ffh          ; kein Joystick
        jr      MovGin
nct     mvi     a,1110$1111b    ; Simulation Feuerknopf
        jr      MovGin

Joy     mvi     d,1110$0000b    ; Lesen Joystick 2
        lxi     b,Cia1+2
        outp    d
        lxi     b,Cia1
        inp     a
        mvi     d,0FFh
        lxi     b,cia1+2
        outp    d

        lxi     d,0
        lxi     b,0

        bit     0,a
        jrnz    NJoyO
        lxi     b,-2
NJoyO   bit     1,a
        jrnz    NJoyU
        lxi     b,2
NJoyU   bit     2,a
        jrnz    NJoyL
        lxi     d,-4
NJoyL   bit     3,a
        jrnz    MovGin
        lxi     d,4
MovGin  push    psw            ; Retten Joy-Zustand
        lxi     h,tcstab+39h   ; Neue Position GinCrs
        call    jmpfar
        pop     psw
        bit     4,a
        jnz     CrsLp

        lxi     h,tcstab+36h   ; Loeschen GinCrs
        call    jmpfar

        pop     psw            ; Terminator
        pop     h              ; Parameter iy
        mov     m,c
        inx     h
        mov     m,b
        pop     h              ; Parameter ix
        mov     m,e
        inx     h
        mov     m,d
        pop     h              ; Parameter Terminator
        mov     m,a
        inx     h
        mvi     m,0
        ret
        end

