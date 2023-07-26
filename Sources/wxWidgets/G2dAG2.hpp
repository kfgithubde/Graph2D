/** ****************************************************************************
\file    G2dAG2.hpp
\brief   wX Port: AG2 Common Block G2dAG2
\version 0.1
\author  Dr.-Ing. Klaus Friedewald
\~german
         C++ Header passend zu G2dAG2.fd
\note
   wX-Version, wird nur zur Groessenbestimmung beim sichern verwendet.
   Die Werte werden nur durch ag2.for veraendert.
\~english
         C++ header belonging to G2dAG2.fd
\note
   wX-Version, only used during save in order to determine the block length.
   The values are only changed inside ag2.for.
\~

***************************************************************************** */

extern "C" {
  extern struct G2dAG2 {
    int
     cline,csymbl,csteps;
    float
     cinfin;
    int
     cnpts,cstepl,cnumbr;
	float
	 csizes,csizel;
	int // bool
	 cxyneat[2],cxyzero[2];
	int
	 cxyloc[2],cxylab[2],cxyden[2],cxytics[2],
	 cxylen[2],cxyfrm[2],cxymtcs[2],cxymfrm[2],cxydec[2];
	float
	 cxydmin[2],cxydmax[2];
	int
	 cxysmin[2],cxysmax[2],cxytype[2],
	 cxylsig[2],cxywdth[2],cxyepon[2],
	 cxystep[2],cxystag[2],cxyetyp[2],
	 cxybeg[2],cxyend[2],cxymbeg[2],cxymend[2];
	float
	 cxyamin[2],cxyamax[2];
  } g2dag2_; // use gfortran FTN77 name mangling
}

