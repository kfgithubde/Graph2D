       title 'TCS: direkte Graphik'
$*Macro
; Tektronix Plot 10 Emulation fuer C128
;
;      Grundfunktionen des TCS
;              20.4.88         Dr.-Ing. K. Friedewald
;
;      Version 1.1
;       Ergaenzung Dummy-Routinen der Windowsversion
;        bckcol, lincol, txtcol, defaultcolour
;        dblsiz, nrmsiz, winlbl
;
;       Einführung WIN16-Linestyles in DSHABS und DSHREL
;
;              20.10.2002      K. Friedewald

       maclib  z80

       public  initt1, seelc1, swind1,statst,alpha

       public  tinput, bell, finitt, erase
       public  movabs, pntabs, drwabs, dshabs
       public  movrel, pntrel, drwrel, dshrel
       public  toutpt, toutst
       public  anmode, alfmod, pclipt, iowait
       public  bckcol, lincol, txtcol, defaultcolour
       public  dblsiz, nrmsiz, winlbl

       name    'TCSDRIVR'

tcstab equ     4000h
jmpfar equ     0fff1h
bdos   equ     5

initt1 lxi     h,tcstab
       jmp     jmpfar

tinput push    h               ; Adresse Parameter
       mvi     c,6             ; Direct Console I/O
       mvi     e,0FDh          ; Wait for Char
       call    bdos
       pop     h
       mov     m,a
       inx     h               ; Ergaenzen auf 16bit
       mvi     a,0
       mov     m,a
DumRet ret                     ; auch: Ruecksprung Dummy-Routinen

anmode equ     dumRet          ; Dummy-Routinen
iowait equ     DumRet
alfmod equ     DumRet
pclipt equ     DumRet
bckcol equ     DumRet
lincol equ     DumRet
txtcol equ     DumRet
defaultcolour equ DumRet
dblsiz equ     DumRet
nrmsiz equ     DumRet
winlbl equ     DumRet

bell   mvi     c,2             ; Console Out
       mvi     e,7             ; Bell
       jmp     bdos

statst push    h               ; retten Parameter
       push    d
       lxi     d,0
       lxi     b,5
       lxi     h,tcstab+12h    ; MovAbs
       call    jmpfar
       lxi     d,0
       lxi     b,0
       lxix    639
       lxiy    4
       lxi     h,tcstab+1eh    ; Setzen Window
       call    jmpfar
       pop     d
       pop     h
       call    toutst
       lxi     d,0             ; altes Window
       lxi     b,0
       lxix    639
       lxiy    199
       lxi     h,tcstab+1eh
       jmp     jmpfar          ; und zurueck

finitt lxi     h,fimsgl        ; 2 Dummy-Parameter
       lxi     d,fimssg
       call    statst          ; Ausgabe Meldung
       call    bell
       mvi     c,6             ; Warten auf beliebigen Tastendruck
       mvi     e,0FDh
       call    bdos
alpha  lxi     h,tcstab+3      ; alpha
       jmp     jmpfar

       dseg
fimssg db      'press any key to continue'
fimsgl dw      $-fimssg
       cseg

erase  lxi     h,tcstab+33h
       jmp     jmpfar

toutpt mov     e,m             ; Parameter: ASCII
       lxi     h,tcstab+1bh    ; CharOut
       jmp     jmpfar

toutst mov     b,m             ; N Char
       xchg
toutlp mov     e,m
       push    h               ; Adr. String
       push    b
       lxi     h,tcstab+1bh    ; CharOut
       call    jmpfar
       pop     b
       pop     h
       inx     h               ; Naechster Buchstabe
       djnz    toutlp
       ret

pntabs call    getxy
       lxi     h,tcstab+09h    ; SetPnt
       jmp     jmpfar

drwabs lxix    0FFh            ; Durchgezogene Linie
Line   call    getxy
       lxi     h,tcstab+0fh    ; Line
       jmp     jmpfar

dshabs sbcd    dshab1+2        ; Adresse der LinienMaske
       inx     b               ; Pointer auf High-Byte
       ldax    b               ; High-Byte in [A]
       dcx     b               ; Restore Pointer
       cpi     00h
       jnz     dshab1          ; HiByte>0 -> kein WIN16 Linienstil
       ldax    b               ; LowByte der Maske
       cpi     00h             ; Durchgezogene Linie
       jnz     dshab2          ; Pruefe weiter falls nicht
       lxix    0ffffh
       jr      line
dshab2 cpi     01h             ; dotted?
       jnz     dshab3          ; nein
       lxix    0aaaah
       jr      line
dshab3 cpi     02h             ; dash-dotted?
       jnz     dshab4          ; nein
       lxix    0e4e4h
       jr      line
dshab4 cpi     03h             ; dashed?
       jnz     dshab1          ; nein-> benutze Parameter als Maske
       lxix    0f0fh
       jr      line
dshab1 lixd    1234            ; Parameter wird eingetragen
       jr      line            ; Rest wie DRWABS

movabs call    getxy
       lxi     h,tcstab+12h
       jmp     jmpfar

pntrel call    movrel
       lxi     d,0             ; DrwRel mit 0,0
       lxi     b,0
       lxix    0FFh
       lxi     h,tcsTab+18h
       jmp     jmpfar

drwrel lxix    0FFh
LinRel call    getrel
       lxi     h,tcstab+18h
       jmp     jmpfar

dshrel sbcd    dshre1+2
       inx     b               ; Pointer auf High-Byte
       ldax    b               ; High-Byte in [A]
       dcx     b               ; Restore Pointer
       cpi     00h
       jnz     dshre1          ; HiByte>0 -> kein WIN16 Linienstil
       ldax    b               ; LowByte der Maske
       cpi     00h             ; Durchgezogene Linie
       jnz     dshre2          ; Pruefe weiter falls nicht
       lxix    0ffffh
       jr      LinRel
dshre2 cpi     01h             ; dotted?
       jnz     dshre3          ; nein
       lxix    0aaaah
       jr      LinRel
dshre3 cpi     02h             ; dash-dotted?
       jnz     dshre4          ; nein
       lxix    0e4e4h
       jr      LinRel
dshre4 cpi     03h             ; dashed?
       jnz     dshre1          ; nein-> benutze Parameter als Maske
       lxix    0f0fh
       jr      LinRel
dshre1 lixd    1234            ; wird veraendert!
       jr      LinRel          ; Rest wie DRWREL

movrel call    getrel
       lxi     h,tcstab+15h
       jmp     jmpfar

swind1 push    b               ; Rette Parameterblock
       call    getxy
       pop     h
       push    d               ; xmin
       push    b               ; ymax
       mov     e,m
       inx     h
       mov     d,m             ; DE: x2
       push    d
       inx     h
       mov     e,m
       inx     h
       mov     d,m             ; DE: y2
       pop     h
       call    getxy           ; ymin ok
       push    d               ; xmax
       popix
       popiy                   ; ymax
       pop     d               ; xmin
       lxi     h,tcstab+1Eh
       jmp     jmpfar

seelc1 push    h               ; rette Parameter
       push    d
       lxi     h,tcstab+2Dh    ; Where
       call    jmpfar
       pop     h               ; 2. Parameter
       mov     m,c             ; Yloc
       inx     h
       mov     m,b
       pop     h
       mov     m,e
       inx     h
       mov     m,d
       ret

;
; Hole x-y Koordinaten
;
getxy  push    d               ; Rette 2 Parameter
       mov     e,m
       inx     h
       mov     d,m             ; X-Koordinate
       pop     h
       mov     c,m
       inx     h
       mov     b,m             ; Y-Koordinate
;
; Umrechnung Koordinatensystem (1024/780) auf (640/195)
;
xytran inx     d               ; runden der Koordinaten
       inx     b
       inx     b

       srar    d               ; x=x/2+x/8
       rarr    e               ; /2
       mov     h,d
       mov     l,e
       srar    d
       rarr    e               ; /4
       srar    d
       rarr    e               ; /8
       dad     d
       xchg                    ; X ok

       lxi     h,199           ; Y=199-y/4
       srar    b
       rarr    c               ; /2
       srar    b
       rarr    c               ; /4
       ana     a               ; Clear Carry
       dsbc    b
       mov     b,h
       mov     c,l
       ret

;
; XY Koordinaten fuer REL-Befehle
;
getrel call    getxy
       lxi     h,-199          ; Hier: Ohne Offset!!
       dad     b
       mov     b,h
       mov     c,l
       ret
