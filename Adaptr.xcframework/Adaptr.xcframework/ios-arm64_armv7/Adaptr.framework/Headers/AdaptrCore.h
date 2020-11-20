//
//  FeedMediaCore.h
//  FeedMediaCore
//
//  Created by Eric Lambrecht on 9/6/17.
//  Copyright © 2017 Feed Media. All rights reserved.
//

#define ADAPTR_CLIENT_VERSION @"0.0.1"

// All public headers

#import "AdaptrSimulcastStreamer.h"
#import "Audiofile.h"
#import "AdaptrAudioPlayer.h"
#import "LockScreenDelegate.h"
#import "ADError.h"
#import "ADLog.h"
#import "Station.h"
#import "StationArray.h"

#if TARGET_OS_TV || TARGET_OS_MACCATALYST
#else
#import "CWStatusBarNotification.h"
#endif
