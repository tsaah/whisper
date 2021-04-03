#include "servermodel.h"
#include "dispatcher.h"
#include "servercontroller.h"

namespace whisper {
namespace server {

ServerModel::ServerModel(Dispatcher *dispatcher, QObject* parent)
    : QAbstractItemModel(parent)
    , dispatcher_(dispatcher)
{
    rootItem_ = new Item({ "Server" });
    connect(dispatcher_, &Dispatcher::controllerAdded, this, &ServerModel::onControllerAdded);
    connect(dispatcher_, &Dispatcher::controllerRemoved, this, &ServerModel::onControllerRemoved);
}

ServerModel::~ServerModel() {
    delete rootItem_;
}

QModelIndex ServerModel::index(int row, int column, const QModelIndex &parent) const {
    if (!hasIndex(row, column, parent)) {
        return {};
    }

    Item* parentItem = nullptr;

    if (!parent.isValid()) {
        parentItem = rootItem_;
    } else {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    Item* childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }
    return {};
}

QModelIndex ServerModel::parent(const QModelIndex &child) const {
    if (!child.isValid()) {
        return {};
    }

    Item* childItem = static_cast<Item*>(child.internalPointer());
    Item* parentItem = childItem->parentItem();

    if (parentItem == rootItem_) {
        return {};
    }

    return createIndex(parentItem->row(), 0, parentItem);
}

int ServerModel::rowCount(const QModelIndex &parent) const {
    Item* parentItem = nullptr;
    if (parent.column() > 0) {
        return 0;
    }

    if (!parent.isValid()) {
        parentItem = rootItem_;
    } else {
        parentItem = static_cast<Item*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int ServerModel::columnCount(const QModelIndex &parent) const {
    if (parent.isValid()) {
        return static_cast<Item*>(parent.internalPointer())->columnCount();
    }
    return rootItem_->columnCount();
}

QVariant ServerModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return {};
    }

    if (role != Qt::DisplayRole) {
        return {};
    }

    Item* item = static_cast<Item*>(index.internalPointer());

    return item->data(index.column());
}

QVariant ServerModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem_->data(section);
    }

    return {};
}

Qt::ItemFlags ServerModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

void ServerModel::onControllerAdded(ServerController *controller) {
    beginResetModel();
    auto* connection = new Item({ QString::number(qintptr(controller)), QString::number(qintptr(controller)) }, rootItem_);
    rootItem_->appendChild(connection);


    endResetModel();
}

void ServerModel::onControllerRemoved(ServerController *controller) {
//    beginResetModel();
//    rootItem_->appendChild(new Item({ QString::number(qintptr(controller)), QString::number(qintptr(controller)) }, rootItem_));
//    endResetModel();
}

} // namespace server
} // namespace whisper



