#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H

#include <QObject>
#include "QVcf/vcfreader.h"

class AddressBook : public QObject
{
    Q_OBJECT
public:
    explicit AddressBook(QObject *parent = nullptr);

signals:

public slots:
};

#endif // ADDRESSBOOK_H
