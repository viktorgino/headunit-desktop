#ifndef PHONEBOOKMODEL_H
#define PHONEBOOKMODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QString>
#include <QFile>
#include <QUrl>
#include <QMultiHash>
#include "qversitreader/qversitreader.h"

class PhonebookModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QUrl vcardFile READ getVcardFile WRITE importContacts NOTIFY vcardChanged)
public:
    enum MediaLibraryContainerContentRoles {
        NameRole = Qt::UserRole + 1,
        NumbersRole,
        ImageRole,
        CallTypeRole
    };
    PhonebookModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QUrl getVcardFile() {
        return m_vcardFile;
    }
    Q_INVOKABLE void importContacts(const QUrl& url, bool sort = false);
signals:
    void vcardChanged();
private slots:
    void readerStateChanged(QtVersit::QVersitReader::State state);
private:
    QVariantList m_phonebookContent;
    QUrl m_vcardFile;
    QtVersit::QVersitReader m_reader;
    QUrl m_lastImportUrl;
    QList<QtVersit::QVersitDocument> m_vcards;
    bool m_sort;
};

#endif // PHONEBOOKMODEL_H
