#include "kpent.h"
#include <QPainter>
#include <QtMath>
#include <QSvgGenerator>

#include "kglobaldata.h"

KPent::KPent(QObject* parent)
	:KShape(parent)
{
}

KPent::~KPent()
{
}

void KPent::drawShape(QPaintDevice* parent)
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

    QPolygonF pentagon = calculatePentagonPoints();
    painter.drawPolygon(pentagon);
}

void KPent::drawSave(QPainter& painter)
{
    QPen pen;
    pen.setColor(getBorderColor());
    pen.setStyle(getPenStyle());
    pen.setWidth(getBorderWidth());

    QBrush brush(getFillColor()); // 获取填充颜色
    painter.setBrush(brush);

    painter.setPen(pen);

    QPolygonF pentagon = calculatePentagonPoints();
    painter.drawPolygon(pentagon);
    
}

KShapeType KPent::getShapeType() const
{
	return KShapeType::PentShapeType;
}

QPolygonF KPent::calculatePentagonPoints()
{
    QPointF startPoint = getStartPoint();
    QPointF endPoint = getEndPoint();

    // 计算中心点
    qreal centerX = (startPoint.x() + endPoint.x()) / 2.0;
    qreal centerY = (startPoint.y() + endPoint.y()) / 2.0;

    // 计算五边形的顶点
    qreal topX1 = centerX;
    qreal topY1 = startPoint.y();

    qreal rightX1 = endPoint.x();
    qreal rightY1 = centerY;

    qreal bottomX1 = startPoint.x() + 3*(endPoint.x() - startPoint.x()) / 4.0;
    qreal bottomY1 = endPoint.y();

    qreal bottomX2 = startPoint.x() + (endPoint.x() - startPoint.x() ) / 4.0;
    qreal bottomY2 = endPoint.y();

    qreal leftX1 = startPoint.x();
    qreal leftY1 = centerY;

    QPolygonF polygon;
    polygon << QPointF(topX1, topY1);       // Top vertex
    polygon << QPointF(rightX1, rightY1);   // Right vertex
    polygon << QPointF(bottomX1, bottomY1); // Bottom vertex
    polygon << QPointF(bottomX2, bottomY2); // Bottom vertex
    polygon << QPointF(leftX1, leftY1);     // Left vertex
    polygon << QPointF(topX1, topY1);       // Closing the pentagon

    return polygon;
}