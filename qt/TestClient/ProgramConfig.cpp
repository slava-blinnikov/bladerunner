#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QApplication>

#include "ProgramConfig.h"

ProgramConfig::ProgramConfig(const QString &fn)
{
	m_filename = fn;
	m_document = 0;
}

bool ProgramConfig::read()
{
	if (m_document)
	{
		delete m_document;
		m_document = 0;
	}
	QFile file(m_filename);
	QString errorStr;
	int errorLine;
	int errorColumn;
	m_document = new QDomDocument();
	if (!m_document->setContent(&file, true, &errorStr, &errorLine, &errorColumn))
	{
		delete m_document;
		m_document = 0;
		QMessageBox::critical(0, qApp->applicationName(), QObject::tr("Ошибка разбора файла-конфигурации!\n%1\nстрока: %2\nстолбец: %3").arg(errorStr).arg(errorLine).arg(errorColumn));
		return false;
	}
	return true;
}

bool ProgramConfig::save()
{
	if (m_document)
	{
		QFile file(m_filename);
		file.open(QIODevice::WriteOnly);
		QTextStream out(&file);
		m_document->save(out, 4);
		return true;
	}
	return false;
}

QDomDocument *ProgramConfig::document()
{
	return m_document;
}

QString ProgramConfig::getCDATA(const QDomElement &tag)
{
	QDomNode snode;
	QDomCDATASection cdata;
	snode = tag.firstChild();
	if (!snode.isNull())
	{
		if (snode.isCDATASection())
		{
			cdata = snode.toCDATASection();
			if (!cdata.isNull()) return cdata.data();
		}
	}
	return QString();
}
