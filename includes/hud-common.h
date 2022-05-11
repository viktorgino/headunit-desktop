#ifndef HUDCOMMON_H
#define HUDCOMMON_H

#include <QObject>
#include <QVariantMap>

typedef struct PanelItem {
    QString name;
    QString label;
    QString source;
    bool fillSpace = false;
    QObject *contextProperty = nullptr;
    QVariantMap properties;

} PanelItem;
Q_DECLARE_METATYPE(PanelItem)
#endif // HUDCOMMON_H
