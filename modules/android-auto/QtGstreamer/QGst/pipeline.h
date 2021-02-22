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
#ifndef QGST_PIPELINE_H
#define QGST_PIPELINE_H

#include "bin.h"

#ifdef Q_CC_MSVC
# pragma warning(push)
# pragma warning(disable:4250) //Pipeline inherits QGst::Object::ref/unref via dominance
#endif

namespace QGst {

/*! \headerfile pipeline.h <QGst/Pipeline>
 * \brief Wrapper class for GstPipeline
 */
class QTGSTREAMER_EXPORT Pipeline : public Bin
{
    QGST_WRAPPER(Pipeline)
public:
    static PipelinePtr create(const char *name = NULL);

    BusPtr bus() const;

    ClockPtr clock() const;
    bool setClock(const ClockPtr & clock);
    void useClock(const ClockPtr & clock);
    void enableAutoClock();
};

}

QGST_REGISTER_TYPE(QGst::Pipeline)

#ifdef Q_CC_MSVC
# pragma warning(pop)
#endif

#endif
