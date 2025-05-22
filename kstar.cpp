#include "kstar.h"

#include <qmath.h>
#include <qsvggenerator.h>

#include "kglobaldata.h"

KStar::KStar(QObject* parent)
	:KShape(parent)
{
}

KStar::~KStar()
{
}

void KStar::drawShape(QPaintDevice* parent)
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
    painter.drawPolygon(generateStar());
}

void KStar::drawSave(QPainter& painter)
{
    QPen pen;
    pen.setColor(getBorderColor());
    pen.setStyle(getPenStyle());
    pen.setWidth(getBorderWidth());
    QBrush brush(getFillColor()); // 获取填充颜色
    painter.setBrush(brush);
    painter.setPen(pen);
    painter.drawPolygon(generateStar());
}

KShapeType KStar::getShapeType() const
{
    return KShapeType::StarShapeType;
}

QPolygonF KStar::generateStar()
{
    QPointF startP = getStartPoint();
    QPointF endP = getEndPoint();
    QPolygonF starPoints;

    qreal centerX = (startP.x() + endP.x()) / 2;
    qreal centerY = (startP.y() + endP.y()) / 2;
    qreal width = endP.x() - startP.x();
    qreal height = endP.y() - startP.y();

    QPointF pentagonPoints[5];
    pentagonPoints[0] = QPointF(centerX, startP.y());
    pentagonPoints[1] = QPointF(startP.x(), centerY);
    pentagonPoints[2] = QPointF(centerX - width / 4, endP.y());
    pentagonPoints[3] = QPointF(centerX + width / 4, endP.y());
    pentagonPoints[4] = QPointF(endP.x(), centerY);
    pentagonPoints[1].setY(centerY - height / 8);
    pentagonPoints[4].setY(centerY - height / 8);

    for (int i = 0; i < 5; ++i) 
    {
        starPoints << pentagonPoints[i];
        QPointF outerPoint1 = pentagonPoints[i];
        QPointF outerPoint2 = pentagonPoints[(i + 1) % 5];
        QPointF innerPoint = (outerPoint1 + outerPoint2) / 2;
        qreal dx = innerPoint.x() - centerX;
        qreal dy = innerPoint.y() - centerY;
        qreal ratio = 0.382; // 黄金分割比例
        QPointF innerStarPoint(centerX + dx * ratio, centerY + dy * ratio);
        starPoints << innerStarPoint;
    }

    return starPoints;
}
