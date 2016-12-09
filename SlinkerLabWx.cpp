// wxWidgets "Hello world" Program
// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/menu.h>
#include <wx/string.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/statusbr.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/choice.h>
#include <wx/gbsizer.h>
#include <wx/sizer.h>
#include <wx/statbox.h>
#include <wx/button.h>
#include <wx/frame.h>
#include <wsc.h>
#include <wsc_comPort.h>
#include <comportlist.h>
#include "APTAPI.h"
#include "Servo.h"
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
#include <thread>




class MyApp : public wxApp
{
public:
	virtual bool OnInit();
	

private:
	
};

class MyFrame : public wxFrame
{
public:
	MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	int count;
	WSC_ComPort port;
	//ComPortList list;
	bool stop = false;
	std::string str;

    
private:
	long numHWUnits, *serials;
	void OnExit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnGoButton(wxCommandEvent& event);
	void initMeasure();
	void ConnectKeithley();
	void DisconnectKeithley();
	void makeServos();
	void delServos();
	void KISweep();
	std::vector <std::string> KVSweep();
	wxMenuBar* m_menubar2;
	wxMenu* m_menu2;
	wxStatusBar* m_statusBar2;
	wxStaticText* m_staticText12;
	wxStaticText* m_staticText14;
	wxTextCtrl* m_textCtrl5;
	wxChoice* m_choice2;
	wxStaticText* m_staticText16;
	wxTextCtrl* m_textCtrl6;
	wxChoice* m_choice3;
	wxStaticText* m_staticText17;
	wxTextCtrl* m_textCtrl7;
	wxStaticText* m_staticText18;
	wxStaticText* m_staticText19;
	wxTextCtrl* m_textCtrl8;
	wxChoice* m_choice4;
	wxStaticText* m_staticText20;
	wxTextCtrl* m_textCtrl9;
	wxChoice* m_choice5;
	wxStaticText* m_staticText21;
	wxTextCtrl* m_textCtrl10;
	wxStaticText* m_staticText22;
	wxTextCtrl* m_textCtrl11;
	wxStaticText* m_staticText23;
	wxTextCtrl* m_textCtrl12;
	wxStaticText* m_staticText24;
	wxTextCtrl* m_textCtrl13;
	wxStaticText* m_staticText25;
	wxTextCtrl* m_textCtrl14;
	wxStaticText* m_staticText27;
	wxChoice* m_choice6;
	wxStaticText* m_staticText28;
	wxChoice* m_choice7;
	wxStaticText* m_staticText29;
	wxStaticText* m_staticText30;
	wxTextCtrl* m_textCtrl15;
	wxChoice* m_choice8;
	wxButton* m_button15;
	wxButton *button1;
	
	long xSN, ySN, zSN, xQuant, yQuant, xSep, ySep, dwn, up;
	Servo *xServo, *yServo, *zServo;
	wxDECLARE_EVENT_TABLE();
	std::ofstream output;

	};

enum
{
	//ID_Hello = 1,
	ID_GoButton,
	ID_sweepType,

};

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
//EVT_MENU(ID_Hello, MyFrame::OnHello)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_BUTTON(ID_GoButton, MyFrame::OnGoButton)
wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame *frame = new MyFrame("SlinkerLabWx", wxPoint(50, 50), wxSize(600, 800));		//create wxwidgets frame
	frame->Show(true);
	return true;
}

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	up = 5;
	long err = APTInit(); // initialize thorlabs motor controller software

						  //implement error handling

	err = GetNumHWUnitsEx((long)31, &numHWUnits); // how many servos are connected?
												  //error handling

	serials = new long[numHWUnits];
	wxArrayString strSerials;

	for (int i = 0; i < numHWUnits; i++)
	{
		err = GetHWSerialNumEx((long)31, (long)i, &serials[i]);  //populate array 'serials' with serial number of connected servos
															 //error handling
	}

	for (int i = 0; i < numHWUnits; i++)
	{
		strSerials.Add( wxString::FromAscii ( (std::to_string(serials[i])).c_str()) );

	}

	wxArrayString unitsI;
	unitsI.Add(wxString::FromUTF8("mA"));
	unitsI.Add(wxString::FromUTF8((std::string("\xCE\xBC") + "A").c_str()));
	unitsI.Add(wxString::FromUTF8("nA"));
	unitsI.Add(wxString::FromUTF8("pA"));

	wxArrayString unitsV;
	unitsV.Add(wxString::FromUTF8("mV"));
	unitsV.Add(wxString::FromUTF8((std::string("\xCE\xBC") + "V").c_str()));
	unitsV.Add(wxString::FromUTF8("nV"));
	unitsV.Add(wxString::FromUTF8("pV"));

	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	m_menubar2 = new wxMenuBar(0);
	m_menu2 = new wxMenu();
	m_menu2->AppendSeparator();

	m_menubar2->Append(m_menu2, wxT("File"));

	this->SetMenuBar(m_menubar2);

	m_statusBar2 = this->CreateStatusBar(1, wxST_SIZEGRIP, wxID_ANY);
	wxGridSizer* gSizer1;
	gSizer1 = new wxGridSizer(2, 2, 0, 0);

	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Measurments")), wxVERTICAL);

	wxBoxSizer* bSizer12;
	bSizer12 = new wxBoxSizer(wxVERTICAL);
/*
	m_staticText12 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Current Sweep"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText12->Wrap(-1);
	bSizer12->Add(m_staticText12, 0, wxALL, 5);

	wxGridBagSizer* gbSizer3;
	gbSizer3 = new wxGridBagSizer(0, 0);
	gbSizer3->SetFlexibleDirection(wxBOTH);
	gbSizer3->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText14 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText14->Wrap(-1);
	gbSizer3->Add(m_staticText14, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl5 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer3->Add(m_textCtrl5, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice2Choices(unitsI);
	m_choice2 = new wxChoice(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice2Choices, 0);
	m_choice2->SetSelection(0);
	gbSizer3->Add(m_choice2, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 5);


	bSizer12->Add(gbSizer3, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer5;
	gbSizer5 = new wxGridBagSizer(0, 0);
	gbSizer5->SetFlexibleDirection(wxBOTH);
	gbSizer5->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText16 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText16->Wrap(-1);
	gbSizer5->Add(m_staticText16, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl6 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer5->Add(m_textCtrl6, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	/*
	wxArrayString m_choice3Choices(unitsI);
	m_choice3 = new wxChoice(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice3Choices, 0);
	m_choice3->SetSelection(0);
	gbSizer5->Add(m_choice3, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 5);
	//close comment here

	bSizer12->Add(gbSizer5, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer6;
	gbSizer6 = new wxGridBagSizer(0, 0);
	gbSizer6->SetFlexibleDirection(wxBOTH);
	gbSizer6->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText17 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Resolution"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText17->Wrap(-1);
	gbSizer6->Add(m_staticText17, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl7 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer6->Add(m_textCtrl7, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer12->Add(gbSizer6, 1, wxEXPAND, 5);
	*/
	m_staticText18 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Voltage"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText18->Wrap(-1);
	bSizer12->Add(m_staticText18, 0, wxALL, 5);

	wxGridBagSizer* gbSizer7;
	gbSizer7 = new wxGridBagSizer(0, 0);
	gbSizer7->SetFlexibleDirection(wxBOTH);
	gbSizer7->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText19 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Start"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText19->Wrap(-1);
	gbSizer7->Add(m_staticText19, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl8 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer7->Add(m_textCtrl8, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice4Choices(unitsV);
	m_choice4 = new wxChoice(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice4Choices, 0);
	m_choice4->SetSelection(0);
	gbSizer7->Add(m_choice4, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 5);


	bSizer12->Add(gbSizer7, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer8;
	gbSizer8 = new wxGridBagSizer(0, 0);
	gbSizer8->SetFlexibleDirection(wxBOTH);
	gbSizer8->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText20 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Stop"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText20->Wrap(-1);
	gbSizer8->Add(m_staticText20, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl9 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer8->Add(m_textCtrl9, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	/*wxArrayString m_choice5Choices(unitsV);
	m_choice5 = new wxChoice(sbSizer1->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice5Choices, 0);
	m_choice5->SetSelection(0);
	gbSizer8->Add(m_choice5, wxGBPosition(0, 2), wxGBSpan(1, 1), wxALL, 5);*/


	bSizer12->Add(gbSizer8, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer9;
	gbSizer9 = new wxGridBagSizer(0, 0);
	gbSizer9->SetFlexibleDirection(wxBOTH);
	gbSizer9->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText21 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Resolution"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText21->Wrap(-1);
	gbSizer9->Add(m_staticText21, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl10 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer9->Add(m_textCtrl10, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	bSizer12->Add(gbSizer9, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer17;
	gbSizer17 = new wxGridBagSizer(0, 0);
	gbSizer17->SetFlexibleDirection(wxBOTH);
	gbSizer17->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText30 = new wxStaticText(sbSizer1->GetStaticBox(), wxID_ANY, wxT("Sweep Count"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText30->Wrap(-1);
	gbSizer17->Add(m_staticText30, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl15 = new wxTextCtrl(sbSizer1->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer17->Add(m_textCtrl15, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);

	bSizer12->Add(gbSizer17, 1, wxEXPAND, 5);


	sbSizer1->Add(bSizer12, 1, wxEXPAND, 5);


	bSizer6->Add(sbSizer1, 1, wxEXPAND, 5);


	gSizer1->Add(bSizer6, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Sample Dimensions")), wxVERTICAL);

	wxBoxSizer* bSizer15;
	bSizer15 = new wxBoxSizer(wxVERTICAL);

	wxGridBagSizer* gbSizer10;
	gbSizer10 = new wxGridBagSizer(0, 0);
	gbSizer10->SetFlexibleDirection(wxBOTH);
	gbSizer10->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText22 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("X contacts"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText22->Wrap(-1);
	gbSizer10->Add(m_staticText22, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl11 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer10->Add(m_textCtrl11, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer15->Add(gbSizer10, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer11;
	gbSizer11 = new wxGridBagSizer(0, 0);
	gbSizer11->SetFlexibleDirection(wxBOTH);
	gbSizer11->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText23 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Y contacts"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText23->Wrap(-1);
	gbSizer11->Add(m_staticText23, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl12 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer11->Add(m_textCtrl12, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer15->Add(gbSizer11, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer12;
	gbSizer12 = new wxGridBagSizer(0, 0);
	gbSizer12->SetFlexibleDirection(wxBOTH);
	gbSizer12->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText24 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("X Separation"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText24->Wrap(-1);
	gbSizer12->Add(m_staticText24, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl13 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer12->Add(m_textCtrl13, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer15->Add(gbSizer12, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer13;
	gbSizer13 = new wxGridBagSizer(0, 0);
	gbSizer13->SetFlexibleDirection(wxBOTH);
	gbSizer13->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText25 = new wxStaticText(sbSizer2->GetStaticBox(), wxID_ANY, wxT("Y Separation"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText25->Wrap(-1);
	gbSizer13->Add(m_staticText25, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	m_textCtrl14 = new wxTextCtrl(sbSizer2->GetStaticBox(), wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0);
	gbSizer13->Add(m_textCtrl14, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer15->Add(gbSizer13, 1, wxEXPAND, 5);


	sbSizer2->Add(bSizer15, 1, wxEXPAND, 5);


	bSizer9->Add(sbSizer2, 1, wxEXPAND, 5);


	gSizer1->Add(bSizer9, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer3;
	sbSizer3 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Servo Control")), wxVERTICAL);

	wxBoxSizer* bSizer17;
	bSizer17 = new wxBoxSizer(wxVERTICAL);

	wxGridBagSizer* gbSizer14;
	gbSizer14 = new wxGridBagSizer(0, 0);
	gbSizer14->SetFlexibleDirection(wxBOTH);
	gbSizer14->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText27 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("X Serial"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText27->Wrap(-1);
	gbSizer14->Add(m_staticText27, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice6Choices(strSerials);
	m_choice6 = new wxChoice(sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice6Choices, 0);
	m_choice6->SetSelection(0);
	gbSizer14->Add(m_choice6, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer17->Add(gbSizer14, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer15;
	gbSizer15 = new wxGridBagSizer(0, 0);
	gbSizer15->SetFlexibleDirection(wxBOTH);
	gbSizer15->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText28 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Y Serial"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText28->Wrap(-1);
	gbSizer15->Add(m_staticText28, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice7Choices(strSerials);
	m_choice7 = new wxChoice(sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice7Choices, 0);
	m_choice7->SetSelection(1);
	gbSizer15->Add(m_choice7, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer17->Add(gbSizer15, 1, wxEXPAND, 5);

	wxGridBagSizer* gbSizer16;
	gbSizer16 = new wxGridBagSizer(0, 0);
	gbSizer16->SetFlexibleDirection(wxBOTH);
	gbSizer16->SetNonFlexibleGrowMode(wxFLEX_GROWMODE_SPECIFIED);

	m_staticText29 = new wxStaticText(sbSizer3->GetStaticBox(), wxID_ANY, wxT("Z Serial"), wxDefaultPosition, wxDefaultSize, 0);
	m_staticText29->Wrap(-1);
	gbSizer16->Add(m_staticText29, wxGBPosition(0, 0), wxGBSpan(1, 1), wxALL, 5);

	wxArrayString m_choice8Choices(strSerials);
	m_choice8 = new wxChoice(sbSizer3->GetStaticBox(), wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice8Choices, 0);
	m_choice8->SetSelection(2);
	gbSizer16->Add(m_choice8, wxGBPosition(0, 1), wxGBSpan(1, 1), wxALL, 5);


	bSizer17->Add(gbSizer16, 1, wxEXPAND, 5);


	sbSizer3->Add(bSizer17, 1, wxEXPAND, 5);


	bSizer10->Add(sbSizer3, 1, wxEXPAND, 5);


	gSizer1->Add(bSizer10, 1, wxEXPAND, 5);

	wxBoxSizer* bSizer11;
	bSizer11 = new wxBoxSizer(wxVERTICAL);

	wxStaticBoxSizer* sbSizer4;
	sbSizer4 = new wxStaticBoxSizer(new wxStaticBox(this, wxID_ANY, wxT("Output")), wxVERTICAL);

	m_button15 = new wxButton(sbSizer4->GetStaticBox(), wxID_ANY, wxT("Go!"), wxDefaultPosition, wxDefaultSize, 0);
	sbSizer4->Add(m_button15, 0, wxALL, 5);


	bSizer11->Add(sbSizer4, 1, wxEXPAND, 5);


	gSizer1->Add(bSizer11, 1, wxEXPAND, 5);


	this->SetSizer(gSizer1);
	this->Layout();

	this->Centre(wxBOTH);

	m_button15->Connect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler(MyFrame::OnGoButton), NULL, this);

}

void MyFrame::OnExit(wxCommandEvent& event)
{
	Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxMessageBox("SlinkerLabWx v0.1",
		"About SlinkerLabWx", wxOK | wxICON_INFORMATION);
}

/*void MyFrame::OnGoButton(wxCommandEvent& event)  //Go button clicked event
{
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	str = oss.str();
	int numSweeps = atoi(m_textCtrl15->GetLineText(0).ToStdString().c_str());
	output.open("sweep" + str + ".csv");
	long up = 5;
	std::vector < std::vector <std::string> > sweeps;
	//Connect to keithley
	ConnectKeithley();
	makeServos();

	
	//zServo->gotoAbsPos(up);
	xServo->gotoAbsPos(0);
	yServo->gotoAbsPos(0);
	//zServo->gotoHome();
	port.write("SYST:AZER ON");
	int count = 1;
	for (int i = 0; i < yQuant; i++)
	{
		for (int j = 0; j < xQuant; j++)
		{
			output << "Sample " << count++ << "\n";
			for (int i = 0; i < numSweeps; i++)
			{
				output << "Sweep " << i + 1 << ",,";
				//KISweep();
				sweeps.push_back(KVSweep());
			}
			output << "\n";
			for (int j = 0; j < sweeps[0].size(); j++)
			{
				for (int i = 0; i < sweeps.size(); i++)
				{
					output << sweeps[i][j] << ",";
				}
				output << "\n";
			}
			sweeps.clear();
			
			//zServo->gotoAbsPos(up);
			if (j != xQuant - 1)
			{
				xServo->step(xSep);
				Sleep(100);
			}
			else
				xServo->gotoHome();
			//zServo->gotoHome();
		}
		//zServo->gotoAbsPos(up);
		yServo->step(ySep);
		//zServo->gotoHome();
	}
	//zServo->gotoAbsPos(up);
	yServo->gotoAbsPos(0);
	
	output.close();
	DisconnectKeithley();
	//delServos();
}
*/

void MyFrame::OnGoButton(wxCommandEvent& event)
{
	if (port.connected)
	{
		stop = true;
		m_button15->SetBackgroundColour(wxNullColour);
		m_button15->SetLabel("Go!");
	}
	else
	{
		stop = false;
		std::thread t1( &MyFrame::initMeasure, this );
		m_button15->SetBackgroundColour(*wxRED);
		m_button15->SetLabel("Stop");
		t1.join();
	}
	
}

void MyFrame::initMeasure()
{
	//get timestamp for file name and open "sweepDATEtime.csv"
	auto t = std::time(nullptr);
	auto tm = *std::localtime(&t);
	std::ostringstream oss;
	oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
	str = oss.str();
	output.open("sweep" + str + ".csv");
	
	//create vector to store keithley output for formating
	int numSweeps = atoi(m_textCtrl15->GetLineText(0).ToStdString().c_str());
	std::vector < std::vector <std::string> > sweeps;

	//Connect to keithley and servo controllers
	ConnectKeithley();
	makeServos();


	zServo->gotoAbsPos(up);
	xServo->gotoAbsPos(0);
	yServo->gotoAbsPos(0);
	zServo->gotoAbsPos(dwn);
	port.write("SYST:AZER ON");
	int count = 1;
	for (int i = 0; i < yQuant; i++)
	{
		if (stop)
		{
			break;
		}
		for (int j = 0; j < xQuant; j++)
		{
			
			output << "Sample " << count++ << "\n";
			
			for (int i = 0; i < numSweeps; i++)
			{
				output << "Sweep " << i + 1 << ",,";
				//KISweep();
				sweeps.push_back(KVSweep());
				if (stop)
				{
					break;
				}
			}
			
			output << "\n";
			
			for (int j = 0; j < sweeps[0].size(); j++)
			{
				for (int i = 0; i < sweeps.size(); i++)
				{
					output << sweeps[i][j] << ",";
				}
				output << "\n";
			}
			sweeps.clear();
			if (stop)
			{
				break;
			}
			zServo->gotoAbsPos(up);
			if (j != xQuant - 1)
			{
				xServo->step(xSep);
				Sleep(100);
			}
			else
				xServo->gotoAbsPos(0);
			zServo->step( -1 * up);
		}
		zServo->step(up);
		yServo->step(ySep);
		zServo->step(-1 * up);
	}
	zServo->step(up);
	yServo->gotoAbsPos(0);
	xServo->gotoAbsPos(0);
	if(output.is_open())
		output.close();
	DisconnectKeithley();
	delServos();
	m_button15->SetBackgroundColour(wxNullColour);
	m_button15->SetLabel("Go!");
}

void MyFrame::ConnectKeithley()
{
	port.connect("ACPI\\PNP0501\\2", 9600, 2, 8, 'N', '\n', 4096, 4096);
}

void MyFrame::DisconnectKeithley()
{
	port.disconnect();
}

void MyFrame::makeServos()
{
	

	//get sample array dimensions 
	if (m_textCtrl11->IsSingleLine())
		xQuant = std::stoi(m_textCtrl11->GetLineText(0).ToStdString());
	else
		;//throw exception

	if (m_textCtrl12->IsSingleLine())
			yQuant = std::stoi(m_textCtrl12->GetLineText(0).ToStdString());
	else
			;//throw exception
	
	if (m_textCtrl13->IsSingleLine())
		xSep = std::stoi(m_textCtrl13->GetLineText(0).ToStdString());
	else
		;//throw exception

	if (m_textCtrl14->IsSingleLine())
		ySep = std::stoi(m_textCtrl14->GetLineText(0).ToStdString());
	else
		;//throw exception

	//get servo serials
	xSN = serials[m_choice6->GetCurrentSelection()];
	ySN = serials[m_choice7->GetCurrentSelection()];
	zSN = serials[m_choice8->GetCurrentSelection()];

	xServo = new Servo(xSN);
	yServo = new Servo(ySN);
	zServo = new Servo(zSN);

}

void MyFrame::delServos()
{
	delete xServo;
	delete yServo;
	delete zServo;
}

void MyFrame::KISweep()
{
	std::vector< std::vector<double> > measurements;
	int x = 0;
	int startoom;
	switch(m_choice2->GetSelection()){
	case 0: startoom = -3;
		break;
	case 1: startoom = -6;
		break;
	case 2: startoom = -9;
		break;
	case 3: startoom = -12;
		break;
	}

	double start = ( atof(m_textCtrl5->GetLineText(0).ToStdString().c_str()) ) * (double)(10 ^ startoom);
	double stop = ( atof(m_textCtrl6->GetLineText(0).ToStdString().c_str()) ) * (double)(10 ^ startoom);
	double res = (stop - start) / atof(m_textCtrl7->GetLineText(0).ToStdString().c_str());
	port.write("*RST");
	Sleep(1000);
	port.write("SOUR:FUNC:MODE CURR");
	port.write("SOUR:CURR:MODE FIX");
	//port.write("SENS:CURR:PROT ");
	port.write("OUTP ON");

	for (double i = start; i < stop; i += res)
	{
		port.write("SOUR:CURR " + std::to_string(i));
		Sleep(100);
		port.write("MEAS:VOLT?");
		std::string reply = port.read(5);
		measurements[x][0] = i;
		measurements[x][1] = atof(reply.c_str());
		x++;
	}
	port.write("OUTP OFF");
	std::ofstream output;
	output.open("sweep" + str + ".csv");
	output << "current,voltage\n";
	for (int i = 0; i < measurements.size(); i++)
	{
		output << std::to_string(measurements[x][0]) << "," << std::to_string(measurements[x][1]) << "\n";
	}
	output.close();
	
}

std::vector <std::string> MyFrame::KVSweep()
{
	std::vector<std::string> measurements;
	double startoom;
	switch (m_choice4->GetSelection()) {
	case 0: startoom = pow(10, -3);
		break;
	case 1: startoom = pow(10, -6);
		break;
	case 2: startoom = pow(10, -9);
		break;
	case 3: startoom = pow(10, -12);
		break;
	}

	double start = atof(m_textCtrl8->GetLineText(0).ToStdString().c_str())  * startoom;
	double stop = atof( m_textCtrl9->GetLineText(0).ToStdString().c_str() ) * startoom;
	double res = (stop - start) / atof(m_textCtrl10->GetLineText(0).ToStdString().c_str());
	port.write("*RST");
	Sleep(1000);
	port.write("SOUR:FUNC:MODE VOLT");
	port.write("SOUR:VOLT:MODE FIX");
	port.write("OUTP ON");
	
	for (double i = start; i < stop; i += res)
	{
		port.write("SOUR:VOLT " + std::to_string(i));
		Sleep(100);
		port.write("MEAS:CURR?");
		std::string reply = port.read(5);
		int pos = reply.find(",");
		pos = reply.find(",", pos+1);
		reply = reply.substr(0,pos);
		measurements.push_back(reply);
	}
	port.write("OUTP OFF");

	return measurements;

}
