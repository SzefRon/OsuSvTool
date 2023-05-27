#include "MainFrame.h"

#include <cstdlib>
#include <wx/filepicker.h>

#include "MainNotebook.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
        wxPanel *mainPanel = new wxPanel(this);
        mainPanel->SetBackgroundColour(wxColour("pink"));

        wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

        wxFilePickerCtrl *filePicker = new wxFilePickerCtrl(mainPanel, 1, wxEmptyString, wxString("Select a map's .osu file"), wxString("*.osu"));
        wxFileName osuSongsPath(wxString(std::getenv("LOCALAPPDATA")) + wxString("/osu!/Songs"));
        filePicker->SetInitialDirectory(osuSongsPath.GetAbsolutePath());
        sizer->Add(filePicker, 0, wxGROW | wxALL, 10);

        MainNotebook *mainNotebook = new MainNotebook(mainPanel, 2, wxDefaultPosition, wxDefaultSize);
        sizer->Add(mainNotebook, 1, wxGROW | wxLEFT | wxRIGHT | wxDOWN, 10);

        mainPanel->SetSizer(sizer);
}