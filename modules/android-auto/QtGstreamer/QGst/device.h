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
#ifndef QGST_DEVICE_H
#define QGST_DEVICE_H

#include "object.h"

namespace QGst {

    /*! \headerfile device.h <QGst/Device>
     * \brief Wrapper class for GstDevice
     *
     * Device are objects representing a hardware device, they contain relevant
     * metadata about the device, such as its class and the Caps representing
     * the media types it can produce or handle.
     *
     */
class QTGSTREAMER_EXPORT Device : public Object
{
    QGST_WRAPPER(Device)
public:
    QString displayName() const;
    CapsPtr caps() const;

    ElementPtr createElement(const char* name) const;
    bool reconfigureElement(const ElementPtr & element) const;

    QString deviceClass() const;
    bool hasClasses(const char *classes) const;
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Device)

#endif
