#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "qboxlayout.h"
#include "qlabel.h"
#include "qpushbutton.h"
#include "spotifypp.h"
#include <QMainWindow>
#include <qtmetamacros.h>
#include <QWidget>
#include <QLineEdit>

    class loginwindow : public QMainWindow {
        Q_OBJECT
        
    public:
        explicit loginwindow(spotifypp::credentials creds, QWidget *parent = nullptr);
        ~loginwindow();
        QPushButton *login_button;
        QLabel *instructions;
        QLabel *link;
        QLineEdit *client_id_input;
        QLineEdit *secret_id_input;
        QWidget *centralWidget;
        QVBoxLayout *first_layout;
        QVBoxLayout *second_layout;
        spotifypp::credentials creds;
        string client_id;
        string secret_id;
    private slots:
        void login_pressed();
    };

#endif // LOGIN_H