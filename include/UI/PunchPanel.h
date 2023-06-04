#pragma once

#include <wx/wx.h>

class PunchPanel : public wxPanel 
{
public:
    PunchPanel(wxWindow *parent, wxWindowID id=wxID_ANY, const wxPoint &pos=wxDefaultPosition, const wxSize &size=wxDefaultSize, long style=wxTAB_TRAVERSAL, const wxString &name=wxPanelNameStr);
private:
    wxSpinCtrl *noSVsSpinCtrl;

    wxStaticText *normSVlabel;
    wxSlider *normSVslider;

    wxStaticText *topSVlabel;
    wxSlider *topSVslider;

    void OnDraw(wxPaintEvent &event);
    void OnNormSVsliderChange(wxCommandEvent &event);
    void OnTopSVsliderChange(wxCommandEvent &event);
    void OnNoSVsSpinCtrlChange(wxSpinEvent &event);
};