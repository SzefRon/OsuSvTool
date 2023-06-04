#include "UI/PunchPanel.h"

#include "wx/gbsizer.h"
#include "wx/collpane.h"
#include "wx/spinctrl.h"
#include "wx/graphics.h"

#include <iomanip>
#include <sstream>

PunchPanel::PunchPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name)
{
    wxGridBagSizer *sizer = new wxGridBagSizer(wxVERTICAL);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(0, 0), wxGBSpan(1, 4));

    // Starting point
    wxStaticText *startingPointLabel = new wxStaticText(this, wxID_ANY, "Starting time point:");
    sizer->Add(startingPointLabel, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

    wxTextCtrl *startintPointText = new wxTextCtrl(this, wxID_ANY, "00:00:000 - ");
    sizer->Add(startintPointText, wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW | wxLEFT, 10);

    // Beat snap
    wxStaticText *beatSnapLabel = new wxStaticText(this, wxID_ANY, "Beat snap divisor:");
    sizer->Add(beatSnapLabel, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    wxString choicesArr[11] {"1/1","1/2","1/3","1/4","1/5","1/6","1/7","1/8","1/9","1/12","1/16"};
    wxArrayString beatSnapChoices(11, choicesArr);
    wxChoice *beatSnapChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, beatSnapChoices);
    beatSnapChoice->SetSelection(3);
    sizer->Add(beatSnapChoice, wxGBPosition(1, 3), wxGBSpan(1, 1), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(2, 0), wxGBSpan(1, 4));

    // Number of SVs
    wxStaticText *noSVsLabel = new wxStaticText(this, wxID_ANY, "Number of SVs:");
    sizer->Add(noSVsLabel, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    noSVsSpinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 16384L, 2, 100, 3);
    sizer->Add(noSVsSpinCtrl, wxGBPosition(3, 1), wxGBSpan(1, 1), wxGROW | wxLEFT, 10);

    // Additional divisor
    wxStaticText *divisorLabel = new wxStaticText(this, wxID_ANY, "Additional divisor:");
    sizer->Add(divisorLabel, wxGBPosition(3, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    wxSpinCtrl *divisorSpinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 16384L, 1, 100, 1);
    sizer->Add(divisorSpinCtrl, wxGBPosition(3, 3), wxGBSpan(1, 1), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(4, 0), wxGBSpan(1, 4));

    // SV to normalize to value
    wxStaticBoxSizer *normSVsizer = new wxStaticBoxSizer(wxVERTICAL, this, "SV to normalize to");

    normSVlabel = new wxStaticText(this, wxID_ANY, "1");
    normSVsizer->Add(normSVlabel, 0, wxALIGN_CENTER);

    normSVslider = new wxSlider(this, wxID_ANY, 10, 1, 50, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS);
    normSVsizer->Add(normSVslider, 1, wxGROW | wxDOWN, 10);

    sizer->Add(normSVsizer, wxGBPosition(5, 0), wxGBSpan(1, 4), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(6, 0), wxGBSpan(1, 4));

    // Top SV value
    wxStaticBoxSizer *topSVsizer = new wxStaticBoxSizer(wxVERTICAL, this, "First SV value");

    topSVlabel = new wxStaticText(this, wxID_ANY, "1");
    topSVsizer->Add(topSVlabel, 0, wxALIGN_CENTER);

    topSVslider = new wxSlider(this, wxID_ANY, 10, 1, 30, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS);
    topSVsizer->Add(topSVslider, 1, wxGROW | wxDOWN, 10);

    wxPanel *drawingPanel = new wxPanel(this, wxID_ANY);
    topSVsizer->Add(drawingPanel, 1, wxGROW);
    
    sizer->Add(topSVsizer, wxGBPosition(7, 0), wxGBSpan(1, 4), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(8, 0), wxGBSpan(1, 4));

    sizer->AddGrowableCol(1, 1);
    sizer->AddGrowableCol(3, 1);

    sizer->AddGrowableRow(0, 1);
    sizer->AddGrowableRow(2, 1);
    sizer->AddGrowableRow(4, 1);
    sizer->AddGrowableRow(6, 1);
    sizer->AddGrowableRow(8, 1);

    this->SetSizer(sizer);

    // Event binding
    Bind(wxEVT_PAINT, &PunchPanel::OnDraw, this, this->GetId());
    Bind(wxEVT_SLIDER, &PunchPanel::OnNormSVsliderChange, this, normSVslider->GetId());
    Bind(wxEVT_SLIDER, &PunchPanel::OnTopSVsliderChange, this, topSVslider->GetId());
    Bind(wxEVT_SPINCTRL, &PunchPanel::OnNoSVsSpinCtrlChange, this, noSVsSpinCtrl->GetId());
}

void PunchPanel::OnDraw(wxPaintEvent & event)
{
    /*wxPaintDC dc(this);
    dc.DrawText(wxT("Testing"), 40, 60);

    // draw a circle
    dc.SetBrush(*wxGREEN_BRUSH); // green filling
    dc.SetPen( wxPen( wxColor(255,0,0), 5 ) ); // 5-pixels-thick red outline
    dc.DrawCircle( wxPoint(200,100), 25 );

    // draw a rectangle
    dc.SetBrush(*wxBLUE_BRUSH); // blue filling
    dc.SetPen( wxPen( wxColor(255,175,175), 10 ) ); // 10-pixels-thick pink outline
    dc.DrawRectangle( 300, 100, 400, 200 );

    // draw a line
    dc.SetPen( wxPen( wxColor(0,0,0), 3 ) ); // black line, 3 pixels thick

    dc.DrawLine( 300, 100, 700, 300 ); // draw line across the rectangle*/
}

void PunchPanel::OnNormSVsliderChange(wxCommandEvent &event)
{
    double val = normSVslider->GetValue() / 10.0;
    std::stringstream ss;
    ss << std::setprecision(2) << val;
    normSVlabel->SetLabel(wxString(ss.str()));

    topSVslider->SetMax(noSVsSpinCtrl->GetValue() * normSVslider->GetValue() - 1);
}

void PunchPanel::OnTopSVsliderChange(wxCommandEvent & event)
{
    double val = topSVslider->GetValue() / 10.0;
    std::stringstream ss;
    ss << std::setprecision(2) << val;

    topSVlabel->SetLabel(wxString(ss.str()));
}

void PunchPanel::OnNoSVsSpinCtrlChange(wxSpinEvent & event)
{
    topSVslider->SetMax(noSVsSpinCtrl->GetValue() * normSVslider->GetValue() - 1);
}