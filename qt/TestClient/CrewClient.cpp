#include "CrewClient.h"
#include "CrewTcpClient.h"

CrewClient::CrewClient(const QString &name, const QString &password, QObject *parent) : BlrClient(Crew, name, name, password, parent)
{
}

TcpClient *CrewClient::createTcpClient()
{
	return new CrewTcpClient(m_handler.name, m_handler.password);
}

void CrewClient::createConnections()
{
	BlrClient::createConnections();
	CrewTcpClient *tcpclient = qobject_cast<CrewTcpClient *>(m_tcpclient);
	connect(this, SIGNAL(requestMemberAvatar(int)), tcpclient, SLOT(requestMemberAvatar(int)));
	connect(tcpclient, SIGNAL(memberAvatarChanged(int,QString)), this, SIGNAL(memberAvatarChanged(int,QString)));
	connect(tcpclient, SIGNAL(memberList(QList<int>)), this, SIGNAL(memberList(QList<int>)));
	connect(tcpclient, SIGNAL(memberTitle(int,QString,QString,uint,bool,bool,int)), this, SIGNAL(memberTitle(int,QString,QString,uint,bool,bool,int)));
	connect(this, SIGNAL(requestMemberTitle(int)), tcpclient, SLOT(requestMember(int)));
	connect(tcpclient, SIGNAL(membersOnline(QList<int>)), this, SIGNAL(membersOnline(QList<int>)));
	connect(this, SIGNAL(requestMembersOnline()), tcpclient, SLOT(requestMembersOnline()));
	connect(tcpclient, SIGNAL(memberConnected(int)), this, SIGNAL(memberConnected(int)));
	connect(tcpclient, SIGNAL(memberDisconnected(int)), this, SIGNAL(memberDisconnected(int)));
	connect(this, SIGNAL(startMemberBeacon(int)), tcpclient, SLOT(requestStartMemberBeacon(int)));
	connect(this, SIGNAL(stopMemberBeacon(int)), tcpclient, SLOT(requestStopMemberBeacon(int)));
	connect(tcpclient, SIGNAL(memberBeaconState(int,bool)), this, SIGNAL(memberBeaconState(int,bool)));
	connect(tcpclient, SIGNAL(memberLocationChanged(int,uint,double,double,double,bool)), this, SIGNAL(memberLocationChanged(int,uint,double,double,double,bool)));
	connect(tcpclient, SIGNAL(memberTrackingPermission(int,bool)), this, SIGNAL(memberTrackingPermission(int,bool)));
	connect(tcpclient, SIGNAL(memberRecordingPermission(int,bool)), this, SIGNAL(memberRecordingPermission(int,bool)));
}
