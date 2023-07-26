/***************************************************************
 * Name:      wxDemoApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Klaus Friedewald (kfgithub@web.de)
 * Created:   2022-11-15
 * Copyright: Klaus Friedewald ()
 * License:   GNU LESSER GENERAL PUBLIC LICENSE Version 3
 **************************************************************/

#include "wxDemoApp.h"

//(*AppHeaders
#include "wxDemoMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(wxDemoApp);

bool wxDemoApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	wxDemoFrame* Frame = new wxDemoFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
