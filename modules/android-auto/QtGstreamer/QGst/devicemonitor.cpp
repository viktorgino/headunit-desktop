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
#include "global.h"
#include "devicemonitor.h"
#include <gst/gst.h>

namespace QGst {

//static
DeviceMonitorPtr DeviceMonitor::create()
{
    GstDeviceMonitor *m = gst_device_monitor_new();
    if (m) {
        gst_object_ref_sink(m);
    }
    return DeviceMonitorPtr::wrap(m, false);
}

BusPtr DeviceMonitor::bus() const
{
    return BusPtr::wrap(gst_device_monitor_get_bus(object<GstDeviceMonitor>()), false);
}

quint32 DeviceMonitor::addFilter(const char* classes, const CapsPtr & caps)
{
    return gst_device_monitor_add_filter(object<GstDeviceMonitor>(), classes, caps);
}

bool DeviceMonitor::removeFilter(quint32 filterId)
{
    return gst_device_monitor_remove_filter(object<GstDeviceMonitor>(), filterId);
}

bool DeviceMonitor::start()
{
    return gst_device_monitor_start(object<GstDeviceMonitor>());
}

void DeviceMonitor::stop()
{
    gst_device_monitor_stop(object<GstDeviceMonitor>());
}

QList<DevicePtr> DeviceMonitor::devices() const
{
    QList<DevicePtr> output;
    GList *input = gst_device_monitor_get_devices(object<GstDeviceMonitor>());

    while (input) {
        output += DevicePtr::wrap(GST_DEVICE(input->data), false);
        input = g_list_delete_link(input, input);
    }

    return output;
}

}
