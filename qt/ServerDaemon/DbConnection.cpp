#include "DbConnection.h"
#include "qtservice.h"

DbConnection::DbConnection(const QString &host, int port, const QString &db_name, const QString &user, const QString &password, const QString &cname, QObject *parent) : QObject(parent)
{
	m_connection_name = QString("bladerunner-%1-%2-%3").arg(user).arg((quint64)this).arg(cname);
	if (!QSqlDatabase::contains(m_connection_name)) m_database = QSqlDatabase::addDatabase("QPSQL", m_connection_name);
		else m_database = QSqlDatabase::database(m_connection_name, false);
	if (m_database.isValid())
	{
		m_database.setDatabaseName(db_name);
		m_database.setHostName(host);
		m_database.setPort(port);
		m_database.setUserName(user);
		m_database.setPassword(password);
	}
}

DbConnection::~DbConnection()
{
	m_database = QSqlDatabase();
	QSqlDatabase::removeDatabase(m_connection_name);
}

bool DbConnection::login()
{
	bool ok = false;
	if (m_database.isValid())
	{
		ok = m_database.open();
		QtServiceBase::instance()->logMessage(QString("Database %1 at %2:%3 with user '%4' pass '%5' %6\nconnection name '%7'")
						      .arg(m_database.databaseName())
						      .arg(m_database.hostName())
						      .arg(m_database.port())
						      .arg(m_database.userName())
						      .arg(m_database.password())
						      .arg(ok ? "succesfully opened." : "opening failure!")
						      .arg(m_connection_name),
						      QtServiceBase::Information);
	}
	return ok;
}

void DbConnection::close()
{
	if (m_database.isValid()) m_database.close();
}

bool DbConnection::exec(QSqlQuery *query)
{
	if (!query->exec())
	{
		if (query->lastError().type() == QSqlError::ConnectionError)
		{
			m_database.close();
			emit connectionLost();
		}
		return false;
	}
	return true;
}

bool DbConnection::exec(QSqlQuery *query, const QString &sql)
{
	if (!query->exec(sql))
	{
		if (query->lastError().type() == QSqlError::ConnectionError)
		{
			m_database.close();
			emit connectionLost();
		}
		return false;
	}
	return true;
}

QSqlQuery *DbConnection::createQuery()
{
	QSqlQuery *ret = 0;
	if (m_database.isOpen()) ret = new QSqlQuery(m_database);
	return ret;
}

QSqlDatabase *DbConnection::database()
{
	return &m_database;
}

QString DbConnection::host()
{
	return m_database.hostName();
}

int DbConnection::port()
{
	return m_database.port();
}

QString DbConnection::databaseName()
{
	return  m_database.databaseName();
}

QString DbConnection::user()
{
	return  m_database.userName();
}

QString DbConnection::password()
{
	return  m_database.password();
}
