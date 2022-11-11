/** ****************************************************************************
\file       GetMainInstance.c
\brief      MS Windows Port: Get Main Window and Instance
\version    1.5
\author     (C) 2022 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Ermittlung Instanz und Fenster der FTN77 Hauptprogramme
\~english
         Get Instance and Window of the FTN77 Main Program
\~

***************************************************************************** */


#if defined(__WATCOMC__) && defined(__WINDOWS__)
 #define NULL 0           // nur win16: Ueberlagern #define NULL ( (void *) 0)
#endif                    // aus aus stddef.h, string.h...



#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>



/*
-------------------------- Externe Bezüge ------------------------------
*/

#ifdef __WATCOMC__     // Bis 11.0c: WATCOM Fortran Default Window System 10.0
 #if (__WATCOMC__ == 1100) // Source OpenWatcom 0.8, bld\clib\defwin\c bzw. \h
  extern HWND _MainWindow; // winglob.c, wmain.c, winmain.c, win.h
  #define EXTERN_WINDOW _MainWindow
  #undef EXTERN_INSTANCE
 #elif (__WATCOMC__ >= 1200)                 // Open Watcom 1.0 bis 1.9:
  #if (!defined(__WIN32__) && !defined(_WIN32))         // 16bit-Windows
   #ifndef __SW_BW
    #error 16bit Windows requieres Default Window System, use the /bw switch
   #else
    extern HWND _MainWindow;     // Open Watcom Default Window System 1.0
    #define EXTERN_WINDOW _MainWindow
    #undef EXTERN_INSTANCE
   #endif
  #else              // 32bit-Windows: Default Window System deaktiviert
   #if defined (__SW_BW)
    #pragma message ("OpenWatcom >=1.0: Default Window System disabled!")
    #undefine __SW_BW
   #endif
   HWND _TCSMainWindow= NULL;
   #define EXTERN_WINDOW _TCSMainWindow
   #undef EXTERN_INSTANCE
  #endif
  #if (__WATCOMC__ > 1300)
   #pragma message ("New Compiler. Check if _MainWindow is defined")
   #pragma message (" (in bld\clib\defwin\c\winglob.c to compile for win16)")
   #pragma message (" Status V2.0 (__WATCOMC__ = 1300): unmodified since 3 years")
  #endif
 #else
  #pragma message ("Untested Compiler.") // Alte kommerzielle Compilerversionen
  HWND _TCSMainWindow= NULL;    // Ohne Default Window System?
  #define EXTERN_WINDOW _TCSMainWindow
  #undef EXTERN_INSTANCE
 #endif
 #pragma aux GetMainInstAndWin "^";     // fuer DLL: Fenster muss im Haupt-
 #pragma aux SaveMainInstAndWin "^";    // programm gespeichert werden
#endif

#ifdef __GNUC__              // MinGW und GNU:
 #if __GNUC__<4 // bis GCC 4.0 Verwendung von g77, ab 4.0 gfortran
  extern HINSTANCE _MainInst; // Symbole werden durch das (selbstgeschriebene)
  extern HWND _MainWindow;    // WinMain.c erzeugt und belegt
 #else // gfortran: Init WinMain durch Constructor, nicht libfrtbegin
  static HINSTANCE _MainInst; // Falls von mehreren Bibliotheken(TekLib,ProcInp)
  static HWND _MainWindow;    // verwendet wird nur 1 Instanz gelinkt
 #endif
 #define EXTERN_INSTANCE _MainInst
 #define EXTERN_WINDOW _MainWindow
 #define GetMainInstAndWin getmaininstandwin_
 #define SaveMainInstAndWin savemaininstandwin_
#endif

#ifdef _MSC_VER         // Microsoft Visual Cpp 6.0, ungeprueft da ohne FTN
 extern HINSTANCE hInst;
 #define EXTERN_INSTANCE hInst
 #define EXTERN_WINDOW HWND_DESKTOP
#endif



/** **************************************************************************

 \~german
 \brief  Ermittlung Instanz und Fenster der FTN77 Hauptprogramme

  Es muss in jedem Fall zu dem Hauptprogramm gelinkt werden und darf sich
  nicht in einer DLL befinden, da sonst die Instanz der DLL ermittelt wird!
  Das Unterprogramm ist von Fortran aufrufbar.

  \param[out] hMainProgInst Instanz des Hauptprogrammes
  \param[out] hMainProgWindow Fenster des Hauptprogrammes
         Ermittlung Instanz und Fenster der FTN77 Hauptprogramme
 \~english
 \brief  Determination of instance and window of FTN77 main programs

  This routine has to be linked to the main program under all circumstances.
  In case of beeing part of a DLL, the instance handle of the DLL would be returned!
  The routine is fortran-callable.

  \param[out] hMainProgInst instance of main
  \param[out] hMainProgWindow window of main
 \~
 
************************************************************************** **/


void GetMainInstAndWin (HINSTANCE * hMainProgInst, HWND * hMainProgWindow)

{
    #if defined EXTERN_WINDOW
     *hMainProgWindow= EXTERN_WINDOW;
    #else
     *hMainProgWindow= NULL;  // wird bei Bedarf spaeter erzeugt
    #endif

    #if defined EXTERN_INSTANCE
     *hMainProgInst= EXTERN_INSTANCE;
    #else
     *hMainProgInst= NULL;
    #endif

    if (*hMainProgInst == NULL) {
     #if defined EXTERN_WINDOW
      if (EXTERN_WINDOW != NULL ) { // Hauptprogramm besitzt (bekanntes) Fenster
       #if defined __WATCOMC__      // Watcom Default Window System 16/32 bit
        #if (!defined(__WIN32__) && !defined(_WIN32))
         *hMainProgInst= (HINSTANCE)GetWindowWord(EXTERN_WINDOW, GWW_HINSTANCE);
        #else                       // Watcom ohne 64bit Windows
         *hMainProgInst= (HINSTANCE)GetWindowLong(EXTERN_WINDOW, GWL_HINSTANCE);
        #endif
       #else                        // alle anderen Compiler ohne 16bit Windows
        #if (!defined(_WIN64))      // 32 bit
         *hMainProgInst= (HINSTANCE)GetWindowLong(EXTERN_WINDOW, GWL_HINSTANCE);
        #else                       // 64 bit
        *hMainProgInst= (HINSTANCE)GetWindowLongPtr(EXTERN_WINDOW, GWLP_HINSTANCE);
        #endif
       #endif
      } else { // kein offenes Fenster, z.B. Watcom-Consolenanwendung
       *hMainProgInst= GetModuleHandle (NULL);
      }
     #else      // kein Fenster ermittelbar
      *hMainProgInst= GetModuleHandle (NULL);
     #endif
    }
}

/** ***************************************************************************

 \~german
 \brief Aktualisierung globalen Speichervariablen Hauptinstanz und Hauptfenster.

 Notwendig nach Aufruf von CreateMainWindow_IfNecessary, da dort evtl. ein neues
 Fensterhandle erzeugt wird. Da sich das Unterprogramm im Modul des Hauptprogrammes
 befindet, kann das Erzeugen des Fensters auch durch eine DLL erfolgen.

 \param[in] hMainProgInst Instanzenhandle
 \param[in] hMainProgWindow Fensterhandle
 \~english
 \brief  Update the global variables containing instance and window of main

  Necessary after invoking CreateMainWindow_IfNecessary, where a new window handle
  could be created. The creation of a new window could be done by a DLL-based routine.

  \param[in] hMainProgInst instance of main
  \param[in] hMainProgWindow window of main
 \~

*************************************************************************** **/


void SaveMainInstAndWin (HINSTANCE * hMainProgInst, HWND * hMainProgWindow)

{
    #if defined EXTERN_INSTANCE
     EXTERN_INSTANCE= *hMainProgInst;
    #endif

    #if defined EXTERN_WINDOW
     EXTERN_WINDOW= *hMainProgWindow;
    #endif
}
