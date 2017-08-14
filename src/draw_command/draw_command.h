#pragma once
#include <pebble.h>
#include "../macros.h"
#include "../path/path.h"
#include "../primitives/circle.h"

/*-----------------------------------------------------------------------------.
|                                                                              |
|                                Draw Commands                                 |
|                                                                              |
|    Draw Commands were some of the most overlooked features of PebbleOS.      |
|    Neographics will continue to support them and even expand on them:        |
|    For example, you may have noticed that Draw Commands originally didn't    |
|    circles centered on subpixels, and that having multiple points in a       |
|    circle would lead to weird behavior in lieu of drawing multiple circles   |
|    with different centers.                                                   |
|                                                                              |
|    To facilitate these changes, neographics defines the Draw Command file    |
|    format version 2. Note that previously invalid files may now count as     |
|    valid. Neographics should be backwards-compatible with all valid files    |
|    from Draw Command version 1.                                              |
|                                                                              |
|    The `flags` byte (offset 1 in commands) now also lets users specify       |
|    specific fallback colors for b/w screens.                                 |
|                                                                              |
`-----------------------------------------------------------------------------*/

/*! \addtogroup draw_command Draw Command
 *  Tiny, modifiable, animatable vector graphics.
 *  @{
 */

#define __N_DRAW_COMMAND_MAXIMUM_SUPPORTED_VERSION 2

/*!
 * The draw command type attribute of a draw command describes the command's
 * type. The `precise` versions allow for subpixel alignment.
 */
typedef enum n_GDrawCommandType {
    n_GDrawCommandTypeInvalid = 0,
    n_GDrawCommandTypePath = 1,
    n_GDrawCommandTypeCircle = 2,
    n_GDrawCommandTypePrecisePath = 3,
    n_GDrawCommandTypePreciseCircle = 4,
} n_GDrawCommandType;

/*!
 * The draw command black/white color allows for specific colors being used to
 * execute the draw command on black/white hardware.
 *
 * In effect, this is useful for commands that should be rendered with different
 * colors based on the display type without having to spend time changing it at
 * runtime or using separate, platform-specific resources.
 */
typedef enum n_GDrawCommandBWColor {
    n_GDrawCommandBWColorClear = 0,
    n_GDrawCommandBWColorBlack = 1,
    n_GDrawCommandBWColorGray  = 2,
    n_GDrawCommandBWColorWhite = 3,
} n_GDrawCommandBWColor;

/*!
 * The draw command flags contain the "hidden" attribute as per the original
 * definition, but additionally allow for specific BW colors
 * (see #n_GDrawCommandBWColor)
 */
typedef struct n_GDrawCommandFlags {
    bool hidden:1;
    bool use_bw_color:1;
    uint8_t bw_stroke:2;
    uint8_t bw_fill:2;
} n_GDrawCommandFlags;

/*!
 * The contents of a n_GDrawCommand.
 */
typedef struct n_GDrawCommand {
    n_GDrawCommandType type;
    n_GDrawCommandFlags flags;
    n_GColor stroke_color;
    uint8_t stroke_width;
    n_GColor fill_color;
    union {
        struct {
            bool path_open:1;
        } path_flags;
        uint16_t circle_radius;
    };
    uint16_t num_points;
    n_GPoint points[];
} __attribute((__packed__)) n_GDrawCommand;

/*!
 * A list of n_GDrawCommand(s)
 */
typedef struct n_GDrawCommandList {
    uint16_t num_commands;
    n_GDrawCommand commands[];
} __attribute((__packed__)) n_GDrawCommandList;

/*!
 * Wrapper around a n_GDrawCommandList which additionally contains viewbox and
 * version information. Prefixed with `PDCI` as a magic word in storage.
 */
typedef struct n_GDrawCommandImage {
    uint64_t reserved_0;
    uint8_t version;
    uint8_t reserved_1;
    n_GSize view_box;
    n_GDrawCommandList command_list[];
} __attribute((__packed__)) n_GDrawCommandImage;

/*!
 * Wrapper around a n_GDrawCommandList which additionally contains frame
 * duration. Multiple of these comprise a n_GDrawCommandSequence.
 */
typedef struct n_GDrawCommandFrame {
    uint16_t duration;
    n_GDrawCommandList command_list[];
} __attribute((__packed__)) n_GDrawCommandFrame;

/*!
 * An animation comprised of n_GDrawCommandFrame(s). Additionally contains play
 * count (zero meaning infinity) and a view box. Prefixed with `PDCS` as a magic
 * word in storage.
 */
typedef struct n_GDrawCommandSequence {
    uint64_t reserved_0;
    uint8_t version;
    uint8_t reserved_1;
    n_GSize view_box;
    uint16_t play_count;
    uint16_t num_frames;
    n_GDrawCommandFrame frames[];
} __attribute((__packed__)) n_GDrawCommandSequence;

typedef bool (n_GDrawCommandListIteratorCb)(n_GDrawCommand * command, uint32_t index, void * context);

/*!
 * Gets the n_GDrawCommandType of a given n_GDrawCommand.
 */
n_GDrawCommandType n_gdraw_command_get_type(n_GDrawCommand * command);

/*!
 * Gets the amount of n_GPoint(s) of a given n_GDrawCommand.
 */
uint16_t n_gdraw_command_get_num_points(n_GDrawCommand * command);

/*!
 * Gets the stroke n_GColor of a given n_GDrawCommand.
 */
n_GColor n_gdraw_command_get_stroke_color(n_GDrawCommand * command);
/*!
 * Sets the stroke n_GColor of a given n_GDrawCommand.
 */
void     n_gdraw_command_set_stroke_color(n_GDrawCommand * command, n_GColor stroke_color);

/*!
 * Gets the stroke width of a given n_GDrawCommand.
 */
uint8_t  n_gdraw_command_get_stroke_width(n_GDrawCommand * command);
/*!
 * Sets the stroke width of a given n_GDrawCommand.
 */
void     n_gdraw_command_set_stroke_width(n_GDrawCommand * command, uint8_t stroke_width);

/*!
 * Gets the fill n_GColor of a given n_GDrawCommand.
 */
n_GColor n_gdraw_command_get_fill_color(n_GDrawCommand * command);
/*!
 * Sets the fill n_GColor of a given n_GDrawCommand.
 */
void     n_gdraw_command_set_fill_color(n_GDrawCommand * command, n_GColor fill_color);

/*!
 * Gets the n_GPoint at a given index of a given n_GDrawCommand.
 */
n_GPoint n_gdraw_command_get_point(n_GDrawCommand * command, uint16_t index);
/*!
 * Sets the n_GPoint at a given index of a given n_GDrawCommand.
 */
void     n_gdraw_command_set_point(n_GDrawCommand * command, uint16_t index, n_GPoint point);

/*!
 * Gets the radius of a given n_GDrawCommand (assuming its n_GDrawCommandType is n_GDrawCommandTypeCircle or n_GDrawCommandTypePreciseCircle).
 */
uint16_t n_gdraw_command_get_radius(n_GDrawCommand * command);
/*!
 * Sets the radius of a given n_GDrawCommand (assuming its n_GDrawCommandType is n_GDrawCommandTypeCircle or n_GDrawCommandTypePreciseCircle).
 */
void     n_gdraw_command_set_radius(n_GDrawCommand * command, uint16_t radius);

/*!
 * Gets whether the given n_GDrawCommand is open at the ends (assuming its n_GDrawCommandType is n_GDrawCommandTypePath or n_GDrawCommandTypePrecisePath).
 */
bool     n_gdraw_command_get_path_open(n_GDrawCommand * command);
/*!
 * Sets whether the given n_GDrawCommand is open at the ends (assuming its n_GDrawCommandType is n_GDrawCommandTypePath or n_GDrawCommandTypePrecisePath).
 */
void     n_gdraw_command_set_path_open(n_GDrawCommand * command, bool path_open);

/*!
 * Gets the visibility of a given n_GDrawCommand.
 */
bool     n_gdraw_command_get_hidden(n_GDrawCommand * command);
/*!
 * Sets the visibility of a given n_GDrawCommand.
 */
void     n_gdraw_command_set_hidden(n_GDrawCommand * command, bool hidden);

/* draw: defined for image / frame / sequence */

// NB these all take offsets. in the builtins, only image and frame drawing do.
/*!
 * Draws a n_GDrawCommand at a given offset.
 */
void     n_gdraw_command_draw(n_GContext * ctx, n_GDrawCommand * command, n_GPoint offset);
/*!
 * Draws a n_GDrawCommandImage at a given offset.
 */
void     n_gdraw_command_image_draw(n_GContext * ctx, n_GDrawCommandImage * image, n_GPoint offset);
/*!
 * Draws a n_GDrawCommandFrame at a given offset.
 */
void     n_gdraw_command_frame_draw(n_GContext * ctx, n_GDrawCommandSequence * sequence, n_GDrawCommandFrame * frame, n_GPoint offset);
/*!
 * Draws a n_GDrawCommandList at a given offset.
 */
void     n_gdraw_command_list_draw(n_GContext * ctx, n_GDrawCommandList * list, n_GPoint offset);

/*!
 * Gets the n_GDrawCommandList of a given n_GDrawCommandImage.
 */
n_GDrawCommandList * n_gdraw_command_image_get_command_list(n_GDrawCommandImage * image);
/*!
 * Gets the n_GDrawCommandList of a given n_GDrawCommandFrame.
 */
n_GDrawCommandList * n_gdraw_command_frame_get_command_list(n_GDrawCommandFrame * frame);

/*!
 * Gets the amount of n_GDrawCommand(s) in a given n_GDrawCommandList.
 */
uint32_t n_gdraw_command_list_get_num_commands(n_GDrawCommandList * list);
/*!
 * Get the n_GDrawCommand at a given index in a given n_GDrawCommandList.
 */
n_GDrawCommand * n_gdraw_command_list_get_command(n_GDrawCommandList * list, uint32_t index);

/* miscellaneous list-only */

/*!
 * Iterates over n_GDrawCommand(s) in a n_GDrawCommandList.
 */
void    n_gdraw_command_list_iterate(n_GDrawCommandList * list, n_GDrawCommandListIteratorCb cb, void * cb_context);

/* miscellaneous frame-only */

/*!
 * Gets the duration of a n_GDrawCommandFrame.
 */
uint16_t n_gdraw_command_frame_get_duration(n_GDrawCommandFrame * frame);
/*!
 * Sets the duration of a n_GDrawCommandFrame.
 */
void     n_gdraw_command_frame_set_duration(n_GDrawCommandFrame * frame, uint16_t duration);

/* miscellaneous sequence-only */

/*!
 * Gets the frame of a given n_GDrawCommandSequence based on a given elapsed time.
 */
n_GDrawCommandFrame * n_gdraw_command_sequence_get_frame_by_elapsed(n_GDrawCommandSequence * sequence, uint32_t ms);
/*!
* Gets the frame of a given n_GDrawCommandSequence based on its index.
*/
n_GDrawCommandFrame * n_gdraw_command_sequence_get_frame_by_index(n_GDrawCommandSequence * sequence, uint32_t index);
/*!
* Gets the play count of a given n_GDrawCommandSequence. (0: infinity)
*/
uint32_t n_gdraw_command_sequence_get_play_count(n_GDrawCommandSequence * sequence);
/*!
* Sets the play count of a given n_GDrawCommandSequence. (0: infinity)
*/
void     n_gdraw_command_sequence_set_play_count(n_GDrawCommandSequence * sequence, uint16_t play_count);

/*!
 * Gets the total duration of a n_GDrawCommandSequence.
 */
uint32_t n_gdraw_command_sequence_get_total_duration(n_GDrawCommandSequence * sequence);

/*!
* Gets amount of frames in a n_GDrawCommandSequence.
*/
uint16_t n_gdraw_command_sequence_get_num_frames(n_GDrawCommandSequence * sequence);

/* get / set bounds: defined for image and sequence */

/*!
 * Gets the n_GSize demarking the outside of a n_GDrawCommandImage.
 * \note When being drawn, this doesn't actually do anything: The image contents
 *       aren't cropped to this bounds size.
 */
n_GSize  n_gdraw_command_image_get_bounds_size(n_GDrawCommandImage * image);
/*!
 * Sets the n_GSize demarking the outside of a n_GDrawCommandImage.
 * \note When being drawn, this doesn't actually do anything: The image contents
 *       aren't cropped to this bounds size.
 */
void     n_gdraw_command_image_set_bounds_size(n_GDrawCommandImage * image, n_GSize size);

/*!
 * Gets the n_GSize demarking the outside of a n_GDrawCommandSequence.
 * \note When being drawn, this doesn't actually do anything: The image contents
 *       aren't cropped to this bounds size.
 */
n_GSize  n_gdraw_command_sequence_get_bounds_size(n_GDrawCommandSequence * sequence);
/*!
 * Sets the n_GSize demarking the outside of a n_GDrawCommandSequence.
 * \note When being drawn, this doesn't actually do anything: The image contents
 *       aren't cropped to this bounds size.
 */
void     n_gdraw_command_sequence_set_bounds_size(n_GDrawCommandSequence * sequence, n_GSize size);

/* create with resource / clone / destroy */

/*!
 * Loads a n_GDrawCommandImage resource.
 */
n_GDrawCommandImage * n_gdraw_command_image_create_with_resource(uint32_t resource_id);
/*!
 * Duplicates a n_GDrawCommandImage.
 */
n_GDrawCommandImage * n_gdraw_command_image_clone(n_GDrawCommandImage * image);
/*!
 * Frees a n_GDrawCommandImage.
 */
void n_gdraw_command_image_destroy(n_GDrawCommandImage * image);

/*!
 * Loads a n_GDrawCommandSequence resource.
 */
n_GDrawCommandSequence * n_gdraw_command_sequence_create_with_resource(uint32_t resource_id);
/*!
 * Duplicates a n_GDrawCommandSequence.
 */
n_GDrawCommandSequence * n_gdraw_command_sequence_clone(n_GDrawCommandSequence * image);
/*!
 * Frees a n_GDrawCommandSequence.
 */
void n_gdraw_command_sequence_destroy(n_GDrawCommandSequence * sequence);

/*! @}
 */
