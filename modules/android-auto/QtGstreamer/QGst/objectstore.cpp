/*
    Copyright (C) 2010  Collabora Multimedia.
      @author Mauricio Piacentini <mauricio.piacentini@collabora.co.uk>

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

#include <QtCore/QMutex>
#include <QtCore/QHash>
#include <QtCore/QAtomicInt>

#include "objectstore_p.h"

namespace {
    class GlobalStore
    {
    public:
        QMutex mutex;
        QHash<const void *, QAtomicInt> refCount;
    };
}

Q_GLOBAL_STATIC(GlobalStore, globalStore)

namespace QGst {
namespace Private {

bool ObjectStore::put(const void * ptr)
{
    bool mustAddStrongRef = false;
    GlobalStore *const gs = globalStore();
    if (!gs) return mustAddStrongRef;

    QMutexLocker lock(&gs->mutex);
    if (!gs->refCount.contains(ptr)) {
        gs->refCount.insert(ptr, QAtomicInt(0));
        mustAddStrongRef = true;
    }
    (gs->refCount[ptr]).ref();

    return mustAddStrongRef;
}

bool ObjectStore::take(const void * ptr)
{
    bool mustSubtractStrongRef = false;
    GlobalStore *const gs = globalStore();
    if (!gs) return mustSubtractStrongRef;

    QMutexLocker lock(&gs->mutex);

    //Make sure there are no extra unrefs()
    Q_ASSERT(gs->refCount.contains(ptr));

    if (!gs->refCount.contains(ptr)) {
        return false;
    }

    //Decrease our bindings (weak) reference count
    (gs->refCount[ptr]).deref();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
    if (!gs->refCount[ptr].load()) {
#else
    if (!gs->refCount[ptr]) {
#endif
        //refCount is 0
        gs->refCount.remove(ptr);
        mustSubtractStrongRef = true;
    }
    return mustSubtractStrongRef;
}

bool ObjectStore::isEmpty()
{
    GlobalStore *const gs = globalStore();
    if (!gs) return true;

    QMutexLocker lock(&gs->mutex);

    if (gs->refCount.count()>0) {
        return false;
    }

    return true;
}

}
} //namespace QGst
