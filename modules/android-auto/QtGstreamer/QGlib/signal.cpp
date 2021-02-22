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
#include "qglib_signal.h"
#include "quark.h"
#include <glib-object.h>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

//proper initializer for GValue structs on the stack
#define QGLIB_G_VALUE_INITIALIZER {0, {{0}, {0}}}

namespace QGlib {

//BEGIN ******** Signal ********

#ifndef DOXYGEN_RUN

struct QTGLIB_NO_EXPORT Signal::Private : public QSharedData
{
    Private(uint i) : id(i), m_queryInitialized(false) {}

    uint id;
    GSignalQuery *query() const;

private:
    mutable GSignalQuery m_query;
    mutable bool m_queryInitialized;
};

GSignalQuery *Signal::Private::query() const
{
    if (!m_queryInitialized) {
        g_signal_query(id, &m_query);
        m_queryInitialized = true;
    }
    return &m_query;
}

#endif //DOXYGEN_RUN

Signal::Signal(uint id)
    : d(new Private(id))
{
}

Signal::Signal(const Signal & other)
    : d(other.d)
{
}

Signal & Signal::operator=(const Signal & other)
{
    d = other.d;
    return *this;
}

Signal::~Signal()
{
}

bool Signal::isValid() const
{
    return d->id != 0;
}

uint Signal::id() const
{
    return d->id;
}

QString Signal::name() const
{
    return QString::fromUtf8(d->query()->signal_name);
}

Signal::SignalFlags Signal::flags() const
{
    return QFlag(d->query()->signal_flags);
}

Type Signal::instanceType() const
{
    return d->query()->itype;
}

Type Signal::returnType() const
{
    return d->query()->return_type & ~G_SIGNAL_TYPE_STATIC_SCOPE;
}

QList<Type> Signal::paramTypes() const
{
    QList<Type> result;
    for(uint i=0; i<d->query()->n_params; ++i) {
        result.append(d->query()->param_types[i] & ~G_SIGNAL_TYPE_STATIC_SCOPE);
    }
    return result;
}

//static
Signal Signal::lookup(const char *name, Type type)
{
    return Signal(g_signal_lookup(name, type));
}

//static
QList<Signal> Signal::listSignals(Type type)
{
    QList<Signal> result;
    uint n_ids;
    uint *ids = g_signal_list_ids(type, &n_ids);
    for(uint i=0; i<n_ids; ++i) {
        result.append(Signal(ids[i]));
    }
    g_free(ids);
    return result;
}

//END ******** Signal ********

namespace Private {

//BEGIN ******** emit ********

Value emit(void *instance, const char *detailedSignal, Quark detail, const QList<Value> & args)
{
    Value result;
    Type itype = Type::fromInstance(instance);
    QStringList signalParts = QString::fromUtf8(detailedSignal).split(QLatin1String("::"));
    if (!detail && signalParts.size() > 1) {
        detail = Quark::fromString(signalParts[1]);
    }

    //initialize arguments array
    GValue *values = new GValue[args.size() + 1];
    memset(values, 0, sizeof(GValue) * (args.size() + 1));

    //set instance
    g_value_init(&values[0], itype);
    g_value_set_instance(&values[0], instance);

    try {
        //find the signal and perform sanity checks
        Signal signal = Signal::lookup(signalParts[0].toUtf8(), itype);
        if (!signal.isValid()) {
            throw QString(QLatin1String("Could not find any signal named %1 "
                                        "on this instance type")).arg(signalParts[0]);
        }

        QList<Type> paramTypes = signal.paramTypes();
        if (paramTypes.size() != args.size()) {
            throw QString(QLatin1String("The number of arguments that the signal accepts differ "
                                        "from the number of arguments provided to emit"));
        }

        //set arguments
        for(int i=0; i<args.size(); i++) {
            if (!args[i].type().isA(paramTypes[i])) {
                throw QString(QLatin1String("Argument %1 provided to emit is not of the "
                                            "type that the signal expects")).arg(i);
            } else {
                g_value_init(&values[i+1], args[i].type());
                g_value_copy(args[i], &values[i+1]);
            }
        }

        //initialize return value
        GValue returnValue = QGLIB_G_VALUE_INITIALIZER;
        if (signal.returnType() != Type::None) {
            g_value_init(&returnValue, signal.returnType());
        }

        //emit the signal
        g_signal_emitv(values, signal.id(), detail, &returnValue);

        if (G_IS_VALUE(&returnValue)) {
            result = Value(&returnValue);
            g_value_unset(&returnValue);
        }
    } catch (const QString & msg) {
        QString instanceName = Value(&values[0]).toString();

        qCritical() << "Error during emission of signal" << detailedSignal
                    << "on object"<< instanceName << ":" << msg;
    }

    //cleanup
    for(int i=0; i<args.size() + 1; i++) {
        g_value_unset(&values[i]);
    }
    delete[] values;

    return result;
}

//END ******** emit ********

} //namespace Private
} //namespace QGlib
