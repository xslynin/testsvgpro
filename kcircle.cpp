#include "kcircle.h"
#include <QSvgGenerator>

#include "kglobaldata.h"

KCircle::KCircle(QObject* parent)
	:KShape(parent)
{
}

KCircle::~KCircle()
{
}

void KCircle::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);

	// 与画布一起缩放
	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	painter.scale(scaleFactor, scaleFactor);

	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);
	painter.drawEllipse(QRect(getStartPoint(), getEndPoint()));
}

void KCircle::drawSave(QPainter& painter)
{
	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);
	painter.drawEllipse(QRect(getStartPoint(), getEndPoint()));
	
}

KShapeType KCircle::getShapeType() const
{
	return KShapeType::CircleShapeType;
}
