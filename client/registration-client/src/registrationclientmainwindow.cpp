#include "registrationclientmainwindow.h"
#include "ui_registrationclientmainwindow.h"

RegistrationClientMainWindow::RegistrationClientMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegistrationClientMainWindow)
{
    ui->setupUi(this);

    connect( ui->registerBtn, &QPushButton::clicked, this, &RegistrationClientMainWindow::onRegisterBtnClicked );
}

RegistrationClientMainWindow::~RegistrationClientMainWindow()
{
    delete ui;
}

void RegistrationClientMainWindow::onRegisterBtnClicked()
{
    damn::DeviceData dd;
    dd.userID = ui->userLineEdit->text().toStdString();
    dd.activityDetails = ui->activityLineEdit->text().toStdString();

    emit notifyRegistrationRequest( dd );
}
