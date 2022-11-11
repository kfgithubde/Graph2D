        page
;
; Unterprogramme
;
; Initialisierung
;
init    equ     $-StartBnk0+TcsTab
        ImmVek  0,0
        StorVek xmin
        ImmVek  639,199
        StorVek xmax
        ImmVek  0,-7
        StorVek CVek1
        ImmVek  7,0
        StorVek CVek2
        ImmVek  -7,1
        StorVek CVek3
        ImmVek  319,99
        StorVek GinX
        ret
;
; Rueckkehr Bank 1
;
return  equ     $-StartBnk0+TcsTab
        mvi     a,ConfB1        ; Bank 1
        stc     ; Set Carry=> Return
        lspd    OldStack
        jmp     RetFar
;
; Aufruf der 6502-Subroutine (DE)
;
enabl65 equ     $-StartBnk0+TcsTab
        lhld    cnt65   ; Retten Orginal-Wiederaufnahmen
        shld    tmp
        lda     cnt80
        lhld    cnt80+1
        sta     tmp+2
        shld    tmp+3

        sded    cnt65   ; Neue Wiederaufnahmen
        mvi     a,0c3h  ; Code: JMP
        lxi     h,enabl80
        sta     cnt80
        shld    cnt80+1

        jmp     go65

enabl80 equ     $-StartBnk0+TcsTab
        ei      ; bei Prozessorumschaltung abgestellt
        lhld    tmp     ; Alte Wiederaufnahmen
        shld    cnt65
        lda     tmp+2
        lhld    tmp+3
        sta     cnt80
        shld    cnt80+1
        ret
;
; Loeschen des Bildschirmes
;
eras    equ     $-StartBnk0+TcsTab
        FlVDC   0,4000h,0  ; Bildschirm komplett loeschen
        lxi     h,0        ; Setzen Turtle
        shld    xpos
        shld    ypos
        ret
;
; Unterprogramm Loeschen/Setzen eines Punktes
;
cpnt    equ     $-StartBnk0+TcsTab
        mvi     a,028h  ; Maske fuer RES A
        jr      pnt

spnt    equ     $-StartBnk0+TcsTab
        mvi     a,020h  ; Maske fuer SET A

pnt     StorVek xpos    ; Aktualisierung Turtle
; Bereichsueberpruefung
pnt1    equ     $-StartBnk0+TcsTab
        lhld    xmax    ; x>xmax?
        ana     a
        dsbc    de
        rm

        lhld    xmin    ; x<xmin?
        dcx     h       ; x=xmin ok
        ana     a
        dsbc    de
        rp

        lhld    ymax    ; y>ymax?
        ana     a
        dsbc    bc
        rm

        lhld    ymin    ; y<ymin?
        dcx     h       ; y=ymin ok
        ana     a
        dsbc    bc
        rp

        ana     a       ; Clear Carry
        rarr    d       ; x/2
        rarr    e
        rar     ; auch Clear Carry!
        rarr    d; x/4
        rarr    e
        rar
        rarr    d       ; x/8
        rarr    e
        rar
        rrc     ; Einschub Bit SET bzw. RES
        rrc     ; 2. Befehlsbyte SET/RES x,A
        neg     ; Bit 7 steht links!
        sta     bitst+1

        lhld    ypos    ; Berechnung Adresse
        slar    l       ; y*2
        ralr    h
        slar    l       ; y*4
        ralr    h
        dad     b       ; y*5
        slar    l       ; y*10
        ralr    h
        slar    l       ; y*20
        ralr    h
        slar    l       ; y*40
        ralr    h
        slar    l       ; y*80
        ralr    h
        dad     d       ; HL: VDC-Adresse

        call    VDCRam
        inp     a       ; Alter Wert
bitst   equ     $-StartBnk0+TcsTab
        setb    0,a     ; wird veraendert!
        push    psw     ; A wird von Selreg benutzt
        mvi     a,0
        sta     errflg  ; keine Bereichsueberschreitung
        call    VDCRam
        pop     psw     ; Update
        outp    a
        ret
;
; Unterprogramm Ziehen einer Linie
;
Lin     equ     $-StartBnk0+TcsTab
        StorVek xend    ; Merken Endpunkt
        lhld    xpos    ; X links/rechts?
        dsub    d
        jm      rechts
        lxix    -1
        jr      links
rechts  equ     $-StartBnk0+TcsTab
        lhld    xend    ; Berechnung Betrag
        lded    xpos
        dsub    d
        lxix    1
links   xchg    ; relatives Label!

        lhld    ypos    ; Y rauf/runter?
        dsub    b
        jm      runter
        lxiy    -1
        jr      rauf
runter  equ     $-StartBnk0+TcsTab
        lxiy    1
        lhld    yend
        lbcd    ypos
        dsub    b
rauf    mov     b,h     ; relatives Label!
        mov     c,l

        sixd    DeltaX  ; Merken Richtungen
        siyd    Deltay

        dsub    d       ; Berechnung Steigung
        push    psw     ; Rette Flag
        jm      DxGross
        mov     h,b
        mov     l,c
        lxi     b,-1
        jrz     DxGlDy
        inx     d       ; Nur wenn dx<>dy
DxGlDy:
LpY     equ     $-StartBnk0+TcsTab
        inx     b
        dsub    d
        jrnz    NoNullY
        inx     b       ; keine Schrittaenderung
        jr      NullY
NoNullY jp      LpY     ; relatives Label!
NullY   lxi     d,1     ; relatives Label!
        shld    Nxklein ; Teilungsrest
        lxi     h,0
        shld    Nyklein
        jr      DyGross

DxGross equ     $-StartBnk0+TcsTab
        xchg
        inx     b
        lxi     d,-1
LpX     equ     $-StartBnk0+TcsTab
        inx     d
        dsub    b
        jrnz    NoNullX
        inx     d       ; keine Schrittaenderung
        jr      NullX
NoNullX jp      LpX     ; relatives Label!
NullX   lxi     b,1     ; relatives Label!
        shld    Nyklein ; Teilungsrest
        lxi     h,0
        shld    NxKlein

DyGross sbcd    ndx     ; relatives Label!
        sded    ndy     ; Merken Steigung
        sbcd    CountX  ; Initialisieren Zeichnen
        sded    CountY

LinLp   equ     $-StartBnk0+TcsTab
        LoadVek xpos    ; relatives Label!
        lhld    xend
        dsub    d
        jrnz    NoXend
        pop     psw     ; Flag x/y-Lauf
        jm      finish
        push    psw
NoXend  lhld    yend    ; relatives Label!
        dsub    b
        jrnz    NoYend
        pop     psw
        jp      Finish
        push    psw
NoYend  lda     maske   ; relatives Label!
        rlc
        sta     maske
        jrnc    Loch
        call    spnt
        jr      loch1
loch    call    cpnt    ; relatives Label!
loch1   lxi     d,1     ; relatives Label!
        lhld    countx
        dsub    d
        jrnz    xgut
        lbcd    xpos
        lhld    DeltaX
        dad     b
        shld    xpos
        lhld    Nxklein
        ana     a
        dadc    d
        shld    Nxklein
        jrnz    CxOk
        lhld    ndx
        inx     h
        shld    ndx
CxOk    lhld    ndx     ; relatives Label!

xgut    shld    CountX  ; relatives Label!
        lhld    county
        dsub    d
        jrnz    Ygut
        lbcd    ypos
        lhld    DeltaY
        dad     b
        shld    ypos
        lhld    Nyklein
        ana     a
        dadc    d
        shld    Nyklein
        jrnz    CyOk
        lhld    ndy
        inx     h
        shld    ndy
CyOk    lhld    ndy     ; relatives Label!
Ygut    shld    county  ; relatives Label!
        jmp     LinLp

Finish  equ     $-StartBnk0+TcsTab
        LoadVek xend    ; Setzen des letzten Punktes
        lda     maske
        rlc
        jrnc    Loch2
        call    spnt
        jr      loch3
loch2   call    cpnt    ; relatives Label!
loch3   ret             ; relatives Label!
;
; Unterprogramm MoveRel
;
MovRl   equ     $-StartBnk0+TcsTab
        lhld    xpos
        dad     d
        shld    xpos
        lhld    ypos
        dad     b
        shld    ypos
        ret
;
; Unterprogramm DrwRel
;
DrwRl   equ     $-StartBnk0+TcsTab
        lhld    xpos    ; Berechnung xend
        dad     d
        xchg
        lhld    ypos    ; Berechnung yend
        dad     b
        mov     b,h
        mov     c,l
        call    lin
        ret
;
; Unterprogramm Invertieren Punkt
;
xorpnt  equ     $-StartBnk0+TcsTab
        loadvek xpos
        mvi     a,1
        sta     errflg          ; Vorbereitung Bereichsueberpruefung
        mvi     a,30h           ; Maske BIT A
        call    pnt1
        push    psw
        lda     bitst+1
        jrz     xorpn1          ; Punkt ist nicht gesetzt
        ani     1011$1111b      ; RESET-Befehl=SET ohne bit 6
xorpn1  ori     1000$0000b      ; SET-Befehl
        sta     xorpn3+1
        lda     errflg
        cpi     0
        jrz     xorpn2          ; Im Fenster
        pop     psw
        ret
xorpn2  pop     psw             ; alter Wert
xorpn3  equ     $-StartBnk0+TcsTab
        setb    0,a             ; wird veraendert
        push    psw
        call    vdcram          ; HL: noch VIC-Adresse
        pop     psw
        outp    a
        ret
;
; Ersatz fuer Z80-ROM Routine: Anpassung Zeichensatz
;
        IF ZModChr ne 0

ModChar equ     ZModChr         ; Anspringen Z80-ROM

        ELSE

ModChar equ     $-StartBnk0+TcsTab
CopVDC  macro   source,dest,count
        lxi     h,source
        lxi     d,dest
        lxi     b,count
        push    h
        rcall   7Ch
        endm
Cp8VDC  macro   Dest,Count      ; Kopieren Buchstaben
        lxi     h,Dest          ; DE: Source
        mvi     b,Count
        push    h
        rcall   7Eh
        endm

        FlVDC   3800h,0400h,0
        CopVDC  37A0h,38A0h,0008h
        CopVDC  3690h,3890h,0008h
        CopVDC  35e0h,38e0h,0018h
        CopVDC  3010h,3610h,0198h
        FlVDC   3000h,0200h,0
        CopVDC  2000h,3400h,0008h
        CopVDC  21b0h,35b0h,0028h
        CopVDC  21c0h,3800h,0008h
        CopVDC  21e0h,3810h,0018h
        CopVDC  2400h,3c00h,03f8h
        lxi     d,0f1ah
        Cp8VDC  35C0h,1
        Cp8VDC  35e0h,3
        Cp8VDC  37b0h,5
        ret

        ENDIF
;
; Ersatz Move VDC-RAM
;
        IF ZMovVDC ne 0

SMovVDC equ   ZMovVDC

        ELSE

SMovVDC equ   $-StartBnk0+TcsTab
        push  PSW
        call  VDCRam
        mvi   a,24
        call  SelReg
        inp   a
        ori   80h
        outp  a
        mvi   a,32
        call  selreg
        outp  d
        mvi   a,33
        call  SelReg
        outp  e
        mvi   a,30
        call  SelReg
        pop   PSW
        outp  a
        ret

        ENDIF
;
; Ersatz Fill VDC-RAM
;
        IF ZFilVDC ne 0

SFilVDC equ   ZFilVDC

        ELSE

SFilVDC equ   $-StartBnk0+TcsTab
        push  PSW
        call  VDCRam
        outp  d
        pop   PSW
        dcr   a
        rz
        push  PSW
        mvi   a,24
        call  SelReg
        inp   a
        ani   7fh
        outp  a
        mvi   a,30
        call  SelReg
        pop   PSW
        outp  a
        mvi   b,0
        mov   c,a
        inx   b
        dad   b
        push  d
        push  h
        mvi   a,12h
        call  SelReg
        inp   h
        mvi   a,13h
        call  selreg
        inp   l
        pop   d
        pop   b
Floc1   mov   a,h
        cmp   d
        jrnz  Floc2
        mov   a,l
        cmp   e
Floc2   rnc
        push  b
        call  VDCRam
        pop   b
        outp  b
        inx   h
        jr    Floc1

        ENDIF

EndBnk0:
;
; Datenbereich in Bank 0 (wird nicht kopiert)
;
OldStack        equ     $-StartBnk0+TcsTab
xpos    equ     OldStack+2
ypos    equ     xpos+2
xmin    equ     ypos+2
ymin    equ     xmin+2
xmax    equ     ymin+2
ymax    equ     xmax+2
CVek1   equ     ymax+2
CVek2   equ     CVek1+4
CVek3   equ     Cvek2+4
GinX    equ     CVek3+4
GinY    equ     GinX+2
errflg  equ     GinY+2
NewStack        equ     errflg+101h     ; Stack fuer Z80

tmp     equ     NewStack+1             ; temporaerer Arbeitsspeicher

Maske   equ     tmp     ; temporaer fuer Line
Xend    equ     tmp+1
YEnd    equ     tmp+3
Deltax  equ     tmp+5
DeltaY  equ     tmp+7
Ndx     equ     tmp+9
NDy     equ     tmp+11
CountX  equ     tmp+13
CountY  equ     tmp+15
Nxklein equ     tmp+17
Nyklein equ     tmp+19

ChrMsk  equ     tmp+21  ; temporaer fuer CharOut
ChrPtr  equ     tmp+29

ZahlAus equ     tmp+5   ; temporaer fuer i2cBus
Zahlein equ     tmp+6   ; bis tmp+4 fuer call65
Fehlnr  equ     tmp+7
PuffAus equ     tmp+8   ; 12 Byte Ausgabepuffer
PuffEin equ     tmp+20  ; Eingabepuffer
Byte    equ     tmp+32  ; Arbeitsspeicher
BusTim  equ     tmp+33
PegTim  equ     tmp+34

EndTmp  equ     tmp+35

ZerPg65 equ     (high EndTmp)+1 ; temporaerer 6502-Stack
        end
