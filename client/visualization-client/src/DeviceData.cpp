#include "DeviceData.h"

DeviceData::DeviceData(const QString &identifier,
                       const QString &name,
                       const QString &user,
                       const QString &details,
                       const DeviceStatus &status)
    : m_identifier{identifier}
    , m_name{name}
    , m_current_user{user}
    , m_status{status}
    , m_details{details}
{}

QString DeviceData::identifier() const
{
    return m_identifier;
}

void DeviceData::set_identifier(const QString &identifier)
{
    if (m_identifier == identifier)
        return;
    m_identifier = identifier;
}

QString DeviceData::name() const
{
    return m_name;
}

void DeviceData::set_name(const QString &name)
{
    if (m_name == name)
        return;
    m_name = name;
}

DeviceData::DeviceStatus DeviceData::status() const
{
    return m_status;
}

void DeviceData::set_status(DeviceStatus status)
{
    if (m_status == status)
        return;
    m_status = status;
}

QString DeviceData::current_user() const
{
    return m_current_user;
}

void DeviceData::set_current_user(const QString &current_user)
{
    if (m_current_user == current_user)
        return;
    m_current_user = current_user;
}

QString DeviceData::details() const
{
    return m_details;
}

void DeviceData::set_details(const QString &details)
{
    if (m_details == details)
        return;
    m_details = details;
}
