
#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include "stdafx.h"

#pragma warning(disable : 4251)
#pragma warning(disable : 4786)
#pragma warning(disable : 4996)

#define DEF_LOG_FILE _T("USDAGNT.log")

const unsigned long DEBUG_REPORT_LENGTH = 1024;

#define COUNT_OF_ELEMENTS(x) (sizeof(x)/sizeof((x)[0]))

typedef enum LOG_TYPE {LOG_USER,LOG_ERROR} ;

void WritePerformanceLog(CString strMsg);

class Logger
{
public:
	Logger( LPCTSTR module_name );
	virtual ~Logger();

	//File ///////////////////////////////
	bool CreateUserLogFile(LPCTSTR lpLogFileName = NULL, bool bSetStdOut = true);

	void WriteUserLog(LPTSTR format,...);

	void CloseUserLogFile();

	//Debug view ///////////////////////////////
	virtual void FormattedReport( IN LPCTSTR format, ... ); // Report
	virtual void UnformattedReport( IN LPCTSTR format, ... ); // Report2

	bool vFormattedReport( IN LPCTSTR format, IN va_list argptr );
	bool vUnformattedReport( IN LPCTSTR format, IN va_list argptr );	



	


private:

	//File ///////////////////////////////
	//HANDLE m_hLogFile;
	HANDLE m_hUserLogFile;
	bool m_bCreateUserLog;

	void WriteLog(LPCTSTR szMsg);
	//void WriteInformation(LPCTSTR lpProgramName,LOG_TYPE logType=LOG_ERROR);
	BOOL WriteToFile(LPCTSTR szMsg);
	
	//Debug view ///////////////////////////////
	TCHAR module_string[ 1024 ];
	TCHAR function_string[ 1024 ];
	LPCTSTR ModuleStamp() const { return module_string; }
	LPCTSTR FunctionStamp() const { return function_string; }

	bool show_time;
	bool show_module;
	bool show_thread;
	bool output_prohibited;

	bool ShowTime() const; 

	bool ShowModule() const;
	bool ShowThread() const;
	bool OutputProhibited() const;
	
	void CurrentTimeStamp( TCHAR (* time_string)[ 256 ] )
	{

		LCID locale = LOCALE_USER_DEFAULT;

		DWORD how_to_show = TIME_FORCE24HOURFORMAT;

		CONST SYSTEMTIME * USE_CURRENT_LOCAL_SYSTEM_TIME = NULL;
		CONST SYSTEMTIME * time = USE_CURRENT_LOCAL_SYSTEM_TIME;

		LPCTSTR USE_FORMAT_OF_SPECIFIED_LOCALE = NULL;
		LPCTSTR format_picture = USE_FORMAT_OF_SPECIFIED_LOCALE;

		int size_of_string_buffer = sizeof(*time_string);

		if ( 0 == ::GetTimeFormat( 
			locale, how_to_show, time, format_picture, 
			*time_string,  size_of_string_buffer
			) )
		{
			*time_string[0] = 0; 
		}
	}


	unsigned long UniqueThreadId() { return ::GetCurrentThreadId(); }

	void CurrentTaskStamp( TCHAR (* task_string)[ 128 ] )
	{
		_stprintf( *task_string, _T("%lX"), UniqueThreadId() );
	}

};
#endif // LOGGER_H_INCLUDED