#include <QtDebug>

#include <QDir>
#include <QFile>
#include <QDateTime>

#include "Resources.h"

ProgramConfig *Resources::config = 0;
QString Resources::host("127.0.0.1");
quint16 Resources::port = 7682;
QString Resources::avatars_path(".");
QRect Resources::window_geometry(0, 0, 1024, 768);
Qt::WindowState Resources::window_state = Qt::WindowNoState;
QString Resources::user_name;
QString Resources::password;
QString Resources::watcher_name;
unsigned int Resources::reconnection_interval = 15;
int Resources::current_crew = -1;
int Resources::current_member = -1;
QString Resources::current_member_name;
QVector<Avatar> Resources::avatars;

void Resources::init(ProgramConfig *cfg)
{
	config = cfg;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QString s;
			bool ok;
			QDomElement tag_window = tag_root.firstChildElement("window");
			if (!tag_window.isNull())
			{
				int g;
				s = tag_window.attribute("left");
				if (!s.isEmpty())
				{
					g = s.toInt(&ok);
					if (ok) window_geometry.setLeft(g);
				}
				s = tag_window.attribute("top");
				if (!s.isEmpty())
				{
					g = s.toInt(&ok);
					if (ok) window_geometry.setTop(g);
				}
				s = tag_window.attribute("width");
				if (!s.isEmpty())
				{
					g = s.toInt(&ok);
					if (ok) window_geometry.setWidth(g);
				}
				s = tag_window.attribute("height");
				if (!s.isEmpty())
				{
					g = s.toInt(&ok);
					if (ok) window_geometry.setHeight(g);
				}
				s = tag_window.attribute("state").trimmed().toLower();
				if (!s.isEmpty())
				{
					if (s == "minimized") window_state = Qt::WindowMinimized;
					else if (s == "maximized") window_state = Qt::WindowMaximized;
					else if (s == "full_screen") window_state = Qt::WindowFullScreen;
				}
			}
			QDomElement tag_server = tag_root.firstChildElement("server");
			if (!tag_server.isNull())
			{
				s = tag_server.attribute("host");
				if (!s.isEmpty()) host = s;
				s = tag_server.attribute("port");
				if (!s.isEmpty())
				{
					quint16 p = s.toUShort(&ok);
					if (ok) port = p;
				}
				s = tag_server.attribute("reconnection_interval");
				if (!s.isEmpty())
				{
					int t = s.toInt(&ok);
					if (ok) reconnection_interval = t;
				}
			}
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull())
			{
				s = tag_user.attribute("name");
				if (!s.isEmpty()) user_name = s;
				s = tag_user.attribute("password");
				if (!s.isEmpty()) password = s;
				s = tag_user.attribute("watcher");
				if (!s.isEmpty()) watcher_name = s;
				s = tag_user.attribute("path");
				if (!s.isEmpty()) avatars_path = s;
				s = tag_user.attribute("id");
				if (!s.isEmpty())
				{
					int ci = s.toInt(&ok);
					if (ok) current_crew = ci;
				}
				s = tag_user.attribute("member_id");
				if (!s.isEmpty())
				{
					int mi = s.toInt(&ok);
					if (ok) current_member = mi;
				}
				s = tag_user.attribute("member_name");
				if (!s.isEmpty()) current_member_name = s;
				QDomElement tag_avatar = tag_user.firstChildElement("avatar");
				while (!tag_avatar.isNull())
				{
					Avatar ava;
					ava.file_exists = false;
					ava.db_index = tag_avatar.attribute("id").toInt(&ok);
					if (ok)
					{
						s = tag_avatar.attribute("changetime");
						if (!s.isEmpty())
						{
							QDateTime dt = QDateTime::fromString(s, "yyyy-MM-dd hh:mm:ss");
							if (dt.isValid())
							{
								ava.changetime = dt.toTime_t();
								s = tag_avatar.attribute("type").trimmed().toLower();
								ava.type = ClientUnknown;
								if (s == "crew") ava.type =  Crew;
									else if (s == "member") ava.type =  Member;
								if (ava.type != ClientUnknown) avatars.append(ava);
							}
						}
					}
					tag_avatar = tag_avatar.nextSiblingElement("avatar");
				}
			}
		}
	}
}

void Resources::setHost(const QString &hst, int prt)
{
	host	= hst;
	port = prt;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_server = tag_root.firstChildElement("server");
			if (!tag_server.isNull())
			{
				tag_server.setAttribute("host", hst);
				tag_server.setAttribute("port", prt);
			}
		}
	}
}

void Resources::setLogon(const QString &usrname, const QString &pass)
{
	user_name = usrname;
	password = pass;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull())
			{
				tag_user.setAttribute("name", user_name);
				tag_user.setAttribute("password", password);
			}
		}
	}
}

void Resources::setWatcherName(const QString &watcher)
{
	watcher_name = watcher;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull()) tag_user.setAttribute("watcher", watcher_name);
		}
	}
}

void Resources::setAvatarsPath(const QString &path)
{
	avatars_path = path;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull()) tag_user.setAttribute("path", path);
		}
	}
}

void Resources::setWindowState(const QRect &g, Qt::WindowState ste)
{
	window_geometry = g;
	window_state = ste;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_window = tag_root.firstChildElement("window");
			if (!tag_window.isNull())
			{
				tag_window.setAttribute("left", window_geometry.left());
				tag_window.setAttribute("top", window_geometry.top());
				tag_window.setAttribute("width", window_geometry.width());
				tag_window.setAttribute("height", window_geometry.height());
				switch (window_state)
				{
					case Qt::WindowMinimized: tag_window.setAttribute("state", "minimized"); break;
					case Qt::WindowMaximized: tag_window.setAttribute("state", "maximized"); break;
					case Qt::WindowFullScreen: tag_window.setAttribute("state", "full_screen"); break;
					default: tag_window.setAttribute("state", "normal"); break;
				}
			}
		}
	}
}

void Resources::setCurrentMember(int id, const QString &name)
{
	current_member = id;
	current_member_name = name;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull())
			{
				tag_user.setAttribute("member_id", id);
				tag_user.setAttribute("member_name", name);
			}
		}
	}
}

void Resources::setCrewID(int id)
{
	current_crew = id;
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull()) tag_user.setAttribute("id", id);
		}
	}
}

Avatar Resources::findAvatar(ClientType type, int db_id)
{
	Avatar ret;
	ret.db_index = -1;
	ret.file_exists = false;
	QVectorIterator<Avatar> i(avatars);
	while (i.hasNext())
	{
		const Avatar &ava = i.next();
		if (ava.db_index == db_id && ava.type == type)
		{
			QString file_path = QString("%1/%2.%3.png").arg(Resources::avatars_path).arg(type == Crew ? "crew" : "member").arg(db_id);
			ret = ava;
			ret.file_exists = QFile::exists(file_path);
			break;
		}
	}
	return ret;
}

bool Resources::saveAvatar(ClientType type, int db_id, uint changetime, const QByteArray &data)
{
	QString stype = type == Crew ? "crew" : "member";
	QString file_path = QString("%1/%2.%3.png").arg(Resources::avatars_path).arg(stype).arg(db_id);
	QFile file(file_path);
	if (file.open(QIODevice::WriteOnly))
	{
		bool ok = file.write(data) >= 0;
		file.close();
		if (ok)
		{
			if (config)
			{
				QDomElement tag_root = config->document()->documentElement();
				if (!tag_root.isNull())
				{
					QDomElement tag_user = tag_root.firstChildElement("user");
					if (!tag_user.isNull())
					{
						QDateTime avatar_changetime;
						avatar_changetime.setTime_t(changetime);
						QString attr_id, attr_type;
						QString schangetime = avatar_changetime.toString("yyyy-MM-dd hh:mm:ss");
						QDomElement tag_avatar = tag_user.firstChildElement("avatar");
						ok = false;
						while (!tag_avatar.isNull())
						{
							attr_id = tag_avatar.attribute("id");
							attr_type = tag_avatar.attribute("type");
							if (attr_type == stype && attr_id.toInt() == db_id)
							{
								tag_avatar.setAttribute("changetime", schangetime);
								ok = true;
								break;
							}
							tag_avatar = tag_avatar.nextSiblingElement("avatar");
						}
						if (!ok)
						{
							tag_avatar = config->document()->createElement("avatar");
							tag_avatar.setAttribute("id", db_id);
							tag_avatar.setAttribute("type", stype);
							tag_avatar.setAttribute("changetime", schangetime);
							tag_user.appendChild(tag_avatar);
						}
						config->save();
					}
				}
			}
			ok = false;
			Avatar avatar;
			for (int i = 0; i < avatars.size(); i++)
			{
				avatar = avatars.at(i);
				if (avatar.db_index == db_id && avatar.type == type)
				{
					avatars[i].file_exists = true;
					avatars[i].changetime = changetime;
					ok = true;
				}
			}
			if (!ok)
			{
				avatar.db_index = db_id;
				avatar.type = type;
				avatar.file_exists = true;
				avatar.changetime = changetime;
				avatars.append(avatar);
			}
			return true;
		}
	}
	return false;
}

void Resources::clearAvatars()
{
	avatars.clear();
	if (config)
	{
		QDomElement tag_root = config->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_user = tag_root.firstChildElement("user");
			if (!tag_user.isNull())
			{
				QDomElement tag_ava = tag_user.firstChildElement("avatar");
				while (!tag_ava.isNull())
				{
					tag_user.removeChild(tag_ava);
					tag_ava = tag_user.firstChildElement("avatar");
				}
			}
		}
	}
	QString file_path;
	QDir dir(avatars_path);
	QStringList flist = dir.entryList(QDir::Files);
	foreach (QString fname, flist)
	{
		file_path = QString("%1/%2").arg(dir.absolutePath()).arg(fname);
		QFile::setPermissions(file_path, QFile::ReadOwner | QFile::WriteOwner);
		QFile::remove(file_path);
	}
}
