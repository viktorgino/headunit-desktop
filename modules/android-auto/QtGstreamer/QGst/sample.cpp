/*
    Copyright (C) 2013  Diane Trout
      @author Diane Trout <diane@ghic.org>

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
#include "buffer.h"
#include "caps.h"
#include "sample.h"
#include "structure.h"
#include "segment.h"
#include <QtCore/QDebug>
#include <gst/gst.h>

namespace QGst {

SamplePtr Sample::create(const BufferPtr & buffer, const CapsPtr & caps,
                         const Segment & segment, const Structure & info)
{
    GstStructure *cinfo = NULL;
    if (info.isValid())
        cinfo = gst_structure_copy(info);

    return SamplePtr::wrap(gst_sample_new(buffer, caps, segment, cinfo), false);
}

BufferPtr Sample::buffer() const
{
    return BufferPtr::wrap(gst_sample_get_buffer(object<GstSample>()));
}

CapsPtr Sample::caps() const
{
    return CapsPtr::wrap(gst_sample_get_caps(object<GstSample>()));
}

Structure Sample::info() const
{
    return Structure(gst_sample_get_info(object<GstSample>()));
}

Segment Sample::segment() const
{
    return Segment(gst_sample_get_segment(object<GstSample>()));
}

} //namespace QGst
