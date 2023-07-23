#ifndef MEMBERITEMDELEGATE_H
#define MEMBERITEMDELEGATE_H

#include <QItemDelegate>
#include <QIcon>
#include <QPainter>

#include "Resources.h"

class MemberItemDelegate : public QItemDelegate
{
	Q_OBJECT
public:
	MemberItemDelegate(QObject *parent = 0);

protected:
	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
	QIcon m_icon_checked;
	QIcon m_icon_unchecked;
	QIcon m_icon_enabled;
	QIcon m_icon_disabled;
};

#endif // MEMBERITEMDELEGATE_H
