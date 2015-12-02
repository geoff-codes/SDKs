/*
 *  CTLine.h
 *  CoreText
 *
 *  Copyright (c) 2003-2012 Apple Inc. All rights reserved.
 *
 */

/*!
    @header

    Thread Safety Information

    All functions in this header are thread safe unless otherwise specified.
*/

#ifndef __CTLINE__
#define __CTLINE__

#include <CoreText/CTDefines.h>
#include <CoreFoundation/CFArray.h>
#include <CoreFoundation/CFAttributedString.h>
#include <CoreGraphics/CGContext.h>
#include <AvailabilityMacros.h>

#if defined(__cplusplus)
extern "C" {
#endif

/* --------------------------------------------------------------------------- */
/* Line Types */
/* --------------------------------------------------------------------------- */

typedef const struct __CTLine * CTLineRef;


/*!
    @enum       CTLineBoundsOptions
    @abstract   Options for CTLineGetBoundsWithOptions.

    @constant   kCTLineBoundsExcludeTypographicLeading
                Pass this option to exclude typographic leading.

    @constant   kCTLineBoundsExcludeTypographicShifts
                Pass this option to ignore cross-stream shifts due to
                positioning (such as kerning or baseline alignment).

    @constant   kCTLineBoundsUseHangingPunctuation
                Normally line bounds include all glyphs; pass this option to
                treat standard punctuation hanging off either end of the line
                as fully hanging.

    @constant   kCTLineBoundsUseGlyphPathBounds
                Pass this option to use glyph path bounds rather than the
                default typographic bounds.

    @constant   kCTLineBoundsUseOpticalBounds
                Pass this option to use optical bounds. This option overrides
                kCTLineBoundsUseGlyphPathBounds.
*/

typedef CF_OPTIONS(CFOptionFlags, CTLineBoundsOptions) {
    kCTLineBoundsExcludeTypographicLeading  = 1 << 0,
    kCTLineBoundsExcludeTypographicShifts   = 1 << 1,
    kCTLineBoundsUseHangingPunctuation      = 1 << 2,
    kCTLineBoundsUseGlyphPathBounds         = 1 << 3,
    kCTLineBoundsUseOpticalBounds           = 1 << 4
};


/*!
    @enum       CTLineTruncationType
    @abstract   Truncation types required by CTLineCreateTruncatedLine. These
                will tell truncation engine which type of truncation is being
                requested.

    @constant   kCTLineTruncationStart
                Truncate at the beginning of the line, leaving the end portion
                visible.

    @constant   kCTLineTruncationEnd
                Truncate at the end of the line, leaving the start portion
                visible.

    @constant   kCTLineTruncationMiddle
                Truncate in the middle of the line, leaving both the start
                and the end portions visible.
*/

typedef CF_ENUM(uint32_t, CTLineTruncationType) {
    kCTLineTruncationStart  = 0,
    kCTLineTruncationEnd    = 1,
    kCTLineTruncationMiddle = 2
};


/*!
    @function   CTLineGetTypeID
    @abstract   Returns the CFType of the line object
*/

CFTypeID CTLineGetTypeID( void ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/* --------------------------------------------------------------------------- */
/* Line Creation */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTLineCreateWithAttributedString
    @abstract   Creates a single immutable line object directly from an
                attributed string.

    @discussion This will allow clients who need very simple line generation to
                create a line without needing to create a typesetter object. The
                typesetting will be done under the hood. Without a typesetter
                object, the line cannot be properly broken. However, for simple
                things like text labels and other things, this is not an issue.

    @param      string
                The string which the line will be created for.

    @result     This function will return a reference to a CTLine object if the
                call was successful. Otherwise, it will return NULL.
*/

CTLineRef CTLineCreateWithAttributedString(
    CFAttributedStringRef string ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineCreateTruncatedLine
    @abstract   Creates a truncated line from an existing line.

    @param      line
                The line that you want to create a truncated line for.

    @param      width
                The width at which truncation will begin. The line will be
                truncated if its width is greater than the width passed in this.

    @param      truncationType
                The type of truncation to perform if needed.

    @param      truncationToken
                This token will be added to the point where truncation took place
                to indicate that the line was truncated. Usually, the truncation
                token is the ellipsis character (U+2026). If this parameter is
                set to NULL, then no truncation token is used, and the line is
                simply cut off. The line specified in truncationToken should have
                a width less than the width specified by the width parameter. If
                the width of the line specified in truncationToken is greater,
                this function will return NULL if truncation is needed.

    @result     This function will return a reference to a truncated CTLine
                object if the call was successful. Otherwise, it will return
                NULL.
*/

CTLineRef CTLineCreateTruncatedLine(
    CTLineRef line,
    double width,
    CTLineTruncationType truncationType,
    CTLineRef truncationToken ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineCreateJustifiedLine
    @abstract   Creates a justified line from an existing line.

    @param      line
                The line that you want to create a justified line for.

    @param      justificationFactor
                Allows for full or partial justification. When set to 1.0 or
                greater indicates, full justification will be performed. If less
                than 1.0, varying degrees of partial justification will be
                performed. If set to 0 or less, then no justification will be
                performed.

    @param      justificationWidth
                The width to which the resultant line will be justified. If
                justificationWidth is less than the actual width of the line,
                then negative justification will be performed ("text squishing").

    @result     This function will return a reference to a justified CTLine
                object if the call was successful. Otherwise, it will return
                NULL.
*/

CTLineRef CTLineCreateJustifiedLine(
    CTLineRef line,
    CGFloat justificationFactor,
    double justificationWidth ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/* --------------------------------------------------------------------------- */
/* Line Access */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTLineGetGlyphCount
    @abstract   Returns the total glyph count for the line object.

    @discussion The total glyph count is equal to the sum of all of the glyphs in
                the glyph runs forming the line.

    @param      line
                The line that you want to obtain the glyph count for.

    @result     The total glyph count for the line passed in.
*/

CFIndex CTLineGetGlyphCount(
    CTLineRef line ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetGlyphRuns
    @abstract   Returns the array of glyph runs that make up the line object.

    @param      line
                The line that you want to obtain the glyph run array for.

    @result     A CFArrayRef containing the CTRun objects that make up the line.
*/

CFArrayRef CTLineGetGlyphRuns(
    CTLineRef line ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetStringRange
    @abstract   Gets the range of characters that originally spawned the glyphs
                in the line.

    @param      line
                The line that you want to obtain the string range from.

    @result     A CFRange that contains the range over the backing store string
                that spawned the glyphs. If the function fails for any reason, an
                empty range will be returned.
*/

CFRange CTLineGetStringRange(
    CTLineRef line ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetPenOffsetForFlush
    @abstract   Gets the pen offset required to draw flush text.

    @param      line
                The line that you want to obtain a flush position from.

    @param      flushFactor
                Specifies what kind of flushness you want. A flushFactor of 0 or
                less indicates left flush. A flushFactor of 1.0 or more indicates
                right flush. Flush factors between 0 and 1.0 indicate varying
                degrees of center flush, with a value of 0.5 being totally center
                flush.

    @param      flushWidth
                Specifies the width that the flushness operation should apply to.

    @result     A value which can be used to offset the current pen position for
                the flush operation.
*/

double CTLineGetPenOffsetForFlush(
    CTLineRef line,
    CGFloat flushFactor,
    double flushWidth ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineDraw
    @abstract   Draws a line.

    @discussion This is a convenience call, since the line could be drawn
                run-by-run by getting the glyph runs and accessing the glyphs out
                of them. Note that this call may leave the graphics context in
                any state and does not flush the context after the draw
                operation.

    @param      line
                The line that you want to draw.

    @param      context
                The context to which the line will be drawn.
*/

void CTLineDraw(
    CTLineRef line,
    CGContextRef context ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/* --------------------------------------------------------------------------- */
/* Line Measurement */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTLineGetTypographicBounds
    @abstract   Calculates the typographic bounds for a line.

	@discussion A line's typographic width is the distance to the rightmost
	            glyph advance width edge. Note that this distance includes
	            trailing whitespace glyphs.

    @param      line
                The line that you want to calculate the typographic bounds for.

    @param      ascent
                Upon return, this parameter will contain the ascent of the line.
                This may be set to NULL if not needed.

    @param      descent
                Upon return, this parameter will contain the descent of the line.
                This may be set to NULL if not needed.

    @param      leading
                Upon return, this parameter will contain the leading of the line.
                This may be set to NULL if not needed.

    @result     The typographic width of the line. If line is invalid, this
                function will always return zero.

    @seealso    CTLineGetTrailingWhitespaceWidth
*/

double CTLineGetTypographicBounds(
    CTLineRef line,
    CGFloat* ascent,
    CGFloat* descent,
    CGFloat* leading ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetBoundsWithOptions
    @abstract   Calculates the bounds for a line.

    @param      line
                The line that you want to calculate the bounds for.

    @param      options
                Desired options or 0 if none.

    @result     The bounds of the line as specified by the type and options,
                such that the coordinate origin is coincident with the line
                origin and the rect origin is at the bottom left. If the line
                is invalid this function will return CGRectNull.
*/

CGRect CTLineGetBoundsWithOptions(
    CTLineRef line,
    CTLineBoundsOptions options ) CT_AVAILABLE_STARTING( __MAC_10_8, __IPHONE_6_0);


/*!
    @function   CTLineGetTrailingWhitespaceWidth
    @abstract   Calculates the trailing whitespace width for a line.

    @param      line
                The line that you want to calculate the trailing whitespace width
                for. Creating a line for a width can result in a line that is
                actually longer than the desired width due to trailing
                whitespace. Normally this is not an issue due to whitespace being
                invisible, but this function may be used to determine what amount
                of a line's width is due to trailing whitespace.

    @result     The width of the line's trailing whitespace. If line is invalid,
                this function will always return zero.
*/

double CTLineGetTrailingWhitespaceWidth(
    CTLineRef line ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetImageBounds
    @abstract   Calculates the image bounds for a line.

    @discussion The image bounds for a line is the union of all non-empty glyph
                bounding rects, each positioned as it would be if drawn using
                CTLineDraw using the current context. Note that the result is
                ideal and does not account for raster coverage due to rendering.
                This function is purely a convenience for using glyphs as an
                image and should not be used for typographic purposes.

    @param      line
                The line that you want to calculate the image bounds for.

    @param      context
                The context which the image bounds will be calculated for. This
                is required because the context could have settings in it that
                can cause changes in the image bounds.

    @result     A rectangle that tightly encloses the paths of the line's glyphs,
                which will be translated by the supplied context's text position.
                If the line or context is invalid, CGRectNull will be returned.

    @seealso    CTLineGetTypographicBounds
    @seealso    CTLineGetBoundsWithOptions
    @seealso    CTLineGetPenOffsetForFlush
*/

CGRect CTLineGetImageBounds(
    CTLineRef line,
    CGContextRef context ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/* --------------------------------------------------------------------------- */
/* Line Caret Positioning and Highlighting */
/* --------------------------------------------------------------------------- */

/*!
    @function   CTLineGetStringIndexForPosition
    @abstract   Performs hit testing.

    @discussion This function can be used to determine the string index for a
                mouse click or other event. This string index corresponds to the
                character before which the next character should be inserted.
                This determination is made by analyzing the string from which a
                typesetter was created and the corresponding glyphs as embodied
                by a particular line.

    @param      line
                The line being examined.

    @param      position
                The location of the mouse click relative to the line's origin.

    @result     The string index for the position. Relative to the line's string
                range, this value will be no less than the first string index and
                no greater than one plus the last string index. In the event of
                failure, this function will return kCFNotFound.
*/

CFIndex CTLineGetStringIndexForPosition(
    CTLineRef line,
    CGPoint position ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


/*!
    @function   CTLineGetOffsetForStringIndex
    @abstract   Determines the graphical offset(s) for a string index.

    @discussion This function returns the graphical offset(s) corresponding to
                a string index, suitable for movement between adjacent lines or
                for drawing a custom caret. For the former, the primary offset
                may be adjusted for any relative indentation of the two lines;
                a CGPoint constructed with the adjusted offset for its x value
                and 0.0 for its y value is suitable for passing to
                CTLineGetStringIndexForPosition. In either case, the primary
                offset corresponds to the portion of the caret that represents
                the visual insertion location for a character whose direction
                matches the line's writing direction.

    @param      line
                The line from which the offset is requested.

    @param      charIndex
                The string index corresponding to the desired position.

    @param      secondaryOffset
                An output parameter that will be set to the secondary offset
                along the baseline for charIndex. When a single caret is
                sufficient for a string index, this value will be the same as
                the primary offset, which is the return value of this function.
                This parameter may be NULL.

    @result     The primary offset along the baseline for charIndex, or 0.0 in
                the event of failure.
*/

CGFloat CTLineGetOffsetForStringIndex(
    CTLineRef line,
    CFIndex charIndex,
    CGFloat* secondaryOffset ) CT_AVAILABLE_STARTING( __MAC_10_5, __IPHONE_3_2);


#if defined(__cplusplus)
}
#endif

#endif
