/*
    Copyright 2014-2015 Harald Sitter <sitter@kde.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) version 3, or any
    later version accepted by the membership of KDE e.V. (or its
    successor approved by the membership of KDE e.V.), which shall
    act as a proxy defined in Section 6 of version 3 of the license.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "globalactioncollection.h"


GlobalAction::GlobalAction(QObject *parent)
    : QAction(parent)
{
}

GlobalActionCollection::GlobalActionCollection()
{
}

QString GlobalActionCollection::name() const
{
    return m_name;
}

void GlobalActionCollection::setName(const QString &name)
{
    m_name = name;
    emit nameChanged();
}

QString GlobalActionCollection::displayName() const
{
    return m_displayName;
}

void GlobalActionCollection::setDisplayName(const QString &displayName)
{
    m_displayName = displayName;
    emit displayNameChanged();
}

void GlobalActionCollection::componentComplete()
{
    foreach (QObject *item, children()) {
        GlobalAction *action = qobject_cast<GlobalAction *>(item);
        if (!action) {
            continue;
        }
        action->setProperty("componentName", m_name);
        action->setProperty("componentDisplayName", m_displayName);

    }

    QQuickItem::componentComplete();
}
