#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "BlrClient.h"

class ClientWidget : public QWidget
{
	Q_OBJECT
public:
	ClientWidget(BlrClient *client, QWidget *parent = 0);
	~ClientWidget();
	void start();

protected:
	BlrClient *m_client;
	QWidget *m_workarea;

private:
	QLabel *m_lbl_ava;
	QLabel *m_lbl_name;
	QPushButton *m_btn_connect;
	QPushButton *m_btn_close;

protected slots:
	void onConnected();
	void onDisconnected();
	void onAuthorized();

private slots:
	void switchConnectionState();

public slots:
	void setAvatar(const QString &filename);

signals:
	void closeMe();
};

#endif // CLIENTWIDGET_H

