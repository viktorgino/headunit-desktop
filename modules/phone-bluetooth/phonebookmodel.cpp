#include "phonebookmodel.h"

PhonebookModel::PhonebookModel(QObject *parent) : QAbstractListModel(parent)
{
    connect(&m_reader, &QtVersit::QVersitReader::stateChanged, this, &PhonebookModel::readerStateChanged);
}
QHash<int, QByteArray> PhonebookModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[NumbersRole] = "numbers";
    roles[ImageRole] = "image";
    roles[CallTypeRole] = "callType";
    return roles;
}

int PhonebookModel::rowCount(const QModelIndex &parent) const{
    Q_UNUSED(parent)
    return m_vcards.size();
}

QVariant PhonebookModel::data(const QModelIndex &index, int role) const{
    QtVersit::QVersitDocument card = m_vcards[index.row()];

    QString name;
    QVariantList numbers;
    QString image;
    QString callType;
    for(QtVersit::QVersitProperty property : card.properties()) {
        if(property.name() == "FN") {
            name = property.value();
        } else if(property.name() == "TEL") {
            QVariantMap numberDetails;
            QMultiHash<QString, QString> parameters = property.parameters();

            numberDetails["number"] = property.value();

            for(auto item = parameters.begin(); item != parameters.end(); item++) {
                if(item.key() == "TYPE") {
                    numberDetails["type"] = item.value();
                }
            }
            numbers << numberDetails;
        } else if(property.name() == "PHOTO") {
            image = property.value();
        } else if(property.name() == "X-IRMC-CALL-DATETIME"){
            QMultiHash<QString, QString> parameters = property.parameters();
            if(parameters.size() > 0){
                callType = parameters.begin().value();
            }

        }
    }

    switch (role) {
    case NameRole:
        return name;
        break;
    case NumbersRole:
        return numbers;
        break;
    case ImageRole:
        return image;
        break;
    case CallTypeRole:
        return callType;
        break;
    default:
        return "";
        break;
    }
}

static QString urlToLocalFileName(const QUrl& url)
{
    if (!url.isValid()) {
        return url.toString();
    } else if (url.scheme() == "qrc") {
        return url.toString().remove(0, 5).prepend(':');
    } else {
        return url.toLocalFile();
    }

}
void PhonebookModel::importContacts(const QUrl& url, bool sort)
{
    m_sort = sort;
    // Reader is capable of handling only one request at the time.
    int importError = 0;
    if (m_reader.state() != QtVersit::QVersitReader::ActiveState) {
        QString fileName = urlToLocalFileName(url);
        QFile file(fileName);

        if (file.open(QIODevice::ReadOnly)) {
            QTextStream stream(&file);

            m_reader.setData(QByteArray(file.readAll()));
            if (m_reader.startReading()) {
                m_lastImportUrl = url;
                file.close();
                return;
            }
            file.close();
            importError = m_reader.error();
        } else {
            importError = 1;
        }
    }
    if(importError > 0)
        qDebug() << "Import error " << importError << url;
}

bool sortAsc(const QtVersit::QVersitDocument &a, const QtVersit::QVersitDocument &b)
{
    QString nameA, nameB;
    for(QtVersit::QVersitProperty property : a.properties()) {
        if(property.name() == "FN") {
            nameA = property.value();
        }
    }
    for(QtVersit::QVersitProperty property : b.properties()) {
        if(property.name() == "FN") {
            nameB = property.value();
        }
    }
    return nameA < nameB;
}

void PhonebookModel::readerStateChanged(QtVersit::QVersitReader::State state) {
    if (state == QtVersit::QVersitReader::FinishedState || state == QtVersit::QVersitReader::CanceledState) {
        beginResetModel();
        m_vcards = m_reader.results();
        if(m_sort){
            std::sort(m_vcards.begin(), m_vcards.end(), sortAsc);
        }
        endResetModel();
    }
}

