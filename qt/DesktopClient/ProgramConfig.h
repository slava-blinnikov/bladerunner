#ifndef PROGRAMCONFIG_H
#define PROGRAMCONFIG_H

#include <QDomDocument>
#include <QDomElement>

class ProgramConfig
{
public:
	ProgramConfig(const QString &fn);
	bool read();
	bool save();
	QDomDocument *document();

protected:
	QString m_filename;
	QDomDocument *m_document;
};

#endif // PROGRAMCONFIG_H
