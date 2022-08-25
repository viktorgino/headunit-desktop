#include "ofono_dbus_types.h"

QDBusArgument &operator<<(QDBusArgument &arg, const ObjectPathProperties &props)
{
    arg.beginStructure();
    arg << props.path << props.properties;
    arg.endStructure();
    return arg;
}

const QDBusArgument &operator>>(const QDBusArgument &arg, ObjectPathProperties &props)
{
    arg.beginStructure();
    arg >> props.path >> props.properties;
    arg.endStructure();
    return arg;
}
void registerMetaTypes()
{

}
