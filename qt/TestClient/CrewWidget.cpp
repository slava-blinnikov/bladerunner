#include <QVBoxLayout>

#include "CrewWidget.h"
#include "CrewClient.h"

CrewWidget::CrewWidget(CrewClient *client, QWidget *parent)
	: ClientWidget(client, parent)
{
	QVBoxLayout *vbl = new QVBoxLayout();
	vbl->setMargin(0);
	vbl->setContentsMargins(0, 0, 0, 0);
	m_table = new MembersTableView(client, m_workarea);
	vbl->addWidget(m_table);
	m_workarea->setLayout(vbl);
}
