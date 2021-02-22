/*
    Copyright (C) 2011 Collabora Ltd. <info@collabora.co.uk>
      @author George Kiagiadakis <george.kiagiadakis@collabora.co.uk>

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
#include "applicationsource.h"
#include "../elementfactory.h"
#include <gst/app/gstappsrc.h>

namespace QGst {
namespace Utils {

#ifndef DOXYGEN_RUN

struct QTGSTREAMERUTILS_NO_EXPORT ApplicationSource::Priv
{
public:
    ElementPtr m_appsrc;

    void lazyConstruct(ApplicationSource *self);
    void setCallbacks(ApplicationSource *self);

    inline GstAppSrc *appSrc()
    {
        return reinterpret_cast<GstAppSrc*>(static_cast<GstElement*>(m_appsrc));
    }

private:
    static void need_data(GstAppSrc *src, guint length, gpointer user_data);
    static void enough_data(GstAppSrc *src, gpointer user_data);
    static gboolean seek_data(GstAppSrc *src, guint64 offset, gpointer user_data);

    static void need_data_noop(GstAppSrc*, guint, gpointer) {}
    static void enough_data_noop(GstAppSrc*, gpointer) {}
    static gboolean seek_data_noop(GstAppSrc*, guint64, gpointer) { return FALSE; }
};

void ApplicationSource::Priv::lazyConstruct(ApplicationSource *self)
{
    if (!m_appsrc) {
        m_appsrc = QGst::ElementFactory::make("appsrc");
        if (!m_appsrc) {
            qWarning() << "Failed to construct appsrc";
        }
        setCallbacks(self);
    }
}

void ApplicationSource::Priv::setCallbacks(ApplicationSource *self)
{
    if (m_appsrc) {
        if (self) {
            static GstAppSrcCallbacks callbacks = { &need_data, &enough_data, &seek_data, NULL };
            gst_app_src_set_callbacks(appSrc(), &callbacks, self, NULL);
        } else {
            static GstAppSrcCallbacks callbacks = { &need_data_noop, &enough_data_noop, &seek_data_noop, NULL };
            gst_app_src_set_callbacks(appSrc(), &callbacks, NULL, NULL);
        }
    }
}

void ApplicationSource::Priv::need_data(GstAppSrc *src, guint length, gpointer user_data)
{
    Q_UNUSED(src);
    static_cast<ApplicationSource*>(user_data)->needData(length);
}

void ApplicationSource::Priv::enough_data(GstAppSrc *src, gpointer user_data)
{
    Q_UNUSED(src);
    static_cast<ApplicationSource*>(user_data)->enoughData();
}

gboolean ApplicationSource::Priv::seek_data(GstAppSrc *src, guint64 offset, gpointer user_data)
{
    Q_UNUSED(src);
    return static_cast<ApplicationSource*>(user_data)->seekData(offset) ? TRUE : FALSE;
}

#endif //DOXYGEN_RUN

ApplicationSource::ApplicationSource()
    : d(new Priv)
{
}

ApplicationSource::~ApplicationSource()
{
    d->setCallbacks(NULL); //remove the callbacks from the source
    delete d;
}

ElementPtr ApplicationSource::element() const
{
    d->lazyConstruct(const_cast<ApplicationSource*>(this));
    return d->m_appsrc;
}

void ApplicationSource::setElement(const ElementPtr & appsrc)
{
    Q_ASSERT(QGlib::Type::fromInstance(appsrc).isA(GST_TYPE_APP_SRC));
    d->setCallbacks(NULL); //remove the callbacks from the previous source
    d->m_appsrc = appsrc;
    d->setCallbacks(this);
}

CapsPtr ApplicationSource::caps() const
{
    CapsPtr c;
    if (d->appSrc()) {
        c = CapsPtr::wrap(gst_app_src_get_caps(d->appSrc()), false);
    }
    return c;
}

void ApplicationSource::setCaps(const CapsPtr & caps)
{
    d->lazyConstruct(this);
    if (d->appSrc()) {
        gst_app_src_set_caps(d->appSrc(), caps);
    }
}

quint64 ApplicationSource::minLatency() const
{
    guint64 ret = -1;
    if (d->appSrc()) {
        gst_app_src_get_latency(d->appSrc(), &ret, NULL);
    }
    return ret;
}

quint64 ApplicationSource::maxLatency() const
{
    guint64 ret = -1;
    if (d->appSrc()) {
        gst_app_src_get_latency(d->appSrc(), NULL, &ret);
    }
    return ret;
}

void ApplicationSource::setLatency(quint64 min, quint64 max)
{
    d->lazyConstruct(this);
    if (d->appSrc()) {
        gst_app_src_set_latency(d->appSrc(), min, max);
    }
}

qint64 ApplicationSource::size() const
{
    return d->appSrc() ? gst_app_src_get_size(d->appSrc()) : -1;
}

void ApplicationSource::setSize(qint64 size)
{
    d->lazyConstruct(this);
    if (d->appSrc()) {
        gst_app_src_set_size(d->appSrc(), size);
    }
}

AppStreamType ApplicationSource::streamType() const
{
    return d->appSrc() ? static_cast<AppStreamType>(gst_app_src_get_stream_type(d->appSrc()))
                       : AppStreamTypeStream;
}

void ApplicationSource::setStreamType(AppStreamType type)
{
    d->lazyConstruct(this);
    if (d->appSrc()) {
        gst_app_src_set_stream_type(d->appSrc(), static_cast<GstAppStreamType>(type));
    }
}

quint64 ApplicationSource::maxBytes() const
{
    return d->appSrc() ? gst_app_src_get_max_bytes(d->appSrc()) : 0;
}

void ApplicationSource::setMaxBytes(quint64 max)
{
    d->lazyConstruct(this);
    if (d->appSrc()) {
        gst_app_src_set_max_bytes(d->appSrc(), max);
    }
}

bool ApplicationSource::blockEnabled() const
{
    return d->m_appsrc ? d->m_appsrc->property("block").toBool() : false;
}

void ApplicationSource::enableBlock(bool enable)
{
    d->lazyConstruct(this);
    if (d->m_appsrc) {
        d->m_appsrc->setProperty("block", enable);
    }
}

bool ApplicationSource::isLive() const
{
    return d->m_appsrc ? d->m_appsrc->property("is-live").toBool() : false;
}

void ApplicationSource::setLive(bool islive)
{
    d->lazyConstruct(this);
    if (d->m_appsrc) {
        d->m_appsrc->setProperty("is-live", islive);
    }
}

uint ApplicationSource::minPercent() const
{
    return d->m_appsrc ? d->m_appsrc->property("min-percent").toUInt() : 0;
}

void ApplicationSource::setMinPercent(uint min)
{
    d->lazyConstruct(this);
    if (d->m_appsrc) {
        d->m_appsrc->setProperty("min-percent", min);
    }
}

Format ApplicationSource::format() const
{
    return d->m_appsrc ? d->m_appsrc->property("format").get<Format>() : FormatBytes;
}

void ApplicationSource::setFormat(Format f)
{
    d->lazyConstruct(this);
    if (d->m_appsrc) {
        d->m_appsrc->setProperty("format", f);
    }
}

FlowReturn ApplicationSource::pushBuffer(const BufferPtr & buffer)
{
    if (d->appSrc()) {
        return static_cast<FlowReturn>(gst_app_src_push_buffer(d->appSrc(), gst_buffer_ref(buffer)));
    } else {
        return FlowFlushing;
    }
}

FlowReturn ApplicationSource::endOfStream()
{
    if (d->appSrc()) {
        return static_cast<FlowReturn>(gst_app_src_end_of_stream(d->appSrc()));
    } else {
        return FlowFlushing;
    }
}

void ApplicationSource::needData(uint length)
{
    Q_UNUSED(length);
}

void ApplicationSource::enoughData()
{
}

bool ApplicationSource::seekData(quint64 offset)
{
    Q_UNUSED(offset);
    return false;
}

} //namespace Utils
} //namespace QGst
