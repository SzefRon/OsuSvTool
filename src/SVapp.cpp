#include "SVapp.h"
 
bool SVApp::OnInit()
{
    mainFrame = new MainFrame("Osu SV tool", wxDefaultPosition, wxSize(450, 340));
    mainFrame->Show(true);
    return true;
}