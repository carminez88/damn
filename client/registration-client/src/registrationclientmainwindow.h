#ifndef REGISTRATIONCLIENTMAINWINDOW_H
#define REGISTRATIONCLIENTMAINWINDOW_H

#include <QMainWindow>
#include "RequestData.h"

namespace Ui {
class RegistrationClientMainWindow;
}

class RegistrationClientMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit RegistrationClientMainWindow(QString id, QWidget *parent = nullptr);
    ~RegistrationClientMainWindow();

signals:
    void notifyRequest(damn::RequestData);

private:
    damn::RequestData makeDeviceData(damn::RequestType type);

    void cleanFields();

    void toogleFields(bool active);

private:
    Ui::RegistrationClientMainWindow *ui;
};

Q_DECLARE_METATYPE(damn::RequestData);

#endif // REGISTRATIONCLIENTMAINWINDOW_H
