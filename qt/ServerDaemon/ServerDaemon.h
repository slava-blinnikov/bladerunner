#ifndef SERVERDAEMON_H
#define SERVERDAEMON_H

#include <QCoreApplication>
#include "qtservice.h"

#include "TCPServer.h"


class ServerDaemon : public QtService<QCoreApplication>
{
public:
	ServerDaemon(int argc, char **argv);

protected:
	void start();
	void stop();
	void pause();
	void resume();

private:
	TCPServer m_server;
};
#endif // SERVERDAEMON_H
