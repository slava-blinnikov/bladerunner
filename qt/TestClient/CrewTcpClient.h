#ifndef CREWTCPCLIENT_H
#define CREWTCPCLIENT_H

#include "TcpClient.h"

class CrewTcpClient : public TcpClient
{
	Q_OBJECT
public:
	CrewTcpClient(const QString &name, const QString &password, QObject *parent = 0);

protected:
	virtual bool processPacket(ServerQuery qtype, const char *data,  unsigned int bufflen, unsigned int &bpos, bool &processed);

private:
	void onMembersList();
	void onMember();
	void onMembersOnline();
	void onMemberConnected();
	void onMemberDisconnected();
	void onMemberBeaconState();
	void onMemberLocation();
	void onMemberSetTrackingPermission();
	void onMemberSetRecordingPermission();

public slots:
	void requestMemberAvatar(int member_id);
	void requestMember(int member_id);
	void requestMembersOnline();
	void requestStartMemberBeacon(int member_id);
	void requestStopMemberBeacon(int member_id);

signals:
	void memberList(const QList<int> &members);
	void memberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool tracking_allowed, bool tracing_allowed, int trctimeout);
	void membersOnline(const QList<int> &members);
	void memberConnected(int member_id);
	void memberDisconnected(int member_id);
	void memberBeaconState(int member_id, bool enabled);
	void memberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	void memberTrackingPermission(int member_id, bool enabled);
	void memberRecordingPermission(int member_id, bool enabled);
};

#endif // CREWTCPCLIENT_H
