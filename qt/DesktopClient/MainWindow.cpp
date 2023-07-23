#include  <QtDebug>

#include  <QHBoxLayout>
#include  <QVBoxLayout>

#include "MainWindow.h"
#include "ConfigDialog.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Семья Рядом"));
	m_is_online = false;
	m_is_activated = false;
	m_is_current_member_online = false;
	m_is_marker_visible = false;
	m_icn_no_member = QIcon(":images/no_member_avatar.png");
	m_pxm_online = QPixmap(":images/bullet_green.png");
	m_pxm_offline = QPixmap(":images/bullet_red.png");
	QVBoxLayout *vbl;
	QHBoxLayout *hbl;

	QWidget *wdg_central = new QWidget(this);
	QVBoxLayout *vbl_central = new QVBoxLayout();
	vbl_central->setMargin(0);
	vbl_central->setContentsMargins(0, 0, 0, 0);

	QSize ava_size(60, 60);
	QHBoxLayout *hbl_tools = new QHBoxLayout();

	m_btn_member = new QPushButton(wdg_central);
	m_btn_member->setEnabled(false);
	m_btn_member->setIconSize(ava_size);
	m_btn_member->setFlat(true);
	bool ok = false;
	if (Resources::current_member >= 0)
	{
		Avatar ava = Resources::findAvatar(Member, Resources::current_member);
		if (ava.file_exists)
		{
			m_btn_member->setIcon(QIcon(QString("%1/member.%2.png").arg(Resources::avatars_path).arg(Resources::current_member)));
			ok = true;
		}
	}
	if (!ok) m_btn_member->setIcon(m_icn_no_member);
	hbl_tools->addWidget(m_btn_member);

	hbl = new QHBoxLayout();
	hbl->setMargin(0);
	hbl->setContentsMargins(0, 0, 0, 0);
	m_lbl_online = new QLabel(wdg_central);
	m_lbl_online->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_lbl_online->setPixmap(m_pxm_offline);
	hbl->addWidget(m_lbl_online);
	m_lbl_name = new QLabel(Resources::current_member_name, wdg_central);
	m_lbl_name->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	hbl->addWidget(m_lbl_name);
	hbl_tools->addLayout(hbl);

	hbl_tools->addSpacing(20);

	m_lbl_speed = new QLabel(wdg_central);
	QFont fnt("Courier New", 25, QFont::Bold, false);
	m_lbl_speed->setFont(fnt);
	hbl_tools->addWidget(m_lbl_speed);

	hbl_tools->addStretch();
	QIcon icn_crew(":images/no_crew_avatar.png");
	if (Resources::current_crew >= 0)
	{
		Avatar ava = Resources::findAvatar(Crew, Resources::current_crew);
		if (ava.file_exists) icn_crew= QIcon(QString("%1/crew.%2.png").arg(Resources::avatars_path).arg(Resources::current_crew));
	}
	m_btn_crew = new QPushButton(icn_crew, QString(), wdg_central);
	m_btn_crew->setIconSize(ava_size);
	m_btn_crew->setFlat(true);
	hbl_tools->addWidget(m_btn_crew);

	vbl_central->addLayout(hbl_tools);
	m_map = new MapView(wdg_central);
	vbl_central->addWidget(m_map);

	wdg_central->setLayout(vbl_central);
	setCentralWidget(wdg_central);

	m_sca_members = new QScrollArea(m_map);
	m_sca_members->setVisible(false);
	m_wgd_members = new QWidget(m_sca_members);
	vbl = new QVBoxLayout();
	vbl->setMargin(0);
	vbl->setContentsMargins(0, 0, 0, 0);
	vbl->addStretch();
	m_wgd_members->setLayout(vbl);
	m_sca_members->setMinimumWidth(300);
	connect(m_btn_member, SIGNAL(clicked()), this, SLOT(showMembersDialog()));
	m_sca_members->setWidget(m_wgd_members);
	connect(m_btn_crew, SIGNAL(clicked()), this, SLOT(showConfigDialog()));
}

void MainWindow::showWindow()
{
	setGeometry(Resources::window_geometry);
	switch (Resources::window_state)
	{
		case Qt::WindowMinimized: showMinimized(); break;
		case Qt::WindowMaximized: showMaximized(); break;
		case Qt::WindowFullScreen: showFullScreen(); break;
		default: showNormal(); break;
	}
	m_map->fitMapSize();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	Qt::WindowState state = Qt::WindowNoState;
	Qt::WindowStates sts = windowState();
	if (sts & Qt::WindowMinimized) state = Qt::WindowMinimized;
	else if (sts & Qt::WindowMaximized) state = Qt::WindowMaximized;
	else if (sts & Qt::WindowFullScreen) state = Qt::WindowFullScreen;
	Resources::setWindowState(geometry(), state);
	Resources::config->save();
	QMainWindow::closeEvent(event);
}

void MainWindow::showEvent(QShowEvent *event)
{
	if (!m_is_activated)
	{
		m_is_activated = true;
		emit applicationActivated();
	}
	QMainWindow::showEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
	if (m_wgd_members->isVisible()) updateMembersDialogGeometry();
}

void MainWindow::updateMembersDialogGeometry()
{
	QRect g = m_sca_members->geometry();
	g.setRect(0, 0, g.width(), m_map->height());
	m_sca_members->setGeometry(g);
}

void MainWindow::showMembersDialog()
{
	if (!m_sca_members->isVisible())
	{
		updateMembersDialogGeometry();
		m_sca_members->setVisible(true);
	}
	else m_sca_members->setVisible(false);
}

void MainWindow::showConfigDialog()
{
	ConfigDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		QString new_host = dlg.host();
		qint16 new_port = dlg.port();
		QString new_crew = dlg.crewName();
		QString new_password = dlg.password();
		QString new_watcher = dlg.watcher();
		if (new_host != Resources::host ||
			new_port != Resources::port ||
			new_crew != Resources::user_name ||
			new_password != Resources::password ||
			new_watcher != Resources::watcher_name)
		{
			Resources::setHost(new_host, new_port);
			Resources::setLogon(new_crew, new_password);
			Resources::setWatcherName(new_watcher);
			Resources::setCrewID(-1);
			Resources::setCurrentMember(-1, "");
			Resources::clearAvatars();
			m_btn_member->setIcon(m_icn_no_member);
			m_lbl_name->clear();
			clientDisconnected();
			emit reconnect();
		}
	}
}

void MainWindow::clientConnected()
{
	m_is_online = true;
	m_btn_member->setEnabled(true);
	m_lbl_online->setEnabled(true);
	m_lbl_name->setEnabled(true);
}

void MainWindow::clientDisconnected()
{
	m_sca_members->setVisible(false);
	qDeleteAll(m_mbr_widgets);
	m_mbr_widgets.clear();
	m_btn_member->setEnabled(false);
	m_is_online = false;
	m_is_current_member_online = false;
	m_lbl_online->setEnabled(false);
	m_lbl_name->setEnabled(false);
	m_lbl_online->setPixmap(m_pxm_offline);
	m_lbl_speed->clear();
	m_is_marker_visible = false;
	m_map->hideMarker();
}

void MainWindow::clientAvatarChanged(int client_type, int db_id)
{
	if (client_type == Member)
	{
		Avatar ava = Resources::findAvatar(Member, db_id);
		if (ava.file_exists)
		{
			if (Resources::current_member == db_id) m_btn_member->setIcon(QIcon(QString("%1/member.%2.png").arg(Resources::avatars_path).arg(db_id)));
			MemberWidget *wdg;
			for (int i = 0; i < m_mbr_widgets.size(); i++)
			{
				wdg = m_mbr_widgets.at(i);
				if (wdg->dbIndex() == db_id)
				{
					wdg->updateAvatar();
					break;
				}
			}
		}
	}
	else
	{
		if (Resources::current_crew == db_id)
		{
			Avatar ava = Resources::findAvatar(Crew, db_id);
			if (ava.file_exists) m_btn_crew->setIcon(QIcon(QString("%1/crew.%2.png").arg(Resources::avatars_path).arg(db_id)));
		}
	}
}

void MainWindow::clientMemberList(const IntList &members)
{
	m_members.clear();
	int n = members.size();
	if (n > 0)
	{
		int id;
		for (int i = 0; i < n; i++)
		{
			id = members.at(i);
			MemberRecord member;
			member.db_index = id;
			member.is_online = false;
			member.tracking_allowed = false;
			member.tracking_period = -1;
			member.is_tracing = false;
			m_members[id] = member;
		}
	}
	qDeleteAll(m_mbr_widgets);
	m_mbr_widgets.clear();
	QVBoxLayout *vbl = (QVBoxLayout *)(m_wgd_members->layout());
	QHashIterator<int, MemberRecord> i(m_members);
	int h = 0;
	while (i.hasNext())
	{
		i.next();
		MemberWidget *wdg = new MemberWidget(i.value(), m_wgd_members);
		vbl->insertWidget(0, wdg);
		connect(wdg, SIGNAL(memberSelected(int)), this, SLOT(setCurrentMember(int)));
		connect(wdg, SIGNAL(memberSelected(int)), this, SLOT(showMembersDialog()));
		m_mbr_widgets.append(wdg);
		h += wdg->minimumSizeHint().height();
	}
	m_wgd_members->setGeometry(0,0, m_sca_members->width() - 15, h);
}

void MainWindow::clientMemberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool tracking_allowed, bool tracing_allowed, int trctimeout)
{
	Q_UNUSED(avatime)
	if (m_members.contains(member_id))
	{
		MemberRecord member = m_members[member_id];
		member.name = usrname;
		member.note = note;
		member.tracking_allowed = tracking_allowed;
		member.is_tracing = tracing_allowed;
		member.tracking_period = trctimeout;
		m_members[member_id] = member;
	}
	if (member_id == Resources::current_member)
	{
		m_lbl_name->setText(usrname);
		m_lbl_name->setToolTip(note);
	}
	MemberWidget *wdg;
	for (int i = 0; i < m_mbr_widgets.size(); i++)
	{
		wdg = m_mbr_widgets.at(i);
		if (wdg->dbIndex() == member_id)
		{
			wdg->updateName(usrname);
			wdg->updateTrackingPermission(tracking_allowed);
			wdg->updateRecordingPermission(tracing_allowed);
			break;
		}
	}
}

void MainWindow::clientMembersOnline(const IntList &members)
{
	MemberWidget *wdg;
	int member_id;
	QHashIterator<int, MemberRecord> i(m_members);
	while (i.hasNext())
	{
		i.next();
		member_id = i.key();
		MemberRecord member =  i.value();
		member.is_online = members.contains(member_id);
		m_members[member_id] = member;
		for (int k = 0; k < m_mbr_widgets.size(); k++)
		{
			wdg = m_mbr_widgets.at(k);
			if (wdg->dbIndex() == member_id)
			{
				wdg->updateOnline(member.is_online);
				break;
			}
		}
	}
	member_id = Resources::current_member;
	if (m_members.contains(member_id))
	{
		MemberRecord member =  m_members[member_id];
		m_is_current_member_online = member.is_online;
		if (m_is_current_member_online)
		{
			emit startBeacon(member_id);
			m_lbl_online->setPixmap(m_pxm_online);
		}
		else
		{
			m_lbl_online->setPixmap(m_pxm_offline);
			m_is_marker_visible = false;
			m_map->hideMarker();
		}
	}
}

void MainWindow::clientMemberConnected(int member_id)
{
	if (m_members.contains(member_id)) m_members[member_id].is_online = true;
	if (member_id == Resources::current_member)
	{
		emit startBeacon(member_id);
		m_is_current_member_online = true;
		m_lbl_online->setPixmap(m_pxm_online);
		m_lbl_speed->clear();
	}
	MemberWidget *wdg;
	for (int i = 0; i < m_mbr_widgets.size(); i++)
	{
		wdg = m_mbr_widgets.at(i);
		if (wdg->dbIndex() == member_id)
		{
			wdg->updateOnline(true);
			break;
		}
	}
}

void MainWindow::clientMemberDisconnected(int member_id)
{
	if (m_members.contains(member_id)) m_members[member_id].is_online = false;
	if (member_id == Resources::current_member)
	{
		m_is_current_member_online = false;
		m_lbl_online->setPixmap(m_pxm_offline);
		m_lbl_speed->clear();
		m_is_marker_visible = false;
		m_map->hideMarker();
	}
	MemberWidget *wdg;
	for (int i = 0; i < m_mbr_widgets.size(); i++)
	{
		wdg = m_mbr_widgets.at(i);
		if (wdg->dbIndex() == member_id)
		{
			wdg->updateOnline(false);
			break;
		}
	}
}

void MainWindow::clientMemberBeaconState(int member_id, bool enabled)
{
	Q_UNUSED(member_id)
	Q_UNUSED(enabled)
	qDebug() << "beacon id =" << member_id << (enabled ? "is enabled" : "is disabled");
}

void MainWindow::clientMemberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps)
{
	Q_UNUSED(time)
	Q_UNUSED(gps)
	if (member_id == Resources::current_member && m_is_current_member_online)
	{
		if (!m_is_marker_visible)
		{
			m_is_marker_visible = true;
			m_map->showMarker(latitude, longitude);
		}
		else m_map->moveMarker(latitude, longitude);
		m_lbl_speed->setText(tr("%1 км/ч").arg(qRound(speed * 3.6)));
	}
}

void MainWindow::clientMemberTrackingPermission(int member_id, bool enabled)
{
	bool start_beacon = false;
	if (m_members.contains(member_id))
	{
		MemberRecord member = m_members[member_id];
		start_beacon = !member.tracking_allowed && enabled;
		member.tracking_allowed = enabled;
		m_members[member_id] = member;
	}
	MemberWidget *wdg;
	for (int i = 0; i < m_mbr_widgets.size(); i++)
	{
		wdg = m_mbr_widgets.at(i);
		if (wdg->dbIndex() == member_id)
		{
			wdg->updateTrackingPermission(enabled);
			break;
		}
	}
	if (member_id == Resources::current_member && m_members.contains(member_id))
	{
		if (!enabled)
		{
			m_lbl_speed->clear();
			m_is_marker_visible = false;
			m_map->hideMarker();
		}
		if (start_beacon) emit startBeacon(member_id);
	}
}

void MainWindow::clientMemberRecordingPermission(int member_id, bool enabled)
{
	if (m_members.contains(member_id))
	{
		MemberRecord member = m_members[member_id];
		member.is_tracing = enabled;
		m_members[member_id] = member;
	}
	MemberWidget *wdg;
	for (int i = 0; i < m_mbr_widgets.size(); i++)
	{
		wdg = m_mbr_widgets.at(i);
		if (wdg->dbIndex() == member_id)
		{
			wdg->updateRecordingPermission(enabled);
			break;
		}
	}
}

void MainWindow::setCurrentMember(int member_id)
{
	if (member_id != Resources::current_member && m_members.contains(member_id))
	{
		if (Resources::current_member >= 0 && m_members[Resources::current_member].is_online) emit stopBeacon(Resources::current_member);
		m_lbl_speed->clear();
		QString mname = m_members[member_id].name;
		Resources::setCurrentMember(member_id, mname);
		m_is_current_member_online = m_members[member_id].is_online;
		if (m_is_current_member_online)
		{
			m_lbl_online->setPixmap(m_pxm_online);
			emit startBeacon(member_id);
		}
		else
		{
			m_lbl_online->setPixmap(m_pxm_offline);
			m_is_marker_visible = false;
			m_map->hideMarker();
		}
		m_lbl_name->setText(mname);

		//qDebug() << "let find ava for member id=" << member_id;
		Avatar ava = Resources::findAvatar(Member, member_id);
		if (ava.db_index >= 0 && ava.file_exists)
		{
			//qDebug() << QString("I find it: id=%1; type=%2; exists=%3").arg(ava.db_index).arg(ava.type == Crew ? "Crew" : "Member").arg(ava.file_exists).toLatin1().constData();
			m_btn_member->setIcon(QIcon(QString("%1/member.%2.png").arg(Resources::avatars_path).arg(member_id)));
		}
		else m_btn_member->setIcon(m_icn_no_member);
	}
}
