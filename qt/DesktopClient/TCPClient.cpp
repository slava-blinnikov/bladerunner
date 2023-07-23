#include  <QtDebug>

#include "TCPClient.h"

TCPClient::TCPClient(QObject *parent) : QObject(parent)
{
	m_db_index = -1;
	m_socket = 0;
	m_keep_alive = false;
	m_reconnection_timer = 0;
	m_heartbeat_timer = 0;
	initCrc32Table(m_crc32_table);
}

TCPClient::~TCPClient()
{
	if (m_socket)
	{
		delete m_socket;
		m_socket = 0;
	}
}

bool TCPClient::waitStartMarker(const char *data, unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos)
{
	if (bufflen > START_MARKER_LENGTH)
	{
		int smi = m_buffer.indexOf(PACKET_START_MARKER);
		if (smi >= 0)
		{
			unsigned int qtype_index = smi + START_MARKER_LENGTH;
			if (qtype_index < bufflen)
			{
				qtype = (ServerQuery)data[qtype_index];
				bpos = qtype_index + 1;
				return true;
			}
		}
	}
	return false;
}

bool TCPClient::processPacket(ServerQuery qtype, const char *data, unsigned int bufflen, unsigned int &bpos, bool &processed)
{
	bool is_last;
	bool is_valid;
	bool repeat = true;
	processed = true;
	switch (qtype)
	{
		case QueryClientHeartbeat:
			repeat = processPacketHeader(bufflen, bpos);
			handleQueryClientHeartbeat();
			break;
		case QueryServerHeartbeat:
			repeat = processPacketHeader(bufflen, bpos);
			handleQueryServerHeartbeat();
			break;
		case QueryAuthorization:
			repeat = processPacketHeader(bufflen, bpos);
			handleQueryAuthorization();
			break;
		case QueryAuthorizationOk:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryAuthorizationOk();
			break;
		case QueryAvatarChangeTime:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryAvatarChangeTime();
			break;
		case QueryAvatar:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryAvatar();
			break;
		case QueryMembersList:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryMembersList();
			break;
		case QueryMember:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryMember();
			break;
		case QueryMembersOnline:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryMembersOnline();
			break;
		case QueryMemberConnected:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryMemberConnected();
			break;
		case QueryMemberDisconnected:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryMemberDisconnected();
			break;
		case QueryBeaconState:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryBeaconState();
			break;
		case QueryLocation:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQueryLocation();
			break;
		case QuerySetTrackingPermission:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQuerySetTrackingPermission();
			break;
		case QuerySetRecordingPermission:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) handleQuerySetRecordingPermission();
			break;
		default: processed = false;
	}
	return repeat;
}

bool TCPClient::processPacketHeader(const char *data, unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid)
{
	is_last = false;
	is_valid = false;
	if (bpos + 3 < bufflen)
	{
		unsigned char cf = data[bpos++];
		bool is_first = !(cf & 0x01);
		if (is_first)
		{
			if (bpos + 14 < bufflen)
			{
				Data8Bytes dl;
				memcpy(dl.bytes, data + bpos, 8);
				m_data_length = dl.data;
				bpos += 8;
				Data4Bytes crc;
				memcpy(crc.bytes, data + bpos, 4);
				m_crc32 = crc.data;
				bpos += 4;
				m_query_data.clear();
			}
			else return false;
		}
		Data2Bytes pl;
		memcpy(pl.bytes, data + bpos, 2);
		bpos += 2;
		if (bpos < bufflen)
		{
			unsigned int psize = bpos + pl.data;
			if (psize <= bufflen)
			{
				m_query_data.append(m_buffer.mid(bpos, pl.data));
				m_buffer.remove(0, psize);
				is_last = cf & 0x02;
				unsigned int ncrc = crc32((unsigned char *)m_query_data.constData(), m_query_data.size(), m_crc32_table);
				is_valid = ncrc == m_crc32;
				return true;
			}
		}
	}
	return false;
}

bool TCPClient::processPacketHeader(unsigned int bufflen, unsigned int &bpos)
{
	if (bpos <= bufflen) m_buffer.remove(0, bpos);
	return true;
}

void TCPClient::send(unsigned char qtype)
{
	if (m_socket)
	{
		QByteArray data;
		data.append(PACKET_START_MARKER);
		data.append(qtype);
		m_socket->write(data);
	}
}

void TCPClient::send(unsigned char qtype, const QByteArray &payload)
{
	if (m_socket)
	{
		unsigned long payload_size = (unsigned long)payload.size();
		if (payload_size > 0)
		{
			unsigned short packet_size;
			QByteArray data;
			unsigned long ni;
			unsigned long i = 0;
			bool cnt = true;
			while (cnt)
			{
				data.clear();
				data.append(PACKET_START_MARKER);
				data.append(qtype);
				ni = i +  PACKET_MAX_SIZE;
				cnt = ni < payload_size;
				if (cnt) packet_size =  PACKET_MAX_SIZE; else packet_size = payload_size - i;
				unsigned char cf;
				if (i == 0)
				{
					if (cnt) cf = 0; else cf = 2;
					data.append(cf);
					Data8Bytes dl;
					dl.data = payload_size;
					data.append((const char *)dl.bytes, 8);
					Data4Bytes crc;
					crc.data = crc32((unsigned char *)payload.constData(), payload_size, m_crc32_table);
					data.append((const char *)crc.bytes, 4);
				}
				else
				{
					if (cnt) cf = 1; else cf = 3;
					data.append(cf);
				}
				Data2Bytes pl;
				pl.data = packet_size;
				data.append((const char *)pl.bytes, 2);
				data.append(payload.mid(i, packet_size));
				m_socket->write(data);
				i = ni;
			}
		}
		else send(qtype);
	}
}

void TCPClient::sendAvatarQuery(ClientType ctype, int db_id)
{
	QByteArray payload;
	unsigned char ct = (unsigned char)ctype;
	payload.append(ct);
	Data4Bytes id;
	id.data = (unsigned int)db_id;
	payload.append((const char *)id.bytes, 4);
	send(QueryAvatar, payload);
}

void TCPClient::handleQueryClientHeartbeat()
{
	m_heartbeat_sended = false;
}

void TCPClient::handleQueryServerHeartbeat()
{
	send(QueryServerHeartbeat);
}

void TCPClient::handleQueryAuthorization()
{
	qDebug() << tr("authorization user: '%1'; pass: '%2';").arg(Resources::user_name).arg(Resources::password).toLatin1().constData();
	QByteArray payload;
	payload.append((unsigned char)Crew);
	Data2Bytes len;
	QString name = Resources::user_name;
	QString pass = Resources::password;
	QString watcher = Resources::watcher_name;
	len.data = (unsigned short)name.length();
	payload.append((const char *)len.bytes, 2);
	payload.append(name.toUtf8());
	len.data = (unsigned short)watcher.length();
	payload.append((const char *)len.bytes, 2);
	if (len.data > 0) payload.append(watcher.toUtf8());
	len.data = (unsigned short)pass.length();
	payload.append((const char *)len.bytes, 2);
	payload.append(pass.toUtf8());
	send(QueryAuthorization, payload);
}

void TCPClient::handleQueryAuthorizationOk()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes d;
		memcpy(d.bytes, qdata, 4);
		m_db_index = (int)d.data;
		Resources::setCrewID(m_db_index);
		qdata += 4;
		memcpy(d.bytes, qdata, 4);
		int timeout = (int)d.data * 1000;
		QByteArray payload;
		payload.append((unsigned char)Crew);
		d.data = (unsigned int)m_db_index;
		payload.append((const char *)d.bytes, 4);
		send(QueryAvatarChangeTime, payload);
		send(QueryMembersList);
		m_heartbeat_sended = false;
		if (!m_heartbeat_timer)
		{
			m_heartbeat_timer = new QTimer();
			m_heartbeat_timer->setInterval(timeout);
			m_heartbeat_timer->setSingleShot(false);
			connect(m_heartbeat_timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
			connect(this, SIGNAL(disconnected()), m_heartbeat_timer, SLOT(stop()));
		}
		else m_heartbeat_timer->stop();
		m_heartbeat_timer->start();
		qDebug() << "authorized!";
		emit connected();
	}
}

void TCPClient::handleQueryAvatarChangeTime()
{
	if (m_query_data.size() >= 9)
	{
		const char *qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		Data4Bytes d;
		memcpy(d.bytes, qdata + 1, 4);
		int db_id = (int)d.data;
		memcpy(d.bytes, qdata + 5, 4);
		unsigned int changetime = d.data;
		Avatar ava = Resources::findAvatar(ctype, db_id);
		if (!ava.file_exists || ava.changetime != changetime) sendAvatarQuery(ctype, db_id);
	}
}

void TCPClient::handleQueryAvatar()
{
	if (m_query_data.size() > 9)
	{
		const char *qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		Data4Bytes id;
		memcpy(id.bytes, qdata + 1, 4);
		Data4Bytes chtime;
		memcpy(chtime.bytes, qdata + 5, 4);
		if (Resources::saveAvatar(ctype, id.data, chtime.data, m_query_data.mid(9)))
			emit avatarChanged(ctype, id.data);
	}
}

void TCPClient::handleQueryMembersList()
{
	if (m_query_data.size() >= 2)
	{
		const char *qdata = m_query_data.constData();
		Data2Bytes count;
		memcpy(count.bytes, qdata, 2);
		qdata += 2;
		Data4Bytes id;
		IntList members;
		for (unsigned short i = 0; i < count.data; i++)
		{
			memcpy(id.bytes, qdata, 4);
			members.append(id.data);
			qdata += 4;
		}
		emit memberList(members);
		for (int i = 0; i < members.size(); i++)
		{
			id.data = (unsigned int)members.at(i);
			send(QueryMember, QByteArray((const char*)id.bytes, 4));
		}
		send(QueryMembersOnline);
	}
}

void TCPClient::handleQueryMember()
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
		bool tracking_permission = *(qdata++);
		bool recording_permission = *(qdata++);
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
		qDebug() << QString("len=%1; note=\"%2\"").arg(len.data).arg(note).toLocal8Bit();
		emit memberTitle(id.data, usrname, note, changetime.data, tracking_permission, recording_permission, trctimeout.data);
		Avatar ava = Resources::findAvatar(Member, id.data);
		if (!ava.file_exists || ava.changetime != changetime.data) sendAvatarQuery(Member, id.data);
	}
}

void TCPClient::handleQueryMembersOnline()
{
	if (m_query_data.size() >= 2)
	{
		const char *qdata = m_query_data.constData();
		Data2Bytes count;
		memcpy(count.bytes, qdata, 2);
		qdata += 2;
		Data4Bytes id;
		IntList members;
		for (unsigned short i = 0; i < count.data; i++)
		{
			memcpy(id.bytes, qdata, 4);
			members.append(id.data);
			qdata += 4;
		}
		emit membersOnline(members);
	}
}

void TCPClient::handleQueryMemberConnected()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		emit memberConnected(id.data);
	}
}

void TCPClient::handleQueryMemberDisconnected()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);
		emit memberDisconnected(id.data);
	}
}

void TCPClient::handleQueryBeaconState()
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

void TCPClient::handleQueryLocation()
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

void TCPClient::handleQuerySetTrackingPermission()
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

void TCPClient::handleQuerySetRecordingPermission()
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

void TCPClient::onSocketConnected()
{
	QTcpSocket *socket = qobject_cast<QTcpSocket *>(sender());
	if (socket)
	{
		qDebug() << "connected!";
		m_socket = socket;
		connect(socket, SIGNAL(disconnected()), this, SLOT(onSocketDisconnected()));
		connect(socket, SIGNAL(readyRead()), this, SLOT(onSocketReadyRead()));
		if (m_reconnection_timer)
		{
			delete m_reconnection_timer;
			m_reconnection_timer = 0;
		}
	}
}

void TCPClient::onSocketReadyRead()
{
	m_buffer.append(m_socket->readAll());
	ServerQuery qtype;
	bool repeat;
	bool processed;
	do
	{
		const char *data = m_buffer.constData();
		unsigned int bufflen = m_buffer.size();
		unsigned int bpos = 0;
		repeat = waitStartMarker(data, bufflen, qtype, bpos);
		if (repeat)
		{
			repeat = processPacket(qtype, data, bufflen, bpos, processed);
			if (!processed) processPacketHeader(bufflen, bpos);
		}
	}
	while (repeat);
}

void TCPClient::onSocketError(QAbstractSocket::SocketError error)
{
	if (error == QAbstractSocket::ConnectionRefusedError || error == QAbstractSocket::HostNotFoundError) onSocketDisconnected();
}

void TCPClient::onSocketDisconnected()
{
	m_socket = 0;
	emit disconnected();
	if (m_keep_alive)
	{
		if (!m_reconnection_timer)
		{
			m_reconnection_timer = new QTimer();
			m_reconnection_timer->setInterval(Resources::reconnection_interval * 1000);
			m_reconnection_timer->setSingleShot(true);
			connect(m_reconnection_timer, SIGNAL(timeout()), this, SLOT(connectToServer()));
		}
		m_reconnection_timer->start();
	}
}

void TCPClient::heartbeat()
{
	if (!m_heartbeat_sended)
	{
		send(QueryClientHeartbeat);
		m_heartbeat_sended = true;
	}
}

void TCPClient::start()
{
	qDebug() << "start";
	m_keep_alive = true;
	connectToServer();
}

void TCPClient::connectToServer()
{
	if (m_socket)
	{
		m_socket->blockSignals(true);
		if (m_socket->state() == QTcpSocket::ConnectedState) m_socket->disconnectFromHost();
		delete m_socket;
		m_socket = 0;
	}
	QTcpSocket *socket = new QTcpSocket();
	connect(socket, SIGNAL(connected()), this, SLOT(onSocketConnected()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onSocketError(QAbstractSocket::SocketError)));
	connect(this, SIGNAL(disconnected()), socket, SLOT(deleteLater()));
	qDebug() << tr("connecting to: %1:%2").arg(Resources::host).arg(Resources::port).toLatin1().constData();
	socket->connectToHost(Resources::host, Resources::port);
}

void TCPClient::stop()
{
	qDebug() << "stop";
	m_keep_alive = false;
	if (m_reconnection_timer)
	{
		delete m_reconnection_timer;
		m_reconnection_timer = 0;
	}
	if (m_heartbeat_timer)
	{
		delete m_heartbeat_timer;
		m_heartbeat_timer = 0;
	}
	if (m_socket)
	{
		if (m_socket->state() == QTcpSocket::ConnectedState) m_socket->disconnectFromHost();
	}
}

void TCPClient::requestStartMemberBeacon(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryStartBeacon, payload);
}

void TCPClient::requestStopMemberBeacon(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryStopBeacon, payload);
}

