#include <QHeaderView>

#include "MembersTableView.h"

MembersTableView::MembersTableView(CrewClient *tcp_client, QWidget *parent) : QTableView(parent)
{
	m_client = tcp_client;
	m_model = new MembersTableModel();
	setModel(m_model);
	m_delegate = new MemberItemDelegate();
	setItemDelegate(m_delegate);
	verticalHeader()->setDefaultSectionSize(82);
	setSelectionMode(QAbstractItemView::SingleSelection);
	setSelectionBehavior(QAbstractItemView::SelectRows);

	connect(m_client, SIGNAL(memberList(QList<int>)), m_model, SLOT(initMemberList(QList<int>)));
	connect(m_client, SIGNAL(memberTitle(int,QString,QString,uint,bool,bool,int)), m_model, SLOT(setMemberTitle(int,QString,QString,uint,bool,bool,int)));
	connect(m_model, SIGNAL(requestMemberTitle(int)), m_client, SIGNAL(requestMemberTitle(int)));
	connect(m_client, SIGNAL(disconnected()), m_model, SLOT(clearRows()));
	connect(m_model, SIGNAL(requestMembersOnline()), m_client, SIGNAL(requestMembersOnline()));
	connect(m_client, SIGNAL(membersOnline(QList<int>)), m_model, SLOT(setMembersOnline(QList<int>)));
	connect(m_client, SIGNAL(memberConnected(int)), m_model, SLOT(setOnlineStatusOn(int)));
	connect(m_client, SIGNAL(memberDisconnected(int)), m_model, SLOT(setOnlineStatusOff(int)));
	connect(m_model, SIGNAL(requestMemberAvatar(int)), m_client, SIGNAL(requestMemberAvatar(int)));
	connect(m_client, SIGNAL(memberAvatarChanged(int,QString)), m_model, SLOT(setMemberAvatar(int,QString)));
	connect(this, SIGNAL(startBeacon(int)), m_model, SLOT(setBeaconOn(int)));
	connect(this, SIGNAL(stopBeacon(int)), m_model, SLOT(setBeaconOff(int)));
	connect(m_model, SIGNAL(requestStartMemberBeacon(int)), m_client, SIGNAL(startMemberBeacon(int)));
	connect(m_model, SIGNAL(requestStopMemberBeacon(int)), m_client, SIGNAL(stopMemberBeacon(int)));
	connect(m_client, SIGNAL(memberBeaconState(int,bool)), m_model, SLOT(setTrackingState(int,bool)));
	connect(m_client, SIGNAL(memberLocationChanged(int,uint,double,double,double,bool)), m_model, SLOT(setMemberLocation(int,uint,double,double,double,bool)));
	connect(m_client, SIGNAL(memberTrackingPermission(int,bool)), m_model, SLOT(setPermissionTracking(int,bool)));
	connect(m_client, SIGNAL(memberRecordingPermission(int,bool)), m_model, SLOT(setPermissionRecording(int,bool)));
}

void MembersTableView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && event->modifiers() == Qt::NoModifier)
	{
		QModelIndex idx = indexAt(event->pos());
		if (idx.isValid())
		{
			int col = idx.column();
			if (col == 12)
			{
				if (idx.data().toBool())
				{
					emit stopBeacon(idx.data(MemberIdRole).toInt());
				}
				else
				{
					emit startBeacon(idx.data(MemberIdRole).toInt());
				}
				return;
			}
		}
	}
	QTableView::mouseReleaseEvent(event);
}

void MembersTableView::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Space && event->modifiers() == Qt::NoModifier)
	{
		QModelIndex idx = currentIndex();
		if (idx.isValid())
		{
			int col = idx.column();
			if (col == 12)
			{
				if (idx.data().toBool())
				{
					emit stopBeacon(idx.data(MemberIdRole).toInt());
				}
				else
				{
					emit startBeacon(idx.data(MemberIdRole).toInt());
				}
				return;
			}
		}
	}
	QTableView::keyPressEvent(event);
}



