#include <QtDebug>

#include "MemberTcpClient.h"

MemberTcpClient::MemberTcpClient(const QString &crew, const QString &name, const QString &password, QObject *parent)
	: TcpClient(Member, crew, name, password, parent)
{
	m_gps_timer = 0;
	qsrand(QDateTime::currentDateTime().toTime_t());
	m_config_inited = false;
	m_beacon = false;
}

bool MemberTcpClient::processPacket(ServerQuery qtype, const char *data, unsigned int bufflen, unsigned int &bpos, bool &processed)
{
	bool repeat = TcpClient::processPacket(qtype, data, bufflen, bpos, processed);
	if (!processed)
	{
		bool is_last;
		bool is_valid;
		processed = true;
		switch (qtype)
		{
			case QueryStartBeacon:
				//repeat = processPacketHeader(bufflen, bpos);
				//onStartBeacon();
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onStartBeacon();
				break;
			case QueryStopBeacon:
				repeat = processPacketHeader(bufflen, bpos);
				onStopBeacon();
				break;
			case QueryMemberConfig:
				repeat = processPacketHeader(data, bufflen, bpos, is_last, is_valid);
				if (repeat && is_last && is_valid) onMemberConfig();
				break;
			default: processed = false;
		}
	}
	return repeat;
}

void MemberTcpClient::onStartBeacon()
{
	if (m_query_data.size() >= 2)
	{
		const char *qdata = m_query_data.constData();
		Data2Bytes len;
		memcpy(len.bytes, qdata, 2);
		QString crew_name;
		if (len.data > 0)
		{
			crew_name = QString(m_query_data.mid(2, len.data));
		}
		else
		{
			crew_name = "scheduler";
		}
		if (!m_gps_timer)
		{
			m_gps_timer = new QTimer();
			connect(m_gps_timer, SIGNAL(timeout()), this, SLOT(beacon()));
		}
		m_gps_timer->setSingleShot(false);
		if (m_config_inited)
		{
			m_gps_timer->setInterval(m_tracking_timeout * 1000);
			m_gps_timer->start();
		}
		m_beacon = true;
		QByteArray payload;
		payload.append((char)m_beacon);
		send(QueryBeaconState, payload);

		qDebug() << QString("start beacon id = %1; crew name = %2").arg(m_db_id).arg(crew_name).toLatin1().constData();
	}
}

void MemberTcpClient::onStopBeacon()
{
	qDebug() << QString("stop beacon id = %1").arg(m_db_id).toLatin1().constData();
	if (m_gps_timer)
	{
		delete m_gps_timer;
		m_gps_timer = 0;
	}
	m_beacon = false;
	QByteArray payload;
	payload.append((char)m_beacon);
	send(QueryBeaconState, payload);
}

void MemberTcpClient::onMemberConfig()
{
	if (m_query_data.size() >= 6)
	{
		const char *qdata = m_query_data.constData();
		m_tracking_allowed = (bool)(*qdata);
		qdata++;
		m_tracing_allowed = (bool)(*qdata);
		qdata++;
		Data4Bytes d;
		memcpy(d.bytes, qdata, 4);
		m_tracking_timeout = (int)d.data;
		m_config_inited = true;
		if (m_beacon)
		{
			if (!m_gps_timer)
			{
				m_gps_timer = new QTimer();
				connect(m_gps_timer, SIGNAL(timeout()), this, SLOT(beacon()));
			} else m_gps_timer->stop();
			m_gps_timer->setSingleShot(false);
			m_gps_timer->setInterval(m_tracking_timeout * 1000);
			m_gps_timer->start();
		}
	}
}

void MemberTcpClient::beacon()
{
	Data4Bytes t;
	Data8Bytes f;
	double latitude, longitude, speed;
	bool gps = true;
	QByteArray payload;
	t.data = QDateTime::currentDateTime().toTime_t();
	payload.append((const char*)t.bytes, 4);
	latitude = 50.0 + (((double)qrand() / (double)RAND_MAX) * 10.0);
	f.fdata = latitude;
	payload.append((const char*)f.bytes, 8);
	longitude = 48.0 + (((double)qrand() / (double)RAND_MAX) * 10.0);
	f.fdata = longitude;
	payload.append((const char*)f.bytes, 8);
	speed = 40.0 + (((double)qrand() / (double)RAND_MAX) * 30.0);
	f.fdata = speed;
	payload.append((const char*)f.bytes, 8);
	payload.append((char)gps);
	send(QueryLocation, payload);
	qDebug() << QString("%1(%2) beacon: %3").arg(m_handler.type == Crew ? "Crew" : "Member").arg(m_db_id).arg(QTime::currentTime().toString("hh:mm:ss zzz")).toLatin1().constData();

}
