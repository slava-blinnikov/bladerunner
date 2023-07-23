#include <QCoreApplication>
#include <QWebFrame>

#include "MapView.h"

MapView::MapView(QWidget *parent) : QWebView(parent)
{
	connect(this, SIGNAL(loadFinished(bool)), this, SLOT(fitMapSize()));
	QString html_file_path = QString("file:///%1/index.yandex.html").arg(QCoreApplication::applicationDirPath());
	load(QUrl(html_file_path));
}

void MapView::showMarker(double latitude, double longitude)
{
	page()->mainFrame()->evaluateJavaScript(QString("showMarker(%1, %2)").arg(latitude, 0, 'f', 12).arg(longitude, 0, 'f', 12));
}

void MapView::moveMarker(double latitude, double longitude)
{
	page()->mainFrame()->evaluateJavaScript(QString("moveMarker(%1, %2)").arg(latitude, 0, 'f', 12).arg(longitude, 0, 'f', 12));
}

void MapView::hideMarker()
{
	page()->mainFrame()->evaluateJavaScript("hideMarker()");
}

void MapView::resizeEvent(QResizeEvent *event)
{
	fitMapSize();
	QWebView::resizeEvent(event);
}

void MapView::fitMapSize()
{
	QSize sze = size();
	page()->mainFrame()->evaluateJavaScript(QString("setMapGeometry('%1px', '%2px')").arg(sze.width() - 20).arg(sze.height() - 30));
}

