#include "UI/SVapp.h"
 
bool SVApp::OnInit()
{
    mainFrame = new MainFrame("Osu SV tool", wxDefaultPosition, wxSize(600, 450));
    mainFrame->Show(true);
    return true;
}