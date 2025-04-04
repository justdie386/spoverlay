#include "loginwindow.h"
#include "qboxlayout.h"
#include "qlineedit.h"
#include "qpushbutton.h"
#include <QLayout>
#include <QDesktopServices>

loginwindow::loginwindow(spotifypp::credentials creds, QWidget *parent) 
    : QMainWindow(parent) 
{
    this->creds = creds;

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget); // Set the central widget

    client_id_input = new QLineEdit(centralWidget);
    secret_id_input = new QLineEdit(centralWidget);
    login_button = new QPushButton("Confirm", centralWidget);


    first_layout = new QVBoxLayout; // Use QVBoxLayout instead of QHBoxLayout

    client_id_input->setPlaceholderText("client id");
    secret_id_input->setPlaceholderText("secret id");

    first_layout->addWidget(client_id_input);
    first_layout->addWidget(secret_id_input);
    first_layout->addWidget(login_button);

    connect(login_button, &QPushButton::released, this, &loginwindow::login_pressed);
    
    centralWidget->setLayout(first_layout); // Apply layout to central widget
}

    loginwindow::~loginwindow() {}

    void loginwindow::login_pressed()
    {
        this->creds.set_id(this->client_id_input->text().toStdString(), this->secret_id_input->text().toStdString());
        this->creds.create_auth_url();
        QDesktopServices::openUrl(QUrl(QString::fromStdString(this->creds.auth_url)));
        this->creds.set_id(client_id_input->text().toStdString(), secret_id_input->text().toStdString());

        this->creds.get_auth();
        this->creds.convert_to_base64();
        this->creds.get_access_n_refresh();
        this->creds.create_config();
        this->close();
    }