#ifndef __BASE_H
#define __BASE_H

// Main App
class MainApp : public wxApp
{
public:
    virtual bool OnInit();
};

// IDs
enum
{
    ID_BUTTON_RUN = 130,

    ID_TEXT_SRCVOLT = 200,
    ID_TEXT_PROGVOLT = 201,
    ID_TEXT_PROGCURR = 202,
    ID_TEXT_PROGFREQ = 203,
    ID_TEXT_STATACTRESIST = 204,
    ID_TEXT_ROTACTRESIST = 205,
    ID_TEXT_AIRACTRESIST = 206,
    ID_TEXT_STATREACTRESIST = 207,
    ID_TEXT_ROTREACTRESIST = 208,
    ID_TEXT_MAGNETICREACTRESIST = 209,
    ID_TEXT_POLES = 210,
    ID_TEXT_SRCCURR = 211,

    ID_LABEL_SRCVOLT = 300,
    ID_LABEL_PROGVOLT = 301,
    ID_LABEL_PROGCURR = 302,
    ID_LABEL_PROGFREQ = 303,
    ID_LABEL_STATACTRESIST = 304,
    ID_LABEL_ROTACTRESIST = 305,
    ID_LABEL_AIRACTRESIST = 306,
    ID_LABEL_STATREACTRESIST = 307,
    ID_LABEL_ROTREACTRESIST = 308,
    ID_LABEL_MAGNETICREACTRESIST = 309,
    ID_LABEL_POLES = 310,
    ID_LABEL_SRCCURR = 311,

    ID_CHECK_SWITCHREACT = 500,
    ID_CHECK_ISCURRPERSIST = 501,

    ID_PLOT_OUTGRAPH = 900,
};

// MainPanel
class MainPanel : public wxPanel
{
public:
    MainPanel(wxFrame *frame);

    void onRunButton(wxCommandEvent &evt);

    virtual ~MainPanel();

private:
    wxButton *m_btn_run;

    wxTextEntry *m_txt_srcvolt;
    wxTextEntry *m_txt_progvolt;
    wxTextEntry *m_txt_progcurr;
    wxTextEntry *m_txt_progfreq;
    wxTextEntry *m_txt_statactres;
    wxTextEntry *m_txt_rotactres;
    wxTextEntry *m_txt_airactres;
    wxTextEntry *m_txt_statreactres;
    wxTextEntry *m_txt_rotreactres;
    wxTextEntry *m_txt_magnreactres;
    wxTextEntry *m_txt_poles;
    wxTextEntry *m_txt_srccurr;

    wxStaticText *m_lbl_srcvolt;
    wxStaticText *m_lbl_progvolt;
    wxStaticText *m_lbl_progcurr;
    wxStaticText *m_lbl_progfreq;
    wxStaticText *m_lbl_statactres;
    wxStaticText *m_lbl_rotactres;
    wxStaticText *m_lbl_airactres;
    wxStaticText *m_lbl_statreactres;
    wxStaticText *m_lbl_rotreactres;
    wxStaticText *m_lbl_magnreactres;
    wxStaticText *m_lbl_poles;
    wxStaticText *m_lbl_srccurr;

    wxCheckBox *m_chk_switchreact;
    wxCheckBox *m_chk_iscurrpers;

    mpWindow *m_mpw_outgraph;

    DECLARE_EVENT_TABLE();
};

// MainFrame
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    virtual ~MainFrame();

    void onExit(wxCommandEvent &evt);

private:
    MainPanel *m_panel;

    DECLARE_EVENT_TABLE();
};

DECLARE_APP(MainApp)
#endif // __BASE_H
