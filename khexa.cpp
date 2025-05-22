#include "khexa.h"
#include <QSvgGenerator>

#include "kglobaldata.h"

KHexa::KHexa(QObject* parent)
	:KShape(parent)
{
}

KHexa::~KHexa()
{
}

void KHexa::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
    painter.setRenderHint(QPainter::Antialiasing);

    // 与画布一起缩放
    QTransform transform;
    qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();
    transform.scale(scaleFactor, scaleFactor);
    painter.setTransform(transform);

    QPen pen;
    pen.setColor(getBorderColor());
    pen.setStyle(getPenStyle());
    pen.setWidth(getBorderWidth());

    QBrush brush(getFillColor()); // 获取填充颜色
    painter.setBrush(brush);

    painter.setPen(pen);
    painter.drawPolygon(calculateHexaPoints());
}

void KHexa::drawSave(QPainter& painter)
{
    QPen pen;
    pen.setColor(getBorderColor());
    pen.setStyle(getPenStyle());
    pen.setWidth(getBorderWidth());

    QBrush brush(getFillColor()); // 获取填充颜色
    painter.setBrush(brush);

    painter.setPen(pen);
    painter.drawPolygon(calculateHexaPoints());
    
}

KShapeType KHexa::getShapeType() const
{
	return KShapeType::HexaShapeType;
}

QPolygonF KHexa::calculateHexaPoints()
{
    QPointF startPoint = getStartPoint();
    QPointF endPoint = getEndPoint();

    // 计算中心点
    qreal centerY = (startPoint.y() + endPoint.y()) / 2.0;

    // 计算顶点
    qreal topX1 = startPoint.x() + (endPoint.x() - startPoint.x()) / 4.0;
    qreal topY1 = startPoint.y();

    qreal topX2 = startPoint.x() + 3 * (endPoint.x() - startPoint.x()) / 4.0;
    qreal topY2 = startPoint.y();

    qreal rightX1 = endPoint.x();
    qreal rightY1 = centerY;

    qreal bottomX1 = startPoint.x() + 3 * (endPoint.x() - startPoint.x()) / 4.0;
    qreal bottomY1 = endPoint.y();

    qreal bottomX2 = startPoint.x() + (endPoint.x() - startPoint.x()) / 4.0;
    qreal bottomY2 = endPoint.y();

    qreal leftX1 = startPoint.x();
    qreal leftY1 = centerY;

    QPolygonF polygon;
    polygon << QPointF(topX1, topY1); 
    polygon << QPointF(topX2, topY2);
    polygon << QPointF(rightX1, rightY1); 
    polygon << QPointF(bottomX1, bottomY1); 
    polygon << QPointF(bottomX2, bottomY2); 
    polygon << QPointF(leftX1, leftY1);     
    polygon << QPointF(topX1, topY1);       
    return polygon;
}