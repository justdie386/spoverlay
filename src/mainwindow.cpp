#include "mainwindow.h"
#include "qboxlayout.h"
#include "qnamespace.h"
#include "qobject.h"
#include "qpixmap.h"
#include "qprogressbar.h"
#include "qpushbutton.h"
#include "qtimer.h"
#include "qwidget.h"
#include "spotifypp.h"
#include "threading.h"
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>
#include <QPalette>
#include <QColor>

using namespace std;

mainwindow::mainwindow(spotifypp::credentials creds, QWidget *parent) : QMainWindow(parent) {
    this->creds = creds;

    // Create and configure your progress bar
    bar = new QProgressBar(this);
    bar->setRange(0, 100);
    bar->setFixedHeight(5);  // Fixed height for the progress bar
    bar->setStyleSheet("QProgressBar {"
        "border-radius: 0px;"  
        "border: 1px solid gray;"  
        "background-color: lightgray;"  
        "}"
        "QProgressBar::chunk {"
        "background-color: #4CAF50;"  
        "}");

    bar->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);  // Fixed size policy for the progress bar

    // Album cover label setup
    albumCoverLabel = new QLabel(this);
    albumCoverLabel->setScaledContents(true);
    albumCoverLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    albumCoverLabel->setFixedHeight(300);  // Fixed height for the album cover

    // Setup for the skip forward and backward buttons
    skip_forward_icon = *new QIcon(":/images/skip_forward.png");
    skip_forward = new QPushButton(this);
    skip_forward->setIcon(skip_forward_icon);
    skip_forward->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

    skip_backward_icon = *new QIcon(":/images/skip_backward.png");
    skip_backward = new QPushButton(this);
    skip_backward->setIcon(skip_backward_icon);
    skip_backward->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

    // Setup play/pause button
    song_activity = new QPushButton();
    song_activity->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");
    pause = new QIcon(":/images/pause.png");
    play = new QIcon(":/images/play.png");
    song_activity->setIcon(*play);

    connect(song_activity, &QPushButton::released, this, &mainwindow::playorpause);
    connect(skip_forward, &QPushButton::released, this, &mainwindow::skip_forward_action);
    connect(skip_backward, &QPushButton::released, this, &mainwindow::skip_backward_action);

    // Create button layout and set spacing
    QVBoxLayout *buttonLayout = new QVBoxLayout();
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->addStretch();
    buttonLayout->addWidget(skip_backward); 
    buttonLayout->addWidget(song_activity);
    buttonLayout->addWidget(skip_forward);
    buttonLayout->addStretch();

    // Create main layout (vertical) for the album cover, buttons, and progress bar
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Add album cover
    mainLayout->addWidget(albumCoverLabel);  // Album cover at the top

    // Add button layout (buttons are now evenly spaced)
    mainLayout->addLayout(buttonLayout);    // Button layout below album cover

    // Add progress bar at the bottom
    mainLayout->addWidget(bar); // Progress bar at the bottom

    // Central widget setup
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Timer setup for refresh functionality
    QTimer *timer = new QTimer(this);
    QTimer *refresh_the_token = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &mainwindow::loop);
    connect(refresh_the_token, &QTimer::timeout, this, &mainwindow::refresh_token);

    refresh_the_token->start(3599000);
    timer->start(1000);

    current_song = "";
    current_album = "";
}

mainwindow::~mainwindow() {}

void mainwindow::loop() {
    WorkerThread *workerThread = new WorkerThread(this->creds);
    
    connect(workerThread, &WorkerThread::resultReady, this, &mainwindow::handleResults);
    connect(workerThread, &WorkerThread::finished, workerThread, &QObject::deleteLater);
    
    workerThread->start();
}

void mainwindow::refresh_token() {
    creds.use_refresh_token();
}

void mainwindow::handleResults(const spotifypp::song result) {
    if (result.song_name != current_song) {
        this->song = result;
        //songLabel->setText(QString::fromStdString(result.song_name));
        current_song = result.song_name;
    }

    if (result.album_data != current_album) {
        QImage image;
        image.loadFromData(result.album_data);
        QPixmap pixmap = QPixmap::fromImage(image);

        albumCoverLabel->setPixmap(pixmap);
        albumCoverLabel->setFixedSize(pixmap.size());

        current_album = result.album_data;
    }
}

void mainwindow::skip_forward_action() {
    this->creds.skip_forward();
    song_activity->setIcon(*pause);
    isplaying = true;
}

void mainwindow::skip_backward_action() {
    this->creds.skip_backward();
    song_activity->setIcon(*pause);
    isplaying = true;
}

void mainwindow::playorpause() {
    if (isplaying) {
        isplaying = false;
        song_activity->setIcon(*play);
        this->creds.pause();
    } else {
        song_activity->setIcon(*pause);
        this->creds.unpause();
        isplaying = true;
    }
}

void mainwindow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_pCursor = event->globalPos() - geometry().topLeft();
        event->accept();
    }
}

void mainwindow::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        move(event->globalPosition().toPoint() - m_pCursor);
        event->accept();
    }
}
