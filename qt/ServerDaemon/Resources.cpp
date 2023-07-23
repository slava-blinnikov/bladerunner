#include <QFile>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNode>
#include <QDomCDATASection>

#include "Resources.h"

qint16 Resources::server_port = 7682;
DbConnection *Resources::db = 0;
int Resources::login_timeout = 15;
int Resources::ping_timeout = 15;
int Resources::schedule_timeout = 60;
QMap<QString, QString> Resources::queries;
QString Resources::www_path;

bool Resources::init(const QString &cfg_filename)
{
	if (!cfg_filename.isEmpty())
	{
		QFile cfg_file(cfg_filename);
		if (cfg_file.exists())
		{
			QString error_str;
			int error_line;
			int error_column;
			QDomDocument cfg_document;
			if (cfg_document.setContent(&cfg_file, true, &error_str, &error_line, &error_column))
			{
				QDomElement tag_root = cfg_document.documentElement();
				if (!tag_root.isNull())
				{
					QString str;
					int v;
					bool ok;
					QDomElement tag_server = tag_root.firstChildElement("server");
					if (!tag_server.isNull())
					{
						str = tag_server.attribute("port");
						if (!str.isEmpty())
						{
							v = str.toInt(&ok);
							if (ok) server_port = v;
						}
						str = tag_server.attribute("login_timeout");
						if (!str.isEmpty())
						{
							v = str.toInt(&ok);
							if (ok) login_timeout = v;
						}
						str = tag_server.attribute("ping_timeout");
						if (!str.isEmpty())
						{
							v = str.toInt(&ok);
							if (ok) ping_timeout = v;
						}
						str = tag_server.attribute("schedule_timeout");
						if (!str.isEmpty())
						{
							v = str.toInt(&ok);
							if (ok) schedule_timeout = v;
						}
						www_path = tag_server.attribute("www_path");
					}
					QDomElement tag_database = tag_root.firstChildElement("database");
					if (!tag_database.isNull())
					{
						QString host  = tag_database.attribute("host", "localhost");
						int port = tag_database.attribute("port", "5432").toInt();
						QString name = tag_database.attribute("name", "bladerunner");
						QString user = tag_database.attribute("user", "dispatcher");
						QString password = tag_database.attribute("password", "qwerty");
						db = new DbConnection(host, port, name, user, password, "dispather");

						QDomNode snode;
						QDomCDATASection cdata;
						QDomElement tag_qry = tag_database.firstChildElement("query");
						while (!tag_qry.isNull())
						{
							snode = tag_qry.firstChild();
							if (!snode.isNull())
							{
								if (snode.isCDATASection())
								{
									cdata = snode.toCDATASection();
									if (!cdata.isNull())
									{
										queries[tag_qry.attribute("type")] = cdata.data();
									}
								}
							}
							tag_qry = tag_qry.nextSiblingElement("query");
						}
					}
					return true;
				}
			}
		}
	}
	return false;
}

void Resources::clear()
{
	if (db)
	{
		db->close();
		delete db;
		db = 0;
	}
	queries.clear();
}
