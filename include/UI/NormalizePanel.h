#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

class NormalizePanel : public wxPanel 
{
public:
    NormalizePanel(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
private:
    wxSpinCtrlDouble *bpmSpinCtrl;
    void OnAutoButtonPress(wxCommandEvent& event);
    void OnCustomBPMButtonPress(wxCommandEvent& event);
};