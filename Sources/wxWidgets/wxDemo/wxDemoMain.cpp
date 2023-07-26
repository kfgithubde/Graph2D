/***************************************************************
 * Name:      wxDemoMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Klaus Friedewald (kfgithub@web.de)
 * Created:   2022-11-15
 * Copyright: Klaus Friedewald ()
 * License:   GNU LESSER GENERAL PUBLIC LICENSE Version 3
 **************************************************************/

#include "wxDemoMain.h"

#include <wx/msgdlg.h>

//(*InternalHeaders(wxDemoFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

// ----------------   My own declarations ----------------------

#include <wx/valtext.h>  // constants for input validation
#include "../graph2d.h"

// Definition of Plotfunctions and Fortran interface

extern "C" {
    float f1ftn (float* x);            // function f1 in Fortran

    float f2c (float* x)               // function f2 in C
    {
        return float(5.0f + (*x)*(*x));
    }

    float f3c (float* x)               // function f3 in C
    {
        return float(sin(*x) / (5.0f + (*x)*(*x)));
    }

    void f1pltftn (void* f1, float* xmin, float* xmax); // subroutine plot f1 in Fortran
}

float x1min=0.,x1max=0., x2min=0.,x2max=0., x3min=0.,x3max=0.;

wxTextValidator NumValidator (wxFILTER_NUMERIC); // für wxTextCtrl: nur Zahleneingabe

// ---- End of my declarations. Now code basically from wxSmith ----

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(wxDemoFrame)
const long wxDemoFrame::ID_STATICTEXT1 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT2 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL1 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT3 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT4 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT5 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL2 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT6 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT7 = wxNewId();
const long wxDemoFrame::ID_BUTTON1 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT8 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL3 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT9 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT13 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT10 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL5 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT11 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT12 = wxNewId();
const long wxDemoFrame::ID_BUTTON2 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT14 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL4 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT15 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT16 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT17 = wxNewId();
const long wxDemoFrame::ID_TEXTCTRL6 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT18 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT19 = wxNewId();
const long wxDemoFrame::ID_BUTTON3 = wxNewId();
const long wxDemoFrame::ID_STATICTEXT20 = wxNewId();
const long wxDemoFrame::ID_BUTTON4 = wxNewId();
const long wxDemoFrame::ID_BUTTON5 = wxNewId();
const long wxDemoFrame::ID_BUTTON6 = wxNewId();
const long wxDemoFrame::ID_BUTTON7 = wxNewId();
const long wxDemoFrame::ID_BUTTON8 = wxNewId();
const long wxDemoFrame::ID_BUTTON9 = wxNewId();
const long wxDemoFrame::ID_BUTTON10 = wxNewId();
const long wxDemoFrame::ID_PANEL1 = wxNewId();
const long wxDemoFrame::ID_PANEL3 = wxNewId();
const long wxDemoFrame::ID_PANEL4 = wxNewId();
const long wxDemoFrame::ID_PANEL5 = wxNewId();
const long wxDemoFrame::ID_PANEL2 = wxNewId();
const long wxDemoFrame::idMenuQuit = wxNewId();
const long wxDemoFrame::idMenuAbout = wxNewId();
const long wxDemoFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(wxDemoFrame,wxFrame)
    //(*EventTable(wxDemoFrame)
    //*)
END_EVENT_TABLE()

wxDemoFrame::wxDemoFrame(wxWindow* parent,wxWindowID id)
{

    //(*Initialize(wxDemoFrame)
    wxBoxSizer* BoxSizer1;
    wxBoxSizer* BoxSizer2;
    wxBoxSizer* BoxSizer3;
    wxBoxSizer* BoxSizer4;
    wxBoxSizer* BoxSizer5;
    wxBoxSizer* BoxSizer6;
    wxBoxSizer* BoxSizer7;
    wxBoxSizer* BoxSizer8;
    wxBoxSizer* BoxSizer9;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, _("wxDemo"), wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    BoxSizer1 = new wxBoxSizer(wxVERTICAL);
    Panel1 = new wxPanel(this, ID_PANEL1, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    BoxSizer2 = new wxBoxSizer(wxVERTICAL);
    BoxSizer3 = new wxBoxSizer(wxHORIZONTAL);
    StaticText1 = new wxStaticText(Panel1, ID_STATICTEXT1, _("Eingabe des Plotintervalls"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    BoxSizer3->Add(StaticText1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
    StaticText2 = new wxStaticText(Panel1, ID_STATICTEXT2, _("Funktion 1 (Fortran):      Xmin = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT2"));
    BoxSizer4->Add(StaticText2, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl1 = new wxTextCtrl(Panel1, ID_TEXTCTRL1, _("0"), wxDefaultPosition, wxDefaultSize, 0, NumValidator, _T("ID_TEXTCTRL1"));
    BoxSizer4->Add(TextCtrl1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText3 = new wxStaticText(Panel1, ID_STATICTEXT3, _("f1 (Xmin) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT3"));
    BoxSizer4->Add(StaticText3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText4 = new wxStaticText(Panel1, ID_STATICTEXT4, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT4"));
    BoxSizer4->Add(StaticText4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText5 = new wxStaticText(Panel1, ID_STATICTEXT5, _("Xmax = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT5"));
    BoxSizer4->Add(StaticText5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl2 = new wxTextCtrl(Panel1, ID_TEXTCTRL2, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    BoxSizer4->Add(TextCtrl2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText6 = new wxStaticText(Panel1, ID_STATICTEXT6, _("f1 (Xmax) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT6"));
    BoxSizer4->Add(StaticText6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText7 = new wxStaticText(Panel1, ID_STATICTEXT7, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    BoxSizer4->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button1 = new wxButton(Panel1, ID_BUTTON1, _("Plot f1 (Fortran)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    BoxSizer4->Add(Button1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer5 = new wxBoxSizer(wxHORIZONTAL);
    StaticText8 = new wxStaticText(Panel1, ID_STATICTEXT8, _("Funktion 2 (C):        Xmin = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT8"));
    BoxSizer5->Add(StaticText8, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl3 = new wxTextCtrl(Panel1, ID_TEXTCTRL3, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    BoxSizer5->Add(TextCtrl3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(Panel1, ID_STATICTEXT9, _("f2 (Xmin) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT9"));
    BoxSizer5->Add(StaticText9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText13 = new wxStaticText(Panel1, ID_STATICTEXT13, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT13"));
    BoxSizer5->Add(StaticText13, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText10 = new wxStaticText(Panel1, ID_STATICTEXT10, _("Xmax = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT10"));
    BoxSizer5->Add(StaticText10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl5 = new wxTextCtrl(Panel1, ID_TEXTCTRL5, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    BoxSizer5->Add(TextCtrl5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText11 = new wxStaticText(Panel1, ID_STATICTEXT11, _("f2 (Xmax) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT11"));
    BoxSizer5->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(Panel1, ID_STATICTEXT12, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    BoxSizer5->Add(StaticText12, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button2 = new wxButton(Panel1, ID_BUTTON2, _("Plot f2 (Fortran)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    BoxSizer5->Add(Button2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer6 = new wxBoxSizer(wxHORIZONTAL);
    StaticText14 = new wxStaticText(Panel1, ID_STATICTEXT14, _("Funktion 3 (C):        Xmin = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT14"));
    BoxSizer6->Add(StaticText14, 2, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl4 = new wxTextCtrl(Panel1, ID_TEXTCTRL4, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    BoxSizer6->Add(TextCtrl4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText15 = new wxStaticText(Panel1, ID_STATICTEXT15, _("f3(Xmin) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT15"));
    BoxSizer6->Add(StaticText15, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText16 = new wxStaticText(Panel1, ID_STATICTEXT16, _("\?\?\?"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT16"));
    BoxSizer6->Add(StaticText16, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText17 = new wxStaticText(Panel1, ID_STATICTEXT17, _("Xmax = "), wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT, _T("ID_STATICTEXT17"));
    BoxSizer6->Add(StaticText17, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    TextCtrl6 = new wxTextCtrl(Panel1, ID_TEXTCTRL6, _("0"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    BoxSizer6->Add(TextCtrl6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText18 = new wxStaticText(Panel1, ID_STATICTEXT18, _("f3 (Xmax) = "), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE|wxALIGN_RIGHT, _T("ID_STATICTEXT18"));
    BoxSizer6->Add(StaticText18, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText19 = new wxStaticText(Panel1, ID_STATICTEXT19, _("\?\?\?"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE, _T("ID_STATICTEXT19"));
    BoxSizer6->Add(StaticText19, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button3 = new wxButton(Panel1, ID_BUTTON3, _("Plot f3 (C)"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    BoxSizer6->Add(Button3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer7 = new wxBoxSizer(wxHORIZONTAL);
    StaticText20 = new wxStaticText(Panel1, ID_STATICTEXT20, _("Aufruf der Demoprogramme"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT20"));
    BoxSizer7->Add(StaticText20, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer8 = new wxBoxSizer(wxHORIZONTAL);
    Button4 = new wxButton(Panel1, ID_BUTTON4, _("ColourDemo"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    BoxSizer8->Add(Button4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button5 = new wxButton(Panel1, ID_BUTTON5, _("D1"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON5"));
    BoxSizer8->Add(Button5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button6 = new wxButton(Panel1, ID_BUTTON6, _("D2"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON6"));
    BoxSizer8->Add(Button6, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button7 = new wxButton(Panel1, ID_BUTTON7, _("D3"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON7"));
    BoxSizer8->Add(Button7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button8 = new wxButton(Panel1, ID_BUTTON8, _("D4"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON8"));
    BoxSizer8->Add(Button8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button9 = new wxButton(Panel1, ID_BUTTON9, _("D5"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON9"));
    BoxSizer8->Add(Button9, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Button10 = new wxButton(Panel1, ID_BUTTON10, _("D6"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON10"));
    BoxSizer8->Add(Button10, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    BoxSizer2->Add(BoxSizer8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel1->SetSizer(BoxSizer2);
    BoxSizer2->Fit(Panel1);
    BoxSizer2->SetSizeHints(Panel1);
    BoxSizer1->Add(Panel1, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2 = new wxPanel(this, ID_PANEL2, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL2"));
    BoxSizer9 = new wxBoxSizer(wxHORIZONTAL);
    Panel3 = new wxPanel(Panel2, ID_PANEL3, wxDefaultPosition, wxSize(340,260), wxTAB_TRAVERSAL, _T("ID_PANEL3"));
    BoxSizer9->Add(Panel3, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel4 = new wxPanel(Panel2, ID_PANEL4, wxDefaultPosition, wxSize(10,260), wxTAB_TRAVERSAL, _T("ID_PANEL4"));
    Panel4->SetBackgroundColour(wxColour(192,192,192));
    BoxSizer9->Add(Panel4, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel5 = new wxPanel(Panel2, ID_PANEL5, wxDefaultPosition, wxSize(340,260), wxTAB_TRAVERSAL, _T("ID_PANEL5"));
    BoxSizer9->Add(Panel5, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Panel2->SetSizer(BoxSizer9);
    BoxSizer9->Fit(Panel2);
    BoxSizer9->SetSizeHints(Panel2);
    BoxSizer1->Add(Panel2, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    SetSizer(BoxSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    BoxSizer1->Fit(this);
    BoxSizer1->SetSizeHints(this);

    Connect(ID_TEXTCTRL1,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl1TextX1min);
    Connect(ID_TEXTCTRL2,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl2TextX1max);
    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton1ClickPlotF1);
    Connect(ID_TEXTCTRL3,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl3TextX2min);
    Connect(ID_TEXTCTRL5,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl5TextX2max);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton2ClickPlotF2);
    Connect(ID_TEXTCTRL4,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl4TextX3min);
    Connect(ID_TEXTCTRL6,wxEVT_COMMAND_TEXT_UPDATED,(wxObjectEventFunction)&wxDemoFrame::OnTextCtrl6TextX3max);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton3ClickPlotF3);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton4ClickColourDemo);
    Connect(ID_BUTTON5,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton5ClickD1);
    Connect(ID_BUTTON6,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton6ClickD2);
    Connect(ID_BUTTON7,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton7ClickD3);
    Connect(ID_BUTTON8,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton8ClickD4);
    Connect(ID_BUTTON9,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton9ClickD5);
    Connect(ID_BUTTON10,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&wxDemoFrame::OnButton10ClickD6);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxDemoFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&wxDemoFrame::OnAbout);
    //*)
}

wxDemoFrame::~wxDemoFrame()
{
    //(*Destroy(wxDemoFrame)
    //*)
}

void wxDemoFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void wxDemoFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void wxDemoFrame::OnButton4ClickColourDemo(wxCommandEvent& event)
{
    wxExecute ("../ColourDemo.exe");
}

void wxDemoFrame::OnButton5ClickD1(wxCommandEvent& event)
{
    wxExecute ("../D1.exe");
}

void wxDemoFrame::OnButton6ClickD2(wxCommandEvent& event)
{
    wxExecute ("../D2.exe");
}

void wxDemoFrame::OnButton7ClickD3(wxCommandEvent& event)
{
    wxExecute ("../D3.exe");
}

void wxDemoFrame::OnButton8ClickD4(wxCommandEvent& event)
{
    wxExecute ("../D4.exe");
}

void wxDemoFrame::OnButton9ClickD5(wxCommandEvent& event)
{
    wxExecute ("../D5.exe");
}

void wxDemoFrame::OnButton10ClickD6(wxCommandEvent& event)
{
    wxExecute ("../D6.exe");
}

void wxDemoFrame::OnTextCtrl1TextX1min(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl1->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x1min= (float) xIn;
      xIn= (double) f1ftn(& x1min);
      Str= Str.Format (wxT("f1 (%f) = "),x1min);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText3->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText4->SetLabel (Str2);
}

void wxDemoFrame::OnTextCtrl2TextX1max(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl2->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x1max= (float) xIn;
      xIn= (double) f1ftn(& x1max);
      Str= Str.Format (wxT("f1 (%f) = "),x1max);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText6->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText7->SetLabel (Str2);
}

void wxDemoFrame::OnTextCtrl3TextX2min(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl3->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x2min= (float) xIn;
      xIn= (double) f2c(& x2min);
      Str= Str.Format (wxT("f1 (%f) = "),x2min);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText9->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText13->SetLabel (Str2);
}

void wxDemoFrame::OnTextCtrl5TextX2max(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl5->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x2max= (float) xIn;
      xIn= (double) f2c(& x2max);
      Str= Str.Format (wxT("f1 (%f) = "),x2max);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText11->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText12->SetLabel (Str2);
}

void wxDemoFrame::OnTextCtrl4TextX3min(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl4->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x3min= (float) xIn;
      xIn= (double) f3c(& x3min);
      Str= Str.Format (wxT("f1 (%f) = "),x3min);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText15->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText16->SetLabel (Str2);
}

void wxDemoFrame::OnTextCtrl6TextX3max(wxCommandEvent& event)
{
double xIn;
wxString Str,Str2;
    Str= TextCtrl6->GetValue ();
    if (Str.ToDouble (&xIn)) {  // Success: Number o.k.
      x3max= (float) xIn;
      xIn= (double) f3c(& x3max);
      Str= Str.Format (wxT("f1 (%f) = "),x3max);
      Str2= Str.Format (wxT("%f"),xIn);
    } else {
      Str= wxT("f1 (?!?) = ");
      Str2 = wxT(" ");
    }
    StaticText18->SetLabel (Str); // Flag wxST_NO_AUTORESIZE notwendig! (sonst linksbuendige Ausgabe)
    StaticText19->SetLabel (Str2);
}

void wxDemoFrame::OnButton1ClickPlotF1(wxCommandEvent& event)
{
  static int iD=1; // My own ID for the plotwindow
  bool winNotReady;
    winNotReady= winselect_ (&iD);
    if (winNotReady) {
      winlbl0 ("%: Plot in new Window", "Statwin", "Graph2DGerman.xml");
      initt1 (1, this, nullptr, nullptr);
    }
    Button1->Enable(false); // prevent reentrance into plot routine
    f1pltftn ((void*) f1ftn, & x1min, & x1max);
    Button1->Enable (true); // a clean second call of the plot routine is o.k.
}

void wxDemoFrame::OnButton2ClickPlotF2(wxCommandEvent& event)
{
  static int iD=2; // My own ID for the plotwindow
  bool winNotReady;
    winNotReady= winselect_ (&iD);
    if (winNotReady) {
      winlbl0 ("Plot on Panel", "Statwin", "Graph2DGerman.xml");
      initt1 (3, this, (wxFrame*) Panel3, StatusBar1);
    }
    Button2->Enable(false); // prevent reentrance into plot routine
    f1pltftn ((void*) f2c, & x2min, & x2max);
    Button2->Enable (true); // a clean second call of the plot routine is o.k.
}

void wxDemoFrame::OnButton3ClickPlotF3(wxCommandEvent& event)
{
  static int iD=3; // My own ID for the plotwindow
  bool winNotReady;
    winNotReady= winselect_ (&iD);
    if (winNotReady) {
      winlbl0 ("Plot on Panel", "Statwin", "Graph2DGerman.xml");
      initt1 (3, this, (wxFrame*) Panel5, StatusBar1);
    }
    Button3->Enable(false); // prevent re-entrance into plot routine

/* ********** Here comes the C-plotroutine ************************** */

// Fill arrays with data points
  const int nPts = 100+1; // Teklong array: [0] is length
  int i;
  float dx, x[nPts], y[nPts];
    dx= (x3max-x3min) / (nPts-1.0f);
    x[0]= float (nPts-1); // nPts-1= number of data elements
    y[0]= float (nPts-1);
    for (i=1; i<nPts; i++) {
      x[i]= x3min+ (i-1)*dx;
      y[i]= f3c( &x[i]);
    }
// Demonstration for calling Fortran subroutines with string parameters
  char WinNam[] = "Window Name"; size_t i1= strlen(WinNam);
                                 size_t i2=strlen("String constant");
                                 size_t i3=1; // Constant length for empty string
    WINLBL (WinNam, "String constant", "", i1, i2, i3);

// Plotting routines
    INITT (0); // Dummy arguments can be passed by value
    BINITT ();
    DEFAULTCOLOUR ();
    i=2; LINCOL (&i); // Parameter must have an address
    i=6; BCKCOL (&i);
    ERASE ();
    CHECK (x,y); // Arrays are always passed "by address"
    DSPLAY (x,y);

// Lets see the diagram
    IOWAIT (0);

/* ********** Plotting done, allow useractions ********************** */

    Button3->Enable (true); // A clean second call of the plot routine is o.k.
}
