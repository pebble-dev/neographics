/*\
|*|
|*|   Neographics: a tiny graphics library.
|*|   Copyright (C) 2016 Johannes Neubrand <johannes_n@icloud.com>
|*|
|*|   This program is free software; you can redistribute it and/or
|*|   modify it under the terms of the GNU General Public License
|*|   as published by the Free Software Foundation; either version 2
|*|   of the License, or (at your option) any later version.
|*|
|*|   This program is distributed in the hope that it will be useful,
|*|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|*|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
|*|   GNU General Public License for more details.
|*|
|*|   You should have received a copy of the GNU General Public License
|*|   along with this program; if not, write to the Free Software
|*|   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
|*|
\*/

#pragma once
#include <pebble.h>
#include "../context.h"
#include "../types.h"

#include "../primitives/line.h"

/*! \addtogroup path
 *  Draw an open or closed path
 *  @{
 */

/*!
 * A list of points that makes up an n_GPath.
 */
typedef struct {
    uint32_t num_points;
    n_GPoint * points;
} n_GPathInfo;

/*!
 * A bunch of information used to describe a path. This can be any polygon.
 * An even-odd fill rule is used for all paths.
 */
typedef struct {
    uint32_t num_points;
    n_GPoint * points;
    int32_t angle;
    n_GPoint offset;
    bool open;
} n_GPath;

/*!
 * Creates a n_GPath from some n_GPathInfo.
 */
n_GPath * n_gpath_create(n_GPathInfo * path_info);
/*!
 * Destroys a previously created n_GPath.
 */
void n_gpath_destroy(n_GPath * path);

/*!
 * Draws the outline of a path.
 */
void n_graphics_draw_path(n_GContext * ctx, uint32_t num_points, n_GPoint * points, bool open);
/*!
 * Fills a path using the even-odd fill rule.
 */
void n_graphics_fill_path(n_GContext * ctx, uint32_t num_points, n_GPoint * points);

/*!
 * Draws the outline of a precise path.
 */
void n_graphics_draw_ppath(n_GContext * ctx, uint32_t num_points, n_GPoint * points, bool open);
/*!
 * Fills a precise path using the even-odd fill rule.
 */
void n_graphics_fill_ppath(n_GContext * ctx, uint32_t num_points, n_GPoint * points);

/*!
 * Draws the outline of a n_GPath.
 */
void n_gpath_draw(n_GContext * ctx, n_GPath * path);
/*!
* Fills a n_GPath using the even-odd fill rule.
*/
void n_gpath_fill(n_GContext * ctx, n_GPath * path);

/*!
* Sets the rotation of a n_GPath.
*/
void n_gpath_rotate_to(n_GPath * path, int32_t angle);
/*!
* Sets the offset of a n_GPath.
*/
void n_gpath_move_to(n_GPath * path, n_GPoint offset);

/*!
* Transforms a point array with a given angle and offset.
*/
void n_prv_transform_points(uint32_t num_points, n_GPoint * points_in, n_GPoint * points_out,
                            int16_t angle, n_GPoint offset);

/*! @}
 */
