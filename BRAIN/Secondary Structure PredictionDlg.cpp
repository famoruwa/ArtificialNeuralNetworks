// Secondary Structure PredictionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Brain.h"
#include "Secondary Structure Prediction.h"
#include "Secondary Structure PredictionDlg.h"
#include ".\secondary structure predictiondlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSecondaryStructurePredictionDlg dialog



CSecondaryStructurePredictionDlg::CSecondaryStructurePredictionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSecondaryStructurePredictionDlg::IDD, pParent)
	, currentWeight(0)
	, addSeqWeight(0.5)
	, m_currentTest(_T(""))
	, m_currentOutput(_T("Output.txt"))
	, m_numSteps(2)
	, m_currentInputWeight(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSecondaryStructurePredictionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WEIGHT, currentWeight);
	DDV_MinMaxDouble(pDX, currentWeight, 0, 1);
	DDV_MinMaxDouble(pDX, addSeqWeight, 0, 1);
	DDX_Control(pDX, IDC_BRAINLIST, myBrainList);
	DDX_Text(pDX, IDC_EDIT4, m_currentOutput);
	DDX_Text(pDX, IDC_STEPS, m_numSteps);
	DDV_MinMaxInt(pDX, m_numSteps, 1, 1000);
	DDX_Text(pDX, IDC_WEIGHT2, m_currentInputWeight);
	DDV_MinMaxDouble(pDX, m_currentInputWeight, 0, 1);
	DDX_Control(pDX, IDC_LIST3, myInputControl);
}

BEGIN_MESSAGE_MAP(CSecondaryStructurePredictionDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADDBRAIN, OnBnClickedAddbrain)
	ON_BN_CLICKED(IDC_SETWEIGHT, OnBnClickedSetweight)
	ON_BN_CLICKED(IDC_REMOVEBRAIN, OnBnClickedRemovebrain)
	ON_BN_CLICKED(IDC_BROWSE2, OnBnClickedBrowse2)
	ON_BN_CLICKED(IDC_PREDICT, OnBnClickedPredict)
	ON_BN_CLICKED(IDC_SETSTEPS, OnBnClickedSetsteps)
	ON_BN_CLICKED(IDC_ADDINPUT, OnBnClickedAddinput)
	ON_BN_CLICKED(IDC_REMOVEINPUT, OnBnClickedRemoveinput)
	ON_BN_CLICKED(IDC_SETWEIGHT2, OnBnClickedSetweight2)
END_MESSAGE_MAP()


// CSecondaryStructurePredictionDlg message handlers

BOOL CSecondaryStructurePredictionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	myBrainList.InsertColumn(0,"Brain",0,110,0);
	myBrainList.InsertColumn(1,"Neurons",0,60,0);
	myBrainList.InsertColumn(2,"Inputs",0,60,0);
	myBrainList.InsertColumn(3,"Outputs",0,60,0);
	myBrainList.InsertColumn(4,"Weight",0,94,0);
	myBrainList.InsertColumn(5,"Steps",0,70,0);
	ListView_SetExtendedListViewStyle(myBrainList,LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	myInputControl.InsertColumn(0,"Input",0,180,0);
	myInputControl.InsertColumn(1,"Total Length",0,100,0);
	myInputControl.InsertColumn(2,"Individual Length",0,114,0);
	myInputControl.InsertColumn(3,"Weight",0,60,0);
	ListView_SetExtendedListViewStyle(myInputControl,LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSecondaryStructurePredictionDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSecondaryStructurePredictionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSecondaryStructurePredictionDlg::OnBnClickedAddbrain()
{
	CString brainFile = "";
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Brain\0*.brain\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) {
		brainFile = ofn.lpstrFile;
	}

	if(brainFile == "")
	{
		return;
	}

	Brain* aBrain = new Brain(brainFile.GetBuffer());
	myBrains.push_back(aBrain);
	myWeights.push_back(1);
	mySteps.push_back(2);

	UpdateBrainControl();
}

void CSecondaryStructurePredictionDlg::UpdateBrainControl()
{
	myBrainList.DeleteAllItems();
	CString brainName = "The Brain";
	CString numNeurons;
	CString numOutput;
	CString numInput;
	CString weight;
	CString steps;
	for(int i = 0; i < myBrains.size(); i++)
	{
		myBrainList.InsertItem(i,brainName);
		myBrainList.SetItemData(i,(LPARAM) myBrains[i]);
		numNeurons.Format("%d",myBrains[i]->myNumNeurons);
		numOutput.Format("%d",myBrains[i]->myNumOutputs);
		numInput.Format("%d",myBrains[i]->myNumInputs);
		weight.Format("%f",myWeights[i]);
		steps.Format("%f",mySteps[i]);
		myBrainList.SetItemText(i,1,numNeurons);
		myBrainList.SetItemText(i,2,numInput);
		myBrainList.SetItemText(i,3,numOutput);
		myBrainList.SetItemText(i,4,weight);
		myBrainList.SetItemText(i,5,steps);
	}
}


void CSecondaryStructurePredictionDlg::OnBnClickedSetweight()
{
	UpdateData();
	for(int i = 0; i < myBrainList.GetItemCount(); i++) 
	{
		if(ListView_GetCheckState(myBrainList.m_hWnd,i) ) 
		{
			myWeights[i] = currentWeight;
		}
	}

	UpdateBrainControl();
}

void CSecondaryStructurePredictionDlg::OnBnClickedRemovebrain()
{
	UpdateData();
	myBrains.clear();
	CString weight;
	char* temp;
	myWeights.clear();
	double x = 1000;
	for(int i = myBrainList.GetItemCount() - 1 ; i >= 0; i--) 
	{
		if(!ListView_GetCheckState(myBrainList.m_hWnd,i) ) 
		{
			myBrains.push_back((Brain*)myBrainList.GetItemData(i));
			weight = myBrainList.GetItemText(i,4);
			x = strtod((LPCSTR)weight,&temp);
			myWeights.push_back(x);
		}
	}

	UpdateBrainControl();
}

void CSecondaryStructurePredictionDlg::OnBnClickedBrowse2()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Input\0*.in\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) {
		m_currentOutput = ofn.lpstrFile;
	}

	UpdateData(FALSE);
}
void CSecondaryStructurePredictionDlg::OnBnClickedPredict()
{

	UpdateData();
	if(m_currentOutput == "") {
		MessageBox("Please select an output file");
		return;
	}

	CWaitCursor wait;

	vector< vector< double > > prediction;;
	vector< vector< double > > currentInput;
	vector< char > final;;
	vector< double > output;;
	double highest;
	int classification;
	prediction.resize(myInputs[0]->size());
	for(int i = 0; i < myInputs.size(); i++)
	{
		currentInput = *myInputs[i];
		for(int j = 0; j < currentInput.size(); j++) 
		{
			for(int ii = 0; ii < myBrains.size(); ii++)
			{
				myBrains[ii]->setStates(0);
				myBrains[ii]->forwardPropagate(mySteps[ii],currentInput[j]);
				myBrains[ii]->getOutput(output);
				highest = -5;
				for(int jj = 0; jj < output.size(); jj++)
				{
					if(output[jj] > highest)
					{
						highest = output[jj];
						classification = jj;
					}
				}
				if(ii == 0 && i == 0)
				{
					prediction[j].resize(output.size());
					for(int iii = 0; iii < prediction[j].size(); iii++)
					{
						prediction[j][iii] = 0;
					}
				}
				if(highest > -0.4 || output.size() > 3) {
					prediction[j][classification] += myWeights[ii];
				}
			}
		}
	}

	fstream file;
	file.open(m_currentOutput.GetBuffer(),ios_base::out);



	for(int jjj = 0; jjj < prediction.size(); jjj++) {

		highest = -5;
		for(int iiii = 0; iiii < prediction[jjj].size(); iiii++)
		{
			if(prediction[jjj][iiii] >= highest)
			{
				highest = prediction[jjj][iiii];
				classification = iiii;
			}
		}
		if(prediction[jjj].size() == 8)
		{
			switch(classification) {
			case 0:
				final.push_back('S');
				break;
			case 1:
				final.push_back('G');
				break;
			case 2:
				final.push_back('E');
				break;
			case 3:
				final.push_back('T');
				break;
			case 4:
				final.push_back('B');
				break;
			case 5:
				final.push_back('H');
				break;
			case 6:
				final.push_back('I');
				break;
			case 7:
				final.push_back(' ');
				break;
			default:
				final.push_back('X');
				break;
			}
		}
		if(prediction[jjj].size() == 4)
		{
			switch(classification) {
			case 0:
				final.push_back('H');
				break;
			case 1:
				final.push_back('E');
				break;
			case 2:
				final.push_back('C');
				break;
			case 3:
				final.push_back(' ');
				break;
			default:
				final.push_back('X');
				break;
			}
		}
		if(prediction[jjj].size() == 3)
		{
			switch(classification) {
			case 0:
				final.push_back('H');
				break;
			case 1:
				final.push_back('E');
				break;
			case 2:
				final.push_back('C');
				break;
			default:
				final.push_back('X');
				break;
			}
		}
		if(prediction[jjj].size() == 2)
		{
			if(highest < 0.1) 
			{
				final.push_back('C');
			} else
			{
				switch(classification) {
				case 0:
					final.push_back('H');
					break;
				case 1:
					final.push_back('E');
					break;
				default:
					final.push_back('X');
					break;
				}
			}
		}
	}

	for(int jjjj = 0; jjjj < final.size(); jjjj++)
	{
		file << final[jjjj];
	}

	file.close();
	

}

void CSecondaryStructurePredictionDlg::OnBnClickedSetsteps()
{
	UpdateData();
	for(int i = 0; i < myBrainList.GetItemCount(); i++) 
	{
		if(ListView_GetCheckState(myBrainList.m_hWnd,i) ) 
		{
			mySteps[i] = m_numSteps;
		}
	}

	UpdateBrainControl();
}

void CSecondaryStructurePredictionDlg::UpdateInputControl()
{
	myInputControl.DeleteAllItems();
	CString totalLength;
	CString individualLength;
	CString weight;
	for(int i = 0; i < myInputs.size(); i++)
	{
		myInputControl.InsertItem(i,myInputNames[i]);
		myInputControl.SetItemData(i,(LPARAM) myInputs[i]);
		totalLength.Format("%d",myInputs[i]->size());
		individualLength.Format("%d",myInputs[i]->at(0).size());
		weight.Format("%f",myInputWeights[i]);
		myInputControl.SetItemText(i,1,totalLength);
		myInputControl.SetItemText(i,2,individualLength);
		myInputControl.SetItemText(i,3,weight);
	}
}

void CSecondaryStructurePredictionDlg::OnBnClickedAddinput()
{
	OPENFILENAME ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	//
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	//
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "Input\0*.in\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	if (GetOpenFileName(&ofn)==TRUE) {
		myInputNames.push_back(ofn.lpstrFile);
	}

	vector< vector< double > >* input = new vector< vector< double > >();
	Brain::readInputFile(myInputNames[myInputNames.size()-1].GetBuffer(),*input);

	myInputs.push_back(input);
	myInputWeights.push_back(1);

	UpdateInputControl();
}

void CSecondaryStructurePredictionDlg::OnBnClickedRemoveinput()
{
	UpdateData();
	myInputs.clear();
	myInputNames.clear();
	char* temp;
	myInputWeights.clear();
	for(int i = myInputControl.GetItemCount() - 1 ; i >= 0; i--) 
	{
		if(!ListView_GetCheckState(myInputControl.m_hWnd,i) ) 
		{
			myInputs.push_back((vector< vector< double > >*)myInputControl.GetItemData(i));
			myInputNames.push_back(myInputControl.GetItemText(i,0));
			myInputWeights.push_back(strtod((LPCSTR)myInputControl.GetItemText(i,3),&temp));
		}
	}

	UpdateInputControl();
}

void CSecondaryStructurePredictionDlg::OnBnClickedSetweight2()
{
	UpdateData();
	for(int i = 0; i < myInputControl.GetItemCount(); i++) 
	{
		if(ListView_GetCheckState(myInputControl.m_hWnd,i) ) 
		{
			myInputWeights[i] = m_currentInputWeight;
		}
	}

	UpdateInputControl();
}
