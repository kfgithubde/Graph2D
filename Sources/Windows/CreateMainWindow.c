/** ****************************************************************************
\file       CreateMainWindow.c
\brief      MS Windows Port: Init FTN77 Main  
\version    1.2
\author     (C) 2022 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Erzeugt nur bei Bedarf ein Fenster für das Hauptprogramm
\note
         Die Pointervariablen muessen vom aufrufenden Fortranprogramm
		 ausreichend groß dimensioniert werden, s. TCSinitt.for
\~english
         Only if necessary: creates a main window
\note
         The calling Fortranprogram has to allocate appropriate variables 
		 to receive pointers, q.v. TCSinitt.for
\~

***************************************************************************** */

#if defined(__WATCOMC__) && defined(__WINDOWS__)
 #define NULL 0           // nur win16: Ueberlagern #define NULL ( (void *) 0) 
#endif                    // aus aus stddef.h, string.h...

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <tchar.h>
#include "TCSdWINc.h"   // Unterstuetzung 16/32bit Kompatibilitaet

#if defined(__WATCOMC__) && defined(__SW_BW)
 #include <wdefwin.h>   // Compilerswitch -bw: Watcom Default Window System
#endif

#define WINMAIN_ICON         _T("WinMainIcon")
#define WINMAIN_DEFWINCLASS  _T("WinMainFTN77")

/** ***************************************************************************

\~german
\brief Initialisierung der FTN77 Hauptprogramme    

  Unterprogramm zur Initialisierung von Windows. Erzeugt und zeigt(!) ein 
  Fenster für das Hauptprogramm, falls noch keine Windows-Initialisierung
  anderweitig (z.B. durch den Compiler) vorgenommen wurde. Die Klasse wird
  entsprechend der Konstante WINMAIN_DEFWINCLASS benannt.

  Das Icon kann über ein Resourcefile als WinMainIcon definiert werden.
 
\param[in] hMainProgInst Instanz des Hauptprogrammes
\param[in,out] hMainProgWindow Fenster des Hauptprogrammes
\param[in] szWinName Fenstername des evtl. erzeugten Fensters
\~english

  In case that the compiler has not created a window for the main program,
  this subroutine creates and shows a new main window. The class will be 
  named according to the constant WINMAIN_DEFWINCLASS.

  The window icon can be defined as WinMainIcon by a resource file.
 
\param[in] hMainProgInst Main instance
\param[in,out] hMainProgWindow Main window
\param[in] szWinName Window name in case a main window does not exist
\~


*************************************************************************** */


void CreateMainWindow_IfNecessary (HINSTANCE * hMainProgInst, 
                                   HWND * hMainProgWindow, LPTSTR szWinName)

{    

TCHAR           szClassName [] = WINMAIN_DEFWINCLASS; /* Class Name */
static WNDCLASS wincl;       /* SAVE Data structure for the windowclass */
#if defined(__WIN32__) || defined(_WIN32)
 DWORD           ErrorCode;
 LPVOID          lpMsgBuf;
#endif


    if (*hMainProgWindow == NULL ) { // Hauptprogramm ohne (bekanntes) Fenster
     
     /* Create MainWindow */

     wincl.hInstance = *hMainProgInst;
     wincl.lpszClassName = szClassName;
     wincl.lpfnWndProc = DefWindowProc;      /* keine eigene Windowsroutine */
     wincl.style =  CS_DBLCLKS;              /* Catch double-clicks */

     wincl.hIcon = LoadIcon (*hMainProgInst, WINMAIN_ICON);
     wincl.hCursor = NULL;
     wincl.lpszMenuName = NULL;    // No menu
     wincl.cbClsExtra = 0;         // No extra bytes after the window class
     wincl.cbWndExtra = 0;         // structure or the window instance
     wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

     /* Register the window class. Fail: most probable UNICODE on win98 */
     if (!RegisterClass (&wincl)) {
      #if defined(__WIN32__) || defined(_WIN32)
       ErrorCode= GetLastError(); // win32-Funktion
//     if (ErrorCode == ERROR_CLASS_ALREADY_EXISTS) {
//      Hier bei Bedarf Fehlerbehandlung einführen
//     } else {                      
        FormatMessage(
         FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
         NULL,
         ErrorCode,
         MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
         (LPTSTR) &lpMsgBuf,
         0,
         NULL
        );
        MessageBox (NULL, lpMsgBuf,_T("Error in CreateMainWindow"), MB_ICONSTOP);
        LocalFree( lpMsgBuf ); // Free the buffer
//     } // Ende der Fehlerbehandlung
      #else // rudimentaere Fehlerbehandlung 16bit Windows
       MessageBox (NULL, _T("Window Class not registered"),
                            _T("Error in CreateMainWindow"), MB_ICONSTOP);
      #endif
      return;
     }
     
     /* The class is registered, let's create the program */
     *hMainProgWindow = CreateWindow (
       szClassName,                  // Classname
       szWinName,                    // Title Text
       WS_POPUPWINDOW | WS_DISABLED, // disabled -> Prozessverwaisung verhindern
       CW_USEDEFAULT,                // Windows decides the position
       CW_USEDEFAULT,                // of the Window
       0,                            // The programs width
       0,                            // and height in pixels
       HWND_DESKTOP,                 // Parent: desktop
       NULL,                         // No menu
       *hMainProgInst,               // Program Instance handler
       NULL                          // No Window Creation data
     );
     ShowWindow (*hMainProgWindow, SW_SHOW);
    } else {    // Mainwindow bereits vorhanden
     #if defined(__WATCOMC__) && defined(__SW_BW)
      _dwSetAppTitle (szWinName);    // Fenstername Watcom Default Window
     #endif
    }
}

