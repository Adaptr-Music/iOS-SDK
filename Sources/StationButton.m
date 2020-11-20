//
//  StationButton.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 5/14/15.
//  Copyright (c) 2015 Feed Media. All rights reserved.
//

#import "StationButton.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-implementations"


@interface StationButton ()

#if !TARGET_INTERFACE_BUILDER
@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;
#endif

@end

@implementation StationButton

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
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(stationUpdated:) name:AdaptrAudioPlayerActiveStationDidChangeNotification object:_feedPlayer];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(stateUpdated:) name:
     AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    
    [self addTarget:self action:@selector(onClick) forControlEvents:UIControlEventTouchUpInside];
    
    [self updatePlayerState];
}

- (void) stationUpdated: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) stateUpdated: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) onClick {
    Station *currentStation = _feedPlayer.activeStation;
    
    if ([_station isEqual:currentStation]) {
        
        if (!_playOnClick) {
            return;
        }
        
        AdaptrAudioPlayerPlaybackState state = _feedPlayer.playbackState;

        if ((state == AdaptrAudioPlayerPlaybackStateComplete) ||
            (state == AdaptrAudioPlayerPlaybackStateReadyToPlay) ||
            (state == AdaptrAudioPlayerPlaybackStatePaused)) {
            [_feedPlayer play];
            
        } else {
            [_feedPlayer pause];
            
        }

    } else {
        [_feedPlayer setActiveStation:_station withCrossfade:_crossfade];
        
        if (_playOnClick) {
            [_feedPlayer play];
            
        }
    }
    
    [self updatePlayerState];

}

- (void) setStationName:(NSString *)stationName {

    if (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateUninitialized) {
        // just in case this was set before music was available
        [_feedPlayer whenAvailable:^{
            [self setStationName:stationName];
            
        } notAvailable:^{
            // nada
        }];
        return;
    }
    
    NSArray *stations = [[AdaptrAudioPlayer sharedPlayer] stationList];
    
    for (Station *station in stations) {
        if ([station.name isEqualToString:stationName]) {
            _station = station;
            break;
        }
    }

    [self updatePlayerState];
}

- (NSString *)stationName {
    return _station.name;
}

- (void) setStation:(Station *)station {
    _station = station;
    
    [self updatePlayerState];
}

- (void) setPlayOnClick:(BOOL)playOnClick {
    _playOnClick = playOnClick;
    
    [self updatePlayerState];
}

- (void) setHideWhenActive:(BOOL)hideWhenActive {
    _hideWhenActive = hideWhenActive;
    
    [self updatePlayerState];
}

- (void) updatePlayerState {

    // disable player when we can't do playback
    if ((_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateUninitialized)
        || (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateUnavailable)) {
        self.enabled = NO;
        self.selected = NO;
        return;
    }
    
    self.enabled = YES;
    
    // if the station is active right now
    if ([_feedPlayer.activeStation isEqual:_station]
        && (_feedPlayer.playbackState != AdaptrAudioPlayerPlaybackStateComplete)
        && (_feedPlayer.playbackState != AdaptrAudioPlayerPlaybackStateReadyToPlay)) {

        if (_hideWhenActive) {
            self.hidden = YES;

        } else {
            self.hidden = NO;
            
            AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;
            
            // highlighted = YES = show the pause button
            // highlighted = NO = show the play button
            
            switch (newState) {
                case AdaptrAudioPlayerPlaybackStateWaitingForItem:
                case AdaptrAudioPlayerPlaybackStateStalled:
                case AdaptrAudioPlayerPlaybackStateRequestingSkip:
                case AdaptrAudioPlayerPlaybackStatePlaying:
                    [self setSelected:YES];

                    break;
                    
                case AdaptrAudioPlayerPlaybackStateComplete:
                case AdaptrAudioPlayerPlaybackStateReadyToPlay:
                case AdaptrAudioPlayerPlaybackStatePaused:
                    [self setSelected:NO];

                    break;
                case AdaptrAudioPlayerPlaybackStateOfflineOnly:
                case AdaptrAudioPlayerPlaybackStateUninitialized:
                case AdaptrAudioPlayerPlaybackStateUnavailable:
                    // shouldn't happen
                    break;
            }
            
        }

    } else {
        // station isn't active
        self.selected = NO;
        self.hidden = NO;
        
    }
}

#endif

@end

#pragma clang diagnostic pop
