#include "NormalizePanel.h"

#include <wx/gbsizer.h>
#include <wx/spinctrl.h>

NormalizePanel::NormalizePanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name)
{
    wxGridBagSizer *sizer = new wxGridBagSizer(10, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(0, 0), wxGBSpan(1, 2));

    // Text + spinctrl
    wxStaticText *bpmText = new wxStaticText(this, -1, "Select bpm:");
    sizer->Add(bpmText, wxGBPosition(1, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxLEFT, 10);

    wxSpinCtrl *bpmSpinCtrl = new wxSpinCtrl(this, 1, "Leave unchanged or put 0 for auto-detect");
    sizer->Add(bpmSpinCtrl, wxGBPosition(1, 1), wxGBSpan(1, 1), wxGROW | wxRIGHT, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(2, 0), wxGBSpan(1, 2));

    // Button
    wxButton *generateButton = new wxButton(this, 2, "Generate SVs");
    sizer->Add(generateButton, wxGBPosition(3, 0), wxGBSpan(1, 2), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(4, 0), wxGBSpan(1, 2));

    sizer->AddGrowableCol(1, 1);

    sizer->AddGrowableRow(0, 1);
    sizer->AddGrowableRow(2, 1);
    sizer->AddGrowableRow(3, 2);
    sizer->AddGrowableRow(4, 1);

    this->SetSizer(sizer);
}
