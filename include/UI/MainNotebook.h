#pragma once

#include <wx/wx.h>
#include <wx/notebook.h>

#include "UI/NormalizePanel.h"
#include "UI/PunchPanel.h"

class MainNotebook : public wxNotebook
{
private:
    NormalizePanel *normalizePanel;
    PunchPanel *punchPanel;
public:
    MainNotebook(wxWindow *parent, wxWindowID id, const wxPoint &pos = wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=0, const wxString &name=wxNotebookNameStr);
    void reset();
};