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
#ifndef QGLIB_PARAMSPEC_H
#define QGLIB_PARAMSPEC_H

#include "global.h"
#include "refpointer.h"
#include "type.h"
#include "value.h"
#include <QtCore/QString>

namespace QGlib {

/*! \headerfile paramspec.h <QGlib/ParamSpec>
 * \brief Wrapper class for GParamSpec
 *
 * GParamSpec is an object structure that encapsulates the metadata
 * required to specify parameters, such as e.g. GObject properties.
 */
class QTGLIB_EXPORT ParamSpec : public RefCountedObject
{
    QGLIB_WRAPPER(ParamSpec)
public:
    enum ParamFlag { //codegen: prefix=G_PARAM_, ReadWrite=READWRITE
        Readable = 1<<0,
        Writable = 1<<1,
        ReadWrite = Readable | Writable,
        Construct = 1<<2,
        ConstructOnly = 1<<3,
        LaxValidation = 1<<4,
        Deprecated = 1<<31
    };
    Q_DECLARE_FLAGS(ParamFlags, ParamFlag);

    QString name() const;
    QString nick() const;
    QString description() const;

    ParamFlags flags() const;
    Type valueType() const;
    Type ownerType() const;

    void *quarkData(const Quark & quark) const;
    void *stealQuarkData(const Quark & quark) const;
    void setQuarkData(const Quark & quark, void *data, void (*destroyCallback)(void*) = NULL);

protected:
    virtual void ref(bool increaseRef);
    virtual void unref();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(ParamSpec::ParamFlags)

} //namespace QGlib

QGLIB_REGISTER_TYPE(QGlib::ParamSpec) //codegen: GType=G_TYPE_PARAM
QGLIB_REGISTER_TYPE(QGlib::ParamSpec::ParamFlags)
QGLIB_REGISTER_WRAPIMPL_FOR_SUBCLASSES_OF(QGlib::ParamSpec, QGlib::Private::wrapParamSpec)

// HACK to support glib 2.24 when compiling the generated assertions of ParamFlag
// REMOVE THIS in future versions
#ifdef INCLUDED_FROM_CODEGEN
# if !GLIB_CHECK_VERSION(2,26,0)
#  define G_PARAM_DEPRECATED QGlib::ParamSpec::Deprecated
# endif
#endif //CODEGEN_RUN

#endif
