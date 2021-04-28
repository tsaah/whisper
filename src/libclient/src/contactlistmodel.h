#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include "contact.h"

#include <QAbstractItemModel>

namespace whisper {
namespace client {

class ClientSqliteDataStorage;

class WHISPER_LIBCLIENT ContactListModel: public QAbstractItemModel {
    Q_OBJECT
public:
    ContactListModel(ClientSqliteDataStorage* db, QObject* parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void initializeModelFromList(const QList<Contact> &contactList);
    QList<Contact> getContactList() const;
    void addContact(const Contact &contact);
    void removeContact(const Contact &contact);

private:
    ClientSqliteDataStorage* db_{ nullptr };
    QHash<quint64, Contact> contactHash_;
    QHash<quint64, int> contactIndexHash_;
};

} // namespace client
} // namespace whisper

#endif // CONTACTLISTMODEL_H
