; // DOXYGEN Dokumentation TCSdDOS.asm: als C-Programm möglich da ";" C-Leerbefehl entspricht
; /** \file TCSdDosa.asm \brief DOS Port: x86 Assembler Routinen \version 1.4 
; \author  (C) 2022 Dr.-Ing. Klaus Friedewald 
; \copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3   */

; //! \brief Tastaturabfrage \param[out] [AX] Funktionsrückgabe ASCII
; (int) ktinput ()

; //! \brief Signalton
; (void) bell ()

; //! \brief Initialisierung Graphikmaus
; //! \param[out] iAvail Maus vorhanden
; //! \param[out] iButton Anzahl Tasten
; //! \param[in] iXmin, iXmax, iYmin, iYmax Zeichenfläche
; (void) GinCrsIn (bool iAvail,int iButton,int iXmin,int iXmax,int iYmin,int iYmax)

; //! \brief Abfrage Graphikmaus
; //! \param[out] ic Gedrueckte Taste
; //! \param[out] ix, iy  Cursorposition
; (void) GinCrs (int ic,int ix,int iy)

; //! \brief Reset Graphikmaus
; (void) GinCrsEx ()

; //! \brief Abfrage Enviromentvariable  
; //! \param[in,out] Buf in=Variable out=Uebersetzung  
; //! \param[in]   BufLen

; (void) GetEnv (char Buf, int BufLen)
; //! \brief Kopieren eines Feldes  
; //! \param[in] iByte Anzahl verschiebender Bytes (0 zulässig)
; //! \param[in] Source zu kopierende Daten
; //! \param[out] Dest Zielfeld, kann auch Source überlappen

; (void) lib_movc3 (int iByte, char Source,char Dest)
; //! \brief Oeffnen eines Bytefiles
; //! \param[out] iErr Errorflag
; //! \param[out] iHandle Filehandle
; //! \param[in] FilNam Dateiname

; (void) OpenBytFil(int iErr,int iHandle,char FilNam)
; //! \brief WrtBytFil   Byteweises Schreiben ohne Steuerzeichen
; //! \param[out] iErr Errorflag
; //! \param[in] iHandle Filehandle
; //! \param[in] buf Daten
; //! \param[in] iWrite Anzahl zu schreibender Bytes

; (void) WrtBytFil (int iErr,int iHandle, char buf, int iWrite)
; //! \brief Schliesen eines Bytefiles
; //! \param[in] iHandle Filehandle

; (void) CloseBytFil (int iHandle)
; //! \cond
; ---------------------------   Changelog   ------------------------------
;
;  Version 1.2
;     25.10.01                Dr. Ing. K. Friedewald
;
;           ktinput:    Tastaturabfrage
;           bell:       Signalton
;           GinCrsIn:   Initialisierung Graphikmaus
;           GinCrs:     Abfrage Graphikmaus
;           GinCrsEx:   Wiederherstellen Graphikmaus
;
;           GetEnv:     Abfrage Enviromentvariable (C-Characterformat!)
;                        Input: Pufferfeld, Vorbesetzt mit Variablenname
;                               max. Länge Pufferfeld (einschliesslich char(0))
;                        Output:Pufferfeld, Übersetzter Wert
;
;           Lib_movC3   Kopieren eines Feldes
;                        Input: iByte, Anzahl verschiebender Bytes (0 zulässig)
;                               Source, zu kopierende Daten
;                        Output:Dest, Zielfeld, kann auch Source überlappen
;
;           OpenBytFil  Oeffnen eines Bytefiles
;                        Input: FilNam
;                        Output:iErr, iHandle
;
;           WrtBytFil   Byteweises Schreiben ohne Steuerzeichen
;                        Input: iHandle, Buf(*), iCount
;                        Output:iErr
;
;           CloseBytFil Schliesen eines Bytefiles
;                         Input: iHandle
;
;
;
;  Version 1.31
;     30.05.02                Dr. Ing. K. Friedewald
;
;           Anpassung an WATCOM-Assembler:
;            Auskommentieren der Microsoft-spezifischen Assemblerdirektiven
;            .no87, .list, title, subtitle, page
;           Bugfix: Fehlerhafte Parameterübergabe WRTBYTFIL:
;                    DS von Buf wurde überschrieben
;                    iErr jetzt übergeben (Programm: MOV, Deklaration:Offset)
;
;
;  Version 1.32
;     25.10.02                Dr. Ing. K. Friedewald
;
;           Bugfix: Schnell aufeinanderfolgende GINCRS-Aufrufe fehlerhaft
;                   Warten auf nicht gedrueckte Maustaste ergaenzt
;
;  Version 1.33
;     29.10.04                Dr. Ing. K. Friedewald
;
;           Anpassung an OpenWatcom-Linker 1.3: Großschreibung PUBLIC-Symbole
;
;  Version 1.4
;     04.12.20                Dr. Ing. K. Friedewald
;
;           Dokumentation durch DOXYGEN
;
;

;           title       'TCS Assembler Routinen'
            .8086
;            .no87
;            .list
            .model large

            public      KTINPUT     ; FORTRAN: integer*2 function ktinput ()

            public      BELL        ; FORTRAN: call bell ()

            public      GINCRS      ; FORTRAN: call gincrs (ic,ix,iy)
iC          equ   [BP] + 14         ; Integer*2 (Rückgabe 1,2: linke,rechte Maustaste sonst ASCII
iX          equ   [BP] + 10         ; Integer*2
iY          equ   [BP] + 6          ; Integer*2

            public      GINCRSIN    ; FORTRAN: call gincrsIn (iAvail, iButton, iX0,iX1,iY0,iY1)
iAvail      equ   [BP] + 26         ; Integer*2 oder Logical*2
iButton     equ   [BP] + 22         ; Integer*2
iX0         equ   [BP] + 18         ; Integer*2
iX1         equ   [BP] + 14         ; Integer*2
iY0         equ   [BP] + 10         ; Integer*2
iY1         equ   [BP] + 6          ; Integer*2

            public      GINCRSEX    ; FORTRAN: call GinCrsEx ()

            public      GETENV      ; FORTRAN: call GetEnv (CHARBUF, CharBufL)
CharBuf     equ   [BP] + 10         ; Vorbesetzt mit "NAME="//char(0)
CharBufL    equ   [BP] + 6

            public      OPENBYTFIL  ; FORTRAN: call OpenBytFil (iErr, iHandle, Filnam)
iErrO       equ   [BP] + 14
iHandleO    equ   [BP] + 10         ; integer*2 iHandle <> 0 falls o.k.
FilNam      equ   [BP] + 6          ; C-String

            public      WRTBYTFIL   ; FORTRAN: call WrtBytFil (iErr, iHandle, Buf, iCount)
iErr        equ   [BP] + 18
iHandle     equ   [BP] + 14         ; Integer*2
Buf         equ   [BP] + 10         ; byte array
iCount      equ   [BP] + 6          ; Integer*2

            public      CLOSEBYTFIL ; FORTRAN: call CloseBytFil (iHandle)
iHandleC    equ   [BP] + 6

            public      LIB_MOVC3_  ; FORTRAN: call Lib_MovC3_ (iByte, Source, Dest)
iByte       equ   [BP] + 14
Source      equ   [BP] + 10
Dest        equ   [BP] + 6

TCSdDosA_data segment public 'DATA'   ; obligatorischer Name für MS-Compiler


CrsDefHotX  equ   0                 ; Definition Graphikmousecursor
CrsDefHotY  equ   0                 ; Vorsicht, Cursor kann nicht über linke, obere Ecke geclippt werden!
CrsDef      dw    16 dup (0ffffh)   ; Screenmask (wird AND verküpft)
            dw    07c00h, 0c000h    ; Cursorform (wird XOR verknüpft)
            dw    0a000h, 09000h
            dw    08800h, 08400h
            dw    00200h, 00100h
            dw    00080h, 00000h
            dw    00000h, 00000h
            dw    00000h, 00000h
            dw    00000h, 00000h

TCSdDosA_data  ends

DGROUP      group TCSdDosA_data

;            subtitle    'TCS Basisfunktionen'
;            page

TcsdDosA_text segment public 'code'   ; obligatorischer Name für MS-Compiler

            assume CS:TcsdDosA_text, DS:DGROUP, SS:DGROUP

DOS         equ   021h              ; DOS-Interrupt
MOUSE       equ   033h              ; Mousedriver
VideoBIOS   equ   010h

;
; ********************
; *                  *
; * Function KTINPUT *
; *                  *
; ********************
;

ktinput     proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            mov   ah, 07h           ; DOS 7: Zeichen ohne Echo einlesen
            int   DOS
            mov   ah,0h

            pop   ds
            pop   bp
            ret

ktinput     endp
;
; *******************
; *                 *
; * Subroutine BELL *
; *                 *
; *******************
;
bell        proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            mov   ah, 0eh           ; Video-Bios: TTY Out
            mov   al, 07h           ; Bell
            mov   bh,0              ; Bildschirmnummer
            mov   bl,0              ; Grafik-Vordergrundfarbe
            int   VideoBIOS

            pop   ds
            pop   bp
            ret

bell        endp

;            subtitle    'Graphic Input Cursor'
;            page
;
; ***********************
; *                     *
; * Subroutine GINCRSIN *
; *                     *
; ***********************
;
ginCrsIn    proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds
            push  es

            mov   ax, 00h           ; FN : Reset Mouse
            int   MOUSE
            push  bx                ; Freimachen Indexregister
            lds   bx, iAvail        ; Adresse iAvail nach BX laden
            mov   [bx],ax           ; Wert AX nach iAvail
            lds   bx, iButton       ; Adresse iButton nach BX laden
            pop   ax
            mov   [bx],ax           ; Wert AX nach iButton

            mov   ax, 07h           ; FN : Setzen iXmin und iXmax
            lds   bx, iX0
            mov   cx,[bx]
            lds   bx, iX1
            mov   dx,[bx]
            int   MOUSE

            mov   ax, 08h           ; FN : Setzen iYmin und iYmax
            lds   bx, iY0
            mov   cx,[bx]
            lds   bx, iY1
            mov   dx,[bx]
            int   MOUSE

            mov   ax, 09h           ; FN : Definition Cursorform
            mov   bx, CrsDefHotX
            mov   cx, CrsDefHotY
            mov   dx, seg CrsDef    ; Mousedriver: Adressangabe über ES!
            mov   es, dx
            mov   dx, offset CrsDef
            int   MOUSE

            pop   es
            pop   ds
            pop   bp
            ret   24                 ; Parameteranzahl * 4 Bytes freigeben
gincrsIn    endp
;
; ***********************
; *                     *
; * Subroutine GINCRSEX *
; *                     *
; ***********************
;
ginCrsEx    proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            mov   ax, 00h           ; FN : Reset Mouse
            int   MOUSE

            pop   ds
            pop   bp
            ret   0                 ; Parameteranzahl * 4 Bytes freigeben
gincrsEx    endp
;
; *********************
; *                   *
; * Subroutine GINCRS *
; *                   *
; *********************
;
gincrs      proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            mov   ax, 01h           ; FN : Show Cursor
            int   MOUSE

WaitUp:     mov   ax, 03h           ; FN: Get Button Status
            int   MOUSE
            test  bx,bx             ; Taste noch gedrueckt?
            jnz   WaitUp            ; noch vom letzten mal -> Warte

KeyLoop:    mov   ax, 03h           ; FN : Get Button Status
            int   MOUSE             ; MouseDriver-Call
            test  bx,bx             ; Bit0 linke, Bit 1 rechte Maustaste
            jnz   ExitKeyLp         ; Taste gedrückt -> fertig

            mov   ah,06h            ; DOS 6: Zeichen ohne Warten einlesen
            mov   dl,0ffh
            int   DOS
            jz    KeyLoop           ; keine Keyboardtaste gedrückt -> weiter

            mov   ah,0h
            push  ax                ; Terminator
            mov   ax, 03h           ; FN : Get Mouse Koordinaten
            int   MOUSE
            pop   bx                ; Terminator ASCII

ExitKeyLp:  push  bx                ; Terminator
            lds   bx, iX            ; Adresse iX nach BX laden
            mov   [bx],cx           ; CX: horizontale Mauskoordinate
            lds   bx, iY            ; Adresse iY nach BX laden
            mov   [bx],dx           ; DX: vertikale Mauskoordinate
            pop   ax                ; Terminator
            lds   bx, iC            ; Adresse iC nach BX laden
            mov   [bx],ax           ; Übergabe in iC


            mov   ax, 02h           ; FN : Hide Cursor
            int   MOUSE

            pop   ds
            pop   bp
            ret   12                ; Parameteranzahl * 4 Bytes freigeben
gincrs      endp

;            subtitle    'Get Enviroment'
;            page
;
; *********************
; *                   *
; * Subroutine GETENV *
; *                   *
; *********************
;
GetEnv      proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds
            push  es
            push  di
            push  si
            pushf                   ; Rette Direction Flag!

            cld                     ; Stringsuche aufwärts
;
; Bestimmung Stringlänge Suchstring
;
            mov   cx, 0             ; Counter
            lds   si, CharBuf       ; Buffer = Suchstring
LenLoop:    mov   al,byte ptr ds:[si]; nächstes Zeichen
            or    al,al             ; Char(0) = Ende?
            jz    LenDone           ; ja
            inc   cx
            inc   si
            jmp   LenLoop

LenDone:    push  cx                ; Länge des Suchstrings
;
; Get Enviroment
;
            mov   ah, 62h           ; DOS 62h: Get PSP
            int   DOS
            mov   es,bx             ; ES:00 jetzt auf PSP
            mov   bx,es:[2ch]       ; PSP Element 2c: Enviroment
            mov   es, bx
            xor   di,di             ; Jetzt: ES:DI auf 1. Eintrag Enviroment

SearchLoop: lds   si, CharBuf       ; Suchstring in DS:AX
            pop   cx                ; Länge Suchstring
            push  cx
            repe  cmpsb             ; vergleichen mit Enviroment
            jz    Found
            xor   al,al             ; Ende Enviromenteintrag suchen
            mov   cx,-1
            repnz scasb
            cmp   byte ptr es:[di],0; letzter Eintrag?
            jnz   SearchLoop
            jmp   NotFound
;
; Abspeichern in den Puffer
;
NotFound:                           ; ES:DI auf Char(0)
Found:                              ; ES:DI auf Inhalt Enviromentvariable

            lds   bx, CharBufL      ; Parameter Bufferlänge
            mov   cx,[bx]           ; Counter = Bufferlänge

            lds   si, CharBuf       ; Zieladresse
StoreLoop:  mov   al,byte ptr es:[di]; nächstes Zeichen
            mov   byte ptr ds:[si],al; speichern
            or    al,al             ; Char(0) = Ende?
            jz    StoreDone         ; ja
            inc   di
            inc   si
            dec   cx
            jz    StoreDone         ; Bufferende erreicht
            jmp   StoreLoop

StoreDone:  pop   ax                ; Clear Stack, Suchstringlänge

            popf                    ; Restore Status
            pop   si
            pop   di
            pop   es
            pop   ds
            pop   bp
            ret   8

GetEnv      endp

;            subtitle    'Byte Files'
;            page
;
; ***********************
; *                     *
; * Function OpenBytFil *
; *                     *
; ***********************
;
OpenBytFil  proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            lds   dx,FilNam
            xor   cx,cx             ; Löschen Attribut -> unbeschränkter Zugriff
            mov   ah,05bh           ; Open New File
            int   DOS

            lds   bx, iHandleO      ; Adresse iButton nach BX laden
            mov   [bx],ax           ; FileHandle nach iHandle

            lds   bx, iErrO
            jc    ErrO              ; kein Carryflag -> iErr=0: i.O.
            xor   ax,ax             ; iErr=3: path not found, =4 too many open files
ErrO:       mov   [bx],ax           ; =5 access denied, =50h file exists

            pop   ds
            pop   bp
            ret   12                ; 12 = 3 Parameter

OpenBytFil  endp
;
;
; ***********************
; *                     *
; * Function WrtBytFil *
; *                     *
; ***********************
;

WrtBytFil   proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            lds   bx,iCount
            mov   cx,[bx]
            jcxz  NoWrt             ; keine Bytes zu schreiben

            lds   bx,iHandle
            mov   bx,[bx]

            lds   dx,Buf            ; letzter Befehl vor DOS-call, DS auf Buf!

            mov   ah,040h           ; Write File
            int   DOS

            lds   bx,iCount
            mov   cx,[bx]
            xor   dx,dx             ; Clear Error-Flag
            cmp   ax,cx             ; Count IST < Count SOLL?
            jnl   WrtIO
            mov   dx,0ffffh         ; SET Error-Flag
WrtIO:      lds   bx,iErr           ; Store Error-Flag
            mov   [bx],dx

NoWrt:      pop   ds
            pop   bp
            ret   16                ; 16 = 4 Parameter

WrtBytFil   endp
;
; ************************
; *                      *
; * Function CloseBytFil *
; *                      *
; ************************
;
CloseBytFil proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds

            lds   bx,iHandleC
            mov   bx,[bx]
            mov   ah,03eh           ; Close File
            int   DOS

            pop   ds
            pop   bp
            ret   4                 ; 4 = 1 Parameter

CloseBytFil endp

;            subtitle    'lib$MoveC3'
;            page
;
; ************************
; *                      *
; * Subroutine lib_MovC3 *
; *                      *
; ************************
;
lib_movc3_  proc far

            push  bp
            mov   bp,sp             ; lokale Basis
            push  ds
            push  es
            push  di
            push  si
            pushf                   ; Rette Direction Flag!

;
; Kopieren des Strings
;

            lds   bx,iByte
            mov   cx,[bx]           ; Counter
            lds   si, Source        ; Buffer = Suchstring
            les   di, Dest

            cld                     ; aufwärts
            cmp   di,si
            jb    domove

            add   di,cx
            dec   di
            add   si,cx
            dec   si
            std                     ; abwärts

domove:     rep   movsb

            popf                    ; Restore Status
            pop   si
            pop   di
            pop   es
            pop   ds
            pop   bp
            ret   12

lib_movc3_  endp

TcsdDosA_text ends

            end
;
; //! \endcond

