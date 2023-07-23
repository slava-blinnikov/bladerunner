#include<QtDebug>

#include<QHBoxLayout>
#include<QVBoxLayout>

#include "MemberWidget.h"

MemberWidget::MemberWidget(const MemberRecord &member, QWidget *parent) : QWidget(parent)
{
	m_db_index = member.db_index;
	m_pxm_online = QPixmap(":images/bullet_green.png");
	m_pxm_offline = QPixmap(":images/bullet_red.png");
	m_pxm_allow = QPixmap(":images/tick.png");
	m_pxm_notallow = QPixmap(":images/cross.png");
	QVBoxLayout *vbl;
	QHBoxLayout *hbl;

	QHBoxLayout *hbl_main = new QHBoxLayout();
	hbl_main->setMargin(5);
	hbl_main->setContentsMargins(0, 0, 0, 15);

	m_btn_avatar = new QPushButton(this);
	m_btn_avatar->setIconSize(QSize(80, 80));
	m_btn_avatar->setFlat(true);
	Avatar ava = Resources::findAvatar(Member, m_db_index);
	if (ava.db_index >= 0 && ava.file_exists) m_btn_avatar->setIcon(QIcon(QString("%1/member.%2.png").arg(Resources::avatars_path).arg(m_db_index)));
		else	m_btn_avatar->setIcon(QIcon(":images/no_member_avatar.png"));
	hbl_main->addWidget(m_btn_avatar);

	vbl = new QVBoxLayout();
	vbl->setMargin(0);
	vbl->setContentsMargins(0, 0, 0, 0);

	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	m_lbl_online = new QLabel(this);
	m_lbl_online->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_lbl_online->setPixmap(member.is_online ? m_pxm_online : m_pxm_offline);
	hbl->addWidget(m_lbl_online);
	m_lbl_name = new QLabel(member.name, this);
	m_lbl_name->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	hbl->addWidget(m_lbl_name);
	vbl->addLayout(hbl);

	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	m_lbl_trallowed = new QLabel(this);
	m_lbl_trallowed->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_lbl_trallowed->setPixmap(member.tracking_allowed ? m_pxm_allow : m_pxm_notallow);
	hbl->addWidget(m_lbl_trallowed);
	m_lbl_tratitle = new QLabel(member.tracking_allowed ? tr("Слежение разрешено") : tr("Слежение запрещено"), this);
	m_lbl_tratitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	hbl->addWidget(m_lbl_tratitle);
	vbl->addLayout(hbl);

	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	m_lbl_recording = new QLabel(this);
	m_lbl_recording->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_lbl_recording->setPixmap(member.is_tracing ? m_pxm_allow : m_pxm_notallow);
	hbl->addWidget(m_lbl_recording);
	m_lbl_rectitle = new QLabel(member.is_tracing ? tr("Запись разрешена") : tr("Запись запрещена"), this);
	m_lbl_rectitle->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	hbl->addWidget(m_lbl_rectitle);
	vbl->addLayout(hbl);

	hbl_main->addLayout(vbl);

	hbl_main->addStretch();
	setLayout(hbl_main);
	connect(m_btn_avatar, SIGNAL(clicked()), this, SLOT(onAvaClicked()));
}

int MemberWidget::dbIndex()
{
	return m_db_index;
}

void MemberWidget::updateName(const QString &name)
{
	m_lbl_name->setText(name);
}

void MemberWidget::updateOnline(bool is_online)
{
	m_lbl_online->setPixmap(is_online ? m_pxm_online : m_pxm_offline);
}

void MemberWidget::updateTrackingPermission(bool is_allowed)
{
	m_lbl_trallowed->setPixmap(is_allowed ? m_pxm_allow : m_pxm_notallow);
	m_lbl_tratitle->setText(is_allowed ? tr("Слежение разрешено") : tr("Слежение запрещено"));
}

void MemberWidget::updateRecordingPermission(bool is_allowed)
{
	m_lbl_recording->setPixmap(is_allowed ? m_pxm_allow : m_pxm_notallow);
	m_lbl_rectitle->setText(is_allowed ? tr("Запись разрешена") : tr("Запись запрещена"));
}

void MemberWidget::updateAvatar()
{
	Avatar ava = Resources::findAvatar(Member, m_db_index);
	if (ava.db_index >= 0 && ava.file_exists) m_btn_avatar->setIcon(QIcon(QString("%1/member.%2.png").arg(Resources::avatars_path).arg(m_db_index)));
		else	m_btn_avatar->setIcon(QIcon(":images/no_member_avatar.png"));
}

void MemberWidget::mouseReleaseEvent(QMouseEvent *event)
{
	QWidget::mouseReleaseEvent(event);
	onAvaClicked();
}

void MemberWidget::onAvaClicked()
{
	emit memberSelected(m_db_index);
}

