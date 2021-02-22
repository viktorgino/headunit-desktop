/*
    Copyright (C) 2012-2013 Collabora Ltd. <info@collabora.com>
      @author George Kiagiadakis <george.kiagiadakis@collabora.com>

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

#include "videoitem.h"
#include "videosurface_p.h"
#include <QtQuick/QSGNode>
#include <QtQuick/QSGFlatColorMaterial>
#include "../../QGlib/Signal"

namespace QGst {
namespace Quick {

struct VideoItem::Private
{
    QPointer<VideoSurface> surface;
    bool surfaceDirty;
    QRectF targetArea;
};

VideoItem::VideoItem(QQuickItem *parent)
    : QQuickItem(parent), d(new Private)
{
    d->surfaceDirty = true;
    setFlag(QQuickItem::ItemHasContents, true);
}

VideoItem::~VideoItem()
{
    setSurface(0);
    delete d;
}

VideoSurface *VideoItem::surface() const
{
    return d->surface.data();
}

void VideoItem::setSurface(VideoSurface *surface)
{
    if (d->surface) {
        d->surface.data()->d->items.remove(this);
    }

    d->surface = surface;
    d->surfaceDirty = true;

    if (d->surface) {
        d->surface.data()->d->items.insert(this);
    }
}

QSGNode* VideoItem::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *data)
{
    Q_UNUSED(data)

    QRectF r = boundingRect();
    QSGNode *newNode = 0;

    if (d->surfaceDirty) {
        delete oldNode;
        oldNode = 0;
        d->surfaceDirty = false;
    }

    if (!d->surface || d->surface.data()->d->videoSink.isNull()) {
        if (!oldNode) {
            QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
            material->setColor(Qt::black);

            QSGGeometryNode *node = new QSGGeometryNode;
            node->setMaterial(material);
            node->setFlag(QSGNode::OwnsMaterial);
            node->setFlag(QSGNode::OwnsGeometry);

            newNode = node;
            d->targetArea = QRectF(); //force geometry to be set
        } else {
            newNode = oldNode;
        }

        if (r != d->targetArea) {
            QSGGeometry *geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
            geometry->vertexDataAsPoint2D()[0].set(r.x(), r.y());
            geometry->vertexDataAsPoint2D()[1].set(r.x(), r.height());
            geometry->vertexDataAsPoint2D()[2].set(r.width(), r.y());
            geometry->vertexDataAsPoint2D()[3].set(r.width(), r.height());

            QSGGeometryNode *node = static_cast<QSGGeometryNode*>(newNode);
            node->setGeometry(geometry);

            d->targetArea = r;
        }
    } else {
        newNode = (QSGNode*) QGlib::emit<void*>(d->surface.data()->d->videoSink,
                "update-node", (void*)oldNode,
                r.x(), r.y(), r.width(), r.height());
    }

    return newNode;
}

} // namespace Quick
} // namespace QGst
