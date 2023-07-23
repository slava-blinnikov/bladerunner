#include "TCPServer.h"
#include "Resources.h"

#include "qtservice.h"

TCPServer::TCPServer(QObject *parent) : QTcpServer(parent)
{
	qRegisterMetaType<IntList>("IntList");
}

void TCPServer::incomingConnection(qintptr descriptor)
{
	QtServiceBase::instance()->logMessage(tr("Incoming connection %1").arg(descriptor), QtServiceBase::Information);

	Channel *cnannel = new Channel(descriptor);
	connect(cnannel, SIGNAL(requestChannelRolename(int,QString,int,QString)), this, SLOT(getChannelRolename(int,QString,int,QString)));
	connect(cnannel, SIGNAL(authorized(int)), this, SLOT(onChannelAuthorized(int)));
	connect(cnannel, SIGNAL(finished(int)), this, SLOT(onChannelFinished(int)));
	m_channels[descriptor] = cnannel;
	cnannel->start();
}

void TCPServer::createConnections(Channel *cnlMember, Channel *cnlCrew)
{
	ChannelWorker *wrkMember = cnlMember->worker();
	ChannelWorker *wrkCrew = cnlCrew->worker();
	connect(wrkMember, SIGNAL(locationChanged(int,uint,double,double,double,bool)), wrkCrew, SLOT(onMemberLocationChanged(int,uint,double,double,double,bool)));
	connect(wrkCrew, SIGNAL(startBeaconRequest(int,int,QString)), wrkMember, SLOT(startBeacon(int,int,QString)));
	connect(wrkCrew, SIGNAL(stopBeaconRequest(int,int,QString)), wrkMember, SLOT(stopBeacon(int,int,QString)));
	connect(wrkMember, SIGNAL(beaconStateChanged(int,bool)), wrkCrew, SLOT(memberBeaconStateChanged(int,bool)));
	connect(wrkMember, SIGNAL(trackingPermissionChanged(int,bool)), wrkCrew, SLOT(memberTrackingPermissionChanged(int,bool)));
	connect(wrkMember, SIGNAL(recordingPermissionChanged(int,bool)), wrkCrew, SLOT(memberRecordingPermissionChanged(int,bool)));
}

void TCPServer::getChannelRolename(int descriptor, const QString &crew, int type, const QString &name)
{
	QtServiceBase::instance()->logMessage(tr("Connection %1 request rolename").arg(descriptor), QtServiceBase::Information);

	if (m_channels.contains(descriptor))
	{
		QString rolename;
		QSqlQuery *qry = Resources::db->createQuery();
		int crew_id = -1;
		int id = -1;
		if (qry)
		{
			DbConnection::QueryKiller qk(qry);
			if (type == Crew)
			{
				if (qry->prepare(Resources::queries["get_crew_role"]))
				{
					qry->bindValue(":crew_name", crew);
					if (Resources::db->exec(qry))
					{
						if (qry->next())
						{
							rolename = qry->value(0).toString();
							id = qry->value(1).toInt();
							crew_id = id;
						}
					}
				}
			}
			else if (type == Member)
			{
				if (qry->prepare(Resources::queries["get_member_role"]))
				{
					qry->bindValue(":crew_name", crew);
					qry->bindValue(":member_name", name);
					if (Resources::db->exec(qry))
					{
						if (qry->next())
						{
							rolename = qry->value(0).toString();
							id = qry->value(1).toInt();
							crew_id = qry->value(2).toInt();
						}
					}
				}
			}
		}
		if (id >= 0)
		{
			Channel *channel = m_channels[descriptor];
			channel->setRolename(type, rolename, crew_id, id);
			disconnect(channel, SIGNAL(requestChannelRolename(int,QString,int,QString)), this, SLOT(getChannelRolename(int,QString,int,QString)));
		}
	}
}

void TCPServer::onChannelAuthorized(int descriptor)
{
	if (m_channels.contains(descriptor))
	{
		Channel *channel = m_channels[descriptor];
		int new_id = channel->clientID();
		if (channel->clientType() == Crew)
		{
			connect(channel, SIGNAL(requestMembersOnline(int)), this, SLOT(getMembersOnline(int)));
			Channel *icnl;
			QHashIterator<qintptr, Channel*> i(m_channels);
			while (i.hasNext())
			{
				i.next();
				if (i.key() != descriptor)
				{
					icnl = i.value();
					if (icnl->clientType() == Member && icnl->crewID() == new_id)
					{
						createConnections(icnl, channel);
						icnl->onCrewConnected(descriptor, channel->watcherName());
					}
				}
			}
		}
		else if (channel->clientType() == Member)
		{
			Channel *icnl;
			QHashIterator<qintptr, Channel*> i(m_channels);
			while (i.hasNext())
			{
				i.next();
				if (i.key() != descriptor)
				{
					icnl = i.value();
					if (icnl->clientType() == Member)
					{
						int id = icnl->clientID();
						if (id >= 0 && id == new_id) icnl->stop();
					}
					else if (icnl->clientType() == Crew && icnl->clientID() == channel->crewID())
					{
						createConnections(channel, icnl);
						icnl->onMemberConnected(new_id);
					}
				}
			}
		}
		disconnect(channel, SIGNAL(authorized(int)), this, SLOT(onChannelAuthorized(int)));
	}
}

void TCPServer::getMembersOnline(int descriptor)
{
	if (m_channels.contains(descriptor))
	{
		Channel *channel = m_channels[descriptor];
		if (channel->clientType() == Crew)
		{
			int crew_id = channel->clientID();
			IntList members;
			QHashIterator<qintptr, Channel*> i(m_channels);
			while (i.hasNext())
			{
				i.next();
				Channel *cnl = i.value();
				if (cnl->clientType() ==  Member && cnl->crewID() == crew_id) members.append(cnl->clientID());
			}
			channel->setMembersOnline(members);
		}
	}
}

void TCPServer::onChannelFinished(int descriptor)
{
	QtServiceBase::instance()->logMessage(tr("Connection %1 closed").arg(descriptor), QtServiceBase::Information);

	Channel *channel = m_channels[descriptor];
	m_channels.remove(descriptor);
	if (channel->clientType() == Member)
	{
		int crew_id = channel->crewID();
		bool replacement = false;
		QHashIterator<qintptr, Channel*> i(m_channels);
		while (i.hasNext())
		{
			i.next();
			Channel *icnl = i.value();
			if (icnl->clientType() == Member && icnl->crewID() == crew_id && icnl->clientID() == channel->clientID())
			{
				replacement = true;
				break;
			}
		}
		if (!replacement)
		{
			i.toFront();
			while (i.hasNext())
			{
				i.next();
				Channel *icnl = i.value();
				if (icnl->clientType() == Crew && icnl->clientID() == crew_id) icnl->onMemberDisconnected(channel->clientID());
			}
		}
	}
	else if (channel->clientType() == Crew)
	{
		QHashIterator<qintptr, Channel*> i(m_channels);
		while (i.hasNext())
		{
			i.next();
			Channel *icnl = i.value();
			if (icnl->clientType() == Member && icnl->crewID() == channel->clientID()) icnl->onCrewDisconnected(descriptor, channel->watcherName());
		}
	}
	channel->deleteLater();
}

void TCPServer::scheduleTimeout()
{
	IntList members;
	QHashIterator<qintptr, Channel*> i(m_channels);
	while (i.hasNext())
	{
		i.next();
		Channel *icnl = i.value();
		if (icnl->clientType() == Member) members.append(icnl->clientID());
	}
	emit onlineMembers(members);
}

void TCPServer::scheduleMembers(const IntList &started, const IntList &finished)
{
	QHashIterator<qintptr, Channel*> i(m_channels);
	while (i.hasNext())
	{
		i.next();
		Channel *icnl = i.value();
		if (icnl->clientType() == Member)
		{
			if (started.contains(icnl->clientID()))
			{
				icnl->onScheduleStart();
			}
			else if (finished.contains(icnl->clientID()))
			{
				icnl->onScheduleStop();
			}
		}
	}
}

bool TCPServer::start()
{
	bool ok = listen(QHostAddress::Any, Resources::server_port);
	if (ok)
	{
		DbScheduleWorker *sch_worker = m_db_scheduler.worker();
		connect(sch_worker, SIGNAL(timeout()), this, SLOT(scheduleTimeout()));
		connect(this, SIGNAL(onlineMembers(IntList)), sch_worker, SLOT(onlineMembers(IntList)));
		connect(sch_worker, SIGNAL(scheduleMembers(IntList,IntList)), this, SLOT(scheduleMembers(IntList,IntList)));
		m_db_scheduler.start();
	}
	return ok;
}

void TCPServer::stop()
{
	m_db_scheduler.stop();
	close();
	QHashIterator<qintptr, Channel*> i(m_channels);
	while (i.hasNext())
	{
		i.next();
		i.value()->stop();
	}
	m_channels.clear();
}
