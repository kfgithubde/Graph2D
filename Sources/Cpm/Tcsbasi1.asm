        title   'C128 Grundgraphik'
;
;      Graphikgrundpaket fuer die Ansteuerung des C128 VDC-Chips
;
;      Das Paket wird durch das Unterprogramm TCSinit in die Bank 0 kopiert.
;      Anschliessend werden die einzelnen Funktionen ueber
;              call jmpfar
;      ausgefuehrt. Die gewuenschte Funktion und die Parameter werden in den
;      Prozessorregistern uebergeben. Hierbei enthaelt HL die Sprungadresse,
;      die uebrigen Register DE, BC, IX, IY die max. 4 Parameter.
;
;      In den Treiberroutinen muessen die Konstanten
;              TcsTab = 4000h
;       und    JmpFar = FFF1h
;      verwendet werden.
;
;      12.5.88         Dr.-Ing. K. Friedewald
;
;      Version 2.0     4.1.94
;       - Implementation des i2c-Bustreibers
;       - Versionsidentifikation
;      Version 2.1      7.3.95
;       - Beruecksichtigung 64 k VDC-RAM
;       - VDCMove, VDCFill, VDCRegR, VDCRegW, VDCinit
;
;                      Dr. K. Friedewald
;
;      Version 2.2      8.12.20
;       - Entfernung Code i2c-Bustreiber
;
$*Macro

        public  TCSinit
        name    'TCSinit'

        maclib  z80
        maclib  x6502

vers    equ     2h      ; Versionsnummer
release equ     1h

VDC64k  equ     1       ; 0=16k, 1=64k RAM

i2cflg  equ     0       ; i2c-Bustreiber wird installiert (bis V2.1)

ZModChr equ     05bdh   ; Modifiziere Zeichensatz (ROM-Routine)
ZMovVDC equ     08d8h   ; VDC-Move (ROM-Routine)
ZFilVDC equ     086ch   ; VDC-Fill (ROM-Routine)
                        ; Lage ROM-Routine unbekannt: equ 0

BiosVector      equ 0f000h  ; BDOS (50,25) setzt XMOVE zurueck!
                ; equ 0: BiosVector wird RUNTIME berechnet

xmove   equ     BiosVector+3*29
move    equ     BiosVector+3*25

TcsTab  equ     4000h   ; Lage in Bank 0

MMU     equ     0d500h
confreg equ     0ff00h  ; Konfigurationsregister
ConfB0  equ     3fh     ; Bank 0
ConfB1  equ     7fh     ; Bank 1
ConfBI  equ     3eh     ; Bank 0 + IO
ConfKI  equ     0eh     ; Kernal + IO
ConfKC  equ     0fh     ; Kernal + CharROM

CharRom equ     0d800h  ; Character-Rom
go65    equ     0ffe0h
cnt65   equ     0ffddh  ; Startvektor 6502
go80    equ     0ffd0h
cnt80   equ     0ffeeh  ; Startbefehl Z80
JmpFar  equ     cnt80+3

richtung equ 0000h      ; Prozessorport 6502
port     equ 0001h      ; fuer I2C
vic      equ 0d000h
btim     equ 10         ; Zeitkonstanten
ptim     equ 20         ; fuer I2C

dlchr   equ     0ff62h  ; Kopieren des Charrom (8502)

SelReg  equ     0189h   ; Anwahl VDC-Register
VDCRam  equ     0186h   ; Update VDC-Ram
FilVdc  equ     07Ah    ; Fuellen VDC-Ram
VicCode equ     70h     ; Codewandlung Ascii-Cbm
print   equ     68h     ; print text

ChrH    equ     8       ; Hoehe der Buchstaben
GinW    equ     16      ; Breite Gin Cursor
GinH    equ     8       ; Hoehe GinCrs.

rcall   macro   FuncNr  ; Aufruf Z80-ROM
        mvi     l,FuncNr
        rst     5
        endm

call65  macro   adr     ; Unterprogrammaufruf fuer 8502
        lxi     d,adr
        call    enabl65
        endm

LoadVek macro   adr     ; Laden 2-Byte Vektor
        lded    adr
        lbcd    adr+2
        endm

ImmVek  macro   valx,valy       ; Setzen 2-Byte Vektor
        lxi     d,valx
        lxi     b,valy
        endm

StorVek macro   adr     ; Abspeichern 2-Byte Vektor
        sded    adr
        sbcd    adr+2
        endm

dsub    macro   DubReg
        ana     a       ; clear Carry
        dsbc    DubReg
        endm

FlVDC   macro   Start,Len,Val
        lxi     h,Start
        lxi     b,Len
        mvi     d,Val
        push    h
        rcall   FilVDC
        endm

        page
;
; Start Initialisierung
;
TCSinit IF BiosVector eq 0
         lhld   1
         lxi    d,Move-3
         dad    d
         shld   MoveVc+1
         lxi    d,XMove-Move
         dad    d
         shld   XMoveVc+1
        ENDIF
        lxi     b,JmpFar2-JmpFar1       ; Installation JMPFAR
        lxi     d,JmpFar        ; Destination
        lxi     h,JmpFar1       ; Source
        ldir

        lxi     d,StartBnk0
        lxi     h,TcsTab
CopLp   push    d
        push    h
        lxi     h,EndBnk0
        dsub    d
        jz      done
        push    h       ; noch zu kopierende Bytes
        lxi     d,128   ; Maximal 128 Bytes / MOVE
        dsub    d
        jm      Rest    ; NByte<128
        pop     h
        push    d       ; Kopiere 128 Bytes

Rest    mvi     b,0     ; Kopieren Code in Bank 0
        mvi     c,1     ; aus Bank 1
XMoveVc call    xmove
        pop     b
        pop     h
        pop     d
MoveVc  call    move

        jmp     CopLp

done    pop     h       ; Restore Stack
        pop     h

        ret
;
; Kopie JMPFAR
;
jmpfar1 mvi     a,ConfB0        ; Bank 0
        ana     a       ; Clear Carry => JMP
RetFar  equ     $-jmpfar1+jmpfar
        sta     confreg
        rc
        jmp     dojob
jmpfar2:
;
; Arbeitsspeicher Bank 1
;
        dseg
SouAdd  dw      StartBnk0
DstAdd  dw      TcsTab
NByte   dw      EndBnk0-StartBnk0
        cseg
        page
;
; Code fuer Bank 0
;
StartBnk0:
;
; Sprungtabelle
;
        jmp     Graphic
        jmp     Alpha   ; TcsTab + $03
        jmp     Colour  ; + $06
        jmp     SetPnt  ; + $09
        jmp     ClrPnt  ; + $0c
        jmp     Line    ; + $0f
        jmp     movabs  ; + $12
        jmp     MovRel  ; + $15
        jmp     DrwRel  ; + $18
        jmp     CharOut ; + $1b
        jmp     Window  ; + $1e
        jmp     SetChar ; + $21
        jmp     ChkPnt  ; + $24
        jmp     PeekVDC ; + $27
        jmp     PokeVDC ; + $2a
        jmp     Where   ; + $2d
        jmp     GetWin  ; + $30
        jmp     erase   ; + $33
        jmp     GinCrs  ; + $36
        jmp     GinMrl  ; + $39
        jmp     i2cBus  ; + $3c
        jmp     version ; + $3f
        jmp     VDCmove ; + $42
        jmp     VDCfill ; + $45
        jmp     VDCRegR ; + $48
        jmp     VDCRegW ; + $4b
        jmp     VDCinit ; + $4e
;
; Start aller Jobs
;
dojob   equ     $-StartBnk0+TcsTab
        sspd    OldStack        ; Neuer Stack in Bank 0!
        lxi     sp,NewStack
        pchl    ; Sprung in Tabelle
;
; Einschalten Graphic-Mode
;
Graphic equ     $-StartBnk0+TcsTab
        mvi     a,25    ; VDC-Register
        call    SelReg
        inp     d
        setb    7,d     ; Graphic ein
        res     6,d     ; Attribut aus
        call    selreg
        outp    d

        call    init
        call    eras

        mvi     e,0fh   ; weisser Vordergrund
        mvi     c,0     ; schwarzer Hintergrund
        jmp     Colour  ; und zurueck Bank 0
;
; Abschalten Graphic-Mode, Kopieren Zeichensatz
;
alpha   equ     $-StartBnk0+TcsTab
        mvi     a,25    ; VDC-Register
        call    SelReg
        inp     d
        res     7,d     ; Text-Mode
        setb    6,d     ; Attribute
        call    selreg
        outp    d

        call65  dlchr1
        call    ModChar ; Anpassung PET-ASCII

        rcall   print
        db      0ffh,0 ; CRT loeschen

        mvi     e,0fh   ; weisser Vordergrund
        mvi     c,0h    ; Schwarzer Hintergrund
        jmp     Colour  ; und zurueck Bank 0

dlchr1  equ     $-StartBnk0+TcsTab
        @lda    ConfBI,#        ; IO einblenden
        @sta    ConfReg

        @lda    MMU+08h         ; Verlegen Zero-Page
        @sta    dlchr2+1        ; Merken fuer Rueckkehr
        @and    11110000b,#     ; Bank 0
        @ldx    ZerPg65,#
        @sta    MMU+08h
        @stx    MMU+07h
        @inx                    ; sowie Stack
        @lda    MMU+0ah
        @sta    dlchr3+1
        @and    11110000b,#
        @sta    MMU+0ah
        @stx    MMU+09h

        @lda    ConfKI,#        ; Kopieren Zeichensatz
        @sta    ConfReg
        @jsr    dlchr
        @lda    ConfBI,#
        @sta    ConfReg

dlchr2  equ     $-StartBnk0+TcsTab
        @lda    0,#            ; wird ueberschrieben
        @sta    MMU+08h
        @ldx    0,#            ; alte Zeropage und Stack
        @stx    MMU+07h
dlchr3  equ     $-StartBnk0+TcsTab
        @lda    0,#
        @sta    MMU+0ah
        @inx
        @stx    MMU+09h

        @lda    ConfB0,#        ; RAM
        @sta    ConfReg
        @jmp    go80
;
; Setzen der Zeichen- und Hintergrundfarbe
;
Colour  equ     $-StartBnk0+TcsTab
        push    b       ; Hintergrund

        mvi     a,26    ; Lesen alter Wert
        call    selreg
        inp     a

        sta     tmp     ; alte Farben
        call    MakeCol
        pop     d
        call    MakeCol ; dito fuer Background
        lda     tmp+1
        mov     d,a
        mvi     a,26    ; Schreiben neuer Wert
        call    selreg
        outp    d
        jmp     return

MakeCol equ     $-StartBnk0+TcsTab
        lxi     h,tmp
        rld             ; A: alterWert
        bit     4,e     ; Neuer Wert=16?
        jrnz    OldCol
        mov     a,e
OldCol  inx     h       ; Einschieben in tmp+1
        rld
        ret
;
; Setzen/Loeschen eines Punktes
;
SetPnt  equ     $-StartBnk0+TcsTab
        call    spnt
        jmp     return

clrpnt  equ     $-StartBnk0+TcsTab
        call    cpnt
        jmp     return
;
; Ziehen einer Linie
;
Line    equ     $-StartBnk0+TcsTab
        sixd    maske   ; Maske, ueberschreibt xend!
        call    Lin
        jmp     return
;
; Move Turtle
;
movabs  equ     $-StartBnk0+TcsTab
        StorVek xpos
        jmp     return
;
; Move Turtle relativ
;
MovRel  equ     $-StartBnk0+TcsTab
        call    MovRl
        jmp     return
;
; Draw relativ
;
DrwRel  equ     $-StartBnk0+TcsTab
        sixd    maske   ; Maske, ueberschreibt xend!
        call    DrwRl
        jmp     return
;
; Ausgabe eines Buchstabens
;
CharOut equ     $-StartBnk0+TcsTab
        mov     a,e
        cpi     20h     ; Blank
        jp      NoCntrl
        cpi     0dh     ; CR
        jnz     return
        lded    xmin
        lbcd    ypos
        lxi     h,ChrH
        dad     b
        mov     b,h
        mov     c,l
        jmp     movabs

NoCntrl equ     $-StartBnk0+TcsTab
        mov     b,a
        rcall   VicCode ; Berechnung Adresse Bitmuster
        mvi     h,0
        mov     l,a
        dad     h       ; *2
        dad     h       ; *4
        dad     h       ; *8
        lxi     d,CharRom
        dad     d       ; Adresse Bitmuster ok
        shld    ChrAdr
        call65  GetChar

        LoadVek CVek1
        call    MovRl
        lxi     h,chrmsk
        shld    chrptr
chrloop lhld    chrptr  ; relatives label
        mov     a,m
        sta     maske
        LoadVek CVek2
        call    DrwRl
        lhld    chrptr
        inx     h
        shld    chrptr
        lxi     d,ChrMsk+8
        ana     a       ; Clear Carry
        dsbc    d
        jrz     cdone
        LoadVek CVek3
        call    movrl
        jr      chrloop
Cdone   jmp     return

GetChar equ     $-StartBnk0+TcsTab
        @lda    ConfKC,#
        @sta    confreg
        @ldx    7,#
ChrLp   @lda    CharRom,x       ; Adresse wird veraendert!
ChrAdr  equ     ChrLp+1-StartBnk0+TcsTab
        @sta    chrmsk,x
        @dex
        @bpl    chrlp
        @lda    ConfB0,#
        @sta    confreg
        @jmp    go80
;
; Setzen des Bildschirmfensters
;
Window  equ     $-StartBnk0+TcsTab
        StorVek xmin
        sixd    xmax
        siyd    ymax
        jmp     return
;
; Setzen der Schreibrichtung
;
SetChar equ     $-StartBnk0+TcsTab
        StorVek CVek2
        sixd    CVek3
        siyd    CVek3+2
        lbcd    cvek3   ; Berechnung cvek1=-7(cvek2+cvek3)
        call    Add7
        sded    CVek1   ; X-Koordinate
        lded    CVek2+2
        lbcd    CVek3+2
        call    Add7
        sded    CVek1+2 ; Y-Koordinate
        jmp     return

Add7    equ     $-StartBnk0+TcsTab
        mov     a,e     ; nur 4-Bit Rechnung!
        add     c
        mov     e,a
        neg
        slar    a
        slar    a
        slar    a       ; *(-8)
        add     e       ; *(-7)
        mov     e,a
        mvi     d,0
        rp
        mvi     d,0ffh  ; Erweiterung 16-bit negativ
        ret
;
; Ueberpruefung, ob Punkt gesetzt
;
ChkPnt  equ     $-StartBnk0+TcsTab
        mvi     a,1     ; Setzen Error-Flag
        sta     errflg
        mvi     a,030h  ; Maske fuer BIT A
        lhld    ypos    ; benoetigt fuer Rechnung
        push    h
        sbcd    ypos
        call    pnt1
        lxi     d,01h
        jrnz    pntset
        lxi     d,0
PntSet  lda     errflg  ; Bereichsueberpruefung erfolgreich?
        cpi     0
        jrz     Inside
        lxi     d,-1
Inside  pop     h       ; Wiederherstellung YPos
        shld    ypos
        jmp     return
;
; Lesen bzw. Beschreiben des Bildschirmspeichers
;
PeekVdc equ     $-StartBnk0+TcsTab
        xchg    ; Adresse nach HL
        call    VdcRam
        inp     e
        mvi     d,0     ; Ergaenzung auf 16-Bit
        jmp     return

PokeVdc equ     $-StartBnk0+TcsTab
        mov     a,c
        push    psw     ; Neuer Wert
        xchg
        call    VdcRam
        pop     psw
        outp    a
        jmp     return
;
; Ausgabe Turtle
;
where   equ     $-StartBnk0+TcsTab
        LoadVek xpos
        jmp     return
;
; Ausgabe Bildschirmfenster
;
GetWin  equ     $-StartBnk0+TcsTab
        LoadVek xmin
        lixd    xmax
        liyd    ymax
        jmp     return
;
; Bild loeschen
;
Erase   equ     $-StartBnk0+TcsTab
        call    eras
        jmp     return
;
; Verschieben (relativ) des Gin-Cursors
;
GinMrl  equ     $-StartBnk0+tcsTab
        lhld    GinX
        dad     d
        shld    GinX
        lhld    GinY
        dad     b
        shld    GinY            ; jetzt: Zeichnen
;
; Zeichnen des GIN-Cursors
;
GinCrs  equ     $-StartBnk0+TcsTab
        LoadVek xpos
        StorVek tmp
        LoadVek GinX
        rept    (GinW-1)/2      ; Zeichnen horizontale Linie
         dcx    d
        endm
        StorVek xpos
        mvi     b,GinW-1        ; Zaehler
GinCx   push    b
        call    xorpnt
        lhld    xpos
        inx     h
        shld    xpos
        pop     b
        djnz    GinCx
        Loadvek Ginx            ; Zeichnen vertikale Linie
        rept    (GinH-1)/2
         dcx     b
        endm
        StorVek xpos
        mvi     b,GinH-1
GinCy   push    b
        call    xorpnt
        lhld    ypos
        inx     h
        shld    ypos
        pop     b
        djnz    GinCy
        Loadvek tmp             ; Wiederherstellung Turtle
        Storvek xpos
        LoadVek GinX
        jmp     return
;
; i2c Bus Handling
;
i2cBus  equ     $-StartBnk0+TcsTab
        if i2cflg eq 1

         ; *********************************
         ; *  Bereich fuer i2c-Bustreiber  *
         ; *********************************
         mvi    d,03            ; Dummyroutine
         jmp    return

        else                    ; i2cflg=0
         mvi    d,03            ; Dummyroutine
         jmp    return
        endif
;
; Identifikation der Programmversion
;
version equ     $-StartBnk0+TcsTab
        mvi     d,vers*16+release

        if (Biosvector or ZModChr or ZMovVDC or ZFilVDC) eq 0
option   set    1b
        else
option   set    0b
        endif
        if i2cflg eq 1
option   set    option or 10b
        endif
        if VDC64k eq 1
option   set    option or 100b
        endif
        mvi     e,option

        jmp     return
;
; Blockverschiebung VDC-RAM
;
VDCmove equ     $-StartBnk0+TcsTab
        pushix
        pop     h
        mov     a,c
        call    SMovVDC
        jmp     return
;
; Fuellem VDC-RAM
;
VDCfill equ     $-StartBnk0+TcsTab
        pushix
        pop     h
        mov     a,c
        call    SFilVDC
        jmp     return
;
; Lesen VDC-Register
;
VDCRegR equ     $-StartBnk0+TcsTab
        mov     a,d
        call    SelReg
        inp     e
        mvi     d,0h
        jmp     return
;
; Schreiben VDC-Register
;
VDCRegW equ     $-StartBnk0+TcsTab
        mov     a,d
        call    SelReg
        outp    e
        jmp     return
;
; Initialisierung VDC
;
VDCinit equ     $-StartBnk0+TcsTab

        if      VDC64k eq 1
        mvi     a,28d
        call    SelReg
        inp     e
        setb    4,e
        mvi     a,28d
        call    SelReg
        outp    e
        call65  dlchr1
        call    ModChar
        endif

        rcall   print
        db      0ffh,0          ; CRT loeschen
        jmp     return

