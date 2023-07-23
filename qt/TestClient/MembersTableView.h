#ifndef MEMBERSTABLEVIEW_H
#define MEMBERSTABLEVIEW_H

#include <QTableView>
#include <QMouseEvent>
#include <QKeyEvent>

#include "CrewClient.h"
#include "MembersTableModel.h"
#include "MemberItemDelegate.h"

class MembersTableView : public QTableView
{
	Q_OBJECT
public:
	MembersTableView(CrewClient *tcp_client, QWidget *parent = 0);

protected:
	void mouseReleaseEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent *event);

private:
	CrewClient *m_client;
	MembersTableModel *m_model;
	MemberItemDelegate *m_delegate;

signals:
	void startBeacon(int member_id);
	void stopBeacon(int member_id);
};

#endif // MEMBERSTABLEVIEW_H
