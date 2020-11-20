//
//  FMProgressView.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015 Feed Media. All rights reserved.
//

#import "AudioProgressView.h"

#define kFMProgressBarUpdateTimeInterval 0.5

@interface AudioProgressView ()

#if !TARGET_INTERFACE_BUILDER
@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;
@property float actualProgress;
#endif

@end

@implementation AudioProgressView

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
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onPlayerUpdated:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onPlayerUpdated:) name:AdaptrAudioPlayerTimeElapseNotification object:_feedPlayer];

    [self updatePlayerState];
}

- (void) setProgress: (float) progress {
    // ignore passed in values, and just use what we've calculated
    [super setProgress:_actualProgress];
}

- (void) setProgress:(float)progress animated:(BOOL)animated {
    // ignore passed in values, and just use what we've calculated
    [super setProgress:_actualProgress animated:animated];
}

- (void) onPlayerUpdated: (NSNotification *) notification {
    [self updatePlayerState];
}

- (void) updatePlayerState {
    AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;
    
    switch (newState) {
        case AdaptrAudioPlayerPlaybackStatePaused:
        case AdaptrAudioPlayerPlaybackStatePlaying:
        case AdaptrAudioPlayerPlaybackStateStalled:
            [self updateProgress];
            break;

        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateUninitialized:
        case AdaptrAudioPlayerPlaybackStateUnavailable:
            [self resetProgress];
            break;
            
        default:
            // nada
            break;
    }
}

- (void)updateProgress {
    NSTimeInterval duration = _feedPlayer.currentItemDuration;
    if(duration > 0) {
        _actualProgress = _feedPlayer.currentPlaybackTime / duration;
        [super setProgress:_actualProgress animated:false];
    }
    else {
        _actualProgress = 0.0;
        [super setProgress:_actualProgress animated:false];
    }
}

- (void)resetProgress {
    _actualProgress = 0.0;
    [super setProgress: _actualProgress animated:false];
}

#endif



@end

#undef kFMProgressBarUpdateTimeInterval

