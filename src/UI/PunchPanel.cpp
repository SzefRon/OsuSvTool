#include "UI/PunchPanel.h"

#include "wx/gbsizer.h"
#include "wx/collpane.h"
#include "wx/spinctrl.h"
#include "wx/graphics.h"

#include <iomanip>
#include <string>
#include <sstream>

#include "Logic/MapConfig.h"

PunchPanel::PunchPanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name)
{
    timer = new wxTimer(this, wxID_ANY);
    wxGridBagSizer *sizer = new wxGridBagSizer(wxVERTICAL);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(0, 0), wxGBSpan(1, 4));

    // Starting point
    wxStaticText *startingPointLabel = new wxStaticText(this, wxID_ANY, "Starting time point:");
    sizer->Add(startingPointLabel, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT, 10);

    startingPointText = new wxTextCtrl(this, wxID_ANY, "00:00:000 - ");
    sizer->Add(startingPointText, wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW | wxLEFT, 10);

    // Beat snap
    wxStaticText *beatSnapLabel = new wxStaticText(this, wxID_ANY, "Beat snap divisor:");
    sizer->Add(beatSnapLabel, wxGBPosition(1, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    wxString choicesArr[11] {"1/1","1/2","1/3","1/4","1/5","1/6","1/7","1/8","1/9","1/12","1/16"};
    wxArrayString beatSnapChoices(11, choicesArr);
    beatSnapChoice = new wxChoice(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, beatSnapChoices);
    beatSnapChoice->SetSelection(3);
    sizer->Add(beatSnapChoice, wxGBPosition(1, 3), wxGBSpan(1, 1), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(2, 0), wxGBSpan(1, 4));

    // Number of SVs
    wxStaticText *noSVsLabel = new wxStaticText(this, wxID_ANY, "Number of SVs:");
    sizer->Add(noSVsLabel, wxGBPosition(3, 0), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    noSVsSpinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 16384L, 2, 20, 3);
    sizer->Add(noSVsSpinCtrl, wxGBPosition(3, 1), wxGBSpan(1, 1), wxGROW | wxLEFT, 10);

    // Additional divisor
    wxStaticText *divisorLabel = new wxStaticText(this, wxID_ANY, "Additional divisor:");
    sizer->Add(divisorLabel, wxGBPosition(3, 2), wxGBSpan(1, 1), wxALIGN_CENTER_VERTICAL | wxLEFT , 10);

    divisorSpinCtrl = new wxSpinCtrl(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 16384L, 1, 100, 1);
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

    topSVlabel = new wxStaticText(this, wxID_ANY, "1.5");
    topSVsizer->Add(topSVlabel, 0, wxALIGN_CENTER);

    topSVslider = new wxSlider(this, wxID_ANY, 15, 1, 30, wxDefaultPosition, wxDefaultSize, wxSL_AUTOTICKS);
    topSVsizer->Add(topSVslider, 1, wxGROW | wxDOWN, 10);

    wxPanel *drawingPanel = new wxPanel(this, wxID_ANY);
    topSVsizer->Add(drawingPanel, 1, wxGROW);
    
    sizer->Add(topSVsizer, wxGBPosition(7, 0), wxGBSpan(1, 4), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(8, 0), wxGBSpan(1, 4));

    // Generate button
    generateButton = new wxButton(this, wxID_ANY, "Generate SVs");
    sizer->Add(generateButton, wxGBPosition(9, 0), wxGBSpan(1, 4), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty space
    sizer->Add(0, 0, wxGBPosition(10, 0), wxGBSpan(1, 4));

    sizer->AddGrowableCol(1, 1);
    sizer->AddGrowableCol(3, 1);

    sizer->AddGrowableRow(0, 1);
    sizer->AddGrowableRow(2, 1);
    sizer->AddGrowableRow(4, 1);
    sizer->AddGrowableRow(6, 1);
    sizer->AddGrowableRow(8, 1);
    sizer->AddGrowableRow(9, 1);
    sizer->AddGrowableRow(10, 1);

    this->SetSizer(sizer);

    // Event binding
    Bind(wxEVT_PAINT, &PunchPanel::OnDraw, this, this->GetId());
    Bind(wxEVT_SLIDER, &PunchPanel::OnNormSVsliderChange, this, normSVslider->GetId());
    Bind(wxEVT_SLIDER, &PunchPanel::OnTopSVsliderChange, this, topSVslider->GetId());
    Bind(wxEVT_SPINCTRL, &PunchPanel::OnNoSVsSpinCtrlChange, this, noSVsSpinCtrl->GetId());
    Bind(wxEVT_TEXT, &PunchPanel::OnStartingPointTextChange, this, startingPointText->GetId());
    Bind(wxEVT_CHOICE, &PunchPanel::OnBeatSnapChoice, this, beatSnapChoice->GetId());
    Bind(wxEVT_SPINCTRL, &PunchPanel::OnExtraDivisorSpinCtrlChange, this, divisorSpinCtrl->GetId());
    Bind(wxEVT_BUTTON, &PunchPanel::OnGeneratePress, this, generateButton->GetId());
    Bind(wxEVT_TIMER, &PunchPanel::OnGenerateWait, this, timer->GetId());
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
    double normSVval = normSVslider->GetValue() / 10.0;
    punchGenerator.normSV = normSVval;

    std::stringstream ss;
    ss << std::defaultfloat << normSVval;
    normSVlabel->SetLabel(wxString(ss.str()));

    int maxTopSVval = noSVsSpinCtrl->GetValue() * normSVslider->GetValue() - 1;

    topSVslider->SetMax(maxTopSVval);
    UpdateTopSVlabel();
}

void PunchPanel::OnTopSVsliderChange(wxCommandEvent & event)
{
    UpdateTopSVlabel();
}

void PunchPanel::OnNoSVsSpinCtrlChange(wxSpinEvent & event)
{
    int noSVsVal = noSVsSpinCtrl->GetValue();
    punchGenerator.noSVs = noSVsVal;

    int maxTopSVval = noSVsVal * normSVslider->GetValue() - 1;

    topSVslider->SetMax(maxTopSVval);
    UpdateTopSVlabel();
}

void PunchPanel::OnStartingPointTextChange(wxCommandEvent &event)
{
    std::string textVal = startingPointText->GetValue().ToStdString();
    if (punchGenerator.setStartingPoint(textVal) == 1) {
        startingPointText->SetBackgroundColour(*wxRED);
    } else {
        startingPointText->SetBackgroundColour(*wxWHITE);
    }
    startingPointText->Refresh();
}

void PunchPanel::OnBeatSnapChoice(wxCommandEvent &event)
{
    std::string selection = beatSnapChoice->GetString(beatSnapChoice->GetSelection()).ToStdString();
    punchGenerator.setBSdivisor(selection);
}

void PunchPanel::OnExtraDivisorSpinCtrlChange(wxCommandEvent &event)
{
    punchGenerator.extraDivisor = divisorSpinCtrl->GetValue();
}

void PunchPanel::OnGeneratePress(wxCommandEvent &event)
{
    if (punchGenerator.normSV != punchGenerator.topSV) {
        generateButton->Disable();

        punchGenerator.findMatchingPower();
        if (punchGenerator.generateSVs() == 0) {
            MapConfig::i().saveMap();
            generateButton->SetLabel("Done");
            timer->Start(1500);
        } else {
            generateButton->Enable();
        }
    } else {
        wxMessageDialog *error = new wxMessageDialog(this, "Both sliders can't have the exact same value", "Error", wxICON_ERROR);
        error->ShowModal();
    }
}

void PunchPanel::OnGenerateWait(wxTimerEvent &event)
{
    generateButton->Enable();
    generateButton->SetLabel("Generate SVs");
}

void PunchPanel::UpdateTopSVlabel()
{
    double topSVval = topSVslider->GetValue() / 10.0;
    punchGenerator.topSV = topSVval;

    std::stringstream ss;
    ss << std::defaultfloat << topSVval;
    topSVlabel->SetLabel(wxString(ss.str()));
}