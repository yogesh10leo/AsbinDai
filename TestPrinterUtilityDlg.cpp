// TestPrinterUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TestPrinterUtility.h"
#include "TestPrinterUtilityDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define DEVICE_MDL	L"2950"
#define MAX_PAPER_SIZES	(48) // 0x30
CString stTrayNames[]=
{
	L"Tray 1",
	L"MP Tray",
	L"Tray 2",
	L"Tray 3",
	L"Tray 4",
	L"Tray 5"
};
TRAY_PAPER_SIZE stPaperSizes[] = 
{
	{0, L"Letter"},
	{1, L"Legal"},
	{2, L"Executive"},
	{3, L"Statement"},
	{4, L"Folio"},
	{5, L"US post"},
	{6, L"Index Card"},
	{7, L"No 10 Envelope"},
	{8, L"Monarch Envelope"},
	{9, L"Custom"},
	{0x0A, L"A4"},
	{0x0B, L"A5"},
	{0x0C, L"A6"},
	{0x0D, L"B5(ISO)"},
	{0x0E, L"B5(JIS)"},
	{0x0F, L"DL Envelope"},
	{0x10, L"C5 Envelope"},
	{0x11, L"C6 Envelope"},
	{0x12, L"Postcard 100x148"},
	{0x13, L""},
	{0x14, L""},
	{0x15, L""},
	{0x16, L"JPost"},
	{0x17, L"JPost_D"},
	{0x18, L"Letter R"},
	{0x19, L""},
	{0x1A, L"JIG B5 R"},
	{0x1B, L"A5 R"},
	{0x1C, L"Exec R"},
	{0x1D, L"State R"},
	{0x1E, L"A3 over"},
	{0x1F, L"B5 Env"},
	{0x20, L"Oficio"},
	{0x21, L"Odd"},
	{0x22, L"6 3/4 Envelope"},
	{0x23, L"No 9"},
	{0x24, L"B6"},
	{0x25, L"Hagaki"},
	{0x26, L"A6 R"},
	{0x27, L"post A6 R"},
	{0x28, L"JPost R"},
	{0x29, L"Postcard 4x6"},
	{0xFF, L"N/A"}
};

CString stDeviceStatus[]=
{
	L"Available",
	L"Not Available"
	L"N/A"
};


//#define DEVICE_MDL	L"CLX-3300 Series"

// CTestPrinterUtilityDlg dialog

typedef BOOL (WINAPI	*LPFN_GetTrayPaperSize)(TCHAR *strMDL, BYTE* pBuffer);
typedef BOOL (WINAPI	*LPFN_GetTrayPaperLevel)(TCHAR *strMDL, BYTE* pBuffer);
typedef DWORD (WINAPI	*LPFN_IsPrinterAvailable)(TCHAR *strMDL);
typedef BOOL (WINAPI	*LPFN_GetTonerLevel)(TCHAR *strMDL, int * ary);


CTestPrinterUtilityDlg::CTestPrinterUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestPrinterUtilityDlg::IDD, pParent)
{
	/*m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);*/
}

void CTestPrinterUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRAY_INFO, m_cListCtrl);
}

BEGIN_MESSAGE_MAP(CTestPrinterUtilityDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(ID_TRAYINFO, &CTestPrinterUtilityDlg::OnBnClickedTrayinfo)
	ON_BN_CLICKED(IDC_DEVICESTATUS, &CTestPrinterUtilityDlg::OnBnClickedDevicestatus)
	ON_BN_CLICKED(IDC_TONER_LEVEL, &CTestPrinterUtilityDlg::OnBnClickedTonerLevel)
END_MESSAGE_MAP()


// CTestPrinterUtilityDlg message handlers

BOOL CTestPrinterUtilityDlg::OnInitDialog()
{

	_WRITE_ERROR_LOG(L"[%d]OnInitDialog() - Start", __LINE__);

	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	m_brush.CreateSolidBrush(RGB(255, 255,255));

	// Fetch MDL from ini file
	

	TCHAR szSourceFilePath[MAX_PATH];
	CString m_strSourcePath = L"";
	CString m_strConfigFile = L"";

	GetModuleFileName(NULL, szSourceFilePath, sizeof(szSourceFilePath));
	CString strTemp(szSourceFilePath);
	m_strSourcePath = strTemp.Left(strTemp.ReverseFind('\\'));
	m_strConfigFile.Format(L"%s\\TestPrinterUtility.ini", m_strSourcePath);
	
	memset(m_szMDL, 0, sizeof(m_szMDL));
	::GetPrivateProfileString(L"CONFIG", L"MDL", _T(""), m_szMDL, sizeof(m_szMDL), m_strConfigFile);

	if(_tcslen(m_szMDL) <= 0)
		_tcscpy(m_szMDL, DEVICE_MDL);


	int iColSize = 115;
	ListView_SetExtendedListViewStyle(m_cListCtrl, LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_cListCtrl.InsertColumn(1, _T("Tray Name"), LVCFMT_CENTER, iColSize);
	m_cListCtrl.InsertColumn(2, _T("Paper Type"), LVCFMT_CENTER, iColSize);
	m_cListCtrl.InsertColumn(3, _T("Paper Level"), LVCFMT_CENTER, iColSize);

	// TODO: Add extra initialization here
	_WRITE_ERROR_LOG(L"[%d]OnInitDialog()", __LINE__);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

HBRUSH CTestPrinterUtilityDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   	 return m_brush;

}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTestPrinterUtilityDlg::OnPaint()
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
		/*dc.DrawIcon(x, y, m_hIcon);*/
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTestPrinterUtilityDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CTestPrinterUtilityDlg::OnBnClickedTrayinfo()
{

	_WRITE_ERROR_LOG(L"[%d]OnBnClickedTrayinfo() start", __LINE__);
	m_cListCtrl.DeleteAllItems();

	// Load Liabrary
	bool bSuccess = false;
	HMODULE hModule = LoadLibrary(_T("PrinterUtility.dll"));
	if(NULL == hModule)
		return;

	LPFN_GetTrayPaperSize	pfnGetTrayPaperSize = (LPFN_GetTrayPaperSize)GetProcAddress(hModule, "GetTrayPaperSize");
	LPFN_GetTrayPaperLevel	pfnGetTrayPaperLevel = (LPFN_GetTrayPaperLevel)GetProcAddress(hModule, "GetTrayPaperLevel");
	
	CString strTemp;

	BYTE pTrayPaperSize[255]= {0,};
	CString strTraySizeNType = L"";
	if(TRUE == pfnGetTrayPaperSize(m_szMDL, pTrayPaperSize))
	{
		_WRITE_ERROR_LOG(L"[%d]GetTrayPaperSize() is TRUE", __LINE__);
	}
	else
	{
		_WRITE_ERROR_LOG(L"[%d]GetTrayPaperSize() is FALSE", __LINE__);
		memset(pTrayPaperSize, 0xFF, sizeof(pTrayPaperSize));
	}

	BYTE pTrayPaperLevel[255]= {0,};
	CString strTrayPaperLevel = L"";
	if(TRUE == pfnGetTrayPaperLevel(m_szMDL, pTrayPaperLevel))
	{
		_WRITE_ERROR_LOG(L"[%d]GetTrayPaperLevel() is TRUE", __LINE__);
	}
	else
	{
		_WRITE_ERROR_LOG(L"[%d]GetTrayPaperLevel() is FALSE", __LINE__);
		memset(pTrayPaperLevel, 0xFF, sizeof(pTrayPaperLevel));
	}

	FreeLibrary(hModule);


	// Fill tray info in structure
	int iPaperLevel= 0;
	for(int i=0, j=0, k=2; i<MAX_TRAYS_SUPORTED; i++)
	{
		m_TrayInfo[i].strTrayName.Format(L"%s", stTrayNames[i]);
		m_TrayInfo[i].strTrayPaperSize.Format(L"%s", GetTraySizeStringFromByte(pTrayPaperSize[j]));

		memcpy(&iPaperLevel, &pTrayPaperLevel[k], 2);
		_WRITE_ERROR_LOG(L"Paper level 0x%X", iPaperLevel);

		if(pTrayPaperSize[j] == 0xFF)
		{
			m_TrayInfo[i].strTrayPaperLevel.Format(L"%s", STR_NOT_INSTALLED);
		}
		else if(iPaperLevel == 0xFFFF)
		{
			m_TrayInfo[i].strTrayPaperLevel.Format(L"%s", STR_NOT_SUPPORTED);
		}
		else if(iPaperLevel == 0)
		{
			m_TrayInfo[i].strTrayPaperLevel.Format(L"%s", STR_EMPTY);
		}

		else
		{
			m_TrayInfo[i].strTrayPaperLevel.Format(L"%s", STR_NOT_EMPTY);
		}

		j+=2;
		k+=4;
	}

	// Display Tray Info
	_WRITE_ERROR_LOG(L"Tray  Size  Level");
	for(int i=0; i<MAX_TRAYS_SUPORTED; i++)
	{
		_WRITE_ERROR_LOG(L"%s  %s  %s", m_TrayInfo[i].strTrayName, m_TrayInfo[i].strTrayPaperSize, m_TrayInfo[i].strTrayPaperLevel);
	}

	UpdateList();
	
	_WRITE_ERROR_LOG(L"[%d]OnBnClickedTrayinfo() End", __LINE__);
}


CString CTestPrinterUtilityDlg::GetTraySizeStringFromByte(BYTE iPaperSize)
{

	BYTE	iTempSize = 0;
	int		index = 0;
	CString strPaperSizeString;

	for(int i=0; i<MAX_PAPER_SIZES; i++)
	{
		iTempSize = stPaperSizes[i].iPaperSizeVal;
		if(iTempSize == iPaperSize)
		{
			_WRITE_ERROR_LOG(L"GetTraySizeStringFromByte %d %s", iPaperSize, stPaperSizes[index].iPaperSizeVal);
			strPaperSizeString.Format(L"%s", stPaperSizes[i].strPaperSizeName); 
			return strPaperSizeString; 
		}
	}

	return STR_NOT_INSTALLED;
}

void CTestPrinterUtilityDlg::UpdateList()
{
	_WRITE_ERROR_LOG(L"[%d]UpdateList() start", __LINE__);
	m_cListCtrl.DeleteAllItems();

	for(int i = 0; i < MAX_TRAYS_SUPORTED ; i++)
	{
		m_cListCtrl.InsertItem(i, m_TrayInfo[i].strTrayName);
		m_cListCtrl.SetItemText(i, 1, m_TrayInfo[i].strTrayPaperSize);
		m_cListCtrl.SetItemText(i, 2, m_TrayInfo[i].strTrayPaperLevel);
	}

	UpdateData(FALSE);
_WRITE_ERROR_LOG(L"[%d]UpdateList() End", __LINE__);
}


void CTestPrinterUtilityDlg::OnBnClickedDevicestatus()
{
	_WRITE_ERROR_LOG(L"[%d]OnBnClickedDevicestatus() start", __LINE__);
	// TODO: Add your control notification handler code here
	m_cListCtrl.DeleteAllItems();

	// Load Liabrary
	bool bSuccess = false;
	HMODULE hModule = LoadLibrary(_T("PrinterUtility.dll"));
	if(NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]PrinterUtility.dll cannot call", __LINE__);
	}

	LPFN_IsPrinterAvailable	pfnIsPrinterAvailable = (LPFN_IsPrinterAvailable)GetProcAddress(hModule, "IsPrinterAvailable");

	BYTE pPrinterStatus[255]= {0,};
	CString strTrayPaperLevel = L"";
	int printerStatus = 0;
	printerStatus = pfnIsPrinterAvailable(m_szMDL);

	FreeLibrary(hModule);

	// check the printer status

	m_cListCtrl.InsertItem(0, L"Current Staus");

	if(printerStatus == 1)
	{
		m_cListCtrl.SetItemText(0, 1, L"Available for printing");
	}
	else if(printerStatus == 0)
	{
		m_cListCtrl.SetItemText(0, 1, L"Not Available for printing");
	}
	else if(printerStatus == -1 )
	{
		m_cListCtrl.SetItemText(0, 1, L"I/O Error");
	}
	else
	{
		
	}


	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]OnBnClickedDevicestatus() End", __LINE__);
}


void CTestPrinterUtilityDlg::OnBnClickedTonerLevel()
{

	_WRITE_ERROR_LOG(L"[%d]OnBnClickedTonerLevel() start", __LINE__);
	// TODO: Add your control notification handler code here
	m_cListCtrl.DeleteAllItems();

	// Load Liabrary
	bool bSuccess = false;
	HMODULE hModule = LoadLibrary(_T("PrinterUtility.dll"));
	if(NULL == hModule) FALSE;


	LPFN_GetTonerLevel	pfnGetTonerLevel = (LPFN_GetTonerLevel)GetProcAddress(hModule, "GetTonerLevel");

	BYTE pTrayPaperSize[255]= {0,};
	BYTE pPrinterStatus[255]= {0,};
	CString strTrayPaperLevel = L"";

	int ary[4] ={0,0,0,0};
	if(TRUE == pfnGetTonerLevel(m_szMDL, ary))
	{
		_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() is TRUE", __LINE__);
	}
	else
	{
		_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() is FALSE", __LINE__);
	}

	FreeLibrary(hModule);

	// int to CString
	CString K ; K.Format( _T("%d"), ary[0]);
	CString C ; C.Format( _T("%d"), ary[1]);
	CString M ; M.Format( _T("%d"), ary[2]);
	CString Y ; Y.Format( _T("%d"), ary[3]);
	
	m_cListCtrl.InsertItem(0, L"K");
	m_cListCtrl.SetItemText(0, 1, K + L"%");

	m_cListCtrl.InsertItem(1, L"C");
	m_cListCtrl.SetItemText(1, 1, C + L"%");
	
	m_cListCtrl.InsertItem(2, L"M");
	m_cListCtrl.SetItemText(2, 1, M + L"%");
	
	m_cListCtrl.InsertItem(3, L"Y");	
	m_cListCtrl.SetItemText(3, 1, Y + L"%");
	
	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]OnBnClickedTonerLevel() End", __LINE__);
}
