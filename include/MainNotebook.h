#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

class MainNotebook : public wxNotebook
{
public:
    MainNotebook(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxNotebookNameStr);
};