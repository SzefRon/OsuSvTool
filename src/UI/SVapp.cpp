#include "UI/SVapp.h"
 
bool SVApp::OnInit()
{
    mainFrame = new MainFrame("Osu SV tool", wxDefaultPosition, wxSize(550, 500));
    mainFrame->Show(true);
    return true;
}