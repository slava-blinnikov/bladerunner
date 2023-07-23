#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>
#include <QIntValidator>
#include <QMessageBox>

#include "ConfigDialog.h"

ConfigDialog::ConfigDialog(QWidget *parent) : QDialog(parent)
{
	setWindowTitle(tr("Настройки"));
	QVBoxLayout *vbl_central = new QVBoxLayout();
	QLabel *lbl;
	QGroupBox *gb;
	QVBoxLayout *vbl;
	QHBoxLayout *hbl;
	gb = new QGroupBox(tr("Сервер"), this);
	hbl = new QHBoxLayout();
	vbl = new QVBoxLayout();
	lbl = new QLabel(tr("Хост:"), this);
	m_le_host = new QLineEdit(this);
	vbl->addWidget(lbl);
	vbl->addWidget(m_le_host);
	vbl->addStretch();
	hbl->addLayout(vbl);
	hbl->setStretchFactor(vbl, 5);
	vbl = new QVBoxLayout();
	lbl = new QLabel(tr("Порт:"), this);
	m_le_port = new QLineEdit(this);
	m_le_port->setValidator(new QIntValidator(1, 65535));
	vbl->addWidget(lbl);
	vbl->addWidget(m_le_port);
	vbl->addStretch();
	hbl->addLayout(vbl);
	hbl->setStretchFactor(vbl, 1);
	gb->setLayout(hbl);
	vbl_central->addWidget(gb);
	gb = new QGroupBox(tr("Семья"), this);
	QVBoxLayout *vbl_gb = new QVBoxLayout();
	hbl = new QHBoxLayout();
	vbl = new QVBoxLayout();
	lbl = new QLabel(tr("Имя:"), this);
	m_le_user = new QLineEdit(this);
	vbl->addWidget(lbl);
	vbl->addWidget(m_le_user);
	hbl->addLayout(vbl);
	hbl->setStretchFactor(vbl, 2);
	vbl = new QVBoxLayout();
	lbl = new QLabel(tr("Пароль:"), this);
	m_le_password = new QLineEdit(this);
	m_le_password->setEchoMode(QLineEdit::PasswordEchoOnEdit);
	vbl->addWidget(lbl);
	vbl->addWidget(m_le_password);
	hbl->addLayout(vbl);
	hbl->setStretchFactor(vbl, 1);
	vbl_gb->addLayout(hbl);
	lbl = new QLabel(tr("Представляться как:"), this);
	m_le_watcher = new QLineEdit(this);
	vbl_gb->addWidget(lbl);
	vbl_gb->addWidget(m_le_watcher);
	vbl_gb->addStretch();
	gb->setLayout(vbl_gb);
	vbl_central->addWidget(gb);
	vbl_central->addStretch();
	hbl = new QHBoxLayout();
	hbl->addStretch();
	m_pb_ok = new QPushButton(tr("Ok"), this);
	hbl->addWidget(m_pb_ok);
	m_pb_cancel = new QPushButton(tr("Отмена"), this);
	hbl->addWidget(m_pb_cancel);
	vbl_central->addLayout(hbl);
	setLayout(vbl_central);

	m_le_host->setText(Resources::host);
	m_le_port->setText(QString::number(Resources::port));
	m_le_user->setText(Resources::user_name);
	m_le_password->setText(Resources::password);
	m_le_watcher->setText(Resources::watcher_name);

	connect(m_pb_ok, SIGNAL(clicked(bool)), this, SLOT(okClicked()));
	connect(m_pb_cancel, SIGNAL(clicked(bool)), this, SLOT(reject()));
}

QString ConfigDialog::host()
{
	return m_le_host->text().trimmed();
}

qint16 ConfigDialog::port()
{
	return m_le_port->text().toInt();
}

QString ConfigDialog::crewName()
{
	return m_le_user->text().trimmed();
}

QString ConfigDialog::password()
{
	return m_le_password->text().trimmed();
}

QString ConfigDialog::watcher()
{
	return m_le_watcher->text().trimmed();
}

void ConfigDialog::okClicked()
{
	QString s;
	s = m_le_host->text().trimmed();
	if (s.isEmpty())
	{
		QMessageBox::critical(this, windowTitle(), tr("Укажите адрес сервера!"));
		m_le_host->setFocus();
		return;
	}
	s = m_le_port->text().trimmed();
	if (s.isEmpty() || s.toInt() <= 0)
	{
		QMessageBox::critical(this, windowTitle(), tr("Укажите корректный порт сервера!"));
		m_le_port->setFocus();
		return;
	}
	s = m_le_user->text().trimmed();
	if (s.isEmpty())
	{
		QMessageBox::critical(this, windowTitle(), tr("Укажите имя!"));
		m_le_user->setFocus();
		return;
	}
	s = m_le_password->text().trimmed();
	if (s.isEmpty())
	{
		QMessageBox::critical(this, windowTitle(), tr("Укажите пароль!"));
		m_le_password->setFocus();
		return;
	}
	accept();
}
