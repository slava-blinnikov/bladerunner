#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

#include "Resources.h"

class ConfigDialog : public QDialog
{
	Q_OBJECT
public:
	ConfigDialog(QWidget *parent = 0);
	QString host();
	qint16 port();
	QString crewName();
	QString password();
	QString watcher();

private:
	QLineEdit *m_le_host;
	QLineEdit *m_le_port;
	QLineEdit *m_le_user;
	QLineEdit *m_le_password;
	QLineEdit *m_le_watcher;
	QPushButton *m_pb_ok;
	QPushButton *m_pb_cancel;

private slots:
	void okClicked();
};

#endif // CONFIGDIALOG_H
