#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QImage>
#include <QPalette>
#include <QMouseEvent>  // Added for mouse events
#include <QProgressBar>
#include "qprogressbar.h"
#include "spotifypp.h"

class mainwindow : public QMainWindow {
    Q_OBJECT

public:
    explicit mainwindow(spotifypp::credentials creds, QWidget *parent = nullptr);
    ~mainwindow();

private:
    // Track information
    std::string current_song;
    std::string current_album;

    // Spotify credentials
    spotifypp::credentials creds;

    // UI Elements
    QLabel *songLabel;
    QLabel *albumCoverLabel;
    QPushButton *skip_forward;
    QPushButton *skip_backward;
    QPushButton *song_activity;
    QWidget *buttonContainer;
    // Image handling
    QIcon *pause;
    QIcon *play;
    QIcon skip_forward_icon;
    QIcon skip_backward_icon;
    
    // Layouts 
    QFont font;
    spotifypp::song song;
    bool isplaying;

    // Well...
    QProgressBar *bar;
    QPoint m_pCursor;   


private slots:
    void loop();
    void handleResults(const spotifypp::song song);
    void playorpause();
    void refresh_token();
    void mousePressEvent(QMouseEvent *event);  // Added mouse event declarations
    void mouseMoveEvent(QMouseEvent *event);
    void skip_forward_action();
    void skip_backward_action();
};

#endif // MAINWINDOW_H
