#ifndef DBSCHEDULER_H
#define DBSCHEDULER_H

#include <QObject>
#include <QThread>

#include "DbScheduleWorker.h"

class DbScheduler : public QObject
{
	Q_OBJECT
public:
	DbScheduler(QObject *parent = 0);
	void start();
	void stop();
	DbScheduleWorker *worker();

private:
	QThread *m_thread;
	DbScheduleWorker *m_worker;

signals:
	void scheduleTimeout();
};

#endif // DBSCHEDULER_H
