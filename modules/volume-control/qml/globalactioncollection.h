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

#ifndef GLOBALACTIONCOLLECTION_H
#define GLOBALACTIONCOLLECTION_H

#include <QAction>
#include <QQuickItem>

class GlobalAction : public QAction
{
    Q_OBJECT
public:
    GlobalAction(QObject *parent = nullptr);
};

class GlobalActionCollection : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString displayName READ displayName WRITE setDisplayName NOTIFY displayNameChanged)
public:
    GlobalActionCollection();

    QString name() const;
    void setName(const QString &name);

    QString displayName() const;
    void setDisplayName(const QString &displayName);

signals:
    void nameChanged();
    void displayNameChanged();

protected:
    virtual void componentComplete() Q_DECL_OVERRIDE Q_DECL_FINAL;

private:
    QString m_name;
    QString m_displayName;
};

#endif // GLOBALACTIONCOLLECTION_H
