/** ****************************************************************************
\file      graph2d.h
\brief     C/C++ API definitions
\version   1.2
\author    Dr.-Ing. Klaus Friedewald
\copyright GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         Definitionen fuer in C/C++ geschriebene Anwenderprogramme
\note
         Auch in TCSdrWXcpp.cpp fuer die Forward-Deklarationen verwendet
\~english
         Definitions for user programs, written in C/C++
\note
         Also used by TCSdrWXcpp.cpp as forward declarations
\~

***************************************************************************** */

extern "C" {

// ***************** Routines with string parameters ************************

#ifdef __GNUC__
  #define WINLBL winlbl_
  void winlbl_ (char PloWinNam[], char StatWinNam[], char IniFilNam[], size_t len1, size_t len2, size_t len3);

  #define TOUTSTC toutstc_ // TCS: added character output
  void toutstc_ (char ftnstr[], size_t len);
  #define STATST statst_
  void statst_ (char ftnstr[], size_t len);
  #define GRAPHICERROR graphicerror_
  void graphicerror_ (int* iErr, char msg[], size_t len);

  #define GETHDC gethdc_ // TCS add-on
  void gethdc_ (char FilNam[], size_t len);

  #define NUMSETC numsetc_ // AG2:  added character labeling  routines
  void numsetc_ (float* f, int* iw, int* ib, char ftnstr[], size_t len);
  #define IFORMC iformc_
  void iformc_ (float* f, int* iw, char ftnstr[], size_t len);
  #define FFORMC fformc_
  void fformc_ (float* f, int* iw, int* idec, char ftnstr[], size_t len);
  #define FONLYC fonlyc_
  void fonlyc_ (float* f, int* iw, int* idec, char ftnstr[], size_t len);
  #define EFORMC eformc_
  void eformc_ (float* f, int* iw, int* idec, char ftnstr[], size_t len);
  #define EXPOUTC expoutc_
  void expoutc_ (int* ib, int* ie, char ftnstr[], size_t len);
  #define ALFSETC alfsetc_
  void alfsetc_ (float* fnum, int* il, char ftnstr[], size_t len);
  #define NOTATEC notatec_
  void notatec_ (int* ix, int* iy, char ftnstr[], size_t len);
  #define VLABLC vlablc_
  void vlablc_ (char ftnstr[], size_t len);
  #define JUSTERC justerc_
  void justerc_ (char ftnstr[], size_t len, int* iPos, int* iOff);

#else
  Check definitions in graph2d.h: "String parameters in Fortran functions are only tested with GCC";
#endif // __GNUC__



// ************************** AG2 API *********************************

#define AG2LEV ag2lev_ // System
void ag2lev_ (int* i1);
#define BINITT binitt_
void binitt_ (void);

#define COMDMP comdmp_ // Table and data handling
void comdmp_ (void);
#define COMGET comget_
float comget_ (int* iPar);
#define COMSET comset_
void comset_ (int* iPar, float* val);
#define FINDGE findge_
float findge_ (float* val, float* tab, int* in);
#define FINDLE findle_
float findle_ (float* val, float* tab, int* in);
#define IBASEC ibasec_
int ibasec_ (int* iPar);
#define IBASEX ibasex_
int ibasex_ (int* iPar);
#define IBASEY ibasey_
int ibasey_ (int* iPar);
#define IUBGC iubgc_
void iubgc_ (int* i1, int* i2, int* i3);
#define OUBGC oubgc_
void oubgc_ (int* i1, int* i2, int* i3);
#define LEAP leap_
int leap_ (int* i1);
#define LOCGE locge_
int locge_ (int* i1, int* i2, int* i3);
#define LOCLE locle_
int locle_ (int* i1, int* i2, int* i3);
#define ROUNDD roundd_
float roundd_ (float* f1, float* f2);
#define ROUNDU roundu_
float roundu_ (float* f1, float* f2);
#define SAVCOM savcom_
void savcom_ (int* iArr);
#define RESCOM rescom_
void rescom_ (int* iArr);

#define ALFSET alfset_ // Axis and labeling
void alfset_ (float* f1, int* i1, int* i2, int* i3);
#define NUMSET numset_
void numset_ (float* f1, int* i1, int* i2, int* i3, int* i4);
#define EFORM eform_
void eform_ (float* f1, int* i1, int* i2, int* i3, int* i4);
#define ESPLIT esplit_
void esplit_ (float* f1, int* i1, int* i2, int* i3);
#define EXPOUT expout_
void expout_ (int* i1, int* i2, int* i3, int* i4, int* i5);
#define FFORM fform_
void fform_ (float* f1, int* i1, int* i2, int* i3, int* i4);
#define FONLY fonly_
void fonly_ (float* f1, int* i1, int* i2, int* i3, int* i4);
#define GLINE gline_
void gline_ (int* i1, int* i2, int* i3);
#define HLABEL hlabel_
void hlabel_ (int* i1, int* i2);
#define HSTRIN hstrin_
void hstrin_ (int* i1, int* i2);
#define IFORM iform_
void iform_ (int* i1, int* i2, int* i3, int* i4);
#define IOTHER iother_
int iother_ (int* i1);
#define JUSTER juster_
void juster_ (int* i1, int* i2, int* i3, int* i4, int* i5, int* i6);
#define LWIDTH lwidth_
void lwidth_ (int* i1);
#define MONPOS monpos_
void monpos_ (int* i1, int* i2, float* f3, int* i4);
#define NOTATE notate_
void notate_ (int* i1, int* i2, int* i3, int* i4);
#define PLACE place_
void place_ (int* i1);
#define REMLAB remlab_
void remlab_ (int* i1, int* i2, int* i3, int* i4, int* i5);
#define SPREAD spread_
void spread_ (int* i1);
#define VLABEL vlabel_
void vlabel_ (int* i1, int* i2);
#define VSTRIN vstrin_
void vstrin_ (int* i1);
#define YDYMD ydymd_
void ydymd_ (int* i1, int* i2, int* i3, int* i4, int* i5);
#define YMDYD ymdyd_
void ymdyd_ (int* i1, int* i2, int* i3, int* i4, int* i5);

#define BSYMS bsyms_ // Symbols
void bsyms_ (float* f1, float* f2, int* i3);
#define CALPNT calpnt_
void calpnt_ (float* f1, int* i2);
#define FILBOX filbox_
void filbox_ (int* i1, int* i2, int* i3, int* i4, int* i5, int* i6);
#define GRID grid_
void grid_ (void);
#define KEYSET keyset_
void keyset_ (float* f1, int* i2);
#define LABEL label_
void label_ (int* i1);
#define LOGTIX logtix_
void logtix_ (int* i1, float* f2, float* f3, int* i4, int* i5);
#define SETWIN setwin_
void setwin_ (void);
#define SYMOUT symout_
void symout_ (int* i1, float* f2);
#define TEKSYM teksym_
void teksym_ (int* i1, float* f2);
#define TSET tset_
void tset_ (int* i1);
#define TSET2 tset2_
void tset2_ (int* i1, int* i2, int* i3, int* i4, int* i5, int* i6);
#define WIDTH width_
void width_ (int* i1);

#define BAR bar_ // Display
void bar_ (float* f1, float* f2, int* i3);
#define CALCON calcon_
void calcon_ (float* f1, float* f2, int* i3, int* i4);
#define CMNMX cmnmx_
void cmnmx_ (float* f1, float* f2, float* f3);
#define COPTIM coptim_
void coptim_ (int* i1);
#define CPLOT cplot_
void cplot_ (float* f1, float* f2);
#define DATGET datget_
void datget_ (float* f1, int* i2, int* i3);
#define DSPLAY dsplay_
void dsplay_ (float* x, float* y);
#define FRAME frame_
void frame_ (void);
#define LOPTIM loptim_
void loptim_ (int* i1);
#define MNMX mnmx_
void mnmx_ (float* f1, float* f2, float* f3);
#define OPTIM optim_
void optim_ (int* i1);

#define CHECK check_ // Curve plotting
void check_ (float* x, float* y);
#define DINITX dinitx_
void dinitx_ (void);
#define DINITY dinity_
void dinity_ (void);
#define DLIMX dlimx_
void dlimx_ (float* x, float* y);
#define DLIMY dlimy_
void dlimy_ (float* x, float* y);
#define HBARST hbarst_
void hbarst_ (int* i1, int* i2, int* i3);
#define INFIN infin_
void infin_ (float* f1);
#define LINE line_
void line_ (int* i1);
#define NPTS npts_
void npts_ (int* i1);
#define RGCHEK rgchek_
void rgchek_ (int* i1, float* f2);
#define SIZEL sizel_
void sizel_ (float* f1);
#define SIZES sizes_
void sizes_ (float* f1);
#define SLIMX slimx_
void slimx_ (int* i1, int* i2);
#define SLIMY slimy_
void slimy_ (int* i1, int* i2);
#define STEPL stepl_
void stepl_ (int* i1);
#define STEPS steps_
void steps_ (int* i1);
#define SYMBL symbl_
void symbl_ (int* i1);
#define TYPCK typck_
void typck_ (int* i1, float* f2);
#define VBARST vbarst_
void vbarst_ (int* i1, int* i2, int* i3);
#define XDEN xden_
void xden_ (int* i1);
#define XETYP xetyp_
void xetyp_ (int* i1);
#define XFRM xfrm_
void xfrm_ (int* i1);
#define XLAB xlab_
void xlab_ (int* i1);
#define XLEN xlen_
void xlen_ (int* i1);
#define XLOC xloc_
void xloc_ (int* i1);
#define XLOCTP xloctp_
void xloctp_ (int* i1);
#define XMFRM xmfrm_
void xmfrm_ (int* i1);
#define XMTCS xmtcs_
void xmtcs_ (int* i1);
#define XNEAT xneat_
void xneat_ (int* i1);
#define XTICS xtics_
void xtics_ (int* i1);
#define XTYPE xtype_
void xtype_ (int* i1);
#define XWDTH xwdth_
void xwdth_ (int* i1);
#define XZERO xzero_
void xzero_ (int* i1);
#define YDEN yden_
void yden_ (int* i1);
#define YETYP yetyp_
void yetyp_ (int* i1);
#define YFRM yfrm_
void yfrm_ (int* i1);
#define YLAB ylab_
void ylab_ (int* i1);
#define YLEN ylen_
void ylen_ (int* i1);
#define YLOC yloc_
void yloc_ (int* i1);
#define YLOCRT ylocrt_
void ylocrt_ (int* i1);
#define YMFRM ymfrm_
void ymfrm_ (int* i1);
#define YMTCS ymtcs_
void ymtcs_ (int* i1);
#define YNEAT yneat_
void yneat_ (int* i1);
#define YTICS ytics_
void ytics_ (int* i1);
#define YTYPE ytype_
void ytype_ (int* i1);
#define YWDTH ywdth_
void ywdth_ (int* i1);
#define YZERO yzero_
void yzero_ (int* i1);



// ************************** TCS API *********************************


#define INITT initt_ // Function control routines
void initt_ (int* iBaud);
#define FINITT finitt_
void finitt_ (int* ix, int* iy); // also: forward declaration in TCSdrWX.cpp
#define ERASE erase_
void erase_ (void);
#define IOWAIT iowait_
void iowait_ (int* iWait);
#define LINTRN lintrn_
void lintrn_ (void);

#define TCSLEV tcslev_
void tcslev_ (int* iLevel); // Array (Year, Day, System)
#define SVSTAT svstat_
void svstat_ (char dst[]);
#define RESTAT restat_
void restat_ (char src[]);
#define GENFLG genflg_
int genflg_ (int* i); // int = logical function

#define HDCOPY hdcopy_
void hdcopy_ (void);



#define MOVREL movrel_ // Absolute relative drawing
void movrel_ (int* ix,int* iy);
#define PNTREL pntrel_
void pntrel_ (int* ix,int* iy);
#define DRWREL drwrel_
void drwrel_ (int* ix,int* iy);
#define DSHREL dshrel_
void dshrel_ (int* ix,int* iy, int* iMask);

#define MOVABS movabs_ // Absolute direct drawing
void movabs_ (int* ix,int* iy); // also: forward declaration in TCSdrWX.cpp
#define PNTABS pntabs_
void pntabs_ (int* ix,int* iy);
#define DRWABS drwabs_
void drwabs_ (int* ix,int* iy);
#define DSHABS dshabs_
void dshabs_ (int* ix,int* iy, int* iMask);

#define MOVEA movea_ // Virtual direct drawing
void movea_ (float* x,float* y);
#define POINTA pointa_
void pointa_ (float* x,float* y);
#define DRAWA drawa_
void drawa_ (float* x,float* y);
#define DASHA dasha_
void dasha_ (float* x,float* y, int* iMask);

#define MOVER mover_ // Virtual relative drawing
void mover_ (float* x,float* y);
#define POINTR pointr_
void pointr_ (float* x,float* y);
#define DRAWR drawr_
void drawr_ (float* x,float* y);
#define DASHR dashr_
void dashr_ (float* x,float* y, int* iMask);



#define TOUTPT toutpt_ // Text output
void toutpt_ (int* ic);
#define TOUTST toutst_
void toutst_ (int* nChar, int iCharArr);
#define NRMSIZ nrmsiz_
void nrmsiz_ (void);
#define DBLSIZ dblsiz_
void dblsiz_ (void);
#define ITALIC italic_
void italic_ (void);
#define ITALIR italir_
void italir_ (void);
#define CSIZE csize_
void csize_ (int* ix,int* iy);
#define SETMRG setmrg_
void setmrg_ (int* i1,int* i2);
#define LINHGT linhgt_
int linhgt_ (int* n);
#define LINWDT linwdt_
int linwdt_ (int* n);

#define ANCHO ancho_
void ancho_ (int* ic);
#define ANSTR anstr_
void anstr_ (int* nChar, int iCharArr);
#define BAKSP baksp_
void baksp_ (void);
#define CARTN cartn_
void cartn_ (void);
#define LINEF linef_
void linef_ (void);
#define NEWLIN newlin_
void newlin_ (void);

#define BELL bell_
void bell_ (void);

#define HOME home_
void home_ (void);
#define NEWPAG newpag_
void newpag_ (void);



#define TINPUT tinput_ // Input routines
void tinput_ (int* iC); // also: forward declaration in TCSdrWX.cpp
#define DCURSR dcursr_
void dcursr_ (int *ic,int* ix,int* iy);
#define VCURSR vcursr_
void vcursr_ (int *ic,float* x,float* y);
#define SEELOC seeloc_
void seeloc_ (int* ix,int* iy);



#define SWINDO swindo_ // coordinate systems
void swindo_ (int* ix,int* ilx, int* iy,int* ily);
#define TWINDO twindo_
void twindo_ (int* ix1,int* ix2, int* iy1,int* iy2);
#define DWINDO dwindo_
void dwindo_ (float* x1,float* x2, float* y1,float* y2);
#define VWINDO vwindo_
void vwindo_ (float* x,float* xl, float* y,float* yl);
#define REL2AB rel2ab_
void rel2ab_ (float* x1,float* y1, float* x2,float* y2);
#define WINCOT wincot_
void wincot_ (float* x, float* y, int* ix,int* iy);
#define REVCOT revcot_
void revcot_ (int* ix,int* iy, float* x, float* y);
#define RESCAL rescal_
void rescal_ (void);
#define LOGTRN logtrn_
void logtrn_ (int* i);
#define RROTAT rrotat_
void rrotat_ (float* grd);
#define RSCALE rscale_
void rscale_ (float* fac);
#define SEETRM seetrm_
void seetrm_ (int* i1,int* i2, int* i3,int* i4);
#define SEETRN seetrn_
void seetrn_ (float* f1,float* f2, int* i3);



#define ALFMOD alfmod_ // Dummy routines
void alfmod_ (void);
#define ALPHA alpha_
void alpha_ (void);
#define ANMODE anmode_
void anmode_ (void);
#define PCLIPT pclipt_
void pclipt_ (void);



// ************************** TCS: extensions *********************************

#define BCKCOL bckcol_
void bckcol_ (int* iCol);
#define LINCOL lincol_
void lincol_ (int* iCol);
#define TXTCOL txtcol_
void txtcol_ (int* iCol);
#define DEFAULTCOLOUR defaultcolour_
void defaultcolour_ (void);

#define WINSELECT winselect_
bool winselect_ (int* ID);

/* C subroutine for handling multiple windows, also used in TCSdrWXfor.f08 */
    void initt1 (int iMode, wxFrame* parent, wxFrame* FrameToUse, wxStatusBar* StatusBarToUse);
    void winlbl0 (const char PloWinNam[], const char StatWinNam[], const char IniFilNam[]);

/* TCS: internal subroutines, used in TCSdrWXfor.f08 */
    void TCSGraphicError (int iErr, const char* msg); // also: forward declaration in TCSdrWX.cpp
}
