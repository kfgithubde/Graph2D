/***************************************************************
 * Name:      wxDemoMain.h
 * Purpose:   Defines Application Frame
 * Author:    Klaus Friedewald (kfgithub@web.de)
 * Created:   2022-11-15
 * Copyright: Klaus Friedewald ()
 * License:
 **************************************************************/

#ifndef WXDEMOMAIN_H
#define WXDEMOMAIN_H

//(*Headers(wxDemoFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
//*)

class wxDemoFrame: public wxFrame
{
    public:

        wxDemoFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~wxDemoFrame();

    private:

        //(*Handlers(wxDemoFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnButton4ClickColourDemo(wxCommandEvent& event);
        void OnButton5ClickD1(wxCommandEvent& event);
        void OnButton6ClickD2(wxCommandEvent& event);
        void OnButton7ClickD3(wxCommandEvent& event);
        void OnButton8ClickD4(wxCommandEvent& event);
        void OnButton9ClickD5(wxCommandEvent& event);
        void OnButton10ClickD6(wxCommandEvent& event);
        void OnTextCtrl1TextX1min(wxCommandEvent& event);
        void OnTextCtrl2TextX1max(wxCommandEvent& event);
        void OnButton1ClickPlotF1(wxCommandEvent& event);
        void OnButton2ClickPlotF2(wxCommandEvent& event);
        void OnTextCtrl3TextX2min(wxCommandEvent& event);
        void OnTextCtrl5TextX2max(wxCommandEvent& event);
        void OnButton3ClickPlotF3(wxCommandEvent& event);
        void OnTextCtrl4TextX3min(wxCommandEvent& event);
        void OnTextCtrl6TextX3max(wxCommandEvent& event);
        //*)

        //(*Identifiers(wxDemoFrame)
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL2;
        static const long ID_STATICTEXT6;
        static const long ID_STATICTEXT7;
        static const long ID_BUTTON1;
        static const long ID_STATICTEXT8;
        static const long ID_TEXTCTRL3;
        static const long ID_STATICTEXT9;
        static const long ID_STATICTEXT13;
        static const long ID_STATICTEXT10;
        static const long ID_TEXTCTRL5;
        static const long ID_STATICTEXT11;
        static const long ID_STATICTEXT12;
        static const long ID_BUTTON2;
        static const long ID_STATICTEXT14;
        static const long ID_TEXTCTRL4;
        static const long ID_STATICTEXT15;
        static const long ID_STATICTEXT16;
        static const long ID_STATICTEXT17;
        static const long ID_TEXTCTRL6;
        static const long ID_STATICTEXT18;
        static const long ID_STATICTEXT19;
        static const long ID_BUTTON3;
        static const long ID_STATICTEXT20;
        static const long ID_BUTTON4;
        static const long ID_BUTTON5;
        static const long ID_BUTTON6;
        static const long ID_BUTTON7;
        static const long ID_BUTTON8;
        static const long ID_BUTTON9;
        static const long ID_BUTTON10;
        static const long ID_PANEL1;
        static const long ID_PANEL3;
        static const long ID_PANEL4;
        static const long ID_PANEL5;
        static const long ID_PANEL2;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(wxDemoFrame)
        wxButton* Button10;
        wxButton* Button1;
        wxButton* Button2;
        wxButton* Button3;
        wxButton* Button4;
        wxButton* Button5;
        wxButton* Button6;
        wxButton* Button7;
        wxButton* Button8;
        wxButton* Button9;
        wxPanel* Panel1;
        wxPanel* Panel2;
        wxPanel* Panel3;
        wxPanel* Panel4;
        wxPanel* Panel5;
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText12;
        wxStaticText* StaticText13;
        wxStaticText* StaticText14;
        wxStaticText* StaticText15;
        wxStaticText* StaticText16;
        wxStaticText* StaticText17;
        wxStaticText* StaticText18;
        wxStaticText* StaticText19;
        wxStaticText* StaticText1;
        wxStaticText* StaticText20;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxStatusBar* StatusBar1;
        wxTextCtrl* TextCtrl1;
        wxTextCtrl* TextCtrl2;
        wxTextCtrl* TextCtrl3;
        wxTextCtrl* TextCtrl4;
        wxTextCtrl* TextCtrl5;
        wxTextCtrl* TextCtrl6;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // WXDEMOMAIN_H
