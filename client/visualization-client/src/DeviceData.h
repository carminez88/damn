#pragma once
#include <QObject>

class DeviceData
{
    // clang-format off
  Q_GADGET
  Q_PROPERTY(QString identifier READ identifier WRITE set_identifier)
  Q_PROPERTY(QString name READ name WRITE set_name)
  Q_PROPERTY(DeviceStatus status READ status WRITE set_status)
  Q_PROPERTY(QString current_user READ current_user WRITE set_current_user)
  Q_PROPERTY(QString details READ details WRITE set_details)
    // clang-format on

public:
    enum class DeviceStatus {
        Undefined = 0, //< indicates that the device cannot be detected
        Connecting,    //< indicates that a connection to the device is in progress
        Online         //< indicates that the device is connected
    };
    Q_ENUM(DeviceStatus)

    DeviceData() = default;
    DeviceData(DeviceData const &other) = default;

    DeviceData(QString const &identifier,
               QString const &name,
               QString const &user = {},
               QString const &details = {},
               DeviceStatus const &status = DeviceStatus::Undefined);

    [[nodiscard]] QString identifier() const;
    void set_identifier(const QString &identifier);

    [[nodiscard]] QString name() const;
    void set_name(const QString &name);

    [[nodiscard]] DeviceStatus status() const;
    void set_status(DeviceStatus status);

    [[nodiscard]] QString current_user() const;
    void set_current_user(const QString &current_user);

    [[nodiscard]] QString details() const;
    void set_details(const QString &details);

private:
    QString m_identifier;
    QString m_name;
    DeviceStatus m_status;
    QString m_current_user;
    QString m_details;
};

Q_DECLARE_METATYPE(DeviceData);
Q_DECLARE_METATYPE(DeviceData::DeviceStatus);
