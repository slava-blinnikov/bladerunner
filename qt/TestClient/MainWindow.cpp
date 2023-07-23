#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

#include "Resources.h"
#include "MainWindow.h"
#include "NewConnectionDialog.h"
#include "ClientWidget.h"
#include "CrewWidget.h"
#include "MemberWidget.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
	setWindowTitle(tr("Bladerunner Test Unit"));

	QWidget *wdg_central = new QWidget(this);
	QVBoxLayout *vbl_central = new QVBoxLayout();

	//QVBoxLayout *vbl;
	QHBoxLayout *hbl;
	QLabel *lbl;

	hbl = new QHBoxLayout();
	lbl = new QLabel(tr("host:"), wdg_central);
	hbl->addWidget(lbl);
	m_cb_host = new QComboBox(wdg_central);
	m_cb_host->setMinimumWidth(300);
	m_cb_host->setEditable(true);
	for (int i = 0; i < Resources::server_presets.size(); i++)
	{
		const ServerHandler &srv = Resources::server_presets.at(i);
		m_cb_host->addItem(QString("%1:%2").arg(srv.host).arg(srv.port));
	}
	m_cb_host->setCurrentIndex(-1);
	m_cb_host->setEditText(Resources::host);
	hbl->addWidget(m_cb_host);
	hbl->addSpacing(20);
	lbl = new QLabel(tr("port:"), wdg_central);
	hbl->addWidget(lbl);
	m_le_port = new QLineEdit(wdg_central);
	m_le_port->setText(QString::number(Resources::port));
	m_le_port->setSizePolicy(m_cb_host->sizePolicy());
	hbl->addWidget(m_le_port);
	hbl->addSpacing(20);
	m_btn_applay = new QPushButton(tr("Applay"), wdg_central);
	m_btn_applay->setEnabled(false);
	hbl->addWidget(m_btn_applay);
	hbl->addStretch();
	vbl_central->addLayout(hbl);

	hbl = new QHBoxLayout();
	m_btn_newconnection = new QPushButton(tr("New client"), wdg_central);
	hbl->addWidget(m_btn_newconnection);
	hbl->addStretch();
	vbl_central->addLayout(hbl);

	m_tbw_tabs = new QTabWidget(wdg_central);
	vbl_central->addWidget(m_tbw_tabs);

	wdg_central->setLayout(vbl_central);
	setCentralWidget(wdg_central);

	connect(m_cb_host, SIGNAL(currentIndexChanged(int)), this, SLOT(serverPresetChoosed(int)));
	connect(m_cb_host, SIGNAL(editTextChanged(QString)), this, SLOT(hostChanged()));
	connect(m_le_port, SIGNAL(textChanged(QString)), this, SLOT(hostChanged()));
	connect(m_btn_applay, SIGNAL(clicked()), this, SLOT(applayHost()));
	connect(m_btn_newconnection, SIGNAL(clicked()), this, SLOT(newConnectionDialog()));
}

void MainWindow::serverPresetChoosed(int idx)
{
	const ServerHandler &srv = Resources::server_presets.at(idx);
	m_cb_host->blockSignals(true);
	m_le_port->blockSignals(true);
	m_cb_host->setCurrentIndex(-1);
	m_cb_host->setEditText(srv.host);
	m_le_port->setText(QString::number(srv.port));
	m_cb_host->blockSignals(false);
	m_le_port->blockSignals(false);
}

void MainWindow::hostChanged()
{
	m_btn_applay->setEnabled(true);
}

void MainWindow::applayHost()
{
	QString host = m_cb_host->currentText().trimmed().toLower();
	if (!host.isEmpty())
	{
		bool ok;
		int port = m_le_port->text().toInt(&ok);
		if (ok) if (Resources::applayHost(host, port)) m_cb_host->addItem(QString("%1:%2").arg(host).arg(port));
	}
	m_btn_applay->setEnabled(false);
}

void MainWindow::newConnectionDialog()
{
	NewConnectionDialog dlg(this);
	if (dlg.exec() == QDialog::Accepted)
	{
		ClientWidget *cw = 0;
		QString title;
		switch (dlg.clientType())
		{
			case ClientUnknown:
				title = tr("Unknown: %1.%2").arg(dlg.crewName()).arg(dlg.memberName());
				break;
			case Administrator:
				title = tr("Admin: %1").arg(dlg.crewName());
				break;
			case Crew:
				title = tr("Crew: %1").arg(dlg.crewName());
				cw = new CrewWidget(new CrewClient(dlg.crewName(), dlg.clientPassword()), m_tbw_tabs);
				break;
			case Member:
				title = tr("Member: %1.%2").arg(dlg.crewName()).arg(dlg.memberName());
				cw = new MemberWidget(new MemberClient(dlg.crewName(), dlg.memberName(), dlg.clientPassword()), m_tbw_tabs);
				break;
		}
		if (cw)
		{
			m_tbw_tabs->addTab(cw, title);
			connect(cw, SIGNAL(closeMe()), this, SLOT(closeCurrentTab()));
			cw->start();
		}
	}
}

void MainWindow::closeCurrentTab()
{
	ClientWidget *cw = qobject_cast<ClientWidget *>(sender());
	if (cw)
	{
		int idx = m_tbw_tabs->indexOf(cw);
		if (idx >= 0) m_tbw_tabs->removeTab(idx);
		delete cw;
	}
}
