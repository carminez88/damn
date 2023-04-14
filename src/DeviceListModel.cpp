#include "DeviceListModel.h"
#include "EnumConverter.h"

DeviceListModel::DeviceListModel(QObject *parent)
    : QAbstractListModel{parent}
{}

void DeviceListModel::add_device(const DeviceData &data)
{
    auto const *iterator
        = std::find_if(m_known_devices.cbegin(),
                       m_known_devices.cend(),
                       [data](auto const &device_data) {
                           return device_data.identifier().compare(data.identifier(),
                                                                   Qt::CaseSensitive)
                                  == 0;
                       });

    if (iterator != m_known_devices.cend()) {
        auto const row = std::distance(m_known_devices.cbegin(), iterator);
        m_known_devices.replace(row, data);
        emit dataChanged(index(row, 0), index(row, 0));
    } else {
        beginInsertRows({}, m_known_devices.size(), m_known_devices.size());
        m_known_devices.append(data);
        endInsertRows();
    }
}

int DeviceListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_known_devices.size();
}

QVariant DeviceListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_known_devices.size()) {
        return {};
    }

    auto const &device_data = m_known_devices.at(index.row());

    switch (role) {
    case +Roles::DeviceId:
        return device_data.identifier();
    case +Roles::Name:
        return device_data.name();
    case +Roles::Status:
        return +device_data.status();
    case +Roles::User:
        return device_data.current_user();
    default:
        return {};
    }
    Q_UNREACHABLE();
}

bool DeviceListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    Q_UNUSED(index);
    Q_UNUSED(value);
    Q_UNUSED(role);
    return false;
}

QHash<int, QByteArray> DeviceListModel::roleNames() const
{
    static QHash<int, QByteArray> roles;
    roles[+Roles::DeviceId] = "deviceId";
    roles[+Roles::Name] = "name";
    roles[+Roles::Status] = "status";
    roles[+Roles::User] = "user";
    return roles;
}
