#include "krect.h"
#include "kglobaldata.h"
#include <QPainter>
#include <QSvgGenerator>

KRect::KRect(QObject *parent)
	: KShape(parent)
{
}

KRect::~KRect()
{
}

void KRect::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);

	// 与画布一起缩放
	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();

	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);
	painter.scale(scaleFactor, scaleFactor);
	painter.drawRect(QRect(getStartPoint(),getEndPoint()));
}

void KRect::drawSave(QPainter& painter)
{
	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);
	painter.drawRect(QRect(getStartPoint(), getEndPoint()));
	
}

KShapeType KRect::getShapeType() const
{
	return KShapeType::RectShapeType;
}
