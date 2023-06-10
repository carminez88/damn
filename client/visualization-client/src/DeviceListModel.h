#pragma once

#include <QAbstractListModel>
#include "DeviceData.h"

class DeviceListModel : public QAbstractListModel
{
public:
    enum class Roles {
        DeviceId = Qt::UserRole + 1,
        Name,
        Status,
        User,
        Details,
    };

    DeviceListModel(QObject *parent = nullptr);

    void add_device(DeviceData const &data);

    int rowCount(QModelIndex const &parent = QModelIndex()) const override;
    QVariant data(QModelIndex const &index = QModelIndex(),
                          int role = Qt::DisplayRole) const override;
    bool setData(QModelIndex const &index,
                         QVariant const &value,
                         int role = Qt::EditRole) override;

    QHash<int, QByteArray> roleNames() const override;

private:
    QVector<DeviceData> m_known_devices;
};
