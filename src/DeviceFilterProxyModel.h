#pragma once

#include "DeviceListModel.h"
#include <QObject>
#include <QSortFilterProxyModel>

class DeviceFilterProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(bool filter_active READ filter_active WRITE set_filter_active NOTIFY filter_active_changed)
    Q_PROPERTY(DeviceData::DeviceStatus filter_status READ filter_status WRITE set_filter_status NOTIFY filter_status_changed)
public:
    DeviceFilterProxyModel(QObject *parent = nullptr);
    DeviceFilterProxyModel(DeviceListModel *model, QObject *parent = nullptr);

    [[nodiscard]] bool filter_active() const;
    void set_filter_active(bool filter_active);

    [[nodiscard]] DeviceData::DeviceStatus filter_status() const;
    void set_filter_status(const DeviceData::DeviceStatus &filter_status);

signals:
    void filter_active_changed();
    void filter_status_changed();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    DeviceData::DeviceStatus m_filter_status = DeviceData::DeviceStatus::Online;
    bool m_filter_active = false;
};
