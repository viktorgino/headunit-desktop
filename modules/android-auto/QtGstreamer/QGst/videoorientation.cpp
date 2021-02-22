/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

    This library is free software; you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published
    by the Free Software Foundation; either version 2.1 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "videoorientation.h"
#include <gst/video/videoorientation.h>

namespace QGst {

bool VideoOrientation::horizontalFlipEnabled() const
{
    gboolean flipped;
    //Ignoring the gboolean result of the function, can be tested when setting the property
    gst_video_orientation_get_hflip(object<GstVideoOrientation>(), &flipped);
    return flipped;
}

bool VideoOrientation::verticalFlipEnabled() const
{
    gboolean flipped;
    //Ignoring the gboolean result of the function, can be tested when setting the property
    gst_video_orientation_get_vflip(object<GstVideoOrientation>(), &flipped);
    return flipped;
}

int VideoOrientation::horizontalCenter() const
{
    int center = 0;
    //Ignoring the gboolean result of the function, can be tested when setting the property
    gst_video_orientation_get_hcenter(object<GstVideoOrientation>(), &center);
    return center;
}

int VideoOrientation::verticalCenter() const
{
    int center = 0;
    //Ignoring the gboolean result of the function, can be tested when setting the property
    gst_video_orientation_get_vcenter(object<GstVideoOrientation>(), &center);
    return center;
}

bool VideoOrientation::enableHorizontalFlip(bool enabled)
{
    return gst_video_orientation_set_hflip(object<GstVideoOrientation>(), enabled);
}

bool VideoOrientation::enableVerticalFlip(bool enabled)
{
    return gst_video_orientation_set_vflip(object<GstVideoOrientation>(), enabled);
}

bool VideoOrientation::setHorizontalCenter(int center)
{
    return gst_video_orientation_set_hcenter(object<GstVideoOrientation>(), center);
}

bool VideoOrientation::setVerticalCenter(int center)
{
    return gst_video_orientation_set_hcenter(object<GstVideoOrientation>(), center);
}

} //namespace QGst

