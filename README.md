# iOS-SDK
Adaptr iOS SDK


## Integration
### Swift Package Manager

From Xcode 12, you can use [Swift Package Manager](https://swift.org/package-manager/) to add Adaptr to your project.

0. Select File > Swift Packages > Add Package Dependency. Enter `https://github.com/Adaptr-Music/iOS-SDK` in the "Choose Package Repository" dialog.
0. In the next page, specify the version resolving rule as "Up to Next Major" with "0.0.1" as its earliest version.
0. After Xcode checking out the source and resolving the version, you can choose the "Adaptr" library and add it to your app target.



## Getting started

The SDK centers around a singleton instance of this `AdaptrAudioPlayer` class, which has simple methods to control music playback (play, pause, skip). The AdaptrAudioPlayer holds a list of Station objects (stationList), one of which is always considered the active station (activeStation). Once music playback has begun, there is a current song (currentSong).

Typical initialization and setup is as follows:

As early as you can in your app’s lifecycle (preferably in your AppDelegate or initial ViewController) call
```Objective-C
[AdaptrAudioPlayer setclientToken:@"demo" secret:@"demo"]
```
to asynchronously contact the adaptr servers, validate that the client is in a location that can legally play music, and then retrieve a list of available music stations.


```Objective-C
AdaptrAudioPlayer *player = [AdaptrAudioPlayer sharedPlayer];

[player whenAvailable:^{
  NSLog(@"music is available!");
  // .. do something, now that you know music is available

  // set player settings
  player.crossfadeInEnabled = true;
  player.secondsOfCrossfade = 4;
  [player play];

 } notAvailable: ^{
    NSLog(@"music is not available!");
    // .. do something, like leave music button hidden

 }];
 // Set Notifications for ex to listen for player events
 [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(stateDidChange:) name:AdaptrAudioPlayerPlaybackStateDidChangeNotification object:player];
```

### Documentaion

Complete AppleDocs can be viewed at https://docs.adaptr.com/ios/latest/index.html
