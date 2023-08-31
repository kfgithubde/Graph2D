## Programming Interface

Described here are the extensions of the original AG II library. Port-specific details are documented separately:

* [CP/M](./Doxygen/CPMdox-en.pdf)
* [DOS](./Doxygen/DOSdox-en.pdf)
* [Windows](./Doxygen/WINdox-en.pdf)
* [SDL2 (MinGW and LINUX)](./Doxygen/SDLdox-en.pdf)
* [wX (CodeBlocks/MinGW)](./Doxygen/wXdox-en.pdf)

### Fortran 77 Character Variables

Since the switch from CP/M to DOS, MS-Fortran supports the Fortran 77 standard. This led to some extensions and modifications of the original FORTRAN IV API.

The use of the subroutines remains as described in the manuals. However, direct manipulation of the common block should be avoided, although  the subroutines SAVCOM, RESCOM, IBASEX(0), IBASEY(0) and IOTHER are still supported. The leap year correction LEAP has been extended to a function, in order to isolate the common block:

- integer function LEAP (iyear): =1 in case of a leap year, otherwise =0.

Due to the fact that there is no standard for the implementing of FORTRAN IV holerith variables (4Habcd could be interpreted as packed integer, integer array(4) or as character), the following changes were necessary:

 - Subroutine PLACE (lit): lit is interpreted as integer only, the use of 'STD', 'UPH' is deprecated.
 - The upper and lower index are now indicated by the special characters SOH=char(1) and STX=char(2) instead of -1 and -2.
 
In addition to the subroutines dealing with strings represented in integer arrays, character-based alternatives are available:

- subroutine NUMSETC (fnum,nbase, outstr,fillstr)
- subroutine IFORMC (fnum,iwidth, outstr)
- subroutine FFORMC (fnum,iwidth, idec, outstr)
- subroutine FONLYC (fnum,iwidth,idec, outstr,fillstr)
- subroutine EFORMC (fnum,iwidth,idec, outstr,fillstr)
- subroutine EXPOUTC (nbase,iexp, outstr,fillstr)
- subroutine ALFSETC (fnum,iwidth,labtyp,outstr)
- subroutine NOTATEC (ix,iy, string)
- subroutine VLABLC (string)
- subroutine JUSTERC (string, iPosFlag, iOff)
- subroutine USESETC (fnum, iwidth, nbase, labstr)

<br>

### Line Styles and Colors

The CP/M and DOS-ports use an integer as a 16 bit bitmask to describe the line style. Starting with the win16 implementation, styles are handled in accordance to the Tektronix definition:

- 0: solid
- 1: dotted
- 2: dash-dotted
- 3: dashed
- -1: no Line

The colors are defined as follows:

- 0: background color, black (white)
- 1: primary color, white (black)
- 2: red
- 3: green
- 4: blue
- 5: purple
- 6: yellow
- 7: grey
- 8: violet
- 9: weak red
- 10: weak green
- 11: weak blue
- 12: weak purple
- 13: orange
- 14: weak grey
- 15: weak violet

Colors are set by subroutines LINCOL(iCol), TXTCOL(iCol), BCKCOL(iCol) and DefaultColour() respectively. A new selected background color will not be seen until a call to subroutine ERASE.

<br>

### Hardcopies

- wX: *.hdc (Default), *.bmp or *.jpg - depending on file extension
- SDL2: proprietary ASCII-Journalfile (*.hdc)
- Windows: Windows Metafile (*.wmf) 
- DOS: Bitmap (*.bmp)
- CP/M: Bitmap (*.HDC)

<br>

### Additional Input/Output Routines

#### subroutine toutst (iChar, iString)

Print *iChar* characters of the integer array *iString*. In contrast to subroutine ANSTR, controlcharacters are ignored.

<br>

#### subroutine toutstc (string)

Print the character *string* into the graphic window.

<br>

#### subroutine statst (string)

Print the character *string* into the status window.

<br>

### Utility Routines

#### subroutine winlbl (GraphLbl, StatLbl, Inifile)

character\*(\*) GraphLbl : Label of the graphic window\
character\*(\*) StatLbl : Label of the status/message window\
character\*(\*) IniFile : Name of the configuration file

The windowlabel "~" generates a window without a headline (SDL2 only). The variable "%:" will be expanded to the fully qualified program name (MS Windows and wX).\
IniFile may contain the variables "%:"=program directory, ".%"=default extension, @RootNode(XML-files only), e.g. "%:MyInifile.%@g2d" will be translated to "c:/program/MyInifile.xml" and the XML-parser uses "\<g2d\>" as rootnode.

The wx-Port allows the use of more than one window. To facilitate the call by c, the call by\
 *void winlbl0 (const char GraphicWindowLabel[], const char StatusWindowLabel[], const char IniFilNam[]);*\
is also possible here.

<br>

#### void init1  (iMode, parent, FrameToUse, StatusBarToUse)

int iMode : Type of the new plot canvas\
wxFrame* parent : parent window\
wxFrame* FrameToUse : plot frame\
wxStatusBar* StatusBarToUse\

Create a new plot window (wX only).

<br>

#### logical function WINSELECT (iD)

Selects Window iD for plotting. This call is functional under wX only, otherwise a dummystub. See example wxDemoMain.cpp for further information.

<br>

#### tcslev (iArr), ag2lev (iArr)

integer iArr(3): Returns the version of the software.\
iArr(1)= year, iArr(2) day of release. iArr(3) defines the port:

- 1: CP/M
- 2: MS-DOS
- 3: 16 bit windows
- 4: 32 bit windows
- 5: GCC (MinGW) running on 32bit Windows
- 6: SDL2
- 7: GCC (MinGW) running on 64bit Windows
- 8: wX

<br>

#### GetENV (cBuf, iBufLen)

DOS-only: Gets an enviroment variable. E.g. a call with cBuf= 'PATH='//char(0) will return the path in cBuf and its length in iBufLen.

<br>

#### GraphicError (iErrNo, String)

Outputs the multilingual errormessage #iErrNo, completed with String. The message, together with its severitylevel, can be altered by the "message"-section of the initialization files. The meaning of the severity level is defined as follows:
+ =0: No Message
+ =1: Bell and Message in statuswindow
+ =2: As 1 with an additional Message into the infochannel (SDL2 only)
+ =3: As 1 with a message into the errorchannel (SDL2 only)
+ =5: As 1 and wait for a keystroke
+ =8: Output into a modal infowindow
+ =10: Wait for a keystroke and abort program
+ =12: Output into a modal infowindow and abort program. SDL2 additionally writes into the errrorchannel.

<br>

#### GetHDC (Filename)

Plots the the ASCII-Hardcopyfile of Type 3 (Default in SDL and wX). Can be called as a subroutine or as a logical function. If invoked as a function, the result .false. means success, .true. is returned in case of an error.

<br>

#### ioWait (iDummy)

Originally used to grant Tektronix 4010 Terminals sufficient time in order to execute its commands in case of "fast" Terminal connections (9600 Baud). Now a call to ioWait() in some enviroments forces a redraw of the window.
+ CP/M, DOS, MS-Windows: Dummyroutine
+ SDL2: Forces a FLUSH BACKBUFFER. Rarely needed, because every change of the window and every input refreshes the window.
+ wX: Draws the graph. A call to ioWait() is required at the end of each plot routine that is not terminated by a call to finitt(). 