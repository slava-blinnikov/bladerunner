#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "Resources.h"

class TcpClient : public QObject
{
	Q_OBJECT
public:
	TcpClient(ClientType type, const QString &crew, const QString &name, const QString &password, QObject *parent = 0);
	int dbIndex();
	~TcpClient();

protected:
	int m_db_id;
	ClientHandler m_handler;
	QByteArray m_query_data;
	QByteArray m_buffer;
	void send(unsigned char qtype);
	void send(unsigned char qtype, const QByteArray &payload);
	void sendAvatarChangetimeQuery(ClientType ctype, int db_id);
	void sendAvatarQuery(ClientType ctype, int db_id);
	virtual bool processPacket(ServerQuery qtype, const char *data,  unsigned int bufflen, unsigned int &bpos, bool &processed);
	bool processPacketHeader(const char *data,  unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid);
	bool processPacketHeader(unsigned int bufflen, unsigned int &bpos);

private:
	QTcpSocket *m_socket;
	unsigned int m_crc32_table[256];
	unsigned int m_crc32;
	unsigned long m_data_length;
	unsigned char m_query_type;
	QTimer *m_timer;
	bool m_heartbeat_sended;
	bool waitStartMarker(const char *data,  unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos);
	void sendAuthorization();
	void onAuthorizationConfirm();
	void onAvatarChangeTimeReceived();
	void onAvatarReceived();
	void onClientHeartbeat();
	void onServerHeartbeat();
	QString saveAvatar(ClientType type, int id, uint changetime, const QByteArray &ava);

private slots:
	void socketReadyRead();
	void	socketError(QAbstractSocket::SocketError error);
	void heartbeat();

public slots:
	void start();
	void stop();
	void requestAvatar();

signals:
	void connected();
	void finished();
	void authorized();
	void avatarChanged(const QString &filename);
	void memberAvatarChanged(int member_id, const QString &filename);
};

#endif // TCPCLIENT_H
