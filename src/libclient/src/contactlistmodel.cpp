#include "ContactListModel.h"
#include "ClientSqliteDataStorage.h"

namespace whisper::client {

ContactListModel::ContactListModel(ClientSqliteDataStorage *db, QObject *parent)
    : QAbstractItemModel(parent)
    , db_(db)
{
    initializeModelFromList(db_->getContactList());
}

//QModelIndex ContactListModel::index(int row, int column, const QModelIndex &parent) const {
//    void* dataPointer = nullptr;
//    return createIndex(row, column, dataPointer);
//}

//QModelIndex ContactListModel::parent(const QModelIndex &child) const {
//    return {};
//}

//int ContactListModel::rowCount(const QModelIndex &parent) const {
//    return contactHash_.size();
//}

//int ContactListModel::columnCount(const QModelIndex &parent) const {
//    return 1;
//}

QVariant ContactListModel::data(const QModelIndex &index, int role) const {
//    const auto key = contactHash_.keys().at(index.row());
//    const auto contact = contactHash_.value(key);
    QVariant result;
//    switch (role) {
//        case Qt::UserRole + 0: {
//            result = contact.userId;
//        } break;
//        case Qt::UserRole + 1: {
//            result = contact.approved;
//        } break;
//        case Qt::UserRole + 2: {
//            result = contact.certificate;
//        } break;
//    }
    return result;
}

QHash<int, QByteArray> whisper::client::ContactListModel::roleNames() const {
    return {
        { Qt::UserRole + 0, "userId" },
        { Qt::UserRole + 1, "approved" },
        { Qt::UserRole + 2, "certificate" }
    };
}

void ContactListModel::initializeModelFromList(const QList<Contact>& contactList) {
    beginResetModel();
//    contactIndexHash_.clear();
//    contactHash_.clear();
//    for (const auto& c: contactList) {
//        const auto userId = c.userId;
//        contactIndexHash_.insert(userId, contactHash_.size());
//        contactHash_.insert(userId, c);
//    }
    endResetModel();
}

//QList<Contact> ContactListModel::getContactList() const {
//    return contactHash_.values();
//}

//void ContactListModel::addContact(const Contact& contact) {
//    const auto userId = contact.userId;
//    if (contactHash_.contains(userId)) { return; }
//    beginInsertRows({}, contactHash_.size(), contactHash_.size());
//    contactIndexHash_.insert(userId, contactHash_.size());
//    contactHash_.insert(userId, contact);
//    endInsertRows();
//}

//void ContactListModel::removeContact(const Contact &contact) {
//    const auto userId = contact.userId;
//    if (!contactHash_.contains(userId)) { return; }
//    const auto row = contactIndexHash_.value(userId);
//    beginRemoveRows({}, row, row);
//    contactIndexHash_.remove(userId);
//    contactHash_.remove(userId);
//    for (const auto& k: contactIndexHash_.keys()) {
//        auto& index = contactIndexHash_[k];
//        if (index > row) {
//            --index;
//        }
//    }
//    endRemoveRows();
//}

} // namespace whisper::client



