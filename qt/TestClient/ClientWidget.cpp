#include <QVBoxLayout>
#include <QHBoxLayout>

#include "ClientWidget.h"

ClientWidget::ClientWidget(BlrClient *client, QWidget *parent) : QWidget(parent)
{
	m_client = client;
	connect(m_client, SIGNAL(connected()), this, SLOT(onConnected()));
	connect(m_client, SIGNAL(disconnected()), this, SLOT(onDisconnected()));
	connect(m_client, SIGNAL(authorized()), this, SLOT(onAuthorized()));
	connect(m_client, SIGNAL(avatarChanged(QString)), this, SLOT(setAvatar(QString)));

	QHBoxLayout *hbl;
	QVBoxLayout *vbl_central = new QVBoxLayout();

	hbl = new QHBoxLayout();
	m_lbl_ava = new QLabel(this);
	m_lbl_name = new QLabel(this);
	m_lbl_name->setStyleSheet("font: bold 16px; color: red;");
	m_lbl_ava->setPixmap(QIcon((client->type() == Crew) ? ":/images/no_crew_avatar.png" : ":/images/no_member_avatar.png").pixmap(80, 80));
	if (client->type() == Crew) m_lbl_name->setText(client->crew()); else m_lbl_name->setText(QString("%1.%2").arg(client->crew()).arg(client->name()));
	hbl->addWidget(m_lbl_ava);
	hbl->addWidget(m_lbl_name);
	hbl->addStretch();
	m_btn_connect = new QPushButton(QIcon(":/images/bullet_black.png"), tr("Connect"), this);
	hbl->addWidget(m_btn_connect);
	QPushButton *btn_close = new QPushButton(tr("Close"), this);
	hbl->addWidget(btn_close);
	vbl_central->addLayout(hbl);

	m_workarea = new QWidget(this);
	vbl_central->addWidget(m_workarea);
	setLayout(vbl_central);

	connect(m_btn_connect, SIGNAL(clicked()), this, SLOT(switchConnectionState()));
	connect(btn_close, SIGNAL(clicked()), this, SIGNAL(closeMe()));
}

ClientWidget::~ClientWidget()
{
	delete m_client;
}

void ClientWidget::start()
{
	m_client->start();
}

void ClientWidget::onConnected()
{
	m_btn_connect->setIcon(QIcon(":/images/bullet_green.png"));
	m_btn_connect->setText(tr("Disconnect"));
}

void ClientWidget::onDisconnected()
{
	m_btn_connect->setIcon(QIcon(":/images/bullet_black.png"));
	m_btn_connect->setText(tr("Connect"));
	m_lbl_name->setStyleSheet("font: bold 16px; color: red;");
}

void ClientWidget::onAuthorized()
{
	m_lbl_name->setStyleSheet("font: bold 16px; color: green;");
}

void ClientWidget::switchConnectionState()
{
	if (m_client->isConnected()) m_client->stop(); else m_client->start();
}

void ClientWidget::setAvatar(const QString &filename)
{
	m_lbl_ava->setPixmap(QIcon(filename).pixmap(80, 80));
}

