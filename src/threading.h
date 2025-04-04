#ifndef THREADING_H
#define THREADING_H

#include "qthread.h"
#include "qtmetamacros.h"
#include "spotifypp.h"

class WorkerThread : public QThread
{
    public:
    WorkerThread(spotifypp::credentials creds);
    Q_OBJECT
    void run() override {
        spotifypp::song song(this->creds.access_token);
        emit resultReady(song);
    }
signals:
    void resultReady(const spotifypp::song song);
    public:
        spotifypp::credentials creds;

};


#endif //THREADING_H