#include "MainNotebook.h"

#include "NormalizePanel.h"

MainNotebook::MainNotebook(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, long style, const wxString &name)
    : wxNotebook(parent, id, pos, size, style, name)
{
    this->SetPadding(wxSize(30, 2));
    this->AddPage(new NormalizePanel(this), wxString("Normalize"));
    this->AddPage(new wxPanel(this), wxString("Punch"));
    this->AddPage(new wxPanel(this), wxString("Smooth"));
    this->AddPage(new wxPanel(this), wxString("Teleport"));
}