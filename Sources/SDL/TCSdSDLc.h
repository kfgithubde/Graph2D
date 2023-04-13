/** ****************************************************************************
\file    TCSdSDLc.h
\brief   SDL Port: Low-Level Driver
\version 1.2
\author  (C) 2023 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Headerfile zu TCSdSDLc.c
\~english
         Headerfile for TCSdSDL.c
\~

***************************************************************************** */



/* ---- Zeichenbereich im Tektronix-Koordinatensystem --------------------- */

#define TEK_XMAX 1023
#define TEK_YMAX 780


/* ------------ Compilerspezifische Definitionen -------------------------- */

#ifdef _UNICODE
 #error "GNU f77 basiert nicht auf UNICODE !!!"
#endif


/* Deklaration analog C++ */

 typedef int bool;
 #define false 0
 #define true !false


/* Deklaration Parameteruebergabe Fortran <-> C */

typedef long int logical; // 3 plattformabhaengige Definitionen
typedef long int integer; // evtl. ueberpruefen

typedef logical LOGICAL;
typedef integer FTNINT;
typedef float FTNREAL;
typedef double FTNDOUBLE;
typedef struct {float real, imag;} FTNCOMPLEX;

typedef char FTNCHAR;

typedef size_t ftnlen;  // Ersatz fuer g2c.h
typedef size_t FTNCHARLEN;

typedef struct { FTNCHAR * addr; FTNCHARLEN len; } FTNSTRDESC;
typedef FTNCHAR FTNSTRPAR;
#define FTNSTRPAR_TAIL(ftns) , FTNCHARLEN ftns##_len
#define FTNSTRPARA(ftns) ftns
#define FTNSTRPARL(ftns) ftns##_len
#define CALLFTNSTRA(ftns) ftns.addr
#define CALLFTNSTRL(ftns) , ftns.len
#define FWRDFTNSTRA(ftns) ftns
#define FWRDFTNSTRL(ftns) , ftns##_len

#define TKTRNX tktrnx_ /* Fortran Naming Convention */
#define tcslev3 tcslev3_
#define initt1 initt1_
#define finitt finitt_
#define iowait iowait_
#define GraphicError graphicerror_
#define winlbl winlbl_
#define erase erase_
#define swind1 swind1_
#define movabs movabs_
#define drwabs drwabs_
#define dshabs dshabs_
#define pntabs pntabs_
#define bckcol bckcol_
#define lincol lincol_
#define txtcol txtcol_
#define DefaultColour defaultcolour_
#define outgtext outgtext_
#define italic italic_
#define italir italir_
#define dblsiz dblsiz_
#define nrmsiz nrmsiz_
#define bell bell_
#define outtext outtext_
#define tinput tinput_
#define dcursr dcursr_
#define csize csize_
#define hdcopy hdcopy_
#define lib_movc3 lib_movc3_

/* Deklarationen von durch C aufgerufenen FTN77-Unterprogrammen */

#define GETARG getarg_       // aus GNU F77-Library
FTNINT GETARG (FTNINT *iNo, FTNCHAR *line, FTNCHARLEN line_len);

#define INITT2 initt2_
void INITT2 (void);

#define SUBSTITUTE substitute_
void SUBSTITUTE (FTNSTRPAR *Src, FTNSTRPAR *Dst, FTNSTRPAR *old, FTNSTRPAR *new
                                           FTNSTRPAR_TAIL(Src) FTNSTRPAR_TAIL(Dst)
                                           FTNSTRPAR_TAIL(old) FTNSTRPAR_TAIL(new));

/* Forward Deklarationen: Codiert in C und auch in C verwendet */

void bell (void); //  -> Forward Deklaration
void GraphicError (FTNINT *iErr, FTNSTRPAR *ftn_string,
                   FTNINT *iL  FTNSTRPAR_TAIL(ftn_string));
void outtext(FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) );
void dcursr (FTNINT *ic,FTNINT *ix,FTNINT *iy);
void finitt ();



/* ------------ Programmparameter ----------------------------------------- */


#define STAT_MAXROWS 1             /* vorhandene Statuszeilen */

#define TCS_REL_CHR_HEIGHT 0.023f

#define TCS_WINDOW_NAMELEN 50
#define TCS_FILE_NAMELEN 128
#define TCS_MESSAGELEN 132

#define MAX_HDCCOUNT 1000          /* s.u.: Format TCS_HDCFILE_NAME */

#define INIFILEXTTOKEN ".%"        /* Token fuer den Filenamenparser */
#define PROGDIRTOKEN "%:"

#define TCS_INIFILE_NAME "Graph2D"

#define SAMPLE_RATE 41000 // fuer SDL-Audioausgabe
#define BELL_AMPLITUDE 32000.0
#define BELL_FREQUENCY 441.0f
#define BELL_DURATION 200


/* Actioncodes des Journalfiles */

#define XACTION_INITT       1
#define XACTION_ERASE       2
#define XACTION_MOVABS      3
#define XACTION_DRWABS      4
#define XACTION_DSHSTYLE    5
#define XACTION_DSHABS      6
#define XACTION_PNTABS      7
#define XACTION_GTEXT       8
#define XACTION_ASCII       9
#define XACTION_BCKCOL      10
#define XACTION_LINCOL      11
#define XACTION_TXTCOL      12
#define XACTION_FONTATTR    13
#define XACTION_NOOP        14



/* Zuordnung Fehlernummern zu Meldungen */

#define WRN_NOMSG 1
#define ERR_UNKNGRAPHCARD 2
#define ERR_NOFNTFIL 3
#define ERR_NOFNT 4
#define MSG_NOMOUSE 5
#define WRN_HDCFILOPN 6
#define WRN_HDCFILWRT 7
#define WRN_HDCINTERN 8
#define MSG_USR 9
#define MSG_HDCACT 10
#define WRN_USRPRESSANY 11
#define ERR_EXIT 12
#define WRN_COPYNOMEM 13
#define WRN_COPYLOCK 14
#define WRN_JOUCREATE 15
#define WRN_JOUENTRY 16
#define WRN_JOUADD 17
#define WRN_JOUCLR 18
#define WRN_JOUUNKWN 19
#define ERR_XMLPARSER 20
#define ERR_XMLOPEN 21
#define ERR_UNKNAUDIO 22
#define MSG_USR2 23
#define WRN_INI2 24
#define MSG_MAXERRNO 25



/* Initialisierungskonstanten *.INI, werden sinngemaess auch bei der
    Registry und XML-Initialisierung verwendet.
    Bei Erweiterungen Variableninitialisierung szTCSErrorMsg und TCSErrorLev
    in TCSdWINc.c fuer Registry und XML-Initialisierung nicht vergessen und
    alle Parser (*.ini, Registry und *.xml) beruecksichtigen! */

#define TCS_INISECT0 "Graph2D" // Root-Section, derzeit nur bei XML verwendet

#define TCS_INISECT1 "Names"
 #define TCS_INIVAR_WINNAM "G2dGraphic"
    #define TCS_WINDOW_NAME "Graphics"
 #define TCS_INIVAR_STATNAM "G2dStatus"
    #define TCS_STATWINDOW_NAME "System Messages"
 #define TCS_INIVAR_HDCNAM "G2dHardcopy"
    #if (JOURNALTYP ==1)
       #define TCS_HDCFILE_NAME "HDC%03i.WMF"
    #elif (JOURNALTYP ==2)
       #define TCS_HDCFILE_NAME "HDC%03i.EMF"
    #elif (JOURNALTYP ==3)
       #define TCS_HDCFILE_NAME "HDC%03i.HDC"
    #else
       #define TCS_HDCFILE_NAME "HDC%03i.UNKNOWN"
    #endif

#define TCS_INISECT2 "Layout"
 #define TCS_INIVAR_COPMEN "G2dSysMenuCopy"
    #define TCS_INIDEF_COPMEN "Copy"
 #define TCS_INIVAR_FONT "G2dGraphicFont"
    #define TCS_INIDEF_FONT PROGDIRTOKEN "graph2d"
 #define TCS_INIVAR_SYSFONT "G2dSystemFont"
    #define TCS_INIDEF_SYSFONT PROGDIRTOKEN "graph2d"
 #define TCS_INIVAR_WINPOSX "G2dGraphicPosX"
    #define TCS_INIDEF_WINPOSX 1
 #define TCS_INIVAR_WINPOSY "G2dGraphicPosY"
    #define TCS_INIDEF_WINPOSY 3
 #define TCS_INIVAR_WINSIZX "G2dGraphicSizeX"
    #define TCS_INIDEF_WINSIZX 98
 #define TCS_INIVAR_WINSIZY "G2dGraphicSizeY"
    #define TCS_INIDEF_WINSIZY 85
 #define TCS_INIVAR_STATPOSX "G2dStatusPosX"
    #define TCS_INIDEF_STATPOSX 1
 #define TCS_INIVAR_STATPOSY "G2dStatusPosY"
    #define TCS_INIDEF_STATPOSY 91
 #define TCS_INIVAR_STATSIZX "G2dStatusSizeX"
    #define TCS_INIDEF_STATSIZX 98
 #define TCS_INIVAR_STATSIZY "G2dStatusSizeY"
    #define TCS_INIDEF_STATSIZY 3   // mit X11 o.k.
//    #define TCS_INIDEF_STATSIZY 0 // sonst nur 1 Fenster
 #define TCS_INIVAR_LINCOL "G2dLinCol"
    #define TCS_INIDEF_LINCOL 1
 #define TCS_INIVAR_TXTCOL "G2dTxtCol"
    #define TCS_INIDEF_TXTCOL 1
 #define TCS_INIVAR_BCKCOL "G2dBckCol"
    #define TCS_INIDEF_BCKCOL 0

#define TCS_INISECT3 "Messages"
 #define TCS_INIVAR_UNKNGRAPHCARD "G2dGraphCard"
    #define TCS_INIDEF_UNKNGRAPHCARD "GRAPH2D Video System: Error %s."
    #define TCS_INIVAR_UNKNGRAPHCARDL "G2dGraphCardL"
    #define TCS_INIDEF_UNKNGRAPHCARDL 10
 #define TCS_INIVAR_NOFNTFIL "G2dFntfilOpen"
    #define TCS_INIDEF_NOFNTFIL "GRAPH2D SDLTTF: Error opening Fontfile %s."
    #define TCS_INIVAR_NOFNTFILL "G2dFntfilOpenL"
    #define TCS_INIDEF_NOFNTFILL 10
 #define TCS_INIVAR_NOFNT "G2dFntfilOpen"
    #define TCS_INIDEF_NOFNT "GRAPH2D SDLTTF: Error -> %s."
    #define TCS_INIVAR_NOFNTL "G2dFntfilOpenL"
    #define TCS_INIDEF_NOFNTL 10
 #define TCS_INIVAR_HDCOPN "G2dHdcOpen"
    #define TCS_INIDEF_HDCOPN "GRAPH2D HARDCOPY: Error during OPEN."
    #define TCS_INIVAR_HDCOPNL "G2dHdcOpenL"
    #define TCS_INIDEF_HDCOPNL 5
 #define TCS_INIVAR_HDCWRT "G2dHdcWrite"
    #define TCS_INIDEF_HDCWRT "GRAPH2D HARDCOPY: Error during WRITE."
    #define TCS_INIVAR_HDCWRTL "G2dHdcWriteL"
    #define TCS_INIDEF_HDCWRTL 5
 #define TCS_INIVAR_HDCINT "G2dHdcIntern"
    #define TCS_INIDEF_HDCINT "GRAPH2D HARDCOPY: Internal Error."
    #define TCS_INIVAR_HDCINTL "G2dHdcInternL"
    #define TCS_INIDEF_HDCINTL 5
 #define TCS_INIVAR_USR "G2dUser"
    #define TCS_INIDEF_USR "%s"
    #define TCS_INIVAR_USRL "G2dUserL"
    #define TCS_INIDEF_USRL 5
 #define TCS_INIVAR_HDCACT "G2dHdcActive"
    #define TCS_INIDEF_HDCACT "Hardcopy in progress: File %s created."
    #define TCS_INIVAR_HDCACTL "G2dHdcActiveL"
    #define TCS_INIDEF_HDCACTL 1
 #define TCS_INIVAR_USRWRN "G2dPressAny"
    #define TCS_INIDEF_USRWRN "Press any key to continue."
    #define TCS_INIVAR_USRWRNL "G2dPressAnyL"
    #define TCS_INIDEF_USRWRNL 5
 #define TCS_INIVAR_EXIT "G2dExit"
    #define TCS_INIDEF_EXIT "Press any key to exit program."
    #define TCS_INIVAR_EXITL "G2dExitL"
    #define TCS_INIDEF_EXITL 10
 #define TCS_INIVAR_COPMEM "G2dNoMemory"
    #define TCS_INIDEF_COPMEM "GRAPH2D Clipboard Manager: Out of Memory."
    #define TCS_INIVAR_COPMEML "G2dNoMemoryL"
    #define TCS_INIDEF_COPMEML 1
 #define TCS_INIVAR_COPLCK "G2dClipLock"
    #define TCS_INIDEF_COPLCK "GRAPH2D Clipboard Manager: ClipBoard locked."
    #define TCS_INIVAR_COPLCKL "G2dClipLockL"
    #define TCS_INIDEF_COPLCKL 1
 #define TCS_INIVAR_JOUCREATE "G2dJouCreate"
    #define TCS_INIDEF_JOUCREATE "GRAPH2D Error Creating Journal. Error-No: %s."
    #define TCS_INIVAR_JOUCREATEL "G2dJouCreateL"
    #define TCS_INIDEF_JOUCREATEL 5
 #define TCS_INIVAR_JOUENTRY "G2dJouEntry"
    #define TCS_INIDEF_JOUENTRY "GRAPH2D Error Creating Journal Entry."
    #define TCS_INIVAR_JOUENTRYL "G2dJouEntryL"
    #define TCS_INIDEF_JOUENTRYL 5
 #define TCS_INIVAR_JOUADD "G2dJouAdd"
    #define TCS_INIDEF_JOUADD "GRAPH2D Error Appending Journal Entry."
    #define TCS_INIVAR_JOUADDL "G2dJouAddL"
    #define TCS_INIDEF_JOUADDL 5
 #define TCS_INIVAR_JOUCLR "G2dJouClr"
    #define TCS_INIDEF_JOUCLR "GRAPH2D Error Clearing Journal Entry."
    #define TCS_INIVAR_JOUCLRL "G2dJouClrL"
    #define TCS_INIDEF_JOUCLRL 5
 #define TCS_INIVAR_JOUUNKWN "G2dJouEntryUnknwn"
    #define TCS_INIDEF_JOUUNKWN "GRAPH2D Unknown Journal Entry."
    #define TCS_INIVAR_JOUUNKWNL "G2dJouEntryUnknwnL"
    #define TCS_INIDEF_JOUUNKWNL 5
 #define TCS_INIVAR_XMLPARSER "G2dXMLerror"
    #define TCS_INIDEF_XMLPARSER "GRAPH2D Error parsing XML-File: %s"
    #define TCS_INIVAR_XMLPARSERL "G2dXMLerrorL"
    #define TCS_INIDEF_XMLPARSERL 8
 #define TCS_INIVAR_XMLOPEN "G2dXMLopen"
    #define TCS_INIDEF_XMLOPEN "GRAPH2D Error opening %s"
    #define TCS_INIVAR_XMLOPENL "G2dXMLerrorL"
    #define TCS_INIDEF_XMLOPENL 8
 #define TCS_INIVAR_UNKNAUDIO "G2dAudio"
    #define TCS_INIDEF_UNKNAUDIO "GRAPH2D Audio System: Error %s."
    #define TCS_INIVAR_UNKNAUDIOL "G2dAudioL"
    #define TCS_INIDEF_UNKNAUDIOL 5
 #define TCS_INIVAR_USR2 "G2dUser2"
    #define TCS_INIDEF_USR2 "%s"
    #define TCS_INIVAR_USR2L "G2dUser2L"
    #define TCS_INIDEF_USR2L 5
 #define TCS_INIVAR_INI2 "G2d2xInitt"
    #define TCS_INIDEF_INI2 "%s"
    #define TCS_INIVAR_INI2L "G2d2xInittL"
    #define TCS_INIDEF_INI2L 5
