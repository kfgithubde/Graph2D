/* ***************************************************************************

Nur zur R�ckw�rtskompatibit�t mit dem (veralteten) g77 Compiler erforderlich.
gfortran verwendet dieses Modul nicht mehr.

  WinMain.c    : WinMain f�r GNU-FTN77 Hauptprogramme 
  
  USES: CreateMainWindow.c 
  
  Version 1.3

  Erzeugt und zeigt(!) ein Fenster f�r das FTN-77 Hauptprogramm     
    Das Hauptprogramm wird jetzt nicht mehr mit  
        "c:/Programme/MinGW/lib/libfrtbegin.a"
    sondern nur mit WinMain.o gelinkt!

   F�r die Unterprogramme (z.B. Tektronix etc.) werden die globalen Variablen 
   _MainInst und _MainWindow zur Verf�gung gestellt                          

Falls der Fenstername nicht durch WINMAIN_WINDOWTITLE definiert wird, 
wird der Programmname verwendet. Das Icon kann �ber ein Resourcenfile 
als WinMainIcon definiert werden.

*************************************************************************** */


#include "TCSdWINc.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <tchar.h>

#define WINMAIN_WINDOWTITLE  _T("%:") // Token fuer den Programmnamen
#define CMDLINLEN 255 // L�nge Kommandozeile
#define MAXCMDLINPAR 20 // Maximale Anzahl Kommandozeilenparameter

HINSTANCE _MainInst= NULL; /* Exportierte Symbole f�r die FTN-77 */
HWND _MainWindow= NULL;    /* Unterprogramme, z.B. INITT() etc.  */


void MAIN__ (void);  /* Name des g77 Hauptprogramms */
extern void f_exit(void);

extern void CreateMainWindow_IfNecessary (HINSTANCE * hMainProgInst, 
                                   HWND * hMainProgWindow, LPTSTR szWinName);



int WINAPI WinMain (HINSTANCE hThisInstance, HINSTANCE hPrevInstance, 
                                        LPSTR lpszArgument, int nFunsterStil)
{
LPTSTR szCommandLine;   // Pointer Programmname & Parameter, UNICODEf�hig
char CommandLineCopy[CMDLINLEN]; // Notwendig wegen Einf�gen von \0

int argc;       
char *argv[MAXCMDLINPAR]; // bei Bedarf hier an UNICODE anpassen

char * ptChar;
bool NowBlank, NowQuote; 

/* Initialisierung des Fortran-Systems siehe
   C:\Programme\MinGW\src\gcc-3.2.3-20030504-1\libf2c\libF77\main.c     */

/* Umformatieren der Programmzeile des Windowsaufrufes in Consolenaufruf */

    strncpy (CommandLineCopy,GetCommandLine(),CMDLINLEN);
    CommandLineCopy[CMDLINLEN-1] = '\0';
    szCommandLine= CommandLineCopy; // lpszArgument liefert keinen Programmnamen

    argc= 0; argv[0]= &szCommandLine[0]; // UNICODE m�glich, jedoch folgender  
    ptChar= argv[0]-1;                   // Code -1 statt 2 bei UNICODE
    NowBlank= false; NowQuote= false;
    do {
     ++ptChar;
     if (*ptChar=='"') {
      NowQuote= !NowQuote; // Blanks zwischen Anf�hrungsstrichen
     } else if (*ptChar==' ') {
      NowBlank= !NowQuote; // wie normale Buchstaben behandeln
      if (NowBlank && 
               ((*(ptChar-1)==' ') || (*(ptChar-1)=='"'))) *(ptChar-1)= '\0';
     } else {
      if (NowBlank) {
       if (NowBlank && (*(ptChar-1)==' ')) *(ptChar-1)= '\0';
       NowBlank= false;
       if (argc < MAXCMDLINPAR-1) {
        ++argc;
        argv[argc]= ptChar;
       }
      }
     }
    } while (*ptChar != '\0');
    --ptChar; if (*ptChar == '"') *ptChar= '\0';
    ++argc; // Programmname z�hlt mit

    f_setarg(argc, argv);

/* Rest wie bei Consolenprogrammen */

    f_setsig();
    f_init();
    atexit (f_exit);

/* Initialisierung des Window-Systems */

    _MainInst= hThisInstance; // und �bergabe
    
    szCommandLine= WINMAIN_WINDOWTITLE;
    if (_tcscmp(szCommandLine,_T("%:")) == 0) {
     szCommandLine= GetCommandLine(); // mit Programmnamen (lpszArgument ohne)
    }

    CreateMainWindow_IfNecessary (& _MainInst, & _MainWindow, szCommandLine);    
    
    MAIN__();   // und los gehts

    f_exit();   // Angstaufruf, MAIN__ kehrt i.A. nie zur�ck!
    return 0; 
}

