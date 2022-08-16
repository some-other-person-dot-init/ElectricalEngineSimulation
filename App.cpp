#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif // WX_PRECOMP

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include <matplot/matplot.h>
#include <string.h>

#include "App.h"
#include "Simulation.h"

#define APP_DEFAULT_TEXTSIZE wxSize(50, 20)

IMPLEMENT_APP(MainApp)

// MainApp
bool MainApp::OnInit()
{
    MainFrame *frame = new MainFrame("Electrical Motor Simulation", wxDefaultPosition, wxSize(550, 500));
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}

// MainPanel
BEGIN_EVENT_TABLE(MainPanel, wxPanel)
    EVT_BUTTON(ID_BUTTON_RUN, MainPanel::onRunButton)
    EVT_CHECKBOX(ID_CHECK_ISCURRPERSIST, MainPanel::onSrccurrCheck)
    EVT_CHECKBOX(ID_CHECK_SWITCHREACT, MainPanel::onSwitchReactCheck)
    EVT_TEXT(ID_TEXT_AIRACTRESIST, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_MAGNETICREACTRESIST, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_POLES, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_PROGCURR, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_PROGFREQ, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_PROGVOLT, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_ROTACTRESIST, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_ROTREACTRESIST, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_SRCCURR, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_SRCVOLT, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_STATACTRESIST, MainPanel::onInputTextEntry)
    EVT_TEXT(ID_TEXT_STATREACTRESIST, MainPanel::onInputTextEntry)
END_EVENT_TABLE()

MainPanel::MainPanel(wxFrame *frame) : wxPanel(frame, wxID_ANY)
{
    m_btn_run = new wxButton(this, ID_BUTTON_RUN, "RUN", wxPoint(320, 360), wxSize(180, 60));
    m_btn_run->SetFont(m_btn_run->GetFont().MakeBold());
    m_btn_run->SetFont(m_btn_run->GetFont().Scale(2));

    m_lbl_secsrc = new wxStaticText(this, ID_LABEL_SECTIONSOURCE, "SOURCE", wxPoint(10, 10));
    m_lbl_secsrc->SetFont(m_lbl_secsrc->GetFont().MakeBold());
    m_lbl_srcvolt = new wxStaticText(this, ID_LABEL_SRCVOLT, "Source voltage (V):", wxPoint(10, 30));
    m_lbl_srccurr = new wxStaticText(this, ID_LABEL_SRCCURR, "Source current (A):", wxPoint(10, 70));
    m_lbl_srccurr->SetForegroundColour(*wxLIGHT_GREY);
    m_lbl_secinv = new wxStaticText(this, ID_LABEL_SECTIONINVERTER, "INVERTER", wxPoint(10, 100));
    m_lbl_secinv->SetFont(m_lbl_secinv->GetFont().MakeBold());
    m_lbl_progvolt = new wxStaticText(this, ID_LABEL_PROGVOLT, "Programmed voltage (V):", wxPoint(10, 120));
    m_lbl_progcurr = new wxStaticText(this, ID_LABEL_PROGCURR, "Programmed current (A):", wxPoint(10, 140));
    m_lbl_progfreq = new wxStaticText(this, ID_LABEL_PROGFREQ, "Programmed frequency (Hz):", wxPoint(10, 160));
    m_lbl_seceng = new wxStaticText(this, ID_LABEL_SECTIONENGINE, "ENGINE", wxPoint(10, 190));
    m_lbl_seceng->SetFont(m_lbl_seceng->GetFont().MakeBold());
    m_lbl_poles = new wxStaticText(this, ID_LABEL_POLES, "Number of poles:", wxPoint(10, 210));
    m_lbl_statactres = new wxStaticText(this, ID_LABEL_STATACTRESIST, "Active resistance of a stator (Ohm):", wxPoint(10, 230));
    m_lbl_rotactres = new wxStaticText(this, ID_LABEL_ROTACTRESIST, "Active resistance of a rotor (Ohm):", wxPoint(10, 250));
    m_lbl_airactres = new wxStaticText(this, ID_LABEL_AIRACTRESIST, "Active resistance of air (Ohm):", wxPoint(10, 270));
    m_lbl_statreactres = new wxStaticText(this, ID_LABEL_STATREACTRESIST, "Reactive resistance of a stator (Ohm):", wxPoint(10, 310));
    m_lbl_rotreactres = new wxStaticText(this, ID_LABEL_ROTREACTRESIST, "Reactive resistance of a rotor (Ohm):", wxPoint(10, 330));
    m_lbl_magnreactres = new wxStaticText(this, ID_LABEL_MAGNETICREACTRESIST, "Reactive resistance of Magnetic field (Ohm):", wxPoint(10, 350));
    m_lbl_effic = new wxStaticText(this, ID_LABEL_EFFICIENCY, "Efficiency [0; 1]:", wxPoint(10, 375));
    m_lbl_secout = new wxStaticText(this, ID_LABEL_SECTIONOUTPUT, "OUTPUT", wxPoint(320, 10));
    m_lbl_secout->SetFont(m_lbl_secout->GetFont().MakeBold());

    m_txt_srcvolt = new wxTextCtrl(this, ID_TEXT_SRCVOLT, "0", wxPoint(120, 30), APP_DEFAULT_TEXTSIZE);
    m_txt_srccurr = new wxTextCtrl(this, ID_TEXT_SRCCURR, "0", wxPoint(120, 70), APP_DEFAULT_TEXTSIZE);
    m_txt_srccurr->Enable(false);
    m_txt_progvolt = new wxTextCtrl(this, ID_TEXT_PROGVOLT, "0", wxPoint(170, 120), APP_DEFAULT_TEXTSIZE);
    m_txt_progcurr = new wxTextCtrl(this, ID_TEXT_PROGCURR, "0", wxPoint(170, 140), APP_DEFAULT_TEXTSIZE);
    m_txt_progfreq = new wxTextCtrl(this, ID_TEXT_PROGFREQ, "0", wxPoint(170, 160), APP_DEFAULT_TEXTSIZE);
    m_txt_poles = new wxTextCtrl(this, ID_TEXT_POLES, "0", wxPoint(200, 210), APP_DEFAULT_TEXTSIZE);
    m_txt_statactres = new wxTextCtrl(this, ID_TEXT_STATACTRESIST, "0", wxPoint(200, 230), APP_DEFAULT_TEXTSIZE);
    m_txt_rotactres = new wxTextCtrl(this, ID_TEXT_ROTACTRESIST, "0", wxPoint(200, 250), APP_DEFAULT_TEXTSIZE);
    m_txt_airactres = new wxTextCtrl(this, ID_TEXT_AIRACTRESIST, "0", wxPoint(200, 270), APP_DEFAULT_TEXTSIZE);
    m_txt_statreactres = new wxTextCtrl(this, ID_TEXT_STATREACTRESIST, "0", wxPoint(250, 310), APP_DEFAULT_TEXTSIZE);
    m_txt_rotreactres = new wxTextCtrl(this, ID_TEXT_ROTREACTRESIST, "0", wxPoint(250, 330), APP_DEFAULT_TEXTSIZE);
    m_txt_magnreactres = new wxTextCtrl(this, ID_TEXT_MAGNETICREACTRESIST, "0", wxPoint(250, 350), APP_DEFAULT_TEXTSIZE);
    m_txt_effic = new wxTextCtrl(this, ID_TEXT_EFFICIENCY, "0", wxPoint(100, 375), APP_DEFAULT_TEXTSIZE);

    m_chk_iscurrpers = new wxCheckBox(this, ID_CHECK_ISCURRPERSIST, "Has the source non-infinite current?", wxPoint(10, 50));
    m_chk_switchreact = new wxCheckBox(this, ID_CHECK_SWITCHREACT, "Make reactive resistance dynamic?", wxPoint(10, 290));

    m_txt_output = new wxTextCtrl(this, ID_PLOT_OUTGRAPH, wxEmptyString, wxPoint(320, 30), wxSize(180, 320), wxTE_READONLY | wxTE_MULTILINE);


    this->SetDefValues();
}

void MainPanel::onRunButton(wxCommandEvent &evt)
{

}

void MainPanel::onSrccurrCheck(wxCommandEvent &evt)
{
    if (evt.IsChecked())
    {
        m_lbl_srccurr->SetForegroundColour(*wxBLACK);
        m_lbl_srccurr->Refresh();
        m_txt_srccurr->Enable(true);
    }
    else
    {
        m_lbl_srccurr->SetForegroundColour(*wxLIGHT_GREY);
        m_lbl_srccurr->Refresh();
        m_txt_srccurr->Enable(false);
    }
}

void MainPanel::onSwitchReactCheck(wxCommandEvent &evt)
{
    if (evt.IsChecked())
    {
        m_lbl_statreactres->SetForegroundColour(*wxLIGHT_GREY);
        m_lbl_statreactres->Refresh();
        m_txt_statreactres->Enable(false);
        m_lbl_rotreactres->SetForegroundColour(*wxLIGHT_GREY);
        m_lbl_rotreactres->Refresh();
        m_txt_rotreactres->Enable(false);
        m_lbl_magnreactres->SetForegroundColour(*wxLIGHT_GREY);
        m_lbl_magnreactres->Refresh();
        m_txt_magnreactres->Enable(false);
    }
    else
    {
        m_lbl_statreactres->SetForegroundColour(*wxBLACK);
        m_lbl_statreactres->Refresh();
        m_txt_statreactres->Enable(true);
        m_lbl_rotreactres->SetForegroundColour(*wxBLACK);
        m_lbl_rotreactres->Refresh();
        m_txt_rotreactres->Enable(true);
        m_lbl_magnreactres->SetForegroundColour(*wxBLACK);
        m_lbl_magnreactres->Refresh();
        m_txt_magnreactres->Enable(true);
    }
}

void MainPanel::onInputTextEntry(wxCommandEvent &evt)
{
    // It would better if there was kind of evt.Cancel(), unfortuanetly wxWidgets doesn't provide it :(
    if (!evt.GetString().IsNumber())
    {
        long lastInsPos = ((wxTextCtrl *)evt.GetEventObject())->GetInsertionPoint();
        ((wxTextCtrl *)evt.GetEventObject())->ChangeValue(evt.GetString().Remove(((wxTextCtrl *)evt.GetEventObject())->GetInsertionPoint() - 1, 1));
        ((wxTextCtrl *)evt.GetEventObject())->SetInsertionPoint(lastInsPos - 1);
    }
}

void MainPanel::SetDefValues()
{
    this->m_txt_srcvolt->ChangeValue("750");
    this->m_txt_progvolt->ChangeValue("322");
    this->m_txt_progcurr->ChangeValue("150");
    this->m_txt_progfreq->ChangeValue("15");
    this->m_txt_poles->ChangeValue("4");
    this->m_txt_statactres->ChangeValue("0.0384");
    this->m_txt_rotactres->ChangeValue("0.0384");
    this->m_txt_airactres->ChangeValue("30");
    this->m_txt_statreactres->ChangeValue("1.2");
    this->m_txt_rotreactres->ChangeValue("1.2");
    this->m_txt_effic->ChangeValue("0.91");
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