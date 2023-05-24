#pragma once

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class SVApp : public wxApp
{
public:
    virtual bool OnInit();
};

wxIMPLEMENT_APP(SVApp);