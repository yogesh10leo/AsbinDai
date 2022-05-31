// TestPrinterUtilityDlg.h : header file
//

#pragma once


// CTestPrinterUtilityDlg dialog
class CTestPrinterUtilityDlg : public CDialog
{
// Construction
public:
	CTestPrinterUtilityDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_TestPrinterUtility_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedTrayinfo();
	CString GetTraySizeStringFromByte(BYTE iPaperSize);
	void UpdateList();

private:
	CBrush m_brush;
	CListCtrl m_cListCtrl;
	TRAY_INFO m_TrayInfo[MAX_TRAYS_SUPORTED];
	TCHAR	m_szMDL[256];
public:
	afx_msg void OnBnClickedDevicestatus();
	afx_msg void OnBnClickedTonerLevel();
};

