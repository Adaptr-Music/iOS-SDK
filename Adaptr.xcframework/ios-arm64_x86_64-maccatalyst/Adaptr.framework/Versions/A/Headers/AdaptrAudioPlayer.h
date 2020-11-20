//
//  AdaptrAudioPlayer.h
//  sdktest
//
//  Created by James Anthony on 3/7/13.
//  Copyright (c) 2013-2016 Feed Media, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import "Station.h"
#import "Audiofile.h"
#import "ADError.h"
#import "ADLog.h"
#import "AdaptrAudioPlayer.h"
#import "LockScreenDelegate.h"
#import "StationArray.h"

#if TARGET_OS_TV || TARGET_OS_MACCATALYST
#else
#import "CWStatusBarNotification.h"
#endif


NS_ASSUME_NONNULL_BEGIN

/**
 *  @const AdaptrAudioPlayerPlaybackStateDidChangeNotification
 *  @discussion Sent when <AdaptrAudioPlayer> state is changed.
 *  <AdaptrAudioPlayer> instance is sent as [NSNotification object]
 *  Observe <playbackState> property for the exact state. This will
 *  be the first event issued by the library, and the state will
 *  either be <AdaptrAudioPlayerPlaybackStateUnavailable> or <AdaptrAudioPlayerPlaybackStateReadyToPlay>.
 */
extern NSString *const AdaptrAudioPlayerPlaybackStateDidChangeNotification;

/**
 *  @const AdaptrAudioPlayerCurrentItemDidBeginPlaybackNotification
 *  @discussion Sent when a new song has begun playback. Code that responds
 *    to this event can expect [AdaptrAudioPlayer currentItem] to be non-nil.
 *    This event is only triggered once for any song (and not, for instance,
 *    after resuming playbck from a pause)
 */

extern NSString *const AdaptrAudioPlayerCurrentItemDidBeginPlaybackNotification;

/**
 *  @const AdaptrAudioPlayerMusicQueuedNotification
 * @discussion Sent when the player has loaded music from the current
 * station and is ready for immediate playback. This is triggered by a
 * call to `prepareToPlay`.
 */
 
extern NSString *const AdaptrAudioPlayerMusicQueuedNotification;

/**
 *  @const AdaptrAudioPlayerActiveStationDidChangeNotification
 *  Sent when active station is changed via one of the [AdaptrAudioPlayer setActiveStation]
 *  calls.
 */
extern NSString *const AdaptrAudioPlayerActiveStationDidChangeNotification;

/**
 *  @const AdaptrAudioPlayerSkipStatusNotification
 *  Sent when current item skip status is changed. Its userInfo dictionary contains
 *  'canSkip' key which value means whether you can skip current item.
 *  @see [AdaptrAudioPlayer canSkip]
 */
extern NSString *const AdaptrAudioPlayerSkipStatusNotification;

/**
 *  @const AdaptrAudioPlayerSkipFailedNotification
 *  Sent when current skip request is failed. Its userInfo contains pointer to NSError
 *  object for AdaptrAudioPlayerSkipFailureErrorKey key
 */
extern NSString *const AdaptrAudioPlayerSkipFailedNotification;

/**
 *  @const AdaptrAudioPlayerSkipFailureErrorKey
 *  A userInfo key for AdaptrAudioPlayerSkipFailedNotification
 */
extern NSString *const AdaptrAudioPlayerSkipFailureErrorKey;

/**
 *  @const AdaptrAudioPlayerLikeStatusChangeNotification
 *  Sent by [AdaptrAudioPlayer like] and [AdaptrAudioPlayer dislike] methods.
 *  Its object property points to the current AdaptrAudioPlayer instance.
 */
extern NSString *const AdaptrAudioPlayerLikeStatusChangeNotification;

/**
 *  @const AdaptrAudioPlayerTimeElapseNotification
 *  Sent every half a second during playback to report changing time.
 *  @see [AVPlayer addPeriodicTimeObserverForInterval:queue:usingBlock:]
 */
extern NSString *const AdaptrAudioPlayerTimeElapseNotification;

/**
 *  @const AdaptrAudioPlayerPreCachingCompleted
 *  Sent when precaching for stations is completed. This notification
 * is sent after a call to `prepareStations` or `prepareStations:`
 * completes.
 *  
 */
extern NSString *const AdaptrAudioPlayerPreCachingCompleted;

/**
 * @const AdaptrAudioPlayerNewClientIdAvailable
 *
 * Sent to notify sucessful generation of a new client Id.
 */

extern NSString *const AdaptrAudioPlayerNewClientIdAvailable;

/**
 *  @const AdaptrAudioPlayerStationDownloadProgress
 *  Sent to indicate progress for station download
 *
 */
extern NSString *const AdaptrAudioPlayerStationDownloadProgress;
/**
 *  @const AdaptrAudioPlayerStationListKey
 *  userInfo key for NSArray of Stations from <AdaptrAudioPlayerStationListAvailableNotification>
 */
extern NSString *const AdaptrAudioPlayerStationListKey;

/**
 * @const AudiofileKey
 * userInfo key for Audiofile object sent with <AdaptrAudioPlayerLikeStatusChangeNotification>
 */

extern NSString *const AudiofileKey;

/**
 *  @const FMAudioFormatMP3
 *  Equals to @"mp3"
 */
extern NSString *const FMAudioFormatMP3;
/**
 *  @const FMAudioFormatAAC
 *  Equals to @"aac"
 */
extern NSString *const FMAudioFormatAAC;

/**
 * @enum This enum represents the various states the AdaptrAudioPlayer can
 * be in, as identified by the `[AdaptrAudioPlayer playbackState]`.
 */
typedef NS_ENUM(NSInteger, AdaptrAudioPlayerPlaybackState) {
    
    /**
     * Offline music is available
     *
     */
    
    AdaptrAudioPlayerPlaybackStateOfflineOnly,
    
    /**
     * The server has not responded yet, so we don't know if music
     * is available or not yet.
     */
     
    AdaptrAudioPlayerPlaybackStateUninitialized,
    
    /**
     * The server has told us that we cannot stream music.
     */
    
    AdaptrAudioPlayerPlaybackStateUnavailable,
    
    /**
     *  The player is waiting for the server to give it the next song for playback.
     */
    AdaptrAudioPlayerPlaybackStateWaitingForItem,
    
    /**
     *  Initial state of the audio player.
     */
    /* 4 */  AdaptrAudioPlayerPlaybackStateReadyToPlay,
    
    /**
     *  Playback of audio is in progress
     */
    /* 5 */ AdaptrAudioPlayerPlaybackStatePlaying,
    
    /**
     *  Playback is paused
     */
    /* 6 */ AdaptrAudioPlayerPlaybackStatePaused,
    
    /**
     *  Some media did not arrive in time to continue playback, and the 
     *  player is waiting for more audio data to arrive.
     */
    AdaptrAudioPlayerPlaybackStateStalled,
    
    /**
     * The user has requested that the current song be skipped, and the
     * player is waiting for word back from the server if the skip
     * will be allowed.
     */
    AdaptrAudioPlayerPlaybackStateRequestingSkip,
    
    /**
     * When there is no more music to play in the current station, the
     * player enters this state.
     */
    AdaptrAudioPlayerPlaybackStateComplete
};

NS_ASSUME_NONNULL_END
#define kFMRemoteControlEvent @"FMRemoteControlEvent"




typedef NS_ENUM(NSInteger, MixingAudioPlayerCompletionReason) {
    
    MixingAudioPlayerCompletionReasonReachedEnd,
    
    MixingAudioPlayerCompletionReasonSkipped,
    
    MixingAudioPlayerCompletionReasonFlushed,
    
    MixingAudioPlayerCompletionReasonError
    
};

/**
 * Internal Delegate
 */

@protocol MixingAudioPlayerDelegate<NSObject>


/**
 * This is triggered when the player is READY_TO_PLAY and we have buffered
 * audio so that playback may begin immediately. This is also triggered for
 * songs that are queued up in the background so that we can switch to them
 * as soon as they are queued up, if desired.
 *
 * Note that if the player is STALLED or WAITING_FOR_ITEM then this event
 * won't be triggered - the audio will just begin playback.
 *
 * @param item the audio item ready for playback
 */
- (void) mixingAudioPlayerItemIsReadyForPlayback: (nonnull Audiofile *) item;

/**
 * The player started playback of the given audio item.
 *
 * @param item the audio item that started playback
 * @parm waiting time before playback started
 * @parm buffering time for the item
 */

- (void) mixingAudioPlayerItemDidBeginPlayback: (nonnull Audiofile *) item
                        waitingTimeForPlayBack: (NSTimeInterval) waitingTime
                        bufferingTimeforItem: (NSTimeInterval) bufferingTime;

/**
 * The player finished playback of the given audio item.
 *
 * @param item item the audio item that finished playback
 * @param reason reason the playback completed for this item
 * @param error the error (if any) that stopped playback
 */

- (void) mixingAudioPlayerItemDidFinishPlayback: (nonnull Audiofile *) item
                                    dueToReason: (MixingAudioPlayerCompletionReason) reason
                                       andError: ( NSError *_Nullable) error;

/**
 * The player was unable to prepare or retrieve the given audio item for playback.
 *
 * @param item the item that was not preparable or retrievable
 * @param error the error that caused the prepare to fail
 */

- (void) mixingAudioPlayerItemDidFailToPrepare: (nonnull Audiofile *) item
                                     withError: (NSError * _Nullable) error;

/**
 * The player changed state.
 *
 * @param state the new state of the player
 */

- (void) mixingAudioPlayerStateDidChange: (AdaptrAudioPlayerPlaybackState) state;

/**
 * This is called every .5 seconds to indicate playback time has elapsed
 */

- (void) mixingAudioPlayerPlaybackDidElapse: (CMTime) elapsedTime;

/**
 * Report errors back to Feed.fm
 */

- (void) mixingAudioPlayerError: (NSString * _Nullable) description;



@end



/**
 * Internal protocol method
 */


@protocol MixingAudioPlayer <NSObject>



/**
 * Delegate that receives events.
 */

@property (nonatomic, strong) id<MixingAudioPlayerDelegate> _Nullable eventDelegate;


/**
 * Set or retrieve the audio volume.
 */

@property (nonatomic) float volume;

/**
 * When true (the default), the player will try to normalize song volumes
 */

@property (nonatomic) BOOL normalizeVolume;

/**
 * When true, the player will honor start and end trim hinting.
 */

@property (nonatomic) BOOL trimmingEnabled;

/**
 * This is the number of seconds that songs will overlap when transitioning
 * between two songs. This defaults to 0.
 */

@property (nonatomic) float secondsOfCrossfade;

/**
 * When this is true and secondsOfCrossfade > 0, then the song we are
 * transitioning to will fade in during the crossfade. When this is
 * false, then the song we are transitioning to will start at full volume
 * while the first song starts fading out. This defaults to NO.
 */

@property (nonatomic) BOOL crossfadeInEnabled;

/**
 * Current state of the player. When this is changed, any registered delegate
 * receives notice via 'mixingAudioPlayerStateDidChange' call
 */

@property (nonatomic, readonly) AdaptrAudioPlayerPlaybackState state;

/**
 * Current item that we're trying to play
 */

@property (nonatomic, readonly)  Audiofile * _Nullable currentItem;

/**
 * Current time into the current item we're playing, or kCMTimeZero
 **/
 
@property (nonatomic, readonly) CMTime currentTime;

/**
 * Duration of the current item we're playing, or kCMTimeZero
 */

@property (nonatomic, readonly) CMTime currentDuration;

/**
 * Queue the given audio item into the player.
 *
 * @param audioItem audio item to queue up
 */


- (void) addItem: (nonnull Audiofile *) audioItem;

/**
 * Begin or resume audio playback
 */

- (void) play;

/**
 * Pause audio playback
 */

- (void) pause;

/**
 * Skip to the next song. If playback was paused, this will
 * resume playback.
 */

- (void) skip;

/**
 * Skip to the next song. If playback was paused, this will
 * resume playback.
 *
 * @param applyCrossfade if true, the current song will fade out, rather than immediately stop
 */

- (void) skipWithCrossfade: (BOOL) applyCrossfade;

/**
 * Stop playback and remove all items and queued up items from the player.
 */

- (void) flush;

/**
 * Remove all queued up items in the player and, optionally stop and
 * unload the currently playing song.
 *
 * @param includeCurrentItem if true, stop playback of current song and remove from player
 */

- (void) flushAndIncludeCurrentItem: (BOOL) includeCurrentItem;


- (void) seekStationTo: (CMTime) setTime;

- (CMTime) maxSeekableLength;

// For cancelling out an item that has a network error
- (void) cancelSecondaryItem;


@end




/**
 When a station download request is posted this delegate
 is required to track the progress and the completion of the download.
 */

@protocol StationDownloadDelegate <NSObject>

/**
 This method is called once, at the start of a station download,
 and then again after every file in the station is either fully
 downloaded or aborted.
 
 The first (and possibly final) call to this method will have 0
 failed counts and pending and total will be the number of
 files that will be downloaded for the station.
 
 The final call to this method will have a pendingCount value of 0 
 
 @param station The station that is being downloaded
 @param pendingCount the number of downloads remaining to be downloaded. The last
   call to this method will have pendingCount == 0
 @param failedCount the number of download attempts that failed
 @param totalCount the total number of files that will be downloaded
 */
-(void) stationDownloadProgress:(nonnull Station*)station
                    pendingCount:(int)pendingCount
                     failedCount:(int)failedCount
                      totalCount:(int)totalCount;

@end


/**
 When events are reported to the feed.fm servers via
 `[AdaptrAudioPlayer logEvent]` and `[AdaptrAudioPlayer logEvent:withParameters]`,
 those events will also be reported to any class implementing
 this protocol and assigned to the `[AdaptrAudioPlayer logger]` property.
 
 The SDK uses the `[AdaptrAudioPlayer logEvent]` internally, so this
 is an interface for apps to receive copies of events that feed.fm
 tracks internally so they may be passed on to other logging frameworks.
 
*/

@protocol AdaptrAudioPlayerLogger<NSObject>
/**
 *  Everything logged here goes back to Feed.fm and is also cc'd to the 'logger' property
 *
 *  @param event Log message
 */
- (void)logEvent: (nonnull NSString *)event;

/**
 *  Everything logged here goes back to Feed.fm and is also cc'd to the 'logger' property
 *
 *  @param event      Log message
 *  @param parameters Parameters
 */
- (void)logEvent: (nonnull NSString *)event
  withParameters: (nonnull NSDictionary *)parameters;
@end


/**
 
 The SDK centers around a singleton instance of this `AdaptrAudioPlayer` class, which 
 has simple methods to control music playback (`play`, `pause`, `skip`). The
 `AdaptrAudioPlayer` holds lists of `Station` objects (`stationList` and `localOfflineStationList`), one of which is always
 considered the _active_ station (`activeStation`). Once music playback has begun, there
 is a _current_ song (`currentSong`).
 
 Typical initialization and setup is as follows:

 As early as you can in your app's lifecycle (preferably in your `AppDelegate`
 or initial `ViewController`) call

     [AdaptrAudioPlayer setclientToken:@"demo" secret:@"demo"]

 to asynchronously contact the feed.fm servers, validate that the client is in a location
 that can legally stream music, and kick of a thread to retrieve a list of available
 streaming music stations.
 
 There are a number of sample credentials you can use to assist
 in testing your app out. Use one of the following strings for your
 token and secret to get the desired behavior:
 
 - 'demo' - 3 simple stations with no skip limits
 - 'badgeo' - feed.fm will treat this client as if it were accessing from outside the US
 - 'counting' - a station that just plays really short audio clips
   of a voice saying the numbers 0 through 9

 To receive notice that music is available or not available, use the
 `whenAvailable:notAvailable:` method call, which is guaranteed to call
 only one of its arguments as soon as music is deemed available or not:

    AdaptrAudioPlayer *player = [AdaptrAudioPlayer sharedPlayer];
 
    [player whenAvailable:^{
      NSLog(@"music is available!");
      // .. do something, now that you know music is available

     } notAvailable: ^{
        NSLog(@"music is not available!");
        // .. do something, like leave music button hidden

     }];

 Because music may not be available to this particular client (due
 to lack of network connectivity or geographic restrictions), we
 recommend you leave music functionality hidden by default, and
 reveal it only when confirmation is returned.

 Once music is available, use the `play`, `pause`, `skip` methods to
 control playback. The `stationList` and `localOfflineStationList` properties
 will contain lists of stations the user can switch to with the `setActiveStation:` and
 `setActiveStation:withCrossfade:` calls.

     // pick the station to play music from
     Station *station = [player.stationList getStationWithOptionKey: @"genre" Value: @"HipHop"];
     [player setActiveStation:station withCrossfade:NO];

     // when you have set the station that is about to begin playback, you
     // can optionally call prepareToPlay first so playback is immediate upon
     // calling 'play'
     [player prepareToPlay];
 
     // begin playback!
     // (if you called 'prepareToPlay', then you should first wait for
     // an AdaptrAudioPlayerMusicQueuedNotification notification to be guaranteed
     // that playback will start immediately on this call, with no intervening
     // network requests)
     [player play]
 
 Offline music can be retrieved by selecting a station in the
 `remoteOfflineStationList` and passing it to `downloadAndSyncStation:forTargetMinutes:withDelegate`.
 That will cause the contents of the station to be downloaded onto the device
 (or updated with new music, if the station was already downloaded and new
 music is available), and a new `Station` entry in `localOfflineStationList`
 that can be passed to `setActiveStation`.
 
 The `AdaptrAudioPlayer` registers with iOS so that
 playback can be paused, skipped, liked, and disliked via the lock screen.
 Additionally, iOS will display on the lock screen whatever image
 you've assigned via `[AdaptrAudioPlayer setLockScreenImage:]`.
 
 The 'prepareToPlay' method can be called before playback begins and
 when the client knows that the current station will immediately be
 played next. This call is primarily useful when you want music to begin
 playback immediately upon a call to play, with no intervening network
 requests.
 
 The `AdaptrAudioPlayer` generates events that can be hooked into
 whatever analytics service your app uses. Just have an object in your app
 (your `AppDelegate` is a good choice) implement the `AdaptrAudioPlayerLogger`
 protocol. You just need to implement two
 methods: `logEvent:` and `logEvent:withParameters:`,
 and assign the object to `[FeedMedia sharedPlayer].logger`.
 
 For assistance with building music player UI, check out the
 [UI library](http://demo.feed.fm/sdk/docs/ios/ui/latest/html)
 documentation and check out our demo app:
 
 [https://github.com/feedfm/iOS-RadioPlayer](https://github.com/feedfm/iOS-RadioPlayer)
*/

@interface AdaptrAudioPlayer : NSObject

///-----------------------------------------------------
/// @name Setup
///-----------------------------------------------------

/**
 * This method kicks off asynchronous initialization of the library and communication
 * with the feed.fm servers. This call should be made as early as possible
 * in the lifecycle of the app.
 *
 * @param token public authentication token. Use `@"demo"` during testing/development.
 * @param secret private authentication token. Use `@"demo"` during testing/development.
 */

+ (void)setClientToken:(nonnull NSString *)token secret:(nonnull NSString *)secret;

/**
 * There is only one global `AdaptrAudioPlayer` instance, and it
 * is available via this static property.
 */

+ (nonnull AdaptrAudioPlayer *)sharedPlayer;

/**
 * Change the base url of Api
 * For Internal testing only
 */

+ (void) setBaseUrl: (NSString*_Nonnull) url;


/**
 * Call one of the two callbacks as soon as we know music is available for
 * playback. One of these two blocks is
 * guaranteed to be called, and only one call will ever be made.
 * Just before the onAvailable callback is made, the list of available stations
 * is fully populated.
 *
 * Note that the callbacks are only made after attempting to contact the Feed.fm
 * servers, even if local offline music is available while waiting for a
 * response. If the player is unable to contact the Feed.fm servers or they
 * respond that no streaming music is available, but there is local offline music,
 * then the onAvailable callback will be called.
 *
 * If the player has already contacted (or failed to contact) the feed.fm
 * servers, then one of the two calls passed callbacks will be called immediately,
 * before this method returns.
 *
 *  @param onAvailable    called when music becomes or already is available
 *  @param onNotAvailable called when we determine music is not available
 */

- (void)whenAvailable: (nonnull void (^)(void)) onAvailable
         notAvailable: (nonnull void (^)(void)) onNotAvailable;

/**
 * Called when there is need to update the stationlist from server.
 * This method can  be called periodically for long running apps that do not quit for multiple days
 * If a new nession is not available then no callback will be recived
 *
 *  @param onUpdatedSessionAvailable called when session is refreshed
 */
- (void)updateSession: (nonnull void (^)(void)) onUpdatedSessionAvailable;




///-----------------------------------------------------
/// @name Playback Controls
///-----------------------------------------------------


/**
 * Starts asynchronous loading of the first song in the active station
 * so that a future call to `play` will start music instantaneously.
 * This action runs asynchronously and, when it completes, triggers
 * an `AdaptrAudioPlayerPreCachingCompleted` notification.
 */
- (void)prepareToPlay;

/**
 * Starts retrieval and playback of music in the active station.
 */
- (void)play;

/**
 * Start playback of specific song. This method only works with
 * on-demand stations.
 *
 * @param audioItem the audio item to immediately play
 */
- (void)playAudioItem: (nonnull Audiofile *) audioItem;

/**
 * Pauses music playback.
 */
- (void)pause;

/**
 *  Stops music playback and discards any cached audio.
 */
- (void)stop;

/**
 * Asynchronously request that the player skip the current song. If the
 * request is successful, the current song will stop and the next will
 * begin. If not, an `AdaptrAudioPlayerSkipFailedNotification` will be posted
 * to the default notification center and the current song will continue
 * playback.
 */
- (void)skip;

/**
 * Calls `likeAudioItem:` with the currently playing song
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)like;

/**
 * Marks the specified song as 'liked'. Updates the `[Audiofile liked]`
 * and `[Audiofile disliked]` properties.
 *
 * This triggers an `AdaptrAudioPlayerLikeStatusChangeNotification` notification
 * with a userInfo dictionary that contains the audioItem
 * object indexed by `AudiofileKey`.
 *
 * @param audioItem the audio item that is to be liked. If null, then the currently
 *     active audio item will be liked.
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)likeAudioItem: (nonnull Audiofile *)audioItem;

/**
 * Calls `dislikeAudioItem:` with the currently playing song
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)dislike;

/**
 * Marks the specified song as 'disliked'. Updates the `[Audiofile liked]`
 * and `[Audiofile disliked]` properties. 
 *
 * This triggers an `AdaptrAudioPlayerLikeStatusChangeNotification` notification
 * with a userInfo dictionary that contains the audioItem
 * object indexed by `AudiofileKey`.
 *
 * @param audioItem the audio item that is to be disliked. If null, then the currently
 *     active audio item will be disliked.
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)dislikeAudioItem: (nonnull Audiofile *)audioItem;

/**
 * Calls `unlikeAudioItem:` with the currently playing song
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)unlike;

/**
 * Marks the specified song as neither 'liked' nor 'disliked'. Updates the `[Audiofile liked]`
 * and `[Audiofile disliked]` properties.
 *
 * This triggers an `AdaptrAudioPlayerLikeStatusChangeNotification` notification
 * with a userInfo dictionary that contains the audioItem
 * object indexed by `AudiofileKey`.
 *
 * @param audioItem the audio item that is to be unliked. If null, then the currently
 *     active audio item will be unliked.
 *
 * @see [Audiofile liked]
 * @see [Audiofile disliked]
 */
- (void)unlikeAudioItem: (nonnull Audiofile *)audioItem;

/**
 * Get currently Active ClientID
 */

- (nullable NSString*) getClientId;

/**
 * Sets a previously generated clientid to be the active id.
 *
 * @param cid previously generated client id
 */

- (void) setClientId: (nonnull NSString*) cid;

/**
 * Asynchronously generate a new client id for a new user.
 * When this request is complete a NSNotification `AdaptrAudioPlayerNewClientIdAvailable` is triggered
 * with userInfo dictionary that contains the clientid
 *
 * @see setClientId:
 */
- (void) createNewClientId;


/**
 * Internal Method
 */
- (void)setDefaultControlDelegate;

/**
 * Internal Method
 */
-(void)setPlayerControlDelegate:(id<MixingAudioPlayer>_Nonnull) newPlayerControlDelegate;


/**
 * Enable/Disable auto retrying of network failures. If network disappears and this option in enabled, player will wait until network is restored and resume music playback, insted of shutting down.
 * This option must be set before setClientToken is called. By default this option is set to true.
 * If this opiton is set and setClientToken call fails due to network failure the SDK will call onUnAvailable but as network is restored OnAvailable will be automatically called again.
 */
@property (class, nonatomic) BOOL autoNetworkRetryEnabled;




///-----------------------------------------------------
/// @name Playback Status
///-----------------------------------------------------

/**
 * Current player state. As this state changes, an `AdaptrAudioPlayerPlaybackStateDidChangeNotification`
 * is triggered with the default notification center.
 */

@property (nonatomic, readonly) AdaptrAudioPlayerPlaybackState playbackState;

/**
 * The currently playing or paused song, or null if there
 * is not one.
 */
@property (nonatomic, readonly, nullable)  Audiofile *currentItem;


/**
 *  The elapsed playback time of the current item.
 */

@property (nonatomic, readonly) NSTimeInterval currentPlaybackTime;

/**
 *  Indicates the duration of the current item.
 */

@property (nonatomic, readonly) NSTimeInterval currentItemDuration;

/**
 *  The current rate of playback. Slow/fast play is not supported, so this will always be 0.0 or 1.0
 */

@property (nonatomic, readonly) float currentPlaybackRate;

/**
 * If false, then the user may not skip the currently playing
 * song. If true, the user may be able to skip the current song,
 * but the server will be queried and possibly reject the request.
 * This property is updated every time a new song
 * starts playback or after a failed skip attempt, and a
 * `AdaptrAudioPlayerSkipStatusNotification notification` is sent any
 * time this value changes.
 *
 *  @see skip
 */

@property (readonly) BOOL canSkip;

/**
 * This array holds all the Audiofiles that the user has heard
 * since playback started, including the currently playing
 * song. As new items start playback, they are appended to this array.
 *
 * This history currently does not include songs from past sessions.
 */

@property (nonatomic, readonly, nonnull) NSMutableArray<Audiofile *> *playHistory;



///-----------------------------------------------------
/// @name Playback options
///-----------------------------------------------------

/**
 * The number of seconds to crossfade between songs. This defaults to 0.
 * If crossfading values is set in server side, setting this value will have no effect as server side values have higher priority.
 */

@property (nonatomic) float secondsOfCrossfade;


/**
 * When true (the default), adjust individual song volumes so they are perceived
 * be the same loudness.
 */

@property (nonatomic) BOOL normalizeSongVolume;


/**
 *  A value between 0.0 and 1.0 relative to system volume
 */

@property (nonatomic) float mixVolume;



///-----------------------------------------------------
/// @name Current and available stations
///-----------------------------------------------------

/**
 * This is a list of streaming music stations retrieved from the server.
 * This array will not change once populated.
 **/

@property (nonatomic, readonly, nonnull) StationArray *stationList;


/**
 The list of stations available for immediate offline playback.
 This list contains any stations that have been downloaded at some point
 in the past with downloadAndSyncStation:forTargetMinutes:withDelegate.
 
 Stations listed here are available for playback even if there is no
 network connectivity, and this property is intialized as soon as the
 `AdaptrAudioPlayer` is created (before any attempt is made to contact
 the feed.fm servers.
 
 When there is internet connectivity, every station in this list will
 have a paired station (with the same name, but not necessarily same ID)
 in the remoteOfflineStationList. Use `remoteOfflineStationForLocalOfflineStation:`
 to map to the remote Station. If they have different 'identifier' values,
 then the local station should be updated by passing its pair to
 `downloadAndSyncStation:withDelegate`.
 
 @return List of stations available on disk ready for playback. never returns nil.
 */

@property (readonly, nonatomic, nonnull) StationArray *localOfflineStationList;


/**
 List of stations that are available for downloading.
 
 These stations cannot be tuned to (with setActiveStation:), rather they should be passed
 to downloadAndSyncSation:forTargetMinutes:withDelegate to be downloaded
 and made available for playback in the localOfflineStationList.
 
 This property is only populated when internet connectivity is available and
 the player is declared 'available'.

 If one of these stations has been downloaded, the `localOfflineStationForRemoteOfflineStation:`
 method can be used to find the local equivalent for playback.
 
 @return List of stations that can be downloaded for offline playback
 */

@property (readonly, nonatomic) StationArray  * _Nullable remoteOfflineStationList;


/**
 * The current station from which music is pulled. Any `Station` retrieved
 * from the `stationList` or `localOfflineStationList` properties can be assigned here.
 * Once music is available, this property is never null. When this value changes,
 * `AdaptrAudioPlayerActiveStationDidChangeNotification` is sent to the default
 * notification center.
 */

@property (nonatomic, copy, nonnull) Station *activeStation;


/**
 * Make the given station the `activeStation`. If
 * `withCrossfade` is true, any currently playing music will crossfade into the first
 * song in the new station.
 *
 *  @param station Station to tune to.
 *  @param withCrossfade if true, if crossfading is enabled, and if music is currenty
 *    playing, the currently playing song will fade into the song in the new station
 *    as soon as it is loaded.
 *
 *  @see activeStation
 */

- (void) setActiveStation: (nonnull Station *)station withCrossfade: (BOOL) withCrossfade;



///-----------------------------------------------------
/// @name Offline station management
///-----------------------------------------------------

/**
 Downloads an offline station.
 
 This method kicks off an asynchronous download to retrieve audio
 files for the given station. As the station downloads, events (1
 at a minimum) will be sent to the provided delegate.
 
 The station passed in must come from the `remoteOfflineStationList`
 
 The `minutes` attribute specifies the minimum number of minutes of
 music the client would like to be available offline. The server will make the
 decision as to whether the client already has enough music stored locally
 and, if so, whether that music should be replaced or appended to.
 
 Note that if the requested station has already been downloaded or partially
 downloaded to local storage already, only missing music will be downloaded.
 Some stations can additionally be configured (on the server) so that
 only a subset of the full station contents are saved on client devices,
 and each request to `downloadAndSyncStation:` will cause recently played
 music to be deleted and new music to be downloaded.

 @param remoteStation A station from remoteOfflineStationList
 @param minutes describes target time for music. If you need music for half an hour pass 30.
 @param delegate for receiving updates about the download.
 @see StationDownloadDelegate
 
 */

-(void) downloadAndSyncStation:(nonnull Station *)remoteStation
              forTargetMinutes:(nullable NSNumber*) minutes
                  withDelegate: (nonnull id<StationDownloadDelegate>) delegate;


/**
 Downloads an offline station.
 
 This method is the same as `downloadAndSyncStation:forTargetMinutes:withDelegate`, but
 it lets the server determine the 'targetMinutes' value. This method is preferable
 unless the amount of music to be loaded for each station varies in different contexts
 known only by the client.
 
 @param remoteStation Pass a station from remoteOfflineStationList
 @param delegate for receiving updates about the download.
 @see StationDownloadDelegate
 */

-(void) downloadAndSyncStation:(nonnull Station *)remoteStation
                  withDelegate: (nonnull id<StationDownloadDelegate>) delegate;

/**
 Deletes all locally stored files in a previously downloaded station and
 removes the station from the `localOfflineStationList`.
 
 Does nothing if the station passed in does not
 appear in the `localOfflineStationList` or if the station being
 deleted is the activeStation.
 
 @param localOfflineStation the station whose contents will be deleted.
 */

- (void) deleteOfflineStation: (nonnull Station *) localOfflineStation;

/**
 Deletes all locally stored stations.
 
 This call does nothing if the current active station is a local
 offline station.
 */

- (void) deleteAllOfflineStations;

/**
 Map a remote offline station to a local offline station.
 
 This method is useful in determining which of the available remote offline
 stations have been downloaded and stored as local offline stations.
 
 When a remote station is downloaded, a duplicate Station instance is added to the
 local offline station array. If the remote station is updated (that is,
 music is added or deleted), then the remote Station instance will have an
 ID that differs from the Station instance in the local offline station array.
 This station maps the remote Station instance to its local Station instance,
 regardless of whether their contents match up exactly.
 
 @param remoteOfflineStation a station from remoteOfflineStationList
 @return nil or a station from remoteOfflineStationList
 */

- (nullable Station *) localOfflineStationForRemoteOfflineStation: (nonnull Station *) remoteOfflineStation;

/**
 Map a locally stored offline station to its remote offline pair.
 
 This method is useful to determine if a locally downloaded station can
 be updated with new music. If the local Station `identifier` value
 doesn't match the remote Station `identifier` value, then the remote
 Station can be passed to `downloadAndSyncStation:withDelegate` to update
 the local Station.
 
 @param localOfflineStation a station from localOfflineStationList
 @return a station from remoteOfflineStationList
 */

- (nullable Station *) remoteOfflineStationForLocalOfflineStation: (nonnull Station *) localOfflineStation;

///-----------------------------------------------------
/// @name Logging and reporting
///-----------------------------------------------------


/**
 * Any calls to `logEvent:` or `logEvent:withParameters:`
 * are also sent to the logger assigned to this property, if
 * any.
 */

@property (nonatomic, weak, nullable) id<AdaptrAudioPlayerLogger> logger;

/**
 *  Everything logged here goes back to Feed.fm and is also cc'd to the `logger` property
 *
 *  @param event Log message
 */
- (void) logEvent: (nonnull NSString *)event;

/**
 *  Everything logged here goes back to Feed.fm and is also cc'd to the `logger` property
 *
 *  @param event      Log message
 *  @param parameters Parameters
 */
- (void) logEvent: (nonnull NSString *)event
   withParameters: (nonnull NSDictionary *)parameters;



///-----------------------------------------------------
/// @name Misc..
///-----------------------------------------------------

/**
 * Disable the creation and management of  AVAudioSession by Feed SDK completely
 */


@property (nonatomic) BOOL disableAVAudioSession;
/**
 * Order specifies priority (earlier elements are preferred).
 * Nil-ing this property will allow any format to be served, but is not recommended.
 * Set to `FMAudioFormatMP3` to exclude AAC files.
 * Defaults to `@[FMAudioFormatAAC,FMAudioFormatMP3]`.
 *
 * In the normal course of events, clients do not need to make use of this.
 */
@property (nonatomic, strong, nullable) NSArray *supportedAudioFormats;

/**
 * Set to specify available bandwidth, in kbps. Set to 0 to request the highest available quality.
 * Defaults to 48.
 */
@property (nonatomic) NSInteger maxBitrate;

#if TARGET_OS_TV || TARGET_OS_MACCATALYST
#else

/**
 * This status bar notification is used to announce song changes to
 * the user. Access the properties on this object to change
 * how the notification is styled and to add handlers for when the
 * notification is tapped.
 *
 * Details on this object can be found at https://github.com/cezarywojcik/CWStatusBarNotification
 */


@property (nonatomic, readonly, nullable) CWStatusBarNotification *statusBarNotification;

#endif

/**
 * The player displays a notification at the top of the screen during song
 * transitions by default. If you are currently showing the active song, which
 * means a notification isn't needed, it can be disabled by setting this
 * property to YES. Don't forget to set this to NO when you stop showing the
 * active song.
 *
 * To alter how notifications are displayed, see the `statusBarNotification`
 * property.
 */

@property (nonatomic) BOOL disableSongStartNotifications;

/**
 * If YES (the default), then the Feed.fm library will
 * register with the MPRemoteCommandCenter to handle play/pause/skip/like/dislike
 * commands upon start of playback. 
 */

@property (nonatomic) BOOL doesHandleRemoteCommands;

/**
 * When not NULL, this causes the AdaptrAudioPlayer to not update
 * the MPNowPlayingInfoCenter metadata nor enable or disable the like,
 * dislike, and next track MPFeedbackCommands. See the documentation
 * of FMLockScreenDelegate for more information. Note that the
 * AdaptrAudioPlayer will still register to handle the MPRemoteCommandCenter
 * commands (play/pause/skip/like/dislike) unless doesHandleRemoteCommands
 * is also set to NO.
 */

@property (nonatomic, nullable) id<LockScreenDelegate> lockScreenDelegate;

/**
 *  Assigns the image to be displayed on the lock screen when music is playing.
 *
 *  @param image The image to be added to the lock screen
 */

- (void)setLockScreenImage: (nonnull UIImage *)image;

/**
 * Utility function to map state to string.
 *
 * @param type the playback state to map to an NSString
 */

+ (nonnull NSString *) nameForType:(AdaptrAudioPlayerPlaybackState)type;


/**
 * Destroys the instance of the player, use this call if you wish to set new tokens.
 *
 */

- (void)destroy;


///-----------------------------------------------------
/// @name Deprecated
///-----------------------------------------------------


/**
 @deprecated Clients should use the `StationArray` interface to
 search for stations based on option values.
 
 Search throught the list of available stations, and return one that has
 options that match those passed in via optionKeysAndValues. This differs from
 getStationWithOptionKey:Value: in that you can specify multiple key/value
 pairs, like so:
 
   [player getStationWithOptions: @{ @"genre": @"80s", @"bpm" : @"slow" }
 
 This method returns the first station with the matching values, or nil.
 
 @param optionKeysAndValues key value pairs to search for
 @return a station whose options contain optionKeysAndValues
 
 */

- (nullable Station *) getStationWithOptions: (nonnull NSDictionary *) optionKeysAndValues DEPRECATED_ATTRIBUTE;

/**
 @deprecated Clients should use the `StationArray` interface to
 search for stations based on option values.
 
 Similar to getStationWithOptions:, but this method returns all the stations
 that match the passed in optionsKeysAndValues.
 
 @param optionKeysAndValues key value pairs to search for
 @return an array of stations whose options contain optionKeysAndValues. never nil.
 
 */

- (nullable NSArray<Station *> *) getAllStationsWithOptions: (nonnull NSDictionary *) optionKeysAndValues DEPRECATED_ATTRIBUTE;


/**
 * @deprecated Clients should find Station references by pulling them from
 * stationList or localOfflineStationList and then assigning the reference
 * to the activeStation property or calling setActiveStation:withCrossfade:
 *
 *  Finds a station with the given name and assigns it to the `activeStation`.
 *
 *  @param name Station name. Should not be nil.
 *
 *  @return true if a station with the given name is found
 *  @see activeStation
 */

- (BOOL) setActiveStationByName: (nonnull NSString *)name DEPRECATED_ATTRIBUTE;

/**
 * @deprecated Clients should find Station references by pulling them from
 * stationList or localOfflineStationList and then assigning the reference
 * to the activeStation property or calling setActiveStation:withCrossfade:
 *
 * Finds a station with the given name and assigns it to the `activeStation`. If
 * `withCrossfade` is true, any currently playing music will crossfade into the first
 * song in the new station.
 *
 *  @param name Station name. Should not be nil.
 *  @param withCrossfade if true, if crossfading is enabled, and if music is currenty
 *    playing, the currently playing song will fade into the song in the new station
 *    as soon as it is loaded.
 *
 *  @return true if a station with the given name is found
 *  @see activeStation
 */

- (BOOL) setActiveStationByName: (nonnull NSString *)name withCrossfade: (BOOL) withCrossfade DEPRECATED_ATTRIBUTE;

/**
 * @deprecated Clients should find Station references by pulling them from
 * stationList, localOfflineStationList, or remoteOfflineStationList.
 *
 * Search through the list of available stations, and return the one that has
 * an option attribute named 'key' with a string value of 'value'.
 *
 * @param key name of attribute to inspect
 * @param value attribute value that matching station should contain
 */

- (nullable Station *) getStationWithOptionKey:  (nonnull NSString *) key Value: (nonnull NSObject *) value DEPRECATED_ATTRIBUTE;

/**
 * @deprected This method is called internally now and clients need not call it.
 *
 * @param stations list of stations to prepare
 */
- (void)prepareStations:(nullable NSArray<Station *> *) stations DEPRECATED_ATTRIBUTE;

/**
 * @deprecated Clients should look for the AdaptrAudioPlayerMusicQueuedNotification
 * notification to know when music is queued up in the player, rather than
 * rely on this property, which will be removed in the next major version.
 *
 * Indicates if the SDK has retrieved the next song for playback from the
 * server and is ready to start playing it.
 */
@property (nonatomic, readonly) BOOL isPreparedToPlay DEPRECATED_ATTRIBUTE;

/**
 @deprecated local detection is no longer performed by this library
 
 This call to initialize the library and then detect whether the user had any local
 music available for playback.
 
 @param token public authentication token. Use `@"demo"` during testing/development.
 @param secret private authentication token. Use `@"demo"` during testing/development.
 @param detectLocalMusic when true, the user's local media collection will be queried to
 sample what type of music they listen to
 */

+ (void)setClientToken:(nonnull NSString *)token secret:(nonnull NSString *)secret detectLocalMusic:(BOOL) detectLocalMusic DEPRECATED_ATTRIBUTE;

/**
 * Seek station by give no of seconds.
 * Max allowed value can be obtained by maxSeekableLength
 * @see maxSeekableLength
 * @param time in seconds
 */
- (void) seekStationBy: (NSTimeInterval) time;

/**
 * Max no of seconds that can be seeked in the current station
 * @return Time in seconds 
 */
- (NSTimeInterval) maxSeekableLength ;


/**
 * This method sets the default configuration values when the AdaptrAudioPlayer
 * initializes an AVAudioSession.
 *
 * When playback starts, the AdaptrAudioPlayer needs to ensure there is an
 * active AVAudioSession. By default, the player sets the AVAudioSessionCategory to
 * AVAudioSessionCategoryPlayback, the mode to AVAudioSessionModeDefault,
 * and the options to AVAudioSessionCategoryOptionMixWithOthers. If you would
 * like the player to use alternate settings, assign them here before playback begins.
 *
 * (note: this method does not immediately call
 * AVAudioSession setCategory:mode:options:error - it only assigns the default values
 * that the player will use when it needs to get an AVAudioSession)
 *
 * @param category category to request when player gets AVAudioSession
 * @param mode  mode to request when player gets AVAudioSession
 * @param options category options to set when player gets AVAudioSession
 *
 */
- (void) setAVAudioSessionCategory: (nonnull AVAudioSessionCategory) category
                              mode: (nonnull AVAudioSessionMode) mode
                           options: (AVAudioSessionCategoryOptions) options;

/**
 * This method sends all Feed SDK generated logs since the start of the application
 * to feed.fm for manual debugging. If you are experiencing unexpected behavior with the
 * SDK, call this method after the bad behavior occurs and let your customer support
 * person at feed know what value you passed for 'label'.
 *
 * @param label a unique label to assist feed.fm engineers in finding this specific log
 */

- (void) submitLogsForRemoteDebuggingWithLabel: (nonnull NSString *) label;



-(void) searchForQuery:(NSString *_Nonnull)query
               station:(NSString *_Nullable)stationId
                pageNo:(NSNumber *_Nonnull)page
        resultsPerPage:(NSNumber *_Nullable)resultsPerPage;


-(void) requestTracksForStation:(NSString *_Nonnull)stationId
                         pageNo:(NSNumber *_Nonnull)page
                 resultsPerPage:(NSNumber *_Nullable)resultsPerPage;
@end