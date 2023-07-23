#include <QThread>
#include <QByteArray>
#include <QDateTime>
#include <QFile>

#include "qtservice.h"
#include "Resources.h"
#include "ChannelWorker.h"

ChannelWorker::ChannelWorker(qintptr descriptor, QObject *parent) :
	QObject(parent)
{
	m_socket_descriptor = descriptor;
	m_socket = 0;
	m_timer = 0;
	m_db = 0;
	m_db_id = -1;
	m_type = ClientUnknown;
	m_tracking_timeout = 5;
	m_perm_tracking = true;
	m_perm_recording = false;
	m_ping_sended = false;
	m_recording = false;
	initCrc32Table(m_crc32_table);
}

ChannelWorker::~ChannelWorker()
{
	if (m_db)
	{
		m_db->close();
		delete m_db;
		m_db = 0;
	}
	if (m_socket)
	{
		delete m_socket;
		m_socket = 0;
	}
	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}
}

ClientType ChannelWorker::clientType()
{
	return m_type;
}

QString ChannelWorker::clientCrew()
{
	return m_crew;
}

QString ChannelWorker::clientName()
{
	return m_name;
}

int ChannelWorker::clientID()
{
	return m_db_id;
}

bool ChannelWorker::send(ServerQuery qry)
{
	if (m_socket)
	{
		if (m_socket->state() == QTcpSocket::ConnectedState)
		{
			QByteArray data;
			data.append(PACKET_START_MARKER);
			unsigned char rt = (unsigned char)qry;
			data.append(rt);
			qint64 n = m_socket->write(data);
			return n >= data.size();
		}
	}
	return false;
}

bool ChannelWorker::send(ServerQuery qry, const QByteArray &payload)
{
	unsigned long payload_size = (unsigned long)payload.size();
	if (payload_size > 0)
	{
		unsigned char qtype = (unsigned char)qry;
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
			if (cnt) packet_size =  PACKET_MAX_SIZE; else packet_size = payload_size - i;
			Data2Bytes pl;
			pl.data = packet_size;
			data.append((const char *)pl.bytes, 2);
			data.append(payload.mid(i, packet_size));
			m_socket->write(data);
			i = ni;
		}
	}
	else return send(qry);
	return false;
}

bool ChannelWorker::waitStartMarker(const char *data,  unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos)
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

void ChannelWorker::clientAuthorization()
{
	m_watcher_name.clear();
	const char * qdata = m_query_data.constData();
	unsigned char ct = qdata[0];
	QString s;
	Data2Bytes slen;
	unsigned int nmi = 1;
	memcpy(slen.bytes, qdata + nmi, 2);
	nmi += 2;
	s = QString(m_query_data.mid(nmi, slen.data));
	m_name = s;
	nmi += slen.data;
	memcpy(slen.bytes, qdata + nmi, 2);
	nmi += 2;
	if (ct == Crew)
	{
		m_crew = s;
		if (slen.data > 0)
		{
			m_watcher_name = QString(m_query_data.mid(nmi, slen.data));
			emit watcherNameChanged(m_watcher_name);
		}
	}
	else if (ct == Member)
	{
		m_crew = QString(m_query_data.mid(nmi, slen.data));
	}
	nmi += slen.data;
	memcpy(slen.bytes, qdata + nmi, 2);
	nmi += 2;
	m_client_password = QString(m_query_data.mid(nmi, slen.data));

	QtServiceBase::instance()->logMessage(tr("Authorization request: type=%1; desc=%2; name=%3")
					      .arg(ct == Crew ? tr("Crew") : tr("Member"))
					      .arg(m_socket_descriptor)
					      .arg(ct == Crew ? QString("'%1' (%2)").arg(m_crew).arg(m_watcher_name) : QString("'%1.%2'").arg(m_crew).arg(m_name)),
					      QtServiceBase::Information);

	emit requestChannelRolename(m_socket_descriptor, m_crew, (int)ct, m_name);
}

void ChannelWorker::clientHeartbeat()
{
	QtServiceBase::instance()->logMessage(tr("Client heartbeat [%1]: %2").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(m_socket_descriptor), QtServiceBase::Information);
	send(QueryClientHeartbeat);
}

void ChannelWorker::clientAvatarChangetime()
{
	if (m_query_data.size() >= 5)
	{
		const char * qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		if (ctype == Crew || ctype == Member)
		{
			Data4Bytes dbi;
			memcpy(dbi.bytes, qdata + 1, 4);
			int db_id = (int)dbi.data;
			QSqlQuery *qry = m_db->createQuery();
			if (qry)
			{
				DbConnection::QueryKiller qk(qry);
				QString sq  = ctype == Crew ? Resources::queries["get_crew_avatar"] : Resources::queries["get_member_avatar"];
				if (qry->prepare(sq))
				{
					if (ctype == Crew) qry->bindValue(":crew", QVariant(db_id)); else qry->bindValue(":member", QVariant(db_id));
					if (m_db->exec(qry))
					{
						if (qry->next())
						{
							if (!qry->value(0).isNull())
							{
								Data4Bytes time;
								time.data = qry->value(0).toDateTime().toTime_t();
								QByteArray payload;
								payload.append(qdata, 5);
								payload.append((const char *)time.bytes, 4);
								send(QueryAvatarChangeTime, payload);
							}
						}
					}
				}
			}
		}
	}
}

void ChannelWorker::clientAvatar()
{
	if (m_query_data.size() >= 5)
	{
		const char * qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		if (ctype == Crew || ctype == Member)
		{
			QString avafile;
			QString avapath;
			uint avatime = 0;
			Data4Bytes dbi;
			memcpy(dbi.bytes, qdata + 1, 4);
			int db_id = (int)dbi.data;
			QSqlQuery *qry = m_db->createQuery();
			if (qry)
			{
				DbConnection::QueryKiller qk(qry);
				QString sq  = ctype == Crew ? Resources::queries["get_crew_avatar"] : Resources::queries["get_member_avatar"];
				if (qry->prepare(sq))
				{
					if (ctype == Crew) qry->bindValue(":crew", QVariant(db_id)); else qry->bindValue(":member", QVariant(db_id));
					if (m_db->exec(qry))
					{
						if (qry->next())
						{
							if (!qry->value(1).isNull())
							{
								avatime = qry->value(0).toDateTime().toTime_t();
								avafile = qry->value(1).toString();
								avapath = qry->value(2).toString();
							}
						}
					}
				}
			}
			if (!avafile.isEmpty())
			{
				avafile = QString("%1/users/%2/%3").arg(Resources::www_path).arg(avapath).arg(avafile);
				QFile file(avafile);
				if (file.exists())
				{
					if (file.open(QIODevice::ReadOnly))
					{
						QByteArray payload;
						payload.append(qdata, 5);
						Data4Bytes tme;
						tme.data = avatime;
						payload.append((const char *)tme.bytes, 4);
						payload.append(file.readAll());
						file.close();
						if (file.error() == QFile::NoError) send(QueryAvatar, payload);
					}
				}
			}
		}
	}
}

void ChannelWorker::clientMembersList()
{
	QtServiceBase::instance()->logMessage(tr("Crew '%1' (%2) request members list.")
					      .arg(m_name).arg(m_watcher_name), QtServiceBase::Information);

	if (m_db)
	{
		QSqlQuery *qry = m_db->createQuery();
		if (qry)
		{
			DbConnection::QueryKiller qk(qry);
			if (m_db->exec(qry, Resources::queries["get_members_list"] ))
			{
				Data2Bytes count;
				count.data = 0;
				IntList lst;
				while (qry->next())
				{
					count.data++;
					lst.append(qry->value(0).toInt());
				}
				QByteArray payload;
				payload.append((const char*)count.bytes, 2);
				Data4Bytes id;
				for (unsigned short i = 0; i < count.data; i++)
				{
					id.data = (unsigned int)lst.at(i);
					payload.append((const char*)id.bytes, 4);
				}
				send(QueryMembersList, payload);
			}
		}
	}
}

void ChannelWorker::clientMember()
{
	if (m_db && m_query_data.size() >= 4)
	{
		QSqlQuery *qry = m_db->createQuery();
		if (qry)
		{
			DbConnection::QueryKiller qk(qry);
			if (qry->prepare(Resources::queries["get_member"]))
			{
				const char * qdata = m_query_data.constData();
				Data4Bytes id;
				memcpy(id.bytes, qdata, 4);

				QtServiceBase::instance()->logMessage(tr("Crew '%1' (%2) request member handle with id %3")
								      .arg(m_name).arg(m_watcher_name).arg((int)id.data), QtServiceBase::Information);

				qry->bindValue(":member", QVariant((int)id.data));
				if (m_db->exec(qry))
				{
					if (qry->next())
					{
						QByteArray payload;
						payload.append((const char*)id.bytes, 4); //member id
						Data4Bytes changetime;
						changetime.data  = !qry->value(0).isNull() ? qry->value(0).toDateTime().toTime_t() : 0;
						payload.append((const char*)changetime.bytes, 4); //avatar changetime
						char ta = qry->value(1).toBool();
						payload.append(ta); //tracking permission
						ta = qry->value(2).toBool();
						payload.append(ta); //recording permission
						Data4Bytes trctimeout;
						trctimeout.data = qry->value(3).toInt();
						payload.append((const char*)trctimeout.bytes, 4); //tracking timeout
						Data2Bytes len;
						QString s = qry->value(4).toString();
						len.data = (unsigned short)s.length();
						payload.append((const char *)len.bytes, 2);
						payload.append(s.toUtf8()); //member name
						payload.append((char)0); //member note stub
						payload.append((char)0); //member note stub
						send(QueryMember, payload);
					}
				}
			}
		}
	}
}

void ChannelWorker::clientMembersOnline()
{
	QtServiceBase::instance()->logMessage(tr("Crew '%1' (%2) request members online.")
					      .arg(m_name).arg(m_watcher_name), QtServiceBase::Information);

	emit requestMembersOnline(m_socket_descriptor);
}

void ChannelWorker::clientStartBeacon()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);

		QtServiceBase::instance()->logMessage(tr("Crew '%1' (%2) request to start beacon on member with id = %3")
						      .arg(m_name)
						      .arg(m_watcher_name)
						      .arg((int)id.data), QtServiceBase::Information);

		emit startBeaconRequest((int)m_socket_descriptor, (int)id.data, m_watcher_name);
	}
}

void ChannelWorker::clientStopBeacon()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes id;
		memcpy(id.bytes, qdata, 4);

		QtServiceBase::instance()->logMessage(tr("Crew '%1' (%2) request to stop beacon on member = %3")
						      .arg(m_name)
						      .arg(m_watcher_name)
						      .arg((int)id.data), QtServiceBase::Information);

		emit stopBeaconRequest((int)m_socket_descriptor, (int)id.data, m_watcher_name);
	}
}

void ChannelWorker::clientBeaconState()
{
	if (m_query_data.size() >= 1) emit beaconStateChanged(m_db_id, (bool)m_query_data.at(0));
}

void ChannelWorker::clientMemberConfig()
{
	QByteArray payload;
	payload.append((char)m_perm_tracking);
	payload.append((char)m_perm_recording);
	Data4Bytes d;
	d.data = (unsigned int)m_tracking_timeout;
	payload.append((const char *)d.bytes, 4);
	send(QueryMemberConfig, payload);
}

void ChannelWorker::clientLocation()
{
	if (m_query_data.size() >= 29)
	{
		uint time;
		double latitude;
		double longitude;
		double speed;
		bool gps;
		const char *qdata = m_query_data.constData();
		Data4Bytes t;
		memcpy(t.bytes, qdata, 4);
		time = t.data;
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
		emit locationChanged(m_db_id, time, latitude, longitude, speed, gps);
		writeToDatabase(time, latitude, longitude, speed, gps);
	}
}

void ChannelWorker::clientSetTracking()
{
	//QtServiceBase::instance()->logMessage(QString("clientSetTracking()"), QtServiceBase::Information);
	if (m_query_data.size() >= 1)
	{
		bool new_perm_tracking = (bool)m_query_data.at(0);
		//QtServiceBase::instance()->logMessage(QString("tracking: %1").arg(new_tracking_allowed ? "allow" : "forbid"), QtServiceBase::Information);
		if (new_perm_tracking != m_perm_tracking)
		{
			//QtServiceBase::instance()->logMessage(QString("tracking changed"), QtServiceBase::Information);
			if (m_db)
			{
				QSqlQuery *qry = m_db->createQuery();
				if (qry)
				{
					DbConnection::QueryKiller qk(qry);
					if (qry->prepare(Resources::queries["set_perm_tracking"]))
					{
						qry->bindValue(":perm_tracking", QVariant(new_perm_tracking));
						m_db->exec(qry);
					}
				}
			}
			m_perm_tracking = new_perm_tracking;
			QByteArray payload;
			payload.append((char)m_perm_tracking);
			payload.append((char)m_perm_recording);
			Data4Bytes d;
			d.data = (unsigned int)m_tracking_timeout;
			payload.append((const char *)d.bytes, 4);
			send(QueryMemberConfig, payload);
			emit trackingPermissionChanged(m_db_id, m_perm_tracking);
		}
	}
}

void ChannelWorker::clientSetRecording()
{
	//QtServiceBase::instance()->logMessage(QString("clientSetTracing()"), QtServiceBase::Information);
	if (m_query_data.size() >= 1)
	{
		bool new_perm_recording = (bool)m_query_data.at(0);
		if (new_perm_recording != m_perm_recording)
		{
			bool is_recording = false;
			if (m_db)
			{
				QSqlQuery *qry = m_db->createQuery();
				if (qry)
				{
					DbConnection::QueryKiller qk(qry);
					if (qry->prepare(Resources::queries["set_perm_recording"]))
					{
						qry->bindValue(":perm_recording", QVariant(new_perm_recording));
						if (m_db->exec(qry))
						{
							if (qry->next()) is_recording = qry->value(0).toBool();
						}
					}
				}
			}
			m_perm_recording = new_perm_recording;
			QByteArray payload;
			payload.append((char)m_perm_tracking);
			payload.append((char)m_perm_recording);
			Data4Bytes d;
			d.data = (unsigned int)m_tracking_timeout;
			payload.append((const char *)d.bytes, 4);
			send(QueryMemberConfig, payload);
			if (is_recording && m_perm_tracking)
			{
				payload.clear();
				payload.append((char)0);
				payload.append((char)0);
				send(QueryStartBeacon, payload);
			}
			emit recordingPermissionChanged(m_db_id, m_perm_recording);
		}
	}
}

void ChannelWorker::writeToDatabase(uint time, double latitude, double longitude, double speed, bool gps)
{
	if (m_db)
	{
		QSqlQuery *qry = m_db->createQuery();
		if (qry)
		{
			DbConnection::QueryKiller qk(qry);
			if (qry->prepare(Resources::queries["set_location"]))
			{
				QDateTime dt;
				dt.setTime_t(time);
				qry->bindValue(":time", QVariant(dt));
				qry->bindValue(":latitude", QVariant(latitude));
				qry->bindValue(":longitude", QVariant(longitude));
				qry->bindValue(":speed", QVariant(speed));
				qry->bindValue(":gps", QVariant(gps));
				m_db->exec(qry);
			}
		}
	}
}

bool ChannelWorker::processPacketHeader(const char *data, unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid)
{
	/*QString s;
	for (unsigned int i = 0; i < bufflen; i++) s += QString("%1 ").arg(data[i]);
	QtServiceBase::instance()->logMessage(QString("processPacketHeader: %1").arg(s), QtServiceBase::Information);*/

	is_last = false;
	is_valid = false;
	if (bpos + 3 < bufflen)
	{
		unsigned char cf = data[bpos++];

		//QtServiceBase::instance()->logMessage(QString("processPacketHeader: cf=%1").arg((int)cf), QtServiceBase::Information);

		bool is_first = !(cf & 0x01);
		if (is_first)
		{
			if (bpos + 14 < bufflen)
			{
				Data8Bytes dl;
				memcpy(dl.bytes, data + bpos, 8);
				m_data_length = dl.data;
				//QtServiceBase::instance()->logMessage(QString("processPacketHeader: dl=%1").arg(m_data_length), QtServiceBase::Information);

				bpos += 8;
				Data4Bytes crc;
				memcpy(crc.bytes, data + bpos, 4);
				m_crc32 = crc.data;
				//QtServiceBase::instance()->logMessage(QString("processPacketHeader: crc=%1").arg(m_crc32), QtServiceBase::Information);

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

				//QtServiceBase::instance()->logMessage(QString("processPacketHeader: ncrc=%1").arg(ncrc), QtServiceBase::Information);

				is_valid = ncrc == m_crc32;
				return true;
			}
		}
	}
	return false;
}

bool ChannelWorker::processPacketHeader(unsigned int bufflen, unsigned int &bpos)
{
	if (bpos <= bufflen) m_buffer.remove(0, bpos);
	return true;
}

void ChannelWorker::socketReadyRead()
{
	if (m_type != ClientUnknown)
	{
		m_ping_sended = false;
		m_timer->start();
	}
	m_buffer.append(m_socket->readAll());
	ServerQuery qtype;
	bool repeat;
	bool is_last;
	bool is_valid;
	do
	{
		const char *data = m_buffer.constData();
		unsigned int bufflen = m_buffer.size();
		unsigned int bpos = 0;
		repeat = waitStartMarker(data, bufflen, qtype, bpos);
		if (repeat)
		{
			switch (qtype)
			{
				case QueryClientHeartbeat:
					repeat = processPacketHeader(bufflen, bpos);
					clientHeartbeat();
					break;
				case QueryServerHeartbeat:
					repeat = processPacketHeader(bufflen, bpos);
					break;
				case QueryAuthorization:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientAuthorization();
					break;
				case QueryAvatarChangeTime:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientAvatarChangetime();
					break;
				case QueryAvatar:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientAvatar();
					break;
				case QueryMembersList:
					repeat = processPacketHeader(bufflen, bpos);
					clientMembersList();
					break;
				case QueryMember:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientMember();
					break;
				case QueryMembersOnline:
					repeat = processPacketHeader(bufflen, bpos);
					clientMembersOnline();
					break;
				case QueryStartBeacon:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientStartBeacon();
					break;
				case QueryStopBeacon:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientStopBeacon();
					break;
				case QueryBeaconState:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientBeaconState();
					break;
				case QueryLocation:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientLocation();
					break;
				case QueryMemberConfig:
					repeat = processPacketHeader(bufflen, bpos);
					clientMemberConfig();
					break;
				case QuerySetTrackingPermission:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientSetTracking();
					break;
				case QuerySetRecordingPermission:
					repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
					if (repeat && is_last && is_valid) clientSetRecording();
					break;
				default:
					repeat = processPacketHeader(bufflen, bpos);
					break;
			}
		}
	}
	while (repeat);
}

void ChannelWorker::heartbeatTimer()
{
	if (!m_ping_sended)
	{
		m_ping_sended = true;
		send(QueryServerHeartbeat);
		QtServiceBase::instance()->logMessage(QString("Server heartbeat [%1] sended to channel %2").arg(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss")).arg(m_socket_descriptor), QtServiceBase::Information);
	}
	else emit finished();
}

void ChannelWorker::start()
{
	if (!m_socket)
	{
		m_socket = new QTcpSocket();
		if (m_socket->setSocketDescriptor(m_socket_descriptor))
		{
			connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
			connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(finished()));
			m_timer = new QTimer();
			connect(m_timer, SIGNAL(timeout()), this, SLOT(stop()));
			m_timer->setInterval(Resources::login_timeout * 1000);
			m_timer->setSingleShot(true);
			send(QueryAuthorization);
			m_timer->start();
			return;
		}
	}
	emit finished();
}

void ChannelWorker::stop()
{
	if (m_socket)
	{
		if (m_socket->state() == QTcpSocket::ConnectedState)
		{
			m_socket->disconnectFromHost();
			return;
		}
	}
	emit finished();
}

void ChannelWorker::onReplyRolename(int type, const QString &rolename, int crew_id, int id)
{
	Q_UNUSED(crew_id)
	Q_UNUSED(id)
	DbConnection *wdb = new DbConnection(Resources::db->host(), Resources::db->port(), Resources::db->databaseName(), rolename, m_client_password, "client");
	if (wdb->login())
	{
		m_timer->stop();
		disconnect(m_timer, SIGNAL(timeout()), this, SLOT(stop()));
		m_db = wdb;
		m_type = (ClientType)type;
		m_rolename = rolename;
		m_recording = false;
		QSqlQuery *qry = m_db->createQuery();
		//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 1"), QtServiceBase::Information);
		if (qry)
		{
			//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 2"), QtServiceBase::Information);
			DbConnection::QueryKiller qk(qry);
			QString sq  = m_type == Crew ? Resources::queries["get_crew_id"] : Resources::queries["get_member_id"];
			if (qry->prepare(sq))
			{
				//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 3"), QtServiceBase::Information);
				qry->bindValue(":rolname", QVariant(rolename));
				if (m_db->exec(qry))
				{
					//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 4"), QtServiceBase::Information);
					if (qry->next())
					{
						//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 5"), QtServiceBase::Information);
						if (!qry->value(0).isNull())
						{
							//QtServiceBase::instance()->logMessage(QString("onReplyRolename: 6"), QtServiceBase::Information);
							m_db_id = qry->value(0).toInt();
							if (m_type == Member)
							{
								m_group_rolename   = qry->value(1).toString();
								m_tracking_timeout = qry->value(2).toInt();
								m_perm_tracking = qry->value(3).toBool();
								m_perm_recording   = qry->value(4).toBool();
								m_recording = qry->value(5).toBool();
							}
						}
					}
				}
			}
		}
		emit authorized(m_socket_descriptor);
		Data4Bytes d;
		d.data = m_db_id;
		QByteArray data;
		data.append((const char *)d.bytes, 4);
		d.data = (unsigned int)Resources::ping_timeout;
		data.append((const char *)d.bytes, 4);
		if (m_type == Member)
		{
			data.append(m_perm_tracking ? 1 : 0);
			data.append(m_perm_recording ? 1 : 0);
			d.data = (unsigned int)m_tracking_timeout;
			data.append((const char *)d.bytes, 4);
		}
		send(QueryAuthorizationOk, data);
		m_timer->setSingleShot(false);
		m_timer->setInterval(Resources::ping_timeout * 1000);
		connect(m_timer, SIGNAL(timeout()), this, SLOT(heartbeatTimer()));
		m_ping_sended = false;
		m_timer->start();
		if (m_type == Member && m_perm_tracking && m_recording)
		{
			QByteArray payload;
			payload.append((char)0);
			payload.append((char)0);
			send(QueryStartBeacon, payload);
		}
	}
	else delete wdb;
}

void ChannelWorker::onReplyMembersOnline(const IntList &members)
{
	QByteArray payload;
	Data2Bytes size;
	size.data = (unsigned short)members.size();
	payload.append((const char*)size.bytes, 2);
	Data4Bytes id;
	for (unsigned int i = 0; i < size.data; i++)
	{
		id.data = (unsigned int)members.at(i);
		payload.append((const char*)id.bytes, 4);
	}
	send(QueryMembersOnline, payload);
}

void ChannelWorker::onMemberConnected(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryMemberConnected, payload);
}

void ChannelWorker::onMemberDisconnected(int member_id)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	send(QueryMemberDisconnected, payload);
}

void ChannelWorker::onCrewConnected(int crew_dsc, const QString &watcher_name)
{
	Q_UNUSED(crew_dsc)
	Q_UNUSED(watcher_name)
}

void ChannelWorker::onCrewDisconnected(int crew_dsc, const QString &watcher_name)
{
	if (m_crews.contains(crew_dsc))
	{
		QByteArray payload;
		Data2Bytes slen;
		slen.data = (unsigned short)watcher_name.length();
		payload.append((const char*)slen.bytes, 2);
		if (slen.data > 0) payload.append(watcher_name.toUtf8());
		send(QueryCrewDisconnected, payload);
		m_crews.removeAll(crew_dsc);
	}
	if (m_crews.isEmpty() && !m_recording) send(QueryStopBeacon);
}

void ChannelWorker::onMemberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps)
{
	QByteArray payload;
	Data4Bytes i;
	i.data = (unsigned int)member_id;
	payload.append((const char*)i.bytes, 4);
	i.data = time;
	payload.append((const char*)i.bytes, 4);
	Data8Bytes f;
	f.fdata = latitude;
	payload.append((const char*)f.bytes, 8);
	f.fdata = longitude;
	payload.append((const char*)f.bytes, 8);
	f.fdata = speed;
	payload.append((const char*)f.bytes, 8);
	payload.append((char)gps);
	send(QueryLocation, payload);
}

void ChannelWorker::startBeacon(int crew_dsc, int member_id, const QString &watcher_name)
{
	if (m_db_id == member_id && m_perm_tracking)
	{
		QtServiceBase::instance()->logMessage(QString("Send start beacon request to member with id = %1").arg(m_db_id), QtServiceBase::Information);
		if (!m_crews.contains(crew_dsc)) m_crews.append(crew_dsc);
		QByteArray payload;
		Data2Bytes slen;
		slen.data = (unsigned short)watcher_name.length();
		payload.append((const char*)slen.bytes, 2);
		if (slen.data > 0) payload.append(watcher_name.toUtf8());
		send(QueryStartBeacon, payload);
	}
}

void ChannelWorker::stopBeacon(int crew_dsc, int member_id, const QString &watcher_name)
{
	if (m_db_id == member_id)
	{
		if (m_crews.contains(crew_dsc))
		{
			QByteArray payload;
			Data2Bytes slen;
			slen.data = (unsigned short)watcher_name.length();
			payload.append((const char*)slen.bytes, 2);
			if (slen.data > 0) payload.append(watcher_name.toUtf8());
			send(QueryCrewDisconnected, payload);
			m_crews.removeAll(crew_dsc);
		}
		if (m_crews.isEmpty()) send(QueryStopBeacon);
	}
}

void ChannelWorker::memberBeaconStateChanged(int member_id, bool state)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	payload.append((char)state);
	send(QueryBeaconState, payload);
}

void ChannelWorker::memberTrackingPermissionChanged(int member_id, bool state)
{
	//QtServiceBase::instance()->logMessage(QString("send tracking state to crew"), QtServiceBase::Information);
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	payload.append((char)state);
	send(QuerySetTrackingPermission, payload);
}

void ChannelWorker::memberRecordingPermissionChanged(int member_id, bool state)
{
	QByteArray payload;
	Data4Bytes id;
	id.data = (unsigned int)member_id;
	payload.append((const char*)id.bytes, 4);
	payload.append((char)state);
	send(QuerySetRecordingPermission, payload);
}

void ChannelWorker::onScheduleStart()
{
	m_recording = true;
	if (m_perm_tracking)
	{
		QByteArray payload;
		payload.append((char)0);
		payload.append((char)0);
		send(QueryStartBeacon, payload);
	}
}

void ChannelWorker::onScheduleStop()
{
	m_recording = false;
	if (m_crews.isEmpty()) send(QueryStopBeacon);
}
