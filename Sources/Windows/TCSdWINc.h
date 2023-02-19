/** ****************************************************************************
\file       TCSdWINc.h
\brief      MS Windows Port: Low-Level Driver
\version    1.8
\author     (C) 2022 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Headerfile zu TCSdWINc.c
\note
         Typ-, Konstantendefinitionen und Steuerung C++ / C
\~english
         Headerfile for TCSdWIN.c
\note
         Declarations and adaption to C++ vs. C
\~


***************************************************************************** */


/* ---- Zeichenbereich im Tektronix-Koordinatensystem --------------------- */

#define TEK_XMAX 1023
#define TEK_YMAX 780

/* ---- Erhoehung der Zeichenauflösung fuer hochaufloesende Bildschirme --- */

#if defined PixFac
 #define HiRes(iX) (iX*PixFac)
 #define LoRes(iX) (iX/PixFac)
#else
 #define HiRes(iX) iX
 #define LoRes(iX) iX
#endif



/* ------------------ Kompatibilität 16/32bit ----------------------------- */

#if !defined(__WIN32__) && !defined(_WIN32)

 typedef char TCHAR, *PTCHAR;
 #define LPTSTR LPSTR

 #define EXPORT16 __export /* __export bei virtuellem Adressraum unnötig */
 #define SM_CXMAXIMIZED SM_CXFULLSCREEN  /* notduerftiger Ersatz für ... */
 #define SM_CYMAXIMIZED SM_CYFULLSCREEN  /* ...Win32 Funktion */
 #define GetCommandLine() "WinApp"       /* dito */

#else
 #define EXPORT16
#endif


/* ------------ Compilerspezifische Definitionen -------------------------- */

// ____________________________ Open-Watcom _______________________________
#if defined __WATCOMC__
 #ifdef _UNICODE
  #error "Watcom Ftn77 basiert nicht auf UNICODE !!!"
 #endif

 #if !defined(__WIN32__) && !defined(_WIN32)
  #define TCSLEV3SYS 3 // TCSLEV(3) = 3 fuer Watcom/16 bit Windows
 #else
  #define TCSLEV3SYS 4 // TCSLEV(3) = 4 fuer Watcom/32 bit Windows
 #endif

 /* Deklaration Parameteruebergabe Fortran <-> C */
 typedef long int LOGICAL;
 typedef long int FTNINT;
 typedef float FTNREAL;
 typedef double FTNDOUBLE;
 typedef struct {float real, imag;} FTNCOMPLEX;
 typedef char FTNCHAR;
 typedef unsigned FTNCHARLEN;
 typedef struct { FTNCHAR * addr; FTNCHARLEN len; } FTNSTRDESC;
 typedef FTNSTRDESC FTNSTRPAR;
 #define FTNSTRPAR_TAIL(ftns)
 #define FTNSTRPARA(ftns) ftns->addr
 #define FTNSTRPARL(ftns) ftns->len
 #define CALLFTNSTRA(ftns) & ftns
 #define CALLFTNSTRL(ftns)
 #define FWRDFTNSTRA(ftns) ftns
 #define FWRDFTNSTRL(ftns)

 #pragma aux TKTRNX "^"; /* Fortran Naming Convention */
 #pragma aux tcslev3 "^";
 #pragma aux initt1 "^";
 #pragma aux finitt "^";
 #pragma aux GraphicError "^";
 #pragma aux winlbl "^";
 #pragma aux erase "^";
 #pragma aux swind1 "^";
 #pragma aux movabs "^";
 #pragma aux drwabs "^";
 #pragma aux dshabs "^";
 #pragma aux pntabs "^";
 #pragma aux bckcol "^";
 #pragma aux lincol "^";
 #pragma aux txtcol "^";
 #pragma aux DefaultColour "^"
 #pragma aux outgtext "^";
 #pragma aux italic "^";
 #pragma aux italir "^";
 #pragma aux dblsiz "^";
 #pragma aux nrmsiz "^";
 #pragma aux bell "^";
 #pragma aux outtext "^";
 #pragma aux tinput "^";
 #pragma aux dcursr "^";
 #pragma aux csize "^";
 #pragma aux hdcopy "^";
 #pragma aux lib_movc3 "^";

/* Deklarationen von durch C aufgerufenen FTN77-Unterprogrammen */
 #pragma aux igetarg "^"   // nur WATCOM: F77-Library
 FTNINT igetarg (FTNINT *iNo, FTNSTRDESC *Par);

 #pragma aux initt2 "^"   // nur WATCOM: F77-Library
 void INITT2 (void);

 #pragma aux SUBSTITUTE "^"        // aus STRINGS.FOR
 void SUBSTITUTE (FTNSTRPAR *Src, FTNSTRPAR *Dst, FTNSTRPAR *old, FTNSTRPAR *n
                                  FTNSTRPAR_TAIL(Src) FTNSTRPAR_TAIL(Dst)
                                  FTNSTRPAR_TAIL(old) FTNSTRPAR_TAIL(n));


// ____________________________ GNU-CC ____________________________________
#elif defined __GNUC__
 #ifdef _UNICODE
  #error "GNU f77 basiert nicht auf UNICODE !!!"
 #endif

 #if defined (WINVER)
  #if defined (_WIN64)
   #define TCSLEV3SYS 7 // TCSLEV(3) = 7 fuer GCC / 64bit Windows
  #else
   #define TCSLEV3SYS 5 // TCSLEV(3) = 5 fuer GCC / Windows
  #endif // defined
 #else
  #define TCSLEV3SYS 0 // TCSLEV(3) = 0 fuer unknown
 #endif

/* Deklaration Parameteruebergabe Fortran <-> C */

 //  #include <g2c.h> // nur fuer g77, fuer gfortran s.u.
 typedef long int logical; // 3 (mit ftnlen) plattformabhaengige Definitionen
 typedef long int integer; // Ersatz fuer g2c.h: evtl. ueberpruefen

 typedef logical LOGICAL;
 typedef integer FTNINT;
 typedef float FTNREAL;
 typedef double FTNDOUBLE;
 typedef struct {float real, imag;} FTNCOMPLEX;

 typedef TCHAR FTNCHAR;
 #if __GNUC__ > 7  // GCC V7: size_t definiert, bei win64 8 Byte, nicht 4!
  typedef size_t ftnlen;  // Ersatz fuer g2c.h
  typedef size_t FTNCHARLEN;
 #else
  typedef long int ftnlen;  // Ersatz fuer g2c.h
  typedef ftnlen FTNCHARLEN; // size_t erst ab GCC > 7 definiert
 #endif

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
 #define getarg getarg_       // aus GNU F77-Library
 FTNINT GETARG (FTNINT *iNo, FTNCHAR *line, FTNCHARLEN line_len);

 #define initt2 initt2_
 void INITT2 (void);

 #define SUBSTITUTE substitute_ // universeller Aufruf Watcom/GNU moeglich
 void SUBSTITUTE (FTNSTRPAR *Src, FTNSTRPAR *Dst, FTNSTRPAR *old, FTNSTRPAR *new
                                           FTNSTRPAR_TAIL(Src) FTNSTRPAR_TAIL(Dst)
                                           FTNSTRPAR_TAIL(old) FTNSTRPAR_TAIL(new));

#endif
// ____________Ende systemabhaengige Deklarationen____________________


/* Forward Deklarationen: Codiert in C und auch in C verwendet */

void bell (void); //  -> Forward Deklaration
void outtext(FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) );
void GraphicError (FTNINT *iErr, FTNSTRPAR *ftn_string,
                   FTNINT *iL  FTNSTRPAR_TAIL(ftn_string));
// void dcursr (FTNINT *ic,FTNINT *ix,FTNINT *iy);
void tinput (FTNINT *ic);
void finitt (); // ueberpruefen !!!


/* Systemparameter */

#define MOUSE_XMAX 65535        /* Mousekoordinatensystem (Mickeys) */
#define MOUSE_YMAX 65535        /* s. MS-Dokumentation mouse_event */

#define TCS_WM_COPY 0x0401      /* Raum für Applikationen: 0x0400-0x7fff */

/* ------------ Programmparameter ----------------------------------------- */

#define STAT_MAXROWS 25         /* Gemerkte Statuszeilen (scrollbar) */
#define STAT_MAXCOLUMNS 80
#define STAT_MINLINES 1         /* Default: Angezeigte Statuszeilen */
#define STAT_ADDLINES 9         /* Zusätzlich durch Mausziehen anzeigbar */
#define STAT_PAGESIZ 5          /* Scrollschritte bei großem Statusfenster */

#define TCS_REL_CHR_HEIGHT 1.0f
#define TCS_REL_CHR_SPACE 1.1f  /* Zeilenabstand */

#define TCS_WINDOW_NAMELEN 255
#define TCS_FILE_NAMELEN 128
#define TCS_MESSAGELEN 80
#define TCS_MENUENTRY_LEN 15

#define INIFILEXTTOKEN _T(".%")      /* Token fuer den Filenamenparser */
#define PROGDIRTOKEN _T("%:")

#define TCS_WINDOWCLASS _T("Graph2DWindow")
#define TCS_STAT_WINDOWCLASS _T("Graph2DstatWindow")
#define TCS_DEFAULT_MAINWINDOWCLASS _T("WinMainFTN77")
#define TCS_INIFILE_NAME _T("Graph2D")
#define TCS_WINDOW_ICON _T("Graph2DIcon")
#define TCS_WINDOW_ICONS _T("Graph2DIconS")



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
    alle Parser (*.ini bei INITT1(), Registry bei StoreIni() und 
	*.xml bei sax_callback() beruecksichtigen! */

#define TCS_INISECT0 "Graph2D" // Root-Section, derzeit nur bei XML verwendet

#define TCS_INISECT1 _T("Names")
 #define TCS_INIVAR_WINNAM _T("G2dGraphic")
    #define TCS_WINDOW_NAME _T("Graphics")
 #define TCS_INIVAR_STATNAM _T("G2dStatus")
    #define TCS_STATWINDOW_NAME _T("System Messages")
 #define TCS_INIVAR_HDCNAM _T("G2dHardcopy")
    #if (JOURNALTYP ==1)
       #define TCS_HDCFILE_NAME _T("HDC%03i.WMF")
    #elif (JOURNALTYP ==2)
       #define TCS_HDCFILE_NAME _T("HDC%03i.EMF")
    #elif (JOURNALTYP ==3)
       #define TCS_HDCFILE_NAME _T("HDC%03i.HDC")
    #else
       #define TCS_HDCFILE_NAME _T("HDC%03i.UNKNOWN")
    #endif
 #define TCS_INIVAR_MAINWINNAM _T("G2dMainWindow")
    #define TCS_MAINWINDOW_NAME _T("%:")

#define TCS_INISECT2 _T("Layout")
 #define TCS_INIVAR_COPMEN _T("G2dSysMenuCopy")
    #define TCS_INIDEF_COPMEN _T("Copy")
 #define TCS_INIVAR_FONT _T("G2dGraphicFont")
    #define TCS_INIDEF_FONT _T("Arial Terminal")
 #define TCS_INIVAR_SYSFONT _T("G2dSystemFont")
    #define TCS_INIDEF_SYSFONT _T("Arial Terminal")
 #define TCS_INIVAR_ICONNAM _T("G2dIcon")
    #define TCS_ICONFILE_NAME _T("")
 #define TCS_INIVAR_WINPOSX _T("G2dGraphicPosX")
    #define TCS_INIDEF_WINPOSX 0
 #define TCS_INIVAR_WINPOSY _T("G2dGraphicPosY")
    #define TCS_INIDEF_WINPOSY 0
 #define TCS_INIVAR_WINSIZX _T("G2dGraphicSizeX")
    #define TCS_INIDEF_WINSIZX 100
 #define TCS_INIVAR_WINSIZY _T("G2dGraphicSizeY")
    #define TCS_INIDEF_WINSIZY 100
 #define TCS_INIVAR_STATPOSX _T("G2dStatusPosX")
    #define TCS_INIDEF_STATPOSX 0
 #define TCS_INIVAR_STATPOSY _T("G2dStatusPosY")
    #define TCS_INIDEF_STATPOSY 0
 #define TCS_INIVAR_STATSIZX _T("G2dStatusSizeX")
    #define TCS_INIDEF_STATSIZX 100
 #define TCS_INIVAR_STATSIZY _T("G2dStatusSizeY")
    #define TCS_INIDEF_STATSIZY 100
 #define TCS_INIVAR_LINCOL _T("G2dLinCol")
    #define TCS_INIDEF_LINCOL 1
 #define TCS_INIVAR_TXTCOL _T("G2dTxtCol")
    #define TCS_INIDEF_TXTCOL 1
 #define TCS_INIVAR_BCKCOL _T("G2dBckCol")
    #define TCS_INIDEF_BCKCOL 0

#define TCS_INISECT3 _T("Messages")
 #define TCS_INIVAR_HDCOPN _T("G2dHdcOpen")
    #define TCS_INIDEF_HDCOPN _T("GRAPH2D HARDCOPY: Error during OPEN.")
    #define TCS_INIVAR_HDCOPNL _T("G2dHdcOpenL")
    #define TCS_INIDEF_HDCOPNL 5
 #define TCS_INIVAR_HDCWRT _T("G2dHdcWrite")
    #define TCS_INIDEF_HDCWRT _T("GRAPH2D HARDCOPY: Error during WRITE.")
    #define TCS_INIVAR_HDCWRTL _T("G2dHdcWriteL")
    #define TCS_INIDEF_HDCWRTL 5
 #define TCS_INIVAR_HDCINT _T("G2dHdcIntern")
    #define TCS_INIDEF_HDCINT _T("GRAPH2D HARDCOPY: Internal Error.")
    #define TCS_INIVAR_HDCINTL _T("G2dHdcInternL")
    #define TCS_INIDEF_HDCINTL 5
 #define TCS_INIVAR_USR _T("G2dUser")
    #define TCS_INIDEF_USR _T("%s")
    #define TCS_INIVAR_USRL _T("G2dUserL")
    #define TCS_INIDEF_USRL 5
 #define TCS_INIVAR_HDCACT _T("G2dHdcActive")
    #define TCS_INIDEF_HDCACT _T("Hardcopy in progress: File %s created.")
    #define TCS_INIVAR_HDCACTL _T("G2dHdcActiveL")
    #define TCS_INIDEF_HDCACTL 1
 #define TCS_INIVAR_USRWRN _T("G2dPressAny")
    #define TCS_INIDEF_USRWRN _T("Press any key to continue.")
    #define TCS_INIVAR_USRWRNL _T("G2dPressAnyL")
    #define TCS_INIDEF_USRWRNL 5
 #define TCS_INIVAR_EXIT _T("G2dExit")
    #define TCS_INIDEF_EXIT _T("Press any key to exit program.")
    #define TCS_INIVAR_EXITL _T("G2dExitL")
    #define TCS_INIDEF_EXITL 10
 #define TCS_INIVAR_COPMEM _T("G2dNoMemory")
    #define TCS_INIDEF_COPMEM _T("GRAPH2D Clipboard Manager: Out of Memory.")
    #define TCS_INIVAR_COPMEML _T("G2dNoMemoryL")
    #define TCS_INIDEF_COPMEML 1
 #define TCS_INIVAR_COPLCK _T("G2dClipLock")
    #define TCS_INIDEF_COPLCK _T("GRAPH2D Clipboard Manager: ClipBoard locked.")
    #define TCS_INIVAR_COPLCKL _T("G2dClipLockL")
    #define TCS_INIDEF_COPLCKL 1
 #define TCS_INIVAR_JOUCREATE _T("G2dJouCreate")
    #define TCS_INIDEF_JOUCREATE _T("GRAPH2D Error Creating Journal. Error-No: %s.")
    #define TCS_INIVAR_JOUCREATEL _T("G2dJouCreateL")
    #define TCS_INIDEF_JOUCREATEL 5
 #define TCS_INIVAR_JOUENTRY _T("G2dJouEntry")
    #define TCS_INIDEF_JOUENTRY _T("GRAPH2D Error Creating Journal Entry.")
    #define TCS_INIVAR_JOUENTRYL _T("G2dJouEntryL")
    #define TCS_INIDEF_JOUENTRYL 5
 #define TCS_INIVAR_JOUADD _T("G2dJouAdd")
    #define TCS_INIDEF_JOUADD _T("GRAPH2D Error Appending Journal Entry.")
    #define TCS_INIVAR_JOUADDL _T("G2dJouAddL")
    #define TCS_INIDEF_JOUADDL 5
 #define TCS_INIVAR_JOUCLR _T("G2dJouClr")
    #define TCS_INIDEF_JOUCLR _T("GRAPH2D Error Clearing Journal Entry.")
    #define TCS_INIVAR_JOUCLRL _T("G2dJouClrL")
    #define TCS_INIDEF_JOUCLRL 5
 #define TCS_INIVAR_JOUUNKWN _T("G2dJouEntryUnknwn")
    #define TCS_INIDEF_JOUUNKWN _T("GRAPH2D Unknown Journal Entry.")
    #define TCS_INIVAR_JOUUNKWNL _T("G2dJouEntryUnknwnL")
    #define TCS_INIDEF_JOUUNKWNL 1
 #define TCS_INIVAR_XMLPARSER _T("G2dXMLerror")
    #define TCS_INIDEF_XMLPARSER _T("GRAPH2D Error parsing XML-File: %s")
    #define TCS_INIVAR_XMLPARSERL _T("G2dXMLerrorL")
    #define TCS_INIDEF_XMLPARSERL 8
 #define TCS_INIVAR_XMLOPEN _T("G2dXMLopen")
    #define TCS_INIDEF_XMLOPEN _T("GRAPH2D Error opening %s")
    #define TCS_INIVAR_XMLOPENL _T("G2dXMLerrorL")
    #define TCS_INIDEF_XMLOPENL 8
 #define TCS_INIVAR_USR2 _T("G2dUser2")
    #define TCS_INIDEF_USR2 _T("%s")
    #define TCS_INIVAR_USR2L _T("G2dUser2L")
    #define TCS_INIDEF_USR2L 5
 #define TCS_INIVAR_INI2 _T("G2d2xInitt")
    #define TCS_INIDEF_INI2 _T("%s")
    #define TCS_INIVAR_INI2L _T("G2d2xInittL")
    #define TCS_INIDEF_INI2L 5


/* ------------ Steuerung C++: Klassendefinition / C: Unterprogramme ------ */

#ifdef __cplusplus

class TCSdrWIN
{
public:
            TCSdrWIN();
  virtual   ~TCSdrWIN();

            tcslev3 (FTNINT *SysLev);
            winlbl (FTNSTRDESC * PloWinNam, FTNSTRDESC * StatWinNam,
                FTNSTRDESC * IniFilNam, FTNINT *hIcon, FTNINT hIn, FTNINT hPrevIn);

            initt1 (HINSTANCE *hParentInstance);
            finitt ();
            erase ();
            swindo (FTNINT *ix,FTNINT *iLx, FTNINT *iy,FTNINT *iLy);
            swind1 (FTNINT *ix,FTNINT *iLx, FTNINT *iy,FTNINT *iLy);
            movabs (FTNINT *ix,FTNINT *iy);
            drwabs (FTNINT *ix,FTNINT *iy);
            dshabs (FTNINT *ix,FTNINT *iy, FTNINT *iMask);
            pntabs (FTNINT *ix,FTNINT *iy);
            bckcol (FTNINT *iCol);
            lincol (FTNINT *iCol);
            txtcol (FTNINT *iCol);
            DefaultColour ();
            outgtext(FTNSTRDESC * ftn_string);
            italic ();
            italir ();
            dblsiz ();
            nrmsiz ();
  static    bell ();
  static    outtext (FTNSTRDESC * ftn_string);
            tinput (FTNINT *ic);
            dcursr (FTNINT *ic,FTNINT *ix,FTNINT *iy);
            GraphicErrorMsg (FTNINT *iErr, FTNSTRDESC *ftn_string, FTNINT *iL);
            csize (FTNINT *ix,FTNINT *iy);
            hdcopy ();
            lib_movc3 (FTNINT *len,FTNSTRDESC *sou,FTNSTRDESC *dst);
};

 #define TCSdrWIN__ TCSdrWIN:: /* zur Vereinheitlichung C++ und C */

#else /* __cplusplus */

 #define TCSdrWIN__

 typedef int bool;
 #define false 0
 #define true !false

#endif /* not __cplusplus */

