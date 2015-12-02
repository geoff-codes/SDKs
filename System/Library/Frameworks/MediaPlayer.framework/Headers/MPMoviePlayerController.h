//
//  MPMoviePlayerController.h
//  MediaPlayer
//
//  Copyright 2008 Apple, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <MediaPlayer/MediaPlayerDefines.h>

@class MPMoviePlayerControllerInternal;

typedef enum {
    MPMovieScalingModeNone,        // No scaling applied at all.
    MPMovieScalingModeAspectFit,   // Uniform scale until one dimention fits.  One dimension may be filled with bars the color of the backgroundColor property.
    MPMovieScalingModeAspectFill,  // Uniform scale until the movie fills the visible bounds.  One dimension may have clipped contents.
    MPMovieScalingModeFill         // Non-uniform scale.  Both render dimensions will exactly match the visible bounds.
} MPMovieScalingMode;

typedef enum {
    MPMovieControlModeDefault,     // Standard controls (e.g. play/pause, volume slider, timeline) are visible
    MPMovieControlModeVolumeOnly,  // Only the volume control is visible
    MPMovieControlModeHidden       // No controls are visible
} MPMovieControlMode;


MP_EXTERN_CLASS @interface MPMoviePlayerController : NSObject {
@private
    MPMoviePlayerControllerInternal *_internal;
}

// Create a full screen player for the movie specified by URL.
- (id)initWithContentURL:(NSURL *)url;

// The URL for the video content, provided at init time.
@property(nonatomic, readonly) NSURL *contentURL;

// If not already visible, makes the player visible and then starts playing. If the player is paused, this resumes paused playback.
- (void)play;

// If visible, stops playback and then hides the player.  Calling play again will start movie at beginning.
- (void)stop;

// The background color shown during the transition to and from playback, and if the movie does not fill the screen. Defaults to black (nil).
@property(nonatomic, retain) UIColor *backgroundColor;

// Determines how the content scales to fit the screen. Defaults to MPMovieScalingModeAspectFit.
@property(nonatomic) MPMovieScalingMode scalingMode;

// Enables controls which allow the user to adjust movie playback. Defaults to MPMovieControlModeDefault.
@property(nonatomic) MPMovieControlMode movieControlMode;

// The time relative to the duration of the video when playback should start, if possible. Defaults to 0.0.
// When set, the closest key frame before the provided time will be used as the starting frame.
@property(nonatomic) NSTimeInterval initialPlaybackTime __OSX_AVAILABLE_STARTING(__MAC_NA, __IPHONE_3_0);

@end

MP_EXTERN NSString *const MPMoviePlayerContentPreloadDidFinishNotification; // userInfo contains NSError for @"error" key if preloading fails
MP_EXTERN NSString *const MPMoviePlayerScalingModeDidChangeNotification;
MP_EXTERN NSString *const MPMoviePlayerPlaybackDidFinishNotification;
