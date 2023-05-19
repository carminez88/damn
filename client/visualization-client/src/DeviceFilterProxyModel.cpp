#include "DeviceFilterProxyModel.h"
#include "EnumConverter.h"
#include <QDebug>

DeviceFilterProxyModel::DeviceFilterProxyModel(QObject *parent) : DeviceFilterProxyModel(nullptr, parent)
{
}

DeviceFilterProxyModel::DeviceFilterProxyModel(DeviceListModel *model, QObject *parent) : QSortFilterProxyModel(parent)
{
    setSourceModel(model);
    connect(this, &DeviceFilterProxyModel::filter_active_changed, [this] {
//        if(!m_filter_active){
            invalidateFilter();
//        }
    });
}

bool DeviceFilterProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    Q_ASSERT(sourceModel() != nullptr);

    // Early exit in case of disabled filter
    if(!m_filter_active){
        return true;
    }

    auto const index = sourceModel()->index(source_row, 0, source_parent);
    return  index.data(+DeviceListModel::Roles::Status) == +m_filter_status &&
        index.data(+DeviceListModel::Roles::User).toString().isEmpty();
}

DeviceData::DeviceStatus DeviceFilterProxyModel::filter_status() const
{
    return m_filter_status;
}

void DeviceFilterProxyModel::set_filter_status(const DeviceData::DeviceStatus &filter_status)
{
    if (m_filter_status == filter_status)
        return;
    m_filter_status = filter_status;
    emit filter_status_changed();
}

bool DeviceFilterProxyModel::filter_active() const
{
    return m_filter_active;
}

void DeviceFilterProxyModel::set_filter_active(bool filter_active)
{
    if (m_filter_active == filter_active)
        return;
    m_filter_active = filter_active;
    emit filter_active_changed();
}
