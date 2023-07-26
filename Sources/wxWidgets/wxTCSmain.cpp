/** ****************************************************************************
\file       wxTCSmain.cpp
\brief      Initialization of wxWidgets
\version    0.85
\author     (C) 2023 Dr.-Ing. Klaus Friedewald
\copyright  GNU LESSER GENERAL PUBLIC LICENSE Version 3
\~german
         wxTCSapp zur Ausführung von Fortran-Konsolenprogrammen
\~english
         wxTCSapp for executing Fortran console programs
\~
***************************************************************************** */

#define MainProgram MAIN__
// #define MainProgram ftnmain2sub_

#include <wx/wx.h>
#include "graph2d.h"


extern "C" {
    void MainProgram (); // subroutine plot f1
}

extern "C" {
    void _gfortran_set_args (int argc, char *argv[]);
}



class wxTCSapp : public wxApp
{
public:
	virtual bool OnInit();
	virtual void OnIdle();
private:
    bool MainStarted = false;
    wxFrame* wxAppframe;
};

IMPLEMENT_APP(wxTCSapp)

bool wxTCSapp::OnInit() // Build wx Event Loop
{
int i1,i2;
	wxAppframe = new wxFrame((wxFrame*) NULL, -1, GetAppDisplayName(), wxDefaultPosition,wxDefaultSize, wxDEFAULT_FRAME_STYLE);
	wxAppframe->Show(true);
	SetTopWindow(wxAppframe);

    _gfortran_set_args (wxAppConsole::argc, wxAppConsole::argv); // Initialize FTN command-line intrinsics

    Connect(wxEVT_IDLE,(wxObjectEventFunction)&wxTCSapp::OnIdle);
    initt1 (2, nullptr, wxAppframe, nullptr); // use wxAppframe for plotting

	return true;
}

void wxTCSapp::OnIdle()
{
    if (!MainStarted) {
      MainStarted= true; // 1st statement to avoid recursive invocation, e.g. due to wxYield() in tinput
      MainProgram();
      wxAppframe->Refresh();
    }
    return;
}
