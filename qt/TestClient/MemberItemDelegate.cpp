#include "MemberItemDelegate.h"

MemberItemDelegate::MemberItemDelegate(QObject *parent) : QItemDelegate(parent)
{
	m_icon_checked = QIcon(":/images/tick.png");
	m_icon_unchecked = QIcon(":/images/cross.png");
	m_icon_enabled = QIcon(":/images/bullet_green.png");
	m_icon_disabled = QIcon(":/images/bullet_red.png");
}

void MemberItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	int col = index.column();
	if (col == 0)
	{
		if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());
		QIcon ava = index.data().value<QIcon>();
		ava.paint(painter, option.rect);
	}
	else if (col == 1 || col == 12 || col == 13)
	{
		if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());
		if (index.data().toBool()) m_icon_enabled.paint(painter, option.rect);
		else m_icon_disabled.paint(painter, option.rect);
	}
	else if (col == 10 || col == 11)
	{
		if (option.state & QStyle::State_Selected) painter->fillRect(option.rect, option.palette.highlight());
		if (index.data().toBool()) m_icon_checked.paint(painter, option.rect);
		else m_icon_unchecked.paint(painter, option.rect);
	}
	else QItemDelegate::paint(painter, option, index);
}
