#pragma once

#include <wx/wx.h>
#include <wx/gbsizer.h>
#include <wx/filepicker.h>

#include "UI/MainNotebook.h"

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    wxPanel *mainPanel;
    wxGridBagSizer *sizer;
    wxFilePickerCtrl *mapFilePicker;
    MainNotebook *mainNotebook;
    void OnFileDrop(wxDropFilesEvent& event);
    void OnPathPicked(wxFileDirPickerEvent& event);
    void mapFileChanged(std::wstring fileName);
};