#pragma once

#include "MainFrame.h"

#include <wx/wx.h>

class SVApp : public wxApp
{
private:
    MainFrame *mainFrame;
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(SVApp);