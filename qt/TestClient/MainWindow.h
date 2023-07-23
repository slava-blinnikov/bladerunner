#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QObject>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTabWidget>

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);

private:
	QComboBox *m_cb_host;
	QLineEdit *m_le_port;
	QPushButton *m_btn_applay;
	QPushButton *m_btn_newconnection;
	QTabWidget *m_tbw_tabs;

private slots:
	void serverPresetChoosed(int idx);
	void hostChanged();
	void applayHost();
	void newConnectionDialog();
	void closeCurrentTab();
};

#endif // MAINWINDOW_H
