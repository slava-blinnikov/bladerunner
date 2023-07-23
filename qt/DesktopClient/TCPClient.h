#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>

#include "Resources.h"

class TCPClient : public QObject
{
	Q_OBJECT
public:
	TCPClient(QObject *parent = 0);
	~TCPClient();

private:
	QTcpSocket *m_socket;
	int m_db_index;
	QByteArray m_query_data;
	QByteArray m_buffer;
	unsigned int m_crc32;
	unsigned int m_crc32_table[256];
	bool m_keep_alive;
	QTimer *m_reconnection_timer;
	QTimer *m_heartbeat_timer;
	unsigned long m_data_length;
	unsigned char m_query_type;
	bool m_heartbeat_sended;
	bool waitStartMarker(const char *data,  unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos);
	bool processPacket(ServerQuery qtype, const char *data,  unsigned int bufflen, unsigned int &bpos, bool &processed);
	bool processPacketHeader(const char *data,  unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid);
	bool processPacketHeader(unsigned int bufflen, unsigned int &bpos);
	void send(unsigned char qtype);
	void send(unsigned char qtype, const QByteArray &payload);
	void sendAvatarQuery(ClientType ctype, int db_id);
	void handleQueryClientHeartbeat();
	void handleQueryServerHeartbeat();
	void handleQueryAuthorization();
	void handleQueryAuthorizationOk();
	void handleQueryAvatarChangeTime();
	void handleQueryAvatar();
	void handleQueryMembersList();
	void handleQueryMember();
	void handleQueryMembersOnline();
	void handleQueryMemberConnected();
	void handleQueryMemberDisconnected();
	void handleQueryBeaconState();
	void handleQueryLocation();
	void handleQuerySetTrackingPermission();
	void handleQuerySetRecordingPermission();

private slots:
	void onSocketConnected();
	void onSocketReadyRead();
	void	onSocketError(QAbstractSocket::SocketError error);
	void onSocketDisconnected();
	void heartbeat();

public slots:
	void start();
	void connectToServer();
	void stop();
	void requestStartMemberBeacon(int member_id);
	void requestStopMemberBeacon(int member_id);

signals:
	void connected();
	void disconnected();
	void avatarChanged(int client_type, int db_id);
	void memberList(const IntList &members);
	void memberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool tracking_allowed, bool tracing_allowed, int trctimeout);
	void membersOnline(const IntList &members);
	void memberConnected(int member_id);
	void memberDisconnected(int member_id);
	void memberBeaconState(int member_id, bool enabled);
	void memberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	void memberTrackingPermission(int member_id, bool enabled);
	void memberRecordingPermission(int member_id, bool enabled);
};

#endif // TCPCLIENT_H
