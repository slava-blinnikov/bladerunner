#ifndef CREWCLIENT_H
#define CREWCLIENT_H

#include "BlrClient.h"

class CrewClient : public BlrClient
{
	Q_OBJECT
public:
	CrewClient(const QString &name, const QString &password, QObject *parent = 0);

protected:
	virtual TcpClient *createTcpClient();
	virtual void createConnections();

signals:
	void requestMemberAvatar(int member_id);
	void memberList(const QList<int> &members);
	void requestMemberTitle(int member_id);
	void memberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool tracking_allowed, bool tracing_allowed, int trctimeout);
	void requestMembersOnline();
	void membersOnline(const QList<int> &members);
	void memberConnected(int member_id);
	void memberDisconnected(int member_id);
	void startMemberBeacon(int member_id);
	void stopMemberBeacon(int member_id);
	void memberBeaconState(int member_id, bool enabled);
	void memberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	void memberTrackingPermission(int member_id, bool enabled);
	void memberRecordingPermission(int member_id, bool enabled);
};

#endif // CREWCLIENT_H
