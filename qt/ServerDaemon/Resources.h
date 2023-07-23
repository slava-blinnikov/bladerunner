#ifndef RESOURCES_H
#define RESOURCES_H

#include <QtGlobal>
#include <QMap>
#include <QString>
#include "DbConnection.h"

class Resources
{
public:
	static qint16 server_port;
	static DbConnection *db;
	static int login_timeout;
	static int ping_timeout;
	static int schedule_timeout;
	static QMap<QString, QString> queries;
	static QString www_path;
	static bool init(const QString &cfg_filename);
	static void clear();
};

#endif // RESOURCES_H
