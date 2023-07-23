#include <QDateTime>

#include "DbScheduleWorker.h"
#include "Resources.h"
#include "qtservice.h"

DbScheduleWorker::DbScheduleWorker(QObject *parent) : QObject(parent)
{
	m_timer = 0;
	m_db = 0;
}

DbScheduleWorker::~DbScheduleWorker()
{
	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}
	if (m_db)
	{
		m_db->close();
		delete m_db;
		m_db = 0;
	}
}

void DbScheduleWorker::start()
{
	m_db = new DbConnection(Resources::db->host(), Resources::db->port(), Resources::db->databaseName(), Resources::db->user(), Resources::db->password(), "scheduler");
	if (m_db->login())
	{
		QtServiceBase::instance()->logMessage(tr("Scheduler started with timeout period = %1").arg(Resources::schedule_timeout), QtServiceBase::Information);

		m_timer = new QTimer();
		connect(m_timer, SIGNAL(timeout()), this, SIGNAL(timeout()));
		m_timer->setInterval(Resources::schedule_timeout * 1000);
		m_timer->setSingleShot(true);
		m_timer->start();
	}
	else
	{
		emit finished();
	}
}

void DbScheduleWorker::onlineMembers(const IntList &members)
{
	QtServiceBase::instance()->logMessage(tr("Scheduler timeout: %1").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")), QtServiceBase::Information);
	QSqlQuery *qry = m_db->createQuery();
	if (qry)
	{
		DbConnection::QueryKiller qk(qry);
		QHash<int, bool> is_online;
		int member;

		if (!members.isEmpty())
		{
			if (qry->prepare(Resources::queries["get_member_recording"]))
			{
				for (int i = 0; i < members.size(); i++)
				{
					member = members.at(i);
					qry->bindValue(":member", QVariant(member));
					if (m_db->exec(qry))
					{
						if (qry->next()) is_online[member] = qry->value(0).toBool();
					}
				}
			}
			qry->clear();
		}
		if (m_db->exec(qry, Resources::queries["schedule_recording"]))
		{
			qry->clear();
			if (!is_online.isEmpty())
			{
				if (qry->prepare(Resources::queries["get_member_recording"]))
				{
					IntList started;
					IntList finished;
					bool recording;
					QHashIterator<int, bool> i(is_online);
					while (i.hasNext())
					{
						i.next();
						member = i.key();
						qry->bindValue(":member", QVariant(member));
						if (m_db->exec(qry))
						{
							if (qry->next())
							{
								recording = qry->value(0).toBool();
								if (recording != i.value())
								{
									if (recording) started.append(member); else finished.append(member);
								}
							}
						}
					}
					emit scheduleMembers(started, finished);
				}
			}
		}
	}
	m_timer->start();
}

