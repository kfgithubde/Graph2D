/** ****************************************************************************
\file       TKTRNX.h
\brief      MS Windows Port: TCS Common Block TKTRNX
\version    1.3
\author     (C) 2022 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         C Header passend zu TKTRNX.fd
\~english
         C header belonging to TKTRNX.fd
\~

\~german
\note
   Anpassung an die compilerabhaengige Namenskonvention erfolgt in TCSdSDLc.h
\~english
\note
   Adaption to the compiler specific name convention is done in TCSdSDLc.h
\~

***************************************************************************** */


extern struct TKTRNXcommonBlock {
FTNINT
//           kbaudr,kerror,kgrafl,
     khomey,
//           kkmode,
     khorsz,kversz,
     kitalc,ksizef,
     klmrgn,krmrgn, kScrX,kScrY,
//           ktblsz,khorzt(10),kvertt(10),
     kBeamX,kBeamY,
//           kmovef,kpchar(4),kdasht,
     kminsx,kminsy,kmaxsx,kmaxsy;

FTNREAL
     tminvx,tminvy,tmaxvx,tmaxvy,
//      trealx,trealy,timagx,timagy,
     trcosf,trsinf,trscal
     ,xfac,yfac,xlog,ylog;
FTNINT
    kStCol,
    iLinCol, iBckCol, iTxtCol, iMouse;
} FAR TKTRNX;

