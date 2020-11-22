//
//  FMPlayPauseButton.m
//  UITests
//
//  Created by Eric Lambrecht on 3/6/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import "PlayPauseButton.h"

#if !TARGET_INTERFACE_BUILDER

@interface PlayPauseButton ()

@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;

@end

#endif

@implementation PlayPauseButton

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
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(songChanged:) name:AdaptrAudioPlayerCurrentItemDidBeginPlaybackNotification object:_feedPlayer];
    
    [self addTarget:self action:@selector(onClick) forControlEvents:UIControlEventTouchUpInside];
    
    [self updatePlayerState];
}

- (void) setHideWhenStalled:(BOOL)hideWhenStalled {
    _hideWhenStalled = hideWhenStalled;
    
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
            _audioItem = nil;
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
    _audioItem = nil;
    
    [self updatePlayerState];
}

- (void) setAudioItem:(Audiofile *)audioItem {
    _audioItem = audioItem;
    _station = nil;
    
    [self updatePlayerState];
}

- (void) setHideWhenActive:(BOOL)hideWhenActive {
    _hideWhenActive = hideWhenActive;
    
    [self updatePlayerState];
}


- (void) onClick {
    if (_station) {
        ADLogDebug(@"play limited to station");
        Station *currentStation = _feedPlayer.activeStation;
        
        if (![_station isEqual:currentStation]) {
            [_feedPlayer setActiveStation:_station withCrossfade:_crossfade];
            [_feedPlayer play];
            
            return;
        }
        
    } else if (_audioItem) {
        ADLogDebug(@"play limited to audio item");
        Audiofile *currentAudioItem = _feedPlayer.currentItem;
        
        if (![_audioItem.id isEqualToString:currentAudioItem.id]) {
            [_feedPlayer playAudioItem:_audioItem];
            
            return;
        }
        
    }
    
    if ((_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStatePaused) ||
        (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateReadyToPlay) ||
        (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateComplete)) {

        [_feedPlayer play];
    } else {
        [_feedPlayer pause];
    }
}

- (void) stationUpdated: (NSNotification *)notification {
    if (_station) {
        [self updatePlayerState];
    }
}
- (void) stateUpdated: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) songChanged: (NSNotification *)notification {
    [self updatePlayerState];
}

- (void) updatePlayerState {
    if (_station) {
        if (![_feedPlayer.activeStation isEqual:_station]
            || (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateComplete)
            || (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateReadyToPlay)) {
            // station isn't active
            self.selected = NO;
            self.enabled = YES;
            self.hidden = NO;
            
            return;

        } else if (_hideWhenActive) {
            self.hidden = YES;
            
            return;
        }
    } else if (_audioItem) {
        if (![_feedPlayer.currentItem.id isEqualToString:_audioItem.id]
            || (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateComplete)
            || (_feedPlayer.playbackState == AdaptrAudioPlayerPlaybackStateReadyToPlay)) {
            // song isn't active
            self.selected = NO;
            self.enabled = YES;
            self.hidden = NO;
            
            return;
            
        } else if (_hideWhenActive) {
            self.hidden = YES;
            
            return;
        }
        
        
    }
    
    AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;

    switch (newState) {
        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateStalled:
        case AdaptrAudioPlayerPlaybackStateRequestingSkip:
            [self setSelected:YES];
            [self setEnabled:YES];
            [self setHidden:_hideWhenStalled];
            break;
            
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStatePaused:
            [self setSelected:NO];
            [self setEnabled:YES];
            [self setHidden:NO];
            break;

        case AdaptrAudioPlayerPlaybackStatePlaying:
            [self setSelected:YES];
            [self setEnabled:YES];
            [self setHidden:NO];
            break;
        
        case AdaptrAudioPlayerPlaybackStateOfflineOnly:
        case AdaptrAudioPlayerPlaybackStateUninitialized:
        case AdaptrAudioPlayerPlaybackStateUnavailable:
            [self setSelected:NO];
            [self setEnabled:NO];
            [self setHidden:NO];
            break;
    }
    
    
}
#endif

@end
