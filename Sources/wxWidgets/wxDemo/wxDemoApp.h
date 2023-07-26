/***************************************************************
 * Name:      wxDemoApp.h
 * Purpose:   Defines Application Class
 * Author:    Klaus Friedewald (kfgithub@web.de)
 * Created:   2022-11-15
 * Copyright: Klaus Friedewald ()
 * License:
 **************************************************************/

#ifndef WXDEMOAPP_H
#define WXDEMOAPP_H

#include <wx/app.h>

class wxDemoApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // WXDEMOAPP_H
