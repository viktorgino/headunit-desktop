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
#include "paramspec.h"
#include "string.h"
#include "quark.h"
#include <glib-object.h>

namespace QGlib {

QString ParamSpec::name() const
{
    return QString::fromUtf8(g_param_spec_get_name(object<GParamSpec>()));
}

QString ParamSpec::nick() const
{
    return QString::fromUtf8(g_param_spec_get_nick(object<GParamSpec>()));
}

QString ParamSpec::description() const
{
    return QString::fromUtf8(g_param_spec_get_blurb(object<GParamSpec>()));
}

ParamSpec::ParamFlags ParamSpec::flags() const
{
    return ParamFlags(object<GParamSpec>()->flags);
}

Type ParamSpec::valueType() const
{
    return Type(G_PARAM_SPEC_VALUE_TYPE(object<GParamSpec>()));
}

Type ParamSpec::ownerType() const
{
    return Type(object<GParamSpec>()->owner_type);
}

void *ParamSpec::quarkData(const Quark & quark) const
{
    return g_param_spec_get_qdata(object<GParamSpec>(), quark);
}

void *ParamSpec::stealQuarkData(const Quark & quark) const
{
    return g_param_spec_steal_qdata(object<GParamSpec>(), quark);
}

void ParamSpec::setQuarkData(const Quark & quark, void *data, void (*destroyCallback)(void*))
{
    g_param_spec_set_qdata_full(object<GParamSpec>(), quark, data, destroyCallback);
}

void ParamSpec::ref(bool increaseRef)
{
    if (increaseRef) {
        g_param_spec_ref(G_PARAM_SPEC(m_object));
    }
}

void ParamSpec::unref()
{
    g_param_spec_unref(G_PARAM_SPEC(m_object));
}

} //namespace QGlib
