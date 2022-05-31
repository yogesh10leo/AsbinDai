#include "Logger.h"

#define DEFINE_DEBUG_LOGGER(name) /*inline*/ Logger & \
	USE_DEBUG_REPORTER_HELPER() \
	{ \
		static Logger logger( _T(#name) ); \
		return logger; \
	}

#define USE_DEBUG_LOGGER() extern Logger & \
	USE_DEBUG_REPORTER_HELPER();

// use those macros in .cpp files 
#define _MYTRACE USE_DEBUG_REPORTER_HELPER().FormattedReport
#define _WRITE_ERROR_LOG USE_DEBUG_REPORTER_HELPER().WriteUserLog


#ifdef _DEBUG
//#define _FUNCTION_LOG Logger func_log(_T(__FUNCSIG__));
#define _FUNCTION_LOG Logger func_log(_T(__FUNCTION__));
#else
#define _FUNCTION_LOG Logger func_log(_T(__FUNCTION__));
#endif