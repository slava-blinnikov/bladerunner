#include <QFile>
#include <QTextStream>
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
	m_document = new QDomDocument();
	if (!m_document->setContent(&file, true))
	{
		delete m_document;
		m_document = 0;
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
