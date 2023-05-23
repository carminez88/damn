#include "RegistrationClientMainWindow.h"
#include "ui_registrationclientmainwindow.h"

RegistrationClientMainWindow::RegistrationClientMainWindow(QString id, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RegistrationClientMainWindow)
{
    ui->setupUi(this);
    ui->deviceIDLineEdit->setText( std::move( id ) );

    toogleFields( true );

    connect( ui->registerBtn,   &QPushButton::clicked, [ this ](){ emit notifyRequest( makeDeviceData( damn::RequestType::Registration ) ); toogleFields( false ); } );
    connect( ui->disconnectBtn, &QPushButton::clicked, [ this ](){ emit notifyRequest( makeDeviceData( damn::RequestType::Disconnection ) ); cleanFields(); toogleFields( true ); } );
}

RegistrationClientMainWindow::~RegistrationClientMainWindow()
{
    delete ui;
}

damn::RequestData RegistrationClientMainWindow::makeDeviceData(damn::RequestType type)
{
    damn::RequestData rd;
    rd.userID = ui->userLineEdit->text().toStdString();
    rd.activityDetails = ui->activityLineEdit->text().toStdString();
    rd.requestType = type;

    return rd;
}

void RegistrationClientMainWindow::cleanFields()
{
    ui->userLineEdit->clear();
    ui->activityLineEdit->clear();
}

void RegistrationClientMainWindow::toogleFields(bool active)
{
    ui->userLineEdit->setEnabled( active );
    ui->activityLineEdit->setEnabled( active );
    ui->registerBtn->setEnabled( active );
    ui->disconnectBtn->setEnabled( not active );
}
