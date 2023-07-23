#ifndef BLRCLIENT_H
#define BLRCLIENT_H

#include <QObject>
#include <QThread>

#include "Resources.h"
#include "Types.h"
#include "TcpClient.h"

class BlrClient : public QObject
{
	Q_OBJECT
public:
	BlrClient(ClientType type, const QString &crew, const QString &name, const QString &password, QObject *parent = 0);
	~BlrClient();
	bool isConnected();
	ClientType type();
	QString crew();
	QString name();
	QString password();

protected:
	ClientHandler m_handler;
	TcpClient *m_tcpclient;
	virtual TcpClient *createTcpClient();
	virtual void createConnections();

private:
	bool m_is_running;
	QThread *m_thread;

private slots:
	void threadStarted();
	void threadFinished();

public slots:
	void start();
	void stop();

signals:
	void connected();
	void disconnected();
	void authorized();
	void needAvatar();
	void avatarChanged(const QString &filename);
	void memberAvatarChanged(int member_id, const QString &filename);
};

#endif // BLRCLIENT_H
