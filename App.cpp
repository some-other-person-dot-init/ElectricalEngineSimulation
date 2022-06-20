#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP
#include "App.h"
#include "Simulation.h"

IMPLEMENT_APP(MainApp)

// MainApp
bool MainApp::OnInit()
{
    MainFrame *frame = new MainFrame("Induction Motor Simulation", wxDefaultPosition, wxSize(500,500));
    frame->Show();
    SetTopWindow(frame);

    return true;
}

// MainPanel
MainPanel::MainPanel(wxFrame *frame) : wxPanel(frame, wxID_ANY)
{
}

MainPanel::~MainPanel() {}

// MainFrame
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
    m_panel = new MainPanel(this);
}

MainFrame::~MainFrame() {}
