/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>

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
#ifndef QGST_BUS_H
#define QGST_BUS_H

#include "object.h"
#include "clocktime.h"

namespace QGst {

/*! \headerfile bus.h <QGst/Bus>
 * \brief Wrapper class for GstBus
 *
 * The Bus is an object responsible for delivering Message packets in a first-in
 * first-out way from the streaming threads to the application.
 *
 * Since the application typically only wants to deal with delivery of these messages
 * from one thread, the Bus will marshall the messages between different threads.
 * This is important since the actual streaming of media is done in another thread
 * than the application.
 *
 * There are three ways to get messages from a Bus:
 * \li Poll manually with the peek() and pop() methods.
 * \li Enable the emission of the "sync-message" signal using enableSyncMessageEmission()
 * and connect to this signal. The slot connected to this signal will be called
 * synchronously from the thread that posts the message.
 * \li Add a signal "watch" to the bus. This is an object that will poll the bus from the
 * main event loop and will emit the "message" signal on the main thread whenever a new
 * message is available. Note that the watch will pop messages from the bus, so they
 * won't be available for manual polling.
 *
 * \note In this library, the bus watch is implemented using Qt's mechanisms and is
 * not a wrapper for the GStreamer bus watch mechanism. This allows you to use a watch
 * even if you are not running a Glib event loop. However, this requires a running
 * Qt event loop.
 */
class QTGSTREAMER_EXPORT Bus : public Object
{
    QGST_WRAPPER(Bus)
public:
    /*! Creates a new Bus */
    static BusPtr create();


    /*! Posts a \a message to the Bus */
    bool post(const MessagePtr & message);


    /*! \returns whether there are pending messages in the bus' queue */
    bool hasPendingMessages() const;

    /*! Retrieves the first message from the bus' queue, but does not remove it. */
    MessagePtr peek() const;

    /*! Retrieves the first message from the bus' queue and removes it. If a
     * \a timeout is specified, this function will wait for up to \a timeout
     * nanoseconds for a message to get posted to the Bus and will return as
     * soon as there is a new message or the timeout has expired.
     */
    MessagePtr pop(ClockTime timeout = 0);

    /*! \overload
     * This version of pop() will return only messages that match the specified message
     * \a type. All other messages that have been posted before the returned message
     * will be discarded. You may use an OR combination of MessageTypes as the \a type
     * to filter multiple types.
     */
    MessagePtr pop(MessageType type, ClockTime timeout = 0);


    /*! If flushing is enabled, the bus will discard any messages that are currently
     * on the bus and any messages that will be posted in the future.
     */
    void setFlushing(bool flush);


    /*! This adds a signal "watch" object, an object that will poll the bus from the
     * event loop of the thread that called this function first. In every event loop
     * execution, any pending messages will be popped from the bus and the "message"
     * signal of the bus will be emitted.
     *
     * The caller is responsible to cleanup by calling the removeSignalWatch() function
     * when this functionality is no longer needed. When the bus is destroyed, the watch
     * is removed automatically.
     *
     * If this function is called multiple times, a reference count will be incremented
     * on the same watch. You cannot have multiple watches on the same bus.
     *
     * \note
     * \li This functionality requires a running Qt event loop.
     * \li This is \em not a wrapper for the gst_bus_add_signal_watch() function. It uses
     * a different implementation based on Qt's event loop instead of the Glib one, so that
     * it is possible to use it even if you are not using a Glib event loop underneath.
     */
    void addSignalWatch();

    /*! Removes a signal "watch" object that was previously added with addSignalWatch().
     * If addSignalWatch() has been called multiple times, this function will decrement the
     * watch'es reference count and will remove it only when the reference count reaches zero.
     */
    void removeSignalWatch();


    /*! Enables the emission of the "sync-message" signal. This signal will be emitted
     * synchronously from inside post() whenever a new message is posted. Be aware
     * that messages are typically posted from a different thread.
     *
     * This function may be called multiple times. To clean up, the caller is responsible
     * for calling disableSyncMessageEmission() as many times as this function is called.
     */
    void enableSyncMessageEmission();

    /*! Disables the emission of the "sync-message" signal on the bus.
     * If enableSyncMessageEmission() has been called multiple times, this function will decrement
     * a reference count instead of disabling the emission, so it has to be called exactly as many
     * times as enableSyncMessageEmission() has been called.
     */
    void disableSyncMessageEmission();
};

} //namespace QGst

QGST_REGISTER_TYPE(QGst::Bus)

#endif //QGST_BUS_H
