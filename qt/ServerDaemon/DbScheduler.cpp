#include "DbScheduler.h"

DbScheduler::DbScheduler(QObject *parent) : QObject(parent)
{
	m_thread = new QThread();
	m_worker = new DbScheduleWorker();
	m_worker->moveToThread(m_thread);
	connect(m_thread, SIGNAL(started()), m_worker, SLOT(start()));
	connect(m_worker, SIGNAL(finished()), m_thread, SLOT(quit()));
	connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
	connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
}

void DbScheduler::start()
{
	m_thread->start();
}

void DbScheduler::stop()
{
	if (m_thread)
	{
		m_thread->quit();
		m_thread->wait();
	}
}

DbScheduleWorker *DbScheduler::worker()
{
	return m_worker;
}

