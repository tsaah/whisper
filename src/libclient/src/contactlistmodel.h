#ifndef CONTACTLISTMODEL_H
#define CONTACTLISTMODEL_H

#include <QAbstractItemModel>
#include <QHash>

namespace whisper {
namespace client {

class ContactListModel: public QAbstractItemModel {
    Q_OBJECT
public:
    ContactListModel(QObject* parent = nullptr);

    QModelIndex index(int row, int column, const QModelIndex &parent) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;

public slots:
    void initializeModelFromStorage();
    void saveModelToStorage();
    void addContact(quint64 userId);
    void removeContact(quint64 userId);

private:
    QHash<quint64, quint64> contactList_;
};

} // namespace client
} // namespace whisper

#endif // CONTACTLISTMODEL_H
