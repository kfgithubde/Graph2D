/** ****************************************************************************
\file    TCSdrWXcpp.hpp
\brief   WX Port: Headerfile
\version 0.5
\author  Dr.-Ing. Klaus Friedewald
\~german
         Headerfile zu TCSdrWXcpp.cpp
\note
          - Konfiguration der Bibliothek
          - Definition der Defaultwerte
\~english
         Headerfile for TCSdrWXcpp.cpp
\note
          - Configuration of the library
          - Defining default values
\~

**************************************************************************** **/



/* -------------- Drawing area in Tektronix coordinates --------------------- */

#define TEK_XMAX 1023.0  // Double precision because of
#define TEK_YMAX 780.0   // use in wx::SetLogicalScale ()



/* -------------- Program parameters ----------------------------------------- */

#define TCS_LINEWIDTH 1
#define MAX_OPEN_CANVAS 20         // Maximum number of used canvases

#define STAT_MAXROWS 1             // Analogue to the other ports, not used here

#define TCS_REL_CHR_HEIGHT 0.018f  // Define size / vertical spacing of graphic text
#define TCS_REL_CHR_SPACING 0.7f

#define TCS_WINDOW_NAMELEN 50
#define TCS_FILE_NAMELEN 132

#define TCS_MESSAGELEN 132
#define MAX_HDCCOUNT 1000          // parameter is bound to TCS_HDCFILE_NAME

#define TCS_INIFILE_NAME ""
#define INIFILEXT ".XML"
#define INIFILEXTTOKEN ".%"        // Token for parsing filenames
#define PROGDIRTOKEN "%:"



/* Actioncodes of the journalfiles */

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
#define XACTION_CLIP        15
#define XACTION_CLIP1       16
#define XACTION_CLIP2       17



/* Assign errornumbers */

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



/* Default initialization, can be changed by the ini-XML file */

#define TCS_INISECT0 "Graph2D" // Root-Section for XML, change with WINLBL()

#define TCS_INISECT1 "Names"
 #define TCS_INIVAR_WINNAM "G2dGraphic"
    #define TCS_WINDOW_NAME "Graphics"
 #define TCS_INIVAR_STATNAM "G2dStatus"
    #define TCS_STATWINDOW_NAME "System Messages"
 #define TCS_INIVAR_HDCNAM "G2dHardcopy"
    #define TCS_HDCFILE_NAME "HDC%03i.HDC"



#define TCS_INISECT2 "Layout"
/* #define TCS_INIVAR_COPMEN "G2dSysMenuCopy"
    #define TCS_INIDEF_COPMEN "Copy"
 #define TCS_INIVAR_FONT "G2dGraphicFont"
    #define TCS_INIDEF_FONT PROGDIRTOKEN "graph2d"
 #define TCS_INIVAR_SYSFONT "G2dSystemFont"
    #define TCS_INIDEF_SYSFONT PROGDIRTOKEN "graph2d"
*/
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
    #define TCS_INIDEF_STATSIZY 3

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
    #define TCS_INIDEF_XMLOPENL 1
 #define TCS_INIVAR_UNKNAUDIO "G2dAudio"
    #define TCS_INIDEF_UNKNAUDIO "GRAPH2D Audio System: Error %s."
    #define TCS_INIVAR_UNKNAUDIOL "G2dAudioL"
    #define TCS_INIDEF_UNKNAUDIOL 5
 #define TCS_INIVAR_USR2 "G2dUser2"
    #define TCS_INIDEF_USR2 "%s"
    #define TCS_INIVAR_USR2L "G2dUser2L"
    #define TCS_INIDEF_USR2L 5
 #define TCS_INIVAR_INI2 "G2dInitt"
    #define TCS_INIDEF_INI2 "Error creating windows in subroutine INITT"
    #define TCS_INIVAR_INI2L "G2dInittL"
    #define TCS_INIDEF_INI2L 1
