// Secondary Structure PredictionDlg.h : header file
//

#pragma once

#include "Brain.h"
#include "afxcmn.h"


// CSecondaryStructurePredictionDlg dialog
class CSecondaryStructurePredictionDlg : public CDialog
{
// Construction
public:
	CSecondaryStructurePredictionDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SECONDARYSTRUCTUREPREDICTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

private:
	vector< Brain* > myBrains;
	vector< double > myWeights;
	vector< double > mySteps;
	vector< CString > myInputNames;
	vector< vector< vector< double > >* > myInputs;
	vector< double > myInputWeights;

	void UpdateBrainControl();
	void UpdateInputControl();

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	double currentWeight;
	afx_msg void OnEnChangeEdit1();
	double addSeqWeight;
	CListCtrl myBrainList;
	afx_msg void OnBnClickedAddbrain();
	afx_msg void OnBnClickedSetweight();
	afx_msg void OnBnClickedRemovebrain();
	CString m_currentTest;
	afx_msg void OnBnClickedBrowse2();
	CString m_currentOutput;
	afx_msg void OnBnClickedPredict();
	int m_numSteps;
	afx_msg void OnBnClickedSetsteps();
	afx_msg void OnBnClickedAddinput();
	afx_msg void OnBnClickedRemoveinput();
	afx_msg void OnBnClickedSetweight2();
	double m_currentInputWeight;
	CListCtrl myInputControl;
};
