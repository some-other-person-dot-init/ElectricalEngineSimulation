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
};

// MainPanel
class MainPanel : public wxPanel
{
public:
    MainPanel(wxFrame *frame);

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
