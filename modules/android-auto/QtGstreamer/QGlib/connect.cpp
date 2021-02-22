/*
    Copyright (C) 2010 George Kiagiadakis <kiagiadakis.george@gmail.com>
    Copyright (C) 2010 Collabora Ltd.
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
#include "connect.h"
#include <glib-object.h>
#include <QtCore/QHash>
#include <QtCore/QMutex>
#include <boost/multi_index_container.hpp>
#ifndef Q_MOC_RUN  // See: https://bugreports.qt-project.org/browse/QTBUG-22829
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/ordered_index.hpp>
#endif
#include <boost/multi_index/member.hpp>

namespace QGlib {
namespace Private {

//BEGIN ******** Closure internals ********

static void c_marshaller(GClosure *closure, GValue *returnValue, uint paramValuesCount,
                         const GValue *paramValues, void *hint, void *data)
{
    Q_UNUSED(data);

    ClosureDataBase *cdata = static_cast<ClosureDataBase*>(closure->data);

    QList<Value> params;
    //the signal sender is always the first argument. if we are instructed not to pass it
    //as an argument to the slot, begin converting from paramValues[1]
    for(uint i = cdata->passSender ? 0 : 1; i<paramValuesCount; ++i) {
        params.append(Value(&paramValues[i]));
    }

    try {
        Value result(returnValue);
        cdata->marshaller(result, params);

        if (returnValue && G_IS_VALUE(returnValue)) {
            g_value_copy(result, returnValue);
        }
    } catch (const std::exception & e) {
        QString signalName;
        if (hint != NULL) {
            GSignalInvocationHint *ihint = static_cast<GSignalInvocationHint*>(hint);

            GSignalQuery query;
            g_signal_query(ihint->signal_id, &query);
            signalName = QString::fromUtf8(query.signal_name);

            if (ihint->detail != 0) {
                Quark q(ihint->detail);
                signalName.append(QLatin1String("::"));
                signalName.append(q.toString());
            }
        }

        QString instanceName = params.at(0).get<QString>();

        //attempt to determine the cause of the failure
        QString msg;
        try {
            //dynamic_cast will throw an std::bad_cast if it fails
            dynamic_cast<const InvalidTypeException &>(e);
            //cast succeded, e is indeed an InvalidTypeException
            msg = QLatin1String("One or more of the arguments of the signal are of different "
                                "type than the type that the closure expects");
        } catch (...) {
            try {
                dynamic_cast<const InvalidValueException &>(e);
                //cast succeded, e is indeed an InvalidValueException
                //this is most likely to happen because the signal returns void
                //but the closure returns something non-void. check this first.
                if (returnValue == NULL) {
                    msg = QLatin1String("The signal is defined to return void but the "
                                        "closure returns something non-void");
                } else {
                    msg = QLatin1String("One of the arguments of the signal was not a valid GValue. "
                                        "This is most likely a bug in the code that invoked the signal.");
                }
            } catch (...) {
                msg = QString::fromLatin1(e.what());
            }
        }

        qCritical() << "Error during invocation of closure connected to signal"
                    << signalName << "from object" << instanceName << ":" << msg;
    }
}

static void closureDestroyNotify(void *data, GClosure *closure)
{
    Q_UNUSED(data);
    delete static_cast<ClosureDataBase*>(closure->data);
}

static inline GClosure *createCppClosure(ClosureDataBase *closureData)
{
    GClosure *closure = g_closure_new_simple(sizeof(GClosure), closureData);
    g_closure_set_marshal(closure, &c_marshaller);
    g_closure_add_finalize_notifier(closure, NULL, &closureDestroyNotify);
    g_closure_ref(closure);
    g_closure_sink(closure);
    return closure;
}

//END ******** Closure internals ********
//BEGIN ******** QObjectDestroyNotifier ********

Q_GLOBAL_STATIC(QWeakPointer<DestroyNotifierIface>, s_qobjDestroyNotifier)
Q_GLOBAL_STATIC(QMutex, s_qobjDestroyNotifierMutex)

DestroyNotifierIfacePtr QObjectDestroyNotifier::instance()
{
    QMutexLocker l(s_qobjDestroyNotifierMutex());

    DestroyNotifierIfacePtr ptr = s_qobjDestroyNotifier()->toStrongRef();
    if (!ptr) {
        ptr = DestroyNotifierIfacePtr(new QObjectDestroyNotifier);
        *s_qobjDestroyNotifier() = ptr;
    }
    return ptr;
}

bool QObjectDestroyNotifier::connect(void *receiver, QObject *notificationReceiver, const char *slot)
{
    QObject *qreceiver = reinterpret_cast<QObject*>(receiver);
    return QObject::connect(qreceiver, SIGNAL(destroyed(QObject*)),
                            notificationReceiver, slot, Qt::DirectConnection);
}

bool QObjectDestroyNotifier::disconnect(void* receiver, QObject *notificationReceiver)
{
    QObject *qreceiver = reinterpret_cast<QObject*>(receiver);
    return QObject::disconnect(qreceiver, 0, notificationReceiver, 0);
}

//END ******** QObjectDestroyNotifier ********
//BEGIN ******** ConnectionsStore ********

class ConnectionsStore : public QObject
{
    Q_OBJECT
public:
    inline ConnectionsStore() : QObject(), m_handlerIdInRemoval(0) {}

    ulong connect(void *instance, uint signal, Quark detail,
                  void *receiver, const DestroyNotifierIfacePtr & notifier,
                  uint slotHash, ClosureDataBase *closureData, ConnectFlags flags);

    bool disconnect(void *instance, uint signal, Quark detail,
                    void *receiver, uint slotHash, ulong handlerId);

private:
    struct Connection
    {
        inline Connection(uint signal, Quark detail, void *receiver,
                          uint slotHash, ulong handlerId)
            : signal(signal),
              detail(detail),
              receiver(receiver),
              slotHash(slotHash),
              handlerId(handlerId)
        {
        }

        uint signal;
        Quark detail;
        void *receiver;
        uint slotHash;
        ulong handlerId;
    };

    bool lookupAndExec(void *instance, uint signal, Quark detail, void *receiver, uint slotHash,
                       ulong handlerId, void (ConnectionsStore::*func)(void*, const Connection &));

    void disconnectHandler(void *instance, const Connection & c);
    void disconnectAndDestroyRcvrWatch(void *instance, const Connection & c);

    void setupClosureWatch(void *instance, ulong handlerId, GClosure *closure);
    void onClosureDestroyedAction(void *instance, ulong handlerId);
    static void onClosureDestroyed(void *data, GClosure *closure);

    void setupReceiverWatch(void *instance, void *receiver, const DestroyNotifierIfacePtr & notifier);
    void destroyReceiverWatch(void *instance, const Connection & c);

private Q_SLOTS:
    void onReceiverDestroyed(void *receiver);
    void onReceiverDestroyed(QObject *receiver);

private:
    //tags
    struct sequential {};
    struct by_handlerId {};
    struct by_signal {};
    struct by_receiver {};

    typedef boost::multi_index_container<
        Connection,
        boost::multi_index::indexed_by<
            boost::multi_index::sequenced<
                boost::multi_index::tag<sequential>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_signal>,
                boost::multi_index::member<Connection, uint, &Connection::signal>
            >,
            boost::multi_index::ordered_non_unique<
                boost::multi_index::tag<by_receiver>,
                boost::multi_index::member<Connection, void*, &Connection::receiver>
            >,
            boost::multi_index::ordered_unique<
                boost::multi_index::tag<by_handlerId>,
                boost::multi_index::member<Connection, ulong, &Connection::handlerId>
            >
        >
    > ConnectionsContainer;

    typedef ConnectionsContainer::index<sequential>::type::iterator SequentialIterator;
    typedef ConnectionsContainer::index<by_signal>::type::iterator BySignalIterator;
    typedef ConnectionsContainer::index<by_receiver>::type::iterator ByReceiverIterator;
    typedef ConnectionsContainer::index<by_handlerId>::type::iterator ByHandlerIterator;
    typedef std::pair<BySignalIterator, BySignalIterator> BySignalIterators;
    typedef std::pair<ByReceiverIterator, ByReceiverIterator> ByReceiverIterators;

    struct ReceiverData
    {
        DestroyNotifierIfacePtr notifier;
        QHash<void*, int> senders; //<sender, refcount>
    };

    QMutex m_mutex;
    QHash<void*, ConnectionsContainer> m_connections; // <sender, connections>
    QHash<void*, ReceiverData> m_receivers; // <receiver, data>

    QMutex m_handlerIdInRemovalMutex;
    ulong m_handlerIdInRemoval;
};

Q_GLOBAL_STATIC(ConnectionsStore, s_connectionsStore)

ulong ConnectionsStore::connect(void *instance, uint signal, Quark detail,
                                void *receiver, const DestroyNotifierIfacePtr & notifier,
                                uint slotHash, ClosureDataBase *closureData, ConnectFlags flags)
{
    QMutexLocker l(&m_mutex);
    GClosure *closure = createCppClosure(closureData);

    ulong handlerId = g_signal_connect_closure_by_id(instance, signal, detail, closure,
                                                     (flags & ConnectAfter) ? TRUE : FALSE);

    if (handlerId) {
        m_connections[instance].get<sequential>().push_back(
            Connection(signal, detail, receiver, slotHash, handlerId)
        );

        setupClosureWatch(instance, handlerId, closure);
        setupReceiverWatch(instance, receiver, notifier);
    }

    g_closure_unref(closure);
    return handlerId;
}

bool ConnectionsStore::disconnect(void *instance, uint signal, Quark detail,
                                  void *receiver, uint slotHash, ulong handlerId)
{
    QMutexLocker l(&m_mutex);
    return lookupAndExec(instance, signal, detail, receiver, slotHash, handlerId,
                         &ConnectionsStore::disconnectAndDestroyRcvrWatch);
}

bool ConnectionsStore::lookupAndExec(void *instance, uint signal, Quark detail,
                                     void *receiver, uint slotHash, ulong handlerId,
                                     void (ConnectionsStore::*func)(void*, const Connection &))
{
    bool executed = false;

    if (m_connections.contains(instance)) {
        ConnectionsContainer & container = m_connections[instance];

        if (handlerId) {
            ByHandlerIterator it = container.get<by_handlerId>().find(handlerId);

            if (it != container.get<by_handlerId>().end()) {
                (this->*func)(instance, *it);
                executed = true;

                container.get<by_handlerId>().erase(it);
            }
        } else if (signal) {
            BySignalIterators iterators = container.get<by_signal>().equal_range(signal);

            while (iterators.first != iterators.second) {
                if (!detail ||
                        (detail == iterators.first->detail &&
                            (!receiver ||
                                (receiver == iterators.first->receiver &&
                                    (!slotHash || slotHash == iterators.first->slotHash)
                                )
                            )
                        )
                   )
                {
                    (this->*func)(instance, *iterators.first);
                    executed = true;

                    iterators.first = container.get<by_signal>().erase(iterators.first);
                } else {
                    ++iterators.first;
                }
            }
        } else if (receiver) {
            ByReceiverIterators iterators = container.get<by_receiver>().equal_range(receiver);

            while (iterators.first != iterators.second) {
                if (!slotHash || slotHash == iterators.first->slotHash) {
                    (this->*func)(instance, *iterators.first);
                    executed = true;

                    iterators.first = container.get<by_receiver>().erase(iterators.first);
                } else {
                    ++iterators.first;
                }
            }
        } else {
            for (SequentialIterator it = container.get<sequential>().begin();
                 it != container.get<sequential>().end(); ++it)
            {
                (this->*func)(instance, *it);
                executed = true;
            }
            container.get<sequential>().clear();
        }

        if (container.get<sequential>().empty()) {
            m_connections.remove(instance);
        }
    }

    return executed;
}

void ConnectionsStore::disconnectHandler(void *instance, const Connection & c)
{
    m_handlerIdInRemovalMutex.lock();
    m_handlerIdInRemoval = c.handlerId;
    m_handlerIdInRemovalMutex.unlock();

    /* This will unref the closure and cause onClosureDestroyed to be invoked. */
    g_signal_handler_disconnect(instance, c.handlerId);

    m_handlerIdInRemovalMutex.lock();
    m_handlerIdInRemoval = 0;
    m_handlerIdInRemovalMutex.unlock();
}

void ConnectionsStore::disconnectAndDestroyRcvrWatch(void *instance, const Connection & c)
{
    disconnectHandler(instance, c);
    destroyReceiverWatch(instance, c);
}

void ConnectionsStore::setupClosureWatch(void *instance, ulong handlerId, GClosure *closure)
{
    void *data = new QPair<void*, ulong>(instance, handlerId);
    g_closure_add_finalize_notifier(closure, data, &ConnectionsStore::onClosureDestroyed);
}

//static
void ConnectionsStore::onClosureDestroyed(void *data, GClosure *closure)
{
    Q_UNUSED(closure);
    QPair<void*, ulong> *pair = static_cast< QPair<void*, ulong>* >(data);
    s_connectionsStore()->onClosureDestroyedAction(pair->first, pair->second);
    delete pair;
}

void ConnectionsStore::onClosureDestroyedAction(void *instance, ulong handlerId)
{
    /* Do not do any action if we are being invoked from disconnectHandler() */
    m_handlerIdInRemovalMutex.lock();
    register bool ok = (m_handlerIdInRemoval != handlerId);
    m_handlerIdInRemovalMutex.unlock();

    if (ok) {
        QMutexLocker l(&m_mutex);
        lookupAndExec(instance, 0, Quark(), 0, 0, handlerId, &ConnectionsStore::destroyReceiverWatch);
    }
}

void ConnectionsStore::setupReceiverWatch(void *instance, void *receiver,
                                          const DestroyNotifierIfacePtr & notifier)
{
    if (!m_receivers.contains(receiver)) {
        ReceiverData data;
        data.notifier = notifier;
        if (!notifier->connect(receiver, this, SLOT(onReceiverDestroyed(QObject*)))) {
            notifier->connect(receiver, this, SLOT(onReceiverDestroyed(void*)));
        }
        m_receivers.insert(receiver, data);
    }

    m_receivers[receiver].senders[instance]++;
}

void ConnectionsStore::destroyReceiverWatch(void *instance, const Connection & c)
{
    if (--m_receivers[c.receiver].senders[instance] == 0) {
        m_receivers[c.receiver].senders.remove(instance);
        if (m_receivers[c.receiver].senders.isEmpty()) {
            m_receivers[c.receiver].notifier->disconnect(c.receiver, this);
            m_receivers.remove(c.receiver);
        }
    }
}

void ConnectionsStore::onReceiverDestroyed(void *receiver)
{
    QMutexLocker l(&m_mutex);
    QHashIterator<void*, int> it(m_receivers[receiver].senders);
    while (it.hasNext()) {
        it.next();
        lookupAndExec(it.key(), 0, Quark(), receiver, 0, 0, &ConnectionsStore::disconnectHandler);
    }
    m_receivers.remove(receiver);
}

//optimization hack, to avoid making QObjectDestroyNotifier inherit
//QObject and re-emit QObject::destroyed() with void* argument
void ConnectionsStore::onReceiverDestroyed(QObject *receiver)
{
    onReceiverDestroyed(static_cast<void*>(receiver));
}

//END ******** ConnectionsStore ********
//BEGIN ******** connect ********

ulong connect(void *instance, const char *signal, Quark detail,
              void *receiver, const DestroyNotifierIfacePtr & notifier,
              uint slotHash, ClosureDataBase *closureData, ConnectFlags flags)
{
    guint signalId;
    GQuark detailQuark;

    if (g_signal_parse_name(signal, Type::fromInstance(instance),
                            &signalId, &detailQuark, FALSE))
    {
        if (!detail && detailQuark) {
            detail = detailQuark;
        }
        return s_connectionsStore()->connect(instance, signalId, detail, receiver,
                                             notifier, slotHash, closureData, flags);
    } else {
        qWarning() << "QGlib::connect: Could not parse signal:" << signal
                   << "- Either it does not exist on this instance, or a detail "
                      "was specified but the signal is not detailed";
        delete closureData;
    }

    return 0;
}

//END ******** connect ********
//BEGIN ******** disconnect ********

bool disconnect(void *instance, const char *signal, Quark detail,
                void *receiver, uint slotHash, ulong handlerId)
{
    guint signalId = 0;
    GQuark detailQuark = 0;

    if (signal) {
        if (g_signal_parse_name(signal, Type::fromInstance(instance),
                                &signalId, &detailQuark, FALSE))
        {
            if (!detail && detailQuark) {
                detail = detailQuark;
            }
        } else {
            qWarning() << "QGlib::disconnect: Could not parse signal:" << signal
                       << "- Either it does not exist on this instance, or a detail "
                          "was specified but the signal is not detailed";
            return false;
        }
    }

    return s_connectionsStore()->disconnect(instance, signalId, detail,
                                            receiver, slotHash, handlerId);
}

//END ******** disconnect ********

} //namespace Private
} //namespace QGlib

#include "connect.moc"
