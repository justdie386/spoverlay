#include "loginwindow.h"
#include "mainwindow.h"
#include "qapplication.h"
#include "qnamespace.h"
#include <filesystem>



int main(int argc, char **argv) {


#ifdef _WIN32
  string home = getenv("APPDATA");
  string path = string(home) + "/spoverlay";
#else

string home = getenv("HOME");
string path = string(home) + "/.config/spoverlay";

#endif


  cout << path << endl;
  if (argc > 1) {
    if (strcmp(argv[1], "reset") == 0) {
      cout << "deleting folder" << endl;
      filesystem::remove(path);
    }
  }
  spotifypp::credentials creds;
  cout << path + "/config.json"<< endl;
  if (std::filesystem::exists(path + "/config.json") == true) {

    creds.read_config(path + "/config.json");
    creds.use_refresh_token();
    QApplication a(argc, argv);
    mainwindow w(creds);
    w.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint |
                     Qt::FramelessWindowHint);
    w.show();
    w.setFixedSize(90, 69);
    return a.exec();
  } else {
    if (filesystem::exists(path) == false) {
      cout << "folder doesn't exist already, creating" << endl;
      filesystem::create_directory(path);
    }
    QApplication a(argc, argv);
    loginwindow w(creds);
    w.path = path + "/config.json";
    w.setWindowFlags(Qt::WindowStaysOnTopHint | Qt::WindowCloseButtonHint);
    w.show();
    w.setFixedSize(120, 100);
    cout << "hello?" << endl;
    return a.exec();
  }
}
