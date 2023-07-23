#ifndef MEMBERWIDGET_H
#define MEMBERWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>

#include "Resources.h"

class MemberWidget : public QWidget
{
	Q_OBJECT
public:
	MemberWidget(const MemberRecord &member, QWidget *parent = 0);
	int dbIndex();
	void updateName(const QString &name);
	void updateOnline(bool is_online);
	void updateTrackingPermission(bool is_allowed);
	void updateRecordingPermission(bool is_allowed);
	void updateAvatar();

protected:
	 void mouseReleaseEvent(QMouseEvent *event);

private:
	int m_db_index;
	QPixmap m_pxm_online;
	QPixmap m_pxm_offline;
	QPixmap m_pxm_allow;
	QPixmap m_pxm_notallow;
	QPushButton *m_btn_avatar;
	QLabel *m_lbl_online;
	QLabel *m_lbl_name;
	QLabel *m_lbl_trallowed;
	QLabel *m_lbl_tratitle;
	QLabel *m_lbl_recording;
	QLabel *m_lbl_rectitle;

private slots:
	void onAvaClicked();

signals:
	void memberSelected(int member_id);
};

#endif // MEMBERWIDGET_H
