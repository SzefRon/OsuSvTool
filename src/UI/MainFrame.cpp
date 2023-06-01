#include "UI/MainFrame.h"

#include <cstdlib>
#include <wx/msgdlg.h>

#include "Logic/MapConfig.h"

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // Panel + Sizer
    mainPanel = new wxPanel(this);
    mainPanel->SetBackgroundColour(wxColour("pink"));

    sizer = new wxGridBagSizer(10, 10);

    // Text + file picker
    wxStaticText *mapText = new wxStaticText(mainPanel, wxID_ANY, "Select a map to edit:");
    sizer->Add(mapText, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxLEFT | wxUP, 10);

    mapFilePicker = new wxFilePickerCtrl(mainPanel, wxID_ANY, wxEmptyString, wxString("Select a map's .osu file"), wxString("*.osu"));
    mapFilePicker->DragAcceptFiles(true);
    mapFilePicker->GetTextCtrl()->SetHint("Supports Drag & Drop");
    wxFileName osuSongsPath(wxString(std::getenv("LOCALAPPDATA")) + wxString("/osu!/Songs"));
    if (osuSongsPath.DirExists()) {
        mapFilePicker->SetInitialDirectory(osuSongsPath.GetAbsolutePath());
    }
    sizer->Add(mapFilePicker, wxGBPosition(0, 1), wxGBSpan(1, 1), wxGROW | wxRIGHT | wxUP, 10);

    // Notebook
    mainNotebook = new MainNotebook(mainPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    sizer->Add(mainNotebook, wxGBPosition(1, 0), wxGBSpan(1, 2), wxGROW | wxLEFT | wxRIGHT | wxDOWN, 10);

    sizer->AddGrowableCol(1, 1);

    sizer->AddGrowableRow(1, 1);

    mainPanel->SetSizer(sizer);

    // Event binding
    mapFilePicker->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(MainFrame::OnFileDrop), NULL, this);
    Bind(wxEVT_FILEPICKER_CHANGED, &MainFrame::OnPathPicked, this, mapFilePicker->GetId());
}

void MainFrame::OnFileDrop(wxDropFilesEvent &event)
{
    wxString *droppedFiles = event.GetFiles();
    wxFileName droppedFile(droppedFiles[0]);

    if (droppedFile.GetExt() == wxString("osu")) {
        if (event.GetNumberOfFiles() != 1) {
            wxMessageDialog *multipleFilesDialog = new wxMessageDialog(this, "Multiple files dropped\nOnly the last one will be processed", "Warning", wxICON_WARNING);
            multipleFilesDialog->ShowModal();
        }
        mapFilePicker->SetPath(wxString(droppedFiles[0]));
        std::wstring fileNameW = droppedFiles[0].wchar_str();
        mapFileChanged(fileNameW);
    } else {
        wxMessageDialog *invalidFileDialog = new wxMessageDialog(this, "Invalid file\nFile type must be '.osu'\nNo changes were made", "Error", wxICON_ERROR);
        invalidFileDialog->ShowModal();
    }
}

void MainFrame::OnPathPicked(wxFileDirPickerEvent &event)
{
    wxFileName fileName(event.GetPath());
    if (fileName.FileExists() && fileName.GetExt() == wxString("osu")) {
        std::wstring fileNameW = fileName.GetFullPath().wchar_str();
        mapFileChanged(fileNameW);
    }
}

void MainFrame::mapFileChanged(std::wstring fileName)
{
    mainNotebook->reset();   
    MapConfig::i().loadMap(fileName);
}
