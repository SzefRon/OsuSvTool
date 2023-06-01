#include "UI/MainNotebook.h"

void MainNotebook::reset()
{
    normalizePanel->reset();
}

MainNotebook::MainNotebook(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxNotebook(parent, id, pos, size, style, name)
{
    normalizePanel = new NormalizePanel(this);
    this->SetPadding(wxSize(30, 2));
    this->AddPage(normalizePanel, wxString("Normalize"));
    this->AddPage(new wxPanel(this), wxString("Punch"));
    this->AddPage(new wxPanel(this), wxString("Smooth"));

    this->AddPage(new wxPanel(this), wxString("Teleport"));
}