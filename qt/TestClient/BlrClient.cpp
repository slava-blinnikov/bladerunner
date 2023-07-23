#include <QtDebug>

#include "BlrClient.h"

BlrClient::BlrClient(ClientType type, const QString &crew, const QString &name, const QString &password, QObject *parent) : QObject(parent)
{
	m_thread = 0;
	m_tcpclient = 0;
	m_is_running = false;
	m_handler.type = type;
	m_handler.crew = crew;
	m_handler.name = name;
	m_handler.password = password;
}

BlrClient::~BlrClient()
{
	stop();
}

bool BlrClient::isConnected()
{
	return m_is_running;
}

ClientType BlrClient::type()
{
	return m_handler.type;
}

QString BlrClient::crew()
{
	return m_handler.crew;
}

QString BlrClient::name()
{
	return m_handler.name;
}

QString BlrClient::password()
{
	return m_handler.password;
}

TcpClient *BlrClient::createTcpClient()
{
	return new TcpClient(m_handler.type, m_handler.crew, m_handler.name, m_handler.password);
}

void BlrClient::createConnections()
{
	connect(m_thread, SIGNAL(started()), m_tcpclient, SLOT(start()));
	connect(m_tcpclient, SIGNAL(finished()), m_thread, SLOT(quit()));
	connect(m_thread, SIGNAL(finished()), m_tcpclient, SLOT(stop()));
	connect(m_thread, SIGNAL(started()), this, SLOT(threadStarted()));
	connect(m_thread, SIGNAL(finished()), this, SLOT(threadFinished()));
	connect(m_tcpclient, SIGNAL(finished()), m_tcpclient, SLOT(deleteLater()));
	connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
	connect(m_tcpclient, SIGNAL(connected()), this, SIGNAL(connected()));
	connect(m_tcpclient, SIGNAL(authorized()), this, SIGNAL(authorized()));
	connect(m_thread, SIGNAL(finished()), this, SIGNAL(disconnected()));
	connect(this, SIGNAL(needAvatar()), m_tcpclient, SLOT(requestAvatar()));
	connect(m_tcpclient, SIGNAL(avatarChanged(QString)), this, SIGNAL(avatarChanged(QString)));
}

void BlrClient::threadStarted()
{
	m_is_running = true;
}

void BlrClient::threadFinished()
{
	m_is_running = false;
	m_tcpclient = 0;
	m_thread = 0;
}

void BlrClient::start()
{
	if (!m_is_running)
	{
		m_thread = new QThread();
		m_tcpclient = createTcpClient();
		m_tcpclient->moveToThread(m_thread);
		createConnections();
		m_thread->start();
	}
}

void BlrClient::stop()
{
	if (m_is_running)
	{
		m_thread->quit();
		m_thread->wait();
	}
}
