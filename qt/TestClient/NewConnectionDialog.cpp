#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "NewConnectionDialog.h"
#include "Resources.h"

NewConnectionDialog::NewConnectionDialog(QWidget *parent) : QDialog(parent, Qt::Dialog)
{
	setWindowTitle(QString("%1 - %2").arg(parent->windowTitle()).arg(tr("New connection")));
	QVBoxLayout *vbl_central = new QVBoxLayout();
	QLabel *lbl;

	lbl = new QLabel(tr("Connection type:"), this);
	vbl_central->addWidget(lbl);
	m_cb_type = new QComboBox(this);
	m_cb_type->addItem(tr("Crew"));
	m_cb_type->addItem(tr("Member"));
	for (int i = 0; i < Resources::client_presets.size(); i++)
	{
		const ClientHandler &cs = Resources::client_presets.at(i);
		m_cb_type->addItem(QString("%1: %2")
				   .arg(cs.type == Crew ? tr("Crew") : tr("Member"))
				   .arg(cs.type == Crew ? cs.crew : QString("%1.%2").arg(cs.crew).arg(cs.name)));
	}
	vbl_central->addWidget(m_cb_type);
	vbl_central->addSpacing(10);

	lbl = new QLabel(tr("Crew:"), this);
	vbl_central->addWidget(lbl);
	m_le_crew = new QLineEdit(this);
	vbl_central->addWidget(m_le_crew);
	vbl_central->addSpacing(10);

	m_wdg_member = new QWidget(this);
	QVBoxLayout *vbl = new QVBoxLayout();
	vbl->setMargin(0);
	lbl = new QLabel(tr("Member:"), m_wdg_member);
	vbl->addWidget(lbl);
	m_le_member = new QLineEdit(m_wdg_member);
	vbl->addWidget(m_le_member);
	m_wdg_member->setLayout(vbl);
	vbl_central->addWidget(m_wdg_member);
	vbl_central->addSpacing(10);

	lbl = new QLabel(tr("Password:"), this);
	vbl_central->addWidget(lbl);
	m_le_password = new QLineEdit(this);
	vbl_central->addWidget(m_le_password);

	vbl_central->addStretch();

	QHBoxLayout *hbl = new QHBoxLayout();
	hbl->addStretch();
	QPushButton *btn_ok = new QPushButton(tr("Ok"), this);
	hbl->addWidget(btn_ok);
	QPushButton *btn_cancel = new QPushButton(tr("Cancel"), this);
	hbl->addWidget(btn_cancel);
	vbl_central->addLayout(hbl);

	setLayout(vbl_central);
	resize(600, 300);

	m_wdg_member->setVisible(false);

	connect(m_cb_type, SIGNAL(currentIndexChanged(int)), this, SLOT(clientTypeChanged(int)));
	connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(btn_ok, SIGNAL(clicked()), this, SLOT(accept()));
}

ClientType NewConnectionDialog::clientType()
{
	int idx = m_cb_type->currentIndex();
	if (idx < 2)
	{
		return idx == 1 ? Member : Crew;
	}
	else
	{
		idx -= 2;
		return Resources::client_presets.at(idx).type;
	}
}

QString NewConnectionDialog::crewName()
{
	return m_le_crew->text().trimmed();
}

QString NewConnectionDialog::memberName()
{
	if (clientType() == Crew) return crewName();
	return m_le_member->text().trimmed();
}

QString NewConnectionDialog::clientPassword()
{
	return m_le_password->text().trimmed();
}

void NewConnectionDialog::clientTypeChanged(int idx)
{
	if (idx < 2)
	{
		m_wdg_member->setVisible(idx == 1);
		m_le_crew->clear();
		m_le_member->clear();
		m_le_password->clear();
	}
	else
	{
		idx -= 2;
		const ClientHandler &cs = Resources::client_presets.at(idx);
		m_wdg_member->setVisible(cs.type == Member);
		m_le_crew->setText(cs.crew);
		m_le_member->setText(cs.name);
		m_le_password->setText(cs.password);
	}
}
