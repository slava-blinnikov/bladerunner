#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>
#include <QThread>

#include "Resources.h"
#include "MainWindow.h"
#include "TCPClient.h"

int main(int argc, char **argv)
{
	qRegisterMetaType<IntList>("IntList");
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	/*QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::Socks5Proxy);
	proxy.setType(QNetworkProxy::HttpProxy);
	proxy.setHostName("10.10.0.101");
	proxy.setPort(3128);
	QNetworkProxy::setApplicationProxy(proxy);*/

	QString cfg_file = a.applicationFilePath();
	#ifdef Q_OS_WIN32
		cfg_file.replace(".exe", ".xml");
	#else
		cfg_file += ".xml";
	#endif
	ProgramConfig config(cfg_file);
	if (!config.read())
	{
		QMessageBox::critical(0, QObject::tr("Семья Рядом"), QObject::tr("Ошибка чтения файла настроек!"));
		return 0;
	}
	Resources::init(&config);

	MainWindow w;

	QThread client_thread;
	TCPClient tcp_client;
	tcp_client.moveToThread(&client_thread);
	QObject::connect(&client_thread, SIGNAL(started()), &tcp_client, SLOT(start()));
	QObject::connect(&client_thread, SIGNAL(finished()), &tcp_client, SLOT(stop()));
	QObject::connect(&w, SIGNAL(applicationActivated()), &client_thread, SLOT(start()));
	QObject::connect(&a, SIGNAL(aboutToQuit()), &client_thread, SLOT(quit()));

	QObject::connect(&w, SIGNAL(reconnect()), &tcp_client, SLOT(connectToServer()));
	QObject::connect(&tcp_client, SIGNAL(connected()), &w, SLOT(clientConnected()));
	QObject::connect(&tcp_client, SIGNAL(disconnected()), &w, SLOT(clientDisconnected()));
	QObject::connect(&tcp_client, SIGNAL(avatarChanged(int,int)), &w, SLOT(clientAvatarChanged(int,int)));
	QObject::connect(&tcp_client, SIGNAL(memberList(IntList)), &w, SLOT(clientMemberList(IntList)));
	QObject::connect(&tcp_client, SIGNAL(memberTitle(int,QString,QString,uint,bool,bool,int)), &w, SLOT(clientMemberTitle(int,QString,QString,uint,bool,bool,int)));
	QObject::connect(&tcp_client, SIGNAL(membersOnline(IntList)), &w, SLOT(clientMembersOnline(IntList)));
	QObject::connect(&tcp_client, SIGNAL(memberConnected(int)), &w, SLOT(clientMemberConnected(int)));
	QObject::connect(&tcp_client, SIGNAL(memberDisconnected(int)), &w, SLOT(clientMemberDisconnected(int)));
	QObject::connect(&tcp_client, SIGNAL(memberBeaconState(int,bool)), &w, SLOT(clientMemberBeaconState(int,bool)));
	QObject::connect(&tcp_client, SIGNAL(memberLocationChanged(int,uint,double,double,double,bool)), &w, SLOT(clientMemberLocationChanged(int,uint,double,double,double,bool)));
	QObject::connect(&tcp_client, SIGNAL(memberTrackingPermission(int,bool)), &w, SLOT(clientMemberTrackingPermission(int,bool)));
	QObject::connect(&tcp_client, SIGNAL(memberRecordingPermission(int,bool)), &w, SLOT(clientMemberRecordingPermission(int,bool)));

	QObject::connect(&w, SIGNAL(startBeacon(int)), &tcp_client, SLOT(requestStartMemberBeacon(int)));
	QObject::connect(&w, SIGNAL(stopBeacon(int)), &tcp_client, SLOT(requestStopMemberBeacon(int)));

	w.showWindow();
	int ret = a.exec();
	client_thread.wait();
	return ret;
}
