//
//  PlayList.h
//  FeedMediaCore
//
//  Created by Arveen kumar on 07/12/2020.
//  Copyright © 2020 Feed Media. All rights reserved.
//

#ifndef Header_h
#define Header_h


#endif /* Header_h */



#import <Foundation/Foundation.h>
#import "Audiofile.h"


extern NSString * _Nonnull const AdaptrPlaylistChangedNotification;
/**
 * Editor is part of every playlist  that can be used to modify a playlist and has built in handy function to make your work easier.
 * To make any changes to a playlist final do not forget to call save.
 */
@protocol Editor<NSObject>

/**
 * Get the fist Item in the play list
 */
-(Audiofile*_Nullable) first;
/**
 * Shuffle the playlist in random order
 */
-(void) shufflePlaylist;
/**
 * View current playlist as an NSArray
 */
-(NSArray<Audiofile*>*_Nullable) viewPlayList;
/**
* Clear all items from the playlist
 */
-(void) clearPlaylist;
/**
 * Add files to Playlist
 */
-(void) addFilestoPlaylist:(NSArray<Audiofile*>*_Nonnull) files;
/**
 * Add a single file
 */

-(void) addFiletoPlaylist:(Audiofile*_Nonnull) file;

/**
 * Remove an item fron playlist
 */
-(void) removeFromPlaylist:(Audiofile*_Nonnull) file;
/**
 * Save changes to the playlist, this should be called eveytime any changes are made, even on the current playlist
 */
-(void) saveChanges;

/**
 * Change the location of items insode the playlist.
 */

-(void) moveItem:(Audiofile*_Nonnull) file toIndex:(int) index;


@end

/**
 * Plalist are user created lists of songs and do not correspond to the playlists created on the server side. Playlists created in client portal are treated as on demand stations in SDKs
 */

@interface PlayList : NSObject 


/*
 * Playlist identifier.
 */
@property (readonly) NSString * _Nonnull playListId;

/**
 *  Playlist  name
 */
@property (readonly) NSString * _Nonnull name;

/**
 * A short description of playlist
 */

@property (readonly) NSString * _Nullable playListDescription;

/**
 * Playlist Editor to be used for manipulation of the playlist
 */

@property (readonly) id<Editor> _Nonnull editor;

/**
 * Constructor to create new playlists. Internal use only.
 */
- (id _Nonnull) initWithName: (NSString*_Nonnull) name andDescription:(NSString*_Nonnull) description andAudioFiles:(NSArray<Audiofile*>*_Nullable) audiofiles;


@end
    

