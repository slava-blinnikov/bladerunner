#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QShowEvent>
#include <QResizeEvent>
#include <QPushButton>
#include <QLabel>
#include <QIcon>
#include <QPixmap>
#include <QHash>
#include <QVector>
#include <QScrollArea>
#include <QTimer>

#include "Resources.h"
#include "MemberWidget.h"
#include "MapView.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	void showWindow();

protected:
	void closeEvent(QCloseEvent *event);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent *event);

private:
	bool m_is_activated;
	QPushButton *m_btn_member;
	QIcon m_icn_no_member;
	QPixmap m_pxm_online;
	QPixmap m_pxm_offline;
	QLabel *m_lbl_online;
	QLabel *m_lbl_name;
	QPushButton *m_btn_crew;
	MapView *m_map;
	QLabel *m_lbl_speed;
	bool m_is_current_member_online;
	bool m_is_marker_visible;
	bool m_is_online;
	QHash<int, MemberRecord> m_members;
	QScrollArea *m_sca_members;
	QWidget *m_wgd_members;
	QVector<MemberWidget *> m_mbr_widgets;
	void updateMembersDialogGeometry();

private slots:
	void showMembersDialog();
	void showConfigDialog();

signals:
	void applicationActivated();
	void startBeacon(int member_id);
	void stopBeacon(int member_id);
	void reconnect();

public slots:
	void clientConnected();
	void clientDisconnected();
	void clientAvatarChanged(int client_type, int db_id);
	void clientMemberList(const IntList &members);
	void clientMemberTitle(int member_id, const QString &usrname, const QString &note, uint avatime, bool tracking_allowed, bool tracing_allowed, int trctimeout);
	void clientMembersOnline(const IntList &members);
	void clientMemberConnected(int member_id);
	void clientMemberDisconnected(int member_id);
	void clientMemberBeaconState(int member_id, bool enabled);
	void clientMemberLocationChanged(int member_id, uint time, double latitude, double longitude, double speed, bool gps);
	void clientMemberTrackingPermission(int member_id, bool enabled);
	void clientMemberRecordingPermission(int member_id, bool enabled);
	void setCurrentMember(int member_id);
};

#endif // MAINWINDOW_H
