#ifndef TYPES
#define TYPES

#include <QString>
#include <QVector>
#include <QDateTime>
#include <QIcon>

#include "../MetaTypes/MetaTypes.h"

struct ServerHandler
{
	QString host;
	int port;
};
typedef QVector<ServerHandler> ServerPresetList;

struct ClientHandler
{
	ClientType type;
	QString crew;
	QString name;
	QString password;
};
typedef QVector<ClientHandler> ClientPresetList;

struct AvatarHandler
{
	QString filename;
	QDateTime changetime;
};

enum MembersTableRoles
{
	MemberIdRole = Qt::UserRole + 1
};

Q_DECLARE_METATYPE (QIcon)

#endif // TYPES

