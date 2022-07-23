#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <mathplot.h>

#include "App.h"
#include "Simulation.h"

IMPLEMENT_APP(MainApp)

// MainApp
bool MainApp::OnInit()
{
    MainFrame *frame = new MainFrame("Electrical Motor Simulation", wxDefaultPosition, wxSize(500,500));
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}

// MainPanel
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
    EVT_BUTTON(ID_BUTTON_RUN, MainPanel::onRunButton)
END_EVENT_TABLE()

MainPanel::MainPanel(wxFrame *frame) : wxPanel(frame, wxID_ANY)
{
    m_btn_run = new wxButton(this, ID_BUTTON_RUN, "Run", wxPoint(300,350), wxSize(70,60));
}

void MainPanel::onRunButton(wxCommandEvent &evt)
{
}

MainPanel::~MainPanel() {}

// MainFrame
BEGIN_EVENT_TABLE(MainFrame, wxFrame)
    //EVT_BUTTON(ID_BUTTON_EXIT, MainFrame::onExit)
END_EVENT_TABLE()

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame((wxFrame *) NULL, -1, title, pos, size)
{
    m_panel = new MainPanel(this);

    CreateStatusBar(2);
}

MainFrame::~MainFrame() {}
