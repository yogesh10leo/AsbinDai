//#include "./stdafx.h"
#include "stdafx.h"
#include "Logger.h"

#define MEMSET(pval) ZeroMemory(pval, sizeof(pval));
#define MAX_BUFFER 4096


void WritePerformanceLog(CString strMsg)
{
	DWORD t1;
	CString strPerformanceLog;

	t1 = GetTickCount();
	strPerformanceLog.Format(L"PerformanceLog : %s Time: %d",strMsg, t1) ;
	_WRITE_ERROR_LOG(L"%s", strPerformanceLog);
}

// implementation
Logger::Logger( LPCTSTR module_name ) : 
show_time(false), show_module(true), show_thread(false), output_prohibited(true)
{

#if defined(_DEBUG)
	output_prohibited = false;
#else

	DWORD dwData = 0;
	CString strSection, strKey;
	/// Code dependancy
	strSection.Format(_T("Software\\Samsung\\PrintingAlert\\"));
	strKey.Format(_T("LiteSMDebugMode"));

	HKEY hKey = NULL;
	LONG lRet = ::RegOpenKeyEx(HKEY_CURRENT_USER, strSection, 0, KEY_READ, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		DWORD dwType = 0;
		DWORD dwSize = sizeof(DWORD);
		lRet = ::RegQueryValueEx(hKey, strKey, 0, &dwType, (LPBYTE)&dwData, &dwSize);
		::RegCloseKey(hKey);
		if (lRet != ERROR_SUCCESS)
		{
			output_prohibited = true;
		}
		else
		{
			output_prohibited = dwData ? false : true;			
		}
	} //if
#endif

	memset(module_string, 0, sizeof(module_string));
	memset(function_string, 0, sizeof(function_string));

	int eCount = COUNT_OF_ELEMENTS(module_string);
	_tcscpy_s(module_string, eCount, module_name);
	UnformattedReport(_T("%s --> ENTER"), module_string);

	m_hUserLogFile = NULL;
	m_bCreateUserLog = false;

#ifdef _DEBUG
	//CreateUserLogFile();
#endif

}

Logger::~Logger()
{
	CloseUserLogFile();

	UnformattedReport(_T("%s <-- LEAVE"), module_string);
}

void Logger::CloseUserLogFile()
{
	if(m_hUserLogFile && m_bCreateUserLog)
	{
		CloseHandle(m_hUserLogFile);
		m_hUserLogFile = NULL;
	}
}

bool Logger::CreateUserLogFile(LPCTSTR lpLogFileName,bool bSetStdOut)
{
#ifdef _DEBUG
	HANDLE hFile = INVALID_HANDLE_VALUE;
	TCHAR szModuleFile[MAX_PATH];
	TCHAR szTempDir[MAX_PATH];
	TCHAR szLogFile[MAX_PATH];
	TCHAR *pFileName = NULL, *pDest = NULL;

	MEMSET(szModuleFile);
	MEMSET(szTempDir);
	MEMSET(szLogFile);

	bool bRetval = true;

	try
	{
		if(lpLogFileName && _tcslen(lpLogFileName))
		{
			_tcscpy_s(szLogFile,lpLogFileName);
		}
		else
		{
			if(!GetModuleFileName(NULL,szModuleFile,sizeof(szModuleFile)))
			{
				throw (bool)false;
			}

			pFileName = _tcsrchr(szModuleFile,'\\');

			if(pFileName)
			{
				pFileName ++;
			}
			else
			{
				pFileName = DEF_LOG_FILE;
			}

			GetTempPath(MAX_PATH,szTempDir);

			_stprintf_s(szLogFile,_T("%s%s.log"),szTempDir, pFileName);
		}


		// Search backward. 		
		MEMSET(szTempDir);
		//ZeroMemory(szTempDir, sizeof(szTempDir));

		pDest = _tcsrchr(szLogFile, '\\');	
		_tcsncpy_s(szTempDir,  szLogFile, pDest - szLogFile + 1);


		//CFileOperation::CreateDir(szTempDir);

		if(!CreateDirectory(szTempDir,NULL))
		{
			if (GetLastError() != ERROR_ALREADY_EXISTS)
			{
				return false;
			}			
		}

		hFile = CreateFile(	szLogFile,
			GENERIC_WRITE|GENERIC_READ,
			FILE_SHARE_READ,
			NULL, 
			CREATE_ALWAYS, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			DWORD dwError = GetLastError();

			throw (bool)false;
		}

		m_hUserLogFile = hFile;

		m_bCreateUserLog = true;

		if(bSetStdOut)
		{
			if(SetStdHandle(STD_OUTPUT_HANDLE,hFile))
			{
				WriteUserLog(_T("Set Standard Out Handle Success"));
			}
		}

		bRetval = true;
	}
	catch(bool b)
	{
		bRetval = b;
	}
	catch (...)
	{
		OutputDebugString(_T("An error occurred in CreateLogFile"));
		bRetval = false;
	}

	return bRetval;

#endif
	return FALSE;
}


void Logger::WriteUserLog(LPTSTR format,...)
{

	TCHAR szBuffer[MAX_BUFFER];

	if (m_hUserLogFile == NULL)
	{
		return;
	}

	va_list args; 

	try
	{
		MEMSET(szBuffer);

		va_start(args, format); 

		_vstprintf_s(szBuffer, format, args); /* Format the string */

		va_end(args);
		
#ifdef _DEBUG
	WriteLog(szBuffer);
#endif

		// Output Debug String

	}
	catch(...)
	{
		//CUtl::MyDebugString(_T(" ==!!!!!! An exception was occurred  !!!!!!!== [CLog::Write]"));
		OutputDebugString(_T(" ==!!!!!! An exception was occurred  !!!!!!!== [Logger::WriteUserLog]"));
	}
}

void Logger::WriteLog(LPCTSTR szMsg)
{
#ifdef _DEBUG
	TCHAR szDebugInfo[ DEBUG_REPORT_LENGTH ];
	TCHAR szBuffer[MAX_BUFFER];
	
	try
	{
		MEMSET(szDebugInfo);
		MEMSET(szBuffer);
/*
		if ( OutputProhibited() )
		{
			return;
		}

*/	
		bool need_separator = false;

		//if ( ShowTime() )
		{
			TCHAR time_string[ 256 ];
			CurrentTimeStamp( &time_string );		

			_tcscat_s( szDebugInfo, _T("[") );
			_tcscat_s( szDebugInfo, time_string );
			_tcscat_s( szDebugInfo, _T("]") );

			need_separator = true;
		}

		//if ( ShowModule() )
		{
			if ( need_separator )
			{
				_tcscat_s( szDebugInfo, _T(" ") );
			}
			_tcscat_s( szDebugInfo, ModuleStamp() );
			need_separator = true;
		}

		//if ( ShowThread() )
		{
			if ( need_separator )
			{
				_tcscat_s( szDebugInfo, _T(" ") );
			}
			TCHAR task_string[ 128 ];
			CurrentTaskStamp( &task_string );

			_tcscat_s( szDebugInfo, _T("{") );
			_tcscat_s( szDebugInfo, task_string );
			_tcscat_s( szDebugInfo, _T("}") );
			need_separator = true;
		}

		if ( need_separator )
		{
			_tcscat_s( szDebugInfo, _T(": ") );
		}
		
		_stprintf_s(szBuffer,_T("%s %s\r\n"),szDebugInfo,szMsg);

		OutputDebugString(szBuffer);
		OutputDebugString(_T("\n"));

		WriteToFile(szBuffer);		
	}
	catch (...)
	{
		OutputDebugString(_T(" ==!!!!!! An exception was occurred  !!!!!!!== [Logger::WriteLog]"));
	}
#endif
}


BOOL Logger::WriteToFile(LPCTSTR szMsg)
{
	BOOL bRetval = FALSE;
	DWORD dwcbByte = 0;

	HANDLE hLog = NULL;

	char szBuffer[MAX_BUFFER];

	if(m_hUserLogFile)
	{
		hLog = m_hUserLogFile;
	}
	else
	{		
		hLog = m_hUserLogFile = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	if(hLog == NULL) return false;

	try
	{
		SetEndOfFile(hLog);

		MEMSET(szBuffer);

		
#ifdef _UNICODE
		WideCharToMultiByte( CP_ACP, 0, szMsg, -1,szBuffer, sizeof(szBuffer), NULL, NULL );
		bRetval = WriteFile(hLog,szBuffer,(DWORD)strlen(szBuffer),&dwcbByte,NULL);
#else
		bRetval = WriteFile(hLog,szMsg,(DWORD)strlen(szMsg),&dwcbByte,NULL);
#endif

		
	}
	catch(...)
	{
		OutputDebugString(_T(" ==!!!!!! An exception was occurred  !!!!!!!== [Logger::WriteToFile]"));
	}

	return bRetval;
}

void Logger::FormattedReport( IN LPCTSTR format, ... )
{
	if ( OutputProhibited() )
	{
		return;
	}
	va_list args;
	va_start( args, format ) ;
	vFormattedReport( format, args );
	va_end( args );
}

void Logger::UnformattedReport( IN LPCTSTR format, ... )
{	
	if ( OutputProhibited() )
	{
		return;
	}
	va_list args;
	va_start( args, format ) ;
	vUnformattedReport( format, args );
	va_end( args );
}

bool Logger::vFormattedReport( 
									 IN LPCTSTR format, 
									 IN va_list argptr 
									 )
{
	if ( OutputProhibited() )
	{
		return true;
	}

	TCHAR output_array[ DEBUG_REPORT_LENGTH ];
	output_array[0] = 0;

	bool need_separator = false;

	if ( ShowTime() )
	{
		TCHAR time_string[ 256 ];
		CurrentTimeStamp( &time_string );		

		_tcscat_s( output_array, _T("[") );
		_tcscat_s( output_array, time_string );
		_tcscat_s( output_array, _T("]") );

		need_separator = true;
	}

	if ( ShowModule() )
	{
		if ( need_separator )
		{
			_tcscat_s( output_array, _T(" ") );
		}
		_tcscat_s( output_array, ModuleStamp() );
		need_separator = true;
	}

	if ( ShowThread() )
	{
		if ( need_separator )
		{
			_tcscat_s( output_array, _T(" ") );
		}
		TCHAR task_string[ 128 ];
		CurrentTaskStamp( &task_string );

		_tcscat_s( output_array, _T("{") );
		_tcscat_s( output_array, task_string );
		_tcscat_s( output_array, _T("}") );
		need_separator = true;
	}

	if ( need_separator )
	{
		_tcscat_s( output_array, _T(": ") );
	}

	TCHAR message[ DEBUG_REPORT_LENGTH ] = {0};

	_vstprintf_s(message, format, argptr);


	_tcscat_s( output_array, message );
	_tcscat_s( output_array, _T("\n"));

	OutputDebugString( output_array );

	return true;
}


bool Logger::vUnformattedReport( 
									   IN LPCTSTR format, 
									   IN va_list argptr 
									   )
{
	if ( OutputProhibited() )
	{
		return true;
	}

	TCHAR message[ DEBUG_REPORT_LENGTH ] = {0};

	_vstprintf( message, format, argptr );

	OutputDebugString( message );


	return true;
}

bool Logger::ShowTime() const 
{ 
	// ::printf( (show_time) ? " t+": " t-");  
	return show_time; 
}

bool Logger::ShowModule() const 
{ 
	// ::printf( (show_module) ? " m+": " m-");  
	return show_module; 
}

bool Logger::ShowThread() const 
{ 
	// ::printf( (show_thread) ? " h+": " h-"); 
	return show_thread;  
}

bool Logger::OutputProhibited() const 
{
	return output_prohibited;
}
