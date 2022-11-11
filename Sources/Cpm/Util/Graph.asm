        title   'Initialisierung C128 Grundgraphik'
;
; Installationsprogramm fuer die Graphikroutinen in Bank 0
;
;      Aufruf:  GRAPH
;                 Kopiert die Routinen nach Bank0. Bei Angabe eines
;                 beliebigen Buchstabens als Parameter wird zusaetzlich
;                 der Textmodus eingeschaltet.
;
;      19.4.88       Dr.-Ing. K. Friedewald
;
$*Macro

        extrn   TCSinit
        maclib  z80
TcsTab  equ     4000h
Jmpfar  equ     0FFF1h
CmdTail equ     080h

        call    TCSinit

        lda     CmdTail
        cmp     0
        jrz     NoText
        lxi     h,TcsTab+3      ; TextMode
        call    jmpfar

NoText  jmp     0
        end

