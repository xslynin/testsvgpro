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

	// �뻭��һ������
	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();

	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // ��ȡ�����ɫ
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

	QBrush brush(getFillColor()); // ��ȡ�����ɫ
	painter.setBrush(brush);

	painter.setPen(pen);
	painter.drawRect(QRect(getStartPoint(), getEndPoint()));
	
}

KShapeType KRect::getShapeType() const
{
	return KShapeType::RectShapeType;
}
