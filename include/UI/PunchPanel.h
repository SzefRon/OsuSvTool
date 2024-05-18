#pragma once

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include "Logic/PunchGenerator.h"

class PunchPanel : public wxPanel 
{
public:
    PunchPanel(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
private:
    PunchGenerator punchGenerator;
    wxTimer *timer;

    wxTextCtrl *startingPointText;

    wxChoice *beatSnapChoice;
    wxSpinCtrl *divisorSpinCtrl;

    wxSpinCtrl *noSVsSpinCtrl;

    wxStaticText *normSVlabel;
    wxSlider *normSVslider;

    wxStaticText *topSVlabel;
    wxSlider *topSVslider;

    wxButton *generateButton;

    void OnDraw(wxPaintEvent &event);
    void OnNormSVsliderChange(wxCommandEvent& event);
    void OnTopSVsliderChange(wxCommandEvent& event);
    void OnNoSVsSpinCtrlChange(wxSpinEvent& event);
    void OnStartingPointTextChange(wxCommandEvent& event);
    void OnBeatSnapChoice(wxCommandEvent& event);
    void OnExtraDivisorSpinCtrlChange(wxCommandEvent& event);
    void OnGeneratePress(wxCommandEvent& event);
    void OnGenerateWait(wxTimerEvent& event);

    void UpdateTopSVlabel();
};