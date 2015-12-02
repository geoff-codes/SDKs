//
//  NSTextStorage.h
//  UIKit
//
//  Copyright (c) 2011-2013, Apple Inc. All rights reserved.
//

#import <Foundation/NSObject.h>
#import <Foundation/NSNotification.h>
#import <Foundation/NSAttributedString.h>
#import <UIKit/NSAttributedString.h>

@class NSArray, NSLayoutManager;

@protocol NSTextStorageDelegate;

typedef NS_OPTIONS(NSUInteger, NSTextStorageEditActions) {
    NSTextStorageEditedAttributes = (1 << 0),
    NSTextStorageEditedCharacters = (1 << 1)
} NS_ENUM_AVAILABLE_IOS(7_0);


/* Note for subclassing NSTextStorage: NSTextStorage is a semi-abstract subclass of NSMutableAttributedString. It implements change management (beginEditing/endEditing), verification of attributes, delegate handling, and layout management notification. The one aspect it does not implement is the actual attributed string storage --- this is left up to the subclassers, which need to override the two NSMutableAttributedString primitives in addition to two NSAttributedString primitives:
 
 - (NSString *)string;
 - (NSDictionary *)attributesAtIndex:(NSUInteger)location effectiveRange:(NSRangePointer)range;

 - (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)str;
 - (void)setAttributes:(NSDictionary *)attrs range:(NSRange)range;
 
 These primitives should perform the change then call edited:range:changeInLength: to get everything else to happen.
*/

NS_CLASS_AVAILABLE_IOS(7_0) @interface NSTextStorage : NSMutableAttributedString

/**************************** Layout manager ****************************/

// NSLayoutManager objects owned by the receiver.
@property(readonly, NS_NONATOMIC_IOSONLY) NSArray *layoutManagers;

// Adds aLayoutManager to the receiver.  Sends -[NSLayoutManager setTextStorage:] to aLayoutManager with the receiver.
- (void)addLayoutManager:(NSLayoutManager *)aLayoutManager;

// Removes aLayoutManager from the receiver if already owned by it.  Sends -[NSLayoutManager setTextStorage:] to aLayoutManager with nil.
- (void)removeLayoutManager:(NSLayoutManager *)aLayoutManager;


/**************************** Pending edit info ****************************/
/* These methods return information about the editing status. Especially useful when there are outstanding beginEditing calls or during processEditing...
*/

// The NSTextStorageEditActions mask indicating that there are pending changes for attributes, characters, or both.
@property(NS_NONATOMIC_IOSONLY) NSTextStorageEditActions editedMask;

// The range for pending changes. {NSNotFound, 0} when there is no pending changes.
@property(NS_NONATOMIC_IOSONLY) NSRange editedRange;

// The length delta for the pending changes.
@property(NS_NONATOMIC_IOSONLY) NSInteger changeInLength;


/**************************** Delegate ****************************/

@property(assign, NS_NONATOMIC_IOSONLY) id <NSTextStorageDelegate> delegate;


/**************************** Edit management ****************************/

// Notifies and records a recent change.  If there are no outstanding -beginEditing calls, this method calls -processEditing to trigger post-editing processes.  This method has to be called by the primitives after changes are made if subclassed and overridden.  editedRange is the range in the original string (before the edit).
- (void)edited:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta;

// Sends out -textStorage:willProcessEditing, fixes the attributes, sends out -textStorage:didProcessEditing, and notifies the layout managers of change with the -processEditingForTextStorage:edited:range:changeInLength:invalidatedRange: method.  Invoked from -edited:range:changeInLength: or -endEditing.
- (void)processEditing;


/**************************** Attribute fixing ****************************/

// Indicates if the receiver fixes invalidated attributes lazily.  The concrete UIKit subclass fixes attributes lazily by default.  The abstract class (hence, all custom subclasses) is not lazy.
@property(readonly, NS_NONATOMIC_IOSONLY) BOOL fixesAttributesLazily;

// Notes the range of attributes that requires validation.  If the NSTextStorage is not lazy this just calls fixAttributesInRange:.  If it is lazy this instead just records the range needing fixing in order to do it later.
- (void)invalidateAttributesInRange:(NSRange)range;

// Ensures all attributes in range are validated and ready to be used.  An NSTextStorage that is lazy is required to call the following method before accessing any attributes.  This gives the attribute fixing a chance to occur if necessary.  NSTextStorage subclasses that wish to support laziness must call it from all attribute accessors that they implement.  The default concrete subclass does call this from its accessors.
- (void)ensureAttributesAreFixedInRange:(NSRange)range;

@end


/****  NSTextStorage delegate methods ****/

@protocol NSTextStorageDelegate <NSObject>
@optional

// Sent inside -processEditing right before fixing attributes.  Delegates can change the characters or attributes.
- (void)textStorage:(NSTextStorage *)textStorage willProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta NS_AVAILABLE_IOS(7_0);

// Sent inside -processEditing right before notifying layout managers.  Delegates can change the attributes.
- (void)textStorage:(NSTextStorage *)textStorage didProcessEditing:(NSTextStorageEditActions)editedMask range:(NSRange)editedRange changeInLength:(NSInteger)delta NS_AVAILABLE_IOS(7_0);

@end


/**** Notifications ****/

UIKIT_EXTERN NSString * const NSTextStorageWillProcessEditingNotification NS_AVAILABLE_IOS(7_0);
UIKIT_EXTERN NSString * const NSTextStorageDidProcessEditingNotification NS_AVAILABLE_IOS(7_0);
