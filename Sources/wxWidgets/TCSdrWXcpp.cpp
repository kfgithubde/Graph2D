/** ****************************************************************************
\file       TCSdrWXcpp.cpp
\brief      wX Port: Low-Level Driver
\version    0.9
\author     (C) 2023 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Systemnahe Graphikroutinen für die Tektronix Emulation
\note \verbatim
          1. ToDo
\endverbatim
\~english
         system-specific subroutines of the Tektronix emulation
\note \verbatim
          1. ToDo
\endverbatim
\~
***************************************************************************** */


/*
------------------- Debug Switches ---------------------------------------------
*/

// #define wxDEBUG_LEVEL 0
#define wxDEBUG_LEVEL 2 // Debug: Output into the status window
// #define TRACE_CALLS // additional debug output: journalpointer

/*
------------------- Headerfiles ------------------------------------------------
*/

// #include <wx/intl.h>
// #include <wx/string.h>

#include <wx/frame.h>      // needed for: class cTSCcanvas
#include <wx/panel.h>
#include <wx/sizer.h>
// #include <wx/display.h>
// #include <wx/gdicmn.h>

#include <wx/dc.h>         // needed for: subroutine RepaintBuffer
#include <wx/dcclient.h>

#include <wx/log.h>        // needed for: subroutine TCSGraphicError
#include <wx/msgdlg.h>

#include <wx/stdpaths.h>   // needed for: winlbl
#include <wx/filename.h>

#include <wx/xml/xml.h>    // Read inifiles

#include <wx/file.h>

#include "sglib.h"         // Journal for repaint / hardcopy

#include "TCSdrWXcpp.hpp"  // program configuration
#include "TKTRNX.hpp"      // common block TCS
#include "G2dAG2.hpp"      // common block AG2
#include "graph2d.h"       // contains forward declarations



/*
------------------- Declarations ---------------------------------------------
*/

typedef struct xJournalEntry_typ {struct xJournalEntry_typ * previous;
                                  struct xJournalEntry_typ * next;
                                  int action; int i1; int i2;}
               xJournalEntry_typ;


class cTCScanvas
{
	public:

        wxFrame* TCSframe; // windows
		wxPanel* TCSpanel;
        wxLogWindow* logWindow;
        wxStatusBar* TCSstatusBar;

        wxWindowID ID_TCSframe;
        wxWindowID ID_TCSpanel;
        wxWindowID ID_TCSstatus;

        wxPen      TCSpen; //resources
        wxBrush    TCSbrush;
        wxFont     TCSfont;

        bool ClippingNotActive = true; // drawing status
        int TCSpanelKeyPressed;
        int TCSmouseButtonDown, TCSmouseX, TCSmouseY;

        xJournalEntry_typ* xTCSJournal = NULL; // journal used as a drawing metafile

        struct TKTRNX TekSav; // notepad for changing instances
        struct G2dAG2 AG2Sav;
        int    DefaultLinColSav, DefaultTxtColSav, DefaultBckColSav;
        char   HardcopyFileSav[TCS_FILE_NAMELEN], sect0Sav[TCS_FILE_NAMELEN];

        cTCScanvas(int iMode, wxFrame* parent, wxFrame* FrameToUse, wxStatusBar* StatusBarToUse);
		virtual ~cTCScanvas();

	protected:

	private:

        void CompleteCanvas (wxSize UseScreen, wxPoint PosScreen, wxSize MinScreen); // Add sizers, menues etc.

		void OnTCSClose(wxCloseEvent& event); // event handlers
		void OnTCSpanelPaint(wxPaintEvent& event);
		void OnTCSpanelResize(wxSizeEvent& event);
		void OnTCSpanelKey(wxKeyEvent& event);
		void OnTCSmouseLeft(wxMouseEvent& event);
		void OnTCSmouseMiddle(wxMouseEvent& event);
		void OnTCSmouseRight(wxMouseEvent& event);

};



/*
-------------------- Global Variables ------------------------------------------
*/

static char     szTCSWindowName[TCS_WINDOW_NAMELEN] = TCS_WINDOW_NAME,
                szTCSstatWindowName[TCS_WINDOW_NAMELEN] = TCS_STATWINDOW_NAME,
                szTCSIniFile[TCS_FILE_NAMELEN] = TCS_INIFILE_NAME,
                szTCSHardcopyFile[TCS_FILE_NAMELEN] = TCS_HDCFILE_NAME,
/*  129                 szTCSGraphicFont[TCS_FILE_NAMELEN] = TCS_INIDEF_FONT,
  130                 szTCSSysFont[TCS_FILE_NAMELEN] = TCS_INIDEF_SYSFONT,
*/
                szTCSsect0[TCS_FILE_NAMELEN] = TCS_INISECT0;


static  int     TCSwindowIniXrelpos = TCS_INIDEF_WINPOSX, // window size/position
                TCSwindowIniYrelpos = TCS_INIDEF_WINPOSY, // at initt in % of Screen
                TCSwindowIniXrelsiz = TCS_INIDEF_WINSIZX,
                TCSwindowIniYrelsiz = TCS_INIDEF_WINSIZY,
//                TCSstatWindowIniXrelpos = TCS_INIDEF_STATPOSX, // dito
//                TCSstatWindowIniYrelpos = TCS_INIDEF_STATPOSY, // Statusfenster
//                TCSstatWindowIniXrelsiz = TCS_INIDEF_STATSIZX,
//                TCSstatWindowIniYrelsiz = TCS_INIDEF_STATSIZY,
                TCSDefaultLinCol = TCS_INIDEF_LINCOL,
                TCSDefaultTxtCol = TCS_INIDEF_TXTCOL,
                TCSDefaultBckCol = TCS_INIDEF_BCKCOL,
                iHardcopyCount = 1;  // Zähler zur Erzeugung Filenamen



/*
   Assign error numbers to error messages
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
                 "DOS",
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
                 10,
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
   Assign colour numbers VGA palette coordinates
*/


#define MAX_COLOR_INDEX 15

 static wxColour TCSColorTable[MAX_COLOR_INDEX+1] = {
                   {240,240,240,wxALPHA_OPAQUE }, /* iCol= 00: weiss (DOS: 01) */
                   {  0,  0,  0,wxALPHA_OPAQUE }, /* iCol= 01: schwarz(DOS:00) */
                   {240, 80, 80,wxALPHA_OPAQUE }, /* iCol= 02: rot             */
                   { 80,240, 80,wxALPHA_OPAQUE }, /* iCol= 03: gruen           */
                   { 80,240,240,wxALPHA_OPAQUE }, /* iCol= 04: blau            */
                   { 80, 80,240,wxALPHA_OPAQUE }, /* iCol= 05: lila            */
                   {240,240, 80,wxALPHA_OPAQUE }, /* iCol= 06: gelb            */
                   {160,160,160,wxALPHA_OPAQUE }, /* iCol= 07: grau            */
                   {240, 80,240,wxALPHA_OPAQUE }, /* iCol= 08: violett         */
                   {160,  0,  0,wxALPHA_OPAQUE }, /* iCol= 09: mattrot         */
                   {  0,160,  0,wxALPHA_OPAQUE }, /* iCol= 10: mattgruen       */
                   {  0,  0,160,wxALPHA_OPAQUE }, /* iCol= 11: mattblau        */
                   {  0,160,160,wxALPHA_OPAQUE }, /* iCol= 12: mattlila        */
                   {160, 80,  0,wxALPHA_OPAQUE }, /* iCol= 13: orange          */
                   { 80, 80, 80,wxALPHA_OPAQUE }, /* iCol= 14: mattgrau        */
                   {160,  0,160,wxALPHA_OPAQUE }  /* iCol= 15: mattviolett     */
                };


// static int      TCSEventFilterData; // Userdata, z.Zt. nicht verwendet

static cTCScanvas*     ActiveCanvas = NULL;
static wxWindowID      ActiveCanvasID = 0;
static cTCScanvas*     OpenCanvases[MAX_OPEN_CANVAS] = {};



// ---------------- Internal subroutines -------------------------------------


/*
  Initialization COMMON TKTRNX before creating new object of class  cTCScanvas
*/

void initt0 ()
{
  tktrnx_.iLinCol= TCSDefaultLinCol; // reset colours
  tktrnx_.iTxtCol= TCSDefaultTxtCol;
  tktrnx_.iBckCol= TCSDefaultBckCol;

  tktrnx_.ksizef = 0; // Reset FONT
  tktrnx_.kitalc = 0;

  tktrnx_.xlog= 255.; // call LINTRN
  tktrnx_.ylog= 255.;
  tktrnx_.kminsx= 0; // call SWINDO (0,1023,0,780)
  tktrnx_.kmaxsx= (int) TEK_XMAX;
  tktrnx_.kminsy= 0;
  tktrnx_.kmaxsy= (int) TEK_YMAX;
  tktrnx_.tminvx= 0.; // call VWINDO (0.,1023.,0.,780.)
  tktrnx_.tmaxvx= TEK_XMAX;
  tktrnx_.tminvy= 0.;
  tktrnx_.tmaxvy= TEK_YMAX;
  tktrnx_.xfac= 1.; // subroutine RESCAL, called from LINTRN...VWINDO
  tktrnx_.yfac= 1.;
  tktrnx_.trsinf= 0.; // call RROTAT (0.)
  tktrnx_.trcosf= 1.;
  tktrnx_.trscal= 1.; // call RSCALE (1.)

  tktrnx_.klmrgn= 0; //  call SETMRG (0,1023)
  tktrnx_.krmrgn= (int) TEK_XMAX;
}


wxWindowID getCanvasID (wxWindowID win2search)
{
  int i;

    i= MAX_OPEN_CANVAS-1;
    while (i >= 0) {
      if (OpenCanvases[i] != nullptr) {
        if ( (OpenCanvases[i]->ID_TCSframe == win2search) ||
             (OpenCanvases[i]->ID_TCSpanel == win2search) ) return i;
      }
      i--;
    }
    return i; // i<0 -> window is not a member of any canvas
}



void RepaintBuffer (wxDC& dc)
{
  xJournalEntry_typ * xJournalEntry;
  int DashStyle;
  wxCoord w,h;
  int iStringLen, iStringActual;
  char szString [TCS_MESSAGELEN+1];

    wxLogDebug ( wxT("RepaintBuffer> called"));
#ifdef TRACE_CALLS
    wxLogDebug ( wxT("RepaintBuffer> xTCSJournal: Ptr= %p / Current Entry: Ptr= %p"), ActiveCanvas->xTCSJournal, xJournalEntry);
#endif // TRACE_CALLS

    SGLIB_DL_LIST_GET_LAST(xJournalEntry_typ, ActiveCanvas->xTCSJournal, previous, next, xJournalEntry)
    while (xJournalEntry != NULL) {

#ifdef TRACE_CALLS
     wxLogDebug ( wxT("RepaintBuffer> xTCSJournal: Ptr= %p"), ActiveCanvas->xTCSJournal);
     wxLogDebug ( wxT("RepaintBuffer> Current Entry: Ptr= %p / previous: Ptr= %p / next: Ptr= %p"),
                     xJournalEntry, xJournalEntry->previous, xJournalEntry->next);
     wxLogDebug ( wxT("RepaintBuffer> XACTION_??? = %i (i1= %i, i2= %i)"),
                     xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );
#endif // TRACE_CALLS

      switch (xJournalEntry->action) {
        case XACTION_INITT: {
          initt0 ();

          ActiveCanvas->TCSpen.SetColour (TCSColorTable[tktrnx_.iLinCol]);
          ActiveCanvas->TCSpen.SetStyle (wxPENSTYLE_SOLID);
          dc.SetPen(ActiveCanvas->TCSpen); // Umbedingt Initialstift setzen !!!

          tktrnx_.kbeamx = tktrnx_.klmrgn;  //  call HOME, first guess khomey in INITT1()
          tktrnx_.kbeamy = tktrnx_.khomey;
        } // continue with Erase
        case XACTION_ERASE: {
          ActiveCanvas->TCSbrush.SetColour (TCSColorTable[tktrnx_.iBckCol]);
          dc.SetBrush (ActiveCanvas->TCSbrush);
          dc.SetBackground (ActiveCanvas->TCSbrush);
          dc.Clear();

          ActiveCanvas->TCSfont = wxFont(wxFONTSIZE_MEDIUM, wxFONTFAMILY_TELETYPE,
                                       wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);
          ActiveCanvas->TCSfont.SetFractionalPointSize (TEK_YMAX*TCS_REL_CHR_HEIGHT*(1+tktrnx_.ksizef));
          dc.SetFont(ActiveCanvas->TCSfont);

          dc.GetTextExtent ("MMMMMMMMMM", &w, &h);
          tktrnx_.khorsz = (int) (w*0.1+0.5);
          tktrnx_.kversz = h;
          tktrnx_.khomey= (int) TEK_YMAX - tktrnx_.kversz;

          break; // Erase don't change the cursor position
        }
        case XACTION_MOVABS: {
          tktrnx_.kbeamx= xJournalEntry->i1;
          tktrnx_.kbeamy= xJournalEntry->i2;
          break;
        }
        case XACTION_DRWABS: {
          if (!ActiveCanvas->ClippingNotActive) {
            dc.SetClippingRegion(tktrnx_.kminsx, tktrnx_.kminsy,
               tktrnx_.kmaxsx-tktrnx_.kminsx, tktrnx_.kmaxsy-tktrnx_.kminsy);
          }
          dc.DrawLine (tktrnx_.kbeamx,tktrnx_.kbeamy ,
                       xJournalEntry->i1, xJournalEntry->i2);
          tktrnx_.kbeamx= xJournalEntry->i1;
          tktrnx_.kbeamy= xJournalEntry->i2;
          dc.DrawPoint (tktrnx_.kbeamx, tktrnx_.kbeamy); // Set last point of line
          if (!ActiveCanvas->ClippingNotActive) dc.DestroyClippingRegion();
          break;
        }
        case XACTION_DSHSTYLE: {
          switch (xJournalEntry->i1) {
            case 0:  DashStyle= wxPENSTYLE_SOLID;
                     break;
            case 1:  DashStyle= wxPENSTYLE_DOT;
                     break;
            case 2:  DashStyle= wxPENSTYLE_DOT_DASH;
                     break;
            case 3:  DashStyle= wxPENSTYLE_LONG_DASH;
                     break;
            default: DashStyle= wxPENSTYLE_SOLID;
          }
          break;
        }
        case XACTION_DSHABS: {
          ActiveCanvas->TCSpen.SetStyle (DashStyle);
          dc.SetPen(ActiveCanvas->TCSpen);
          if (!ActiveCanvas->ClippingNotActive) {
            dc.SetClippingRegion(tktrnx_.kminsx, tktrnx_.kminsy,
               tktrnx_.kmaxsx-tktrnx_.kminsx, tktrnx_.kmaxsy-tktrnx_.kminsy);
          }
          dc.DrawLine (tktrnx_.kbeamx,tktrnx_.kbeamy ,
                       xJournalEntry->i1, xJournalEntry->i2);
          if (!ActiveCanvas->ClippingNotActive) dc.DestroyClippingRegion();
          ActiveCanvas->TCSpen.SetStyle (wxPENSTYLE_SOLID);
          dc.SetPen(ActiveCanvas->TCSpen); // reset to SOLID

          tktrnx_.kbeamx= xJournalEntry->i1;
          tktrnx_.kbeamy= xJournalEntry->i2;
          break;
        }
        case XACTION_PNTABS: {
          tktrnx_.kbeamx= xJournalEntry->i1;
          tktrnx_.kbeamy= xJournalEntry->i2;
          if (!ActiveCanvas->ClippingNotActive) {
            dc.SetClippingRegion(tktrnx_.kminsx, tktrnx_.kminsy,
               tktrnx_.kmaxsx-tktrnx_.kminsx, tktrnx_.kmaxsy-tktrnx_.kminsy);
          }
          dc.DrawPoint (tktrnx_.kbeamx, tktrnx_.kbeamy);
          if (!ActiveCanvas->ClippingNotActive) dc.DestroyClippingRegion();
          break;
        }
        case XACTION_BCKCOL: {
          tktrnx_.iBckCol= xJournalEntry->i1;
          ActiveCanvas->TCSbrush.SetColour (TCSColorTable[tktrnx_.iBckCol]);
          dc.SetBrush (ActiveCanvas->TCSbrush);
          dc.SetBackground (ActiveCanvas->TCSbrush);
          break;
        }
        case XACTION_LINCOL: {
          tktrnx_.iLinCol= xJournalEntry->i1;
          ActiveCanvas->TCSpen.SetColour (TCSColorTable[tktrnx_.iLinCol]);
          dc.SetPen(ActiveCanvas->TCSpen);
          break;
        }
        case XACTION_TXTCOL: {
          tktrnx_.iTxtCol= xJournalEntry->i1;
          dc.SetTextForeground (TCSColorTable[tktrnx_.iTxtCol]);
          break;
        }
        case XACTION_FONTATTR: {
          tktrnx_.kitalc= xJournalEntry->i1;
          if (tktrnx_.kitalc > 0) {
            ActiveCanvas->TCSfont.SetStyle (wxFONTSTYLE_ITALIC);
          } else {
            ActiveCanvas->TCSfont.SetStyle (wxFONTSTYLE_NORMAL);
          }

          if (tktrnx_.ksizef != xJournalEntry->i2) {
            tktrnx_.ksizef= xJournalEntry->i2;
            if (tktrnx_.ksizef > 0) {
              ActiveCanvas->TCSfont.SetFractionalPointSize (2.0* TEK_YMAX*TCS_REL_CHR_HEIGHT);
            } else {
              ActiveCanvas->TCSfont.SetFractionalPointSize (TEK_YMAX *TCS_REL_CHR_HEIGHT);
            }
          }
          dc.SetFont(ActiveCanvas->TCSfont);
          dc.GetTextExtent ("MMMMMMMMMM", &w, &h);
          tktrnx_.khorsz = (int) (w*0.1+0.5);
          tktrnx_.kversz = h;
          tktrnx_.khomey= TEK_YMAX - tktrnx_.kversz;
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
              dc.GetTextExtent (szString, &w, &h);
              dc.DrawText (szString, tktrnx_.kbeamx, tktrnx_.kbeamy+ TCS_REL_CHR_SPACING*h); // +h: Plot text from UPPER left corner
              tktrnx_.kbeamx += w; // move cursor to End of String
            }
        break;
        }
        case XACTION_ASCII: {
          if (iStringActual < iStringLen) {
            szString[iStringActual++]= xJournalEntry->i1;
            if (iStringActual < iStringLen) szString[iStringActual++]= xJournalEntry->i2;
            if (iStringActual >= iStringLen ) {
              szString[iStringActual]= '\0';
              dc.GetTextExtent (szString, &w, &h);
              dc.DrawText (szString, tktrnx_.kbeamx, tktrnx_.kbeamy+ TCS_REL_CHR_SPACING*h);
              tktrnx_.kbeamx += w;
            }
          }
          break;
        }
        case XACTION_NOOP: {
          break;
        }
        case XACTION_CLIP: {
          ActiveCanvas->ClippingNotActive= (xJournalEntry->i1 == 0);
          break;
        }
        case XACTION_CLIP1: {
          tktrnx_.kminsx= xJournalEntry->i1;
          tktrnx_.kminsy= xJournalEntry->i2;
          break;
        }
        case XACTION_CLIP2: {
          tktrnx_.kmaxsx= xJournalEntry->i1;
          tktrnx_.kmaxsy= xJournalEntry->i2;
          break;
        }
        default: {
          wxLogDebug (wxT("RepaintBuffer> XACTION_XXX"));
          break;
        }
      }
    xJournalEntry= xJournalEntry -> previous;
    }
#ifdef TRACE_CALLS
    wxLogDebug ( wxT("RepaintBuffer> xTCSJournal: Ptr= %p / Last Entry: Ptr= %p"), ActiveCanvas->xTCSJournal, xJournalEntry);
#endif // TRACE_CALLS
 }


/*
  Setting default values before reading the initialization files
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

    strncpy (szTCSWindowName, TCS_WINDOW_NAME, TCS_WINDOW_NAMELEN);
    strncpy (szTCSstatWindowName, TCS_STATWINDOW_NAME, TCS_WINDOW_NAMELEN);
    strncpy (szTCSIniFile, "", TCS_FILE_NAMELEN);
    strncpy (szTCSsect0, TCS_INISECT0, TCS_FILE_NAMELEN);

    // No Reset of Hardcopyname and Counter

    // Error messages should be changed only once

}



void CustomizeProgPar ()
#if (TCS_WINDOW_NAMELEN <= TCS_FILE_NAMELEN) // Get a safe buffer
  #define TMPSTRLEN TCS_FILE_NAMELEN
#else
  #define TMPSTRLEN TCS_WINDOW_NAMELEN
#endif
{
  size_t iL;
  char* szTemp;
  char TmpStr[TMPSTRLEN];
  wxString wxTmpStr;
  wxFileName wxTmpFilNam;

    szTemp= strstr (szTCSWindowName, PROGDIRTOKEN); // Default ProgDir?
    if (szTemp != NULL) {
      strncpy (TmpStr, szTCSWindowName, TMPSTRLEN);
      wxTmpFilNam= wxStandardPaths::Get().GetExecutablePath();
      wxTmpStr= wxTmpFilNam.GetFullName();
      iL= szTemp-szTCSWindowName;
      if ((TCS_WINDOW_NAMELEN-iL) > 1) {
        strncpy (szTemp, wxTmpStr, TCS_WINDOW_NAMELEN-iL);
        if ((TCS_WINDOW_NAMELEN-iL-wxTmpStr.length()) > 1) {
          strncpy (&szTCSWindowName[iL+wxTmpStr.length()],
                   &TmpStr[iL+strlen(PROGDIRTOKEN)], TCS_WINDOW_NAMELEN-iL-wxTmpStr.length());
        }
      }
      szTCSWindowName[TCS_WINDOW_NAMELEN-1]= '\0'; // just in case...
    }
#undef TMPSTRLEN
}



void XMLreadProgPar (const char * filname)
{
  wxXmlDocument xmlDoc;
  wxXmlNode *node, *node1, *NodeSect0;

  size_t iL;

  long longTmp;
  wxString wxTmpStr;


    if (filname[0] != '\0') {
      if (!wxFileExists(filname)) {
         TCSGraphicError (ERR_XMLOPEN, filname); // No input file
         return; // give warning and continue with defaults
      }
      if (!xmlDoc.Load(filname)) {
        TCSGraphicError (ERR_XMLOPEN, filname); // Unknown file error
        return; // unexpected file error -> handle error in any case
      }
      if (xmlDoc.GetRoot() == nullptr) {
        TCSGraphicError (ERR_XMLOPEN, filname); // No root node
        return;
      }
      NodeSect0=  nullptr;
      if (xmlDoc.GetRoot()->GetName().IsSameAs(TCS_INISECT0)) {
        NodeSect0= xmlDoc.GetRoot() ;
      } else {
        node= xmlDoc.GetRoot()->GetChildren();
        while (node != nullptr) {
          if (node->GetName().IsSameAs(TCS_INISECT0)) {
            NodeSect0= node;
            break;
          }
          node= node->GetNext();
        }
      }
      if (NodeSect0 != nullptr) {
        node1= NodeSect0->GetChildren();
        while (node1 != nullptr) {
          if (node1->GetName().IsSameAs(TCS_INISECT1)) {
            node= node1->GetChildren();
            while (node != nullptr) {
              if (node->GetName().IsSameAs(TCS_INIVAR_WINNAM)) {
                iL= node->GetNodeContent().length();
                if (iL > 0) {
                  wxTmpStr= node->GetNodeContent().Truncate(TCS_WINDOW_NAMELEN);
                  strncpy (szTCSWindowName, wxTmpStr.c_str(), TCS_WINDOW_NAMELEN);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_STATNAM)) {
                iL= node->GetNodeContent().length();
                if (iL > 0) {
                  wxTmpStr= node->GetNodeContent().Truncate(TCS_WINDOW_NAMELEN);
                  strncpy (szTCSstatWindowName, wxTmpStr.c_str(), TCS_WINDOW_NAMELEN);
                }
              }
              node= node->GetNext();
            } // end dataloop TCS_INISECT1

          } else if (node1->GetName().IsSameAs(TCS_INISECT2)) { // TCS_INISECT2: Layout
            node= node1->GetChildren();
            while (node != nullptr) {
              wxTmpStr= node->GetNodeContent();
              if (node->GetName().IsSameAs(TCS_INIVAR_WINPOSX)) {
                if (wxTmpStr.IsNumber()) {
                  TCSwindowIniXrelpos= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_WINPOSY)) {
                if (wxTmpStr.IsNumber()) {
                  TCSwindowIniYrelpos= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_WINSIZX)) {
                if (wxTmpStr.IsNumber()) {
                  TCSwindowIniXrelsiz= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_WINSIZY)) {
                if (wxTmpStr.IsNumber()) {
                  TCSwindowIniYrelsiz= wxAtoi(wxTmpStr);
                }
/*
              } else if (node->GetName().IsSameAs(TCS_INIVAR_STATPOSX)) {
                if (wxTmpStr.IsNumber()) {
                  TCSstatWindowIniXrelpos= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_STATPOSY)) {
                if (wxTmpStr.IsNumber()) {
                  TCSstatWindowIniYrelpos= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_STATSIZX)) {
                if (wxTmpStr.IsNumber()) {
                  TCSstatWindowIniXrelsiz= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_STATSIZY)) {
                if (wxTmpStr.IsNumber()) {
                  TCSstatWindowIniYrelsiz= wxAtoi(wxTmpStr);
                }
*/
              } else if (node->GetName().IsSameAs(TCS_INIVAR_LINCOL)) {
                if (wxTmpStr.IsNumber()) {
                  TCSDefaultLinCol= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_TXTCOL)) {
                if (wxTmpStr.IsNumber()) {
                  TCSDefaultTxtCol= wxAtoi(wxTmpStr);
                }
              } else if (node->GetName().IsSameAs(TCS_INIVAR_BCKCOL)) {
                if (wxTmpStr.IsNumber()) {
                  TCSDefaultBckCol= wxAtoi(wxTmpStr);
                }

              }
              node= node->GetNext();
            } // end dataloop TCS_INISECT2
          }
          node1= node1->GetNext();
        }
      }
    }
 }



/* --------- Object cTCScanvas ------ */


cTCScanvas::cTCScanvas(int iMode, wxFrame* parent, wxFrame* FrameToUse, wxStatusBar* StatusBarToUse)
{
  wxRect Screen;
  wxSize UseScreen, MinScreen;
  wxPoint PosScreen;

    if (iMode == 0) return;

    if (FrameToUse == nullptr) {
      ID_TCSframe = wxNewId();  // TCSframe->GetID()
      TCSframe= new wxFrame(parent, ID_TCSframe, szTCSWindowName, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, wxString::Format(wxT("%i"),ID_TCSframe));
      TCSstatusBar= TCSframe->GetStatusBar();
    } else {
      TCSframe= FrameToUse; // Use given plot frame
      ID_TCSframe = FrameToUse->GetId();
    }

    TCSstatusBar= StatusBarToUse;
    if ( StatusBarToUse != nullptr ) {
      ID_TCSstatus = TCSstatusBar->GetId();
    } else {
      ID_TCSstatus = wxID_NONE;
    }

    if (iMode <= 2) { // New window: use screensize/title from TCS initialization
      Screen = wxGetClientDisplayRect (); // usable screen size
      if (TCSwindowIniYrelsiz > 0) {
        UseScreen.x = TCSwindowIniXrelsiz * Screen.width / 100;
        UseScreen.y = TCSwindowIniYrelsiz * Screen.height / 100;  // TCSframe->GetMaxClientSize()
        PosScreen.x = TCSwindowIniXrelpos * Screen.width / 100;
        PosScreen.y = TCSwindowIniYrelpos * Screen.height / 100;  // TCSframe->GetScreenPosition()
        MinScreen = wxSize(-1,-1); // No restriction
      }
      if (strlen(szTCSWindowName)>0) TCSframe->SetLabel(szTCSWindowName); // only for iMode=2 relevant

      if (TCSstatusBar == nullptr) {
        ID_TCSstatus = wxNewId();
        TCSstatusBar = new wxStatusBar(TCSframe, ID_TCSstatus, wxSTB_DEFAULT_STYLE, wxString::Format(wxT("%i"),ID_TCSstatus));
        TCSstatusBar->SetFieldsCount(1);
        TCSframe->SetStatusBar(TCSstatusBar);
      }
    } else { // keep current screensize and title
      UseScreen = TCSframe->GetClientSize ();
      PosScreen = wxPoint(-1,-1); // x < 0 -> don't touch position
      MinScreen = UseScreen; // don't allow screensize 0
    }
    CompleteCanvas(UseScreen, PosScreen, MinScreen);
}



void cTCScanvas::CompleteCanvas (wxSize UseScreen, wxPoint PosScreen, wxSize MinScreen)
{
  wxBoxSizer* TCSBoxSizer;
    ID_TCSpanel = wxNewId();
    TCSpanel = new wxPanel(TCSframe, ID_TCSpanel, wxDefaultPosition, UseScreen, wxTAB_TRAVERSAL, wxString::Format(wxT("%i"),ID_TCSpanel));
    TCSpanel->SetMinSize(MinScreen);
    TCSpanel->SetMaxSize(wxSize(-1,-1));
    TCSBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    TCSBoxSizer->Add(TCSpanel, 1, wxALL|wxEXPAND, 5);
    TCSframe->SetSizer(TCSBoxSizer);
    TCSBoxSizer->Fit(TCSframe);
    TCSBoxSizer->SetSizeHints(TCSframe);

    TCSframe->SetClientSize (UseScreen);
    if (PosScreen.x > 0) {
     TCSframe->Move (PosScreen);
    }

    TCSframe->Connect(wxID_ANY,wxEVT_CLOSE_WINDOW,(wxObjectEventFunction)&cTCScanvas::OnTCSClose);

    TCSpanel->Connect(wxEVT_PAINT,(wxObjectEventFunction)&cTCScanvas::OnTCSpanelPaint,0,this->TCSframe);
    TCSpanel->Connect(wxEVT_SIZE, (wxObjectEventFunction)&cTCScanvas::OnTCSpanelResize,0,this->TCSframe);
    TCSpanel->Connect(wxEVT_KEY_DOWN,(wxObjectEventFunction)&cTCScanvas::OnTCSpanelKey);
    TCSpanel->Connect(wxEVT_LEFT_DOWN ,(wxObjectEventFunction)&cTCScanvas::OnTCSmouseLeft);
    TCSpanel->Connect(wxEVT_MIDDLE_DOWN ,(wxObjectEventFunction)&cTCScanvas::OnTCSmouseMiddle);
    TCSpanel->Connect(wxEVT_RIGHT_DOWN ,(wxObjectEventFunction)&cTCScanvas::OnTCSmouseRight);
}



cTCScanvas::~cTCScanvas()
{
    finitt_ (NULL, NULL); // -> Destroy ();
}


void cTCScanvas::OnTCSClose(wxCloseEvent& event)
{
    if ((event.GetId() == ActiveCanvas->ID_TCSframe) ||
                       (event.GetId() == ActiveCanvas->ID_TCSpanel)) {
      finitt_ (NULL, NULL); // -> Destroy ();
    }
}


void cTCScanvas::OnTCSpanelPaint(wxPaintEvent& event)
{
  wxWindowID RequestingWindowID, WorkWindowID;

    WorkWindowID = ActiveCanvasID; // store for further plotting
    RequestingWindowID = getCanvasID (event.GetId());
    if (RequestingWindowID >= 0) { // requested window belongs to a TCScanvas
      if (RequestingWindowID != WorkWindowID) WINSELECT (&RequestingWindowID);
      wxPaintDC dc (ActiveCanvas->TCSpanel);
      dc.GetSize (&tktrnx_.kScrX, &tktrnx_.kScrY);
      dc.SetAxisOrientation (true, true);      // y-axis bottom->up
      dc.SetDeviceOrigin (0., tktrnx_.kScrY);  // (0,0) lower left corner
      dc.SetLogicalScale (tktrnx_.kScrX/TEK_XMAX, tktrnx_.kScrY/TEK_YMAX);
      RepaintBuffer (dc);
      if (RequestingWindowID != WorkWindowID) WINSELECT (&WorkWindowID);
    }
}



void cTCScanvas::OnTCSpanelResize(wxSizeEvent& event)
{
  wxWindowID RequestingWindowID;

    RequestingWindowID = getCanvasID (event.GetId());
    if (RequestingWindowID >= 0) { // requesting window belongs to a TCScanvas
      OpenCanvases[RequestingWindowID]->TCSpanel->Refresh (); // Redraw with new scale -> wxEVT_PAINT
    } // Only OnTCSpanelPaint() switches windows
}



void cTCScanvas::OnTCSpanelKey(wxKeyEvent& event)
{
    ActiveCanvas->TCSpanelKeyPressed= event.GetKeyCode();
    if ((!event.m_shiftDown) && (ActiveCanvas->TCSpanelKeyPressed > 0x40)
                             && (ActiveCanvas->TCSpanelKeyPressed < 0x5b) ) {
      ActiveCanvas->TCSpanelKeyPressed+= 0x20; // lower case ASCII
    }
}



void cTCScanvas::OnTCSmouseLeft(wxMouseEvent& event)
{
    ActiveCanvas->TCSmouseButtonDown= 1;
    event.GetPosition(&ActiveCanvas->TCSmouseX, &ActiveCanvas->TCSmouseY);
    ActiveCanvas->TCSmouseX= ActiveCanvas->TCSmouseX * TEK_XMAX/tktrnx_.kScrX;
    ActiveCanvas->TCSmouseY= TEK_YMAX - (ActiveCanvas->TCSmouseY * TEK_YMAX/tktrnx_.kScrY);
}



void cTCScanvas::OnTCSmouseMiddle(wxMouseEvent& event)
{
    ActiveCanvas->TCSmouseButtonDown= 4; // same as in DOS-port
    event.GetPosition(&ActiveCanvas->TCSmouseX, &ActiveCanvas->TCSmouseY);
    ActiveCanvas->TCSmouseX= ActiveCanvas->TCSmouseX * TEK_XMAX/tktrnx_.kScrX;
    ActiveCanvas->TCSmouseY= TEK_YMAX - (ActiveCanvas->TCSmouseY * TEK_YMAX/tktrnx_.kScrY);
}


void cTCScanvas::OnTCSmouseRight(wxMouseEvent& event)
{
    ActiveCanvas->TCSmouseButtonDown= 2;
    event.GetPosition(&ActiveCanvas->TCSmouseX, &ActiveCanvas->TCSmouseY);
    ActiveCanvas->TCSmouseX= ActiveCanvas->TCSmouseX * TEK_XMAX/tktrnx_.kScrX;
    ActiveCanvas->TCSmouseY= TEK_YMAX - (ActiveCanvas->TCSmouseY * TEK_YMAX/tktrnx_.kScrY);
}



/*
------------------- Userroutinen: Initialization ------------------------------
*/


extern "C" {
    void winlbl0 (const char PloWinNam[], const char StatWinNam[], const char IniFilNam[])
    {
      size_t iL;
      char* szTemp;
      char  tmpstr[TCS_FILE_NAMELEN], PathSeparator[2];

        iL= strlen(PloWinNam);
        if (iL > (TCS_WINDOW_NAMELEN-1)) iL= TCS_WINDOW_NAMELEN-1;
        if (iL > 0) {
          strncpy( szTCSWindowName, PloWinNam, iL); // Destination is zero-padded
          szTCSWindowName[iL]= '\0'; // just in case iL>= TCS_WINDOW_NAMELEN
        }

        iL= strlen(StatWinNam);
        if (iL > (TCS_WINDOW_NAMELEN-1)) iL= TCS_WINDOW_NAMELEN-1;
        if (iL > 0) {
          strncpy( szTCSstatWindowName, StatWinNam, iL);
          szTCSstatWindowName[iL]= '\0';
        }

        iL= strlen(IniFilNam);
        if (iL > (TCS_FILE_NAMELEN-1)) iL= TCS_FILE_NAMELEN-1;
        if (iL > 0) {
          strncpy( szTCSIniFile, IniFilNam, iL);
          szTCSIniFile[iL]= '\0';
          szTemp= strstr (szTCSIniFile, "@"); // section Level0?
          if (szTemp != 0) {
            strncpy (szTCSsect0, &szTemp[1], iL); // len(szSect0)=TCS_FILE_NAMELEN --> iL o.k.
            szTemp[0]= '\0'; // cut of @Section0 in szTCSIniFile
          }
        }
        iL= strlen(szTCSIniFile); // perhaps shortened by @ processing
        if (iL > 0) {
          szTemp= strstr (szTCSIniFile, INIFILEXTTOKEN); // Default extension?
          if (szTemp != 0) {
            strncpy (szTemp, INIFILEXT, iL); // Sideeffect: szTCSIniFile with extension
            szTCSIniFile[TCS_FILE_NAMELEN-1]= '\0'; // just in case...
          }
        }
        iL= strlen(szTCSIniFile); // perhaps extended by .% processing
        if (iL > 0) {
          szTemp= strstr (szTCSIniFile, PROGDIRTOKEN); // Default ProgDir?
          if (szTemp != 0) {
            strncpy (tmpstr, szTCSIniFile, TCS_FILE_NAMELEN);
            strncpy (szTCSIniFile, wxStandardPaths::Get().GetDataDir(), TCS_FILE_NAMELEN);
            PathSeparator[0]= wxFileName::GetPathSeparator();
            PathSeparator[1]= char (0);
            strncpy (&szTCSIniFile[iL], PathSeparator, TCS_FILE_NAMELEN-iL);
            iL= strlen(szTCSIniFile);
            strncpy (&szTCSIniFile[iL], &tmpstr[strlen(PROGDIRTOKEN)], TCS_FILE_NAMELEN-iL);
            szTCSIniFile[TCS_FILE_NAMELEN-1]= '\0'; // just in case...
          }
        }
     }
}



extern "C" {
    bool WINSELECT (wxWindowID* iD)
    {
      size_t numbytes;

        if (*iD >= MAX_OPEN_CANVAS) {
          TCSGraphicError (WRN_INI2," ");
          return true; // Error handling !?
        } else {
          if (ActiveCanvas != nullptr) { // already active -> save status
            numbytes= sizeof (struct TKTRNX); // save TKTRNX
            memmove (&ActiveCanvas->TekSav.khomey, &tktrnx_.khomey, numbytes);
            numbytes= sizeof (struct G2dAG2); // save AG2
            memmove (&ActiveCanvas->AG2Sav.cline, &g2dag2_.cline, numbytes);

            ActiveCanvas->DefaultLinColSav = TCSDefaultLinCol;
            ActiveCanvas->DefaultTxtColSav = TCSDefaultTxtCol;
            ActiveCanvas->DefaultBckColSav = TCSDefaultBckCol;
            memmove (ActiveCanvas->HardcopyFileSav, szTCSHardcopyFile, TCS_FILE_NAMELEN);
            memmove (ActiveCanvas->sect0Sav, szTCSsect0, TCS_FILE_NAMELEN);
          }
          if (OpenCanvases[*iD] != nullptr) { // restore TKTRNX
            numbytes= sizeof (struct G2dAG2);
            memmove (&tktrnx_.khomey, &OpenCanvases[*iD]->TekSav.khomey, numbytes);
            numbytes= sizeof (struct G2dAG2);
            memmove (&g2dag2_.cline, &OpenCanvases[*iD]->AG2Sav.cline, numbytes);

            TCSDefaultLinCol = OpenCanvases[*iD]->DefaultLinColSav;
            TCSDefaultTxtCol = OpenCanvases[*iD]->DefaultTxtColSav;
            TCSDefaultBckCol = OpenCanvases[*iD]->DefaultBckColSav;
            memmove (szTCSHardcopyFile,&OpenCanvases[*iD]->HardcopyFileSav, TCS_FILE_NAMELEN);
            memmove (szTCSsect0, &OpenCanvases[*iD]->sect0Sav, TCS_FILE_NAMELEN);
          }
          ActiveCanvasID= *iD;
          ActiveCanvas= OpenCanvases[*iD];
       }
        return (OpenCanvases[*iD] == nullptr);
    }
}


extern "C" {
    void initt1 (int iMode, wxFrame* parent, wxFrame* FrameToUse, wxStatusBar* StatusBarToUse)
    {
       wxSize UseScreen;
       xJournalEntry_typ * xJournalEntry;

         PresetProgPar(); // restore initialization after finitt()
         XMLreadProgPar (szTCSIniFile);
         CustomizeProgPar (); // substitute %: with program directory
         initt0(); // initialize COMMON TKTRNX

         if (ActiveCanvas != NULL) { // Reset journal
           SGLIB_DL_LIST_MAP_ON_ELEMENTS (xJournalEntry_typ, ActiveCanvas->xTCSJournal,
                          xJournalEntry,previous,next, { free (xJournalEntry);}); // free all
           ActiveCanvas->xTCSJournal= NULL;
           xJournalEntry= (xJournalEntry_typ *)  malloc (sizeof (xJournalEntry_typ));
           if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUCREATE,"");
           xJournalEntry->action=  XACTION_NOOP; // mark  beginning of the list with NOOP
           xJournalEntry->i1= 0;
           xJournalEntry->i2= 0;
           SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
           xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
           if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
           xJournalEntry->action=  XACTION_INITT;
           xJournalEntry->i1= 0;
           xJournalEntry->i2= 0;
           SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
           return; // Remaining reset will be done during redraw due to XACTION_INITT
         }

         ActiveCanvas = new cTCScanvas (iMode, parent, FrameToUse, StatusBarToUse);
         OpenCanvases[ActiveCanvasID] = ActiveCanvas;

         ActiveCanvas->TCSpen = wxPen(TCSColorTable[tktrnx_.iLinCol], TCS_LINEWIDTH, wxPENSTYLE_SOLID);
         ActiveCanvas->TCSbrush = wxBrush (TCSColorTable[tktrnx_.iBckCol], wxBRUSHSTYLE_SOLID);
         ActiveCanvas->TCSfont = wxFont(wxFONTSIZE_MEDIUM, wxFONTFAMILY_TELETYPE,
                                        wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false);

         UseScreen = ActiveCanvas->TCSpanel->GetClientSize ();
         tktrnx_.kversz = (int) (TEK_YMAX *TCS_REL_CHR_HEIGHT +0.5); // first guess
         tktrnx_.khorsz = (int) ((float)UseScreen.y/(float)UseScreen.x*(float)tktrnx_.kversz +0.5);
         ActiveCanvas->TCSfont.SetFractionalPointSize (TEK_YMAX *TCS_REL_CHR_HEIGHT);

         tktrnx_.khomey= TEK_YMAX - tktrnx_.kversz;
         tktrnx_.kbeamx = tktrnx_.klmrgn;  //  call HOME
         tktrnx_.kbeamy = tktrnx_.khomey;

         ActiveCanvas->TCSframe->Show();

         //  Logging Window

         ActiveCanvas->logWindow = new wxLogWindow(ActiveCanvas->TCSframe, szTCSstatWindowName, false, false);
         wxLog::SetActiveTarget(ActiveCanvas->logWindow);
         wxLog::SetTimestamp(""); // don't write timestamps before messages
         wxLogStatus (""); // without a first message wxLog::show() will crash

         //  Create journal

         ActiveCanvas->xTCSJournal = (xJournalEntry_typ*) NULL;
         wxLogDebug ( wxT("INITT1> xTCSJournal initialisiert: Ptr= %p"), ActiveCanvas->xTCSJournal);

         xJournalEntry= (xJournalEntry_typ *)  malloc (sizeof (xJournalEntry_typ));
         if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUCREATE,"");
#ifdef TRACE_CALLS
         wxLogDebug ( wxT("INITT1> Nach 1. malloc: xJournalEntry: Ptr= %p"), xJournalEntry);
#endif // TRACE_CALLS

         xJournalEntry->action=  XACTION_NOOP; // mark  beginning of the list with NOOP
         xJournalEntry->i1= 0;
         xJournalEntry->i2= 0;
         SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
#ifdef TRACE_CALLS
         wxLogDebug ( wxT("INITT1> LIST_ADD=Create Journal: xTCSJournal: Ptr= %p / xJournalEntry: Ptr= %p"), ActiveCanvas->xTCSJournal, xJournalEntry);
         wxLogDebug ( wxT("INITT1> previous: Ptr= %p / next: Ptr= %p"), xJournalEntry -> previous, xJournalEntry -> next);
         wxLogDebug ( wxT("INITT1> XACTION_??? = %i (i1= %i, i2= %i)"), xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );
#endif // TRACE_CALLS

         xJournalEntry= (xJournalEntry_typ *)  malloc (sizeof (xJournalEntry_typ));
         if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUENTRY,"");
         xJournalEntry->action=  XACTION_INITT;
         xJournalEntry->i1= 0;
         xJournalEntry->i2= 0;
         SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
#ifdef TRACE_CALLS
         wxLogDebug ( wxT("INITT1> Nach 2. LIST_ADD=Create Journal: xTCSJournal: Ptr= %p / xJournalEntry: Ptr= %p"), ActiveCanvas->xTCSJournal, xJournalEntry);
         wxLogDebug ( wxT("INITT1> previous: Ptr= %p / next: Ptr= %p"), xJournalEntry -> previous, xJournalEntry -> next);
         wxLogDebug ( wxT("INITT1> XACTION_??? = %i (i1= %i, i2= %i)"), xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2 );
#endif // TRACE_CALLS

         return;
    }
}



extern "C" {
    void FINITT (int* ix, int* iy)
    {
      cTCScanvas* CanvasToKill;
      xJournalEntry_typ * xJournalEntry;

        if (ActiveCanvas == NULL) return;
        CanvasToKill = ActiveCanvas; // Window could be changed due to user action
        do {
          if (ActiveCanvas == CanvasToKill) {
            TCSGraphicError (ERR_EXIT,""); // User can accept or change window here
          } else {
            wxYield(); // Allow processing in case of a changed window
          }
        } while (ActiveCanvas != CanvasToKill); // Don't kill a wrong window

        SGLIB_DL_LIST_MAP_ON_ELEMENTS (xJournalEntry_typ, ActiveCanvas->xTCSJournal,
                          xJournalEntry,previous,next, { free (xJournalEntry);}); // free all
        ActiveCanvas->xTCSJournal= nullptr;

        ActiveCanvas->TCSframe->Destroy();
        ActiveCanvas = nullptr;
        OpenCanvases[ActiveCanvasID] = nullptr;

        return;
    }
}



extern "C" {
    void IOWAIT (int* iWait)
    {
        ActiveCanvas->TCSpanel->Refresh(); // wxEVT_PAINT will be executed after wxYield()
        wxYield();                   // process event loop -> be aware of recursive loops!
    }
}



/*
------------------- TCS API: Drawing -------------------------------------
*/



extern "C" {
    void swind1_ (int* ix1, int* iy1, int* ix2, int* iy2)
    {
      xJournalEntry_typ * xJournalEntry;

        ActiveCanvas->ClippingNotActive = (*ix1==0) && (*iy1==0) &&
                                         (*ix2==TEK_XMAX) && (*iy2==TEK_YMAX);
        /* Same meaning of bool variable in DOS, SDL2 ... */

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_CLIP;
        if (ActiveCanvas->ClippingNotActive) {
          xJournalEntry->i1= 0;
        } else {
          xJournalEntry->i1= 1;
        }
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        if (!ActiveCanvas->ClippingNotActive) {
          xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
          if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
          xJournalEntry->action=  XACTION_CLIP1;
          xJournalEntry->i1= *ix1;
          xJournalEntry->i2= *iy1;
          SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

          xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
          if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
          xJournalEntry->action=  XACTION_CLIP2;
          xJournalEntry->i1= *ix2;
          xJournalEntry->i2= *iy2;
          SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
        }
    }
}



extern "C" {
    void ERASE (void)
    {
      xJournalEntry_typ * xJournalEntry;

        SGLIB_DL_LIST_MAP_ON_ELEMENTS (xJournalEntry_typ, ActiveCanvas->xTCSJournal,
                          xJournalEntry,previous,next, {free (xJournalEntry);}); // free all
        ActiveCanvas->xTCSJournal= NULL; // create new journal

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_NOOP; // root element without predecessor;
        xJournalEntry->i1= 0;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_LINCOL;
        xJournalEntry->i1= tktrnx_.iLinCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_TXTCOL;
        xJournalEntry->i1= tktrnx_.iTxtCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_BCKCOL;
        xJournalEntry->i1= tktrnx_.iBckCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_ERASE;
        xJournalEntry->i1= 0;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void MOVABS (int* ix,int* iy)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.kbeamx= *ix;
        tktrnx_.kbeamy= *iy;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_MOVABS;
        xJournalEntry->i1= *ix;
        xJournalEntry->i2= *iy;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void DRWABS (int* ix,int* iy)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.kbeamx= *ix;
        tktrnx_.kbeamy= *iy;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_DRWABS;
        xJournalEntry->i1= *ix;
        xJournalEntry->i2= *iy;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void DSHABS (int* ix,int* iy, int* iMask)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.kbeamx= *ix;
        tktrnx_.kbeamy= *iy;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_DSHSTYLE;
        xJournalEntry->i1= *iMask;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_DSHABS;
        xJournalEntry->i1= *ix;
        xJournalEntry->i2= *iy;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void PNTABS (int* ix,int* iy)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.kbeamx= *ix;
        tktrnx_.kbeamy= *iy;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_PNTABS;
        xJournalEntry->i1= *ix;
        xJournalEntry->i2= *iy;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void BCKCOL (int* iCol)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.iBckCol= *iCol;
        if (*iCol > MAX_COLOR_INDEX) tktrnx_.iBckCol= MAX_COLOR_INDEX;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_BCKCOL;
        xJournalEntry->i1= tktrnx_.iBckCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void LINCOL (int* iCol)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.iLinCol= *iCol;
        if (*iCol > MAX_COLOR_INDEX) tktrnx_.iLinCol= MAX_COLOR_INDEX;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_LINCOL;
        xJournalEntry->i1= tktrnx_.iLinCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void TXTCOL (int* iCol)
    {
      xJournalEntry_typ * xJournalEntry;

        tktrnx_.iTxtCol= *iCol;
        if (*iCol > MAX_COLOR_INDEX) tktrnx_.iTxtCol= MAX_COLOR_INDEX;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_TXTCOL;
        xJournalEntry->i1= tktrnx_.iTxtCol;
        xJournalEntry->i2= 0;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}


extern "C" {
    void DEFAULTCOLOUR (void)
    {
      lincol_ (&TCSDefaultLinCol);
      txtcol_ (&TCSDefaultTxtCol);
      bckcol_ (&TCSDefaultBckCol);
    }
}



/*
------------------- TCS API: Graphic text output ----------------------------------
*/



extern "C" {
    void outgtext_ (char strng[] )
    {
      int i, iL;
      struct xJournalEntry_typ    * xJournalEntry;

        iL= strlen(strng);
        tktrnx_.kbeamx+= iL*tktrnx_.khorsz;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_GTEXT;
        xJournalEntry->i1= iL;
        xJournalEntry->i2= strng[0];
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)

        i= 1;
        while (i < iL) {
          xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
          if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
          xJournalEntry->action=  XACTION_ASCII;
          xJournalEntry->i1= strng [i++];
          if ( i<iL ) {
            xJournalEntry->i2= strng[i++];
          } else {
            xJournalEntry->i2= 0;
          }
          SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
        }

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_MOVABS;
        xJournalEntry->i1= tktrnx_.kbeamx;
        xJournalEntry->i2= tktrnx_.kbeamy;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void ITALIC (void)
    {
      struct xJournalEntry_typ    * xJournalEntry;

        tktrnx_.kitalc = 1;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_FONTATTR;
        xJournalEntry->i1= tktrnx_.kitalc;
        xJournalEntry->i2= tktrnx_.ksizef;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void ITALIR (void)
    {
      struct xJournalEntry_typ    * xJournalEntry;

        tktrnx_.kitalc = 0;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_FONTATTR;
        xJournalEntry->i1= tktrnx_.kitalc;
        xJournalEntry->i2= tktrnx_.ksizef;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void DBLSIZ (void)
    {
      struct xJournalEntry_typ    * xJournalEntry;

        if (tktrnx_.ksizef == 0) {
          tktrnx_.khorsz = tktrnx_.khorsz * 2;
          tktrnx_.kversz = tktrnx_.kversz * 2;
          tktrnx_.khomey= TEK_YMAX - tktrnx_.kversz;
        }
        tktrnx_.ksizef = 1;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_FONTATTR;
        xJournalEntry->i1= tktrnx_.kitalc;
        xJournalEntry->i2= tktrnx_.ksizef;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



extern "C" {
    void NRMSIZ (void)
    {
      struct xJournalEntry_typ    * xJournalEntry;


        if (tktrnx_.ksizef == 1) {
          tktrnx_.khorsz = tktrnx_.khorsz / 2;
          tktrnx_.kversz = tktrnx_.kversz / 2;
          tktrnx_.khomey= TEK_YMAX - tktrnx_.kversz;
        }
        tktrnx_.ksizef = 0;

        xJournalEntry= (xJournalEntry_typ *) malloc (sizeof (xJournalEntry_typ));
        if (xJournalEntry == NULL) TCSGraphicError (WRN_JOUADD,"");
        xJournalEntry->action=  XACTION_FONTATTR;
        xJournalEntry->i1= tktrnx_.kitalc;
        xJournalEntry->i2= tktrnx_.ksizef;
        SGLIB_DL_LIST_ADD (xJournalEntry_typ, ActiveCanvas->xTCSJournal, xJournalEntry, previous, next)
    }
}



/*
------------------- TCS API: Messages ----------------------------------
*/



extern "C" {
    void BELL (void)
    {
        wxBell();
    }
}



extern "C" {
    void outtext_ (char strng[] )
    {
      if (ActiveCanvas != nullptr) {
        if (ActiveCanvas->TCSstatusBar != nullptr) {
          ActiveCanvas->TCSstatusBar->SetStatusText(strng);
        }
      }
    }
}



extern "C" {
    void TCSGraphicError (int iErr, const char* msg)
    {
      char cBuf[TCS_MESSAGELEN];
      int i; // Dummyparameter

        snprintf( cBuf, TCS_MESSAGELEN, szTCSErrorMsg[iErr], msg );
        if (ActiveCanvas == nullptr) {
          wxLogStatus (cBuf); // TCS not initialized
        } else {
            if (ActiveCanvas->TCSstatusBar == nullptr) {
              wxLogStatus (cBuf); // no own space for logging
            } else {
               if (TCSErrorLev[iErr] > 0) {
                 wxBell ();
                 ActiveCanvas->TCSstatusBar->SetStatusText(cBuf);
                 if (TCSErrorLev[iErr] < 5) return;
                 if ((TCSErrorLev[iErr] == 5) || (TCSErrorLev[iErr] == 10)) {
                   tinput_ (&i); // Press Any Key
                   ActiveCanvas->TCSstatusBar->SetStatusText("");
                 } else if ((TCSErrorLev[iErr]==8) || (TCSErrorLev[iErr]==12)) {
                    wxMessageBox (cBuf, szTCSstatWindowName, wxOK||wxICON_ERROR, ActiveCanvas->TCSpanel,wxDefaultCoord);
                 }
                 if (TCSErrorLev[iErr] < 10) return;
                 if (iErr != ERR_EXIT) { // Error-Level of finitt() can be changed by XML-Initfile
                   finitt_ (&i,&i);        // Forced exit for all Levels >= 10 over finitt()
                 }
               }
            }
        }
    }
}



/*
 ------------------- TCS API: User Input ---------------------------------
*/



 extern "C" {
    void DCURSR (int *ic,int* ix,int* iy)
    {
        ActiveCanvas->TCSmouseButtonDown= 0; // don't use old mouseclicks
        ActiveCanvas->TCSpanelKeyPressed= 0; // or old keystrokes
        ActiveCanvas->TCSpanel->Refresh();   // wxEVT_PAINT will be executed after wxYield()
        ActiveCanvas->TCSpanel->SetFocus();
        do {
          wxYield(); // process event loop -> be aware of recursive loops!
          wxMilliSleep(100); // wait for MOUSE_BUTTON_DOWN event
        } while ((ActiveCanvas->TCSmouseButtonDown == 0) && (ActiveCanvas->TCSpanelKeyPressed == 0));
        *ic= ActiveCanvas->TCSmouseButtonDown;
        if (*ic == 0) {
          *ic= ActiveCanvas->TCSpanelKeyPressed;
        }
        *ix= ActiveCanvas->TCSmouseX;
        *iy= ActiveCanvas->TCSmouseY;
    }
 }



 extern "C" {
    void TINPUT (int *ic)
    {
        ActiveCanvas->TCSpanelKeyPressed= 0; // don't use old keystrokes
        ActiveCanvas->TCSpanel->Refresh();   // wxEVT_PAINT will be executed after wxYield()
        ActiveCanvas->TCSpanel->SetFocus();
        do {
          wxYield(); // process event loop -> be aware of recursive loops!
          wxMilliSleep(100); // wait for KEY_DOWN event
        } while (ActiveCanvas->TCSpanelKeyPressed == 0);
        *ic= ActiveCanvas->TCSpanelKeyPressed;
    }
 }



/*
------------------- TCS API: Hardcopy -------------------------------------
*/



extern "C" {
   void HDCOPY (void)
   {
      wxString FilNam, TmpString;
      wxFile HDCfile;
      struct xJournalEntry_typ *xJournalEntry;

        do {
          FilNam.Printf(szTCSHardcopyFile,iHardcopyCount++);
        } while ((iHardcopyCount < MAX_HDCCOUNT) && (wxFileExists(FilNam)) );
        if (iHardcopyCount >= MAX_HDCCOUNT) {
          TCSGraphicError (WRN_HDCFILOPN, "???"); // no unused filename
        }
        if (!HDCfile.Open (FilNam, wxFile::write, wxS_DEFAULT) ) {
          TCSGraphicError (WRN_HDCFILOPN, FilNam.c_str()); // error during open
        };

        TCSGraphicError (MSG_HDCACT, FilNam.c_str());
        SGLIB_DL_LIST_GET_LAST(xJournalEntry_typ, ActiveCanvas->xTCSJournal, previous, next, xJournalEntry)
        while (xJournalEntry != NULL) {
          TmpString.Printf("%02i#%04i-%03i\n", xJournalEntry->action, xJournalEntry->i1, xJournalEntry->i2);
          if (!HDCfile.Write (TmpString) ) {
            TCSGraphicError (WRN_HDCFILWRT, FilNam.c_str());
          }
          xJournalEntry= xJournalEntry -> previous;
        }
        HDCfile.Close();
    }
}



extern "C" {
   void SVSTAT (char dst[])
   {
     size_t numbytes;
       numbytes= sizeof (struct TKTRNX);
       memmove (dst, &tktrnx_.khomey, numbytes);
   }
}



extern "C" {
   void RESTAT (char src[])
   {
     size_t numbytes;
       numbytes= sizeof (struct TKTRNX);
       memmove (&tktrnx_.khomey, src, numbytes);
       movabs_ (&tktrnx_.kbeamx, &tktrnx_.kbeamy);
   }
}



/*
------------------- subroutine LIB_MOVC3 ------------------------------
      Subroutine is not used here, for downward compatibility only
*/

extern "C" {
   void lib_movc3_ (int *len,char sou[],char dst[])
   {
       memmove (dst, sou, (size_t) *len);
   }
}
