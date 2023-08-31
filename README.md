# Graph2D

Graph2D is a software library for creating scientific 2-D plots. The API is backward compatible with the Tektronix Plot 10 and Advanced Graphing II procedures. It currently supports Windows (all versions from 16 to 64 bit), wxWidgets, SDL2, DOS and CP/M (Commodore 128 only).

## Screenshots

You can view screenshots of the application [here](./Doc/Screenshots).

## Documentation

The historic Tektronix manuals contain a complete documentation of the basic API:

* [TCS User Manual](https://archive.org/details/bitsavers_tektronixp0TerminalControlSystem4010UsersManualApr_5470592)
* [TCS System Manual](https://archive.org/details/bitsavers_tektronixp01PLOT10TerminalControlSystemSystemManua_5952860)
* [Advanced Graphing II User Manual](https://archive.org/details/bitsavers_tektronixp0AdvancedGraphingIIUsersManualFeb82_5136332)
* [Advanced Graphing II System Manual](https://archive.org/details/bitsavers_tektronixp02PLOT10AdvancedGraphingIISystemManual19_7978638)

The additional subroutines dealing with windows, colors, hardcopies etc. are described [here](./Doc/API.md). 

## Installation

The /Binaries subdirectory contains ready-to-use libraries, along with examples for initialization and font files. You will also find there executables of some demonstration programs, as described in the manuals.

## Build

Extract the complete directory tree (including the code for all ports) and open a CMD console in the /Sources/Build subdirectory. Window users can use the three batch files getfiles.bat, purge.bat and putfiles.bat. Porting them to bash should be self-explanatory.

Note: The batchfiles are protected against del \*.\* in the console, but not against deletion by the Windows Explorer! To clean up the /Build directory, use  $purge / $del \*.\* in the command terminal. 

Copy the necessary files by executing getfiles.bat and selecting the correct development system. Valid options include:

-	MINGW-GCC for 64/32 bit Windows, wxWidgets and SDL2 libraries  
with Code::Blocks: use Graph2D.workspace
-	Linux-GCC on Raspberry Pi: Copy and compile on the target machine
-	Open Watcom for 32 and 16 bit win libraries (static and/or dynamic)  
Project file: teklib.wpj. To buid the dynamic library, compile the targets using the following sequence: LnkFtn1.lib, graph2d.dll, graph2dd.lib, graph2du.dll.
-	Open Watcom for DOS: Use TekDos.wpj
-	MS-Fortran for CP/M: Copy and compile on the target machine

All libraries and documentation are created in this subdirectory. From here the new library and the Doxygen output should be copied to the /Binaries and /Doc subdirectories by using putfiles.bat.

Also included are some of the test files, as described in the AGII user manual.



## Open content

The sources of the public domain components used are stored in the /OpenContent directory, along with their licenses. Compiled versions and documentation can be found in the subdirectories /OpenContent/binaries and /OpenContent/docs.
