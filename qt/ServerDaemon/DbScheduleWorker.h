#ifndef DBSCHEDULEWORKER_H
#define DBSCHEDULEWORKER_H

#include <QObject>
#include <QTimer>

#include "DbConnection.h"
#include "Channel.h"

class DbScheduleWorker : public QObject
{
	Q_OBJECT
public:
	DbScheduleWorker(QObject *parent = 0);
	~DbScheduleWorker();

private:
	QTimer *m_timer;
	DbConnection *m_db;

public slots:
	void start();
	void onlineMembers(const IntList &members);

signals:
	void timeout();
	void scheduleMembers(const IntList &started, const IntList &finished);
	void finished();
};

#endif // DBSCHEDULEWORKER_H
