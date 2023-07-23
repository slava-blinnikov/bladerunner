#ifndef MEMBERWIDGET_H
#define MEMBERWIDGET_H

#include "ClientWidget.h"
#include "MemberClient.h"

class MemberWidget : public ClientWidget
{
	Q_OBJECT
public:
	MemberWidget(MemberClient *client,  QWidget *parent = 0);
};

#endif // MEMBERWIDGET_H
