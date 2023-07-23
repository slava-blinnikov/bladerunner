#ifndef CHANNELWORKER_H
#define CHANNELWORKER_H

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QList>

#include "../MetaTypes/MetaTypes.h"
#include "DbConnection.h"

typedef QList<int> IntList;

class ChannelWorker : public QObject
{
	Q_OBJECT
public:
	ChannelWorker(qintptr descriptor, QObject *parent = 0);
	~ChannelWorker();
	ClientType clientType();
	QString clientCrew();
	QString clientName();
	int clientID();

private:
	DbConnection *m_db;
	QTcpSocket *m_socket;
	QByteArray m_buffer;
	QByteArray m_query_data;
	unsigned long m_data_length;
	unsigned int m_crc32_table[256];
	unsigned int m_crc32;
	qintptr m_socket_descriptor;
	int m_db_id;
	ClientType m_type;
	QString m_crew;
	QString m_name;
	QString m_watcher_name;
	QString m_rolename;
	QString m_group_rolename;
	QString m_client_password;
	QTimer *m_timer;
	IntList m_crews;
	int m_tracking_timeout;
	bool m_perm_tracking;
	bool m_perm_recording;
	bool m_ping_sended;
	bool m_recording;
	bool send(ServerQuery qry);
	bool send(ServerQuery qry, const QByteArray &payload);
	bool waitStartMarker(const char *data,  unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos);
	bool processPacketHeader(const char *data,  unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid);
	bool processPacketHeader(unsigned int bufflen, unsigned int &bpos);
	void clientAuthorization();
	void clientHeartbeat();
	void clientAvatarChangetime();
	void clientAvatar();
	void clientMembersList();
	void clientMember();
	void clientMembersOnline();
	void clientStartBeacon();
	void clientStopBeacon();
	void clientBeaconState();
	void clientMemberConfig();
	void clientLocation();
	void clientSetTracking();
	void clientSetRecording();
	void writeToDatabase(uint time, double latitude, double longitude, double speed, bool gps);

private slots:
	void socketReadyRead();
	void heartbeatTimer();

public slots:
	void start();
	void stop();
	void onReplyRolename(int type, const QString &rolename, int crew_id, int id);
	void onReplyMembersOnline(const IntList &members);
	//--------------------------------------------------------
	//вызываются у канала типа Crew при входе или выходе очередного Member в сеть (сигналы инициируются сервером)
	void onMemberConnected(int member_id);
	void onMemberDisconnected(int member_id);
	//вызываются у канала типа Member при выходе наблюдающего за ним канала Crew (сигнал инициируются сервером)
	void onCrewConnected(int crew_dsc, const QString &watcher_name);
	void onCrewDisconnected(int crew_dsc, const QString &watcher_name);
	//вызываетсяу канала типа Crew когда канал типа Member прислал свои координаты
	void onMemberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	//вызываются у канала типа Member по запросу от канала Crew
	void startBeacon(int crew_dsc, int member_id, const QString &watcher_name);
	void stopBeacon(int crew_dsc, int member_id, const QString &watcher_name);
	//вызывается у канала типа Crew когда канал Member изменил состояние своего маячка
	void memberBeaconStateChanged(int member_id, bool state);
	//вызывается у канала типа Crew когда канал Member изменил разрешение на слежение
	void memberTrackingPermissionChanged(int member_id, bool state);
	//вызывается у канала типа Crew когда канал Member изменил разрешение на запись
	void memberRecordingPermissionChanged(int member_id, bool state);
	//вызываются у канала типа Member при старте или останове маячка по расписанию (сигнал инициируются сервером)
	void onScheduleStart();
	void onScheduleStop();

signals:
	void finished();
	void requestChannelRolename(int descriptor, const QString &crew, int type, const QString &name);
	void watcherNameChanged(const QString &watcher_name);
	void authorized(int desc);
	void requestMembersOnline(int descriptor);
	//--------------------------------------------------------
	//вызывается каналом типа Member по успешному ответу gps датчика
	void locationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	//вызывается каналом типа Crew при поступлении запроса на начало слежения за указанным Member (member_id)
	void startBeaconRequest(int crew_dsc, int member_id, const QString &watcher_name);
	void stopBeaconRequest(int crew_dsc, int member_id, const QString &watcher_name);
	//вызывается каналом типа Member при изменении состояния маячка (вкл/выкл)
	void beaconStateChanged(int member_id, bool state);
	//вызывается каналом типа Member при изменении разрешения на слежение
	void trackingPermissionChanged(int member_id, bool state);
	//вызывается каналом типа Member при изменении разрешения на запись
	void recordingPermissionChanged(int member_id, bool state);
};

#endif // CHANNELWORKER_H
