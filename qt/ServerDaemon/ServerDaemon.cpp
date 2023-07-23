#include "ServerDaemon.h"
#include "Resources.h"

ServerDaemon::ServerDaemon(int argc, char **argv) : QtService<QCoreApplication>(argc, argv, "Bladerunner-Server")
{
	setServiceDescription("Bladerunner GeoTracking Server");
	setServiceFlags(QtServiceBase::Default | QtServiceBase::NeedsStopOnShutdown);
	setStartupType(QtServiceController::AutoStartup);
}

void ServerDaemon::start()
{
	QString cfg_filename = application()->applicationFilePath();
#ifdef Q_OS_UNIX
	cfg_filename += ".xml";
#else
	cfg_filename.replace(".exe", ".xml");
#endif
	bool ok = Resources::init(cfg_filename);
	if (ok)
	{
		if (Resources::db->login())
		{
			m_server.start();
			QtServiceBase::instance()->logMessage(QString("Service started."), QtServiceBase::Information);
			return;
		}
	}
	application()->quit();
}

void ServerDaemon::stop()
{
	Resources::clear();
	m_server.stop();
	application()->quit();
	QtServiceBase::instance()->logMessage(QString("Service stopped."), QtServiceBase::Information);
}

void ServerDaemon::pause()
{
	m_server.stop();
}

void ServerDaemon::resume()
{
	m_server.start();
}
