#pragma once

#include <wx/wx.h>
#include <wx/filepicker.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    wxFilePickerCtrl *mapFilePicker;
    void MainFrame::OnFileDrop(wxDropFilesEvent& event);
};