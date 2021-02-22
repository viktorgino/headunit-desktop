/*
    Copyright (C) 2015  Pavel Bludov <pbludov@gmail.com>

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
#ifndef QGST_DEVICEMONITOR_H
#define QGST_DEVICEMONITOR_H

#include "bus.h"
#include "caps.h"
#include "device.h"
#include "object.h"

namespace QGst {

/*! \headerfile devicemonitor.h <QGst/DeviceMonitor>
 * \brief Wrapper class for GstDeviceMonitor
 * 
 * A device monitor and prober
 */
class QTGSTREAMER_EXPORT DeviceMonitor : public Object
{
    QGST_WRAPPER(DeviceMonitor)
public:
    static DeviceMonitorPtr create();

    BusPtr bus() const;

    quint32 addFilter(const char* classes, const CapsPtr & caps);
    bool removeFilter(quint32 filterId);

    bool start();
    void stop();
    
    QList<DevicePtr> devices() const;
};

}

QGST_REGISTER_TYPE(QGst::DeviceMonitor)

#endif
