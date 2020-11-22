//
//  FMProgressView.h
//  iOS-UI-SDK
//
//  This class extends UIProgressView so that it updates its progress
//  automatically by listening to events from the Adaptr
//  player.
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

/**
 
 This progress view updates its `progress` value automatically to
 reflect the playback progress into the currently playing song,
 if any. If no song is playing, then the progress is set to 0.
 
 */

#import <UIKit/UIKit.h>
#import "AdaptrCore.h"


//NOT_IB_DESIGNABLE
@interface AudioProgressView : UIProgressView



@end
