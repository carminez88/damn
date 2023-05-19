#include "Controller.h"

#include <QPointer>

Controller* Controller::instance() {
    static QPointer<Controller> _instance;
    if (!_instance) {
        _instance = new Controller;
    }
    return _instance;
}

std::shared_ptr<DeviceListModel> Controller::model() const { return m_model; }

void Controller::set_model(const std::shared_ptr<DeviceListModel>& model) {
    if (m_model != model) {
        return;
    }
    m_model = model;
    emit model_changed();
}

Controller::Controller(QObject* parent) : QObject(parent) {
    m_model = std::make_shared<DeviceListModel>(this);
    m_filter_model =
        std::make_shared<DeviceFilterProxyModel>(m_model.get(), this);
}

DeviceFilterProxyModel* Controller::model_data() const {
    return m_filter_model.get();
}

std::shared_ptr<DeviceFilterProxyModel> Controller::filter_model() const {
    return m_filter_model;
}

void Controller::set_filter_model(
    const std::shared_ptr<DeviceFilterProxyModel>& filter_model) {
    if (m_filter_model == filter_model)
        return;
    m_filter_model = filter_model;
    emit filter_model_changed();
}

void Controller::register_device_information(const DeviceData& deviceData) {
    m_model->add_device(deviceData);
}
