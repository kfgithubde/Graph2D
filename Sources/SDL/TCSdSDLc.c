/** ****************************************************************************
\file       TCSdSDLc.c
\brief      SDL Port: Low-Level Driver
\version    1.5
\author     (C) 2023 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Systemnahe Graphikroutinen für die Tektronix Emulation
\note \verbatim
          1. Falls der erste Buchstabe des Fensternamens ein '~' ist, wird
             das betreffende Fenster ohne Titel und Rahmen gezeichnet.
          2. Die System- und Statusmeldungen erfolgen in einem eigenen
		     einzeiligem Fenster. Falls die Statusfensterhöhe <= 0 ist,
			 erfolgen nur noch Systemfehlermeldungen über den Error-Channel.
          3. Der Videotreiber des Raspberry Pi4 kann über SSH keine zwei
             unabhängige Renderer für die beiden Fenster verwalten. Jedoch
             liefert der zweite Aufruf von SDL_CreateRenderer für das
             Statusfenster keinen Errorcode, sondern führt zu einem Programm-
             absturz. Entweder MUSS hier die Statusfensterhöhe <= 0 gesetzt
             oder X11 gestartet sein.
          4. Durch den Parameter HIGHQUALCHAR erfolgt die Textausgabe "Blended".
             Zur Performancesteigerung kann bei leistungsschwachen Systemen
             durch Auskommentieren auf "Solid" gewechselt werden.
\endverbatim
\~english
         system-specific subroutines of the Tektronix emulation
\note \verbatim
          1. If the first letter of the window name is '~', the window will be
             drawn without title and frame.
          2. System- and status messages are shown in an one-line window. If
             the height of the window is <= 0, only system errors are signaled
             through the error channel.
          3. When called inside a ssh terminal, the Raspberry Pi videodriver
             crashes during the second call of SDL_renderer . If the height of
             the status window is 0, no problem arises.
          4. If the parameter HIGHQUALCHAR is defined, textoutput is "Blended".
             Undefining HIGHQUALCHAR on slow systems changes output to "Solid".
\endverbatim
\~
***************************************************************************** */

/*
        Anmerkungen:
		  1. In der Routine WINLBL werden die SDL-Funktion SDL_GetBasePath ()
		     sowie SDL_free verwendet. In der Dokumentation ist jedoch nicht
		     explizit beschrieben, dass diese Funktion immer (wie SDL_logxxx)
		     bereits vor dem Aufruf von SDL_Init() funktioniert. Die in der
		     Source herauskommentierten Zeilen
             SDL_Init (0); und SDL_Quit(); koennen dann bei Problemen wieder
             verwendet werden.
          2. Skalierung vom Tektronix- auf das Bildschirmkoordinatensystem muss
             von Hand erfolgen, da SDL_RenderSetLogicalSize nicht durchgängig
             implementiert ist (Bug bis SDL2 Version 2.0.5 verifiziert).
             Insbesondere verwendet DrawLine die Skalierung nicht bei geneigten
             Geraden.
          3. Journalfile wird verwendet um Hardcopies erzeugen zu können

*/


/*
------------------- Konfiguration des Zielystems -------------------------------
*/

#define INIFILEXT ".xml"
#define FNTFILEXT ".ttf"
#define AUDIOSUPPORT
#define HIGHQUALCHAR


/*
------------------- Debug Switches ---------------------------------------------
*/

#define LOGLEVEL   SDL_LOG_PRIORITY_ERROR
// #define LOGLEVEL   SDL_LOG_PRIORITY_DEBUG
// #define LOGLEVEL   SDL_LOG_PRIORITY_VERBOSE // Ausgaben < Error in Fehlerkanal
// #define TRACE_CALLS // zusaetzliche Debugausgaben


/*
------------------- Headerfiles ------------------------------------------------
*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h> // Fuer HDCOPY: sprintf

#ifdef AUDIOSUPPORT
 #include <math.h>
#endif

#include "SDL.h"
#include "SDL_ttf.h"

#ifdef AUDIOSUPPORT
 #include "SDL_audio.h"
#endif

#include "mxml.h"

#include "sglib.h"

#include "TCSdSDLc.h"
#include "TKTRNX.h"


/*
------------------- Globale Variablen ------------------------------------------
*/

static int      TCSEventFilterData; // Userdata, z.Zt. nicht verwendet

static  float   PixFacX, PixFacY; // Anpassung Bildschirmauflösung

static  bool    TCSinitialized = false,
                ClippingNotActive = true;

static char     szTCSWindowName[TCS_WINDOW_NAMELEN] = TCS_WINDOW_NAME,
                szTCSstatWindowName[TCS_WINDOW_NAMELEN] = TCS_STATWINDOW_NAME,
                szTCSIniFile[TCS_FILE_NAMELEN] = "",
                szTCSHardcopyFile[TCS_FILE_NAMELEN] = TCS_HDCFILE_NAME,
                szTCSGraphicFont[TCS_FILE_NAMELEN] = TCS_INIDEF_FONT,
                szTCSSysFont[TCS_FILE_NAMELEN] = TCS_INIDEF_SYSFONT,
                szTCSsect0[TCS_FILE_NAMELEN] = TCS_INISECT0;

static  int     TCSwindowIniXrelpos = TCS_INIDEF_WINPOSX, // rel. Bildschirmpos.
                TCSwindowIniYrelpos = TCS_INIDEF_WINPOSY, // bei Init in %
                TCSwindowIniXrelsiz = TCS_INIDEF_WINSIZX,
                TCSwindowIniYrelsiz = TCS_INIDEF_WINSIZY,
                TCSstatWindowIniXrelpos = TCS_INIDEF_STATPOSX, // dito
                TCSstatWindowIniYrelpos = TCS_INIDEF_STATPOSY, // Statusfenster
                TCSstatWindowIniXrelsiz = TCS_INIDEF_STATSIZX,
                TCSstatWindowIniYrelsiz = TCS_INIDEF_STATSIZY,
                TextLineHeight,
                TCSDefaultLinCol = TCS_INIDEF_LINCOL,
                TCSDefaultTxtCol = TCS_INIDEF_TXTCOL,
                TCSDefaultBckCol = TCS_INIDEF_BCKCOL,
                iHardcopyCount = 1;  // Zähler zur Erzeugung Filenamen



/*
  Zuordnung Fehlernummern zu Meldungen
*/

typedef char   ErrMsg[TCS_MESSAGELEN];
static  ErrMsg  szTCSErrorMsg[(int) MSG_MAXERRNO+1] =
                {"Element 0 unused","DOS",
                TCS_INIDEF_UNKNGRAPHCARD, // Errno 2
                TCS_INIDEF_NOFNTFIL,      // Errno 3
                TCS_INIDEF_NOFNT,         // Errno 4
                "DOS",
                TCS_INIDEF_HDCOPN,        // Errno 6
                TCS_INIDEF_HDCWRT,        // Errno 7
                TCS_INIDEF_HDCINT,        // Errno 8
                TCS_INIDEF_USR,           // Errno 9
                TCS_INIDEF_HDCACT,        // Errno 10
                TCS_INIDEF_USRWRN,        // Errno 11
                TCS_INIDEF_EXIT,          // Errno 12
                "Windows",
                "Windows",
                TCS_INIDEF_JOUCREATE,     // Errno 15
                TCS_INIDEF_JOUENTRY,      // Errno 16
                TCS_INIDEF_JOUADD,        // Errno 17
                TCS_INIDEF_JOUCLR,        // Errno 18
                TCS_INIDEF_JOUUNKWN,      // Errno 19
                TCS_INIDEF_XMLPARSER,     // Errno 20
                TCS_INIDEF_XMLOPEN,       // Errno 21
                TCS_INIDEF_UNKNAUDIO,     // Errno 22
                TCS_INIDEF_USR2,          // Errno 23
                TCS_INIDEF_INI2,          // Errno 24
                "Maxerr only for internal Use" };

static  int     TCSErrorLev[(int) MSG_MAXERRNO+1] =
                {10,10,
                TCS_INIDEF_UNKNGRAPHCARDL,// Errno 2
                TCS_INIDEF_NOFNTFILL,     // Errno 3
                TCS_INIDEF_NOFNTL,        // Errno 4
				10,
                TCS_INIDEF_HDCOPNL,       // Errno 6
                TCS_INIDEF_HDCWRTL,       // Errno 7
                TCS_INIDEF_HDCINTL,       // Errno 8
                TCS_INIDEF_USRL,          // Errno 9
                TCS_INIDEF_HDCACTL,       // Errno 10
                TCS_INIDEF_USRWRNL,       // Errno 11
                TCS_INIDEF_EXITL,         // Errno 12
                10,
                10,
                TCS_INIDEF_JOUCREATEL,    // Errno 15
                TCS_INIDEF_JOUENTRYL,     // Errno 16
                TCS_INIDEF_JOUADDL,       // Errno 17
                TCS_INIDEF_JOUCLRL,       // Errno 18
                TCS_INIDEF_JOUUNKWNL,     // Errno 19
                TCS_INIDEF_XMLPARSERL,    // Errno 20
                TCS_INIDEF_XMLOPENL,      // Errno 21
                TCS_INIDEF_UNKNAUDIOL,    // Errno 22
                TCS_INIDEF_USR2L,         // Errno 23
                TCS_INIDEF_INI2L,         // Errno 24
                10};



/*
  Zuordnung der Farbennummern zur VGA-Palette
*/

static  SDL_Color sdlColorTable[] = {
                {240,240,240,SDL_ALPHA_OPAQUE }, /* iCol= 00: weiss (DOS: 01) */
                {  0,  0,  0,SDL_ALPHA_OPAQUE }, /* iCol= 01: schwarz(DOS:00) */
                {240, 80, 80,SDL_ALPHA_OPAQUE }, /* iCol= 02: rot             */
                { 80,240, 80,SDL_ALPHA_OPAQUE }, /* iCol= 03: gruen           */
                { 80,240,240,SDL_ALPHA_OPAQUE }, /* iCol= 04: blau            */
                { 80, 80,240,SDL_ALPHA_OPAQUE }, /* iCol= 05: lila            */
                {240,240, 80,SDL_ALPHA_OPAQUE }, /* iCol= 06: gelb            */
                {160,160,160,SDL_ALPHA_OPAQUE }, /* iCol= 07: grau            */
                {240, 80,240,SDL_ALPHA_OPAQUE }, /* iCol= 08: violett         */
                {160,  0,  0,SDL_ALPHA_OPAQUE }, /* iCol= 09: mattrot         */
                {  0,160,  0,SDL_ALPHA_OPAQUE }, /* iCol= 10: mattgruen       */
                {  0,  0,160,SDL_ALPHA_OPAQUE }, /* iCol= 11: mattblau        */
                {  0,160,160,SDL_ALPHA_OPAQUE }, /* iCol= 12: mattlila        */
                {160, 80,  0,SDL_ALPHA_OPAQUE }, /* iCol= 13: orange          */
                { 80, 80, 80,SDL_ALPHA_OPAQUE }, /* iCol= 14: mattgrau        */
                {160,  0,160,SDL_ALPHA_OPAQUE }  /* iCol= 15: mattviolett     */
             };
#define MAX_COLOR_INDEX 15


static	SDL_Window *TCSwindow = NULL;
static 	SDL_Renderer *TCSrenderer = NULL;
static  TTF_Font* TCSfont = NULL;
static  TTF_Font* TCSstatusfont = NULL;

static	SDL_Window *TCSstatwindow = NULL;
static 	SDL_Renderer *TCSstatrenderer = NULL;

struct xJournalEntry_typ {struct xJournalEntry_typ * previous;
                          struct xJournalEntry_typ * next;
                          FTNINT action; FTNINT i1; FTNINT i2;};
 static struct xJournalEntry_typ* xTCSJournal = NULL;

#ifdef AUDIOSUPPORT
 static SDL_AudioSpec      SDL_AudioDev_optained;
 static SDL_AudioSpec      SDL_AudioDev_wanted;

 static int                AudioSample_nr = 0;
#endif





// ---------------- interne Unterprogramme -------------------------------------


/* --- Anpassung der Zeichenaufloesung an die Bildschirme --- */

int HiResX(FTNINT iX)
{
    return (PixFacX*iX) +0.25f;
}


int HiResY(FTNINT iY)
{
    return (PixFacY*iY)+0.25f;
}


int LoResX(FTNINT iX)
{
    return (int)( ( (float)iX/PixFacX) +0.25f );
}


int LoResY(FTNINT iY)
{
    return (int)( ((float)iY/PixFacY)+0.25f );
}



/* --- Clippingroutinen --- */

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

/* Zeichnen einer gestrichelten Linie in den Backbuffer */

void DrawHiResDashLine (FTNINT ix,FTNINT iy, FTNINT ix2,FTNINT iy2,FTNINT *iMask)
{
FTNINT ixx,iyy, ixx2,iyy2;
float xx,yy, dx,dy, dLin,dBlank;

    if (*iMask <= 0) {
     dLin= 10., dBlank=0.; // solid
    } else if (*iMask == 1) {
     dLin= 1.; dBlank=1.; // dotted
    } else if (*iMask == 2) {
     dLin= 3.; dBlank=1.; //  substitute dashed-dotted
    } else if (*iMask == 3) {
     dLin= 3.; dBlank=3.; //  dashed
    } else {
     dLin= 3., dBlank=3.; // unrecognized -> dashed
    }

    if (abs(ix2-ix) >= abs(iy2-iy)) {
     dx= ix2 >= ix ?  3. : -3.;
     dy= ((float)(iy2-iy))/((float)(ix2-ix))*dx;

     xx= (float)ix; yy= (float)iy;
     while (dx != 0.) {
      ixx= (FTNINT) xx; iyy= (FTNINT) yy;
      ixx2=(FTNINT) (xx+dLin*dx); iyy2=(FTNINT) (yy+dLin*dy);
      xx+= (dLin+dBlank)*dx; yy+= (dLin+dBlank)*dy;
      if (   ((dx>=0.) && ((FTNINT)xx>=ix2) )
          || ((dx<=0.) && ((FTNINT)xx<=ix2) )   ) {
       ixx2= ix2; iyy2= iy2;
       dx= 0.;
      }
      SDL_RenderDrawLine(TCSrenderer, HiResX(ixx),HiResY(TEK_YMAX-iyy),
                                      HiResX(ixx2),HiResY(TEK_YMAX-iyy2));
     }

    } else {
     dy= iy2 >= iy ?  3. : -3.;
     dx= ((float)(ix2-ix))/((float)(iy2-iy))*dy;

     xx= (float)ix; yy= (float)iy;
     while (dy != 0.) {
      ixx= (FTNINT) xx; iyy= (FTNINT) yy;
      ixx2=(FTNINT) (xx+dLin*dx); iyy2=(FTNINT) (yy+dLin*dy);
      xx+= (dLin+dBlank)*dx; yy+= (dLin+dBlank)*dy;
      if (   ((dy>=0.) && ((FTNINT)yy>=iy2) )
          || ((dy<=0.) && ((FTNINT)yy<=iy2) )   ) {
       ixx2= ix2; iyy2= iy2;
       dy= 0.;
      }
      SDL_RenderDrawLine(TCSrenderer, HiResX(ixx), HiResY(TEK_YMAX-iyy),
                                      HiResX(ixx2), HiResY(TEK_YMAX-iyy2));
     }
    }
}



void PlotText (const char *outtxt)
{
SDL_Rect dstrect;
SDL_Surface* surface;
SDL_Texture* texture;

#ifdef HIGHQUALCHAR
    surface = TTF_RenderUTF8_Blended(TCSfont, outtxt, sdlColorTable[TKTRNX.iTxtCol]);
#else
    surface = TTF_RenderUTF8_Solid(TCSfont, outtxt, sdlColorTable[TKTRNX.iTxtCol]);
#endif
    texture = SDL_CreateTextureFromSurface(TCSrenderer, surface);

    SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
    dstrect.x= HiResX(TKTRNX.kBeamX);
    dstrect.y= HiResY(TEK_YMAX-TKTRNX.kBeamY)-dstrect.h;

    SDL_RenderCopy(TCSrenderer, texture, NULL, &dstrect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    TKTRNX.kBeamX= TKTRNX.kBeamX + LoResX(dstrect.w);
}



void RepaintBuffer () // Hier nicht GraphicError verwenden(Rekursionsschleifen)!
{
FTNINT DashStyle;
int wx, wz, iStringLen, iStringActual;
char szString [TCS_MESSAGELEN+1];
struct xJournalEntry_typ *xJournalEntry;

#ifdef TRACE_CALLS
    SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> called");
#endif

    DashStyle= 0; // Vorbesetzung nur notwendig bei fehlerhaftem Journal
    iStringActual= 0; // Zahler Einlesen String ueber XACTION_ASCII
    SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iBckCol].r
                                      , sdlColorTable[TKTRNX.iBckCol].g
                                      , sdlColorTable[TKTRNX.iBckCol].b
                                      , sdlColorTable[TKTRNX.iBckCol].a);
	SDL_RenderClear (TCSrenderer); // Backbuffer nach RenderPresent undefiniert

 #ifdef TRACE_CALLS
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> xTCSJournal: Ptr= %p", xTCSJournal);
 #endif
    SGLIB_DL_LIST_GET_LAST(struct xJournalEntry_typ, xTCSJournal, previous, next, xJournalEntry)
    while (xJournalEntry != NULL) {
 #ifdef TRACE_CALLS
     SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> xTCSJournal: Ptr= %p", xTCSJournal);
     SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> Current Entry: Ptr= %p / previous: Ptr= %p / next: Ptr= %p",
                     xJournalEntry, xJournalEntry->previous, xJournalEntry->next);
     SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_??? = %i (i1= %i, i2= %i)",
                   xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );
 #endif
     switch (xJournalEntry->action) {
       case XACTION_INITT: {
        TKTRNX.iLinCol= TCSDefaultLinCol;
        TKTRNX.iTxtCol= TCSDefaultTxtCol;
        TKTRNX.iBckCol= TCSDefaultBckCol;

        INITT2(); // Reset TKTRNX (Margin, Scale...)

        TKTRNX.ksizef = 0; // Reset FONT
        TKTRNX.kitalc = 0;
        if (!TCSfont)TTF_CloseFont(TCSfont);
        TCSfont = TTF_OpenFont(szTCSGraphicFont,
                               HiResY(TEK_YMAX *TCS_REL_CHR_HEIGHT));
        if (!TCSfont) {
         SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_INITT Error Opening Fontfile");
        } else {
         TTF_SetFontStyle(TCSfont, TTF_STYLE_NORMAL);
         if(TTF_SizeText(TCSfont,"M",&wx,&wz)) {
          SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_INITT Fontsize?");
         } else {
          TKTRNX.khorsz= LoResX(wx);
          TKTRNX.kversz= LoResY(wz);
          TKTRNX.khomey= TEK_YMAX - TKTRNX.kversz;
         }
		}
        TKTRNX.kBeamX= TKTRNX.klmrgn; // HOME
        TKTRNX.kBeamY= TKTRNX.khomey;

       } // weiter mit Erase
       case XACTION_ERASE: {
        SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iBckCol].r
	                                      , sdlColorTable[TKTRNX.iBckCol].g
	                                      , sdlColorTable[TKTRNX.iBckCol].b
	                                      , sdlColorTable[TKTRNX.iBckCol].a);
        SDL_RenderClear (TCSrenderer);
        break; // Erase ohne Auswirkungen auf die Cursorposition!
       }
       case XACTION_MOVABS: {
        TKTRNX.kBeamX= xJournalEntry->i1;
        TKTRNX.kBeamY= xJournalEntry->i2;
        break;
       }
       case XACTION_DRWABS: {
        SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                          , sdlColorTable[TKTRNX.iLinCol].g
                                          , sdlColorTable[TKTRNX.iLinCol].b
                                          , sdlColorTable[TKTRNX.iLinCol].a );
        SDL_RenderDrawLine(TCSrenderer, HiResX(TKTRNX.kBeamX),
                                        HiResY(TEK_YMAX-TKTRNX.kBeamY),
                                        HiResX(xJournalEntry->i1),
                                        HiResY(TEK_YMAX-xJournalEntry->i2) );
        TKTRNX.kBeamX= xJournalEntry->i1;
        TKTRNX.kBeamY= xJournalEntry->i2;
        break;
       }
       case XACTION_DSHSTYLE: {
        DashStyle= xJournalEntry->i1;
        break;
       }
       case XACTION_DSHABS: {
        SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                          , sdlColorTable[TKTRNX.iLinCol].g
                                          , sdlColorTable[TKTRNX.iLinCol].b
                                          , sdlColorTable[TKTRNX.iLinCol].a );
        DrawHiResDashLine (TKTRNX.kBeamX,TKTRNX.kBeamY, xJournalEntry->i1,xJournalEntry->i2,&DashStyle);
        TKTRNX.kBeamX= xJournalEntry->i1;
        TKTRNX.kBeamY= xJournalEntry->i2;
        break;
       }
       case XACTION_PNTABS: {
        SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                          , sdlColorTable[TKTRNX.iLinCol].g
                                          , sdlColorTable[TKTRNX.iLinCol].b
                                          , sdlColorTable[TKTRNX.iLinCol].a );
        SDL_RenderDrawPoint(TCSrenderer, HiResX(xJournalEntry->i1),
                                         HiResY(TEK_YMAX-xJournalEntry->i2) );
        TKTRNX.kBeamX= xJournalEntry->i1;
        TKTRNX.kBeamY= xJournalEntry->i2;
        break;
       }
       case XACTION_BCKCOL: {
        TKTRNX.iBckCol= xJournalEntry->i1;
        break;
       }
       case XACTION_LINCOL: {
        TKTRNX.iLinCol= xJournalEntry->i1;
        break;
       }
      case XACTION_TXTCOL: {
        TKTRNX.iTxtCol= xJournalEntry->i1;
        break;
       }
        case XACTION_FONTATTR: {
        TKTRNX.kitalc= xJournalEntry->i1;
        if (TKTRNX.kitalc > 0) {
         TTF_SetFontStyle(TCSfont, TTF_STYLE_ITALIC);
        } else {
         TTF_SetFontStyle(TCSfont, TTF_STYLE_NORMAL);
        }

        if (TKTRNX.ksizef != xJournalEntry->i2) {
         TKTRNX.ksizef= xJournalEntry->i2;
         if (!TCSfont) TTF_CloseFont(TCSfont);
         TCSfont = TTF_OpenFont(szTCSGraphicFont,
                         HiResY((1+TKTRNX.ksizef)*TCS_REL_CHR_HEIGHT*TEK_YMAX));
         if (!TCSfont) {
          SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_FONTATTR");
         } else {
          if(TTF_SizeText(TCSfont,"M",&wx,&wz)) {
           SDL_LogError (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_FONTATTR Size");
          } else {
           TKTRNX.khorsz= LoResX(wx);
           TKTRNX.kversz= LoResY(wz);
           TKTRNX.khomey= TEK_YMAX - TKTRNX.kversz;
          }
	     }
        }
        break;
       }
       case XACTION_GTEXT: {
        iStringActual= 0;
        iStringLen= xJournalEntry->i1;
        if (iStringLen > TCS_MESSAGELEN) iStringLen= TCS_MESSAGELEN;
        if (iStringLen == 0) break;
        szString[iStringActual++]= xJournalEntry->i2;
        if (iStringLen == 1) {
         szString[iStringActual]= '\0';
         PlotText (szString);
        }
        break;
       }
       case XACTION_ASCII: {
        if (iStringActual < iStringLen) {
         szString[iStringActual++]= xJournalEntry->i1;
         if (iStringActual < iStringLen) szString[iStringActual++]= xJournalEntry->i2;
         if (iStringActual >= iStringLen ) {
          szString[iStringActual]= '\0';
          PlotText (szString);
         }
        }
        break;
       }
       case XACTION_NOOP: {
        break;
       }
       default: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> XACTION_XXX");
        break;
       }
     }
     xJournalEntry= xJournalEntry -> previous;
    }
 #ifdef TRACE_CALLS
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "RepaintBuffer> xTCSJournal: Ptr= %p / Last Entry: Ptr= %p", xTCSJournal, xJournalEntry);
 #endif
}



void TCSGraphicError (int iErr, const char* msg)
{
char cBuf[TCS_MESSAGELEN];
FTNINT i; // Dummyparameter

    snprintf( cBuf, TCS_MESSAGELEN, szTCSErrorMsg[iErr], msg );
    if (!TCSinitialized) { // Vor Systeminitalisierung nur Basismeldungen
     SDL_LogError (SDL_LOG_CATEGORY_VIDEO, cBuf);
	 SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                       szTCSstatWindowName, cBuf, TCSwindow);
    } else { // ab jetzt mit bell, outtext...
     SDL_RenderPresent (TCSrenderer);
     RepaintBuffer ();
	 if (TCSErrorLev[iErr] > 0) {
      bell ();
      outtext (cBuf, strlen (cBuf) );
      if (TCSErrorLev[iErr] == 2) {
       SDL_LogInfo (SDL_LOG_CATEGORY_VIDEO, cBuf);
	  }
	  if (TCSErrorLev[iErr] == 3) {
       SDL_LogError (SDL_LOG_CATEGORY_VIDEO, cBuf);
	  } else if (TCSErrorLev[iErr] < 10) {
       SDL_LogWarn (SDL_LOG_CATEGORY_VIDEO, cBuf);
       if (TCSErrorLev[iErr] == 5) {
        dcursr (&i,&i,&i); // Press Any Key
       } else if (TCSErrorLev[iErr]==8) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                         szTCSstatWindowName, cBuf, TCSwindow);
       }
	  } else {
       if (TCSErrorLev[iErr] == 10) {
        dcursr (&i,&i,&i); // Press Any Key
       }
       if (TCSErrorLev[iErr] == 12) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR,
                         szTCSstatWindowName, cBuf, TCSwindow);
       }
       if (iErr != ERR_EXIT) { // Error-Level von finitt durch XML veraenderbar
        SDL_LogError (SDL_LOG_CATEGORY_VIDEO, cBuf);
        finitt ();                  // Erzwungenes Beenden durch finitt
       }
      }
     }
    }
}





/* Eventhandler zum Fensterhandling */

int TCSEventFilter(void* UserData, SDL_Event* event)
{
SDL_Point winsiz;

    if (event->type == SDL_WINDOWEVENT) {
     switch (event->window.event) {
      case SDL_WINDOWEVENT_RESIZED:
      case SDL_WINDOWEVENT_MAXIMIZED:
      case SDL_WINDOWEVENT_RESTORED:
       if (event->window.windowID == SDL_GetWindowID(TCSwindow)) {
        if (SDL_GetRendererOutputSize(TCSrenderer, &winsiz.x, &winsiz.y) != 0) {
         TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
        } else {
         PixFacX= (float)(winsiz.x) / (float) TEK_XMAX;
         PixFacY= (float)(winsiz.y) / (float) TEK_YMAX;
         SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "WINSIZ> PixFac: x= %f, y= %f", PixFacX, PixFacY);
        }
       }
      case SDL_WINDOWEVENT_EXPOSED:
       if (event->window.windowID == SDL_GetWindowID(TCSwindow)) {
        SDL_RenderPresent (TCSrenderer);
        RepaintBuffer ();
       } else { if (event->window.windowID == SDL_GetWindowID(TCSstatwindow)) {
        SDL_RenderPresent (TCSstatrenderer);
       } }
       break;
      default:
       break;
     }
    }
    return 1;
}



#ifdef AUDIOSUPPORT
 void audio_callback(void *sample_nr, Uint8 *raw_buffer, int bytes)
 {
 int i, length;
 float time, value;
 Sint16* buffer;
 SDL_AudioCVT cvt;

    buffer= (Sint16*) raw_buffer;
    length = 8*bytes /SDL_AUDIO_BITSIZE(SDL_AudioDev_optained.format) / SDL_AudioDev_optained.channels; // Bytes = Variablenlänge (Bit/8) pro Kanal
    for(i=0; i < length; i++, *((int*)sample_nr)=*((int*)sample_nr)+1 ) {
     time = ((float)( *((int*)sample_nr)) / SAMPLE_RATE);
     value= BELL_AMPLITUDE * sin(2.0f * M_PI * BELL_FREQUENCY * time);
     buffer[i] = (Sint16)(value);
    }
    SDL_BuildAudioCVT(&cvt, AUDIO_S16SYS, 1, SAMPLE_RATE, SDL_AudioDev_optained.format, SDL_AudioDev_optained.channels, SDL_AudioDev_optained.freq);
    cvt.len = length*2;  // Sint16 = 2 Bytes
    cvt.buf = raw_buffer;
    SDL_ConvertAudio(&cvt); // Konvertiere in das Deviceformat
#ifdef TRACE_CALLS
    SDL_LogVerbose (SDL_LOG_CATEGORY_AUDIO, "audio_callback>> Number of Samples= %d Bytes allocated= %d ", length,bytes);
    SDL_LogVerbose (SDL_LOG_CATEGORY_AUDIO, "audio_callback>> Bytes 16bit Audio= %d Bytes needed= %d", cvt.len,cvt.len_cvt);
#endif
 }
#endif



/* Eventhandler zum Parsen von XML-Dateien */


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
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCNAM) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSHardcopyFile);
        }
        break;
       }

       case 2: { // Section = Layout
        if ((strcmp(mxmlGetElement(node),TCS_INIVAR_FONT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSGraphicFont);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_SYSFONT) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSSysFont);

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
        if ((strcmp(mxmlGetElement(node),TCS_INIVAR_UNKNGRAPHCARD) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_UNKNGRAPHCARD]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_UNKNGRAPHCARDL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_UNKNGRAPHCARD]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_NOFNTFIL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_NOFNTFIL]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_NOFNTFILL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_NOFNTFIL]);

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_HDCOPN) == 0)  ) {
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

        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_UNKNAUDIO) == 0)  ) {
         mxmlElementSetAttr (node,"typ","opaque");
         mxmlElementSetAttrf(node,"store","%p",&szTCSErrorMsg[ERR_UNKNAUDIO]);
        } else if ((strcmp(mxmlGetElement(node),TCS_INIVAR_UNKNAUDIOL) == 0)  ) {
         mxmlElementSetAttr (node,"typ","integer");
         mxmlElementSetAttrf(node,"store","%p",&TCSErrorLev[ERR_UNKNAUDIO]);

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


mxml_type_t	 sax_type_callback(mxml_node_t  *node)
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


void sax_error_callback (char *mssg)
{
    TCSGraphicError (ERR_XMLPARSER, mssg);
    return;
}



/*
------------------- Userroutinen: Initialisierung ------------------------------
*/


void XMLreadProgPar (const char * filname)
{
int ParserState;
FILE *fp;
mxml_node_t *tree;

    if (filname[0] != '\0') {
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
}


/*
Setzen der Defaultwerte vor dem Einlesen der Initialisierungsdaten
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
char        szTmpString[TCS_FILE_NAMELEN], szTmpString1[TCS_FILE_NAMELEN];
FTNSTRDESC  ftn_WorkString, o, n;

    ftn_WorkString.len= TCS_FILE_NAMELEN; // Ersatz %: durch Programmverzeichnis
    ftn_WorkString.addr= szTCSGraphicFont;
    n.addr= SDL_GetBasePath(); // Neuer Substring = Directory
    n.len= strlen(n.addr);
    o.addr= PROGDIRTOKEN; // Alter Substring
    o.len= strlen (o.addr);
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
    strncpy(szTCSGraphicFont, ftn_WorkString.addr, TCS_FILE_NAMELEN);

    ftn_WorkString.addr= szTCSSysFont;
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
    strncpy(szTCSSysFont, ftn_WorkString.addr, TCS_FILE_NAMELEN);

	SDL_free (n.addr); // SDL_BasePath nicht mehr benoetigt

	n.addr= FNTFILEXT; // "Ersatz .% durch .TTF oder kein Punkt durch .TTF
	n.len= strlen(n.addr);
    o.addr= INIFILEXTTOKEN; // Alter Substring
    o.len= strlen (o.addr);
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
    strncpy(szTCSSysFont, ftn_WorkString.addr, TCS_FILE_NAMELEN);
    if (strchr(szTCSSysFont,'.') == 0) {
        strncat (szTCSSysFont, n.addr, TCS_FILE_NAMELEN-n.len);
    }

    ftn_WorkString.addr= szTCSGraphicFont;
    SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                CALLFTNSTRL(ftn_WorkString)
                CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
    strncpy(szTCSGraphicFont, ftn_WorkString.addr, TCS_FILE_NAMELEN);
    if (strchr(szTCSGraphicFont,'.') == 0) {
        strncat (szTCSGraphicFont, n.addr, TCS_FILE_NAMELEN-n.len);
    }
}


extern void winlbl (FTNSTRPAR * PloWinNam, FTNSTRPAR * StatWinNam,
                                            FTNSTRPAR *IniFilNam
                                            FTNSTRPAR_TAIL(PloWinNam)
                                            FTNSTRPAR_TAIL(StatWinNam)
                                            FTNSTRPAR_TAIL(IniFilNam)      )

{
// Absicherung der Definition der Programmparameter
#if (TCS_WINDOW_NAMELEN <= TCS_FILE_NAMELEN)
 #define TMPSTRLEN TCS_FILE_NAMELEN
#else
 #define TMPSTRLEN TCS_WINDOW_NAMELEN
#endif

int         i;
FTNINT      iL;
char        szTmpString[TMPSTRLEN], szTmpString1[TCS_FILE_NAMELEN];
char *      iAt;
FTNSTRDESC  ftn_WorkString, o, n;

    iL= FTNSTRPARL(PloWinNam);                       // Name des Grahikfensters
    if (iL > (TMPSTRLEN-1)) iL= TMPSTRLEN-1;
    strncpy(szTmpString, FTNSTRPARA(PloWinNam),iL);
    szTmpString[iL]= '\0'; // Fortranstring evtl. ohne \0
    iL= strlen (szTmpString);
    if (iL > (TCS_WINDOW_NAMELEN-1)) iL= TCS_WINDOW_NAMELEN-1;
    if (iL > 0) {
     strncpy( szTCSWindowName, szTmpString, iL);
     szTCSWindowName[iL]= '\0';
    }

    iL= FTNSTRPARL(StatWinNam);                      // Name des Statusfensters
    if (iL > (TMPSTRLEN-1)) iL= TMPSTRLEN-1;
    strncpy(szTmpString, FTNSTRPARA(StatWinNam), iL);
    szTmpString[iL]= '\0'; // Fortranstring evtl. ohne \0
    iL= strlen (szTmpString);
    if (iL > (TCS_WINDOW_NAMELEN-1)) iL= TCS_WINDOW_NAMELEN-1;
    if (iL > 0) {
     strncpy( szTCSstatWindowName, szTmpString, iL);
     szTCSstatWindowName[iL]= '\0';
    }

    iL= FTNSTRPARL(IniFilNam);                // Name der Initialisierungsdatei
    if (iL > (TMPSTRLEN-1)) iL= TMPSTRLEN-1;
    strncpy(szTmpString, FTNSTRPARA(IniFilNam), iL);
    szTmpString[iL]= '\0'; // Fortranstring evtl. ohne \0

    iL= strlen(szTmpString);
    if (iL > (TCS_FILE_NAMELEN-1)) iL= TCS_FILE_NAMELEN-1;
    if (iL > 0) {
     strncpy( szTCSIniFile, szTmpString, iL);
     szTCSIniFile[iL]= '\0';

     iAt= strstr (szTCSIniFile, "@"); // Section Level0?
     if (iAt != 0) {
      strncpy (szTCSsect0, &iAt[1], iL);
      iAt[0]= '\0'; // Abschneiden von @Section0 in szTCSIniFile
     }

     ftn_WorkString.len= TCS_FILE_NAMELEN;
     ftn_WorkString.addr= szTCSIniFile;

     n.addr= SDL_GetBasePath(); // Neuer Substring = Directory
     n.len= strlen(n.addr);
     o.addr= PROGDIRTOKEN; // Alter Substring
     o.len= strlen (o.addr);
     SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                 CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                 CALLFTNSTRL(ftn_WorkString)
                 CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
     SDL_free (n.addr);

     n.addr= INIFILEXT; // Neuer Substring = Default Extension
     n.len= strlen (INIFILEXT);
     o.addr= INIFILEXTTOKEN; // Alter Substring
     o.len= strlen (o.addr);
     SUBSTITUTE( CALLFTNSTRA(ftn_WorkString),
                 CALLFTNSTRA(ftn_WorkString), CALLFTNSTRA(o), CALLFTNSTRA(n)
                 CALLFTNSTRL(ftn_WorkString)
                 CALLFTNSTRL(ftn_WorkString) CALLFTNSTRL(o) CALLFTNSTRL(n) );
     strncpy(szTCSIniFile, ftn_WorkString.addr, TCS_FILE_NAMELEN);
    }

#ifdef TRACE_CALLS
    SDL_LogSetAllPriority(LOGLEVEL); // Ausgabe in Fehlerkanal vor INIT moeglich
    SDL_LogDebug (SDL_LOG_CATEGORY_SYSTEM,
             "WINLBL> Setting Windowname >%s< Statusname >%s< Inifile >%s<\n\r",
                            szTCSWindowName, szTCSstatWindowName, szTCSIniFile);
#endif

// Absicherung TMPSTRLEN nicht mehr benoetigt
#undef TMPSTRLEN
}



extern void initt1 ()
{
int iD;
Uint32 flags;
SDL_Point winsiz;
SDL_Rect rect;

struct xJournalEntry_typ * xJournalEntry;


    if (TCSinitialized) return;   /* Bereits initialisiert */

    SDL_LogSetAllPriority(LOGLEVEL); // Ausgabe in Fehlerkanal bereits moeglich

    PresetProgPar(); // Compilerinitialisierung nach finitt() wiederherstellen

    /*
        Falls Extension des Ini-Files .XML: XML-Parser -> hier immer XML
    */
#if defined(XMLSUPPORT)
    XMLreadProgPar (szTCSIniFile);
#endif

    CustomizeProgPar (); // Ersatz %: durch Programmverzeichnis

     /*
     Übernahme der durch den Nutzer angepassten Initialisierungsdaten
     */

    TKTRNX.iLinCol= TCSDefaultLinCol;
    TKTRNX.iTxtCol= TCSDefaultTxtCol;
    TKTRNX.iBckCol= TCSDefaultBckCol;

    /*
        Initialisierung des SDL2-Systems
    */

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    }
	if (TTF_Init() != 0) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    }
#ifdef AUDIOSUPPORT
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
     TCSGraphicError (ERR_UNKNAUDIO, SDL_GetError());
    }
#endif

    /*
        Ermittlung allgemeiner systemspezifischer Parameter
    */

    iD= SDL_GetNumVideoDisplays();
    if (iD <= 0) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    } else {
     SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> SDL_GetNumVideoDisplays = %i", iD);
    }

    iD= iD-1;
    if (SDL_GetDisplayUsableBounds(iD, &rect) != 0) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    } else {
     SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> UsableDisplayBounds: x= %i, y= %i, w= %i, h= %i", rect.x,rect.y,rect.w,rect.h);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_SetEventFilter(TCSEventFilter,&TCSEventFilterData);

    /*
        Erzeugung des Graphikfensters
    */

    flags= SDL_WINDOW_RESIZABLE;
    if (szTCSWindowName[0] == '~') {
     flags= flags | SDL_WINDOW_BORDERLESS;
    }
	TCSwindow = SDL_CreateWindow(szTCSWindowName,
                             TCSwindowIniXrelpos *rect.w / 100,
                             TCSwindowIniYrelpos *rect.h / 100,
                             TCSwindowIniXrelsiz *rect.w / 100,
                             TCSwindowIniYrelsiz *rect.h / 100,
                             flags );
	TCSrenderer = SDL_CreateRenderer(TCSwindow, -1, 0);



    if (SDL_GetRendererOutputSize(TCSrenderer, &winsiz.x, &winsiz.y) != 0) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    } else {
     SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> RendererBounds: x= %i, y= %i", winsiz.x,winsiz.y);
     PixFacX= (float)(winsiz.x) / (float) TEK_XMAX;
     PixFacY= (float)(winsiz.y) / (float) TEK_YMAX;
     SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> PixFac: x= %f, y= %f", PixFacX, PixFacY);
    }

	SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iBckCol].r
	                                  , sdlColorTable[TKTRNX.iBckCol].g
	                                  , sdlColorTable[TKTRNX.iBckCol].b
	                                  , sdlColorTable[TKTRNX.iBckCol].a );
	SDL_RenderClear (TCSrenderer);
    SDL_RenderPresent (TCSrenderer);

    TCSfont = TTF_OpenFont(szTCSGraphicFont,
                  HiResY(TCS_REL_CHR_HEIGHT*TEK_YMAX));
    if (!TCSfont) {
     TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
    }         // TKTRNX wird durch INITT gesetzt

    /*
        Erzeugung des Statusfensters
    */

    if (TCSstatWindowIniYrelsiz > 0 ) {
     flags= SDL_WINDOW_RESIZABLE;
     if (szTCSstatWindowName[0] == '~') {
      flags= flags | SDL_WINDOW_BORDERLESS;
     }
	 TCSstatwindow = SDL_CreateWindow(szTCSstatWindowName,
                             TCSstatWindowIniXrelpos *rect.w / 100,
                             TCSstatWindowIniYrelpos *rect.h / 100,
                             TCSstatWindowIniXrelsiz *rect.w / 100,
                             TCSstatWindowIniYrelsiz *rect.h / 100,
                             flags);

	 TCSstatrenderer = SDL_CreateRenderer(TCSstatwindow, -1, 0);

	 SDL_SetRenderDrawColor(TCSstatrenderer, sdlColorTable[TCSDefaultBckCol].r
	                                  , sdlColorTable[TCSDefaultBckCol].g
	                                  , sdlColorTable[TCSDefaultBckCol].b
	                                  , sdlColorTable[TCSDefaultBckCol].a );
	 SDL_RenderClear (TCSstatrenderer);
	 SDL_RenderPresent (TCSstatrenderer);

     TextLineHeight= HiResY(TCS_REL_CHR_HEIGHT*TEK_YMAX);
     TCSstatusfont = TTF_OpenFont(szTCSSysFont, TextLineHeight);
     if (!TCSstatusfont) {
      TCSGraphicError (ERR_UNKNGRAPHCARD, SDL_GetError());
     }
     TKTRNX.kStCol= 1; // Nur einzeilige Ausgabe
    }

    /*
        Initialisierung des Audiosystems
    */

#ifdef AUDIOSUPPORT

    SDL_AudioDev_wanted.freq = SAMPLE_RATE;
    SDL_AudioDev_wanted.format = AUDIO_S16SYS; // 16 bit integer
    SDL_AudioDev_wanted.channels = 1; // Mono
    SDL_AudioDev_wanted.samples = 2048; // buffer-size
    SDL_AudioDev_wanted.callback = audio_callback;
    SDL_AudioDev_wanted.userdata = &AudioSample_nr; // Zaehler zur Sinusberechnung

    if(SDL_OpenAudio(&SDL_AudioDev_wanted, &SDL_AudioDev_optained) < 0) {
     TCSGraphicError (ERR_UNKNAUDIO, SDL_GetError());
    } else {
     if(SDL_AudioDev_wanted.format != SDL_AudioDev_optained.format) {
      SDL_LogInfo(SDL_LOG_CATEGORY_AUDIO, "INITT1> Failed to get the desired AudioSpec");
     }
    }
    SDL_LogDebug (SDL_LOG_CATEGORY_AUDIO, "INITT1> want.frequ= %i  want.channels= %i  want.samples= %i  want.size= %i",
                  SDL_AudioDev_wanted.freq, SDL_AudioDev_wanted.channels, SDL_AudioDev_wanted.samples, SDL_AudioDev_wanted.size);
    SDL_LogDebug (SDL_LOG_CATEGORY_AUDIO, "INITT1> optained.frequ= %i  optained.channels= %i  optained.samples= %i  optained.size= %i",
                  SDL_AudioDev_optained.freq, SDL_AudioDev_optained.channels, SDL_AudioDev_optained.samples, SDL_AudioDev_optained.size);
#endif

    /*
        Anlegen des Journals
    */

    xTCSJournal= NULL;
    SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> xTCSJournal initialisiert: Ptr= %p", xTCSJournal);

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUCREATE,"");
    SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> Nach 1. malloc: xJournalEntry: Ptr= %p", xJournalEntry);

    xJournalEntry->action=  XACTION_NOOP; // Erkennung Listenanfang: Wurzelelement ohne Funktion
    xJournalEntry->i1= 0;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
    SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> LIST_ADD=Create Journal: xTCSJournal: Ptr= %p / xJournalEntry: Ptr= %p", xTCSJournal, xJournalEntry);
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "INITT1> previous: Ptr= %p / next: Ptr= %p", xJournalEntry -> previous, xJournalEntry -> next);

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
    xJournalEntry->action=  XACTION_INITT;
    xJournalEntry->i1= 0;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
    SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "INITT1> Nach 2. LIST_ADD: xTCSJournal: Ptr= %p / xJournalEntry: Ptr= %p", xTCSJournal, xJournalEntry);
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "INITT1> previous: Ptr= %p / next: Ptr= %p", xJournalEntry -> previous, xJournalEntry -> next);

    /*
        Initialisierung erfolgreich abgeschlossen
    */

    TCSinitialized= true;

    return;
}



extern void finitt ()
{
struct xJournalEntry_typ    * xJournalEntry;

    if (!TCSinitialized) return; /* Graphiksystem nicht initialisiert */

    TCSGraphicError (ERR_EXIT,"");
    SDL_LogDebug (SDL_LOG_CATEGORY_SYSTEM, "finitt> Quit SDL");

    TCSinitialized= false;       /* Ab jetzt nicht mehr funktionsfähig */

    SGLIB_DL_LIST_MAP_ON_ELEMENTS (struct xJournalEntry_typ, xTCSJournal,
           xJournalEntry,previous,next, { free (xJournalEntry);}); // free all
    xTCSJournal= NULL;

    TTF_CloseFont(TCSfont);
    TTF_CloseFont(TCSstatusfont);

    SDL_DestroyRenderer(TCSrenderer);
    SDL_DestroyWindow(TCSwindow);

    if (TCSstatWindowIniYrelsiz > 0 ) {
     SDL_DestroyRenderer(TCSstatrenderer);
     SDL_DestroyWindow(TCSstatwindow);
    }

#ifdef AUDIOSUPPORT
    SDL_CloseAudio();
#endif

    TTF_Quit();
	SDL_Quit();

    if (TCSErrorLev[ERR_EXIT] >= 10) exit (EXIT_SUCCESS);
    return;
}



extern void iowait (void)
{
    SDL_RenderPresent (TCSrenderer);
    RepaintBuffer ();
}



/*
------------------- Userroutinen: Zeichnen -------------------------------------
*/



extern void swind1 (FTNINT *ix1,FTNINT *iy1,FTNINT *ix2,FTNINT *iy2)
{
    ClippingNotActive = (*ix1==0) && (*iy1==0) &&
                                        (*ix2==TEK_XMAX) && (*iy2==TEK_YMAX);
    /* Berechnung BOOL zur Wahrung der Programmstruktur der DOS-Version */
}



extern void erase (void)
{
struct xJournalEntry_typ    * xJournalEntry;

	SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iBckCol].r
	                                  , sdlColorTable[TKTRNX.iBckCol].g
	                                  , sdlColorTable[TKTRNX.iBckCol].b
	                                  , sdlColorTable[TKTRNX.iBckCol].a );
	SDL_RenderClear (TCSrenderer);
    SDL_RenderPresent (TCSrenderer);

     SGLIB_DL_LIST_MAP_ON_ELEMENTS (struct xJournalEntry_typ, xTCSJournal,
           xJournalEntry,previous,next, {free (xJournalEntry);}); // free all

     xTCSJournal= NULL; // create new journal
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUCLR,"");
     xJournalEntry->action=  XACTION_NOOP; // Wurzelelement ohne Vorgaenger
     xJournalEntry->i1= 0;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_LINCOL;
     xJournalEntry->i1= TKTRNX.iLinCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_TXTCOL;
     xJournalEntry->i1= TKTRNX.iTxtCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_BCKCOL;
     xJournalEntry->i1= TKTRNX.iBckCol;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ)); // New Plot
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
     xJournalEntry->action=  XACTION_ERASE;
     xJournalEntry->i1= 0;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void movabs (FTNINT *ix,FTNINT *iy)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    if (PointInWindow (*ix, *iy)) {
     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= *ix;
     xJournalEntry->i2= *iy;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
    }
}



extern void drwabs (FTNINT *ix,FTNINT *iy)
{
FTNINT iXClip, iYClip, iXClip2, iYClip2;
struct xJournalEntry_typ    * xJournalEntry;

    if (ClipLineStart(TKTRNX.kBeamX,TKTRNX.kBeamY, *ix,*iy, &iXClip,&iYClip)) {
     ClipLineStart(*ix,*iy, TKTRNX.kBeamX,TKTRNX.kBeamY, &iXClip2,&iYClip2); // geclippter Endpunkt
     SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                       , sdlColorTable[TKTRNX.iLinCol].g
                                       , sdlColorTable[TKTRNX.iLinCol].b
                                       , sdlColorTable[TKTRNX.iLinCol].a );
     SDL_RenderDrawLine(TCSrenderer, HiResX(iXClip),HiResY(TEK_YMAX-iYClip),
                                   HiResX(iXClip2),HiResY(TEK_YMAX-iYClip2));

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_DRWABS;
     xJournalEntry->i1= iXClip2;
     xJournalEntry->i2= iYClip2;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
    }
    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action=  XACTION_MOVABS;
    xJournalEntry->i1= *ix;
    xJournalEntry->i2= *iy;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void dshabs (FTNINT *ix,FTNINT *iy, FTNINT *iMask)
{
FTNINT iXClip,iYClip, iXClip2, iYClip2;
FTNINT ixx,iyy, ixx2,iyy2;
float xx,yy, dx,dy, dLin,dBlank;
struct xJournalEntry_typ    * xJournalEntry;

    if (ClipLineStart(TKTRNX.kBeamX,TKTRNX.kBeamY, *ix,*iy, &iXClip,&iYClip)) {
     ClipLineStart(*ix,*iy, TKTRNX.kBeamX,TKTRNX.kBeamY, &iXClip2,&iYClip2); // Clip Endpunkt
     SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                       , sdlColorTable[TKTRNX.iLinCol].g
                                       , sdlColorTable[TKTRNX.iLinCol].b
                                       , sdlColorTable[TKTRNX.iLinCol].a );
     DrawHiResDashLine (iXClip,iYClip, iXClip2,iYClip2,iMask);

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= iXClip;
     xJournalEntry->i2= iYClip;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_DSHSTYLE;
     xJournalEntry->i1= *iMask;
     xJournalEntry->i2= 0;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_DSHABS;
     xJournalEntry->i1= iXClip2;
     xJournalEntry->i2= iYClip2;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
    }
    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action=  XACTION_MOVABS;
    xJournalEntry->i1= *ix;
    xJournalEntry->i2= *iy;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void pntabs (FTNINT *ix,FTNINT *iy)
{
struct xJournalEntry_typ    * xJournalEntry;
FTNINT ActPntMov;

    TKTRNX.kBeamX= *ix; TKTRNX.kBeamY= *iy;
    if (PointInWindow (*ix, *iy)) {
     SDL_SetRenderDrawColor(TCSrenderer, sdlColorTable[TKTRNX.iLinCol].r
                                       , sdlColorTable[TKTRNX.iLinCol].g
                                       , sdlColorTable[TKTRNX.iLinCol].b
                                       , sdlColorTable[TKTRNX.iLinCol].a );
     SDL_RenderDrawPoint(TCSrenderer, HiResX(*ix),HiResX(TEK_YMAX-*iy));
     ActPntMov= XACTION_PNTABS;
    } else {
     ActPntMov= XACTION_MOVABS;
    }
    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action= ActPntMov;
    xJournalEntry->i1= *ix;
    xJournalEntry->i2= *iy;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void bckcol (FTNINT *iCol)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.iBckCol= *iCol;
    if (*iCol > MAX_COLOR_INDEX) TKTRNX.iBckCol= MAX_COLOR_INDEX;

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action=  XACTION_BCKCOL;
    xJournalEntry->i1= TKTRNX.iBckCol;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void lincol (FTNINT *iCol)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.iLinCol= *iCol;
    if (*iCol > MAX_COLOR_INDEX) TKTRNX.iLinCol= MAX_COLOR_INDEX;

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action=  XACTION_LINCOL;
    xJournalEntry->i1= TKTRNX.iLinCol;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}




extern void txtcol (FTNINT *iCol)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.iTxtCol= *iCol;
    if (*iCol > MAX_COLOR_INDEX) TKTRNX.iTxtCol= MAX_COLOR_INDEX;

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action=  XACTION_TXTCOL;
    xJournalEntry->i1= TKTRNX.iTxtCol;
    xJournalEntry->i2= 0;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void DefaultColour (void)
{
    TKTRNX.iLinCol= TCSDefaultLinCol;
    TKTRNX.iTxtCol= TCSDefaultTxtCol;
    TKTRNX.iBckCol= TCSDefaultBckCol;

    lincol (&TKTRNX.iLinCol);
    txtcol (&TKTRNX.iTxtCol);
    bckcol (&TKTRNX.iBckCol);
}



/*
------------------- Userroutinen: Graphiktext ----------------------------------
*/



extern void outgtext(FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) )
{
int i, iL;
char outbuf [TCS_MESSAGELEN+1];
struct xJournalEntry_typ    * xJournalEntry;

    if (FTNSTRPARA(ftn_string)[0] == '\0' ) return; // Leerstring char(0)

    iL= 0; // Bei Bedarf String mit char(0) abschliessen -> Kopie in outbuf
    while ( (FTNSTRPARA(ftn_string)[iL] != '\0')  &&     // c-String bis \0
                    (iL < FTNSTRPARL(ftn_string)) &&     // String= Fortran Konstante
                    (iL < TCS_MESSAGELEN-1)       ) {    // Buffer Overflow
     outbuf[iL]= FTNSTRPARA(ftn_string)[iL];
     iL++;
    }
    outbuf[iL]= '\0'; //

    PlotText (outbuf);

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_GTEXT;
     xJournalEntry->i1= (FTNINT) iL;
     xJournalEntry->i2= (FTNINT) FTNSTRPARA(ftn_string)[0];
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

     i= 1;
     while (i < iL) {
      xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
      if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
      xJournalEntry->action=  XACTION_ASCII;
      xJournalEntry->i1= (FTNINT) FTNSTRPARA(ftn_string)[i++];
      if ( i<iL ) {
       xJournalEntry->i2= (FTNINT) FTNSTRPARA(ftn_string)[i++];
      } else {
       xJournalEntry->i2= (FTNINT) 0;
      }
      SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
     }

     xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
     if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
     xJournalEntry->action=  XACTION_MOVABS;
     xJournalEntry->i1= TKTRNX.kBeamX;
     xJournalEntry->i2= TKTRNX.kBeamY;
     SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)

}



extern void italic (void)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.kitalc = 1;
    TTF_SetFontStyle(TCSfont, TTF_STYLE_ITALIC);

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void italir (void)
{
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.kitalc = 0;
    TTF_SetFontStyle(TCSfont, TTF_STYLE_NORMAL);

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void dblsiz (void)
{
int wx,wz;
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.ksizef = 1;

    if (!TCSfont)TTF_CloseFont(TCSfont);
    TCSfont = TTF_OpenFont(szTCSGraphicFont, 2*HiResY(TEK_YMAX *TCS_REL_CHR_HEIGHT));
    if (!TCSfont) {
     TCSGraphicError (ERR_NOFNT,TTF_GetError() );
    } else {
     if(TTF_SizeText(TCSfont,"M",&wx,&wz)) {
      TCSGraphicError (ERR_NOFNT,TTF_GetError() );
     } else {
      TKTRNX.khorsz= LoResX(wx);
      TKTRNX.kversz= LoResY(wz);
      TKTRNX.khomey= TEK_YMAX - TKTRNX.kversz;
     }
	}

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}



extern void nrmsiz (void)
{
int wx, wz;
struct xJournalEntry_typ    * xJournalEntry;

    TKTRNX.ksizef = 0;

    if (!TCSfont)TTF_CloseFont(TCSfont);
    TCSfont = TTF_OpenFont(szTCSGraphicFont, HiResY(TEK_YMAX *TCS_REL_CHR_HEIGHT));
    if (!TCSfont) {
     TCSGraphicError (ERR_NOFNT,TTF_GetError() );
    } else {
     if(TTF_SizeText(TCSfont,"M",&wx,&wz)) {
      TCSGraphicError (ERR_NOFNT,TTF_GetError() );
     } else {
      TKTRNX.khorsz= LoResX(wx);
      TKTRNX.kversz= LoResY(wz);
      TKTRNX.khomey= TEK_YMAX - TKTRNX.kversz;
     }
	}

    xJournalEntry= (struct xJournalEntry_typ*) malloc (sizeof (struct xJournalEntry_typ));
    if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
    xJournalEntry->action= XACTION_FONTATTR;
    xJournalEntry->i1= TKTRNX.kitalc;
    xJournalEntry->i2= TKTRNX.ksizef;
    SGLIB_DL_LIST_ADD (xJournalEntry_typ, xTCSJournal, xJournalEntry, previous, next)
}






extern void csize (FTNINT *ix,FTNINT *iy)
{
     *ix=  TKTRNX.khorsz;
     *iy=  TKTRNX.kversz;
}



extern void outtext(FTNSTRPAR * ftn_string FTNSTRPAR_TAIL(ftn_string) )
{
int iL;
char outbuf [TCS_MESSAGELEN+1];
SDL_Rect dstrect;
SDL_Surface* surface;
SDL_Texture* texture;

    if ( (FTNSTRPARA(ftn_string)[0] == '\0' ) // Leerstring char(0)
       || (TCSstatWindowIniYrelsiz <= 0 ) ) { // kein Statusfenster
     return;
    }
	SDL_RenderPresent (TCSrenderer);
    RepaintBuffer ();

    iL= 0; // Bei Bedarf String mit char(0) abschliessen -> Kopie in outbuf
    while ( (FTNSTRPARA(ftn_string)[iL] != '\0')  &&     // c-String bis \0
                    (iL < FTNSTRPARL(ftn_string)) &&     // String= Fortran Konstante
                    (iL < TCS_MESSAGELEN-1)       ) {    // Buffer Overflow
     outbuf[iL]= FTNSTRPARA(ftn_string)[iL];
     iL++;
    }
    outbuf[iL]= '\0'; //

	SDL_SetRenderDrawColor(TCSstatrenderer, sdlColorTable[TCSDefaultBckCol].r
	                                  , sdlColorTable[TCSDefaultBckCol].g
	                                  , sdlColorTable[TCSDefaultBckCol].b
	                                  , sdlColorTable[TCSDefaultBckCol].a );
	SDL_RenderClear (TCSstatrenderer);

#ifdef HIGHQUALCHAR
    surface = TTF_RenderUTF8_Blended (TCSstatusfont, outbuf, sdlColorTable[TCSDefaultLinCol]);
#else
    surface = TTF_RenderUTF8_Solid (TCSstatusfont, outbuf, sdlColorTable[TCSDefaultLinCol]);
#endif

    texture = SDL_CreateTextureFromSurface(TCSstatrenderer, surface);

    dstrect.x= 0;
    dstrect.y= 0;
    SDL_QueryTexture(texture, NULL, NULL, &dstrect.w, &dstrect.h);
    SDL_RenderCopy(TCSstatrenderer, texture, NULL, &dstrect);

    SDL_RenderPresent(TCSstatrenderer);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}



extern void bell (void)
{
#ifdef AUDIOSUPPORT
    AudioSample_nr= 0;
    SDL_PauseAudio(0); // start playing sound
    SDL_Delay(BELL_DURATION); // wait while sound is playing
    SDL_PauseAudio(1); // stop playing sound
#endif
    return;
}


extern void GraphicError (FTNINT *iErr, FTNSTRPAR *ftn_string,
                                     FTNINT *iL  FTNSTRPAR_TAIL(ftn_string))
{
    TCSGraphicError (*iErr, FTNSTRPARA(ftn_string));

}



/*
------------------- Userroutinen: Graphic Input---------------------------------
*/



extern void dcursr (FTNINT *ic,FTNINT *ix,FTNINT *iy)
{
SDL_Event  event;

    if (!TCSinitialized) return;            /* Aufhängen vermeiden */

    SDL_RenderPresent (TCSrenderer);
    RepaintBuffer ();
    SDL_RaiseWindow(TCSwindow); // Set input focus

    *ic= 0;
    while (*ic == 0) {
     SDL_WaitEvent (&event);
     switch (event.type) {
      case SDL_KEYDOWN:
       if (event.key.keysym.sym < 256) {
        *ic= (FTNINT) event.key.keysym.sym;
       }
       break;
      case SDL_MOUSEBUTTONDOWN:
       if (ix == iy) break; // Aufruf TINPUT, nicht  DCURSR
       switch (event.button.button) { // Tastaturcode analog DOS
        case SDL_BUTTON_LEFT: *ic= 1; break;
        case SDL_BUTTON_RIGHT: *ic= 2; break;
        case SDL_BUTTON_MIDDLE: *ic= 4; break;
       }
       *ix= (FTNINT) (LoResX(event.button.x));
       *iy= (FTNINT) (TEK_YMAX-LoResY(event.button.y));
       break;
      default:
       TCSEventFilter(NULL, &event); // Weiterleitung Standardhandler, ic = Dummy
       break;
     }
    }
}



/*
------------------- Userroutinen: Hardcopy -------------------------------------
*/



extern void hdcopy (void)
{

FTNINT      iErr;
FTNSTRDESC  ftnstrg;
char        szTmpString[TCS_FILE_NAMELEN];
SDL_RWops*  hFile;
struct xJournalEntry_typ *xJournalEntry;

    snprintf( szTmpString,TCS_FILE_NAMELEN, szTCSHardcopyFile, iHardcopyCount++ );
    hFile = SDL_RWFromFile( szTmpString, "r" );
    while ((iHardcopyCount < MAX_HDCCOUNT) && (hFile != NULL) ) {
     SDL_RWclose (hFile);
     snprintf( szTmpString,TCS_FILE_NAMELEN, szTCSHardcopyFile, iHardcopyCount++ );
     hFile = SDL_RWFromFile( szTmpString, "r" );
    }
    SDL_LogDebug (SDL_LOG_CATEGORY_SYSTEM, "HDCOPY> iHardcopyCount Next= %i", iHardcopyCount);
    SDL_LogDebug (SDL_LOG_CATEGORY_SYSTEM, "HDCOPY> Filnam= %s", szTmpString);
    if (hFile != NULL) { // iHardcopyCount zu klein
     SDL_RWclose (hFile);
     SDL_LogError (SDL_LOG_CATEGORY_SYSTEM, "HDCOPY> Open HDC_File: kein freier Filename");
     return;
    }

    hFile = SDL_RWFromFile( szTmpString, "wb" );
    if (hFile == NULL) {
     SDL_LogError (SDL_LOG_CATEGORY_SYSTEM, "HDCOPY> Error openening %s",szTmpString);
     return;
    }

    TCSGraphicError (MSG_HDCACT, szTmpString);

    SGLIB_DL_LIST_GET_LAST (struct xJournalEntry_typ, xTCSJournal, previous, next, xJournalEntry)
#ifdef TRACE_CALLS
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> xTCSJournal: Ptr= %p", xTCSJournal);
    SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> 1. Entry: Ptr= %p / previous: Ptr= %p / next: Ptr= %p", xJournalEntry, xJournalEntry -> previous, xJournalEntry -> next);
#endif
    while (xJournalEntry != NULL) {
     snprintf( szTmpString,TCS_FILE_NAMELEN, "%02i#%04i-%03i\n", xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );
     SDL_RWwrite(hFile, szTmpString, 1, strlen(szTmpString));
#ifdef TRACE_CALLS
     switch (xJournalEntry->action) {
       case XACTION_INITT: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_INITT");
        break;
       }
       case XACTION_ERASE: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_ERASE");
        break;
       }
       case XACTION_MOVABS: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_MOVABS: x= %i, y= %i", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_DRWABS: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_DRWABS: x= %i, y= %i", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_DSHSTYLE: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_DSHSTYLE: x= %i", xJournalEntry->i1);
        break;
       }
       case XACTION_DSHABS: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_DSHABS: x= %i, y= %i", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_PNTABS: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_PNTABS: x= %i, y= %i", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_BCKCOL: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_BCKCOL: x= %i", xJournalEntry->i1);
        break;
       }
       case XACTION_TXTCOL: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_TXTCOL: x= %i", xJournalEntry->i1);
        break;
       }
       case XACTION_LINCOL: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_LINCOL: x= %i", xJournalEntry->i1);
        break;
       }
       case XACTION_FONTATTR: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_FONTATTR: x= %i, y= %i", xJournalEntry->i1, xJournalEntry->i2);
        break;
       }
       case XACTION_GTEXT: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_GTEXT: Len= %i, Char[%i]= %c",
                      xJournalEntry->i1, xJournalEntry->i2, xJournalEntry->i2);
        break;
       }
       case XACTION_ASCII: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_ASCII: Char1[%i]= %c, Char2[%i]= %c",
                      xJournalEntry->i1, xJournalEntry->i1, xJournalEntry->i2, xJournalEntry->i2);
        break;
       }
       case XACTION_NOOP: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_NOOP");
        break;
       }
       default: {
        SDL_LogDebug (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> XACTION_XXX");
        break;
       }
     }
     SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> xJournalEntry: Ptr= %i / previous: Ptr= %i / next: Ptr= %i", xJournalEntry, xJournalEntry -> previous, xJournalEntry -> next);
#endif // TRACE_CALLS
     xJournalEntry= xJournalEntry -> previous;
    }

   SDL_RWclose (hFile);
#ifdef TRACE_CALLS
   SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> xTCSJournal New Current Entry: Ptr= %p", xJournalEntry);
   SDL_LogVerbose (SDL_LOG_CATEGORY_VIDEO, "HDCOPY> Previous: Ptr= %p  Next: Ptr= %p", xJournalEntry->previous, xJournalEntry->next);
#endif // TRACE_CALLS

}



/*
------------------- subroutine LIB_MOVC3 fuer Watcom- und GNU-Compiler ---------
Hier nicht benoetigt, nur wg. Kompatibilitaet zur DOS-Version enthalten
*/


extern void lib_movc3 (FTNINT *len,FTNSTRPAR *sou,FTNSTRPAR *dst
                                FTNSTRPAR_TAIL(sou)  FTNSTRPAR_TAIL(dst) )

{
int n;
    if (FTNSTRPARA(dst) <= FTNSTRPARA(sou) ) {
     for (n=0; n<*len; n++) FTNSTRPARA(dst)[n]= FTNSTRPARA(sou)[n];
    } else {
     for (n= (*len)-1; n>=0; n--) FTNSTRPARA(dst)[n]= FTNSTRPARA(sou)[n];
    };
}
