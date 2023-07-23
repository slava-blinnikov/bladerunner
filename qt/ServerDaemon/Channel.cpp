#include "Channel.h"

Channel::Channel(qintptr descriptor, QObject *parent) :
	QObject(parent)
{
	m_descriptor = descriptor;
	m_thread = 0;
	m_worker = 0;
	m_client_type = ClientUnknown;
	m_client_id = -1;
	m_crew_id = -1;
}

ClientType Channel::clientType()
{
	return m_client_type;
}

int Channel::clientID()
{
	return m_client_id;
}

int Channel::crewID()
{
	return m_crew_id;
}

ChannelWorker *Channel::worker()
{
	return m_worker;
}

QString Channel::watcherName()
{
	return m_watcher_name;
}

void Channel::start()
{
	if (!m_thread)
	{
		m_thread = new QThread();
		m_worker = new ChannelWorker(m_descriptor);
		m_worker->moveToThread(m_thread);

		connect(m_thread, SIGNAL(started()), m_worker, SLOT(start()));
		connect(m_thread, SIGNAL(finished()), m_worker, SLOT(stop()));
		connect(m_thread, SIGNAL(finished()), this, SLOT(workerThreadFinished()));
		connect(m_worker, SIGNAL(finished()), m_thread, SLOT(quit()));
		connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
		connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));

		connect(m_worker, SIGNAL(requestChannelRolename(int,QString,int,QString)), this, SIGNAL(requestChannelRolename(int,QString,int,QString)));
		connect(this, SIGNAL(replyRolename(int,QString,int,int)), m_worker, SLOT(onReplyRolename(int,QString,int,int)));
		connect(m_worker, SIGNAL(authorized(int)), this, SIGNAL(authorized(int)));
		connect(m_worker, SIGNAL(requestMembersOnline(int)), this, SIGNAL(requestMembersOnline(int)));
		connect(this, SIGNAL(replyMembersOnline(IntList)), m_worker, SLOT(onReplyMembersOnline(IntList)));

		connect(m_worker, SIGNAL(watcherNameChanged(QString)), this, SLOT(setWatcherName(QString)));
		connect(this, SIGNAL(crewConnected(int,QString)), m_worker, SLOT(onCrewConnected(int,QString)));
		connect(this, SIGNAL(crewDisconnected(int,QString)), m_worker, SLOT(onCrewDisconnected(int,QString)));
		connect(this, SIGNAL(memberConnected(int)), m_worker, SLOT(onMemberConnected(int)));
		connect(this, SIGNAL(memberDisconnected(int)), m_worker, SLOT(onMemberDisconnected(int)));
		connect(this, SIGNAL(scheduleStart()), m_worker, SLOT(onScheduleStart()));
		connect(this, SIGNAL(scheduleStop()), m_worker, SLOT(onScheduleStop()));

		m_thread->start();
	}
}

void Channel::workerThreadFinished()
{
	m_thread = 0;
	m_worker = 0;
	emit finished(m_descriptor);
}

void Channel::setWatcherName(const QString &watcher_name)
{
	m_watcher_name = watcher_name;
}

void Channel::stop()
{
	if (m_thread) m_thread->quit(); else emit finished(m_descriptor);
}

void Channel::setRolename(int type, const QString &rolename, int crew_id, int id)
{
	m_client_type = ClientUnknown;
	m_client_id = id;
	m_crew_id = crew_id;
	m_client_type = (ClientType)type;
	emit replyRolename(type, rolename, crew_id, id);
}

void Channel::setMembersOnline(const IntList &members)
{
	emit replyMembersOnline(members);
}

void Channel::onMemberConnected(int member_id)
{
	emit memberConnected(member_id);
}

void Channel::onMemberDisconnected(int member_id)
{
	emit memberDisconnected(member_id);
}

void Channel::onCrewConnected(int descriptor, const QString &watcher_name)
{
	emit crewConnected(descriptor, watcher_name);
}

void Channel::onCrewDisconnected(int descriptor, const QString &watcher_name)
{
	emit crewDisconnected(descriptor, watcher_name);
}

void Channel::onScheduleStart()
{
	emit scheduleStart();
}

void Channel::onScheduleStop()
{
	emit scheduleStop();
}
