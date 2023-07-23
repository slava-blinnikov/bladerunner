#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QSettings>
#include <QHash>
#include <QList>

#include "Channel.h"
#include "DbScheduler.h"

class TCPServer : public QTcpServer
{
	Q_OBJECT
public:
	TCPServer(QObject *parent = 0);
	bool start();
	void stop();

protected:
	void incomingConnection(qintptr descriptor);

private:
	QHash<qintptr, Channel*> m_channels;
	DbScheduler m_db_scheduler;
	void createConnections(Channel *cnlMember, Channel *cnlCrew);

private slots:
	void getChannelRolename(int descriptor, const QString &crew, int type, const QString &name);
	void onChannelAuthorized(int descriptor);
	void getMembersOnline(int descriptor);
	void onChannelFinished(int descriptor);

public slots:
	void scheduleTimeout();
	void scheduleMembers(const IntList &started, const IntList &finished);

signals:
	void onlineMembers(const IntList &members);
};

#endif // TCPSERVER_H
