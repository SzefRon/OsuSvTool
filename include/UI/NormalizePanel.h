#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>

class NormalizePanel : public wxPanel 
{
public:
    NormalizePanel(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
    void reset();
private:
    wxSpinCtrlDouble *bpmSpinCtrl;
    wxButton *generateButton;
    wxRadioButton *autoButton;
    wxRadioButton *customBPMbutton;
    void OnAutoButtonPress(wxCommandEvent& event);
    void OnCustomBPMButtonPress(wxCommandEvent& event);
    void OnGeneratePress(wxCommandEvent& event);
};