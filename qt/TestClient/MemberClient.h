#ifndef MEMBERCLIENT_H
#define MEMBERCLIENT_H

#include "BlrClient.h"

class MemberClient : public BlrClient
{
	Q_OBJECT
public:
	MemberClient(const QString &crew, const QString &name, const QString &password, QObject *parent = 0);

protected:
	virtual TcpClient *createTcpClient();
	virtual void createConnections();
};

#endif // MEMBERCLIENT_H
