#include "UI/NormalizePanel.h"

#include <wx/gbsizer.h>
#include "Logic/MapConfig.h"

NormalizePanel::NormalizePanel(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxPanel(parent, id, pos, size, style, name)
{
    wxGridBagSizer *sizer = new wxGridBagSizer(10, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(0, 0), wxGBSpan(1, 2));

    // Box + Radio buttons
    wxStaticBoxSizer *radioSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Normalize mode");
        // Empty column
    radioSizer->Add(0, 0, 1);

        // Auto-detect button
    autoButton = new wxRadioButton(radioSizer->GetStaticBox(), wxID_ANY, "Auto-detect");
    radioSizer->Add(autoButton, 0, wxALIGN_CENTER | wxUP | wxDOWN, 10);
    
        // Empty column
    radioSizer->Add(0, 0, 1);
    
        // Custom BPM button
    customBPMbutton = new wxRadioButton(radioSizer->GetStaticBox(), wxID_ANY, "Custom BPM");
    radioSizer->Add(customBPMbutton, 0, wxALIGN_CENTER | wxUP | wxDOWN, 10);
    
        // Empty column
    radioSizer->Add(0, 0, 1);

    sizer->Add(radioSizer, wxGBPosition(1, 0), wxGBSpan(1, 2), wxGROW | wxLEFT | wxRIGHT, 10);

    // Text + Spinctrl
    wxStaticText *bpmText = new wxStaticText(this, wxID_ANY, "Bpm to normalize to:");
    sizer->Add(bpmText, wxGBPosition(2, 0), wxGBSpan(1, 1), wxALIGN_CENTER | wxLEFT, 10);

    bpmSpinCtrl = new wxSpinCtrlDouble(this, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, 16384L, 0.0, 1000000.0, 120.0, 1.0);
    bpmSpinCtrl->SetDigits(3);
    bpmSpinCtrl->Disable();
    sizer->Add(bpmSpinCtrl, wxGBPosition(2, 1), wxGBSpan(1, 1), wxGROW | wxRIGHT, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(3, 0), wxGBSpan(1, 2));

    // Button
    generateButton = new wxButton(this, wxID_ANY, "Generate SVs");
    generateButton->Disable();
    sizer->Add(generateButton, wxGBPosition(4, 0), wxGBSpan(1, 2), wxGROW | wxLEFT | wxRIGHT, 10);

    // Empty row
    sizer->Add(0, 0, wxGBPosition(5, 0), wxGBSpan(1, 2));

    sizer->AddGrowableCol(1, 1);

    sizer->AddGrowableRow(0, 1);
    sizer->AddGrowableRow(3, 1);
    sizer->AddGrowableRow(4, 2);
    sizer->AddGrowableRow(5, 1);

    this->SetSizer(sizer);

    // Event binding
    Bind(wxEVT_RADIOBUTTON, &NormalizePanel::OnAutoButtonPress, this, autoButton->GetId());
    Bind(wxEVT_RADIOBUTTON, &NormalizePanel::OnCustomBPMButtonPress, this, customBPMbutton->GetId());
    Bind(wxEVT_BUTTON, &NormalizePanel::OnGeneratePress, this, generateButton->GetId());
}

void NormalizePanel::reset()
{
    bpmSpinCtrl->SetValue(120.0);
    generateButton->Disable();
    autoButton->SetValue(false);
    customBPMbutton->SetValue(false);
}

void NormalizePanel::OnAutoButtonPress(wxCommandEvent &event)
{
    normalizer.autoDetectBPM();
    double detectedBPM = normalizer.getBPM();
    bpmSpinCtrl->SetValue(60000.0 / detectedBPM);
    bpmSpinCtrl->Disable();
    generateButton->Enable();
}

void NormalizePanel::OnCustomBPMButtonPress(wxCommandEvent &event)
{
    normalizer.setBPM(bpmSpinCtrl->GetValue());
    bpmSpinCtrl->Enable();
    generateButton->Enable();
}

void NormalizePanel::OnGeneratePress(wxCommandEvent & event)
{
    normalizer.normalize();
    MapConfig::i().saveMap();
}