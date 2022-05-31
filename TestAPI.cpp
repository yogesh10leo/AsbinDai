// TestAPI.cpp : implementation file
//
*this is asbin
#include "stdafx.h"
#include "TestPrinterUtility.h"
#include "TestAPI.h"
#include "afxdialogex.h"

#define DEVICE_MDL	L"2950"
#define MAX_PAPER_SIZES	(48) // 0x30
CString stTrayNames[]=
{
	L"Tray 1",
	L"MP Tray",
	L"Tray 2",
	L"Tray 3",
	L"Tray 4",
	L"Manual feeder",
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
// CTestAPI dialog

//IMPLEMENT_DYNAMIC(CTestAPI, CDialog)

CTestAPI::CTestAPI(CWnd* pParent /*=NULL*/)
	: CDialog(CTestAPI::IDD, pParent)
	, m_OutputInfo(_T(""))
{

}
CTestAPI::~CTestAPI()
{
}
void CTestAPI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_API, m_Combo_API_List);
	DDX_Text(pDX, IDC_LIST2, m_OutputInfo);
}
BOOL CTestAPI::OnInitDialog()
{
    CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB(255, 255,255));

	// Fetch MDL from ini file

	TCHAR szSourceFilePath[MAX_PATH];
	CString m_strSourcePath = L"";
	CString m_strConfigFile = L"";

	GetModuleFileName(NULL, szSourceFilePath, sizeof(szSourceFilePath));
	CString strTemp(szSourceFilePath);
	m_strSourcePath = strTemp.Left(strTemp.ReverseFind('\\'));
	m_strConfigFile.Format(L"%s\\PrinterUtility.ini", m_strSourcePath);
	
	memset(m_szMDL, 0, sizeof(m_szMDL));
	::GetPrivateProfileString(L"CONFIG", L"MDL", _T(""), m_szMDL, sizeof(m_szMDL), m_strConfigFile);

	if(_tcslen(m_szMDL) <= 0)
		_tcscpy(m_szMDL, DEVICE_MDL);

	m_Combo_API_List.ResetContent();
		
	size_t nIndex = 0;
	m_Combo_API_List.InsertString(nIndex++, _T("IsPrinterOnline"));
	m_Combo_API_List.InsertString(nIndex++, _T("IsPrinterAvailableForPrinting"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetPrinterStatus"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetImagingUnitStatus"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetTrayStatus"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetPrinterCounter"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetJobQueuedCount"));
	m_Combo_API_List.InsertString(nIndex++, _T("WakeUpPrinter"));
	m_Combo_API_List.InsertString(nIndex++, _T("ClearPrintJob"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetDeviceIDInfo"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetTonerLevel"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetStatusMessage"));
	m_Combo_API_List.InsertString(nIndex++, _T("GetSerialNumber"));

	m_Combo_API_List.SetCurSel(0);
	
	 return TRUE;
}
HBRUSH CTestAPI::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
   	 return m_brush;

}

BEGIN_MESSAGE_MAP(CTestAPI, CDialog)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CTestAPI::OnBnClickedOk)
END_MESSAGE_MAP()


// CTestAPI message handlers
void CTestAPI::OnBnClickedOk()
{	
	int nSelectedIndex = m_Combo_API_List.GetCurSel();
		
	if(nSelectedIndex == 0) //IsPrinterOnline
	{
		CheckPrinterConnection();
	}
	else if(nSelectedIndex == 1)//IsPrinterAvailableForPrinting
	{
		CheckPrinterStatusForPrinting();
	}
	else if(nSelectedIndex == 2)//GetPrinterStatus
	{
		GetPrinterStatus();
	}
	else if(nSelectedIndex == 3)//GetImagingUnitStatus
	{
		GetImagingUnits();
	}
	else if(nSelectedIndex == 4)//GetTrayStatus
	{
		GetTrayinfo();
	}
	else if(nSelectedIndex == 5)//GetPrinterCounter
	{
		GetPrinterCounter();		
	}
	else if(nSelectedIndex == 6)//GetJobQueuedCount
	{
		GetJobQueuedCount();
	}
	else if(nSelectedIndex == 7)//WakeUpPrinter
	{
		WakeUpPrinter();
	}
	else if(nSelectedIndex == 8)//ClearPrintJob
	{
		ClearPrintJob();
	}
	else if(nSelectedIndex == 9)
	{
		GetDeviceIDInfo();	
	}
	else if(nSelectedIndex == 10)//GetImagingUnitStatus
	{
		GetTonerLevel();
	}
	else if(nSelectedIndex == 11)//GetImagingUnitStatus
	{
		GetStatusMessage();
	}
	else if (nSelectedIndex == 12)//GetImagingUnitStatus
	{
		GetSerialNumber();
	}
	
}

typedef DWORD (WINAPI	*LPFN_IsPrinterOnline)(TCHAR *strMDL);
void CTestAPI::CheckPrinterConnection()
{
	_WRITE_ERROR_LOG(L"[%d]CheckPrinterConnection() start", __LINE__);

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]%s cannot call", __LINE__, szPrinterUtilityDllname);
	}

	LPFN_IsPrinterOnline	pfnLPFN_IsPrinterOnline = (LPFN_IsPrinterOnline)GetProcAddress(hModule, "IsPrinterOnline");

	int printerStatus = 0;
	printerStatus = pfnLPFN_IsPrinterOnline(m_szMDL);

	FreeLibrary(hModule);

	// check the printer status
	m_OutputInfo = _T("API called = IsPrinterOnline\n\n\t");
	if(printerStatus == 0)
	{
		m_OutputInfo += "Current status of device = Disconnected / Offline";
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += "Current status of device = Connected / Online";
	}

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]CheckPrinterConnection() End", __LINE__);
}

typedef DWORD (WINAPI	*LPFN_IsPrinterAvailable)(TCHAR *strMDL);
void CTestAPI::CheckPrinterStatusForPrinting()
{
	_WRITE_ERROR_LOG(L"[%d]CheckPrinterStatusForPrinting() start", __LINE__);

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if (NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]%s cannot call", __LINE__, szPrinterUtilityDllname);
	}

	LPFN_IsPrinterAvailable	pfnLPFN_IsPrinterAvailableForPrint = (LPFN_IsPrinterAvailable)GetProcAddress(hModule, "IsPrinterAvailable");

	int printerStatus = 0;
	printerStatus = pfnLPFN_IsPrinterAvailableForPrint(m_szMDL);

	FreeLibrary(hModule);

	// check the printer status
	m_OutputInfo = _T("API called = IsPrinterAvailableForPrinting\n\n\t");
	if(printerStatus == 0)
	{
		m_OutputInfo += "Current status of device = Not ready for printing";
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += "Current status of device = Ready for printing";
	}
	else if(printerStatus == -1)
	{
		m_OutputInfo += "Current status of device = I/O Error, Not ready for printing";
	}


	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]CheckPrinterStatusForPrinting() End", __LINE__);
}

typedef DWORD (WINAPI	*LPFN_GetStatusMessage)(TCHAR *strMDL, TCHAR * outStatusMsg,TCHAR * outGeneralMsg,TCHAR *outGeneralMsgVersion);
void CTestAPI::GetStatusMessage()
{
	_WRITE_ERROR_LOG(L"[%d]GetPrinterStatus() start", __LINE__);

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if (NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]%s cannot call", __LINE__, szPrinterUtilityDllname);
	}

	LPFN_GetStatusMessage	pfnLPFN_GetStatusMessage = (LPFN_GetStatusMessage)GetProcAddress(hModule, "GetStatusMessage");
	TCHAR outGeneralMsg[512];
	TCHAR outStatusMsg[512];
	TCHAR outGeneralMsgVersion[512];

	memset(outGeneralMsg,'\0',512);
	memset(outStatusMsg,'\0',512);
	memset(outGeneralMsgVersion,'\0',512);


	int printerStatus = 0;
	printerStatus = pfnLPFN_GetStatusMessage(m_szMDL, outStatusMsg, outGeneralMsg,outGeneralMsgVersion);
	m_OutputInfo = _T("API called = GetPrinterStatus\n\n\t");	
	m_OutputInfo += "Current status of device = ";
	if(printerStatus == 0)
	{
		m_OutputInfo += _T("Not available");
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += outStatusMsg;
		m_OutputInfo += "\n\tGeneral Message = ";
		m_OutputInfo += outGeneralMsg;
		m_OutputInfo += "\n\tGeneral Message Version = ";
		m_OutputInfo += outGeneralMsgVersion;
	}	
	else if(printerStatus == -1)
	{
		m_OutputInfo += _T("I/O error");
	}
	
	m_OutputInfo += _T("\n");
	
	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetPrinterStatus() End", __LINE__);

}
typedef DWORD (WINAPI	*LPFN_GetPrinterStatus)(TCHAR *strMDL, TCHAR * outStatus);
void CTestAPI::GetPrinterStatus()
{
	_WRITE_ERROR_LOG(L"[%d]GetPrinterStatus() start", __LINE__);

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if (NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]%s cannot call", __LINE__, szPrinterUtilityDllname);
	}

	LPFN_GetPrinterStatus	pfnLPFN_GetPrinterStatus = (LPFN_GetPrinterStatus)GetProcAddress(hModule, "GetPrinterStatus");
	TCHAR outStatus[512];
	int printerStatus = 0;
	printerStatus = pfnLPFN_GetPrinterStatus(m_szMDL, outStatus);
	m_OutputInfo = _T("API called = GetPrinterStatus\n\n\t");	
	m_OutputInfo += "Current status of device = ";
	if(printerStatus == 0)
	{
		m_OutputInfo += _T("Not available");
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += outStatus;
	}	
	else if(printerStatus == -1)
	{
		m_OutputInfo += _T("I/O error");
	}
	
	m_OutputInfo += _T("\n");
	
	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetPrinterStatus() End", __LINE__);

}

typedef BOOL (WINAPI	*LPFN_GetTrayPaperSize)(TCHAR *strMDL, BYTE* pBuffer);
typedef BOOL (WINAPI	*LPFN_GetTrayPaperLevel)(TCHAR *strMDL, BYTE* pBuffer);
void CTestAPI::GetTrayinfo()
{

	_WRITE_ERROR_LOG(L"[%d]GetTrayinfo() start", __LINE__);

	// Load Liabrary

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if (NULL == hModule)
	{
		_WRITE_ERROR_LOG(L"[%d]%s cannot call", __LINE__, szPrinterUtilityDllname);
		return;
	}

		

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

		

		if(i ==5)//manual feeder wont support paper level and paper level data is for tray1,mptray,tray2,tray3,tray4,tray5 ..same is handled
		{
			m_TrayInfo[i].strTrayPaperLevel.Format(L"%s", STR_NOT_SUPPORTED); 			
		}
		else
		{
			j+=2;
			k+=4;
		}



		
	}

	// Display Tray Info
	UpdateOutputWindow();

	_WRITE_ERROR_LOG(L"Tray  Size  Level");
	for(int i=0; i<MAX_TRAYS_SUPORTED; i++)
	{
		_WRITE_ERROR_LOG(L"%s  %s  %s", m_TrayInfo[i].strTrayName, m_TrayInfo[i].strTrayPaperSize, m_TrayInfo[i].strTrayPaperLevel);
	}

	_WRITE_ERROR_LOG(L"[%d]GetTrayinfo() End", __LINE__);
}

typedef BOOL (WINAPI	*LPFN_GetImagingUnitStatus)(TCHAR *strMDL, int * ary);
void CTestAPI::GetImagingUnits()
{

	_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() start", __LINE__);

	// Load Liabrary

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) return;


	LPFN_GetImagingUnitStatus	pfnLPFN_GetImagingUnitStatus = (LPFN_GetImagingUnitStatus)GetProcAddress(hModule, "GetImagingUnitStatus");
	
	int ary[8] ={0,0,0,0};
	if(TRUE == pfnLPFN_GetImagingUnitStatus(m_szMDL, ary))
	{
		_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() is TRUE", __LINE__);
	}
	else
	{
		_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() is FALSE", __LINE__);
	}

	FreeLibrary(hModule);

	// int to CString
	
	CString K ; K.Format( _T("%d"), ary[4]);
	CString C ; C.Format( _T("%d"), ary[5]);
	CString M ; M.Format( _T("%d"), ary[6]);
	CString Y ; Y.Format( _T("%d"), ary[7]);
		
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = GetImagingUnitStatus\n\n");
	if(ary[0] == 0xff)
		m_OutputInfo += _T("\tK\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tK\t:\t") + K + _T("%\n");
	if(ary[1] == 0xff)
		m_OutputInfo += _T("\tC\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tC\t:\t") + C + _T("%\n");
	if(ary[2] == 0xff)
		m_OutputInfo += _T("\tM\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tM\t:\t") + M + _T("%\n");
	if(ary[3] == 0xff)
		m_OutputInfo += _T("\tY\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tY\t:\t") + Y + _T("%\n");

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() End", __LINE__);
}

typedef BOOL (WINAPI	*LPFN_GetPrinterCounter)(TCHAR *strMDL, int * ary);
void CTestAPI::GetPrinterCounter()
{

	_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() start", __LINE__);

	// Load Liabrary

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) return;


	LPFN_GetPrinterCounter	pfnLPFN_GetPrinterCounter = (LPFN_GetPrinterCounter)GetProcAddress(hModule, "GetPrinterCounter");
	int ary[4] ={0,0,0,0};
	int printerStatus = 0;
	printerStatus = pfnLPFN_GetPrinterCounter(m_szMDL, ary);	
		
	// int to CString
	
	CString PrintCount ; PrintCount.Format( _T("%d"), * ary);
	FreeLibrary(hModule);
			
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = GetPrinterCounter\n\n\t");
	
	
	if(printerStatus == 0) // int printerStatus = 0;
	{
		m_OutputInfo += _T("Print impression\t: Failed\t") + PrintCount;;
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += _T("Print impression\t:\t") + PrintCount;
	}
	else if(printerStatus == -1)
	{
		m_OutputInfo += "Result = I/O Error.";
	}	
	

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetPrinterCounter() End", __LINE__);
}

typedef BOOL (WINAPI	*LPFN_GetJobQueuedCount)(TCHAR *strMDL, int * ary);
void CTestAPI::GetJobQueuedCount()
{

	_WRITE_ERROR_LOG(L"[%d]GetJobQueuedCount() start", __LINE__);

	// Load Liabrary

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) FALSE;


	LPFN_GetJobQueuedCount	pfnLPFN_GetJobQueuedCount = (LPFN_GetJobQueuedCount)GetProcAddress(hModule, "GetJobQueuedCount");
	
	int ary[4] ={0,0,0,0};
	int printerStatus = 0;
	printerStatus = pfnLPFN_GetJobQueuedCount(m_szMDL, ary);

	// int to CString	
	CString PrintCount ; PrintCount.Format( _T("%d"), * ary);
	FreeLibrary(hModule);
			
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = GetJobQueuedCount\n\n\t");
	

	if(1 == printerStatus)
	{
		_WRITE_ERROR_LOG(L"[%d]GetJobQueuedCount() is TRUE", __LINE__);
		m_OutputInfo += _T("Job Queued Count\t:\t") + PrintCount;
	}
	else if(0 == printerStatus)
	{
		_WRITE_ERROR_LOG(L"[%d]GetJobQueuedCount() is FALSE", __LINE__);
		m_OutputInfo += _T("Job Queued Count\t:\tFailed.\n");
	}
	else if(-1 == printerStatus)
	{
		m_OutputInfo += _T("Job Queued Count\t:\ I/O error.\n");
	}	

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetJobQueuedCount() End", __LINE__);
}

typedef DWORD (WINAPI	*LPFN_WakeUpPrinter)(TCHAR *strMDL);
void CTestAPI::WakeUpPrinter()
{
	_WRITE_ERROR_LOG(L"[%d]ClearPrintJob() start", __LINE__);

	// Load Liabrary	

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) FALSE;


	LPFN_WakeUpPrinter	pfnLPFN_WakeUpPrinter = (LPFN_WakeUpPrinter)GetProcAddress(hModule, "WakeUpPrinter");
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = WakeUpPrinter\n\n\t");
	if(TRUE == pfnLPFN_WakeUpPrinter(m_szMDL))
	{
		m_OutputInfo += _T("Wake up command accepted by device.\n");
	}
	else
	{
		m_OutputInfo += _T("Wake up command not accepted by device.\n");
	}

	FreeLibrary(hModule);

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]ClearPrintJob() End", __LINE__);

}

typedef DWORD (WINAPI	*LPFN_ClearPrintJob)(TCHAR *strMDL);
void CTestAPI::ClearPrintJob()
{
	_WRITE_ERROR_LOG(L"[%d]ClearPrintJob() start", __LINE__);

	// Load Liabrary	

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) return;	

	LPFN_ClearPrintJob	pfnLPFN_ClearPrintJob = (LPFN_ClearPrintJob)GetProcAddress(hModule, "ClearPrintJob");
	int printerStatus = 0;
	printerStatus = pfnLPFN_ClearPrintJob(m_szMDL);

	FreeLibrary(hModule);
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = ClearPrintJob\n\n\t");
	
	if(printerStatus == 0)
	{
		m_OutputInfo += _T("Clear print job not accepted by device.\n");
	}
	else if(printerStatus == 1)
	{
		m_OutputInfo += _T("Clear print job accepted by device.\n");
	}
	else if(printerStatus == -1)
	{
		m_OutputInfo += "Result = I/O Error.";
	}	
	
	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]ClearPrintJob() End", __LINE__);

}

typedef DWORD (WINAPI	*LPFN_GetDeviceIDInfo)(TCHAR * outDeviceID);
void CTestAPI::GetDeviceIDInfo()
{
	_WRITE_ERROR_LOG(L"[%d]GetDeviceIDInfo() start", __LINE__);

	// Load Liabrary	

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) return;	

	LPFN_GetDeviceIDInfo	pfnLPFN_GetDeviceIDInfo = (LPFN_GetDeviceIDInfo)GetProcAddress(hModule, "GetDeviceIDInfo");
	TCHAR outDeviceID[512];
	int printerDeviceID = 0;
	printerDeviceID = pfnLPFN_GetDeviceIDInfo(outDeviceID);
	m_OutputInfo = _T("API called = GetDeviceIDInfo\n\n\t");	
	m_OutputInfo += "DeviceIDInfo is = ";
	if(printerDeviceID == 0)
	{
		m_OutputInfo += _T("Not available");
	}
	else if(printerDeviceID == 1)
	{
		m_OutputInfo += outDeviceID;
	}	
	else if(printerDeviceID == -1)
	{
		m_OutputInfo += _T("I/O error");
	}
	FreeLibrary(hModule);
	
	m_OutputInfo += _T("\n");
	
	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetDeviceIDInfo() End", __LINE__);
}

typedef DWORD(WINAPI	*LPFN_GetSerialNumber)(TCHAR *strMDL,TCHAR * outSerialNumber);
void CTestAPI::GetSerialNumber()
{
	_WRITE_ERROR_LOG(L"[%d]GetSerialNumber() start", __LINE__);

	// Load Liabrary	

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if (NULL == hModule) return;

	LPFN_GetSerialNumber	pfnLPFN_GetSerialNumber = (LPFN_GetSerialNumber)GetProcAddress(hModule, "GetSerialNumber");
	TCHAR outSerialNumber[512];
	int printerSerialNumber = 0;
	printerSerialNumber = pfnLPFN_GetSerialNumber(m_szMDL,outSerialNumber);
	m_OutputInfo = _T("API called = GetSerialNumber\n\n\t");
	m_OutputInfo += "Serial Number of Device = ";
	if (printerSerialNumber == 0)
	{
		m_OutputInfo += _T("Not available");
	}
	else if (printerSerialNumber == 1)
	{
		m_OutputInfo += outSerialNumber;
	}
	else if (printerSerialNumber == -1)
	{
		m_OutputInfo += _T("I/O error");
	}
	
	FreeLibrary(hModule);

	m_OutputInfo += _T("\n");

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetSerialNumber() End", __LINE__);
}
///////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// HELPER METHODS ////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

CString CTestAPI::GetTraySizeStringFromByte(BYTE iPaperSize)
{

	BYTE	iTempSize = 0;
	int		index = 0;
	CString strPaperSizeString;

	for(int i=0; i < MAX_PAPER_SIZES; i++)
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

void CTestAPI::UpdateOutputWindow()
{
	_WRITE_ERROR_LOG(L"[%d]UpdateList() start", __LINE__);

	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = GetTrayStatus\n\n");
	for(int i = 0; i < MAX_TRAYS_SUPORTED ; i++)
	{
		if(i==5)
			m_OutputInfo +=  _T("\t") + m_TrayInfo[i].strTrayName + _T("\t:\tPaper size = " + m_TrayInfo[i].strTrayPaperSize + _T(",\tPaper level = ") + m_TrayInfo[i].strTrayPaperLevel + _T("\n"));		
		else
		m_OutputInfo +=  _T("\t") + m_TrayInfo[i].strTrayName + _T("\t\t:\tPaper size = " + m_TrayInfo[i].strTrayPaperSize + _T(",\tPaper level = ") + m_TrayInfo[i].strTrayPaperLevel + _T("\n"));		
	}

	UpdateData(FALSE);
	_WRITE_ERROR_LOG(L"[%d]UpdateList() End", __LINE__);
}

typedef BOOL (WINAPI	*LPFN_GetTonerLevel)(TCHAR *strMDL, int * ary);
void CTestAPI::GetTonerLevel()
{

	_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() start", __LINE__);

	// Load Liabrary

#ifdef _WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility64.dll");
#else //_WIN64
	LPCWSTR szPrinterUtilityDllname = _T("PrinterUtility.dll");
#endif //!_WIN64

	// Load Liabrary
	HMODULE hModule = LoadLibrary(szPrinterUtilityDllname);
	if(NULL == hModule) return;


	LPFN_GetTonerLevel	pfnLPFN_GetTonerLevel = (LPFN_GetTonerLevel)GetProcAddress(hModule, "GetTonerLevel");
	
	int ary[4] ={0,0,0,0};
	if(TRUE == pfnLPFN_GetTonerLevel(m_szMDL, ary))
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
		
	m_OutputInfo = _T("");
	m_OutputInfo = _T("API called = GetTonerLevel\n\n");
	if(ary[0] == 0xff)
		m_OutputInfo += _T("\tK\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tK\t:\t") + K + _T("%\n");
	if(ary[1] == 0xff)
		m_OutputInfo += _T("\tC\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tC\t:\t") + C + _T("%\n");
	if(ary[2] == 0xff)
		m_OutputInfo += _T("\tM\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tM\t:\t") + M + _T("%\n");
	if(ary[3] == 0xff)
		m_OutputInfo += _T("\tY\t:\tNot installed\n");
	else
		m_OutputInfo += _T("\tY\t:\t") + Y + _T("%\n");

	UpdateData(FALSE);

	_WRITE_ERROR_LOG(L"[%d]GetTonerLevel() End", __LINE__);
}