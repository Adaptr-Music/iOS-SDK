//
//  FMTotalTimeLabel.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import "TotalTimeLabel.h"

#define kFMProgressBarUpdateTimeInterval 0.5

#if !TARGET_INTERFACE_BUILDER

@interface TotalTimeLabel ()

@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;

@end

#endif


@implementation TotalTimeLabel

#if !TARGET_INTERFACE_BUILDER

- (id) initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        [self setup];
    }
    
    return self;
}

- (id) initWithCoder:(NSCoder *)aDecoder {
    if (self = [super initWithCoder:aDecoder]) {
        [self setup];
    }
    
    return self;
}

- (id) init {
    if (self = [super init]) {
        [self setup];
    }
    
    return self;
}

- (void) dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) setup {
    _feedPlayer = [AdaptrAudioPlayer sharedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerUpdated:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    
    [self updatePlayerState];
}

- (void) setText: (NSString *)text {
    // ignore
}

- (void) playerUpdated: (NSNotification *) notification {
    [self updatePlayerState];
}

- (void) updatePlayerState {
    AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;
    
    switch (newState) {
        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStateUnavailable:
        case AdaptrAudioPlayerPlaybackStateUninitialized:
            [self resetProgress];
            break;
            
        case AdaptrAudioPlayerPlaybackStatePaused:
        case AdaptrAudioPlayerPlaybackStateRequestingSkip:
        case AdaptrAudioPlayerPlaybackStatePlaying:
            [self updateProgress];
            break;
            
        default:
            // nada
            break;
    }
}

- (void)updateProgress {
    long duration = lroundf(_feedPlayer.currentItemDuration);
    
    if(duration > 0) {
        [super setText: [NSString stringWithFormat:@"%ld:%02ld", duration / 60, duration % 60]];
        
    }
    else {
        [super setText:@"0:00"];
    }
}

- (void)resetProgress {
    [super setText:_textForNoTime];
}

#endif

- (void) setTextForNoTime: (NSString *) theText {
    _textForNoTime = theText;

#if !TARGET_INTERFACE_BUILDER
    [self updatePlayerState];
#else
    [super setText:theText];
#endif
}



@end

#undef kFMProgressBarUpdateTimeInterval


