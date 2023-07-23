#include <QtDebug>

#include "MembersTableModel.h"

MembersTableModel::MembersTableModel(QObject *parent) : QAbstractTableModel(parent)
{
	m_noava = QIcon(":/images/no_member_avatar.png");
}

bool MembersTableModel::setData(const QModelIndex &idx, const QVariant &value, int role)
{
	Q_UNUSED(idx)
	Q_UNUSED(value)
	Q_UNUSED(role)
	return false;
}

int MembersTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return m_records.size();
}

int MembersTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent)
	return 14;
}

QVariant MembersTableModel::data(const QModelIndex &index, int role) const
{
	if (index.isValid())
	{
		if (role == Qt::DisplayRole)
		{
			const MemberRecord &rec = m_records.at(index.row());
			switch (index.column())
			{
				case 0: return QVariant::fromValue(rec.avatar);
				case 1: return QVariant(rec.online);
				case 2: return QVariant(rec.name);
				case 3: return QVariant(rec.note);
				case 4: return QVariant(rec.time.toString("yyyy-MM-dd hh:mm:ss"));
				case 5: return QVariant(QString::number(rec.latitude, 'f', 6));
				case 6: return QVariant(QString::number(rec.longitude, 'f', 6));
				case 7: return QVariant(QString::number(rec.speed, 'f', 2));
				case 8: return QVariant(rec.gps ? tr("ON") : tr("ERR"));
				case 9: return QVariant(QString::number(rec.tracking_timeout));
				case 10: return QVariant(rec.permission_tracking);
				case 11: return QVariant(rec.permission_recording);
				case 12: return QVariant(rec.tracking && rec.tracking_req);
				case 13: return QVariant(rec.recording);
			}
		}
		else if (role == Qt::TextAlignmentRole)
		{
			return QVariant(int(Qt::AlignCenter));
		}
		else if (role == MemberIdRole)
		{
			return QVariant(m_records.at(index.row()).id);
		}
	}
	return QVariant();
}

QVariant MembersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
				case 0: return QVariant(tr("AVA"));
				case 1: return QVariant(tr("Online\nstatus"));
				case 2: return QVariant(tr("Name"));
				case 3: return QVariant(tr("Note"));
				case 4: return QVariant(tr("time"));
				case 5: return QVariant(tr("latitude"));
				case 6: return QVariant(tr("longitude"));
				case 7: return QVariant(tr("speed"));
				case 8: return QVariant(tr("gps"));
				case 9: return QVariant(tr("tracking\ntimeout"));
				case 10: return QVariant(tr("tracking\nallowed"));
				case 11: return QVariant(tr("recording\nallowed"));
				case 12: return QVariant(tr("tracking"));
				case 13: return QVariant(tr("recording"));
			}
		}
	}
	return QVariant();
}

Qt::ItemFlags MembersTableModel::flags(const QModelIndex &index) const
{
	Q_UNUSED(index)
	Qt::ItemFlags flg = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	return flg;
}

void MembersTableModel::clearRows()
{
	beginResetModel();
	m_records.clear();
	endResetModel();
}

void MembersTableModel::initMemberList(const QList<int> &members)
{
	int i;
	beginResetModel();
	m_records.clear();
	for (i = 0; i < members.size(); i++)
	{
		MemberRecord rec;
		rec.id = members.at(i);
		rec.online = false;
		rec.gps = false;
		rec.latitude = 0;
		rec.longitude = 0;
		rec.speed = 0;
		rec.permission_recording = false;
		rec.permission_tracking = false;
		rec.tracking_timeout = 0;
		rec.tracking_req = false;
		rec.recording = false;
		rec.tracking = false;
		rec.avatar = m_noava;
		m_records.append(rec);
	}
	endResetModel();
	for (i = 0; i < members.size(); i++) emit requestMemberTitle(members.at(i));
	emit requestMembersOnline();
}

void MembersTableModel::setMemberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool permission_tracking, bool permission_recording, int trctimeout)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		const MemberRecord &rec = m_records.at(row);
		if (rec.id == member_id)
		{
			m_records[row].name = usrname;
			m_records[row].note = note;
			m_records[row].permission_tracking = permission_tracking;
			m_records[row].permission_recording = permission_recording;
			m_records[row].tracking_timeout = trctimeout;
			m_records[row].avatime = avatime;
			AvatarHandler ava;
			if (Resources::getAvatarHandler(&ava, Member, member_id))
			{
				if (ava.changetime.toTime_t() == avatime) m_records[row].avatar = QIcon(ava.filename);
				else emit requestMemberAvatar(member_id);
			}
			else emit requestMemberAvatar(member_id);

			emit dataChanged(index(row, 0), index(row, columnCount() - 1));
			break;
		}
	}
}

void MembersTableModel::setMembersOnline(const QList<int> &members)
{
	int n = m_records.size();
	if (n > 0)
	{
		for (int row = 0; row < n; row++)
		{
			const MemberRecord &rec = m_records.at(row);
			m_records[row].online = members.contains(rec.id );
		}
		emit dataChanged(index(0, 1), index(n - 1, 1));
	}
}

void MembersTableModel::setOnlineStatusOn(int member_id)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		const MemberRecord &rec = m_records.at(row);
		if (rec.id == member_id)
		{
			m_records[row].online = true;
			QModelIndex idx= index(row, 1);
			emit dataChanged(idx, idx);
			if (rec.tracking_req) emit requestStartMemberBeacon(member_id);
			break;
		}
	}
}

void MembersTableModel::setOnlineStatusOff(int member_id)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		const MemberRecord &rec = m_records.at(row);
		if (rec.id == member_id)
		{
			m_records[row].online = false;
			QModelIndex idx= index(row, 1);
			emit dataChanged(idx, idx);
			m_records[row].tracking = false;
			idx= index(row, 12);
			emit dataChanged(idx, idx);
			break;
		}
	}
}

void MembersTableModel::setMemberAvatar(int member_id, const QString &filename)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].avatar = QIcon(filename);
			QModelIndex idx= index(row, 0);
			emit dataChanged(idx, idx);
			break;
		}
	}
}

void MembersTableModel::setBeaconOn(int member_id)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].tracking_req = true;
			QModelIndex idx= index(row, 12);
			emit dataChanged(idx, idx);
			emit requestStartMemberBeacon(member_id);
			break;
		}
	}
}

void MembersTableModel::setBeaconOff(int member_id)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].tracking_req = false;
			QModelIndex idx= index(row, 12);
			emit dataChanged(idx, idx);
			emit requestStopMemberBeacon(member_id);
			break;
		}
	}
}

void MembersTableModel::setTrackingState(int member_id, bool state)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].tracking = state;
			QModelIndex idx= index(row, 12);
			emit dataChanged(idx, idx);
			break;
		}
	}
}

void MembersTableModel::setMemberLocation(int member_id, uint time, double latitude, double longitude, double speed, bool gps)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].time.setTime_t(time);
			m_records[row].latitude = latitude;
			m_records[row].longitude = longitude;
			m_records[row].speed = speed;
			m_records[row].gps = gps;
			emit dataChanged(index(row, 4), index(row, 8));
			break;
		}
	}
}

void MembersTableModel::setPermissionTracking(int member_id, bool state)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].permission_tracking = state;
			QModelIndex idx= index(row, 10);
			emit dataChanged(idx, idx);
			break;
		}
	}
}

void MembersTableModel::setPermissionRecording(int member_id, bool state)
{
	for (int row = 0; row < m_records.size(); row++)
	{
		if (m_records.at(row).id == member_id)
		{
			m_records[row].permission_recording = state;
			QModelIndex idx= index(row, 11);
			emit dataChanged(idx, idx);
			break;
		}
	}
}
