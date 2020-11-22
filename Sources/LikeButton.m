//
//  FMLikeButton.m
//  iOS-UI-SDK
//
//  Created by Eric Lambrecht on 3/10/15.
//  Copyright (c) 2015  2020 Adaptr All rights reserved.
//

#import "LikeButton.h"

@interface LikeButton ()

@property (strong, nonatomic) AdaptrAudioPlayer *feedPlayer;

@end

@implementation LikeButton

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
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerUpdated:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playLikeStatusUpdated:) name:AdaptrAudioPlayerLikeStatusChangeNotification object:_feedPlayer];
    
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerUpdated:) name:AdaptrAudioPlayerCurrentItemDidBeginPlaybackNotification object:self.feedPlayer];
    
    [self addTarget:self action:@selector(onLikeClick) forControlEvents:UIControlEventTouchUpInside];
    ;

    [self updateButtonState];
    
}

- (void) setImage:(UIImage *)image forState:(UIControlState)state {
    [super setImage:image forState:state];
}

- (void) dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

- (void) setAudioItem:(Audiofile *)audioItem {
    _audioItem = audioItem;
    
    [self updateButtonState];
}

- (void) onLikeClick {
    Audiofile *ai = (_audioItem == nil) ? _feedPlayer.currentItem : _audioItem;

    if (ai.liked) {
        [_feedPlayer unlikeAudioItem:ai];
    } else {
        [_feedPlayer likeAudioItem:ai];
    }
    
    [self updateButtonState];
}

- (void) playLikeStatusUpdated: (NSNotification *)notification {
    Audiofile *notificationAudioItem = notification.userInfo[AudiofileKey];
    
    Audiofile *ai = (_audioItem == nil) ? _feedPlayer.currentItem : _audioItem;
    
    if ([ai.id isEqualToString:notificationAudioItem.id]) {
        
        if ((ai.playId == nil) || ![ai.playId isEqualToString:notificationAudioItem.playId]) {
            // copy like status to our item
            if (notificationAudioItem.liked) {
                [ai like];
            } else if (notificationAudioItem.disliked) {
                [ai dislike];
            } else {
                [ai unlike];
            }
        }
        
        [self updateButtonState];
    }
}

- (void) playerUpdated: (NSNotification *)notification {
    if (_audioItem == nil) {
        [self updateButtonState];
    }
}

- (void) updateButtonState {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self->_audioItem != nil) {
        self.enabled = YES;
            self.selected = self->_audioItem.liked;

        } else {
            AdaptrAudioPlayerPlaybackState newState = self->_feedPlayer.playbackState;
            BOOL liked = self->_feedPlayer.currentItem.liked;
            
            switch (newState) {
                case AdaptrAudioPlayerPlaybackStateOfflineOnly:
                case AdaptrAudioPlayerPlaybackStatePaused:
                case AdaptrAudioPlayerPlaybackStatePlaying:
                case AdaptrAudioPlayerPlaybackStateStalled:
                case AdaptrAudioPlayerPlaybackStateRequestingSkip:
                    self.enabled = YES;
                    self.selected = liked;
                    break;
                case AdaptrAudioPlayerPlaybackStateReadyToPlay:
                case AdaptrAudioPlayerPlaybackStateWaitingForItem:
                case AdaptrAudioPlayerPlaybackStateComplete:
                case AdaptrAudioPlayerPlaybackStateUnavailable:
                case AdaptrAudioPlayerPlaybackStateUninitialized:
                    self.enabled = NO;
                    self.selected = NO;
            }
        }
    });
}

#endif

@end
