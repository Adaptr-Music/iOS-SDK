//
//  FMRemainingTimeLabel.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 6/10/15.
//  Copyright (c) 2015 Feed Media. All rights reserved.
//

#import "RemainingTimeLabel.h"
#import "ElapsedTimeLabel.h"

#define kFMProgressBarUpdateTimeInterval 0.5

@interface RemainingTimeLabel ()

#if !TARGET_INTERFACE_BUILDER
@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;
#endif

@end


@implementation RemainingTimeLabel

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

- (void) setup {
    _feedPlayer = [AdaptrAudioPlayer sharedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onPlayerUpdated:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(onPlayerUpdated:) name:AdaptrAudioPlayerTimeElapseNotification object:_feedPlayer];
    
    [self updatePlayerState];
}

- (void) onPlayerUpdated: (NSNotification *) notification {
    [self updatePlayerState];
}

- (void) dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) setText:(NSString *)text {
    // ignore whatever is passed in during runtime
    [self updateProgress];
}

- (void) updatePlayerState {
    AdaptrAudioPlayerPlaybackState newState = _feedPlayer.playbackState;
    
    switch (newState) {
        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStatePaused:
        case AdaptrAudioPlayerPlaybackStatePlaying:
            [self updateProgress];
            break;
            
        default:
            // nada
            break;
    }
}

- (void)updateProgress {
    NSTimeInterval duration = _feedPlayer.currentItemDuration;
    if(duration > 0) {
        long remainingTime = lroundf(duration - self.feedPlayer.currentPlaybackTime);

        if (remainingTime < 0) {
            [super setText:@"0:00"];
        } else {
            [super setText: [NSString stringWithFormat:@"-%ld:%02ld", remainingTime / 60, remainingTime % 60]];
        }
        
    }
    else {
        [super setText:_textForNoTime];
    }
}

#endif

- (void) setTextForNoTime: (NSString *) textForNoTime {
    _textForNoTime = textForNoTime;
    
#if !TARGET_INTERFACE_BUILDER
    [self updatePlayerState];
#else
    [super setText:textForNoTime];
#endif
    
}



@end

#undef kFMProgressBarUpdateTimeInterval
