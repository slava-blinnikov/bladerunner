#ifndef RESOURCES_H
#define RESOURCES_H

#include <QRect>
#include <QVector>
#include <QDateTime>

#include "ProgramConfig.h"
#include "Types.h"

class Resources
{
public:
	static ProgramConfig *config;
	static QString host;
	static quint16 port;
	static QString avatars_path;
	static QRect window_geometry;
	static Qt::WindowState window_state;
	static QString user_name;
	static QString password;
	static QString watcher_name;
	static unsigned int reconnection_interval;
	static QVector<Avatar> avatars;
	static int current_crew;
	static int current_member;
	static QString current_member_name;
	static void init(ProgramConfig *cfg);
	static void setHost(const QString &hst, int prt);
	static void setLogon(const QString &usrname, const QString &pass);
	static void setWatcherName(const QString &watcher);
	static void setAvatarsPath(const QString &path);
	static void setWindowState(const QRect &g, Qt::WindowState ste);
	static void setCurrentMember(int id, const QString &name);
	static void setCrewID(int id);
	static Avatar findAvatar(ClientType type, int db_id);
	static bool saveAvatar(ClientType type, int db_id, uint changetime, const QByteArray &data);
	static void clearAvatars();
};

#endif // RESOURCES_H
