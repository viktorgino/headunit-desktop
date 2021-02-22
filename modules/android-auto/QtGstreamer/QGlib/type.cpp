/*
    Copyright (C) 2009-2010  George Kiagiadakis <kiagiadakis.george@gmail.com>
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
#include "type.h"
#include "quark.h"
#include <glib-object.h>

namespace QGlib {

Type Type::fromInstance(void *instance)
{
    if (!instance) {
        return Invalid;
    } else {
        return G_TYPE_FROM_INSTANCE(instance);
    }
}

Type Type::fromName(const char *name)
{
    return g_type_from_name(name);
}

QString Type::name() const
{
    return QString::fromUtf8(g_type_name(m_type));
}

Quark Type::nameQuark() const
{
    return g_type_qname(m_type);
}

bool Type::isValid() const
{
    return m_type != Type::Invalid;
}

bool Type::isAbstract() const
{
    return G_TYPE_IS_ABSTRACT(m_type);
}

bool Type::isDerived() const
{
    return G_TYPE_IS_DERIVED(m_type);
}

bool Type::isFundamental() const
{
    return G_TYPE_IS_FUNDAMENTAL(m_type);
}

bool Type::isValueType() const
{
    return G_TYPE_IS_VALUE_TYPE(m_type);
}

bool Type::hasValueTable() const
{
    return G_TYPE_HAS_VALUE_TABLE(m_type);
}

bool Type::isClassed() const
{
    return G_TYPE_IS_CLASSED(m_type);
}

bool Type::isInstantiatable() const
{
    return G_TYPE_IS_INSTANTIATABLE(m_type);
}

bool Type::isDerivable() const
{
    return G_TYPE_IS_DERIVABLE(m_type);
}

bool Type::isDeepDerivable() const
{
    return G_TYPE_IS_DEEP_DERIVABLE(m_type);
}

bool Type::isInterface() const
{
    return G_TYPE_IS_INTERFACE(m_type);
}

Type Type::fundamental() const
{
    return G_TYPE_FUNDAMENTAL(m_type);
}

Type Type::parent() const
{
    return g_type_parent(m_type);
}

uint Type::depth() const
{
    return g_type_depth(m_type);
}

Type Type::nextBase(Type rootType) const
{
    return g_type_next_base(m_type, rootType);
}

bool Type::isA(Type is_a_type) const
{
    return g_type_is_a(m_type, is_a_type);
}

static inline QList<Type> gtypeArrayToList(GType *array, uint n)
{
    QList<Type> result;
    for(uint i = 0; i<n; ++i) {
        result.append(array[i]);
    }
    g_free(array);
    return result;
}

QList<Type> Type::children() const
{
    uint n;
    GType *a = g_type_children(m_type, &n);
    return gtypeArrayToList(a, n);
}

QList<Type> Type::interfaces() const
{
    uint n;
    GType *a = g_type_interfaces(m_type, &n);
    return gtypeArrayToList(a, n);
}

QList<Type> Type::interfacePrerequisites() const
{
    uint n;
    GType *a = g_type_interface_prerequisites(m_type, &n);
    return gtypeArrayToList(a, n);
}

void *Type::quarkData(const Quark & qname) const
{
    return g_type_get_qdata(m_type, qname);
}

void Type::setQuarkData(const Quark & qname, void *data)
{
    g_type_set_qdata(m_type, qname, data);
}

}
