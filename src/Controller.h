#pragma once

#include "DeviceListModel.h"
#include "DeviceFilterProxyModel.h"
#include <memory>
#include <QDate>
#include <QObject>

class Controller : public QObject
{
    Q_OBJECT
    Q_PROPERTY(DeviceFilterProxyModel* filter_model READ model_data NOTIFY filter_model_changed)

public:
    static Controller *instance();

    [[nodiscard]] std::shared_ptr<DeviceListModel> model() const;
    void set_model(std::shared_ptr<DeviceListModel> const& model);

    [[nodiscard]] std::shared_ptr<DeviceFilterProxyModel> filter_model() const;
    void set_filter_model(const std::shared_ptr<DeviceFilterProxyModel> &filter_model);

signals:
    void model_changed();
    void filter_model_changed();
    void devices_changed();

private:
    Controller(QObject *parent = nullptr);

    DeviceFilterProxyModel *model_data() const;

    std::shared_ptr<DeviceListModel> m_model;
    std::shared_ptr<DeviceFilterProxyModel> m_filter_model;
    Controller * m_controller = nullptr;
};

Q_DECLARE_METATYPE(Controller*);
