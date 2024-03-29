//
//  FMSkipButton.h
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import <UIKit/UIKit.h>
#import "AdaptrCore.h"

/**
 
 This button automatically updates its `enabled` property to reflect whether
 the currently playing song (if any) can be skipped, and responds to taps by
 requesting that the current song be skipped. 
 
 If there is a playing or paused song that can be skipped and a skip request
 is not currently in progress, then `enabled` is set to `true`, and `false` otherwise.

 */

//NOT_IB_DESIGNABLE
@interface SkipButton : UIButton


@end
