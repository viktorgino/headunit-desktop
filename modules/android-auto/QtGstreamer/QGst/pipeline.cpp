/*
    Copyright (C) 2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#include "pipeline.h"
#include "bus.h"
#include "clock.h"
#include <gst/gst.h>

namespace QGst {

//static
PipelinePtr Pipeline::create(const char *name)
{
    GstElement *p = gst_pipeline_new(name);
    if (p) {
        gst_object_ref_sink(p);
    }
    return PipelinePtr::wrap(GST_PIPELINE(p), false);
}

BusPtr Pipeline::bus() const
{
    return BusPtr::wrap(gst_pipeline_get_bus(object<GstPipeline>()), false);
}

ClockPtr Pipeline::clock() const
{
    return ClockPtr::wrap(gst_pipeline_get_clock(object<GstPipeline>()), false);
}

bool Pipeline::setClock(const ClockPtr & clock)
{
    return gst_pipeline_set_clock(object<GstPipeline>(), clock);
}

void Pipeline::useClock(const ClockPtr & clock)
{
    gst_pipeline_use_clock(object<GstPipeline>(), clock);
}

void Pipeline::enableAutoClock()
{
    gst_pipeline_auto_clock(object<GstPipeline>());
}

}
