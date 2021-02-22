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
#include "applicationsink.h"
#include "../elementfactory.h"
#include <gst/app/gstappsink.h>

namespace QGst {
namespace Utils {

#ifndef DOXYGEN_RUN

struct QTGSTREAMERUTILS_NO_EXPORT ApplicationSink::Priv
{
public:
    ElementPtr m_appsink;

    void lazyConstruct(ApplicationSink *self);
    void setCallbacks(ApplicationSink *self);

    inline GstAppSink *appSink()
    {
        return reinterpret_cast<GstAppSink*>(static_cast<GstElement*>(m_appsink));
    }

private:
    static void eos(GstAppSink *sink, gpointer user_data);
    static GstFlowReturn new_preroll(GstAppSink *sink, gpointer user_data);
    static GstFlowReturn new_sample(GstAppSink *sink, gpointer user_data);

    static void eos_noop(GstAppSink*, gpointer) {}
    static GstFlowReturn new_preroll_noop(GstAppSink*, gpointer) { return GST_FLOW_OK; }
    static GstFlowReturn new_sample_noop(GstAppSink*, gpointer) { return GST_FLOW_OK; }
};

void ApplicationSink::Priv::lazyConstruct(ApplicationSink *self)
{
    if (!m_appsink) {
        m_appsink = QGst::ElementFactory::make("appsink");
        if (!m_appsink) {
            qWarning() << "Failed to construct appsink";
        }
        setCallbacks(self);
    }
}

void ApplicationSink::Priv::setCallbacks(ApplicationSink *self)
{
    if (m_appsink) {
        if (self) {
            static GstAppSinkCallbacks callbacks = { &eos, &new_preroll, &new_sample };
            gst_app_sink_set_callbacks(appSink(), &callbacks, self, NULL);
        } else {
            static GstAppSinkCallbacks callbacks = { &eos_noop, &new_preroll_noop,
                                                     &new_sample_noop };
            gst_app_sink_set_callbacks(appSink(), &callbacks, NULL, NULL);
        }
    }
}

void ApplicationSink::Priv::eos(GstAppSink* sink, gpointer user_data)
{
    Q_UNUSED(sink);
    static_cast<ApplicationSink*>(user_data)->eos();
}

GstFlowReturn ApplicationSink::Priv::new_preroll(GstAppSink* sink, gpointer user_data)
{
    Q_UNUSED(sink);
    return static_cast<GstFlowReturn>(static_cast<ApplicationSink*>(user_data)->newPreroll());
}

GstFlowReturn ApplicationSink::Priv::new_sample(GstAppSink* sink, gpointer user_data)
{
    Q_UNUSED(sink);
    return static_cast<GstFlowReturn>(static_cast<ApplicationSink*>(user_data)->newSample());
}

#endif //DOXYGEN_RUN


ApplicationSink::ApplicationSink()
    : d(new Priv)
{
}

ApplicationSink::~ApplicationSink()
{
    d->setCallbacks(NULL); //remove the callbacks from the sink
    delete d;
}

ElementPtr ApplicationSink::element() const
{
    d->lazyConstruct(const_cast<ApplicationSink*>(this));
    return d->m_appsink;
}

void ApplicationSink::setElement(const ElementPtr & appsink)
{
    Q_ASSERT(QGlib::Type::fromInstance(appsink).isA(GST_TYPE_APP_SINK));
    d->setCallbacks(NULL); //remove the callbacks from the previous sink
    d->m_appsink = appsink;
    d->setCallbacks(this);
}

CapsPtr ApplicationSink::caps() const
{
    CapsPtr caps;
    if (d->appSink()) {
        caps = CapsPtr::wrap(gst_app_sink_get_caps(d->appSink()), false);
    }
    return caps;
}

void ApplicationSink::setCaps(const CapsPtr & caps)
{
    d->lazyConstruct(this);
    if (d->appSink()) {
        gst_app_sink_set_caps(d->appSink(), caps);
    }
}

bool ApplicationSink::isEos() const
{
    return d->appSink() ? gst_app_sink_is_eos(d->appSink()) : true;
}

uint ApplicationSink::maxBuffers() const
{
    return d->appSink() ? gst_app_sink_get_max_buffers(d->appSink()) : 0;
}

void ApplicationSink::setMaxBuffers(uint maxbuffers)
{
    d->lazyConstruct(this);
    if (d->appSink()) {
        gst_app_sink_set_max_buffers(d->appSink(), maxbuffers);
    }
}

bool ApplicationSink::dropEnabled() const
{
    return d->appSink() ? gst_app_sink_get_drop(d->appSink()) : false;
}

void ApplicationSink::enableDrop(bool enable)
{
    d->lazyConstruct(this);
    if (d->appSink()) {
        gst_app_sink_set_drop(d->appSink(), enable);
    }
}

SamplePtr ApplicationSink::pullPreroll()
{
    SamplePtr buf;
    if (d->appSink()) {
        buf = SamplePtr::wrap(gst_app_sink_pull_preroll(d->appSink()), false);
    }
    return buf;
}

SamplePtr ApplicationSink::pullSample()
{
    SamplePtr buf;
    if (d->appSink()) {
        buf = SamplePtr::wrap(gst_app_sink_pull_sample(d->appSink()), false);
    }
    return buf;
}

void ApplicationSink::eos()
{
}

FlowReturn ApplicationSink::newPreroll()
{
    return FlowOk;
}

FlowReturn ApplicationSink::newSample()
{
    return FlowOk;
}

} //namespace Utils
} //namespace QGst
