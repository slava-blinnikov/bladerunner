#include "CrewTcpClient.h"

CrewTcpClient::CrewTcpClient(const QString &name, const QString &password, QObject *parent)
	:TcpClient(Crew, name, name, password, parent)
{
}

bool CrewTcpClient::processPacket(ServerQuery qtype, const char *data, unsigned int bufflen, unsigned int &bpos, bool &processed)
{
	bool is_last;
	bool is_valid;
	bool repeat = TcpClient::processPacket(qtype, data, bufflen, bpos, processed);
	if (!processed)
	{
		processed = true;
		switch (qtype)
		{
			case QueryMembersList:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMembersList();
				break;
			case QueryMember:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMember();
				break;
			case QueryMembersOnline:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMembersOnline();
				break;
			case QueryMemberConnected:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberConnected();
				break;
			case QueryMemberDisconnected:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberDisconnected();
				break;
			case QueryBeaconState:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberBeaconState();
				break;
			case QueryLocation:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberLocation();
				break;
			case QuerySetTrackingPermission:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberSetTrackingPermission();
				break;
			case QuerySetRecordingPermission:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberSetRecordingPermission();
				break;
			default: processed = false;
		}
	}
	return repeat;
}

void CrewTcpClient::onMembersList()
{
	if (m_query_data.size() >= 2)
	{
		const char *qdata = m_query_data.constData();
		Data2Bytes count;
		memcpy(count.bytes, qdata, 2);
		qdata += 2;
		Data4Bytes id;
		QList<int> members;
		for (unsigned short i = 0; i < count.data; i++)
		{
			memcpy(id.bytes, qdata, 4);
			members.append(id.data);
			qdata += 4;
		}
		emit memberList(members);
	}
}

void CrewTcpClient::onMember()
{
	if (m_query_data.size() > 18)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		qdata += 4;
		Data4Bytes changetime;
		memcpy(changetime.bytes, qdata, 4);
		qdata += 4;
		bool tracking_allowed = *(qdata++);
		bool tracing_allowed = *(qdata++);
		Data4Bytes trctimeout;
		memcpy(trctimeout.bytes, qdata, 4);
		qdata += 4;
		Data2Bytes len;
		memcpy(len.bytes, qdata, 2);
		qdata += 2;
		QString usrname;
		int idx = 16;
		if (len.data > 0) usrname = QString(m_query_data.mid(idx, len.data));
		qdata += len.data;
		idx += len.data + 2;
		memcpy(len.bytes, qdata, 2);
		qdata += 2;
		QString note;
		if (len.data > 0) note = QString(m_query_data.mid(idx, len.data));
		emit memberTitle(id.data, usrname, note, changetime.data, tracking_allowed, tracing_allowed, trctimeout.data);
	}
}

void CrewTcpClient::onMembersOnline()
{
	if (m_query_data.size() >= 2)
	{
		const char *qdata = m_query_data.constData();
		Data2Bytes count;
		memcpy(count.bytes, qdata, 2);
		qdata += 2;
		Data4Bytes id;
		QList<int> members;
		for (unsigned short i = 0; i < count.data; i++)
		{
			memcpy(id.bytes, qdata, 4);
			members.append(id.data);
			qdata += 4;
		}
		emit membersOnline(members);
	}
}

void CrewTcpClient::onMemberConnected()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		emit memberConnected(id.data);
	}
}

void CrewTcpClient::onMemberDisconnected()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		emit memberDisconnected(id.data);
	}
}

void CrewTcpClient::onMemberBeaconState()
{
	if (m_query_data.size() >= 5)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		char state = qdata[4];
		emit memberBeaconState(id.data, state);
	}
}

void CrewTcpClient::onMemberLocation()
{
	if (m_query_data.size() >= 33)
	{
		int member_id;
		uint time;
		double latitude;
		double longitude;
		double speed;
		bool gps;
		const char *qdata = m_query_data.constData();
		Data4Bytes i;
		memcpy(i.bytes, qdata, 4);
		member_id = i.data;
		qdata += 4;
		memcpy(i.bytes, qdata, 4);
		time = i.data;
		qdata += 4;
		Data8Bytes f;
		memcpy(f.bytes, qdata, 8);
		latitude = f.fdata;
		qdata += 8;
		memcpy(f.bytes, qdata, 8);
		longitude = f.fdata;
		qdata += 8;
		memcpy(f.bytes, qdata, 8);
		speed = f.fdata;
		qdata += 8;
		gps = (bool)(*qdata);
		emit memberLocationChanged(member_id, time, latitude, longitude, speed, gps);
	}
}

void CrewTcpClient::onMemberSetTrackingPermission()
{
	if (m_query_data.size() >= 5)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		char state = qdata[4];
		emit memberTrackingPermission(id.data, state);
	}
}

void CrewTcpClient::onMemberSetRecordingPermission()
{
	if (m_query_data.size() >= 5)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		char state = qdata[4];
		emit memberRecordingPermission(id.data, state);
	}
}

void CrewTcpClient::requestMemberAvatar(int member_id)
{
	sendAvatarQuery(Member, member_id);
}

void CrewTcpClient::requestMember(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryMember, payload);
}

void CrewTcpClient::requestMembersOnline()
{
	send(QueryMembersOnline);
}

void CrewTcpClient::requestStartMemberBeacon(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryStartBeacon, payload);
}

void CrewTcpClient::requestStopMemberBeacon(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryStopBeacon, payload);
}
