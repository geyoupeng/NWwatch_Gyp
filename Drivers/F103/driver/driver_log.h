#ifndef __DRIVER_LOG_H
#define __DRIVER_LOG_H
#include "drivers.h"

#define LOG_LEVEL	LOG_DEBUG
//日志级别
typedef enum{
	LOG_INFO,
	LOG_DEBUG,
	LOG_WARN,
	LOG_ERROR
}LogLevel;

void LogPrint(LogLevel level, const char *file, int line, const char *format,...);
void DataInit(void);
#ifdef LOG_DRIVER_USE
#if 0
#define LogInfo(format,...)												\
	do{																	\
		if(LOG_LEVEL>=LOG_INFO){										\
			LogPrint(LOG_INFO,__FILE__,__LINE__,format,##__VA_ARGS__);	\
		}																\
	}while(0)															\

#define LogDebug(format,...)											\
	do{																	\
		if(LOG_LEVEL>=LOG_DEBUG){										\
			LogPrint(LOG_DEBUG,__FILE__,__LINE__,format,##__VA_ARGS__);	\
		}																\
	}while(0)															\

#define LogWarn(format,...)												\
	do{																	\
		if(LOG_LEVEL>=LOG_WARN){										\
			LogPrint(LOG_WARN,__FILE__,__LINE__,format,##__VA_ARGS__);	\
		}																\
	}while(0)															\

#define LogError(format,...)											\
	do{																	\
		if(LOG_LEVEL>=LOG_ERROR){										\
			LogPrint(LOG_ERROR,__FILE__,__LINE__,format,##__VA_ARGS__);	\
		}																\
	}while(0)															\
	
#endif

#define LogInfo(format, ...) 	    
#define LogDebug(format, ...) 	    
#define LogWarn(format, ...) 	    
#define LogError(format, ...) 	   

#endif

#endif //__DRIVER_LOG_H

