//
//  FMSkipWarningView.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 4/27/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import "SkipWarningView.h"

#define kFMWarningDurationInSeconds 3

@interface SkipWarningView ()

#if !TARGET_INTERFACE_BUILDER
@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;
@property (nonatomic) BOOL showing;
#endif

@end

@implementation SkipWarningView

#if !TARGET_INTERFACE_BUILDER

- (id)initWithCoder:(NSCoder *)aDecoder {
    if (self = [super initWithCoder:aDecoder]) {
        [self setup];
    }
    
    return self;
}

- (id)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        [self setup];
    }
    
    return self;
}

- (void)setup {
    self.showing = false;

    self.hidden = true;

    _feedPlayer = [AdaptrAudioPlayer sharedPlayer];
    
    // register to receive notice when the user hits a skip limit
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(skipEncountered:) name:AdaptrAudioPlayerSkipFailedNotification object:_feedPlayer];
}

- (void) skipEncountered: (NSNotification *) notification {
    if (self.showing) {
        // already showing message
        return;
    }

    // display or animate in the view, then wait out for a bit before hiding again
    self.showing = true;
    self.hidden = false;
    
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, kFMWarningDurationInSeconds * NSEC_PER_SEC), dispatch_get_main_queue(),^{
            self.hidden = true;
            self.showing = false;
        
//            [self setNeedsDisplay];
        });
}

#endif


@end

#undef kFMWarningDurationInSeconds

