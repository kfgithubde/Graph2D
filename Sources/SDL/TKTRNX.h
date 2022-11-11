/** ****************************************************************************
\file    TKTRNX.h
\brief   SDL Port: TCS Common Block TKTRNX
\version 1.2
\author  Dr.-Ing. Klaus Friedewald
\~german
         C Header passend zu TKTRNX.fd
\~english
         C header belonging to TKTRNX.fd
\~

\note
   SDL-Version auf Basis der Windows-Version 1.2
   Anpassung an die compilerabhaengige Namenskonvention erfolgt in TCSdSDLc.h

***************************************************************************** */


extern struct TKTRNXcommonBlock {
FTNINT
     khomey,
     khorsz,kversz,
     kitalc,ksizef,
     klmrgn,krmrgn,
     kBeamX,kBeamY,
     kminsx,kminsy,kmaxsx,kmaxsy;

FTNREAL
     tminvx,tminvy,tmaxvx,tmaxvy,
     trcosf,trsinf,trscal
     ,xfac,yfac,xlog,ylog;
FTNINT
    kStCol,
    iLinCol, iBckCol, iTxtCol;
} TKTRNX;
