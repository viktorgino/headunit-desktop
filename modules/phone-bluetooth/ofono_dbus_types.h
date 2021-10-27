#ifndef OFONO_DBUS_TYPES_H
#define OFONO_DBUS_TYPES_H

#include <QtDBus/QtDBus>

struct ObjectPathProperties
{
    QDBusObjectPath path;
    QVariantMap properties;
};

QDBusArgument &operator<<(QDBusArgument &, const ObjectPathProperties &);
const QDBusArgument &operator>>(const QDBusArgument &, ObjectPathProperties &);

Q_DECLARE_METATYPE(ObjectPathProperties)

typedef ::ObjectPathProperties ObjectPathProperties;
typedef ::QList<ObjectPathProperties> ObjectPathPropertiesList;

#endif // OFONO_DBUS_TYPES_H
