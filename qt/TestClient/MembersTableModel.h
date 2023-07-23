#ifndef MEMBERSTABLEMODEL_H
#define MEMBERSTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QDateTime>
#include <QIcon>

#include "Resources.h"

class MembersTableModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	struct MemberRecord
	{
		int id;
		bool online;
		QString name;
		QString note;
		uint avatime;
		QDateTime time;
		float latitude;
		float longitude;
		float speed;
		bool gps;
		int tracking_timeout;
		bool permission_tracking;
		bool permission_recording;
		bool tracking_req;
		bool tracking;
		bool recording;
		QIcon avatar;
	};
	MembersTableModel(QObject *parent = 0);
	bool setData(const QModelIndex &idx, const QVariant &value, int role);
	int rowCount(const QModelIndex &parent = QModelIndex()) const;
	int columnCount(const QModelIndex &parent = QModelIndex()) const;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role) const;
	Qt::ItemFlags flags(const QModelIndex &index) const;

private:
	QVector<MemberRecord> m_records;
	QIcon m_noava;

public slots:
	void clearRows();
	void initMemberList(const QList<int> &members);
	void setMemberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool permission_tracking, bool permission_recording, int trctimeout);
	void setMembersOnline(const QList<int> &members);
	void setOnlineStatusOn(int member_id);
	void setOnlineStatusOff(int member_id);
	void setMemberAvatar(int member_id, const QString &filename);
	void setBeaconOn(int member_id);
	void setBeaconOff(int member_id);
	void setTrackingState(int member_id, bool state);
	void setMemberLocation(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	void setPermissionTracking(int member_id, bool state);
	void setPermissionRecording(int member_id, bool state);

signals:
	void requestMemberTitle(int member_id);
	void requestMembersOnline();
	void requestMemberAvatar(int member_id);
	void requestStartMemberBeacon(int member_id);
	void requestStopMemberBeacon(int member_id);
};

#endif // MEMBERSTABLEMODEL_H
