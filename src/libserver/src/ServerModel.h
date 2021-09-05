#pragma once

#include "libserver_export.h"

#include <QAbstractItemModel>

namespace whisper::server {

class Dispatcher;
class ServerController;

class WHISPER_LIBSERVER Item {
public:
    explicit Item(const QVector<QVariant>& data, Item* parentItem = nullptr)
        : itemData_(data)
        , parentItem_(parentItem)
    {
    }
    virtual ~Item() {
        qDeleteAll(childItems_);
    }

    void appendChild(Item* child) {
        childItems_.append(child);
    }

    Item* child(int row) {
        return childItems_.at(row);
    }
    int childCount() const {
        return childItems_.size();
    }
    int columnCount() const {
        return itemData_.size();
    }
    QVariant data(int column) const {
        if (column < 0 || column >= itemData_.size()) {
            return {};
        }
        return itemData_.at(column);
    }
    int row() const {
        if (parentItem_) {
            return parentItem_->childItems_.indexOf(const_cast<Item*>(this));
        }
        return 0;
    }
    Item* parentItem() {
        return parentItem_;
    }

protected:
    QVector<Item*> childItems_;
    QVector<QVariant> itemData_;
    Item* parentItem_{ nullptr };
};

class WHISPER_LIBSERVER UserItem: public Item {
public:
    UserItem(const QVector<QVariant>& data, Item* parentItem = nullptr): Item(data, parentItem) {}


};



class WHISPER_LIBSERVER ServerModel: public QAbstractItemModel {
    Q_OBJECT
public:
    explicit ServerModel(Dispatcher* dispatcher, QObject* parent = nullptr);
    ~ServerModel();

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private slots:
    void onControllerAdded(ServerController* controller);
    void onControllerRemoved(ServerController* controller);

private:
    Dispatcher* dispatcher_{ nullptr };
    Item* rootItem_{ nullptr };
};

} // namespace whisper::server
