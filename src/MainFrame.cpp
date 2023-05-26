#include "MainFrame.h"

#include "MainNotebook.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        MainNotebook *mainNotebook = new MainNotebook(this, 1, wxDefaultPosition, wxDefaultSize);
        sizer->Add(mainNotebook, 1, wxGROW, 10);

        SetSizer(sizer);
}