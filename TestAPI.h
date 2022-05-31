#pragma once
#include "afxwin.h"
#include "resource.h"

// CTestAPI dialog

class CTestAPI : public CDialog
{
	//DECLARE_DYNAMIC(CTestAPI)

public:
	CTestAPI(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTestAPI();

// Dialog Data
	enum { IDD = IDD_DLG_TEST_API };
private:
	CBrush m_brush;
	TCHAR	m_szMDL[256];
	TRAY_INFO m_TrayInfo[MAX_TRAYS_SUPORTED];
	
	CString GetTraySizeStringFromByte(BYTE iPaperSize);
	void UpdateOutputWindow();

	void CheckPrinterConnection();
	void CheckPrinterStatusForPrinting();
	void GetTrayinfo();
	void GetImagingUnits();
	void GetTonerLevel();
	void GetPrinterCounter();
	void GetJobQueuedCount();
	void WakeUpPrinter();
	void GetPrinterStatus();
	void GetStatusMessage();
	void GetSerialNumber();

	void ClearPrintJob();
	void GetDeviceIDInfo();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_Combo_API_List;
	afx_msg void OnBnClickedOk();
	CString m_OutputInfo;
};
