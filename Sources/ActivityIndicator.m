//
//  FMActivityIndicator.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/11/15.
//  Copyright (c) 2015 Feed Media. All rights reserved.
//

#import "ActivityIndicator.h"

#if !TARGET_INTERFACE_BUILDER

@interface ActivityIndicator ()

@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;

@end

#endif

@implementation ActivityIndicator

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

- (void) playerUpdated: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) updatePlayerState {
    AdaptrAudioPlayerPlaybackState newState;
    
    newState = _feedPlayer.playbackState;

    switch (newState) {
        case AdaptrAudioPlayerPlaybackStateOfflineOnly:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStatePaused:
        case AdaptrAudioPlayerPlaybackStatePlaying:
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateUnavailable:
        case AdaptrAudioPlayerPlaybackStateUninitialized:
            [self stopAnimating];
            break;

        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateStalled:
        case AdaptrAudioPlayerPlaybackStateRequestingSkip:
            [self startAnimating];
            break;
    }
}

#endif

@end
