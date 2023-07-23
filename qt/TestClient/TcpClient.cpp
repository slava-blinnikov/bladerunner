#include <QtDebug>
#include <QDateTime>
#include <QFile>

#include "TcpClient.h"

TcpClient::TcpClient(ClientType type, const QString &crew, const QString &name, const QString &password, QObject *parent) : QObject(parent)
{
	m_socket = 0;
	m_timer = 0;
	m_db_id = -1;
	m_handler.type = type;
	m_handler.crew = crew;
	m_handler.name = name;
	m_handler.password = password;
	initCrc32Table(m_crc32_table);
}

int TcpClient::dbIndex()
{
	return m_db_id;
}

TcpClient::~TcpClient()
{
	if (m_timer)
	{
		delete m_timer;
		m_timer = 0;
	}
	if (m_socket)
	{
		delete m_socket;
		m_socket = 0;
	}
}

bool TcpClient::processPacket(ServerQuery qtype, const char *data, unsigned int bufflen, unsigned int &bpos, bool &processed)
{
	bool is_last;
	bool is_valid;
	bool repeat = true;
	processed = true;
	switch (qtype)
	{
		case QueryClientHeartbeat:
			repeat = processPacketHeader(bufflen, bpos);
			onClientHeartbeat();
			break;
		case QueryServerHeartbeat:
			repeat = processPacketHeader(bufflen, bpos);
			onServerHeartbeat();
			break;
		case QueryAuthorization:
			repeat = processPacketHeader(bufflen, bpos);
			sendAuthorization();
			break;
		case QueryAuthorizationOk:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) onAuthorizationConfirm();
			break;
		case QueryAvatarChangeTime:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) onAvatarChangeTimeReceived();
			break;
		case QueryAvatar:
			repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
			if (repeat && is_last && is_valid) onAvatarReceived();
			break;
		default: processed = false;
	}
	return repeat;
}

void TcpClient::send(unsigned char qtype)
{
	QByteArray data;
	data.append(PACKET_START_MARKER);
	data.append(qtype);
	m_socket->write(data);
}

void TcpClient::send(unsigned char qtype, const QByteArray &payload)
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

bool TcpClient::waitStartMarker(const char *data, unsigned int bufflen, ServerQuery &qtype, unsigned int &bpos)
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

bool TcpClient::processPacketHeader(const char *data, unsigned int bufflen, unsigned int &bpos, bool &is_last, bool &is_valid)
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

bool TcpClient::processPacketHeader(unsigned int bufflen, unsigned int &bpos)
{
	if (bpos <= bufflen) m_buffer.remove(0, bpos);
	return true;
}

void TcpClient::sendAuthorization()
{
	QByteArray payload;
	unsigned char ct = (unsigned char)m_handler.type;
	payload.append(ct);
	Data2Bytes len;
	len.data = (unsigned short)m_handler.name.length();
	payload.append((const char *)len.bytes, 2);
	payload.append(m_handler.name.toUtf8());
	len.data = (unsigned short)m_handler.crew.length();
	payload.append((const char *)len.bytes, 2);
	payload.append(m_handler.crew.toUtf8());
	len.data = (unsigned short)m_handler.password.length();
	payload.append((const char *)len.bytes, 2);
	payload.append(m_handler.password.toUtf8());
	send(QueryAuthorization, payload);
}

void TcpClient::onAuthorizationConfirm()
{
	if (m_query_data.size() >= 4)
	{
		const char *qdata = m_query_data.constData();
		Data4Bytes d;
		memcpy(d.bytes, qdata, 4);
		m_db_id = (int)d.data;
		qdata += 4;
		memcpy(d.bytes, qdata, 4);
		int timeout = (int)d.data * 1000;
		emit authorized();
		sendAvatarChangetimeQuery(m_handler.type, m_db_id);
		if (m_handler.type == Crew) send(QueryMembersList); else send(QueryMemberConfig);
		if (!m_timer) m_timer = new QTimer(); else m_timer->stop();
		m_timer->setInterval(timeout);
		m_timer->setSingleShot(false);
		m_heartbeat_sended = false;
		connect(m_timer, SIGNAL(timeout()), this, SLOT(heartbeat()));
		m_timer->start();
	}
}

void TcpClient::heartbeat()
{
	if (!m_heartbeat_sended)
	{
		send(QueryClientHeartbeat);
		m_heartbeat_sended = true;
	}
	else emit finished();
}

void TcpClient::sendAvatarChangetimeQuery(ClientType ctype, int db_id)
{
	QByteArray payload;
	unsigned char ct = (unsigned char)ctype;
	payload.append(ct);
	Data4Bytes id;
	id.data = (unsigned int)db_id;
	payload.append((const char *)id.bytes, 4);
	send(QueryAvatarChangeTime, payload);
}

void TcpClient::sendAvatarQuery(ClientType ctype, int db_id)
{
	QByteArray payload;
	unsigned char ct = (unsigned char)ctype;
	payload.append(ct);
	Data4Bytes id;
	id.data = (unsigned int)db_id;
	payload.append((const char *)id.bytes, 4);
	send(QueryAvatar, payload);
}

void TcpClient::onAvatarChangeTimeReceived()
{
	if (m_query_data.size() >= 9)
	{
		const char *qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		if (ctype == m_handler.type)
		{
			Data4Bytes d;
			memcpy(d.bytes, qdata + 1, 4);
			if (((int)d.data) == m_db_id)
			{
				memcpy(d.bytes, qdata + 5, 4);
				unsigned int ttime = d.data;
				AvatarHandler ava;
				if (Resources::getAvatarHandler(&ava, ctype, m_db_id))
				{
					if (ava.changetime.toTime_t() != ttime) sendAvatarQuery(ctype, m_db_id);
					else emit avatarChanged(ava.filename);
				}
				else sendAvatarQuery(ctype, m_db_id);
			}
		}
	}
}

void TcpClient::onAvatarReceived()
{
	if (m_query_data.size() > 9)
	{
		const char *qdata = m_query_data.constData();
		ClientType ctype = (ClientType)qdata[0];
		Data4Bytes id;
		memcpy(id.bytes, qdata + 1, 4);
		Data4Bytes chtime;
		memcpy(chtime.bytes, qdata + 5, 4);
		QString filename = saveAvatar(ctype, id.data, chtime.data, m_query_data.mid(9));
		if (!filename.isEmpty())
		{
			if ((ctype == m_handler.type) && ((int)id.data == m_db_id)) emit avatarChanged(filename);
				else if (m_handler.type == Crew && ctype == Member) emit memberAvatarChanged(id.data, filename);
		}
	}
}

void TcpClient::onClientHeartbeat()
{
	qDebug() << QString("%1(%2) Client heartbeat: %3").arg(m_handler.type == Crew ? "Crew" : "Member").arg(m_db_id).arg(QTime::currentTime().toString("hh:mm:ss zzz")).toLatin1().constData();
	m_heartbeat_sended = false;
}

void TcpClient::onServerHeartbeat()
{
	qDebug() << QString("%1(%2) Server heartbeat: %3").arg(m_handler.type == Crew ? "Crew" : "Member").arg(m_db_id).arg(QTime::currentTime().toString("hh:mm:ss zzz")).toLatin1().constData();
	send(QueryServerHeartbeat);
}

QString TcpClient::saveAvatar(ClientType type, int id, uint changetime, const QByteArray &ava)
{
	bool ok = false;
	QString filename = QString("%1/%2.%3.png").arg(Resources::users_path).arg(type == Crew ? "crew" : "member").arg(id);
	QFile file(filename);
	if (file.open(QIODevice::WriteOnly))
	{
		ok = file.write(ava) >= 0;
		file.close();
		if (ok)
		{
			QDomElement tag_root = Resources::cfg->document()->documentElement();
			if (!tag_root.isNull())
			{
				QDomElement tag_users = tag_root.firstChildElement("users");
				if (!tag_users.isNull())
				{
					bool new_user = true;
					QDomElement tag_user = tag_users.firstChildElement("user");
					while (!tag_user.isNull())
					{
						ClientType ct = tag_user.attribute("type") == "crew" ? Crew : Member;
						if (type == ct)
						{
							bool ok;
							QString s = tag_user.attribute("id");
							int tid = s.toInt(&ok);
							if (ok && tid == id)
							{
								QDateTime avatar_changetime;
								avatar_changetime.setTime_t(changetime);
								tag_user.setAttribute("avatar_file", QString("%1.%2.png").arg(type == Crew ? "crew" : "member").arg(id));
								tag_user.setAttribute("avatar_changetime", avatar_changetime.toString("yyyy-MM-dd hh:mm:ss"));
								new_user = false;
								break;
							}
						}
						tag_user = tag_user.nextSiblingElement("user");
					}
					if (new_user)
					{
						QDateTime avatar_changetime;
						avatar_changetime.setTime_t(changetime);
						tag_user = Resources::cfg->document()->createElement("user");
						tag_user.setAttribute("type", type == Crew ? "crew" : "member");
						tag_user.setAttribute("id", id);
						tag_user.setAttribute("avatar_file", QString("%1.%2.png").arg(type == Crew ? "crew" : "member").arg(id));
						tag_user.setAttribute("avatar_changetime", avatar_changetime.toString("yyyy-MM-dd hh:mm:ss"));
						tag_users.appendChild(tag_user);
					}
					Resources::cfg->save();
				}
			}
		}
	}
	if (ok) return filename; else return QString();
}

void TcpClient::socketReadyRead()
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

void TcpClient::socketError(QAbstractSocket::SocketError error)
{
	if (error == QAbstractSocket::ConnectionRefusedError || error == QAbstractSocket::HostNotFoundError) emit finished();
}

void TcpClient::start()
{
	if (!m_socket)
	{
		m_buffer.clear();
		m_query_data.clear();
		m_socket = new QTcpSocket();
		connect(m_socket, SIGNAL(readyRead()), this, SLOT(socketReadyRead()));
		connect(m_socket, SIGNAL(disconnected()), this, SIGNAL(finished()));
		connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));
		connect(m_socket, SIGNAL(connected()), this, SIGNAL(connected()));
		m_socket->connectToHost(Resources::host, Resources::port);
	}
}

void TcpClient::stop()
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

void TcpClient::requestAvatar()
{
	sendAvatarQuery(m_handler.type, m_db_id);
}
