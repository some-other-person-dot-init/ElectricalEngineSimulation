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

    ID_TEXT_SRCVOLTAGE = 200,
    ID_TEXT_PROGCURR = 201,

    ID_LABEL_SRCVOLTAGE = 300,
    ID_LABEL_PROGCURR = 301,

    ID_PLOT_OUTGRAPH = 900,
};

// MainPanel
class MainPanel : public wxPanel
{
public:
    MainPanel(wxFrame *frame);

    void onRunButton(wxCommandEvent &evt);

    virtual ~MainPanel();
};

// MainFrame
class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

    MainPanel *m_panel;

    virtual ~MainFrame();

    DECLARE_EVENT_TABLE()
};

DECLARE_APP(MainApp)
#endif // __BASE_H
