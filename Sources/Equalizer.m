//
//  FMEqualizer.m
//
//  Created by Eric Lambrecht on 1/25/16.
//  Copyright © 2020 Adaptr All rights reserved.
//

#import "Equalizer.h"

@interface Equalizer ()

@property (nonatomic, strong) UIView *equalizerOne;
@property (nonatomic, strong) UIView *equalizerTwo;
@property (nonatomic, strong) UIView *equalizerThree;

@property (nonatomic, weak) AdaptrAudioPlayer *feedPlayer;

@property (nonatomic) bool nowAnimating;

@end

@implementation Equalizer


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
    _nowAnimating = false;
    
    _feedPlayer = [AdaptrAudioPlayer sharedPlayer];

    // create three bars, each just 1 pixel high.
    _equalizerOne = [[UIView alloc] initWithFrame:[self makeFrameAtIndex:0 withHeight:.05]];
    _equalizerOne.backgroundColor = self.tintColor;
    _equalizerTwo.userInteractionEnabled = NO;
    [self addSubview:_equalizerOne];
    
    _equalizerTwo = [[UIView alloc] initWithFrame:[self makeFrameAtIndex:1 withHeight:0.05]];
    _equalizerTwo.backgroundColor = self.tintColor;
    _equalizerTwo.userInteractionEnabled = NO;
    [self addSubview:_equalizerTwo];
    
    _equalizerThree = [[UIView alloc] initWithFrame:[self makeFrameAtIndex:2 withHeight:0.05]];
    _equalizerThree.backgroundColor = self.tintColor;
    _equalizerThree.userInteractionEnabled = NO;
    [self addSubview:_equalizerThree];

    self.userInteractionEnabled = NO;
    
    // if music is currently playing, then start up animation
    if ([self isMusicPlaying]) {
        [self startAnimation];
    }
    
    // watch for changes in playback state
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(playerUpdated:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:_feedPlayer];
}

- (void) playerUpdated: (NSNotification *) notification {
    if (_nowAnimating && ![self isMusicPlaying]) {
        [self stopAnimation];
        
    } else if (!_nowAnimating && [self isMusicPlaying]) {
        [self startAnimation];
    }
}

- (BOOL) isMusicPlaying {
    AdaptrAudioPlayerPlaybackState state = _feedPlayer.playbackState;
    
    switch (state) {
        case AdaptrAudioPlayerPlaybackStateOfflineOnly:
        case AdaptrAudioPlayerPlaybackStateComplete:
        case AdaptrAudioPlayerPlaybackStateReadyToPlay:
        case AdaptrAudioPlayerPlaybackStatePaused:
        case AdaptrAudioPlayerPlaybackStateUnavailable:
        case AdaptrAudioPlayerPlaybackStateUninitialized:
            return false;
            
        case AdaptrAudioPlayerPlaybackStatePlaying:
        case AdaptrAudioPlayerPlaybackStateRequestingSkip:
        case AdaptrAudioPlayerPlaybackStateWaitingForItem:
        case AdaptrAudioPlayerPlaybackStateStalled:
            return true;
            
    }
}

- (CGRect) makeFrameAtIndex: (int)index withHeight: (float)heightPercentage {
    // barSpace takes up 1/3 the width of the view
    float barSpaceWidth = self.frame.size.width / 3;
                      
    // bar takes up 2/3 of the barSpace
    float barWidth = barSpaceWidth * 2 / 3;
                      
    float height = self.frame.size.height;
    
    return CGRectMake(index * barSpaceWidth, (1 - heightPercentage) * height, barWidth, heightPercentage * height);
}

- (void) startAnimation {
    
    /*
    // immediately put the views in active positions
    _equalizerOne.frame = [self makeFrameAtIndex:0 withHeight:0.3f];
    _equalizerTwo.frame = [self makeFrameAtIndex:1 withHeight:0.9f];
    _equalizerThree.frame = [self makeFrameAtIndex:2 withHeight:0.5f];
     */
    
    [UIView animateWithDuration:0.2 delay:0.0f options:UIViewAnimationOptionBeginFromCurrentState animations:^{
        self->_equalizerOne.frame = [self makeFrameAtIndex:0 withHeight:0.3f];
        self->_equalizerTwo.frame = [self makeFrameAtIndex:1 withHeight:0.9f];
        self->_equalizerThree.frame = [self makeFrameAtIndex:2 withHeight:0.5f];
    } completion:^(BOOL arg){

        // get the equalizers animating repeatedly
        [UIView animateKeyframesWithDuration:0.8f delay:0.0f options:UIViewKeyframeAnimationOptionRepeat
                                  animations:^{
                                      [UIView addKeyframeWithRelativeStartTime:0 relativeDuration:0.5 animations:^{
                                          self->_equalizerOne.frame = [self makeFrameAtIndex:0 withHeight:0.7f];
                                      }];
                                      
                                      [UIView addKeyframeWithRelativeStartTime:0.5 relativeDuration:0.5 animations:^{
                                          self->_equalizerOne.frame = [self makeFrameAtIndex:0 withHeight:0.2f];
                                      }];
                                      
                                  } completion:nil];
        
        [UIView animateKeyframesWithDuration:1.2f delay:0.0f options:UIViewKeyframeAnimationOptionRepeat
                                  animations:^{
                                      [UIView addKeyframeWithRelativeStartTime:0 relativeDuration:0.5 animations:^{
                                          self->_equalizerTwo.frame = [self makeFrameAtIndex:1 withHeight:0.3f];
                                      }];
                                      
                                      [UIView addKeyframeWithRelativeStartTime:0.5 relativeDuration:0.5 animations:^{
                                          self->_equalizerTwo.frame = [self makeFrameAtIndex:1 withHeight:0.5f];
                                      }];
                                      
                                  } completion:nil];
        
        [UIView animateKeyframesWithDuration:1.6f delay:0.0f options:UIViewKeyframeAnimationOptionRepeat
                                  animations:^{
                                      [UIView addKeyframeWithRelativeStartTime:0 relativeDuration:0.5 animations:^{
                                          self->_equalizerThree.frame = [self makeFrameAtIndex:2 withHeight:0.9f];
                                      }];
                                      
                                      [UIView addKeyframeWithRelativeStartTime:0.5 relativeDuration:0.5 animations:^{
                                          self->_equalizerThree.frame = [self makeFrameAtIndex:2 withHeight:0.4f];
                                      }];
                                      
                                  } completion:nil];
        

    }];

    _nowAnimating = TRUE;
}

- (void) stopAnimation {
    // stop running animation by throwing in a new, non repeating animation that will
    // cancel any existing animation
    [UIView animateWithDuration:0.1 delay:0.0f options:UIViewAnimationOptionBeginFromCurrentState animations:^{
        self->_equalizerOne.frame = [self makeFrameAtIndex:0 withHeight:0.05f];
        self->_equalizerTwo.frame = [self makeFrameAtIndex:1 withHeight:0.05f];
        self->_equalizerThree.frame = [self makeFrameAtIndex:2 withHeight:0.05f];
        
    } completion:nil];
    
    _nowAnimating = FALSE;
}
@end
