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
#ifndef QGST_UTILS_APPLICATIONSINK_H
#define QGST_UTILS_APPLICATIONSINK_H

#include "global.h"
#include "../element.h"
#include "../sample.h"

namespace QGst {
namespace Utils {

/*! \headerfile applicationsink.h <QGst/Utils/ApplicationSink>
 * \brief Helper class for using a GstAppSink
 *
 * Appsink is a sink plugin that supports many different methods for making the application
 * get a handle on the GStreamer data in a pipeline. Unlike most GStreamer elements, appsink
 * provides external API functions. This class exports those API functions in the bindings
 * and makes it easy to implement a custom sink.
 *
 * The normal way of retrieving sample from appsink is by using the pullSample() and pullPreroll()
 * methods. These methods block until a sample becomes available in the sink or when the sink is
 * shut down or reaches EOS.
 *
 * Appsink will internally use a queue to collect samples from the streaming thread. If the
 * application is not pulling samples fast enough, this queue will consume a lot of memory over
 * time. setMaxBuffers() can be used to limit the queue size. enableDrop() controls whether the
 * streaming thread blocks or if older samples are dropped when the maximum queue size is reached.
 * Note that blocking the streaming thread can negatively affect real-time performance and
 * should be avoided.
 *
 * If a blocking behaviour is not desirable, you can subclass this class and implement the
 * newPreroll(), newSample() and newBufferList() which will be called to notify you when a new
 * sample is available.
 *
 * setCaps() can be used to control the formats that appsink can receive. This property can contain
 * non-fixed caps. The format of the pulled samples can be obtained by getting the sample caps.
 *
 * If one of the pullPreroll() or pullSample() methods return NULL, the appsink is stopped or in
 * the EOS state. You can check for the EOS state with isEos(). The eos() virtual method can also
 * be reimplemented to be informed when the EOS state is reached to avoid polling.
 *
 * The actuall appsink element can be retrieved with element() and set with setElement(). It is
 * not necessary to set an appsink, as this class will create one as soon as it is needed.
 *
 * \note It is not necessary to use this class in order to use GstAppSink. GstAppSink
 * also provides signals and properties that fully substitute the functionality of this class.
 *
 * \sa ApplicationSource
 */
class QTGSTREAMERUTILS_EXPORT ApplicationSink
{
public:
    ApplicationSink();
    virtual ~ApplicationSink();

    /*! \returns the appsink element */
    ElementPtr element() const;

    /*! Sets the appsink element that will be used by this class */
    void setElement(const ElementPtr & appsink);


    /*! \returns the caps that the appsink is configured to accept */
    CapsPtr caps() const;

    /*! Sets the caps that appsink will accept */
    void setCaps(const CapsPtr & caps);


    /*! \returns true if the appsink has received an End Of Stream event
     * or if it is not in PAUSED or PLAYING state. */
    bool isEos() const;


    /*! \returns the maximum number of buffers that the sink is
     * configured to hold in its internal queue. 0 means unlimited. */
    uint maxBuffers() const;

    /*! Sets the maximum number of buffers that the sink will hold in its queue.
     * After this amount of buffers are queued in appsink, any more buffers will
     * block upstream elements until a buffer is pulled from appsink. */
    void setMaxBuffers(uint maxbuffers);


    /*! \returns whether the sink is configured to drop old buffers
     * when the maximum amount of queued buffers is reached. */
    bool dropEnabled() const;

    /*! Enables dropping old buffers when the maximum amount of queued buffers is reached. */
    void enableDrop(bool enable);

    /*! Get the last preroll sample in appsink. This was the sample that caused the appsink
     * to preroll in the PAUSED state. This sample can be pulled many times and remains
     * available to the application even after EOS.
     *
     * This function is typically used when dealing with a pipeline in the PAUSED state.
     * Calling this function after doing a seek will give the sample right after the seek position.
     *
     * Note that the preroll sample will also be returned as the first sample when calling
     * pullSample().
     *
     * If an EOS event was received before any samples, this function returns a null SamplePtr.
     * Use isEos() to check for the EOS condition.
     *
     * This function blocks until a preroll sample or EOS is received or the appsink element
     * is set to the READY/NULL state.
     */
    SamplePtr pullPreroll();

    /*! This function blocks until a sample or EOS becomes available or the appsink
     * element is set to the READY/NULL state.
     *
     * This function will only return samples when the appsink is in the PLAYING state.
     * All rendered samples will be put in a queue so that the application can pull samples
     * at its own rate. Note that when the application does not pull samples fast enough, the
     * queued samples could consume a lot of memory, especially when dealing with raw video frames.
     *
     * If an EOS event was received before any samples, this function returns a null SamplePtr.
     * Use isEos() to check for the EOS condition.
     */
    SamplePtr pullSample();

    /*! This function blocks until a sample list or EOS becomes available or the appsink
     * element is set to the READY/NULL state.
     *
     * This function will only return sample lists when the appsink is in the PLAYING state.
     * All rendered sample lists will be put in a queue so that the application can pull sample
     * lists at its own rate. Note that when the application does not pull sample lists fast
     * enough, the queued sample lists could consume a lot of memory, especially when dealing
     * with raw video frames.
     *
     * If an EOS event was received before any sample lists, this function returns a null
     * SampleListPtr. Use isEos() to check for the EOS condition.
     */
    BufferListPtr pullBufferList();

protected:
    /*! Called when the end-of-stream has been reached.
     * \note This function is called from the steaming thread. */
    virtual void eos();

    /*! Called when a new preroll sample is available. The new preroll sample can be
     * retrieved with pullPreroll() either from this function or from any other thread.
     * \note This function is called from the steaming thread. */
    virtual FlowReturn newPreroll();

    /*! Called when a new sample is available. The new sample can be retrieved
     * with pullSample() either from this function or from any other thread.
     * \note This function is called from the steaming thread. */
    virtual FlowReturn newSample();

private:
    /* vtable padding */
    virtual void reservedVirtual1() {}
    virtual void reservedVirtual2() {}
    virtual void reservedVirtual3() {}

private:
    struct Priv;
    friend struct Priv;
    Priv *const d;
    Q_DISABLE_COPY(ApplicationSink)
};

} //namespace Utils
} //namespace QGst

#endif // QGST_APPLICATIONSINK_H
