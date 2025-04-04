#include "threading.h"

WorkerThread::WorkerThread(spotifypp::credentials creds)
{
    this->creds = creds;
}