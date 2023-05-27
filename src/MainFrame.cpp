#include "MainFrame.h"

#include <cstdlib>
#include <wx/filepicker.h>
#include <wx/gbsizer.h>

#include "MainNotebook.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
        // Panel + Sizer
        wxPanel *mainPanel = new wxPanel(this);
        mainPanel->SetBackgroundColour(wxColour("pink"));

        wxGridBagSizer *sizer = new wxGridBagSizer(10, 10);

        // Text + file picker
        wxStaticText *mapText = new wxStaticText(mainPanel, -1, "Select a map to edit:");
        sizer->Add(mapText, wxGBPosition(0, 0), wxGBSpan(1, 1), wxLEFT | wxUP, 10);

        wxFilePickerCtrl *filePicker = new wxFilePickerCtrl(mainPanel, 1, wxEmptyString, wxString("Select a map's .osu file"), wxString("*.osu"));
        wxFileName osuSongsPath(wxString(std::getenv("LOCALAPPDATA")) + wxString("/osu!/Songs"));
        if (osuSongsPath.DirExists()) {
                filePicker->SetInitialDirectory(osuSongsPath.GetAbsolutePath());
        }
        sizer->Add(filePicker, wxGBPosition(0, 1), wxGBSpan(1, 1), wxGROW | wxRIGHT | wxUP, 10);

        // Notebook
        MainNotebook *mainNotebook = new MainNotebook(mainPanel, 2, wxDefaultPosition, wxDefaultSize);
        sizer->Add(mainNotebook, wxGBPosition(1, 0), wxGBSpan(1, 2), wxGROW | wxLEFT | wxRIGHT | wxDOWN, 10);

        sizer->AddGrowableCol(1, 1);

        sizer->AddGrowableRow(1, 1);

        mainPanel->SetSizer(sizer);
}