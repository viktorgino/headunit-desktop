/*
    Copyright (C) 2013 basysKom GmbH <info@basyskom.com>
      @author Benjamin Federau <benjamin.federau@basyskom.com>

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

#include "../../QGst/Quick/videoitem.h"
#include "../../QGst/Quick/videosurface.h"
#include <QtQml/QQmlExtensionPlugin>

class QtGStreamerPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.freedesktop.gstreamer.QtGStreamerQuick2-1.0"
                      FILE "QtGStreamerQuick2.json")
public:
    virtual void registerTypes(const char *uri);
};

void QtGStreamerPlugin::registerTypes(const char *uri)
{
    // @uri org.freedesktop.gstreamer.QtGStreamerQuick2-1.0
    qmlRegisterType<QGst::Quick::VideoItem>(uri, 1, 0, "VideoItem");
    qmlRegisterUncreatableType<QGst::Quick::VideoSurface>(uri, 1, 0, "VideoSurface",
        QLatin1String("Creating a QGst::Quick::VideoSurface from QML is not supported"));
}

//#include "plugin.moc"
