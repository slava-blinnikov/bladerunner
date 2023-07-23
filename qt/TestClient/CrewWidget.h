#ifndef CREWWIDGET_H
#define CREWWIDGET_H

#include "ClientWidget.h"
#include "MembersTableView.h"

class CrewWidget : public ClientWidget
{
	Q_OBJECT
public:
	CrewWidget(CrewClient *client, QWidget *parent = 0);

private:
	MembersTableView *m_table;
};

#endif // CREWWIDGET_H
