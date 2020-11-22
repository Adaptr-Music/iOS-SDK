//
//  FMSkipButton.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import "SkipButton.h"

#if !TARGET_INTERFACE_BUILDER

@interface SkipButton ()

@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;

@end

#endif

@implementation SkipButton

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
    
    NSNotificationCenter *ns = [NSNotificationCenter defaultCenter];
    [ns addObserver:self selector:@selector(onPlaybackStateDidChange:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    [ns addObserver:self selector:@selector(onCurrentItemDidChange:) name:AdaptrAudioPlayerCurrentItemDidBeginPlaybackNotification object:_feedPlayer];
    [ns addObserver:self selector:@selector(onSkipDidFail:) name:AdaptrAudioPlayerSkipFailedNotification object:_feedPlayer];
    [ns addObserver:self selector:@selector(onSkipStatusChanged:) name:AdaptrAudioPlayerSkipStatusNotification object:_feedPlayer];
    
    [self addTarget:self action:@selector(onSkipClick) forControlEvents:UIControlEventTouchUpInside];
;
    
    [self updatePlayerState];
}

- (void) onSkipClick {
    [_feedPlayer skip];
}

- (void) onPlaybackStateDidChange: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) onCurrentItemDidChange: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) updatePlayerState {
    if (_feedPlayer.currentItem == nil) {
        [self setEnabled:NO];

    } else if (!_feedPlayer.canSkip) {
        [self setEnabled:NO];
        
    } else {
        AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;
    
        switch (newState) {
            case AdaptrAudioPlayerPlaybackStateOfflineOnly:
            case AdaptrAudioPlayerPlaybackStateUninitialized:
            case AdaptrAudioPlayerPlaybackStateUnavailable:
            case AdaptrAudioPlayerPlaybackStateRequestingSkip:
            case AdaptrAudioPlayerPlaybackStateWaitingForItem:
            case AdaptrAudioPlayerPlaybackStateComplete:
            case AdaptrAudioPlayerPlaybackStateReadyToPlay:
                [self setEnabled:NO];
                break;
            case AdaptrAudioPlayerPlaybackStatePaused:
            case AdaptrAudioPlayerPlaybackStatePlaying:
            case AdaptrAudioPlayerPlaybackStateStalled:
                [self setEnabled:YES];
                break;
        }
    }
}

- (void) onSkipDidFail: (NSNotification *)notification {
    [self setEnabled:NO];
}

- (void) onSkipStatusChanged: (NSNotification *)notification {
    [self updatePlayerState];
}


#endif

@end
