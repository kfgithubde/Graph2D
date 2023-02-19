/** ****************************************************************************
\file       TCSdWINc.c
\brief      MS Windows Port: Low-Level Driver
\version    1.96
\author     (C) 2022 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Systemnahe Graphikroutinen für das Tektronix Graphiksystem
\note \verbatim
  TCSdWINc.c    : In C programmierte Routinen

  TCSdrWIN.cpp  : Implementierung der Klasse TCSdrWIN.
                  Das File ist identisch mit TCSdrWIN.c.
\endverbatim
\~english
         system-specific subroutines of the teklib-library
\note \verbatim
  TCSdWINc.c    : Routines programmed in C.

  TCSdrWIN.cpp  : Implementation of class TCSdrWIN.
                  The file is identical to TCSdrWIN.c
 \endverbatim
\~
***************************************************************************** */

/*
        Anmerkungen:
          1. Die Systemmeldungen erfolgen in einem eigenen, im Regelfall
             unsichtbaren, Fenster. Durch Drücken der rechten Maustaste
             im Graphikfenster kann es sichtbar gemacht werden, durch
             Setzen des Fokus auf das Graphikfenster verschwindet es wieder.
             Bei aktiviertem GIN-Cursor kann die Umschaltung über der Titel-
             zeile erfolgen.
          2. Die Art der Protokollierung zum Neuzeichnen eines Fensters wird
             durch die Konstante JOURNALTYP gesteuert:
             --- JOURNALTYP 1 ---
             Die Zeichenbefehle werden mithilfe eines Metafiles im Speicher
             aufgezeichnet. Das Abspielen eines Metafiles in ein anderes führt
             bei Windows bis 3.0 einschließlich zum Systemabsturz! Ab Windows
             3.1 aufwärts ist das Problem behoben. Mögliche Abhilfe bei Windows
             3.0: Verwendung von Festplatten-basierten Metafiles.
             (lt. MS-SDK Dokumentation).
             --- JOURNALTYP 2: ---
             Anstelle eines Windows-Metafiles (*.wmf) wird ein extended
             Metafile (*.emf) verwendet. Funktion wurde im Hinblick auf das
             64bit-Windows entwickelt, für 32bit Windows entsteht im Vergleich
             zum Journaltyp 1 lediglich ein Performancenachteil.
             Anmerkung: MS-WORD besitzt Filter sowohl für *.wmf als auch *.emf
                        Dateien. Jedoch ist der *.emf-Filter bis WORD 2000 SP1
                        fehlerhaft (Buchstaben des stehen evtl. auf dem Kopf)
                        In Windows XP wird nach jedem Neuskalieren das *.emf
                        Metafile immer größer. Hierdurch dauert das Neuzeich-
                        nen unakzeptabel lange. Dieses Problem tritt bei
                        Windows 2000 nicht auf
                        -> JOURNALFILE 1 bei 32-bit Windows Default.
             --- JOURNALTYP 3: ---
             Die Zeichenbefehle werden in einer Liste aufgezeichnet. Ein
             einzelner Befehl hat den Aufbau
             struct xaction_typ {
                        FTNINT action
                        FTNINT i1
                        FTNINT i2
                                } XACTION;
             Die TCS-Befehle im einzelnen:
                    erase ()
                     XACTION.action= XACTION_ERASE;
                    movabs (ix,iy)
                     XACTION.action= XACTION_MOVABS;
                     XACTION.i1= ix;
                     XACTION.i2= ix;
                    drwabs (ix.iy)
                     XACTION.action= XACTION_DRWABS;
                     XACTION.i1= ix;
                     XACTION.i2= ix;
                    dshabs (ix,iy,iDash)
                     XACTION.action= XACTION_DSHSTYLE;
                     XACTION.i1= iDash;
                     XACTION.action= XACTION_DSHABS;
                     XACTION.i1= ix;
                     XACTION.i2= ix;
                    pntabs (ix,iy)
                     XACTION.action= XACTION_PNTABS;
                     XACTION.i1= ix;
                     XACTION.i2= ix;
                    outgtext (string) - Graphiktext
                     XACTION.action= XACTION_GTEXT;
                     XACTION.i1= iChar;
                     XACTION.i2= iASCII_1;
                     XACTION.action= XACTION_ASCII;
                     XACTION.i1= iASCII_2;
                     XACTION.i2= iASCII_3;
                     ...
                     XACTION.action= XACTION_ASCII;
                     XACTION.i1= iASCII_iChar;
                    italic ()
                     XACTION.action= XACTION_FONTATTR;
                     XACTION.i1= 1; // Attribut 1
                     XACTION.i2= 1; // true
                    italir ()
                     XACTION.action= XACTION_FONTATTR;
                     XACTION.i1= 1; // Attribut 1
                     XACTION.i2= 0; // false
                    dblsiz ()
                     XACTION.action= XACTION_FONTATTR;
                     XACTION.i1= 2; // Attribut 2
                     XACTION.i2= 1; // true
                    nrmsiz ()
                     XACTION.action= XACTION_FONTATTR;
                     XACTION.i1= 2; // Attribut 2
                     XACTION.i2= 0; // false

                    bckcol (iCol) - keine Zeichenarbeit, nur Commonblock
                    lincol (iCol)
                    txtcol (iCol)
                    DefaultColour () - keine Zeichenarbeit, nur Commonblock

          3. Clipping: Windows erwartet die Angabe der Clipping-region in
             Devicekoordinaten, daher wird die Clipping-Region bei Vergrößern
             und Verzerren des Fensters nicht angepasst. Abhilfe: Implementa-
             tion einer eigen Clippingroutine, gesteuert über den Tektronix-
             Commonblock. Die (funktionierende) Definition der Clippingregion
             bei Ausgabe in die Zwischenablage wird so überflüssig.
          4. Linestyle in der Regel nur durchgezogen (wird auch durch LINCOL
             zurückgesetzt) -> Merken nicht nötig. Die aktuelle Farbe muß
             jedoch für DASH gemerkt werden!!!
          5. Übergabe der Windows-Instanz:
             A.  Subroutine INITT (iDummy) ruft GetMainInstAndWin auf und
                 speichert Instanz und Windowhandle durch SaveMainInstAndWin.
             B.  Übergabe des Instanz-Handlers als Parameter von INITT1 (hInst)
                 Der Aufruf von INITT1 kann auch mehrmals erfolgen, d.h. möglich
                 ist ein Aufruf von INITT1 durch ein C-Hauptprogramm und ein
                 erneuter INITT1-Aufruf durch FORTRAN-Unterprogramm. Hier gilt
                 dann der erste Aufruf, also die durch C übergebene Instanz.
             C.  Zur Vereinfachung der Programmentwicklung mit MS-Visual C++
                 wird bei INITT1(0) und Kompilierung durch den MS-Compiler
                 die Standardvariable hInst des Visual Studio verwendet.
          6. Initialisierung erfolgt in dem File GRAPH2D.INI
             Default: im Windows-Directory (c:\WINNT)
          7. Abweichend zur DOS-Version entspricht der Farbindex 0 weiss
             (Hintergrund) und der Index 1 schwarz.
          8. Bei Kompilierung als Konsolenanwendung oder als Window-Anwendung
             ohne Default-Windowsystem Fehler möglich. Debuggen durch
             Definition von "extended_error_handling".
             Ursache: fehlendes Fenster für das Hauptprogramm, Fehler ist
             jetzt behoben.
          9. Bei Watcom-Compiler den C-Teil ohne Optimierung compilieren!!!
         10. Getestete Compiler: WATCOM 11.0c, OpenWatcom 1.0 - 2.0.
             Bei neuen Compilern erst mit #define trace_calls übersetzen.
             Prüfen, ob __MainWindow definiert!
         11. Anpassungen an GNU-Compiler. Anstelle des Watcom-Defaultwindow-
             systems wird die eigene Routine WinMain.c verwendet.
         12. Auf Wunsch kann das Statusfenster einen privaten Device-Kontext
             erhalten: Definition des Symbols STAT_WINDOW_PRIVATE
         13. Bei mehreren Fenstern des Hauptprogrammes kann durch <Alt><F6>
             zwischen den einzelnen Fenstern umgeschaltet werden.
         14. Fuer die 16bit-Version ist das Watcom Default Window System
             notwendig. Bei 32bit ist ab der OpenWatcom Version 1.0 das
             Defaultsystem deaktiviert.
         15. Skalierung des Tektronix-Bildschirmkoordinatensystems (1023/780)
             ist bei Bildschirmen höherer Auflösung nicht ausreichend. Falls
             Anzahl der Bildschirmpixel in x-Richtung größer als 1024*Pixfac
             ist, hinterläßt der Rahmen eines über das Graphikfenster gezogenes
             Fensters horizontale und vertikale dünne Linien, die nach Mini-
             mierung und Neuzeichnen des Graphikfensters verschwinden.
             Vorsicht: PixFac *1024 darf bis einschließlich Windows95 nicht
             den 2-Byte int  Zahlenbereich (-32768...+32767) überschreiten!!!
             Bei PixFac=100 kann derzeit kein Refresh des Bildschirms durchge-
             fuehrt werden, nach erstem Zeichnen der Linie ((0,0)->(1023,780))
             erfolgt kein Neuzeichnen. Nicht nur einzige (?!) Ursache ist die
             Verwendung der 16bit GDI Befehle um METAFILE.
             Falls PixFac nicht definiert wird, erfolgt keine zusaetzliche
             Koordinatentransformation -> Performancegewinn bei alten Systemen.
         16. Im Falle von JOURNALTYP=3 darf der Fehler JOUUNKWN nur als
             Warnung definiert werden (G2dJouEntryUnknwnL= 1), da sonst inner-
             halb von TINPUT ein rekursiver Aufruf von TCSWndProc_OnPaint
             ueber GraphicError erfolgt!
             Dieser Punkt ist ab Version 1.93 mit der Verlagerung der Routine
             GraphicError in den c-Teil behoben.
         17. Die Defaultwerte des *.ini-Files müssen fuer die Initialisierung
             durch die Registry und/oder XML-Files auch bei der Variablen-
             definition angegeben werden, da GetPrivateProfileString nicht
             mehr in jedem Fall aufgerufen wird und somit Variablen evtl.
             nicht mehr vorbelegt sein koennen.
         18. Die Steuerung der Initialisierungmethode erfolgt ueber die File-
             extension des Initialisierungfiles.
             *.INI: Windows Initialisierungsfile
             *.REG: 32bit-Windows Registry
             *.XML: XML-Dateien
             Der Default (steuerbar durch das Extensiontoken .%) wird durch
              #define INIFILEXT _TEXT(".REG")         // win32: Registry
             bestimmt.
             Durch die Definition der Konstanten REGSUPPORT bzw. XMLSUPPORT
             wird der entsprechende Programmteil eingebunden.
         19. Aufgrund eines Bugs in der 32-bit Version von win7 darf eine
             Tastaturabfrage nicht ohne Filter efolgen, also nicht
              GetMessage (&msg, NULL, 0, 0);
             sondern
              GetMessage (&msg, NULL, WM_NULL, WM_USER);
             oder
              GetMessage (&msg, hWIND, 0, 0);
             Die früheren Versionen bis XP und auch die 64bit Version von Win7
             sind hiervon nicht betroffen.
         20. XML-Dateien verwenden i.d.R. UTF-8 Codierungen, deswegen erfolgt
             bei _UNICODE keine Einbindung des XML-Parsers.
         21. Journalfile Typ 3: Die verwendete Listenbibliothek verträgt sich
             nicht mit den Makros LoRes und HiRes. Deswegen darf dann PixFac
             nicht definiert werden.

*/


// #define UNICODE   // fuer Windows-Headerfiles -> jedoch Watcom FTN77 nicht
// #define _UNICODE  // fuer C-Runtime Headerfiles  UNICODEfähig !?!


/*
-------------------- Konfiguration des Zielystems ----------------------
*/

// #define PixFac  30                // s. Kommentar 15, 21
// #define STAT_WINDOW_PRIVATE       // s. Kommentar 12
// #define REGSUPPORT                // s. Kommentar 18
// #define XMLSUPPORT                // s. Kommentar 18
// #define INIFILEXT _TEXT(".XML")   // s. Kommentar 18
// #define JOURNALTYP 3              // s. Kommentar 2, 21

#if !defined(JOURNALTYP) // Defaultwerte, falls nicht oben definiert
 #if !defined(__WIN32__) && !defined(_WIN32)
  /* Defaultvorgabe 16bit: langsame CPU, Aufloesung <= 1024x780 Pxl */
  #define JOURNALTYP 1       // s. Kommentar 2, nur *.wmf implementiert
  #undef PixFac              // s. Kommentar 15, LoRes
  #undef STAT_WINDOW_PRIVATE // s. Kommentar 12
 #else
  // Default 32bit: kein extended Metafile, Auflösung <= 30*1024 x 30*780 Pxl
  #define JOURNALTYP 1       // *.emf hoeherer Overhead -> unnoetig
  #define PixFac  30         // Koordinatentransformation hochauflösende CRT's
  #undef STAT_WINDOW_PRIVATE // s. Kommentar 12
 #endif
#endif

#if !defined(INIFILEXT)
 #if !defined(__WIN32__) && !defined(_WIN32)
  #define INIFILEXT _TEXT(".INI") // s. Kommentar 18, win16: *.ini Dateien
  #undef REGSUPPORT               // Keine vollwertige Registry, nur win.ini
  #undef XMLSUPPORT               // Programmgroesse verringern
 #else
  #define INIFILEXT _TEXT(".REG") // win32: Registry
  #define REGSUPPORT
  #if (defined(__WIN64__) || defined(_WIN64))
   #define XMLSUPPORT
  #else
   #undef XMLSUPPORT
  #endif
 #endif
#endif

#if (JOURNALTYP == 3)
 #undef PixFac                   // s. Kommentar 21
#endif

#if defined(UNICODE) || defined(_UNICODE)
 #undef XMLSUPPORT               // s. Kommentar 20
#endif

/*
-------------------- Headerfiles ---------------------------------------
*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>   // Muss unbedingt vor den Standard C-Headern stehen, da
#include <windowsx.h>  // hier NULL fuer 16bit Windows als 0 definiert wird

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <tchar.h>     // Public Domain ueber MINGW-Package, nicht nur MS

#if defined(__WATCOMC__) && defined(__SW_BW)
 #include <wdefwin.h>   // Compilerswitch -bw: Watcom Default Window System
#endif

#ifdef XMLSUPPORT
 #include  "mxml.h"
#endif

#if (JOURNALTYP == 3)
 #include "sglib.h"
#endif

#include "TCSdWINc.h"
#include "TKTRNX.h"

/*
-------------------- Debug Compiler Switches ---------------------------
*/

// #define extended_error_handling
#if !defined(__WIN32__) && !defined(_WIN32)
  #undef extended_error_handling
#endif

// #define trace_calls
/* Debug-Messageboxen / Compilermessages, nach include definieren! */

#ifdef trace_calls

 #ifdef __WATCOMC__
  #if (__WATCOMC__ == 1100)
   #pragma message ( "Symbol __WATCOMC__ defined to 1100 (Version 11.0c)")
  #elif (__WATCOMC__ >= 1200)
   #pragma message ( "Symbol __WATCOMC__ defined (OpenWatcom Version >= 1.0)")
  #else
   /* Andere Versionen noch nicht getestet! */
   #pragma message ( "Untested Version: Symbol __WATCOMC__ defined to :")
   #pragma message (__WATCOMC__) // Erzwingen Fehler zur Erweiterung
  #endif
  #if !defined(__WIN32__) && !defined(_WIN32)
   #pragma message ( "16 bit Windows" )
  #else
   #pragma message ( "32 bit Windows" )
  #endif
 #endif

 #ifdef _MSC_VER
  #pragma message ( "Symbol _MSC_VER defined" )
  #if !defined(__WIN32__) && !defined(_WIN32)
   #pragma message ( "16 bit Windows" )
  #else
   #pragma message ( "32 bit Windows" )
  #endif
 #endif

 #ifdef __GNUC__
  #warning "GNU-Compiler"
  #if !defined(__WIN32__) && !defined(_WIN32)
   #warning "16 bit Windows"
  #elif !defined(__WIN64__) && !defined(_WIN64)
   #warning "32 bit Windows"
  #else
   #warning "64 bit Windows"
  #endif
 #endif

#endif

/*
-------------- Compilerunabhaengige externe Bezüge ---------------------
*/


extern void CreateMainWindow_IfNecessary (HINSTANCE * hMainProgInst,
                                   HWND * hMainProgWindow, LPTSTR szWinName);


/*
------------------------- Globale Variablen ----------------------------
*/

static  RECT    TCSrect = {0,0, HiRes(TEK_XMAX),HiRes(TEK_YMAX)}; // Plotbereich

static  bool    TCSinitialized = false,
                ClippingNotActive = true,
                TCSStatWindowAutomatic = true;

static  HINSTANCE hTCSInst = NULL;

static  HWND    hTCSWindow = NULL,
                hTCSstatWindow = NULL,
                hOwnerWindow = NULL;

static  HDC     hTCSWindowDC;     // privater DC, gilt ganze Fensterlebensdauer

#if (JOURNALTYP == 1)
 static  HDC    hTCSMetaFileDC;   // Metafile als Recorder für WM_PAINT
#elif (JOURNALTYP == 2)
 static  HDC    hTCSMetaFileDC;   // extended Metafile als Recorder WM_PAINT
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ {struct xJournalEntry_typ * previous;
                           struct xJournalEntry_typ * next;
                           FTNINT action; FTNINT i1; FTNINT i2;};
 static struct xJournalEntry_typ* hTCSJournal = NULL;
                                  // Journal zum Neuzeichnen des Fensters
#endif

#ifdef STAT_WINDOW_PRIVATE
 static HDC     hTCSstatWindowDC;
#endif


static  LOGFONT TCSFontdefinition;

static  HFONT   hTCSFont,
                hTCSSysFont;

static  HPEN    hTCSPen;

static  HCURSOR hGinCurs,
                hMouseCurs;

static  TCHAR   szTCSWindowName[TCS_WINDOW_NAMELEN] = "", // Default TCS_WINDOW_NAME erst in ?? gesetzt
                szTCSstatWindowName[TCS_WINDOW_NAMELEN] = "", // TCS_STATWINDOW_NAME,
                szTCSMainWindowName[TCS_WINDOW_NAMELEN] = TCS_MAINWINDOW_NAME,
                szTCSIniFile[TCS_FILE_NAMELEN] = TCS_INIFILE_NAME INIFILEXT,
                szTCSIconFile[TCS_FILE_NAMELEN] = TCS_ICONFILE_NAME,
                szTCSMenuCopyText[TCS_MENUENTRY_LEN] = TCS_INIDEF_COPMEN,
                szTCSHardcopyFile[TCS_FILE_NAMELEN] = TCS_HDCFILE_NAME,
                szTCSGraphicFont[TCS_FILE_NAMELEN] = TCS_INIDEF_FONT,
                szTCSSysFont[TCS_FILE_NAMELEN] = TCS_INIDEF_SYSFONT,
                szTCSsect0[TCS_FILE_NAMELEN] = TCS_INISECT0;


typedef TCHAR   StatLine[STAT_MAXCOLUMNS+1];
static  StatLine TCSstatTextBuf[STAT_MAXROWS];

static  int     TCSwindowIniXrelpos = TCS_INIDEF_WINPOSX, // rel. Bildschirmpos.
                TCSwindowIniYrelpos = TCS_INIDEF_WINPOSY, // bei Init in %
                TCSwindowIniXrelsiz = TCS_INIDEF_WINSIZX,
                TCSwindowIniYrelsiz = TCS_INIDEF_WINSIZY,
                TCSstatWindowIniXrelpos = TCS_INIDEF_STATPOSX, // dito
                TCSstatWindowIniYrelpos = TCS_INIDEF_STATPOSY, // Statusfenster
                TCSstatWindowIniXrelsiz = TCS_INIDEF_STATSIZX,
                TCSstatWindowIniYrelsiz = TCS_INIDEF_STATSIZY,
                TCSstatScrollY, // Position des sichtbaren Scrollbereichs
                TCSstatOrgY,    // Ursprung des log. Koordinatensystems
                TCSstatCursorPosY,
                TCSstatRow,
                TextLineHeight,
                TCSCharHeight,
                TCSBackgroundColour,
                TCSDefaultLinCol = TCS_INIDEF_LINCOL,
                TCSDefaultTxtCol = TCS_INIDEF_TXTCOL,
                TCSDefaultBckCol = TCS_INIDEF_BCKCOL,
                iHardcopyCount =1;  // Zähler zur Erzeugung Filenamen

static  POINT   TCSGinCurPos = { TEK_XMAX / 2, TEK_YMAX / 2};


/* Zuordnung Fehlernummern zu Meldungen,  */

typedef TCHAR   ErrMsg[STAT_MAXCOLUMNS];
static  ErrMsg  szTCSErrorMsg[(int) MSG_MAXERRNO+1] =
                {_T("Element 0 unused"),_T("DOS"),_T("DOS"),_T("DOS"),
                 _T("DOS"),_T("DOS"),   // Errno 0..5
                TCS_INIDEF_HDCOPN,      // Errno 6
                TCS_INIDEF_HDCWRT,      // Errno 7
                TCS_INIDEF_HDCINT,      // Errno 8
                TCS_INIDEF_USR,         // Errno 9
                TCS_INIDEF_HDCACT,      // Errno 10
                TCS_INIDEF_USRWRN,      // Errno 11
                TCS_INIDEF_EXIT,        // Errno 12
                TCS_INIDEF_COPMEM,      // Errno 13
                TCS_INIDEF_COPLCK,      // Errno 14
                TCS_INIDEF_JOUCREATE,   // Errno 15
                TCS_INIDEF_JOUENTRY,    // Errno 16
                TCS_INIDEF_JOUADD,      // Errno 17
                TCS_INIDEF_JOUCLR,      // Errno 18
                TCS_INIDEF_JOUUNKWN,    // Errno 19
                TCS_INIDEF_XMLPARSER,   // Errno 20
                TCS_INIDEF_XMLOPEN,     // Errno 21
                _T("SDL"),
                TCS_INIDEF_USR2,        // Errno 23
                TCS_INIDEF_INI2,        // Errno 24
                _T("Maxerr only for internal Use") };

static  int     TCSErrorLev[(int) MSG_MAXERRNO+1] =
                {10,10,10,10,10,10,
                TCS_INIDEF_HDCOPNL,     // Errno 6
                TCS_INIDEF_HDCWRTL,     // Errno 7
                TCS_INIDEF_HDCINTL,     // Errno 8
                TCS_INIDEF_USRL,        // Errno 9
                TCS_INIDEF_HDCACTL,     // Errno 10
                TCS_INIDEF_USRWRNL,     // Errno 11
                TCS_INIDEF_EXITL,       // Errno 12
                TCS_INIDEF_COPMEML,     // Errno 13
                TCS_INIDEF_COPLCKL,     // Errno 14
                TCS_INIDEF_JOUCREATEL,  // Errno 15
                TCS_INIDEF_JOUENTRYL,   // Errno 16
                TCS_INIDEF_JOUADDL,     // Errno 17
                TCS_INIDEF_JOUCLRL,     // Errno 18
                TCS_INIDEF_JOUUNKWNL,   // Errno 19
                TCS_INIDEF_XMLPARSERL,  // Errno 20
                TCS_INIDEF_XMLOPENL,    // Errno 21
                10,
                TCS_INIDEF_USR2L,       // Errno 23
                TCS_INIDEF_INI2L,       // Errno 24
				10};



/* Zuordnung der Linienarten zu Liniennummern */

static  DWORD dwPenStyle[] = {
                            PS_SOLID,   /* iMask= 0 */
                            PS_DOT,     /* iMask= 1 */
                            PS_DASHDOT, /* iMask= 2 */
                            PS_DASH     /* iMask= 3 */
                           };
#define MAX_PENSTYLE_INDEX 3


/* Zuordnung der Farbennummern zur VGA-Palette */

static  DWORD dwColorTable[] = {
                            RGB (240,240,240), /* iCol= 00: weiss (DOS: 01) */
                            RGB (  0,  0,  0), /* iCol= 01: schwarz(DOS:00) */
                            RGB (240, 80, 80), /* iCol= 02: rot             */
                            RGB ( 80,240, 80), /* iCol= 03: gruen           */
                            RGB ( 80,240,240), /* iCol= 04: blau            */
                            RGB ( 80, 80,240), /* iCol= 05: lila            */
                            RGB (240,240, 80), /* iCol= 06: gelb            */
                            RGB (160,160,160), /* iCol= 07: grau            */
                            RGB (240, 80,240), /* iCol= 08: violett         */
                            RGB (160,  0,  0), /* iCol= 09: mattrot         */
                            RGB (  0,160,  0), /* iCol= 10: mattgruen       */
                            RGB (  0,  0,160), /* iCol= 11: mattblau        */
                            RGB (  0,160,160), /* iCol= 12: mattlila        */
                            RGB (160, 80,  0), /* iCol= 13: orange          */
                            RGB ( 80, 80, 80), /* iCol= 14: mattgrau        */
                            RGB (160,  0,160)  /* iCol= 15: mattviolett     */
                           };
#define MAX_COLOR_INDEX 15



/*
----------------------- Globale Unterprogramme -------------------------
*/



void TCSGraphicError (int iErr, const char* msg)
{
char cBuf[TCS_MESSAGELEN];
FTNINT i; // Dummyparameter
FTNSTRDESC  ftnstrg;

    snprintf( cBuf, TCS_MESSAGELEN, szTCSErrorMsg[iErr], msg );
    if ((iErr == WRN_JOUUNKWN) || // Rekursion von TCSWndProc_OnPaint vermeiden
        (iErr == ERR_XMLOPEN)          ) { // System noch nicht initialisiert
     MessageBox (NULL, _T(cBuf), szTCSWindowName, MB_ICONINFORMATION);
    } else { // ab jetzt mit bell, outtext...
     InvalidateRect (hTCSWindow, NULL, true); /* ,ClientArea, EraseFlag */
     UpdateWindow (hTCSWindow); /* Notwendig bei OnPaint mit Journaltyp=3 */
     bell (); // -> MessgageBeep / winuser.h, ohne Initialisierung verwendbar
     ftnstrg.addr= cBuf; ftnstrg.len= strlen (cBuf);
     TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
     if (TCSErrorLev[iErr] >1) {
      if (TCSErrorLev[iErr] < 10) {
       if (TCSErrorLev[iErr] == 5) {
        tinput (&i); // Press Any Key
       }
       if (TCSErrorLev[iErr]==8) {
        MessageBox (NULL, _T(cBuf), szTCSWindowName, MB_ICONINFORMATION);
       }
      } else {
       if (TCSErrorLev[iErr] == 10) {
        tinput (&i); // Press Any Key
       }
       if (TCSErrorLev[iErr]==12) {
         MessageBox (NULL, _T(cBuf), szTCSWindowName, MB_ICONSTOP);
       }
       if (iErr != ERR_EXIT) { // Error-Level von finitt durch XML veraenderbar
         TCSErrorLev[ERR_EXIT] = 10; // Hier: Fehler mit Programmabbruch
         finitt ();                  // Erzwungenes Beenden durch finitt
       }
      }
     }
    }
}



// ------------- Unterprogramme fuer die Event Handler  ----------------




// ------------- Unterprogramme für die Userroutinen -------------------


#if defined(REGSUPPORT)
 void StoreIni (TCHAR * szSection, TCHAR * szField, TCHAR * szValue)
 {

    if (_tcsicmp (szSection,TCS_INISECT1) == 0 ) { // Section1: Names ---------
     if (_tcsicmp (szField,TCS_INIVAR_WINNAM) == 0 ) {
      if (_tcslen(szTCSWindowName)==0) _tcsncpy(szTCSWindowName,
                                                szValue,TCS_WINDOW_NAMELEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_STATNAM) == 0 ) {
      if (_tcslen(szTCSstatWindowName)==0) _tcsncpy(szTCSstatWindowName,
                                                szValue,TCS_WINDOW_NAMELEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_MAINWINNAM) == 0 ) {
      _tcsncpy(szTCSMainWindowName, szValue,TCS_WINDOW_NAMELEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_HDCNAM) == 0 ) {
      _tcsncpy(szTCSHardcopyFile, szValue,TCS_FILE_NAMELEN-1);
     }

    } else if (_tcsicmp (szSection,TCS_INISECT2) == 0 ) { // Section2: Layout -
     if (_tcsicmp (szField,TCS_INIVAR_COPMEN) == 0 ) {
      _tcsncpy(szTCSMenuCopyText, szValue,TCS_MENUENTRY_LEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_FONT) == 0 ) {
      _tcsncpy(szTCSGraphicFont, szValue,TCS_FILE_NAMELEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_SYSFONT) == 0 ) {
      _tcsncpy(szTCSSysFont, szValue,TCS_FILE_NAMELEN-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_ICONNAM) == 0 ) {
      _tcsncpy(szTCSIconFile, szValue,TCS_FILE_NAMELEN-1);

     } else if (_tcsicmp (szField,TCS_INIVAR_WINPOSX) == 0 ) {
      TCSwindowIniXrelpos= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_WINPOSY) == 0 ) {
      TCSwindowIniYrelpos= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_WINSIZX) == 0 ) {
      TCSwindowIniXrelsiz= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_WINSIZY) == 0 ) {
      TCSwindowIniYrelsiz= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_STATPOSX) == 0 ) {
      TCSstatWindowIniXrelpos= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_STATPOSY) == 0 ) {
      TCSstatWindowIniYrelpos= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_STATSIZX) == 0 ) {
      TCSstatWindowIniXrelsiz= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_STATSIZY) == 0 ) {
      TCSstatWindowIniYrelsiz= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_LINCOL) == 0 ) {
      TCSDefaultLinCol= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_TXTCOL) == 0 ) {
      TCSDefaultTxtCol= * (int*) szValue;
     } else if (_tcsicmp (szField,TCS_INIVAR_BCKCOL) == 0 ) {
      TCSDefaultBckCol= * (int*) szValue;
     }

    } else if (_tcsicmp (szSection,TCS_INISECT3) == 0 ) { // Section3: Messages
     if (_tcsicmp (szField,TCS_INIVAR_HDCOPN) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_HDCFILOPN], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_HDCOPNL) == 0 ) {
      TCSErrorLev[WRN_HDCFILOPN]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_HDCWRT) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_HDCFILWRT], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_HDCWRTL) == 0 ) {
      TCSErrorLev[WRN_HDCFILWRT]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_HDCINT) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_HDCINTERN], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_HDCINTL) == 0 ) {
      TCSErrorLev[WRN_HDCINTERN]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_USR) == 0 ) {
      _tcsncpy(szTCSErrorMsg[MSG_USR], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_USRL) == 0 ) {
      TCSErrorLev[MSG_USR]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_HDCACT) == 0 ) {
      _tcsncpy(szTCSErrorMsg[MSG_HDCACT], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_HDCACTL) == 0 ) {
      TCSErrorLev[MSG_HDCACT]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_USRWRN) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_USRPRESSANY], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_USRWRNL) == 0 ) {
      TCSErrorLev[WRN_USRPRESSANY]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_EXIT) == 0 ) {
      _tcsncpy(szTCSErrorMsg[ERR_EXIT], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_EXITL) == 0 ) {
      TCSErrorLev[ERR_EXIT]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_COPMEM) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_COPYNOMEM], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_COPMEML) == 0 ) {
      TCSErrorLev[WRN_COPYNOMEM]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_COPLCK) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_COPYLOCK], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_COPLCKL) == 0 ) {
      TCSErrorLev[WRN_COPYLOCK]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_JOUCREATE) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_JOUCREATE], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_JOUCREATEL) == 0 ) {
      TCSErrorLev[WRN_JOUCREATE]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_JOUENTRY) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_JOUENTRY], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_JOUENTRYL) == 0 ) {
      TCSErrorLev[WRN_JOUENTRY]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_JOUADD) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_JOUADD], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_JOUADDL) == 0 ) {
      TCSErrorLev[WRN_JOUADD]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_JOUCLR) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_JOUCLR], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_JOUCLRL) == 0 ) {
      TCSErrorLev[WRN_JOUCLR]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_JOUUNKWN) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_JOUUNKWN], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_JOUUNKWNL) == 0 ) {
      TCSErrorLev[WRN_JOUUNKWN]= * (int*) szValue;
 
     } else if (_tcsicmp (szField,TCS_INIVAR_XMLPARSER) == 0 ) {
      _tcsncpy(szTCSErrorMsg[ERR_XMLPARSER], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_XMLPARSERL) == 0 ) {
      TCSErrorLev[ERR_XMLPARSER]= * (int*) szValue;

     } else if (_tcsicmp (szField,ERR_XMLOPEN) == 0 ) {
      _tcsncpy(szTCSErrorMsg[ERR_XMLOPEN], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_XMLOPENL) == 0 ) {
      TCSErrorLev[ERR_XMLOPEN]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_USR2) == 0 ) {
      _tcsncpy(szTCSErrorMsg[MSG_USR2], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_USR2L) == 0 ) {
      TCSErrorLev[MSG_USR2]= * (int*) szValue;

     } else if (_tcsicmp (szField,TCS_INIVAR_INI2) == 0 ) {
      _tcsncpy(szTCSErrorMsg[WRN_INI2], szValue,STAT_MAXCOLUMNS-1);
     } else if (_tcsicmp (szField,TCS_INIVAR_INI2L) == 0 ) {
      TCSErrorLev[WRN_INI2]= * (int*) szValue;

     }

    } // End case section

 }
#endif


bool PointInWindow (FTNINT ix1, FTNINT iy1)
{
    if (ClippingNotActive ) return true;
    return ( (TKTRNX.kminsx <= ix1) && (TKTRNX.kmaxsx >= ix1) &&
                    (TKTRNX.kminsy <= iy1) && (TKTRNX.kmaxsy >= iy1));
}



bool ClipLineStart (FTNINT ix1, FTNINT iy1, FTNINT ix2, FTNINT iy2,
                                                 FTNINT *isx, FTNINT *isy)
/* ClipLineStart=true: isx,isy Startpunkt; =false: Linie nicht zeichnen */
{
    if (ClippingNotActive) {
     *isx= ix1; *isy= iy1;
     return true;
    }

    if (ix1 < TKTRNX.kminsx) { /* Start links vom Fenster */
     if (ix2 < TKTRNX.kminsx) return false;
     *isy= iy1+((TKTRNX.kminsx-ix1) * (iy2-iy1)) / (ix2-ix1);
     if ((TKTRNX.kminsy <= *isy) && (TKTRNX.kmaxsy >= *isy)) {
      *isx= TKTRNX.kminsx;
      return true;
     }
     if (iy1 == iy2) return false;
     if (((ix2-ix1)*(iy2-iy1)) >= 0) { /* Steigung positiv */
      *isx= ix1+ ((TKTRNX.kminsy-iy1)*(ix2-ix1))/(iy2-iy1);
      *isy= TKTRNX.kminsy;
     } else {
      *isx= ix1+ ((TKTRNX.kmaxsy-iy1)*(ix2-ix1))/(iy2-iy1);
      *isy= TKTRNX.kmaxsy;
     }
     if ((*isx > TKTRNX.kmaxsx) || (*isx < TKTRNX.kminsx)) return false;
     return true;

    } else if (ix1 > TKTRNX.kmaxsx) { /* Start rechts vom Fenster */
     if (ix2 > TKTRNX.kmaxsx) return false;
     *isy= iy1+((TKTRNX.kmaxsx-ix1) * (iy2-iy1)) / (ix2-ix1);
     if ((TKTRNX.kminsy <= *isy) && (TKTRNX.kmaxsy >= *isy)) {
      *isx= TKTRNX.kmaxsx;
      return true;
     }
     if (iy1 == iy2) return false;
     if (((ix2-ix1)*(iy2-iy1)) >= 0) { /* Steigung positiv */
      *isx= ix1+ ((TKTRNX.kmaxsy-iy1)*(ix2-ix1))/(iy2-iy1);
      *isy= TKTRNX.kmaxsy;
     } else {
      *isx= ix1+ ((TKTRNX.kminsy-iy1)*(ix2-ix1))/(iy2-iy1);
      *isy= TKTRNX.kminsy;
     }
     if ((*isx > TKTRNX.kmaxsx) || (*isx < TKTRNX.kminsx)) return false;
     return true;

    } else if (iy1 < TKTRNX.kminsy) { /* Start unter dem Fenster */
     if (iy2 < TKTRNX.kminsy) return false;
     *isx= ix1+ ((TKTRNX.kminsy-iy1)*(ix2-ix1))/(iy2-iy1);
     if ((*isx > TKTRNX.kmaxsx) || (*isx < TKTRNX.kminsx)) return false;
     *isy= TKTRNX.kminsy;
     return true;

    } else if (iy1 > TKTRNX.kmaxsy) { /* Start ueber dem Fenster */
     if (iy2 > TKTRNX.kmaxsy) return false;
     *isx= ix1+ ((TKTRNX.kmaxsy-iy1)*(ix2-ix1))/(iy2-iy1);
     if ((*isx > TKTRNX.kmaxsx) || (*isx < TKTRNX.kminsx)) return false;
     *isy= TKTRNX.kmaxsy;
     return true;

    }
    *isx= ix1;                        /* Startpunkt liegt im Fenster */
    *isy= iy1;
    return true;
}



/*
------------------- Event Handler zum Parsen von XML-Dateien ----------
*/

#if defined(XMLSUPPORT)

void sax_callback (mxml_node_t *node, mxml_sax_event_t event, void *usr)
{
char * StorePtr;

    switch (event) {
     case MXML_SAX_ELEMENT_OPEN: {
      switch (*(int*)usr ) {
       case -1: { // Statemachine: noch keine aktive Sektion
        if (strcmp(mxmlGetElement(node),szTCSsect0) == 0) {
         *(int*)usr= 0;  // Parsing active
         mxmlElementSetAttr (node,"typ","none");
        }
        break;
       }
       case 0: {
        if ((strcmp(mxmlGetElement(node),TCS_INISECT1) == 0)  ) {
         *(int*)usr= 1; // State: TCS_INISECT1
        } else if ((strcmp(mxmlGetElement(node),TCS_INISECT2) == 0)  ) {
         *(int*)usr= 2; // State: TCS_INISECT2
        } else if ((strcmp(mxmlGetElement(node),TCS_INISECT3) == 0)  ) {
         *(int*)usr= 3; // State: TCS_INISECT3
        }
        mxmlElementSetAttr (node,"typ","none");
        break;
       }

       case 1: { // Section = Names
        if ((strcmp(mxmlGetElement(node),TCS_INIVAR_WINNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSWindowName);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_STATNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSstatWindowName);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_MAINWINNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSMainWindowName);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSHardcopyFile);
        }
        break;
       }

       case 2: { // Section = Layout
        if ((strcmp(mxmlGetElement(node),TCS_INIVAR_COPMEN) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSMenuCopyText);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_FONT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSGraphicFont);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_SYSFONT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSSysFont);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_ICONNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSIconFile);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_WINPOSX) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSwindowIniXrelpos);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_WINPOSY) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSwindowIniYrelpos);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_WINSIZX) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSwindowIniXrelsiz);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_WINSIZY) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSwindowIniYrelsiz);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_STATPOSX) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSstatWindowIniXrelpos);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_STATPOSY) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSstatWindowIniYrelpos);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_STATSIZX) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSstatWindowIniXrelsiz);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_STATSIZY) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSstatWindowIniYrelsiz);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_LINCOL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSDefaultLinCol);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_TXTCOL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSDefaultTxtCol);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_BCKCOL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSDefaultBckCol);
        }
        break;
       }

       case 3: { // Section = Messages
        if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCOPN) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_HDCFILOPN]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCOPNL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_HDCFILOPN]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCWRT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_HDCFILWRT]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCWRTL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_HDCFILWRT]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCINT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_HDCINTERN]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCINTL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_HDCINTERN]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USR) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[MSG_USR]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USRL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[MSG_USR]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCACT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[MSG_HDCACT]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCACTL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[MSG_HDCACT]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USRWRN) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_USRPRESSANY]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USRWRNL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_USRPRESSANY]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_EXIT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_EXIT]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_EXITL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_EXIT]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_COPMEM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_COPYNOMEM]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_COPMEML) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_COPYNOMEM]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_COPLCK) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_COPYLOCK]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_COPLCKL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_COPYLOCK]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUCREATE) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_JOUCREATE]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUCREATEL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_JOUCREATE]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUENTRY) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_JOUENTRY]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUENTRYL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_JOUENTRY]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUADD) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_JOUADD]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUADDL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_JOUADD]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUCLR) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_JOUCLR]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUCLRL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_JOUCLR]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUUNKWN) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_JOUUNKWN]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_JOUUNKWNL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_JOUUNKWN]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_XMLPARSER) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_XMLPARSER]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_XMLPARSERL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_XMLPARSER]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_XMLOPEN) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_XMLOPEN]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_XMLOPENL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_XMLOPEN]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USR2) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[MSG_USR2]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_USR2L) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[MSG_USR2]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_INI2) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[WRN_INI2]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_INI2L) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[WRN_INI2]);

        }
        break;
       }

      }
      break;
     }

     case MXML_SAX_DATA: {
      switch (mxmlGetType(node)) {
       case MXML_INTEGER: {
        sscanf (mxmlElementGetAttr(mxmlGetParent(node), "store"),"%p",&StorePtr);
        (*(int*)StorePtr)= mxmlGetInteger(node);
        break;
       }
       case MXML_REAL: {
        sscanf (mxmlElementGetAttr(mxmlGetParent(node), "store"),"%p",&StorePtr);
        (*(float*)StorePtr)= mxmlGetReal(node);
        break;
       }
       case MXML_TEXT: {
        sscanf (mxmlElementGetAttr(mxmlGetParent(node), "store"),"%p",&StorePtr);
        strcpy (StorePtr, mxmlGetText(node, NULL));
        break;
       }
       case MXML_OPAQUE: {
        sscanf (mxmlElementGetAttr(mxmlGetParent(node), "store"),"%p",&StorePtr);
        strcpy (StorePtr, mxmlGetOpaque(node));
        break;
       }
      }
      break;
     }

     case MXML_SAX_ELEMENT_CLOSE: {
      if ((*(int*)usr==0) && (strcmp(mxmlGetElement(node),szTCSsect0)==0)) {
       *(int*)usr= -1; // State: idle
      } else if (
             ((*(int*)usr==1) && (strcmp(mxmlGetElement(node),TCS_INISECT1)==0))
          || ((*(int*)usr==2) && (strcmp(mxmlGetElement(node),TCS_INISECT2)==0))
          || ((*(int*)usr==3) && (strcmp(mxmlGetElement(node),TCS_INISECT3)==0))
          ) {
       *(int*)usr= 0; // State: Parsing active
      }
      break;
     }
    }
}


/* ------------------------------------------------------------------------- */


mxml_type_t      sax_type_callback(mxml_node_t  *node)
{
const char *type;

    if ((type = mxmlElementGetAttr(node, "typ")) == NULL) type = "none";
    if (!strcmp(type, "integer"))
     return (MXML_INTEGER);
    else if (!strcmp(type, "opaque") || !strcmp(type, "pre"))
     return (MXML_OPAQUE);
    else if (!strcmp(type, "real"))
     return (MXML_REAL);
    else if (!strcmp(type, "text"))
     return (MXML_TEXT);
    else
     return (MXML_IGNORE);
}

/* ------------------------------------------------------------------------- */


mxml_error_cb_t sax_error_callback (char *mssg)
{
    TCSGraphicError (ERR_XMLPARSER, mssg);
    return;
}

/* ------------------------------------------------------------------------- */

#endif   // Ende XML-Unterstützung




/*
------------------- Event Handler Graphikfenster -----------------------
*/




void TCSWndProc_OnPaint (HWND hWindow)
{
PAINTSTRUCT ps;
#if (JOURNALTYP == 1)
 HMETAFILE hmf;
 HDC hTCSMetaFileDC1;
#elif (JOURNALTYP == 2)
 HENHMETAFILE hmf;
 ENHMETAHEADER emh ;
 HDC hTCSMetaFileDC1;
 RECT   crtrect;
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
 HPEN   hPenDash, hPenOld;
 HFONT  hOldFont;
 int    iMaskIndex;
 int    iGraphTextLen, iGraphTextLenAkt;
 TCHAR  GraphTextBuf[STAT_MAXCOLUMNS+1];
#endif


    BeginPaint (hWindow, &ps);

#if (JOURNALTYP == 1)
    hmf = CloseMetaFile (hTCSMetaFileDC);
    PlayMetaFile (hTCSWindowDC, hmf);           /* Wiederherstellung Anzeige */

    hTCSMetaFileDC1  = CreateMetaFile (NULL);   /* 16bit Windows Metafile */
    PlayMetaFile (hTCSMetaFileDC1, hmf);        /* für neues Journalfile */
    DeleteMetaFile (hmf);                       /* alter Status Bildschirm */
    hTCSMetaFileDC = hTCSMetaFileDC1;           /* bereit zum Weiterzeichnen */

#elif (JOURNALTYP == 2)
    hmf = CloseEnhMetaFile (hTCSMetaFileDC);
    GetEnhMetaFileHeader (hmf, sizeof (emh), &emh) ;
    GetClientRect(hTCSWindow, &crtrect); // Zeichenbereich CRT in Pixeln

    SetViewportExtEx (hTCSWindowDC, crtrect.right-crtrect.left,
                        crtrect.bottom-crtrect.top, NULL); // Zeichne EMF 1:1
    SetViewportOrgEx (hTCSWindowDC, crtrect.left, crtrect.bottom, NULL);
    SetWindowExtEx (hTCSWindowDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSWindowDC, TCSrect.left, TCSrect.bottom, NULL);

    PlayEnhMetaFile (hTCSWindowDC, hmf, &TCSrect); // Wiederherstellung Anzeige

    SetViewportExtEx (hTCSWindowDC, crtrect.right-crtrect.left,
                         crtrect.top-crtrect.bottom, NULL); // Skaliere auf TEK
    SetViewportOrgEx (hTCSWindowDC, crtrect.left, crtrect.top, NULL);
    SetWindowExtEx (hTCSWindowDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSWindowDC, TCSrect.left, TCSrect.bottom, NULL);


    hTCSMetaFileDC1  = CreateEnhMetaFile (hTCSWindowDC, NULL, &emh.rclFrame,
                        _T("TCS for Windows\0Journalfile created by OnPaint\0"));

    SetMapMode (hTCSMetaFileDC1, MM_ANISOTROPIC);
    SetViewportExtEx (hTCSMetaFileDC1, TCSrect.right, TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC1, TCSrect.left, TCSrect.bottom, NULL);
    SetWindowExtEx (hTCSMetaFileDC1, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC1, TCSrect.left, TCSrect.bottom, NULL);

    PlayEnhMetaFile (hTCSMetaFileDC1, hmf, &TCSrect); // neues Journal

    DeleteEnhMetaFile (hmf);                    // Bildschirminhalt restauriert
    hTCSMetaFileDC = hTCSMetaFileDC1;           // bereit zum Weiterzeichnen
    SetViewportExtEx (hTCSMetaFileDC, TCSrect.right, -TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.top, NULL);
    SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #endif
    SetBkMode (hTCSMetaFileDC, TRANSPARENT );   // Metafile weitergegeben !
    SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP); // CP
    SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

#elif (JOURNALTYP == 3)
//      if (hTCSJournal != NULL) {
    SGLIB_DL_LIST_GET_LAST(struct xJournalEntry_typ, hTCSJournal, previous, next, xJournalEntry)
    while (xJournalEntry != NULL) {
     switch (xJournalEntry->action) {
      case XACTION_INITT: {
       TKTRNX.iLinCol= TCSDefaultLinCol;
       TKTRNX.iTxtCol= TCSDefaultTxtCol;
       TKTRNX.iBckCol= TCSDefaultBckCol;
       initt2(); // HOME, Font, Scale...
      } // weiter mit Erase
      case XACTION_ERASE: {
       SetWindowExtEx (hTCSWindowDC, TCSrect.right, TCSrect.bottom, NULL);
       SetWindowOrgEx (hTCSWindowDC, TCSrect.left, TCSrect.bottom, NULL);
       SetBkMode (hTCSWindowDC, TRANSPARENT );
       SetTextAlign (hTCSWindowDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP);
       SetTextColor (hTCSWindowDC, dwColorTable[TKTRNX.iTxtCol]);
       #if !defined(__WIN32__) && !defined(_WIN32)
        SelectPen (hTCSWindowDC, hTCSPen); // 16bit: Makro aus windowsx.h
       #else
        SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
       #endif
       break;
      }
      case XACTION_MOVABS: {
       MoveToEx (hTCSWindowDC, HiRes(xJournalEntry->i1),
                                             HiRes(xJournalEntry->i2), NULL);
       TKTRNX.kBeamX= xJournalEntry->i1;
       TKTRNX.kBeamY= xJournalEntry->i2;
       break;
      }
      case XACTION_DRWABS: {
       LineTo (hTCSWindowDC, HiRes(xJournalEntry->i1),
                   HiRes(xJournalEntry->i2) ); // Endpunkt nicht mitgezeichnet!
       SetPixel (hTCSWindowDC,HiRes(xJournalEntry->i1),
                       HiRes(xJournalEntry->i2), dwColorTable[TKTRNX.iLinCol]);
       TKTRNX.kBeamX= xJournalEntry->i1;
       TKTRNX.kBeamY= xJournalEntry->i2;
       break;
      }
      case XACTION_DSHSTYLE: {
       iMaskIndex= xJournalEntry->i1;
       break;
      }
      case XACTION_DSHABS: {
       hPenDash= CreatePen (dwPenStyle[iMaskIndex], 0,
                                               dwColorTable[TKTRNX.iLinCol]);
       #if !defined(__WIN32__) && !defined(_WIN32)
        SelectPen (hTCSWindowDC, hPenDash); // 16bit: Makro aus windowsx.h
       #else
        SelectObject (hTCSWindowDC, hPenDash); // 32bit: GDI Standardaufruf
       #endif
       LineTo (hTCSWindowDC, HiRes(xJournalEntry->i1),
                                               HiRes(xJournalEntry->i2) );
       #if !defined(__WIN32__) && !defined(_WIN32)
        SelectPen (hTCSWindowDC, hTCSPen); // 16bit: Makro aus windowsx.h
        DeletePen (hPenDash);
       #else
        SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
        DeleteObject (hPenDash);
       #endif
       TKTRNX.kBeamX= xJournalEntry->i1;
       TKTRNX.kBeamY= xJournalEntry->i2;
       break;
      }
      case XACTION_PNTABS: {
       SetPixel (hTCSWindowDC,HiRes(xJournalEntry->i1),
                   HiRes(xJournalEntry->i2), dwColorTable[TKTRNX.iLinCol]);
       TKTRNX.kBeamX= xJournalEntry->i1;
       TKTRNX.kBeamY= xJournalEntry->i2;
       break;
      }
      case XACTION_BCKCOL: {
       TKTRNX.iBckCol= xJournalEntry->i1;
       break;
      }
      case XACTION_LINCOL: {
       hTCSPen= CreatePen (PS_SOLID, 0, dwColorTable[xJournalEntry->i1]);
       #if !defined(__WIN32__) && !defined(_WIN32)
        hPenOld= SelectPen (hTCSWindowDC, hTCSPen);// 16bit: Makro aus windowsx.h
        DeletePen (hPenOld);
       #else
        hPenOld= SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
        DeleteObject (hPenOld);
       #endif
       TKTRNX.iLinCol= xJournalEntry->i1;
       break;
      }
      case XACTION_TXTCOL: {
       SetTextColor (hTCSWindowDC, dwColorTable[xJournalEntry->i1]);
       TKTRNX.iTxtCol= xJournalEntry->i1;
       break;
      }
      case XACTION_FONTATTR: {
       TKTRNX.kitalc= xJournalEntry->i1;
       TCSFontdefinition.lfItalic= (TKTRNX.kitalc > 0);
       hTCSFont= CreateFontIndirect (&TCSFontdefinition);
       #if !defined(__WIN32__) && !defined(_WIN32)
        hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
        DeleteFont (hOldFont);
       #else
        hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
        DeleteObject (hOldFont);
       #endif

       if (TKTRNX.ksizef != xJournalEntry->i2) {
        TKTRNX.ksizef= xJournalEntry->i2;
        TCSFontdefinition.lfHeight= (1+TKTRNX.ksizef)*TCSCharHeight;
        TCSFontdefinition.lfWidth= 0;
        hTCSFont= CreateFontIndirect (&TCSFontdefinition);
        #if !defined(__WIN32__) && !defined(_WIN32)
         hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
         DeleteFont (hOldFont);
        #else
         hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
         DeleteObject (hOldFont);
        #endif
        TKTRNX.khomey = TEK_YMAX - 1.5f*(1+TKTRNX.ksizef)*TCS_REL_CHR_HEIGHT;
       }
       break;
      }
      case XACTION_GTEXT: {
       iGraphTextLenAkt= 0;
       iGraphTextLen= (int) xJournalEntry->i1;
       if (iGraphTextLen > STAT_MAXCOLUMNS) iGraphTextLen= STAT_MAXCOLUMNS;
       if (iGraphTextLen == 0) break;
       GraphTextBuf[iGraphTextLenAkt++]= (TCHAR) xJournalEntry->i2;
       if (iGraphTextLen == 1) {
        GraphTextBuf[iGraphTextLenAkt]= (FTNCHAR) 0;
        TextOut (hTCSWindowDC, 0,0,GraphTextBuf, iGraphTextLen);
       }
       break;
      }
      case XACTION_ASCII: {
       if (iGraphTextLenAkt < iGraphTextLen) {
        GraphTextBuf[iGraphTextLenAkt++]= (TCHAR) xJournalEntry->i1;
        if (iGraphTextLenAkt < iGraphTextLen)
         GraphTextBuf[iGraphTextLenAkt++]= (TCHAR) xJournalEntry->i2;
        if (iGraphTextLenAkt >= iGraphTextLen)
         TextOut (hTCSWindowDC, 0,0,GraphTextBuf, iGraphTextLen);
       }
       break;
      }
      case XACTION_NOOP: {
       break;
      }
      default: {
       TCSGraphicError (WRN_JOUUNKWN,"");
       break;
      }
     }
     xJournalEntry= xJournalEntry -> previous;
    }
//      }
#endif

    EndPaint( hWindow, &ps );
}



void TCSWndProc_OnSize (HWND hWindow, UINT message, WPARAM width, LPARAM height)
{
    switch (message) {
     case SIZE_MINIMIZED:   /* Minimierung -> keine Aktion notwendig */
      break;
     case SIZE_RESTORED:    /*(Erst- oder Neu)Skalierung des Fensters */
     case SIZE_MAXIMIZED:       /* sichtbar: 0<=ix<=1023 / 0<=iy<=780 */
      SetMapMode (hTCSWindowDC, MM_ANISOTROPIC);
      SetViewportExtEx (hTCSWindowDC, width, -height, NULL);
      SetViewportOrgEx (hTCSWindowDC, 0, 0, NULL);
      /* Bei erneuter Änderung des Viewport geht die Auflösung verloren! */
    }
}



void TCSWndProc_OnRbuttondown (HWND hWindow, BOOL DoubleClick, int MouseX,
                                        int MouseY, UINT ShftCtrlKeyMask)
{
    ShowWindow (hTCSstatWindow, SW_SHOW);
    UpdateWindow(hTCSstatWindow);
}



bool TCSWndProc_OnErasebkgnd (HWND hWindow, HDC hDC)
{
RECT ClientArea;
HBRUSH hBack;

    GetClientRect (hWindow, &ClientArea);
    DPtoLP (hDC, (LPPOINT)&ClientArea.left,2);

    hBack= CreateSolidBrush (dwColorTable[TCSBackgroundColour]);
    FillRect(hTCSWindowDC, &ClientArea, hBack);
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteBrush (hBack);
    #else
     DeleteObject (hBack);
    #endif

    return false;
}



bool TCSWndProc_OnCopyClipboard ()
{
#if (JOURNALTYP == 1)
 FTNINT iErr;
 HMETAFILE hmf;
 HDC hTCSNewMetaFileDC;
 HGLOBAL hGlobalMem;
 LPMETAFILEPICT lpMfp;
 HRGN    hWindowRegion;
#elif (JOURNALTYP == 2)
 FTNINT iErr;
 HENHMETAFILE hmf, hmf1;
 ENHMETAHEADER emh ;
 HDC hTCSMetaFileDC1;
#endif


#if (JOURNALTYP == 1)
    hmf = CloseMetaFile (hTCSMetaFileDC);       /* Metafile für WM_PAINT */

    hGlobalMem= GlobalAlloc(GMEM_MOVEABLE | GMEM_SHARE, sizeof(METAFILEPICT));
    if (hGlobalMem == NULL) {
     iErr= WRN_COPYNOMEM;
     #ifndef __cplusplus
      TCSGraphicError (iErr,"");
     #endif
     return false;                      /* Error: OutOfMemory -> ret */
    }
    lpMfp= (LPMETAFILEPICT) GlobalLock (hGlobalMem);

    lpMfp->mm= MM_ANISOTROPIC;
    lpMfp->xExt= 0;             /* Keine Defaultgröße vorgeben */
    lpMfp->yExt= 0;             /* sonst in MM_HIMETRIC Device-Einheiten! */

    hTCSNewMetaFileDC  = CreateMetaFile (NULL);

    ScaleViewportExtEx (hTCSNewMetaFileDC, 1,1,-1,1,NULL);   // für Clipboard

    hWindowRegion= CreateRectRgn(TCSrect.left, TCSrect.top, TCSrect.right,TCSrect.bottom); // rechts,oben
    SelectClipRgn (hTCSNewMetaFileDC, hWindowRegion); // nicht eingeschlossen
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteRgn (hWindowRegion); // Resource freigeben
    #else
     DeleteObject (hWindowRegion);
    #endif

    PlayMetaFile (hTCSNewMetaFileDC, hmf);

    lpMfp->hMF= CloseMetaFile (hTCSNewMetaFileDC);

    GlobalUnlock(hGlobalMem);

    hTCSNewMetaFileDC  = CreateMetaFile (NULL); /* 16bit Windows Metafile */
    PlayMetaFile (hTCSNewMetaFileDC, hmf);      /* für neues Journalfile */
    DeleteMetaFile (hmf);                       /* alter Status Bildschirm */
    hTCSMetaFileDC = hTCSNewMetaFileDC;         /* bereit Weiterzeichnen */

    if (!OpenClipboard (hTCSWindow)) {          /* Error: Clipboard locked */
     GlobalFree (hGlobalMem);
     iErr= WRN_COPYLOCK;
     #ifndef __cplusplus
      TCSGraphicError (iErr,"");
     #endif
     return false;
    }
    EmptyClipboard ();
    SetClipboardData (CF_METAFILEPICT, hGlobalMem);
    CloseClipboard ();  /* Jetzt GlobalFree() NICHT mehr aufrufen */

#elif (JOURNALTYP == 2)
    hmf = CloseEnhMetaFile (hTCSMetaFileDC);    /* Metafile für WM_PAINT */
    hmf1 = CopyEnhMetaFile (hmf, NULL) ;
    if (!OpenClipboard (hTCSWindow)) {          /* Error: Clipboard locked */
     iErr= WRN_COPYLOCK;
     #ifndef __cplusplus
      TCSGraphicError (iErr,"");
     #endif
     return false;
    }
    EmptyClipboard () ;
    SetClipboardData (CF_ENHMETAFILE, hmf1) ;
    CloseClipboard () ;

    GetEnhMetaFileHeader (hmf, sizeof (emh), &emh) ;
    hTCSMetaFileDC1  = CreateEnhMetaFile (hTCSWindowDC, NULL, &emh.rclFrame,
                 _T("TCS for Windows\0Journalfile created by CopyClipboard\0"));
    SetMapMode (hTCSMetaFileDC1, MM_ANISOTROPIC);
    SetViewportExtEx (hTCSMetaFileDC1, TCSrect.right, TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC1, TCSrect.left, TCSrect.bottom, NULL);
    SetWindowExtEx (hTCSMetaFileDC1, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC1, TCSrect.left, TCSrect.bottom, NULL);

    SetBkMode (hTCSMetaFileDC, TRANSPARENT );
    SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP);

    PlayEnhMetaFile (hTCSMetaFileDC1, hmf, &TCSrect); // neues Journal

    DeleteEnhMetaFile (hmf);                      // alter Status Bildschirm
    hTCSMetaFileDC = hTCSMetaFileDC1;           // bereit zum Weiterzeichnen

    SetViewportExtEx (hTCSMetaFileDC, TCSrect.right, -TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.top, NULL);
    SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #endif
    SetBkMode (hTCSMetaFileDC, TRANSPARENT );   // Metafile weitergegeben !
    SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP); // CP
    SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

#endif

    return true;
}



LRESULT CALLBACK EXPORT16 TCSWndProc(HWND hWindow, UINT Message,
                            WPARAM wParam, LPARAM lParam)
{
    switch( Message ) {
     HANDLE_MSG(hWindow, WM_PAINT, TCSWndProc_OnPaint);
     HANDLE_MSG(hWindow, WM_RBUTTONDOWN, TCSWndProc_OnRbuttondown);
     HANDLE_MSG(hWindow, WM_SIZE, TCSWndProc_OnSize);
     HANDLE_MSG(hWindow, WM_ERASEBKGND, TCSWndProc_OnErasebkgnd);
     case WM_SYSCOMMAND:
      if (wParam == TCS_WM_COPY) {
       #ifdef trace_calls
        MessageBox(NULL, "WM_SYSCOMMAND (TCS_WM_COPY)",
                       "Internal Information GRAPH2D - TCSwindowProc",
                        MB_OK | MB_ICONINFORMATION);
       #endif
       TCSWndProc_OnCopyClipboard ();
       break;
      } else {
       return DefWindowProc( hWindow, Message, wParam, lParam );
      }
     case WM_CLOSE: // Schliessen des Graphikfensters nicht zulassen! Meldung
      break;        // kann trotz Menuesperre über <ALT><F4> erzeugt werden
     case WM_ACTIVATEAPP: // Neuzeichnen wg. Fensterminimierung fremde Appl.
      UpdateWindow (hWindow);
      return 0;
     default:
      return DefWindowProc( hWindow, Message, wParam, lParam );
    }
    return 0;
}



/*
------------------- Event Handler Statusfenster ------------------------
*/



void TCSstatWndProc_OnPaint (HWND hWindow)
{
int i;
PAINTSTRUCT ps;

    BeginPaint (hWindow, &ps);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (ps.hdc, hTCSSysFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (ps.hdc, hTCSSysFont);      // Aktuellen Zeichenstatus an
    #endif
    SetMapMode (ps.hdc, MM_TEXT);
    SetWindowOrgEx (ps.hdc, 0,TCSstatOrgY*TextLineHeight, NULL);
    for (i=0; i <= TCSstatRow; i++ )
     TextOut (ps.hdc, 0, i*TextLineHeight, TCSstatTextBuf[i],
                                          _tcslen (TCSstatTextBuf[i]));
    EndPaint( hWindow, &ps );
}



void TCSstatWndProc_OnKillfocus (HWND hWindow, HWND hNewWindow)
{
    if (TCSStatWindowAutomatic) ShowWindow (hWindow, SW_HIDE);
}



void TCSstatWndProc_OnGetminmaxinfo (HWND hWindow, MINMAXINFO FAR* lpMinMaxInfo)
/* Beschränkung User-erzeugbare Fenstergröße */
{
    lpMinMaxInfo -> ptMaxSize.x = GetSystemMetrics (SM_CXMAXIMIZED);
    lpMinMaxInfo -> ptMaxSize.y = (int) (TCS_REL_CHR_SPACE*TextLineHeight) +
                                STAT_MINLINES*GetSystemMetrics (SM_CYMINTRACK);
    lpMinMaxInfo -> ptMaxPosition.x = 0;
    #if !defined(__WIN32__) && !defined(_WIN32)
     lpMinMaxInfo -> ptMaxPosition.y = GetSystemMetrics (SM_CYFULLSCREEN) -
                                STAT_MINLINES*GetSystemMetrics (SM_CYMINTRACK);
    #else
     lpMinMaxInfo -> ptMaxPosition.y = GetSystemMetrics (SM_CYMAXIMIZED) -
                                (lpMinMaxInfo -> ptMaxSize.y);
    #endif
    lpMinMaxInfo -> ptMinTrackSize.x = GetSystemMetrics (SM_CXMINTRACK);
    lpMinMaxInfo -> ptMinTrackSize.y = GetSystemMetrics (SM_CYMINTRACK);
    lpMinMaxInfo -> ptMaxTrackSize.x = GetSystemMetrics (SM_CXMAXIMIZED);
    lpMinMaxInfo -> ptMaxTrackSize.y = STAT_ADDLINES*TextLineHeight+
                                (lpMinMaxInfo -> ptMaxSize.y);
}



void TCSstatWndProc_OnVScroll (HWND hWindow, HWND hNewWindow, WPARAM wParam,
                                                            LPARAM lParam)
{
    switch (wParam) {
     case SB_LINEUP:
      TCSstatScrollY --;
      if (TCSstatScrollY < 0) TCSstatScrollY=0;
      break;
     case SB_LINEDOWN:
      TCSstatScrollY ++;
      if (TCSstatScrollY >= STAT_MAXROWS) TCSstatScrollY=STAT_MAXROWS-1;
      break;
     case SB_PAGEUP:
      TCSstatScrollY -= STAT_PAGESIZ;
      if (TCSstatScrollY < 0) TCSstatScrollY=0;
      break;
     case SB_PAGEDOWN:
      TCSstatScrollY += STAT_PAGESIZ;
      if (TCSstatScrollY >= STAT_MAXROWS) TCSstatScrollY=STAT_MAXROWS-1;
      break;
     case SB_THUMBPOSITION:
      TCSstatScrollY= (int) lParam;
      if (TCSstatScrollY < 0) TCSstatScrollY=0;
      if (TCSstatScrollY >= STAT_MAXROWS) TCSstatScrollY=STAT_MAXROWS-1;
      InvalidateRect (hWindow, NULL, true); /* ,ClientArea, EraseFlag */
      UpdateWindow (hWindow);               /* zwingend notwendig für Win16 */
      break;
    }
    ScrollWindow (hWindow, 0, (TCSstatOrgY-TCSstatScrollY)*TextLineHeight,
                                                                NULL, NULL);
    SetScrollPos (hWindow, SB_VERT, TCSstatScrollY, true);
    TCSstatOrgY= TCSstatScrollY;
}



LRESULT CALLBACK EXPORT16 TCSstatWndProc(HWND hWindow, UINT Message,
                            WPARAM wParam, LPARAM lParam)
{
    switch( Message ) {
     HANDLE_MSG(hWindow, WM_PAINT, TCSstatWndProc_OnPaint);
     HANDLE_MSG(hWindow, WM_KILLFOCUS, TCSstatWndProc_OnKillfocus);
     HANDLE_MSG(hWindow, WM_GETMINMAXINFO, TCSstatWndProc_OnGetminmaxinfo);
     HANDLE_MSG(hWindow, WM_VSCROLL, TCSstatWndProc_OnVScroll);
     default:
      return DefWindowProc( hWindow, Message, wParam, lParam );
    }
    return 0;
}




/*
--------------- Konstruktion/Destruktion fuer C++ ----------------------
*/

#ifdef __cplusplus

TCSdrWIN__ TCSdrWIN()
{
        #ifdef trace_calls
         MessageBox(0, "Constructor", "TCSdrWIN", MB_OK | MB_ICONINFORMATION);
        #endif
        // initt; // Doppelaufruf Userroutine. Vorsicht WINLBL nach INITT!
}



TCSdrWIN__ ~TCSdrWIN()
{
        #if defined trace_calls
         MessageBox(0, "Destructor", "TCSdrWIN", MB_OK | MB_ICONINFORMATION);
        #endif
        // finitt; // Userroutine, Aufruf unbedingt notwendig!
}

#endif /* cplusplus */



/*
---------------------- Userroutinen: Initialisierung -------------------
*/



extern void TCSdrWIN__ tcslev3 (FTNINT *SysLev)

{
    *SysLev= TCSLEV3SYS;
}



#ifdef XMLSUPPORT

void XMLreadProgPar (const char * filname)
{
int ParserState;
FILE *fp;
mxml_node_t *tree;

    fp = fopen(filname, "r");
    if (fp == NULL) {
     TCSGraphicError (ERR_XMLOPEN, filname);
    } else {
      ParserState= -1; // State= idle
      mxmlSetErrorCallback ((mxml_error_cb_t)sax_error_callback);
      tree = mxmlSAXLoadFile(NULL, fp, sax_type_callback, sax_callback, &ParserState);
      fclose(fp);
    }
}

#endif    // Ende XML-Unterstützung



/*
Defaultwerte sind bereits durch Compiler initialisiert worden. Hier werden nur
die Parameter wiederhergestellt, die fuer einen erneuten Aufruf von initt nach
finitt sinnvoll sind.
*/

void PresetProgPar ()
{
    TCSDefaultLinCol= TCS_INIDEF_LINCOL;
    TCSDefaultTxtCol= TCS_INIDEF_TXTCOL;
    TCSDefaultBckCol= TCS_INIDEF_BCKCOL;

    TCSwindowIniXrelpos= TCS_INIDEF_WINPOSX;
    TCSwindowIniYrelpos= TCS_INIDEF_WINPOSY;
    TCSwindowIniXrelsiz= TCS_INIDEF_WINSIZX;
    TCSwindowIniYrelsiz= TCS_INIDEF_WINSIZY;

    TCSstatWindowIniXrelpos= TCS_INIDEF_STATPOSX;
    TCSstatWindowIniYrelpos= TCS_INIDEF_STATPOSY;
    TCSstatWindowIniXrelsiz= TCS_INIDEF_STATSIZX;
    TCSstatWindowIniYrelsiz= TCS_INIDEF_STATSIZY;

    // Fensternamen werden nur durch winlbl vorher veraendert

    // Hardcopyname und Zaehlerstand bleibt!

    // Fehlermeldungen werden bei der Variablendefinition durch den Compiler initialisiert
}



/*
Anpassung der Dateinamen an die Laufzeitumgebung
*/

void CustomizeProgPar ()
{
// Absicherung der Definition der Programmparameter
#if (TCS_WINDOW_NAMELEN <= TCS_FILE_NAMELEN)
 #define TMPSTRLEN TCS_FILE_NAMELEN
#else
 #define TMPSTRLEN TCS_WINDOW_NAMELEN
#endif

int         iL;
char        szTmpString[TMPSTRLEN];
FTNSTRDESC  ftn_WorkString, o, n;

szTmpString[0]= '\0';
n.addr= szTmpString; // Token bei Fonts werden geloescht
n.len= TMPSTRLEN;

#ifdef XMLSUPPORT // Angabe von Dateinamen fuer Fonts bei Windows nicht moeglich
    o.addr= PROGDIRTOKEN; // Token %: loeschen
    o.len= strlen (o.addr);
    ftn_WorkString.len= TCS_FILE_NAMELEN; // Font Graphikfenster
    ftn_WorkString.addr= szTCSGraphicFont;
    o.addr= PROGDIRTOKEN; // Substring %: loeschen
    o.len= strlen (o.addr);
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );

    ftn_WorkString.addr= szTCSSysFont; // Font Statusfenster
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );


    o.addr= INIFILEXTTOKEN; // Token .% loeschen
    o.len= strlen (o.addr); // Font Statusfenster
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );

    ftn_WorkString.addr= szTCSGraphicFont; // Font Graphikfenster
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
#endif // Ende XML-Unterstützung, in *.INI und Registry keine Verwendung Token

    if (strlen(szTCSWindowName) == 0) { // '/0' durch WINLBL -> Default
        strncpy(szTCSWindowName, TCS_WINDOW_NAME, TCS_WINDOW_NAMELEN);
    }
    if (strlen(szTCSstatWindowName) == 0) {
        strncpy(szTCSstatWindowName, TCS_STATWINDOW_NAME, TCS_WINDOW_NAMELEN);
    }

    o.addr= PROGDIRTOKEN; // Substring %: vollstaendiger Programmname
    o.len= strlen (o.addr);
    #if !defined(__WIN32__) && !defined(_WIN32)   /* nicht bei DLL möglich */
     #if defined __WATCOMC__
      iL= 0;            /* Argument 0= Voller Programmname mit Directory */
      iL= igetarg ((FTNINT *) &iL, &n);
     #else
      #error "Kompilation für 16bit Windows nur mit Watcom-Compiler möglich"
     #endif
    #else   /* alternativ nur Win32: hInst=NULL: prozesserzeugende Instanz */
     iL= GetModuleFileName(NULL, n.addr, n.len);
    #endif
    if (iL <= 0) {
     n.addr[0]= (FTNCHAR) 0; /* kein Programmnamen bekannt */
    }
    ftn_WorkString.len= TCS_WINDOW_NAMELEN; // Ersatz %: im Graphikfenster
    ftn_WorkString.addr= szTCSWindowName;
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
   ftn_WorkString.addr= szTCSstatWindowName; // Ersatz %: im Statusfenster
   SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );

// Absicherung TMPSTRLEN nicht mehr benoetigt
#undef TMPSTRLEN
}




extern void TCSdrWIN__ winlbl (FTNSTRPAR * PloWinNam, FTNSTRPAR * StatWinNam,
                                            FTNSTRPAR *IniFilNam
                                            FTNSTRPAR_TAIL(PloWinNam)
                                            FTNSTRPAR_TAIL(StatWinNam)
                                            FTNSTRPAR_TAIL(IniFilNam)      )

{

#if (TCS_WINDOW_NAMELEN <= TCS_FILE_NAMELEN)
 #define TMPSTRLREN TCS_FILE_NAMELEN
#else
 #define TMPSTRLREN TCS_WINDOW_NAMELEN
#endif

FTNCHARLEN  i, iL;
FTNCHAR     szTmpString[TMPSTRLREN], szTmpString1[TMPSTRLREN];
FTNCHAR *   iAt;
FTNSTRDESC  o, n, ftn_WorkString;


    iL= min(FTNSTRPARL(PloWinNam), TMPSTRLREN-1);    // Name des Grahikfensters
    _tcsncpy(szTmpString, FTNSTRPARA(PloWinNam),iL);
    szTmpString[iL]= (FTNCHAR) 0; // Fortranstring evtl. ohne \0
    iL= min (_tcslen (szTmpString), TCS_WINDOW_NAMELEN-1);
    if (iL > 0) {
     _tcsncpy( szTCSWindowName, szTmpString, iL);
     szTCSWindowName[iL]= (FTNCHAR) 0;
    }

    iL= min(FTNSTRPARL(StatWinNam), TMPSTRLREN-1);   // Name des Statusfensters
    _tcsncpy(szTmpString, FTNSTRPARA(StatWinNam), iL);
    szTmpString[iL]= (FTNCHAR) 0; // Fortranstring evtl. ohne \0
    iL= min (_tcslen (szTmpString), TCS_WINDOW_NAMELEN-1);
    if (iL > 0) {
     _tcsncpy( szTCSstatWindowName, szTmpString, iL);
     szTCSstatWindowName[iL]= (FTNCHAR) 0;
    }

    iL= min(FTNSTRPARL(IniFilNam), TMPSTRLREN-1); // Name Initialisierungsdatei
    _tcsncpy(szTmpString, FTNSTRPARA(IniFilNam), iL);
    szTmpString[iL]= (FTNCHAR) 0; // Fortranstring evtl. ohne \0

    iL= min (_tcslen (szTmpString), TCS_FILE_NAMELEN-1);

    if (iL > 0) {
     _tcsncpy( szTCSIniFile, szTmpString, iL);
     szTCSIniFile[iL]= (FTNCHAR) 0;

     iAt= _tcsstr (szTCSIniFile, _T("@")); // Section Level0?
     if (iAt != 0) {
      _tcsncpy(szTCSsect0, &iAt[1], iL); // Abspeichern
      iAt[0]= (FTNCHAR) 0; // Abschneiden von @Section0 in szTCSIniFile
     }

     ftn_WorkString.len= TCS_FILE_NAMELEN;
     ftn_WorkString.addr= szTCSIniFile;

     n.len= _tcslen (INIFILEXT);
     n.addr= INIFILEXT;
     o.len= _tcslen (INIFILEXTTOKEN);
     o.addr= INIFILEXTTOKEN;
     SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                 CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                 CALLFTNSTRL(ftn_WorkString)
                 CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );

     n.len= TCS_FILE_NAMELEN;
     n.addr= (FTNCHAR *) &szTmpString1;
     o.len= _tcslen (PROGDIRTOKEN);
     o.addr= PROGDIRTOKEN;

     _tcsncpy (szTmpString1, szTCSIniFile, TCS_FILE_NAMELEN);
     _tcsrev (szTmpString1); // Abfrage Ende des Strings, Extension rueckwaerts!

     if (_tcsnicmp (szTmpString1, _T("GER."),4) == 0) { // Filename endet .REG?
      n.addr[0]= (FTNCHAR) 0; /* keine Directory sinnvoll -> Token loeschen */
     } else {
      #if !defined(__WIN32__) && !defined(_WIN32)   /* nicht bei DLL möglich */
       #if defined __WATCOMC__
        iL= 0;            /* Argument 0= Voller Programmname mit Directory */
        iL= igetarg ((FTNINT *) &iL, &n);
       #else
        #error "Kompilation für 16bit Windows nur mit Watcom-Compiler möglich"
       #endif
      #else   /* alternativ nur Win32: hInst=NULL: prozesserzeugende Instanz */
       iL= GetModuleFileName(NULL, n.addr, n.len);
      #endif
      if (iL>0) {
       for (i=iL-1;(n.addr[i]!= (FTNCHAR) '\\' ) || (i==0); i--);
       i++;
       if (i < n.len) n.addr[i]= (FTNCHAR) 0; /* jetzt: Programmname entfernt */
      } else {
       n.addr[0]= (FTNCHAR) 0; /* keine Directory bekannt */
      }
     }
     SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                 CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                 CALLFTNSTRL(ftn_WorkString)
                 CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );

    }

#undef TMPSTRLREN
}



extern void TCSdrWIN__ initt1 (HINSTANCE *hParentInstance, HWND *hParentWindow)
{
int         nCmdShow, iX,iY, iSizeX, iSizeY;
DWORD       FirstShow;
WNDCLASS    TCSWndClass;
HMENU       SysMenu;
TCHAR       szTmpString[TCS_FILE_NAMELEN];
TEXTMETRIC  lpTM;

#if defined(__WIN32__) || defined(_WIN32) || defined (REGSUPPORT)
 DWORD       retValue;
 LPVOID      lpMsgBuf;
#endif

#if defined(REGSUPPORT)
 HKEY hSysrootKey, hRootKey,hSectionKey;
 TCHAR szRootKey[TCS_FILE_NAMELEN]= _T("Software\\"); // +IniFilename ohne Ext.
 TCHAR szSectionKey[TCS_FILE_NAMELEN];
 TCHAR szTmpString2[TCS_FILE_NAMELEN];
 DWORD dwSectionKeyLen;
 DWORD TmpStringLen, TmpStringLen2;
 DWORD i, j;
 DWORD retValue2;
#endif

#if (JOURNALTYP == 2)
 RECT   screenrect;
 int iWidthMM, iHeightMM, iWidthPixel, iHeightPixel;
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ * xJournalEntry;
#endif


    if (TCSinitialized) return;   /* Bereits initialisiert */
    TCSinitialized= true;

    PresetProgPar (); // Nach 2.Aufruf: nur Farben keine Namen wiederherstellen

    if ( _tcslen (szTCSIniFile) <= 4) { // Extension muss angegeben werden!
     _tcsncpy (szTCSIniFile, _T("TooShortInitfilename"), TCS_FILE_NAMELEN);
    }

    _tcsncpy (szTmpString, szTCSIniFile, TCS_FILE_NAMELEN);
    _tcsrev (szTmpString); // Abfrage Ende des Strings, Extension rueckwaerts!

    /*
        Falls Extension des Ini-Files .XML: XML-Parser
    */
#if defined(XMLSUPPORT)
    if (_tcsnicmp (szTmpString, _T("LMX."),4) == 0) { // Filename endet .XML?
     XMLreadProgPar (szTCSIniFile);
    } else  // endif Initialisierung ueber *.xml
#endif


    /*
        Falls Extension des Ini-Files .REG: Auswertung der Registry
    */
#if defined(REGSUPPORT)
    if (_tcsnicmp (szTmpString, _T("GER."),4) == 0) { // Filename endet .REG?
     _tcsncat (szRootKey, szTCSIniFile, _tcslen (szTCSIniFile)-4);
     for (hSysrootKey= HKEY_LOCAL_MACHINE; hSysrootKey!= NULL; ) {
      if (!RegOpenKeyEx( hSysrootKey, szRootKey, 0, KEY_READ, &hRootKey)) {
       szSectionKey[0]= (FTNCHAR) 0; // 1. Durchlauf ohne Section
       for (i = 0, retValue= false; !retValue; i++) {
        if (!RegOpenKeyEx( hRootKey, szSectionKey, 0, KEY_READ, &hSectionKey)) {
         for (j = 0, retValue2 = false; !retValue2; j++) {
          TmpStringLen= TCS_FILE_NAMELEN;         // Codewort
          TmpStringLen2= TCS_FILE_NAMELEN;        // Wert des Codewortes
          retValue2= RegEnumValue(hSectionKey, j, szTmpString, &TmpStringLen,
                            NULL, NULL, (LPBYTE) szTmpString2, &TmpStringLen2);
          if (!retValue2) StoreIni (szSectionKey,szTmpString, szTmpString2);
         }
         RegCloseKey(hSectionKey);
        }
        dwSectionKeyLen= TCS_FILE_NAMELEN;
        retValue= RegEnumKeyEx(hRootKey, i, szSectionKey, &dwSectionKeyLen,
                                                    NULL,  NULL, NULL, NULL);
       }
       RegCloseKey(hRootKey);
      }
      if (hSysrootKey == HKEY_LOCAL_MACHINE) {
       hSysrootKey= HKEY_CURRENT_USER;
      } else if (hSysrootKey == HKEY_CURRENT_USER) {
       hSysrootKey= NULL;
      }
     } // 2x: HKEY_LOCAL_MACHINE, HKEY_CURRENT_USER (ueberschreibt LOCAL_MACH.)
    } else  // endif Registryinitialisierung
#endif

    /*
        Falls Extension des Ini-Files .INI: Auswertung der Initialisierungsdatei
    */

    if (_tcsnicmp (szTmpString, _T("INI."),4) == 0) { // Filename endet .INI?
     if (_tcslen(szTCSWindowName)==0)
      GetPrivateProfileString(TCS_INISECT1,TCS_INIVAR_WINNAM,
      TCS_WINDOW_NAME, szTCSWindowName, TCS_WINDOW_NAMELEN, szTCSIniFile);
     if (_tcslen(szTCSstatWindowName)==0)
      GetPrivateProfileString(TCS_INISECT1,TCS_INIVAR_STATNAM,
      TCS_STATWINDOW_NAME,szTCSstatWindowName,TCS_WINDOW_NAMELEN,szTCSIniFile);

     GetPrivateProfileString(TCS_INISECT1,TCS_INIVAR_MAINWINNAM,
      TCS_MAINWINDOW_NAME,szTCSMainWindowName,TCS_WINDOW_NAMELEN,szTCSIniFile);

     GetPrivateProfileString(TCS_INISECT1,TCS_INIVAR_HDCNAM, TCS_HDCFILE_NAME,
                              szTCSHardcopyFile,TCS_FILE_NAMELEN,szTCSIniFile);


     GetPrivateProfileString (TCS_INISECT2,TCS_INIVAR_COPMEN,TCS_INIDEF_COPMEN,
                              szTCSMenuCopyText, STAT_MAXCOLUMNS, szTCSIniFile);
     GetPrivateProfileString (TCS_INISECT2,TCS_INIVAR_FONT,TCS_INIDEF_FONT,
                              szTCSGraphicFont, TCS_FILE_NAMELEN, szTCSIniFile);
     GetPrivateProfileString (TCS_INISECT2,TCS_INIVAR_SYSFONT,TCS_INIDEF_SYSFONT,
                              szTCSSysFont, TCS_FILE_NAMELEN, szTCSIniFile);
     GetPrivateProfileString(TCS_INISECT2,TCS_INIVAR_ICONNAM, TCS_ICONFILE_NAME,
                              szTCSIconFile,TCS_FILE_NAMELEN,szTCSIniFile);

     TCSwindowIniXrelpos= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_WINPOSX, TCS_INIDEF_WINPOSX, szTCSIniFile);
     TCSwindowIniYrelpos= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_WINPOSY, TCS_INIDEF_WINPOSY, szTCSIniFile);
     TCSwindowIniXrelsiz= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_WINSIZX, TCS_INIDEF_WINSIZX, szTCSIniFile);
     TCSwindowIniYrelsiz= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_WINSIZY, TCS_INIDEF_WINSIZY, szTCSIniFile);

     TCSstatWindowIniXrelpos= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_STATPOSX, TCS_INIDEF_STATPOSX, szTCSIniFile);
     TCSstatWindowIniYrelpos= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_STATPOSY, TCS_INIDEF_STATPOSY, szTCSIniFile);
     TCSstatWindowIniXrelsiz= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_STATSIZX, TCS_INIDEF_STATSIZX, szTCSIniFile);
     TCSstatWindowIniYrelsiz= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_STATSIZY, TCS_INIDEF_STATSIZY, szTCSIniFile);

     TCSDefaultLinCol= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_LINCOL,TCS_INIDEF_LINCOL, szTCSIniFile);
     TCSDefaultTxtCol= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_TXTCOL,TCS_INIDEF_TXTCOL, szTCSIniFile);
     TCSDefaultBckCol= GetPrivateProfileInt (TCS_INISECT2,
                       TCS_INIVAR_BCKCOL,TCS_INIDEF_BCKCOL, szTCSIniFile);


     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_HDCOPN,TCS_INIDEF_HDCOPN,
                  szTCSErrorMsg[WRN_HDCFILOPN], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_HDCFILOPN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_HDCOPNL,TCS_INIDEF_HDCOPNL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_HDCWRT,TCS_INIDEF_HDCWRT,
                  szTCSErrorMsg[WRN_HDCFILWRT], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_HDCFILWRT]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_HDCWRTL,TCS_INIDEF_HDCWRTL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_HDCINT,TCS_INIDEF_HDCINT,
                  szTCSErrorMsg[WRN_HDCINTERN], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_HDCFILWRT]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_HDCINTL,TCS_INIDEF_HDCINTL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3, TCS_INIVAR_USR,TCS_INIDEF_USR,
                     szTCSErrorMsg[MSG_USR], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[MSG_USR]= GetPrivateProfileInt (TCS_INISECT3, TCS_INIVAR_USRL,
                     TCS_INIDEF_USRL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3, TCS_INIVAR_HDCACT,TCS_INIDEF_HDCACT,
                     szTCSErrorMsg[MSG_HDCACT], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[MSG_HDCACT]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_HDCACTL,TCS_INIDEF_HDCACTL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_USRWRN,TCS_INIDEF_USRWRN,
                  szTCSErrorMsg[WRN_USRPRESSANY],STAT_MAXCOLUMNS,szTCSIniFile);
     TCSErrorLev[WRN_USRPRESSANY]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_USRWRNL,TCS_INIDEF_USRWRNL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3, TCS_INIVAR_EXIT,TCS_INIDEF_EXIT,
                     szTCSErrorMsg[ERR_EXIT], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[ERR_EXIT]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_EXITL,TCS_INIDEF_EXITL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_COPMEM,TCS_INIDEF_COPMEM,
                  szTCSErrorMsg[WRN_COPYNOMEM], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_COPYNOMEM]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_COPMEML,TCS_INIDEF_COPMEML, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_COPLCK,TCS_INIDEF_COPLCK,
                   szTCSErrorMsg[WRN_COPYLOCK], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_COPYLOCK]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_COPLCKL,TCS_INIDEF_COPLCKL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_JOUCREATE,TCS_INIDEF_JOUCREATE,
                   szTCSErrorMsg[WRN_JOUCREATE], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUCREATE]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_JOUCREATEL,TCS_INIDEF_JOUCREATEL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_JOUENTRY,TCS_INIDEF_JOUENTRY,
                   szTCSErrorMsg[WRN_JOUENTRY], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUENTRY]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_JOUENTRYL,TCS_INIDEF_JOUENTRYL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_JOUADD,TCS_INIDEF_JOUADD,
                   szTCSErrorMsg[WRN_JOUADD], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUADD]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_JOUADDL,TCS_INIDEF_JOUADDL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_JOUCLR,TCS_INIDEF_JOUCLR,
                   szTCSErrorMsg[WRN_JOUCLR], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUCLR]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_JOUCLRL,TCS_INIDEF_JOUCLRL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_JOUUNKWN,TCS_INIDEF_JOUUNKWN,
                   szTCSErrorMsg[WRN_JOUUNKWN], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUUNKWN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_JOUUNKWNL,TCS_INIDEF_JOUUNKWNL, szTCSIniFile);


     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_XMLPARSER,TCS_INIDEF_XMLPARSER,
                   szTCSErrorMsg[ERR_XMLPARSER], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUUNKWN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_XMLPARSERL,TCS_INIDEF_XMLPARSERL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_XMLOPEN,TCS_INIDEF_XMLOPEN,
                   szTCSErrorMsg[ERR_XMLOPEN], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUUNKWN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_XMLOPENL,TCS_INIDEF_XMLOPENL, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_USR2,TCS_INIDEF_USR2,
                   szTCSErrorMsg[MSG_USR2], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUUNKWN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_USR2L,TCS_INIDEF_USR2L, szTCSIniFile);

     GetPrivateProfileString (TCS_INISECT3,TCS_INIVAR_INI2,TCS_INIDEF_INI2,
                   szTCSErrorMsg[WRN_INI2], STAT_MAXCOLUMNS, szTCSIniFile);
     TCSErrorLev[WRN_JOUUNKWN]= GetPrivateProfileInt (TCS_INISECT3,
                     TCS_INIVAR_INI2L,TCS_INIDEF_INI2L, szTCSIniFile);

    } // endif Initialisierung ueber *.ini


    CustomizeProgPar (); // Ersatz %: durch Programmverzeichnis

    /*
    Übernahme der durch den Nutzer angepassten Initialisierungsdaten
    */

    TKTRNX.iLinCol= TCSDefaultLinCol;
    TKTRNX.iTxtCol= TCSDefaultTxtCol;
    TKTRNX.iBckCol= TCSDefaultBckCol;

    /*
        Ermittlung der Instanz des Processes
    */

    hTCSInst= *hParentInstance; // In Hauptprogramm durch INITT ermittelt
    hOwnerWindow= *hParentWindow;

    if (_tcscmp(szTCSMainWindowName,_T("%:")) == 0) {
     _tcsncpy( szTCSMainWindowName,GetCommandLine(), STAT_MAXCOLUMNS);
    }

    CreateMainWindow_IfNecessary (&hTCSInst,&hOwnerWindow,szTCSMainWindowName);

    *hParentWindow= hOwnerWindow;   // Publizieren evtl. neues Handle DLL->Main

    /*
        Ermittlung allgemeiner systemspezifischer Parameter
    */

    TextLineHeight= GetSystemMetrics (SM_CYMENU); /* Höhe Menüeintrag */
    TCSCharHeight= (int)(TCS_REL_CHR_HEIGHT* (float)(HiRes(TextLineHeight)));

    TCSBackgroundColour= TKTRNX.iBckCol;

    TKTRNX.kStCol = STAT_MAXCOLUMNS;
    TKTRNX.iMouse = 3; /* werden z.Zt. bei DCURSR () ausgewertet */

    /*
        Erzeugung des Graphikfensters
    */

    TCSWndClass.style          = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    TCSWndClass.lpfnWndProc    = TCSWndProc;
    TCSWndClass.cbClsExtra     = 0;
    TCSWndClass.cbWndExtra     = 0;
    TCSWndClass.hInstance      = hTCSInst;

    #if (defined(__WIN32__) || defined(_WIN32))
     if (_tcslen (szTCSIconFile) != 0) {
      TCSWndClass.hIcon         = LoadImage (NULL, szTCSIconFile,
                                        IMAGE_ICON,0,0,LR_LOADFROMFILE);
     } else {
      TCSWndClass.hIcon         = LoadIcon (hTCSInst, TCS_WINDOW_ICON);
                               /* Falls Icon nicht definiert->LoadIcon=NULL */
     }
    #else
     TCSWndClass.hIcon         = LoadIcon (hTCSInst, TCS_WINDOW_ICON);
    #endif

    TCSWndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    TCSWndClass.hbrBackground  = NULL; /* Erase-Handler, Brush unnötig */
    TCSWndClass.lpszMenuName   = NULL;
    TCSWndClass.lpszClassName  = TCS_WINDOWCLASS;

     /* Register the window class. Fail: most probable UNICODE on win98 */
    if (!RegisterClass (&TCSWndClass)) {
     #if defined(__WIN32__) || defined(_WIN32)
      retValue= GetLastError(); // win32-Funktion
//    if (retValue == ERROR_CLASS_ALREADY_EXISTS) {
//     Hier bei Bedarf Fehlerbehandlung einführen
//    } else {
       FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         retValue,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
       );
       MessageBox (NULL, lpMsgBuf, szTCSWindowName, MB_ICONSTOP);
       LocalFree( lpMsgBuf ); // Free the buffer
//    } // Ende der Fehlerbehandlung
     #else // rudimentaere Fehlerbehandlung 16bit Windows
      MessageBox (NULL, _T("Window Class not registered"),
                                    szTCSWindowName, MB_ICONSTOP);
     #endif
     return;
    }

    if ((TCSwindowIniXrelsiz < 100) || (TCSwindowIniYrelsiz < 100) ) {
     nCmdShow= SW_SHOWNORMAL; /* Achtung, int = 2Byte bei WIN16!!! */
     iX= (int) ( ( (long int) TCSwindowIniXrelpos *
                 (long int) GetSystemMetrics (SM_CXMAXIMIZED)) / 100);
     iY= (int) ( ( (long int) TCSwindowIniYrelpos *
                 (long int) GetSystemMetrics (SM_CYMAXIMIZED)) / 100);
     iSizeX= (int) ( ( (long int) TCSwindowIniXrelsiz *
                 (long int) GetSystemMetrics (SM_CXMAXIMIZED)) / 100);
     iSizeY= (int) ( ( (long int) TCSwindowIniYrelsiz *
                 (long int) GetSystemMetrics (SM_CYMAXIMIZED)) / 100);
    } else {
     nCmdShow= SW_SHOWMAXIMIZED;
     iX= 0;
     iY= 0;
     iSizeX= GetSystemMetrics (SM_CXMAXIMIZED);
     iSizeY= GetSystemMetrics (SM_CYMAXIMIZED);
    }

    hTCSWindow = CreateWindow(TCS_WINDOWCLASS, szTCSWindowName,
                       WS_OVERLAPPEDWINDOW,
                       iX, iY,
                       iSizeX, iSizeY,
                       hOwnerWindow,
                       (HMENU) NULL,
                       (HINSTANCE) hTCSInst, (LPSTR) NULL);

    if (hTCSWindow == NULL) return;

    hTCSWindowDC = GetDC (hTCSWindow);

    SetWindowExtEx (hTCSWindowDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSWindowDC, TCSrect.left, TCSrect.bottom, NULL);

#if (JOURNALTYP == 1)
    hTCSMetaFileDC = CreateMetaFile (NULL); /* Memory-based 16bit Metafile */
    SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);
    MoveToEx (hTCSMetaFileDC, 0, 0, NULL);  /* Cursorposition Neuzeichnen */

#elif (JOURNALTYP == 2)
    iWidthMM = GetDeviceCaps(hTCSWindowDC, HORZSIZE); // Bildschirmgroesse(mm)
    iHeightMM = GetDeviceCaps(hTCSWindowDC, VERTSIZE);
    iWidthPixel = GetDeviceCaps(hTCSWindowDC, HORZRES); // Bildschirm (Pixel)
    iHeightPixel = GetDeviceCaps(hTCSWindowDC, VERTRES);

    screenrect.left= (TCSrect.left *iWidthMM *100)/iWidthPixel; // in .01 mm
    screenrect.top= (TCSrect.top *iHeightMM *100)/iHeightPixel;
    screenrect.right= (TCSrect.right *iWidthMM *100)/iWidthPixel; // right > left!
    screenrect.bottom= (TCSrect.bottom *iHeightMM *100)/iHeightPixel; // bottom > top!

    hTCSMetaFileDC = CreateEnhMetaFile (hTCSWindowDC, NULL, &screenrect,
            _T("TCS for Windows\0Journalfile created by INITT\0" ));

    SetMapMode (hTCSMetaFileDC, MM_ANISOTROPIC);
    SetViewportExtEx (hTCSMetaFileDC, TCSrect.right, -TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.top, NULL);

    SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    MoveToEx (hTCSMetaFileDC, 0, 0, NULL);  /* Cursorposition Neuzeichnen */
#endif

    ShowWindow (hTCSWindow, nCmdShow);      /* Skalierung Viewport  */
    UpdateWindow(hTCSWindow);               /* in TCSWndProc_OnSize */

    SysMenu = GetSystemMenu (hTCSWindow, FALSE); /* Systemmenu: kein Close */
    DeleteMenu (SysMenu, 6, MF_BYPOSITION);
    AppendMenu (SysMenu,MF_STRING,TCS_WM_COPY,szTCSMenuCopyText);  /* Copy */

    TCSFontdefinition.lfHeight= TCSCharHeight; /* Höhe, Breite */
    TCSFontdefinition.lfWidth= 0;
    TCSFontdefinition.lfEscapement= 0; /* lfEscapement=lfOrientation */
    TCSFontdefinition.lfOrientation= 0;
    TCSFontdefinition.lfWeight= FW_NORMAL; /* Strichstärke */
    TCSFontdefinition.lfItalic= false;
    TCSFontdefinition.lfUnderline= false;
    TCSFontdefinition.lfStrikeOut= false;
    TCSFontdefinition.lfCharSet= ANSI_CHARSET;
    TCSFontdefinition.lfOutPrecision= OUT_TT_ONLY_PRECIS;
    TCSFontdefinition.lfClipPrecision= CLIP_DEFAULT_PRECIS;
    TCSFontdefinition.lfQuality= DRAFT_QUALITY;
    TCSFontdefinition.lfPitchAndFamily= FF_MODERN | FIXED_PITCH;
    _tcscpy (TCSFontdefinition.lfFaceName, szTCSGraphicFont);
                        /* Bevorzugter Font, keine Proportionalschrift!!! */

    hTCSFont= CreateFontIndirect (&TCSFontdefinition);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSWindowDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (hTCSWindowDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #endif
    SetTextColor (hTCSWindowDC, dwColorTable[TKTRNX.iTxtCol]);

    GetTextMetrics (hTCSWindowDC, &lpTM);
    TKTRNX.kitalc= 0;
    TKTRNX.ksizef= 0;
    TKTRNX.khorsz= (FTNINT) ((float)LoRes((float)lpTM.tmAveCharWidth *TEK_XMAX/iSizeX) + 0.25f);
    TKTRNX.kversz= (FTNINT) ((float)LoRes((float)lpTM.tmHeight *TEK_YMAX/iSizeY)  + 0.25f);

    SetBkMode (hTCSWindowDC, TRANSPARENT );   /* Attribut statisch, durch */
    SetTextAlign (hTCSWindowDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP); /* Ort: */

    hTCSPen= CreatePen (PS_SOLID, 0, dwColorTable[TKTRNX.iLinCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSWindowDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

    hGinCurs=LoadCursor(NULL, IDC_CROSS);
    hMouseCurs=LoadCursor(NULL, IDC_ARROW);

#if ( (JOURNALTYP == 1) || (JOURNALTYP == 2) )
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #endif
    SetBkMode (hTCSMetaFileDC, TRANSPARENT );
    SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP);
    SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

#elif (JOURNALTYP == 3)
    hTCSJournal= NULL;
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUCREATE,"");

    xJournalEntry->action=  XACTION_NOOP; // Erkennung Listenanfang: Wurzelelement ohne Funktion
    xJournalEntry->i1= 0;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
    xJournalEntry->action=  XACTION_INITT;
    xJournalEntry->i1= 0;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    /*
        Erzeugung des Statusfensters
    */

    TCSWndClass.style          = CS_HREDRAW | CS_VREDRAW;  //  CS_OWNDC |
    TCSWndClass.lpfnWndProc    = TCSstatWndProc;
    TCSWndClass.hInstance      = hTCSInst;
    TCSWndClass.hIcon          = NULL;
    TCSWndClass.hCursor        = LoadCursor(NULL, IDC_ARROW);
    #if !defined(__WIN32__) && !defined(_WIN32)
     TCSWndClass.hbrBackground  = (HBRUSH) GetStockBrush(WHITE_BRUSH);
    #else
     TCSWndClass.hbrBackground  = GetStockObject(WHITE_BRUSH);
    #endif
    TCSWndClass.lpszMenuName   = NULL;
    TCSWndClass.lpszClassName  = TCS_STAT_WINDOWCLASS;

    if (!RegisterClass (&TCSWndClass)) {
     #if defined(__WIN32__) || defined(_WIN32)
      retValue= GetLastError(); // win32-Funktion
//    if (retValue == ERROR_CLASS_ALREADY_EXISTS) {
//     Hier bei Bedarf Fehlerbehandlung einführen
//    } else {
       FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         retValue,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
       );
       MessageBox (NULL, lpMsgBuf, szTCSWindowName, MB_ICONSTOP);
       LocalFree( lpMsgBuf ); // Free the buffer
//    } // Ende der Fehlerbehandlung
     #else // rudimentaere Fehlerbehandlung 16bit Windows
      MessageBox (NULL, _T("Window Class not registered"),
                                    szTCSWindowName, MB_ICONSTOP);
     #endif
     return;
    }

    if ((TCSstatWindowIniXrelsiz < 100) || (TCSstatWindowIniYrelsiz < 100) ) {
     FirstShow= WS_OVERLAPPED | WS_SIZEBOX | WS_VSCROLL; // WIN16: int*2 !
     iX= (int) ( ( (long int) TCSstatWindowIniXrelpos *
                   (long int) GetSystemMetrics (SM_CXMAXIMIZED)) / 100);
     iY= (int) ( ( (long int) TCSstatWindowIniYrelpos *
                   (long int) GetSystemMetrics (SM_CYMAXIMIZED)) / 100);
     iSizeX= (int) ( ( (long int) TCSstatWindowIniXrelsiz *
                       (long int) GetSystemMetrics (SM_CXMAXIMIZED)) / 100);
     iSizeY= (int) ( ( (long int)  TCSstatWindowIniYrelsiz *
                       (long int) GetSystemMetrics (SM_CYMAXIMIZED) ) / 100);
    } else {
     FirstShow= WS_OVERLAPPED | WS_SIZEBOX | WS_VSCROLL | WS_MAXIMIZE;
     iX= 0;
     iY = GetSystemMetrics (SM_CYMAXIMIZED) -
                    #if defined(__WIN32__) || defined(_WIN32)
                                 (int) (TCS_REL_CHR_SPACE*TextLineHeight) -
                    #endif
                             STAT_MINLINES*GetSystemMetrics (SM_CYMINTRACK);
     iSizeX= GetSystemMetrics (SM_CXMAXIMIZED);
     iSizeY= (int) (TCS_REL_CHR_SPACE*TextLineHeight) +
                             STAT_MINLINES*GetSystemMetrics (SM_CYMINTRACK);
    }

    hTCSstatWindow = CreateWindow(TCS_STAT_WINDOWCLASS, szTCSstatWindowName,
                        FirstShow,
                        iX, iY,
                        iSizeX, iSizeY,
                        (HWND) hTCSWindow, (HMENU) NULL,
                        (HINSTANCE) hTCSInst, (LPSTR) NULL);

    if (hTCSstatWindow == NULL) return;

    #ifdef STAT_WINDOW_PRIVATE
     hTCSstatWindowDC = GetDC (hTCSstatWindow);
    #endif

    TCSFontdefinition.lfHeight= TextLineHeight; /* Buchstabenhöhe */
    _tcscpy (TCSFontdefinition.lfFaceName, szTCSSysFont);
                        /* Bevorzugter Font, keine Proportionalschrift!!! */
    hTCSSysFont= CreateFontIndirect (&TCSFontdefinition);

    TCSFontdefinition.lfHeight= TCSCharHeight; /* Wiederherstellung Graphikzeichensatz */
    _tcscpy (TCSFontdefinition.lfFaceName, szTCSGraphicFont);


    TCSStatWindowAutomatic = true;
    TCSstatCursorPosY= 0;
    TCSstatScrollY= 0;
    TCSstatRow= -1;
    TCSstatOrgY= TCSstatScrollY;
    SetScrollRange (hTCSstatWindow, SB_VERT, 0,STAT_MAXROWS-1, true);
    SetScrollPos (hTCSstatWindow, SB_VERT, TCSstatScrollY, true);

    #ifdef __cplusplus /* Im Komplettpaket durch TCS.FOR in INITT gesetzt */
     TKTRNX.kminsx= 0;
     TKTRNX.kmaxsx= TEK_XMAX;
     TKTRNX.kminsy= 0;
     TKTRNX.kmaxsy= TEK_YMAX;
    #endif

    ShowWindow (hTCSstatWindow, SW_HIDE);

    ClippingNotActive= true;

    return;
}



extern void TCSdrWIN__ finitt ()
{
// FTNINT iErr;
#if (JOURNALTYP == 1)
 HMETAFILE hmf;
#elif (JOURNALTYP == 2)
 HENHMETAFILE hmf;
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ * xJournalEntry;
#endif


    if (!TCSinitialized) return; /* Graphiksystem nicht initialisiert */

    TCSGraphicError (ERR_EXIT,"");  /* TCSinitialized verhindert Rekursion*/

    TCSinitialized= false;           /* Ab jetzt nicht mehr funktionsfähig */

    ReleaseDC (hTCSWindow, hTCSWindowDC);
    DestroyWindow (hTCSWindow);
    UnregisterClass (TCS_WINDOWCLASS, hTCSInst);

#if (JOURNALTYP == 1)
    hmf = CloseMetaFile (hTCSMetaFileDC);
    DeleteMetaFile (hmf);
#elif (JOURNALTYP == 2)
    hmf = CloseEnhMetaFile (hTCSMetaFileDC);
    DeleteEnhMetaFile (hmf);
#elif (JOURNALTYP == 3)
    SGLIB_DL_LIST_MAP_ON_ELEMENTS (struct xJournalEntry_typ, hTCSJournal,
           xJournalEntry,previous,next, {free (xJournalEntry);}); // free all
    hTCSJournal= NULL;
#endif

    #ifdef STAT_WINDOW_PRIVATE
     ReleaseDC (hTCSstatWindow, hTCSstatWindowDC);
    #endif
    DestroyWindow (hTCSstatWindow);
    UnregisterClass (TCS_STAT_WINDOWCLASS, hTCSInst);

    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteFont (hTCSFont);
     DeleteFont (hTCSSysFont);
     DeletePen (hTCSPen);
    #else
     DeleteObject (hTCSFont);
     DeleteObject (hTCSSysFont);
     DeleteObject (hTCSPen);
    #endif

    #if defined(__WATCOMC__) && defined(__SW_BW)
     _dwShutDown();        // Shutdown Watcom Default Window System
    #endif

    if (TCSErrorLev[ERR_EXIT] >= 10) exit (EXIT_SUCCESS); // Programmende
    return; // Bei Fehlerlevel <10 zurück zum Hauptprogramm
}



/*
---------------------- Userroutinen: Zeichnen -------------------
*/



extern void TCSdrWIN__ swind1 (FTNINT *ix1,FTNINT *iy1,FTNINT *ix2,FTNINT *iy2)
{
    ClippingNotActive = (*ix1==0) && (*iy1==0) &&
                                        (*ix2==TEK_XMAX) && (*iy2==TEK_YMAX);
    /* Berechnung BOOL zur Wahrung der Programmstruktur der DOS-Version */
}



extern void TCSdrWIN__ erase (void)
{
#if (JOURNALTYP == 1)
 HMETAFILE hmf;
 HRGN       hWindowRegion;
 HBRUSH     hBack;
#elif (JOURNALTYP == 2)
 HENHMETAFILE   hmf;
 ENHMETAHEADER emh ;
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

#if (JOURNALTYP == 1)
     hmf = CloseMetaFile (hTCSMetaFileDC);   /* Cursor, Farben unverändert! */
     DeleteMetaFile (hmf);                   /* alter Status Bildschirm */
     hTCSMetaFileDC  = CreateMetaFile (NULL);/* für neues Journalfile */
     SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
     SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

     hBack= CreateSolidBrush (dwColorTable[TKTRNX.iBckCol]);
     hWindowRegion= CreateRectRgn (TCSrect.left, TCSrect.top, TCSrect.right,TCSrect.bottom); // rechts,oben
     FillRgn (hTCSMetaFileDC, hWindowRegion, hBack);         // nicht eingeschlossen
     #if !defined(__WIN32__) && !defined(_WIN32)
      DeleteBrush (hBack);
      DeleteRgn (hWindowRegion);                   /* Resourcen freigeben */
      SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
     #else
      DeleteObject (hBack);
      DeleteObject (hWindowRegion);
      SelectObject (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
     #endif

     SetBkMode (hTCSMetaFileDC, TRANSPARENT );
     SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP);
     SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
     #endif

     MoveToEx (hTCSMetaFileDC, HiRes(TKTRNX.kBeamX), HiRes(TKTRNX.kBeamY), NULL);

#elif (JOURNALTYP == 2)
     hmf = CloseEnhMetaFile (hTCSMetaFileDC);
     GetEnhMetaFileHeader (hmf, sizeof (emh), &emh) ;
     DeleteEnhMetaFile (hmf);                      // alter Status Bildschirm

     hTCSMetaFileDC  = CreateEnhMetaFile (hTCSWindowDC, NULL, &emh.rclFrame,
                       _T("TCS for Windows\0Journalfile created by Erase\0\0"));

     SetMapMode (hTCSMetaFileDC, MM_ANISOTROPIC);
     SetViewportExtEx (hTCSMetaFileDC, TCSrect.right, -TCSrect.bottom, NULL);
     SetViewportOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.top, NULL);
     SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
     SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
     #else
      SelectObject (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
     #endif
     SetBkMode (hTCSMetaFileDC, TRANSPARENT );
     SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP);
     SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
     #endif

     MoveToEx (hTCSMetaFileDC, HiRes(TKTRNX.kBeamX),HiRes(TKTRNX.kBeamY), NULL);

#elif (JOURNALTYP == 3)
     SGLIB_DL_LIST_MAP_ON_ELEMENTS (struct xJournalEntry_typ, hTCSJournal,
           xJournalEntry,previous,next, {free (xJournalEntry);}); // free all
     hTCSJournal= NULL;

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_NOOP;
     xJournalEntry->i1= 0;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_LINCOL;
     xJournalEntry->i1= TKTRNX.iLinCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_TXTCOL;
     xJournalEntry->i1= TKTRNX.iTxtCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_BCKCOL;
     xJournalEntry->i1= TKTRNX.iBckCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_ERASE;
     xJournalEntry->i1= 0;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    TCSBackgroundColour=TKTRNX.iBckCol; /* Jetzt in ERASE-Handler wirksam */

    InvalidateRect (hTCSWindow, NULL, true); /* ,ClientArea, EraseFlag */
    UpdateWindow (hTCSWindow); /* 16bit Rechner: gegen Irritation Anwender */

}



#ifdef __cplusplus /* Erweiterte Version in TCS.FOR, nur C++ Version */

extern TCSdrWIN__ swindo (FTNINT *ix,FTNINT *iLx, FTNINT *iy,FTNINT *iLy)
{
    TKTRNX.kminsx= *ix;
    TKTRNX.kmaxsx= *ix + *iLx;
    TKTRNX.kminsy= *iy;
    TKTRNX.kmaxsy= *iy + *iLy;
}

#endif



extern void TCSdrWIN__ movabs (FTNINT *ix,FTNINT *iy)
{
int ixx, iyy; /* Erzwingt Typangleichung Windows-GDI / Fortran */

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    if (PointInWindow (*ix, *iy)) {
     ixx= HiRes(*ix); iyy= HiRes(*iy);
     MoveToEx (hTCSWindowDC, ixx, iyy, NULL);

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     MoveToEx (hTCSMetaFileDC, ixx, iyy, NULL);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= *ix;
     xJournalEntry->i2= *iy;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif
    }
}



extern void TCSdrWIN__ drwabs (FTNINT *ix,FTNINT *iy)
{
FTNINT iXClip, iYClip;
int ixx, iyy;

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    if (ClipLineStart(TKTRNX.kBeamX,TKTRNX.kBeamY, *ix,*iy, &iXClip,&iYClip)) {
     ixx= HiRes(iXClip); iyy= HiRes(iYClip);
     MoveToEx (hTCSWindowDC, ixx,iyy, NULL);
#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     MoveToEx (hTCSMetaFileDC, ixx,iyy, NULL);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

     ClipLineStart(*ix,*iy, TKTRNX.kBeamX,TKTRNX.kBeamY, &iXClip,&iYClip);
     ixx= HiRes(iXClip); iyy= HiRes(iYClip);    /* geclippter Endpunkt */
     LineTo (hTCSWindowDC, ixx,iyy);      /* Endpunkt nicht mitgezeichnet! */
     SetPixel (hTCSWindowDC,ixx,iyy,dwColorTable[TKTRNX.iLinCol]);

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     LineTo (hTCSMetaFileDC, ixx,iyy);
     SetPixel (hTCSMetaFileDC,ixx,iyy, dwColorTable[TKTRNX.iLinCol]);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_DRWABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= *ix;
     xJournalEntry->i2= *iy;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    }
    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;

}



extern void TCSdrWIN__ dshabs (FTNINT *ix,FTNINT *iy, FTNINT *iMask)
{
HPEN     hPenDash;
FTNINT iXClip, iYClip;
int     iMaskIndex, ixx, iyy;

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    if (*iMask < 0) {    /* Verhindern eines Access-Errors bei Integermaskenübergabe */
     iMaskIndex= 0;
    } else if (*iMask > MAX_PENSTYLE_INDEX) {
     iMaskIndex= 1;      /* Style: dotted */
    } else {
     iMaskIndex= *iMask;
    }

    if (ClipLineStart(TKTRNX.kBeamX,TKTRNX.kBeamY, *ix,*iy, &iXClip,&iYClip)) {
     ixx= HiRes(iXClip); iyy= HiRes(iYClip);
     MoveToEx (hTCSWindowDC, ixx,iyy, NULL);

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     MoveToEx (hTCSMetaFileDC, ixx,iyy, NULL);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

     ClipLineStart(*ix,*iy, TKTRNX.kBeamX,TKTRNX.kBeamY, &iXClip,&iYClip);
     ixx= HiRes(iXClip); iyy= HiRes(iYClip);      /* geclippter Endpunkt */

     hPenDash= CreatePen (dwPenStyle[iMaskIndex], 0, dwColorTable[TKTRNX.iLinCol]);
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSWindowDC, hPenDash); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSWindowDC, hPenDash); // 32bit: GDI Standardaufruf
     #endif
     LineTo (hTCSWindowDC, ixx,iyy);   /* Ohne Endpunkt bei Dash o.k! */
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSWindowDC, hTCSPen); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
     #endif

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSMetaFileDC, hPenDash); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSMetaFileDC, hPenDash); // 32bit: GDI Standardaufruf
     #endif
     LineTo (hTCSMetaFileDC, ixx,iyy);
     #if !defined(__WIN32__) && !defined(_WIN32)
      SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
     #else
      SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
     #endif
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_DSHSTYLE;
     xJournalEntry->i1= iMaskIndex;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_DSHABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= *ix;
     xJournalEntry->i2= *iy;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

     #if !defined(__WIN32__) && !defined(_WIN32)
      DeletePen (hPenDash);
     #else
      DeleteObject (hPenDash);
     #endif

    }
    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
}



extern void TCSdrWIN__ pntabs (FTNINT *ix,FTNINT *iy)
{
int     ixx, iyy; /* Erzwingt Typangleichung Windows-GDI / Fortran */

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    if (PointInWindow (*ix, *iy)) {
     ixx= HiRes(*ix); iyy= HiRes(*iy);
     SetPixel (hTCSWindowDC,ixx,iyy, dwColorTable[TKTRNX.iLinCol]);

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     SetPixel (hTCSMetaFileDC,ixx,iyy,dwColorTable[TKTRNX.iLinCol]);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_PNTABS;
     xJournalEntry->i1= *ix;
     xJournalEntry->i2= *iy;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    }
}



extern void TCSdrWIN__ bckcol (FTNINT *iCol)
{

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.iBckCol= min(abs(*iCol),MAX_COLOR_INDEX);

#if (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
    xJournalEntry->action=  XACTION_BCKCOL;
    xJournalEntry->i1= TKTRNX.iBckCol;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

}



extern void TCSdrWIN__ lincol (FTNINT *iCol)
{

HPEN    hPenOld;

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.iLinCol= min(abs(*iCol),MAX_COLOR_INDEX);
    hTCSPen= CreatePen (PS_SOLID, 0, dwColorTable[TKTRNX.iLinCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     hPenOld= SelectPen (hTCSWindowDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     hPenOld= SelectObject (hTCSWindowDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
    xJournalEntry->action=  XACTION_LINCOL;
    xJournalEntry->i1= TKTRNX.iLinCol;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    #if !defined(__WIN32__) && !defined(_WIN32)
     DeletePen (hPenOld);
    #else
     DeleteObject (hPenOld);
    #endif

}




extern void TCSdrWIN__ txtcol (FTNINT *iCol)
{

#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.iTxtCol= min(abs(*iCol),MAX_COLOR_INDEX);
    SetTextColor (hTCSWindowDC, dwColorTable[TKTRNX.iTxtCol]);
#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
    SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
    xJournalEntry->action=  XACTION_TXTCOL;
    xJournalEntry->i1= TKTRNX.iTxtCol;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

}



extern void TCSdrWIN__ DefaultColour (void)
{
    TKTRNX.iLinCol= TCSDefaultLinCol;
    TKTRNX.iTxtCol= TCSDefaultTxtCol;
    TKTRNX.iBckCol= TCSDefaultBckCol;

    lincol (&TKTRNX.iLinCol);
    txtcol (&TKTRNX.iTxtCol);
    bckcol (&TKTRNX.iBckCol);
}



/*
---------------------- Userroutinen: Graphiktext -----------------------
*/



extern void TCSdrWIN__ outgtext(FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) )
{
int iL;
SIZE Size;
POINT CPpos;

#if (JOURNALTYP == 3)
 int i;
 struct xJournalEntry_typ    * xJournalEntry;
#endif

#ifdef extended_error_handling
 HDC         hdc;
 LPVOID      lpMsgBuf;
#endif


    if (FTNSTRPARA(ftn_string)[0] == (FTNCHAR) 0 ) return; // Leerstring char(0)

    iL= 1; // Stringbeginn bei 0 -> Dec Laenge
    while ( (FTNSTRPARA(ftn_string)[iL-1] != (FTNCHAR) 0) &&  // c-String bis \0
                    (iL < FTNSTRPARL(ftn_string)) ) iL++;  // oder Ftn-String
    if (FTNSTRPARA(ftn_string)[iL-1] == (FTNCHAR) 0 ) iL--;   // cString ohne \0


    #ifdef extended_error_handling
     if (GetTextExtentPoint (hTCSWindowDC, FTNSTRPARA(ftn_string),iL,&Size) == 0 ){
      hdc = CreateIC (_T ("DISPLAY"), NULL, NULL, NULL);
      #if !defined(__WIN32__) && !defined(_WIN32)
       SelectFont (hdc, hTCSFont);      // Aktuellen Zeichenstatus an
      #else
       SelectObject (hdc, hTCSFont);      // Aktuellen Zeichenstatus an
      #endif
      GetTextExtentPoint (hdc, FTNSTRPARA(ftn_string),iL,&Size);
      DeleteDC (hdc);

      FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
      );
      MessageBox( NULL, lpMsgBuf,
                        _T("Internal Error GRAPH2D - subroutine _OUTGTEXT"),
                                                  MB_OK|MB_ICONINFORMATION );
      LocalFree( lpMsgBuf ); // Free the buffer
     }
    #else
     #if !defined(__WIN32__) && !defined(_WIN32)
      GetTextExtentPoint (hTCSWindowDC, FTNSTRPARA(ftn_string),iL,&Size);
     #else
      GetTextExtentPoint32 (hTCSWindowDC, FTNSTRPARA(ftn_string),iL,&Size);
     #endif
    #endif

    if (PointInWindow (TKTRNX.kBeamX+LoRes(Size.cx),
                                            TKTRNX.kBeamY+LoRes(Size.cy))) {
     MoveToEx (hTCSWindowDC,HiRes(TKTRNX.kBeamX),HiRes(TKTRNX.kBeamY),NULL);
     TextOut (hTCSWindowDC, 0,0,FTNSTRPARA(ftn_string), iL);

#if ((JOURNALTYP == 1) || (JOURNALTYP == 2))
     MoveToEx (hTCSMetaFileDC,HiRes(TKTRNX.kBeamX),HiRes(TKTRNX.kBeamY),NULL);
     TextOut (hTCSMetaFileDC, 0,0, FTNSTRPARA(ftn_string), iL);
#elif (JOURNALTYP == 3)
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= TKTRNX.kBeamX;
     xJournalEntry->i2= TKTRNX.kBeamY;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     xJournalEntry->action=  XACTION_GTEXT;
     xJournalEntry->i1= (FTNINT) iL;
     xJournalEntry->i2= (FTNINT) FTNSTRPARA(ftn_string)[0];
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)

     i= 1;
     while (i < iL) {
      xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
      xJournalEntry->action=  XACTION_ASCII;
      xJournalEntry->i1= (FTNINT) FTNSTRPARA(ftn_string)[i++];
      if ( i<iL ) xJournalEntry->i2= (FTNINT) FTNSTRPARA(ftn_string)[i++];
      SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
     }
#endif

     GetCurrentPositionEx (hTCSWindowDC, &CPpos); /* Update Beam */
     TKTRNX.kBeamX= LoRes(CPpos.x); TKTRNX.kBeamY= LoRes(CPpos.y);

#if (JOURNALTYP == 3) // Bei Metafiles ist auch nach Neuskalierung CP i.O.
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= TKTRNX.kBeamX;
     xJournalEntry->i2= TKTRNX.kBeamY;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif

    }
}



extern void TCSdrWIN__ italic (void)
{
HFONT   hOldFont;
#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.kitalc = 1;

    TCSFontdefinition.lfItalic= true;
    hTCSFont= CreateFontIndirect (&TCSFontdefinition);
    #if !defined(__WIN32__) && !defined(_WIN32)
     hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
    #else
     hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
    #endif
#if ( (JOURNALTYP == 1) || (JOURNALTYP == 2) )
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);
    #endif
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteFont (hOldFont);
    #else
     DeleteObject (hOldFont);
    #endif
}



extern void TCSdrWIN__ italir (void)
{
HFONT   hOldFont;
#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.kitalc = 0;

    TCSFontdefinition.lfItalic= false;
    hTCSFont= CreateFontIndirect (&TCSFontdefinition);
    #if !defined(__WIN32__) && !defined(_WIN32)
     hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
    #else
     hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
    #endif
#if ( (JOURNALTYP == 1) || (JOURNALTYP == 2) )
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);
    #endif
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteFont (hOldFont);
    #else
     DeleteObject (hOldFont);
    #endif
}



extern void TCSdrWIN__ dblsiz (void)
{
HFONT   hOldFont;
#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.ksizef = 1;
    TKTRNX.khomey = TEK_YMAX - 3.0f*TKTRNX.kversz;

    TCSFontdefinition.lfHeight= 2* TCSCharHeight;
    TCSFontdefinition.lfWidth= 0;
    hTCSFont= CreateFontIndirect (&TCSFontdefinition);
    #if !defined(__WIN32__) && !defined(_WIN32)
     hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
    #else
     hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
    #endif
#if ( (JOURNALTYP == 1) || (JOURNALTYP == 2) )
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);
    #endif
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteFont (hOldFont);
    #else
     DeleteObject (hOldFont);
    #endif
}



extern void TCSdrWIN__ nrmsiz (void)
{
HFONT   hOldFont;
#if (JOURNALTYP == 3)
 struct xJournalEntry_typ    * xJournalEntry;
#endif

    TKTRNX.ksizef = 0;
    TKTRNX.khomey = TEK_YMAX - 1.5f*TKTRNX.kversz;

    TCSFontdefinition.lfHeight= TCSCharHeight;
    TCSFontdefinition.lfWidth= 0;
    hTCSFont= CreateFontIndirect (&TCSFontdefinition);
    #if !defined(__WIN32__) && !defined(_WIN32)
     hOldFont= SelectFont (hTCSWindowDC, hTCSFont);
    #else
     hOldFont= SelectObject (hTCSWindowDC, hTCSFont);
    #endif
#if ( (JOURNALTYP == 1) || (JOURNALTYP == 2) )
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);
    #endif
#elif (JOURNALTYP == 3)
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, hTCSJournal, xJournalEntry, previous, next)
#endif
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteFont (hOldFont);
    #else
     DeleteObject (hOldFont);
    #endif
}



extern void TCSdrWIN__ csize (FTNINT *ix,FTNINT *iy)
{
TEXTMETRIC  lpTM;

#ifdef extended_error_handling
 HDC         hdc;
 LPVOID      lpMsgBuf;
#endif

    #ifdef extended_error_handling
     if (GetTextMetrics (hTCSWindowDC, &lpTM)== 0) {
      /* WATCOM ohne Default-Windowsystem(auch bei Consolenanwendungen):
         evtl. kein Message-Loop vorhanden.
         Workaround: Abfrageschleife in MessageBox                       */

      hdc = CreateIC (_T ("DISPLAY"), NULL, NULL, NULL);
      #if !defined(__WIN32__) && !defined(_WIN32)
       SelectFont (hdc, hTCSFont);
      #else
       SelectObject (hdc, hTCSFont);
      #endif
      GetTextMetrics (hdc, &lpTM);
      DeleteDC (hdc);

      FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
        NULL,
        GetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
        (LPTSTR) &lpMsgBuf,
        0,
        NULL
      );
      MessageBox( NULL, lpMsgBuf, "Internal Error GRAPH2D - subroutine CSIZE",
                                                    MB_OK|MB_ICONINFORMATION );
      LocalFree( lpMsgBuf ); // Free the buffer
     }
    #else
     GetTextMetrics (hTCSWindowDC, &lpTM);
    #endif
    *ix= (int) ((float)LoRes((float)lpTM.tmAveCharWidth) + 0.25f);
    *iy= (int) ((float)LoRes((float)lpTM.tmHeight)  + 0.25f);

}




/*
---------------------- Userroutinen: Graphic Input----------------------
*/



extern void TCSdrWIN__ tinput (FTNINT *ic)
{
MSG msg;        /* Message information */
TCHAR iChar;
HWND hAktWindowInThread;

    if (!TCSinitialized) return;            /* Aufhängen vermeiden */
    TCSStatWindowAutomatic = false;          /* Meldungen lesbar */
    iChar= (TCHAR) 0;
    hAktWindowInThread= GetFocus(); // Fuer Texteingabe eigene Applikation
    while (iChar == (TCHAR) 0) { // Messageschleife jetzt hier -> Usereingabe
     SetFocus (hTCSWindow);         // Kein Zugang Elternfenster (Aufhängen!)
     #ifdef extended_error_handling
      if (GetMessage (&msg, NULL, WM_NULL, WM_USER) == -1) {
       MessageBox(NULL, "GetMessage failed in Mesageloop of Graphic Window",
                        "Internal Information GRAPH2D - Subroutine TINPUT",
                        MB_OK | MB_ICONINFORMATION);
      }
     #else
      GetMessage (&msg, NULL, WM_NULL, WM_USER); // Achtung wg. win7 nicht 0,0)
     #endif
     if ((msg.hwnd != hTCSWindow) && (msg.hwnd != hTCSstatWindow) ) {
      switch (msg.message) {
       case WM_NCLBUTTONDOWN:   /* Fensterbefehle der Elternfenster zulassen */
       case WM_NCLBUTTONUP:
       case WM_NCLBUTTONDBLCLK:
       case WM_SYSKEYDOWN:
       case WM_SYSKEYUP:
       case WM_SYSCOMMAND:
        DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
        break;
       case WM_PAINT:
        UpdateWindow( msg.hwnd);
        break;
       default:
        SetFocus (hTCSWindow);
        UpdateWindow (hTCSWindow);
      }
     } else if (msg.hwnd == hTCSstatWindow) { /* Meldungen Statusfenster */
      switch (msg.message) {
       case WM_NCLBUTTONDOWN:    /* Scrollen und Verschieben zulassen */
       case WM_NCLBUTTONUP:
       case WM_NCLBUTTONDBLCLK:
       case WM_VSCROLL:
        DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
        break;
       case WM_PAINT:
         TCSstatWndProc_OnPaint (hTCSstatWindow);
         break;
       case WM_LBUTTONDOWN:
        iChar= (FTNINT) 27;    /* Verlassen PRESSANY durch Statusfenster */
        break;
      }
     } else { /* eigene Meldungen des Graphikfensters */
      switch (msg.message) {
       case WM_PAINT:
        TCSWndProc_OnPaint (msg.hwnd);
        break;
       case WM_RBUTTONDOWN:      /* Auf Wunsch Statusfenster sichtbar */
        ShowWindow (hTCSstatWindow, SW_SHOWNA);
        UpdateWindow(hTCSstatWindow);
        SetFocus (hTCSWindow);
        UpdateWindow (hTCSWindow);
        break;
       case WM_LBUTTONDOWN:
        ShowWindow (hTCSstatWindow, SW_HIDE);
        break;
       case WM_LBUTTONUP:
       case WM_MBUTTONUP:
       case WM_RBUTTONUP:
       case WM_MBUTTONDOWN:
       case WM_LBUTTONDBLCLK:
       case WM_RBUTTONDBLCLK:
       case WM_MBUTTONDBLCLK:
        SetFocus (hTCSWindow);
        UpdateWindow (hTCSWindow);
        break;
       case WM_KEYDOWN:          /* Hardwareanpassung, dann WM_CHAR */
       case WM_KEYUP:
        TranslateMessage (&msg);
        break;
       case WM_CHAR:             /* nach WM_KEYDOWN jetzt ASCII */
        iChar= (TCHAR) msg.wParam;
        break;
       case WM_KILLFOCUS:
        TCSStatWindowAutomatic= true; /* Statusfenster unsichtbar */
        ShowWindow (hTCSstatWindow, SW_HIDE); /* jetzt DefWindowProc */
        UpdateWindow (hTCSstatWindow);
       case WM_NCLBUTTONDOWN:
       case WM_NCLBUTTONUP:
       case WM_NCLBUTTONDBLCLK:
       case WM_SYSKEYDOWN:       /* Uebersetzt in WM_SYSCOMMAND */
       case WM_SYSKEYUP:
        DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
        break;
       case WM_QUIT:
        #ifdef trace_calls
         MessageBox(NULL, "WM_QUIT Graphic Window",
                         "Internal Information GRAPH2D - Subroutine TINPUT",
                         MB_OK | MB_ICONINFORMATION);
        #endif
       case WM_SYSCOMMAND:       /* und nach WM_SYSKEYDOWN Befehlsauswertung */
        switch (msg.wParam) {
         case SC_CLOSE:
          iChar= (FTNINT) 27;    /* <ALT><F4> -> ESC */
          break;
         case TCS_WM_COPY:
          #ifdef trace_calls
           MessageBox(NULL, "WM_SYSCOMMAND (TCS_WM_COPY)",
                       "Internal Information GRAPH2D - Subroutine TINPUT",
                        MB_OK | MB_ICONINFORMATION);
          #endif
          TCSWndProc_OnCopyClipboard ();
          break;
         default:
          DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam);
          break;
        } /* Systembefehle */
      } /* Window-Messageauswertung */
     } /* Meldungen des Graphikfensters */
    } /* Ende Eingabeschleife */
    *ic= (FTNINT) iChar;
    TCSStatWindowAutomatic= true;
    ShowWindow (hTCSstatWindow, SW_HIDE); /* Statusfenster unsichtbar */
    if (hAktWindowInThread != NULL) SetFocus (hAktWindowInThread);
    return;
}




extern void TCSdrWIN__ dcursr (FTNINT *ic,FTNINT *ix,FTNINT *iy)
{
MSG msg;        /* Message information */
TCHAR iButton, iKey;

#if defined(__WIN32__) || defined(_WIN32)
 POINT MousePos;
#endif

    if (!TCSinitialized) return;             /* Aufhängen vermeiden */
    TCSStatWindowAutomatic = false;           /* Meldungen lesbar */

    InvalidateRect (hTCSWindow, NULL, true); /* ,ClientArea, EraseFlag */
    UpdateWindow (hTCSWindow); /* Notwendig bei OnPaint mit Journaltyp=3 */

    iButton= (TCHAR) 0; iKey= (TCHAR) 0;

    /* Setzen der Maus auf die alte GinCursor Position */

    #if defined(__WIN32__) || defined(_WIN32)
     MousePos.x= HiRes(TCSGinCurPos.x); MousePos.y= HiRes(TCSGinCurPos.y);
     LPtoDP (hTCSWindowDC, (LPPOINT)&MousePos, 1);
     MapWindowPoints(hTCSWindow, HWND_DESKTOP, (LPPOINT)&MousePos, 1);
     MousePos.x=  MousePos.x* MOUSE_XMAX / GetSystemMetrics (SM_CXSCREEN);
     MousePos.y=  MousePos.y* MOUSE_YMAX / GetSystemMetrics (SM_CYSCREEN);
     mouse_event(MOUSEEVENTF_MOVE | MOUSEEVENTF_ABSOLUTE,
                                       MousePos.x,MousePos.y, 0, 0);
    #endif

    SetCursor(hGinCurs);       /* WM_SETCURSOR wird ab hier nicht erzeugt! */
    while (iButton == (TCHAR) 0) {  /* Messageschleife jetzt hier  */
     SetFocus (hTCSWindow);    /* Kein Zugang Elternfenster (Aufhängen!) */
     GetMessage (&msg, NULL, WM_NULL, WM_USER); // Achtung wg. win7 nicht 0,0)
     if (msg.hwnd == hTCSstatWindow) { /* Statusfenster stört -> unsichtbar */
      switch (msg.message) {
       case WM_MOUSEMOVE:                  /* falls Cursor über Client-Area */
        TCSStatWindowAutomatic= true;
        ShowWindow (hTCSstatWindow, SW_HIDE);
       case WM_NCMOUSEMOVE:            /* Cursor ueber Titelleiste -> Pfeil */
        SetCursor (hMouseCurs);
        break;
      }
     }              /* Statuszeile und Scrollbar können noch angewählt werden */
     if (msg.hwnd != hTCSWindow) {
      switch (msg.message) {
       case WM_NCLBUTTONDOWN:    /* Fensterbefehle der Elternfenster zulassen */
       case WM_NCLBUTTONUP:
       case WM_NCLBUTTONDBLCLK:
       case WM_SYSKEYDOWN:
       case WM_SYSKEYUP:
       case WM_SYSCOMMAND:
        DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
        break;
       case WM_PAINT:
        if (msg.hwnd == hTCSstatWindow) {
         TCSstatWndProc_OnPaint (hTCSstatWindow);
        } else {
         UpdateWindow( msg.hwnd);
        }
        break;
       default:
        SetFocus (hTCSWindow);
        UpdateWindow (hTCSWindow);
      }
     } else { /* eigene Meldungen des Graphikfensters */
      switch (msg.message) {
       case WM_PAINT:
        TCSWndProc_OnPaint (msg.hwnd);
        break;
       case WM_NCMOUSEMOVE:    /* Cursor ueber Titelleiste -> Pfeil */
        SetCursor (hMouseCurs);
        break;
       case WM_MOUSEMOVE:     /* GinCursor evtl. von Titelleiste zurück */
        SetCursor (hGinCurs);
        iKey= (TCHAR) 0;      /* Tastenbetätigung außerhalb Graphikfenster */
        break;
       case WM_NCLBUTTONDOWN: /* Titelleiste kann Statusfenster steuern */
        TCSStatWindowAutomatic= true;
        ShowWindow (hTCSstatWindow, SW_HIDE); /* jetzt DefWindowProc ! */
       case WM_NCLBUTTONUP:
       case WM_NCLBUTTONDBLCLK:
       case WM_SYSKEYDOWN:       /* Uebersetzt in WM_SYSCOMMAND */
       case WM_SYSKEYUP:
        DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
        break;
       case WM_NCRBUTTONDOWN:
        ShowWindow (hTCSstatWindow, SW_SHOWNA);
        UpdateWindow(hTCSstatWindow);
        break;
       case WM_LBUTTONDOWN: {
        #if !defined(__WIN32__) && !defined(_WIN32)
LftDwn:
        #endif
        if (iKey== (TCHAR) 0) iButton= 1; else iButton=iKey;
       }
       case WM_RBUTTONDOWN: if (iButton== (TCHAR) 0) iButton= 2;
       case WM_MBUTTONDOWN: if (iButton== (TCHAR) 0) iButton= 4; // wie DOS
        #if !defined(__WIN32__) && !defined(_WIN32)
         TCSGinCurPos= MAKEPOINT (msg.lParam);
        #else
         TCSGinCurPos.x= GET_X_LPARAM (msg.lParam);
         TCSGinCurPos.y= GET_Y_LPARAM (msg.lParam);
        #endif
        DPtoLP (hTCSWindowDC, (LPPOINT)&TCSGinCurPos, 1);
        TCSGinCurPos.x= LoRes(TCSGinCurPos.x);
        TCSGinCurPos.y= LoRes(TCSGinCurPos.y);
        break;
       case WM_LBUTTONUP: /* Falls erneuter Aufruf nach Taste unten wird */
       case WM_RBUTTONUP: /* der Cursor sonst wieder auf Pfeil umgestellt */
       case WM_MBUTTONUP:
        SetCursor (hGinCurs);
        break;
       case WM_KEYDOWN:          /* Hardwareanpassung, dann WM_CHAR */
       case WM_KEYUP:
        TranslateMessage (&msg);
        break;
       case WM_CHAR:             /* nach WM_KEYDOWN jetzt ASCII */
        iKey= (TCHAR) msg.wParam;
        #if !defined(__WIN32__) && !defined(_WIN32)
         goto LftDwn;           /* Workaround Fehlen mouse_event */
        #else
         mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
         break;
        #endif
       case WM_SYSCOMMAND:       /* und nach WM_SYSKEYDOWN Befehlsauswertung */
        switch (msg.wParam) {
         case SC_CLOSE:
          iKey= (FTNINT) 27;     /* <ALT><F4> -> ESC */
          #if !defined(__WIN32__) && !defined(_WIN32)
           goto LftDwn;
          #else
           mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
           break;
          #endif
         case TCS_WM_COPY:
          TCSWndProc_OnCopyClipboard ();
          break;
         default:
          DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam);
          break;                 /* Sonst keine Befehle auswerten */
        } /* Systembefehle */
      } /* Window-Messageauswertung */
     } /* Messages fuer Graphikfenster */
    } /* Ende Eingabeschleife */
    *ic= (FTNINT) iButton;
    *ix=TCSGinCurPos.x;
    *iy=TCSGinCurPos.y;

    TCSStatWindowAutomatic= true;
    ShowWindow (hTCSstatWindow, SW_HIDE); /* Statusfenster unsichtbar */
    return;
}



/*
---------------------- Userroutinen: Statusmeldungen -------------------
*/



extern void TCSdrWIN__ bell (void)
{
    MessageBeep (-1);
}




extern void TCSdrWIN__ outtext (FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) )
{
int i;

    TCSstatRow++;
    if (TCSstatRow >= STAT_MAXROWS) {
     TCSstatRow= STAT_MAXROWS-1;
     for (i=0; i<TCSstatRow;i++)
      _tcscpy( TCSstatTextBuf[i],TCSstatTextBuf[i+1]);
    }

    _tcsncpy( TCSstatTextBuf[TCSstatRow],FTNSTRPARA(ftn_string),
                        min (FTNSTRPARL(ftn_string), STAT_MAXCOLUMNS));
    TCSstatTextBuf[TCSstatRow][STAT_MAXCOLUMNS]= (FTNCHAR) 0;
    // TCSstatTextBuf ist mit STAT_MAXCOLUMNS+1 fuer char(0) dimensioniert!

    TCSstatScrollY= TCSstatRow   /* Anzahl  Zeilen  im Display */;
    ScrollWindow (hTCSstatWindow, 0,
                (TCSstatOrgY-TCSstatScrollY)*TextLineHeight, NULL, NULL);

    TCSstatOrgY= TCSstatScrollY;

    SetScrollPos (hTCSstatWindow, SB_VERT, TCSstatScrollY, true);

    ShowWindow (hTCSstatWindow, SW_SHOW);
    UpdateWindow(hTCSstatWindow);
}



extern void TCSdrWIN__ GraphicError (FTNINT *iErr, FTNSTRPAR *ftn_string,
                                     FTNINT *iL  FTNSTRPAR_TAIL(ftn_string))
{
    TCSGraphicError (*iErr, FTNSTRPARA(ftn_string));

}



/*
---------------------- Userroutinen: Hardcopy -------------------
*/


extern void TCSdrWIN__ hdcopy (void)
{
FTNINT      iErr;
// FTNSTRDESC  ftnstrg;
TCHAR       FilNam[TCS_FILE_NAMELEN], OldFilNam[TCS_FILE_NAMELEN];
OFSTRUCT    ReOpenBuf;
#ifdef __cplusplus
 TCHAR      MessageBuf[STAT_MAXCOLUMNS]
#endif

#if (JOURNALTYP == 1)
 HMETAFILE  hmf, hmf1;
 HDC        hTCSNewMetaFileDC;
 HRGN       hWindowRegion;
 HBRUSH     hBack;
#elif (JOURNALTYP == 2)
 HENHMETAFILE   hmf, hmf1;
 HDC            hTCSNewMetaFileDC;
 ENHMETAHEADER  emh ;
 DWORD          ErrorCode;
 LPVOID         lpMsgBuf;
#elif (JOURNALTYP == 3)
 struct xJournalEntry_typ    *xJournalEntry;
 FILE           *fHandle;
#endif

    FilNam[0] = (FTNCHAR) 0;
    OldFilNam[0] = (FTNCHAR) 0;
    do {     /* Suche erstes nicht existierendes File */
     _tcscpy(OldFilNam, FilNam);
     sprintf( FilNam, szTCSHardcopyFile, iHardcopyCount++ );
    } while ( (OpenFile (FilNam, &ReOpenBuf, OF_EXIST) != HFILE_ERROR) &&
              (_tcsicmp (FilNam,OldFilNam) > 0 )                          );

    if (_tcsicmp (FilNam,OldFilNam) <= 0 ) { /* kein Filename vorhanden */
     #ifndef __cplusplus
      iErr= WRN_HDCFILOPN;
      TCSGraphicError (iErr,"");
     #else
      ftnstrg.addr= szTCSErrorMsg[WRN_HDCFILOPN];
      ftnstrg.len= _tcslen (szTCSErrorMsg[WRN_HDCFILOPN]);
      TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
      TCSdrWIN__ bell ();
     #endif
     return;                                /* Error during Open -> ret */
    }

    #ifndef __cplusplus
     iErr= MSG_HDCACT;
     TCSGraphicError (iErr,FilNam);
    #else
     sprintf( MessageBuf, szTCSErrorMsg[MSG_HDCACT], FilNam );
     ftnstrg.addr= MessageBuf;
     ftnstrg.len= _tcslen (MessageBuf);
     TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
    #endif

#if (JOURNALTYP ==1)
    hTCSNewMetaFileDC  = CreateMetaFile (FilNam);
    if (hTCSNewMetaFileDC == NULL) {
     #ifndef __cplusplus
      iErr= WRN_HDCFILOPN;
      TCSGraphicError (iErr,"");
     #else
      ftnstrg.addr= szTCSErrorMsg[WRN_HDCFILOPN];
      ftnstrg.len= _tcslen (szTCSErrorMsg[WRN_HDCFILOPN]);
      TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
      TCSdrWIN__ bell ();
     #endif
     return;                                /* Error during Open -> ret */
    }

    hmf = CloseMetaFile (hTCSMetaFileDC);       /* Metafile für WM_PAINT */

    SetWindowExtEx (hTCSNewMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSNewMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    ScaleViewportExtEx (hTCSNewMetaFileDC, 1,1,-1,1,NULL);

    hWindowRegion= CreateRectRgn(TCSrect.left, TCSrect.top, TCSrect.right,TCSrect.bottom);
    hBack= CreateSolidBrush (dwColorTable[TCSBackgroundColour]); /* rechts,oben */
    FillRgn (hTCSNewMetaFileDC, hWindowRegion, hBack);  /* nicht eingeschlossen */
    #if !defined(__WIN32__) && !defined(_WIN32)
     DeleteBrush (hBack);
     DeleteRgn (hWindowRegion);                   /* Resourcen freigeben */
    #else
     DeleteObject (hBack);
     DeleteObject (hWindowRegion);
    #endif

    PlayMetaFile (hTCSNewMetaFileDC, hmf);
    hmf1= CloseMetaFile (hTCSNewMetaFileDC);
    if (hmf1 == NULL) {
     #ifndef __cplusplus
      iErr= WRN_HDCFILWRT;
      TCSGraphicError (iErr,"");
     #else
      ftnstrg.addr= szTCSErrorMsg[WRN_HDCFILWRT];
      ftnstrg.len= _tcslen (szTCSErrorMsg[WRN_HDCFILWRT]);
      TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
      TCSdrWIN__ bell ();
     #endif
     return;                                /* Error during Write -> ret */
    } else {
     DeleteMetaFile (hmf1); /* Freigabe Resourcen, nicht Löschen des Files! */
    }

    hTCSNewMetaFileDC  = CreateMetaFile (NULL); /* 16bit Windows Metafile */
    PlayMetaFile (hTCSNewMetaFileDC, hmf);      /* für neues Journalfile */
    DeleteMetaFile (hmf);                       /* alter Status Bildschirm */
    hTCSMetaFileDC = hTCSNewMetaFileDC;         /* bereit Weiterzeichnen */

#elif (JOURNALTYP == 2)
    hmf = CloseEnhMetaFile (hTCSMetaFileDC);    /* Metafile für WM_PAINT */
    hmf1  = CopyEnhMetaFile (hmf, FilNam);
    if (hmf1 == NULL) {
     ErrorCode= GetLastError(); // immer win32 bei emf
//   if (ErrorCode == ERROR_CLASS_ALREADY_EXISTS) {
//    Hier bei Bedarf Fehlerbehandlung einführen
//   } else {
      FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         ErrorCode,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
      );
      MessageBox (NULL, lpMsgBuf, szTCSWindowName, MB_ICONSTOP);
      LocalFree( lpMsgBuf ); // Free the buffer
//   } // Ende der Fehlerbehandlung
     #ifndef __cplusplus
      iErr= WRN_HDCFILOPN;
      TCSGraphicError (iErr,"");
     #else
      ftnstrg.addr= szTCSErrorMsg[WRN_HDCFILOPN];
      ftnstrg.len= _tcslen (szTCSErrorMsg[WRN_HDCFILOPN]);
      TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
      TCSdrWIN__ bell ();
     #endif
     return;                                /* Error during Open -> ret */
    }
    DeleteEnhMetaFile (hmf1); /* Handle freigeben, File  nicht geloescht! */

    GetEnhMetaFileHeader (hmf, sizeof (emh), &emh) ;
    hTCSNewMetaFileDC  = CreateEnhMetaFile (hTCSWindowDC, NULL, &emh.rclFrame,
                         _T("TCS for Windows\0Subroutine HardCopy\0"));
    SetMapMode (hTCSNewMetaFileDC, MM_ANISOTROPIC);
    SetViewportExtEx (hTCSNewMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSNewMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);
    SetWindowExtEx (hTCSNewMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSNewMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    PlayEnhMetaFile (hTCSNewMetaFileDC, hmf, &TCSrect); // neues Journal

    DeleteEnhMetaFile (hmf);                      // alter Status Bildschirm
    hTCSMetaFileDC = hTCSNewMetaFileDC;           // bereit zum Weiterzeichnen

    SetViewportExtEx (hTCSMetaFileDC, TCSrect.right, -TCSrect.bottom, NULL);
    SetViewportOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.top, NULL);
    SetWindowExtEx (hTCSMetaFileDC, TCSrect.right, TCSrect.bottom, NULL);
    SetWindowOrgEx (hTCSMetaFileDC, TCSrect.left, TCSrect.bottom, NULL);

    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectFont (hTCSMetaFileDC, hTCSFont);      // Aktuellen Zeichenstatus an
    #else
     SelectObject (hTCSMetaFileDC, hTCSFont);
    #endif
    SetBkMode (hTCSMetaFileDC, TRANSPARENT );   // Metafile weitergegeben !
    SetTextAlign (hTCSMetaFileDC, TA_LEFT | TA_BOTTOM | TA_UPDATECP); // CP
    SetTextColor (hTCSMetaFileDC, dwColorTable[TKTRNX.iTxtCol]);
    #if !defined(__WIN32__) && !defined(_WIN32)
     SelectPen (hTCSMetaFileDC, hTCSPen); // 16bit: Makro aus windowsx.h
    #else
     SelectObject (hTCSMetaFileDC, hTCSPen); // 32bit: GDI Standardaufruf
    #endif

#elif (JOURNALTYP == 3)
    fHandle= fopen(FilNam, "w+");
    if ( fHandle == NULL) {
     #ifndef __cplusplus
      iErr= WRN_HDCFILOPN;
      TCSGraphicError (iErr,"");
     #else
      ftnstrg.addr= szTCSErrorMsg[WRN_HDCFILOPN];
      ftnstrg.len= _tcslen (szTCSErrorMsg[WRN_HDCFILOPN]);
      TCSdrWIN__ outtext (CALLFTNSTRA(ftnstrg) CALLFTNSTRL(ftnstrg));
      TCSdrWIN__ bell ();
     #endif
     return;                                /* Error during Open -> ret */
    }

    SGLIB_DL_LIST_GET_LAST(struct xJournalEntry_typ, hTCSJournal, previous, next, xJournalEntry)

    while (xJournalEntry != NULL) {
     fprintf( fHandle, "%02i#%04i-%03i\n", xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );

#ifdef TRACE_CALLS
     switch (xJournalEntry->action) {
       case XACTION_INITT: {
        printf  ("%s § \n","Initt ");
        break;
       }
       case XACTION_ERASE: {
        printf  ("%s § \n","Erase ");
        break;
       }
       case XACTION_MOVABS: {
        printf  ("%s x:%i - y: %i § \n","MovAbs ", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_DRWABS: {
        printf  ("%s x:%i - y: %i § \n","DrwAbs ", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_DSHSTYLE: {
        printf  ("%s x:%i § \n","DshStyle ", xJournalEntry->i1);
        break;
       }
       case XACTION_DSHABS: {
        printf  ("%s x:%i - y: %i § \n","DshAbs ", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_PNTABS: {
        printf  ("%s x:%i - y: %i § \n","PntAbs ", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_BCKCOL: {
        printf  ("%s x:%i § \n","BckCol ", xJournalEntry->i1);
        break;
       }
       case XACTION_TXTCOL: {
        printf  ("%s x:%i § \n","TxtCol ", xJournalEntry->i1);
        break;
       }
       case XACTION_LINCOL: {
        printf  ("%s x:%i § \n","LinCol ", xJournalEntry->i1);
        break;
       }
       case XACTION_FONTATTR: {
        printf  ("%s x:%i - %i § \n","Fontattr ", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_GTEXT: {
        printf  ("%s iL:%i - C0: %i [ %c ] § \n","GText ", xJournalEntry->i1, xJournalEntry->i2,
                xJournalEntry->i2);
        break;
       }
       case XACTION_ASCII: {
        printf  ("%s C1:%i - C2: %i [ %c %c ] § \n","ASCII ", xJournalEntry->i1, xJournalEntry->i2,
                            xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       default: {
        printf ("??? %i ??? \n", xJournalEntry->action) ;
        break;
       }
     }
#endif // TRACE_CALLS
     xJournalEntry= xJournalEntry -> previous;
    }
    fclose (fHandle);
#endif // Journaltyp=3
    ShowWindow (hTCSstatWindow, SW_HIDE);
    return;
}



/*
---- subroutine LIB_MOVC3 fuer Watcom- und GNU-Compiler ----------------
Hier nicht benoetigt, nur wg. Kompatibilitaet zur DOS-Version enthalten
*/


extern void TCSdrWIN__ lib_movc3 (FTNINT *len,FTNSTRPAR *sou,FTNSTRPAR *dst
                                FTNSTRPAR_TAIL(sou)  FTNSTRPAR_TAIL(dst) )

{
int n;
    if (FTNSTRPARA(dst) <= FTNSTRPARA(sou) ) {
     for (n=0; n<*len; n++) FTNSTRPARA(dst)[n]= FTNSTRPARA(sou)[n];
    } else {
     for (n= (*len)-1; n>=0; n--) FTNSTRPARA(dst)[n]= FTNSTRPARA(sou)[n];
    };
}
