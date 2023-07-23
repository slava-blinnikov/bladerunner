#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QThread>

#include "ChannelWorker.h"

class Channel : public QObject
{
	Q_OBJECT
public:
	Channel(qintptr descriptor, QObject *parent = 0);
	ClientType clientType();
	int clientID();
	int crewID();
	ChannelWorker *worker();
	QString watcherName();
	void start();
	void stop();
	void setRolename(int type, const QString &rolename, int crew_id, int id);
	void setMembersOnline(const IntList &members);
	void onMemberConnected(int member_id);
	void onMemberDisconnected(int member_id);
	void onCrewConnected(int descriptor, const QString &watcher_name);
	void onCrewDisconnected(int descriptor, const QString &watcher_name);
	void onScheduleStart();
	void onScheduleStop();

private:
	qintptr m_descriptor;
	QThread *m_thread;
	ChannelWorker *m_worker;
	ClientType m_client_type;
	int m_client_id;
	int m_crew_id;
	QString m_watcher_name;

private slots:
	void workerThreadFinished();
	void setWatcherName(const QString &watcher_name);

signals:
	//-------------- сигналы для TCPServer --------------
	void requestChannelRolename(int descriptor, const QString &crew, int type, const QString &name);
	void authorized(int descriptor);
	void requestMembersOnline(int descriptor);
	void finished(int descriptor);
	//-------------- сигналы для сопряженного ChannelWorker --------------
	void replyRolename(int type, const QString &rolename, int crew_id, int id);
	void replyMembersOnline(const IntList &members);
	void memberConnected(int member_id);
	void memberDisconnected(int member_id);
	void crewConnected(int crew_dsc, const QString &watcher_name);
	void crewDisconnected(int crew_dsc, const QString &watcher_name);
	void scheduleStart();
	void scheduleStop();
};

#endif // CHANNEL_H
