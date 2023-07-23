#ifndef TYPES
#define TYPES

#include <QString>
#include <QList>

#include "../MetaTypes/MetaTypes.h"

struct Avatar
{
	ClientType type;
	int db_index;
	unsigned int changetime;
	bool file_exists;
	Avatar &operator=(const Avatar &other)
	{
		type = other.type;
		db_index = other.db_index;
		changetime = other.changetime;
		file_exists = other.file_exists;
		return *this;
	}
};

struct MemberRecord
{
	int db_index;
	QString name;
	QString note;
	bool tracking_allowed;
	bool is_tracing;
	int tracking_period;
	bool is_online;
};

typedef QList<int> IntList;
Q_DECLARE_METATYPE(IntList);

#endif // TYPES

