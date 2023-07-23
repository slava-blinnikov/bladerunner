#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

class DbConnection : public QObject
{
	Q_OBJECT
public:
	class QueryKiller
	{
	public:
		QueryKiller(QSqlQuery* query) :m_query(query){}
		~QueryKiller(){if (m_query) delete m_query;}
	private:
		QSqlQuery* m_query;
	};
	DbConnection(const QString &host, int port, const QString &db_name, const QString &user, const QString &password, const QString &cname, QObject *parent = 0);
	~DbConnection();
	bool login();
	void close();
	bool exec(QSqlQuery *query);
	bool exec(QSqlQuery *query, const QString &sql);
	QSqlQuery *createQuery();
	QSqlDatabase *database();
	QString host();
	int port();
	QString databaseName();
	QString user();
	QString password();

private:
	QSqlDatabase m_database;
	QString m_connection_name;

signals:
	void connectionLost();
};

#endif // DBCONNECTION_H
