//
//  ADLog.h
//  sdktest
//
//  Created by James Anthony on 10/1/12.
//  Copyright (c) 2012-2016 Feed Media Inc. All rights reserved.
//
#ifdef __cplusplus
extern "C"{
#endif

typedef enum ADLogLevel : NSInteger {
    ADLogLevelNone = 0,
    ADLogLevelError = 1,
    ADLogLevelWarn = 2,
    ADLogLevelDebug = 3
} ADLogLevel;

void ADLogSetLevel(ADLogLevel level);
void _ADLog(NSInteger level, NSString *format, ...);


NSString* _ADLogGetLogContent(void);
void _ADLogResetLog(void);

#ifndef ADLog
#define ADLog(level,fmt,...) _ADLog(level, (@"%s (%d): " fmt), __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif

#ifndef ADLogDebug
#define ADLogDebug(...) ADLog(ADLogLevelDebug,__VA_ARGS__)
#endif

#ifndef ADLogWarn
#define ADLogWarn(...) ADLog(ADLogLevelWarn,__VA_ARGS__)
#endif

#ifndef ADLogError
#define ADLogError(...) ADLog(ADLogLevelError,__VA_ARGS__)
#endif

#ifdef __cplusplus
}
#endif
