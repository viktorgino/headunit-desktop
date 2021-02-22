/*
    Copyright (C) 2012 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>

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
#include "videoitem.h"
#include <QtDeclarative/QDeclarativeExtensionPlugin>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
# define Q_PLUGIN_METADATA(x)
#endif

class QtGStreamerPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.freedesktop.gstreamer.Qt5GStreamerQuick1-1.0")
public:
    void registerTypes(const char *uri);
};

void QtGStreamerPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<VideoItem>(uri, 1, 0, "VideoItem");
    qmlRegisterUncreatableType<QGst::Ui::GraphicsVideoSurface>(uri, 1, 0, "GraphicsVideoSurface",
        QLatin1String("Creating a QGst::Ui::GraphicsVideoSurface from QML is not supported"));
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
Q_EXPORT_PLUGIN2(qtgstPlugin, QtGStreamerPlugin)
#endif

#include "plugin.moc"
