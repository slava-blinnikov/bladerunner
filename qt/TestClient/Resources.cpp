#include "Resources.h"

ProgramConfig *Resources::cfg = 0;
QString Resources::host("127.0.0.1");
int Resources::port = 7682;
ServerPresetList Resources::server_presets;
ClientPresetList Resources::client_presets;
QString Resources::users_path;


void Resources::init(ProgramConfig *config)
{
	Resources::cfg = config;
	if (config)
	{
		QDomElement tag_root = Resources::cfg->document()->documentElement();
		if (!tag_root.isNull())
		{
			QDomElement tag_server = tag_root.firstChildElement("server");
			if (!tag_server.isNull())
			{
				QString s;
				s = tag_server.attribute("host");
				if (!s.isEmpty()) Resources::host = s;
				s = tag_server.attribute("port");
				if (!s.isEmpty())
				{
					bool ok;
					int p = s.toInt(&ok);
					if (ok) Resources::port = p;
				}
				QDomElement tag_preset = tag_server.firstChildElement("preset");
				while (!tag_preset.isNull())
				{
					ServerHandler prs;
					prs.host = tag_preset.attribute("host");
					prs.port = tag_preset.attribute("port").toInt();
					Resources::server_presets.append(prs);
					tag_preset = tag_preset.nextSiblingElement("preset");
				}
			}
			QDomElement tag_clients = tag_root.firstChildElement("clients");
			if (!tag_clients.isNull())
			{
				QString s;
				QDomElement tag_preset = tag_clients.firstChildElement("preset");
				while (!tag_preset.isNull())
				{
					s = tag_preset.attribute("type").trimmed().toLower();
					if  (s == "crew" || s == "member")
					{
						ClientHandler prs;
						prs.type = s == "crew" ? Crew : Member;
						prs.crew = tag_preset.attribute("crew");
						prs.name = tag_preset.attribute("name");
						prs.password = tag_preset.attribute("password");
						Resources::client_presets.append(prs);
					}
					tag_preset = tag_preset.nextSiblingElement("preset");
				}
			}
			QDomElement tag_users = tag_root.firstChildElement("users");
			if (!tag_users.isNull())
			{
				users_path = tag_users.attribute("path");
			}
		}
	}
}

bool Resources::applayHost(const QString &hst, int prt)
{
	Resources::host = hst;
	Resources::port = prt;
	bool ok = true;
	for (int i = 0; i < Resources::server_presets.size(); i++)
	{
		const ServerHandler &srv = Resources::server_presets.at(i);
		if (host == srv.host.trimmed().toLower() && port == srv.port)
		{
			ok = false;
			break;
		}
	}
	QDomElement tag_root = Resources::cfg->document()->documentElement();
	if (!tag_root.isNull())
	{
		QDomElement tag_server = tag_root.firstChildElement("server");
		if (!tag_server.isNull())
		{
			tag_server.setAttribute("host", hst);
			tag_server.setAttribute("port", prt);
			if (ok)
			{
				ServerHandler srv;
				srv.host = hst;
				srv.port = prt;
				Resources::server_presets.append(srv);
				QDomElement tag_preset = Resources::cfg->document()->createElement("preset");
				if (!tag_preset.isNull())
				{
					tag_preset.setAttribute("host", hst);
					tag_preset.setAttribute("port", prt);
					tag_server.appendChild(tag_preset);
				}
			}
			Resources::cfg->save();
		}
	}
	return ok;
}

bool Resources::getAvatarHandler(AvatarHandler *hnd, ClientType ctype, int id)
{
	QDomElement tag_root = Resources::cfg->document()->documentElement();
	if (!tag_root.isNull())
	{
		QDomElement tag_users = tag_root.firstChildElement("users");
		if (!tag_users.isNull())
		{
			QString path = tag_users.attribute("path");
			QDomElement tag_user = tag_users.firstChildElement("user");
			while (!tag_user.isNull())
			{
				ClientType ct = tag_user.attribute("type") == "crew" ? Crew : Member;
				if (ctype == ct)
				{
					bool ok;
					QString s = tag_user.attribute("id");
					int tid = s.toInt(&ok);
					if (ok && tid == id)
					{
						hnd->filename = QString("%1/%2").arg(path).arg(tag_user.attribute("avatar_file"));
						hnd->changetime = QDateTime::fromString(tag_user.attribute("avatar_changetime"), "yyyy-MM-dd hh:mm:ss");
						return true;
					}
				}
				tag_user = tag_user.nextSiblingElement("user");
			}
		}
	}
	return false;
}
