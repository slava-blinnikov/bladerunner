#ifndef MEMBERTCPCLIENT_H
#define MEMBERTCPCLIENT_H

#include <QTimer>

#include "TcpClient.h"

class MemberTcpClient : public TcpClient
{
	Q_OBJECT
public:
	MemberTcpClient(const QString &crew, const QString &name, const QString &password, QObject *parent = 0);

protected:
	virtual bool processPacket(ServerQuery qtype, const char *data,  unsigned int bufflen, unsigned int &bpos, bool &processed);

private:
	QTimer *m_gps_timer;
	bool m_config_inited;
	bool m_tracking_allowed;
	bool m_tracing_allowed;
	int m_tracking_timeout;
	bool m_beacon;
	void onStartBeacon();
	void onStopBeacon();
	void onMemberConfig();

private slots:
	void beacon();
};

#endif // MEMBERTCPCLIENT_H
