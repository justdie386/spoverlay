#include "mainwindow.h"
#include "loginwindow.h"
#include "qapplication.h"
#include "qnamespace.h"
#include <filesystem>


int main(int argc, char **argv)
{
    spotifypp::credentials creds;
    if (std::filesystem::exists("config.json") == true) {
        creds.read_config();
        creds.use_refresh_token();
        QApplication a(argc, argv);
        mainwindow w(creds);
        w.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
        w.show();
        w.setFixedSize(64, 70);
        return a.exec();
    } else {
        QApplication a(argc, argv);
        loginwindow w(creds);

        w.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
        w.show();
        w.setFixedSize(120, 100);
        return a.exec();
    }

}
