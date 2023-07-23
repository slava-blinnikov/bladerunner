#include <QApplication>
#include <QMessageBox>
#include <QTextCodec>

#include "Resources.h"
#include "MainWindow.h"

int main(int argc, char **argv)
{
	QApplication a(argc, argv);
	QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

	QString cfg_file = a.applicationFilePath();
	#ifdef Q_OS_WIN32
		cfg_file.replace(".exe", ".xml");
	#else
		cfg_file += ".xml";
	#endif
	ProgramConfig conf(cfg_file);
	if (!conf.read()) return 0;
	Resources::init(&conf);

	MainWindow w;
	w.setGeometry(490, 100, 1000, 700);
	w.show();
	return a.exec();
}
