#ifndef NEWCONNECTIONDIALOG_H
#define NEWCONNECTIONDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QComboBox>
#include <QLineEdit>

#include "Types.h"

class NewConnectionDialog : public QDialog
{
	Q_OBJECT
public:
	NewConnectionDialog(QWidget *parent = 0);
	ClientType clientType();
	QString crewName();
	QString memberName();
	QString clientPassword();

private:
	QComboBox *m_cb_type;
	QLineEdit *m_le_crew;
	QLineEdit *m_le_member;
	QWidget *m_wdg_member;
	QLineEdit *m_le_password;

private slots:
	void clientTypeChanged(int idx);
};

#endif // NEWCONNECTIONDIALOG_H
