/** ****************************************************************************
\file    TKTRNX.hpp
\brief   wX Port: TCS Common Block TKTRNX
\version 1.0
\author  Dr.-Ing. Klaus Friedewald
\~german
         C Header passend zu TKTRNX.fd
\~english
         C header belonging to TKTRNX.fd
\~

\note
   wX-Version auf Basis der SDL-Version 1.2

***************************************************************************** */

extern "C" {
  extern struct TKTRNX {
    int
     khomey,
     khorsz,kversz,
     kitalc,ksizef,
     klmrgn,krmrgn, kScrX,kScrY,
     kbeamx,kbeamy,
     kminsx,kminsy,kmaxsx,kmaxsy;

    float
     tminvx,tminvy,tmaxvx,tmaxvy,
     trcosf,trsinf,trscal
     ,xfac,yfac,xlog,ylog;
    int
     kStCol,
     iLinCol, iBckCol, iTxtCol;
  } tktrnx_; // use gfortran FTN77 name mangling
}

