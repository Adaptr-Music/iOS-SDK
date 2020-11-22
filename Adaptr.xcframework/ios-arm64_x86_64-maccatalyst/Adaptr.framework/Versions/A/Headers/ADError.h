//
//  FMError.h
//  sdktest
//
//  Created by James Anthony on 3/12/13.
//  Copyright (c) 2020 Adaptr All rights reserved.
//

extern NSString * const APIErrorDomain;

/**
 *  @enum ErrorCode
 *  Represents error codes
 *  @const ErrorCodeRequestFailed Request failed because of connection error
 *  @const ErrorCodeUnexpectedReturnType Returned JSON has unexpected structure
 *  @const ErrorCodeInvalidCredentials Authentification is required, but no authentification information available.
 *  @const ErrorCodeInvalidSkip   Invalid Skip Request was sent. It might happen if skip is requested while no song is currently playing
 */
typedef enum ErrorCode : NSInteger {
    ErrorCodeRequestFailed = -4,
    ErrorCodeUnexpectedReturnType = -1,
    ErrorCodeInvalidCredentials = 5,
    ErrorCodeAccessForbidden = 6,
    ErrorCodeSkipLimitExceeded = 7,
    ErrorCodeNoAvailableMusic = 9,
    ErrorCodeInvalidSkip = 12,
    ErrorCodeInvalidParameter = 15,
    ErrorCodeMissingParameter = 16,
    ErrorCodeNoSuchResource = 17,
    ErrorCodeInternal = 18,
    ErrorCodeInvalidRegion = 19
} ErrorCode;
