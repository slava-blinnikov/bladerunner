#include <QSettings>
#include <QDir>

#include "ServerDaemon.h"

int main(int argc, char **argv)
{
	QSettings::setPath(QSettings::NativeFormat, QSettings::SystemScope, QString("%1/.config").arg(QDir::homePath()));
	ServerDaemon daemon(argc, argv);
	return daemon.exec();
}
