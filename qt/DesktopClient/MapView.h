#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QObject>
#include <QWebView>
#include <QResizeEvent>

class MapView : public QWebView
{
	Q_OBJECT
public:
	MapView(QWidget *parent = 0);
	void showMarker(double latitude, double longitude);
	void moveMarker(double latitude, double longitude);
	void hideMarker();

protected:
	void resizeEvent(QResizeEvent *event);

public slots:
	void fitMapSize();
};

#endif // MAPVIEW_H
