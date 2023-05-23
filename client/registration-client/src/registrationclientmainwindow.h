#ifndef REGISTRATIONCLIENTMAINWINDOW_H
#define REGISTRATIONCLIENTMAINWINDOW_H

#include <QMainWindow>
#include "DeviceData.h"

namespace Ui {
class RegistrationClientMainWindow;
}

class RegistrationClientMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit RegistrationClientMainWindow(QWidget *parent = nullptr);
    ~RegistrationClientMainWindow();

signals:
    void notifyRegistrationRequest(damn::DeviceData);

private:
    void onRegisterBtnClicked();

private:
    Ui::RegistrationClientMainWindow *ui;
};

Q_DECLARE_METATYPE(damn::DeviceData);

#endif // REGISTRATIONCLIENTMAINWINDOW_H
