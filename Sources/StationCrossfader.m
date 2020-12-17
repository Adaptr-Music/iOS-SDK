//
//  StationCrossfader.m
//  FeedMedia
//
//  Created by Eric Lambrecht on 9/21/17.
//  Copyright ©   2020 Adaptr All rights reserved.
//

#import "StationCrossfader.h"
#import "Adaptr.h"

@interface CuePoint : NSObject

@property (nonatomic) float time;

@property (nonatomic, strong) Station *station;
@property (nonatomic, strong) NSNumber *volume;

+ (CuePoint *) station: (Station *) station time: (float) time;
+ (CuePoint *) volume: (NSNumber *) volume time: (float) time;

@end

@implementation CuePoint

+ (CuePoint *) station: (Station *) station time: (float) time {
    CuePoint *st = [[CuePoint alloc] init];
    
    st.time = time;
    st.station = station;
    st.volume = nil;
    
    return st;
}

+ (CuePoint *) volume: (NSNumber *) volume time: (float) time {
    CuePoint *st = [[CuePoint alloc] init];
    
    st.time = time;
    st.station = nil;
    st.volume = volume;
    
    return st;
}

@end

@implementation StationCrossfader {
    
    NSMutableArray *_cuePoints;
    AdaptrAudioPlayer *_player;
    Station *_initialStation;
    BOOL _connected;
    
    int _nextCuePointIndex;
    float _lastElapsedTime;
}

- (id) initWithStation: (Station *) station {
    if (self = [super init]) {
        _initialStation = station;
        _cuePoints = [NSMutableArray array];
        _player = [AdaptrAudioPlayer sharedPlayer];
        _connected = NO;
    }
    
    return self;
}

+ (StationCrossfader *) stationCrossfaderWithInitialStation:(NSDictionary *)initialStationOptionKeysAndValues {
    if (initialStationOptionKeysAndValues == nil) {
        return [[StationCrossfader alloc] initWithStation:nil];

    } else {
        AdaptrAudioPlayer *player = [AdaptrAudioPlayer sharedPlayer];
        Station *station = [player.stationList getStationWithOptions: initialStationOptionKeysAndValues];
        
        return [[StationCrossfader alloc] initWithStation:station];
    }
}

+ (StationCrossfader *) stationCrossfaderWithInitialStation: (NSDictionary *) initialStationOptionKeysAndValues
                                                  andCuePoints: (NSArray *) cuePoints {
    StationCrossfader *sc = [StationCrossfader stationCrossfaderWithInitialStation:initialStationOptionKeysAndValues];

    if ((cuePoints.count % 2) != 0) {
        ADLogDebug(@"**WARNING** cuepoints provided to StationCrossfader should come in pairs, but we found %lu items", (unsigned long)cuePoints.count);
    }

    // convert the input values into an array of FMCuePoint objects
    for (int i = 0; i < cuePoints.count; i += 2) {
        NSNumber *timeObject = (NSNumber *) cuePoints[i];

        if ([cuePoints[i+1] isKindOfClass:[NSNumber class]]) {
            NSNumber *volume = (NSNumber *) cuePoints[i+1];
            [sc setVolume:volume startingAtTime:[timeObject floatValue]];
            
        } else if ([cuePoints[i+1] isKindOfClass:[NSDictionary class]]) {
            NSDictionary *optionKeysAndValues = (NSDictionary *) cuePoints[i+1];
            [sc playStation: optionKeysAndValues startingAtTime: [timeObject floatValue]];

        } else {
            ADLogDebug(@"Unknown cue point type at time %@ is %@. Not a NSNumber or NSDictionary", timeObject, cuePoints[i+1]);

        }
    }
    
    return sc;
}

- (void) setVolume:(NSNumber *)volume startingAtTime:(float)time {
    if (_connected) {
        ADLogDebug(@"**WARNING** cannot append cues to crossfader once begin has been called");
        return;
    }

    if (time < 0.0f) {
        return;
    }
    
    //NSLog(@"adding volume %f at time %f", [volume floatValue], time);
    [_cuePoints addObject:[CuePoint volume:volume time:time]];
}

- (void) playStation: (NSDictionary *) optionKeysAndValues startingAtTime: (float) time {
    if (_connected) {
        ADLogDebug(@"**WARNING** cannot append cues to crossfader once begin has been called");
        return;
    }
    
    if (time < 0.0f) {
        return;
    }
    
    if (optionKeysAndValues == nil) {
        ADLogDebug(@"**WARNING** only the initial station may be nil. Unable to add to station crossfader at time index %f.", time);
        return;
    }
    
    Station *station = [_player.stationList getStationWithOptions: optionKeysAndValues];
    
    if (station != nil) {
        //ADLogDebug(@"adding station %@ at time %f", station.name, time);
        [_cuePoints addObject:[CuePoint station:station time: time]];
        
    } else {
        ADLogDebug(@"**WARNING** unable to find station with attributes %@ for time index %f", optionKeysAndValues, time);
    }
}

- (void) connect {
    if (_connected) {
        return;
    }
    
    if (_cuePoints.count == 0) {
        return;
    }

    // make sure all the cue points are ordered properly
    NSSortDescriptor *sd = [NSSortDescriptor sortDescriptorWithKey:@"time" ascending:YES];
    [_cuePoints sortUsingDescriptors:@[ sd ]];
    
    ADLogDebug(@"there are %ld cue points", (long) _cuePoints.count);
    
    _nextCuePointIndex = 0;
    _lastElapsedTime = -1.0f;
    
    if (_initialStation == nil) {
        // no station specified anywhere.. so just reset the player
        [_player stop];

    } else if ([_initialStation isEqual:_player.activeStation]) {
        // already in the station - make sure we're paused
        [_player pause];
        
    } else {
        // tune to the initial station
        [_player stop];
        _player.activeStation = _initialStation;
        
    }

    _connected = YES;
}

- (void) play {
    if (!_connected || (_player.playbackState == AdaptrAudioPlayerPlaybackStateUnavailable)) {
        return;
    }
    
    if (_initialStation != nil) {
        [_player play];
        return;
    }
    
    if (_lastElapsedTime == -1.0f) {
        // no initial station, and we haven't elapsed playback yet, so don't start anything
        return;
    }

    // start playback if we've tuned to a station by now
    //
    for (int x = _nextCuePointIndex; x >= 0; x--) {
        CuePoint *cp = (CuePoint *) _cuePoints[x];
        
        if (cp.station != nil) {
            [_player play];
            return;
        }
    }
    
    // otherwise, don't start playback just yet
}

- (void) pause {
    if (!_connected || (_player.playbackState == AdaptrAudioPlayerPlaybackStateUnavailable)) {
        return;
    }

    [_player pause];
}

- (void) disconnect {
    _connected = NO;
}

- (void) reconnect {
    _connected = YES;
}

- (void) elapseToTime: (float) time {
    if (!_connected || (_cuePoints.count == 0)) {
        return;
    }
    
    //NSLog(@"advancing to time %f. ncpi is %d", time, _nextCuePointIndex);
    
    // user jumped back in time for some reason, so reset things
    if (_lastElapsedTime > time) {
        _lastElapsedTime = -1.0f;
        _nextCuePointIndex = 0;
    }
    
    Station *station = nil;
    NSNumber *volume = nil;
    
    CuePoint *cp = (_cuePoints.count > _nextCuePointIndex) ? _cuePoints[_nextCuePointIndex] : nil;
    while ((cp != nil) && (cp.time <= time)) {
        if (cp.station != nil) {
            station = cp.station;
        }
        
        if (cp.volume != nil) {
            volume = cp.volume;
        }
        if(_cuePoints.count > _nextCuePointIndex)
        {
            _nextCuePointIndex++;
            cp =_cuePoints[_nextCuePointIndex];
        }
        else {
            cp = nil;
        }
        cp = (_cuePoints.count > _nextCuePointIndex) ? _cuePoints[_nextCuePointIndex] : nil;
    }
    
    if (station != nil) {
        if (![_player.activeStation isEqual:station]) {
            //NSLog(@"Changing station to %@", station);
            
            [_player loadStation:station withCrossfade:YES];
        }

        [_player play];

    } else if ((_lastElapsedTime == -1.0f) && (_initialStation != nil)) {
        // if this is the first call to elapseToTime and we have an initial
        // station, then kick off playback of that station
        //NSLog(@"Kicking off initial playback of station %@", station);
        
        [_player play];
    }
    
    if ((volume != nil) && (_player.mixVolume != [volume floatValue])) {
        //NSLog(@"Changing volume to %f", [volume floatValue]);
        
        _player.mixVolume = [volume floatValue];
    }
    
    _lastElapsedTime = time;
}

@end
