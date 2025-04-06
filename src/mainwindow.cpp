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
#include <QColor>
#include <QDebug>
#include <QHBoxLayout>
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>


using namespace std;

mainwindow::mainwindow(spotifypp::credentials creds, QWidget *parent)
    : QMainWindow(parent) {
  this->creds = creds;

  // Create and configure your progress bar
  bar = new QProgressBar(this);
  bar->setRange(0, 100);
  bar->setFixedHeight(5); // Fixed height for the progress bar
  bar->setStyleSheet("QProgressBar {"
                     "border-radius: 0px;"
                     "border: 0.5px solid black;"
                     "background-color: white;"
                     "}"
                     "QProgressBar::chunk {"
                     "background-color: #4CAF50;"
                     "}");

  bar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  albumCoverLabel = new QLabel(this);
  albumCoverLabel->setScaledContents(true);
  albumCoverLabel->setFixedSize(64, 64);

  skip_forward_icon = QIcon(":/images/skip_forward.png");
  skip_forward = new QPushButton(this);
  skip_forward->setIcon(skip_forward_icon);
  skip_forward->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

  skip_backward_icon = QIcon(":/images/skip_backward.png");
  skip_backward = new QPushButton(this);
  skip_backward->setIcon(skip_backward_icon);
  skip_backward->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

  // Setup play/pause button
  pause = QIcon(":/images/pause.png");
  play = QIcon(":/images/play.png");
  song_activity = new QPushButton();
  song_activity->setStyleSheet("QPushButton {border-style: outset; border-width: 0px;}");

  song_activity->setIcon(play);

  connect(song_activity, &QPushButton::released, this,
          &mainwindow::playorpause);
  connect(skip_forward, &QPushButton::released, this,
          &mainwindow::skip_forward_action);
  connect(skip_backward, &QPushButton::released, this,
          &mainwindow::skip_backward_action);

  QVBoxLayout *buttonLayout = new QVBoxLayout();
  buttonLayout->setContentsMargins(0, 0, 0, 0);
  buttonLayout->addWidget(song_activity, 0, Qt::AlignVCenter);
  buttonLayout->addWidget(skip_forward, 0, Qt::AlignVCenter);
  buttonLayout->addWidget(skip_backward, 0, Qt::AlignVCenter);

  QGridLayout *mainLayout = new QGridLayout();
  mainLayout->setContentsMargins(0, 0, 0, 0);

  mainLayout->addWidget(albumCoverLabel, 0, 0);

  mainLayout->addLayout(buttonLayout, 0, 1, Qt::AlignRight);


  mainLayout->addWidget(bar, 1, 0, 1, 2);

  QWidget *centralWidget = new QWidget(this);
  centralWidget->setLayout(mainLayout);
  centralWidget->setStyleSheet("border: 1px solid black");
  setCentralWidget(centralWidget);

  // this timer calls a very poorly made thread that polls the spotify servers for up to date information
  QTimer *timer = new QTimer(this);
  QTimer *refresh_the_token = new QTimer(this);
  connect(timer, &QTimer::timeout, this, &mainwindow::loop);
  connect(refresh_the_token, &QTimer::timeout, this,
          &mainwindow::refresh_token);

  refresh_the_token->start(3599000);
  timer->start(1000);
}

mainwindow::~mainwindow() {}

void mainwindow::loop() {
  WorkerThread *workerThread = new WorkerThread(this->creds);

  connect(workerThread, &WorkerThread::resultReady, this,
          &mainwindow::handleResults);
  connect(workerThread, &WorkerThread::finished, workerThread,
          &QObject::deleteLater);

  workerThread->start();
}

void mainwindow::refresh_token() { creds.use_refresh_token(); }

void mainwindow::handleResults(const spotifypp::song result) {
  
  if (result.song_name != current_song) {
    song = result;
    // songLabel->setText(QString::fromStdString(result.song_name));
    current_song = result.song_name;
  }

  if (result.is_playing  == true)
  {
    this->bar->setValue((result.song_current_lenght / result.song_lenght) * 100);
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
  creds.skip_forward();
  song_activity->setIcon(pause);
  isplaying = true;
}

void mainwindow::skip_backward_action() {
  creds.skip_backward();
  song_activity->setIcon(pause);
  isplaying = true;
}

void mainwindow::playorpause() {
  if (isplaying) {
    isplaying = false;
    song_activity->setIcon(play);
    creds.pause();
  } else {
    song_activity->setIcon(pause);
    creds.unpause();
    isplaying = true;
  }
}

void mainwindow::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    m_pCursor = event->globalPos() - geometry().topLeft();
    event->accept();
  }
}

void mainwindow::mouseMoveEvent(QMouseEvent *event) {
  if (event->buttons() & Qt::LeftButton) {
    move(event->globalPosition().toPoint() - m_pCursor);
    event->accept();
  }
}
