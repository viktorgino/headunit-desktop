/*
    Copyright (C) 2010 Collabora Ltd. <info@collabora.co.uk>
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
#include "refpointer.h"
#include "quark.h"
#include <glib-object.h>

namespace QGlib {

RefCountedObject *constructWrapper(Type instanceType, void *instance)
{
    Quark q = g_quark_from_static_string("QGlib__wrapper_constructor");
    RefCountedObject *cppClass = NULL;

    for(Type t = instanceType; t.isValid(); t = t.parent()) {
        void *funcPtr = t.quarkData(q);
        if (funcPtr) {
            cppClass = (reinterpret_cast<RefCountedObject *(*)(void*)>(funcPtr))(instance);
            Q_ASSERT_X(cppClass, "QGlib::constructWrapper",
                       "Failed to wrap instance. This is a bug in the bindings library.");
            return cppClass;
        }
    }

    Q_ASSERT_X(false, "QGlib::constructWrapper",
               QString(QLatin1String("No wrapper constructor found for this type (") +
                       instanceType.name() + QLatin1String("). Did you forget to call init()?.")).toUtf8());
    return cppClass;
}

namespace Private {

static void qdataDestroyNotify(void *cppInstance)
{
    delete static_cast<RefCountedObject*>(cppInstance);
}

RefCountedObject *wrapObject(void *gobject)
{
    Q_ASSERT(gobject);

    Quark q = g_quark_from_static_string("QGlib__object_wrapper");
    RefCountedObject *obj = static_cast<RefCountedObject*>(g_object_get_qdata(G_OBJECT(gobject), q));

    if (!obj) {
        obj = constructWrapper(Type::fromInstance(gobject), gobject);
        g_object_set_qdata_full(G_OBJECT(gobject), q, obj, &qdataDestroyNotify);
    }

    return obj;
}

RefCountedObject *wrapParamSpec(void *param)
{
    Q_ASSERT(param);

    Quark q = g_quark_from_static_string("QGlib__paramspec_wrapper");
    RefCountedObject *obj = static_cast<RefCountedObject*>(g_param_spec_get_qdata(G_PARAM_SPEC(param), q));

    if (!obj) {
        obj = constructWrapper(Type::fromInstance(param), param);
        g_param_spec_set_qdata_full(G_PARAM_SPEC(param), q, obj, &qdataDestroyNotify);
    }

    return obj;
}

RefCountedObject *wrapInterface(Type interfaceType, void *gobject)
{
    Q_ASSERT(gobject);

    Quark q = Quark::fromString(QLatin1String("QGlib__interface_wrapper__") + interfaceType.name());
    RefCountedObject *obj = static_cast<RefCountedObject*>(g_object_get_qdata(G_OBJECT(gobject), q));

    if (!obj) {
        obj = constructWrapper(interfaceType, gobject);
        g_object_set_qdata_full(G_OBJECT(gobject), q, obj, &qdataDestroyNotify);
    }

    return obj;
}

} //namespace Private
} //namespace QGlib
