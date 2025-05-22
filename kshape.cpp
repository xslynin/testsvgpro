#include "kshape.h"
#include "kglobaldata.h"

#include <QDebug>

KShape::KShape(QObject *parent)
	: QObject(parent)

{
	m_curShapeParams.borderWidth = KGlobalData::getGlobalDataIntance()->getBorderWidth();
	m_curShapeParams.penStyle = KGlobalData::getGlobalDataIntance()->getPenStyle();
	m_curShapeParams.borderColor = KGlobalData::getGlobalDataIntance()->getBoderColor();
	m_curShapeParams.fillColor = KGlobalData::getGlobalDataIntance()->getFillColor();
}

KShape::~KShape()
{
}

void KShape::writeToStream(QDataStream& out) const
{
	out << m_curShapeParams.startPoint << m_curShapeParams.endPoint
		<< m_curShapeParams.borderWidth << m_curShapeParams.borderColor
		<< static_cast<int>(m_curShapeParams.penStyle) << m_curShapeParams.fillColor;
}

void KShape::readFromStream(QDataStream& in)
{
	int penStyleInt;
	in  >> m_curShapeParams.startPoint >> m_curShapeParams.endPoint
		>> m_curShapeParams.borderWidth >> m_curShapeParams.borderColor
		>> penStyleInt >> m_curShapeParams.fillColor;
	m_curShapeParams.penStyle = static_cast<Qt::PenStyle>(penStyleInt);
}

void KShape::move(QPoint offset)
{
	m_curShapeParams.startPoint += offset;
	m_curShapeParams.endPoint += offset;
}

void KShape::moveTop(QPoint pos)
{
	m_curShapeParams.startPoint.ry() = pos.y();
}

void KShape::moveBottom(QPoint pos)
{
	m_curShapeParams.endPoint.ry() = pos.y();
}

void KShape::moveLeft(QPoint pos)
{
	m_curShapeParams.startPoint.rx() = pos.x();
}

void KShape::moveRight(QPoint pos)
{
	m_curShapeParams.endPoint.rx() = pos.x();
}

void KShape::moveTopLeft(QPoint pos)
{
	m_curShapeParams.startPoint = pos;
}

void KShape::moveTopRight(QPoint pos)
{
	m_curShapeParams.startPoint.ry() = pos.y();
	m_curShapeParams.endPoint.rx() = pos.x();
}

void KShape::moveBottomLeft(QPoint pos)
{
	m_curShapeParams.startPoint.rx() = pos.x();
	m_curShapeParams.endPoint.ry() = pos.y();
}

void KShape::moveBottomRight(QPoint pos)
{
	m_curShapeParams.endPoint = pos;
}

// 绘制选中矩形框
void KShape::drawOutLine(QPaintDevice* parent)
{
	QPainter painter(parent);
	QPen pen;

	pen.setColor(QColor(79, 128, 255));
	pen.setWidth(3);
	painter.setPen(pen);

	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	painter.scale(scaleFactor, scaleFactor);
	painter.drawRect(QRect(getStartPoint(), getEndPoint()));
}

QPoint KShape::getStartPoint()
{
	return m_curShapeParams.startPoint;
}

QPoint KShape::getEndPoint()
{
	return m_curShapeParams.endPoint;
}

void KShape::setStartPoint(const QPoint & point)
{
	m_curShapeParams.startPoint = point;
	qDebug() << QString::fromLocal8Bit("设置起始点： ") << m_curShapeParams.startPoint;
}

void KShape::setEndPoint(const QPoint& point)
{
	m_curShapeParams.endPoint = point;
	qDebug() << QString::fromLocal8Bit("设置结束点： ") << m_curShapeParams.endPoint;
}

void KShape::setBorderWidth(const int value)
{
	m_curShapeParams.borderWidth = value;
}

int KShape::getBorderWidth() const
{
	return m_curShapeParams.borderWidth;
}

void KShape::setBorderColor(QColor color)
{
	m_curShapeParams.borderColor = color;
}

void KShape::setPenStyle(Qt::PenStyle style)
{
	m_curShapeParams.penStyle = style;
}

QColor KShape::getBorderColor() const
{
	return m_curShapeParams.borderColor;
}

Qt::PenStyle KShape::getPenStyle() const
{
	return m_curShapeParams.penStyle;
}

void KShape::setFillColor(QColor color)
{
	m_curShapeParams.fillColor = color;
}

QColor KShape::getFillColor() const
{
	return m_curShapeParams.fillColor;
}

int KShape::getWidth() const
{
	return qAbs(m_curShapeParams.startPoint.x() - m_curShapeParams.endPoint.x());
}

void KShape::setWidth(const int w)
{
	m_curShapeParams.width = w;
}

int KShape::getHeight() const
{
	return qAbs(m_curShapeParams.startPoint.y() - m_curShapeParams.endPoint.y());
}

void KShape::setHeight(const int h)
{
	m_curShapeParams.height = h;
}

KShapeType KShape::getShapeType() const
{
	return KShapeType();
}

QRect KShape::getShapeRect() const
{
	 //加入矩形绘制方向判断，解决光标异常问题
	//if (startPoint.y() > endPoint.y() && startPoint.x() < endPoint.x())
	//{
	//	return QRect(QPoint(startPoint.x(),endPoint.y()),
	//		QPoint(endPoint.x(), startPoint.y()));
	//}
	//if (startPoint.x() > endPoint.x() && startPoint.y() < endPoint.y())
	//{
	//	return QRect(QPoint(endPoint.x(), startPoint.y()),
	//		QPoint(startPoint.x(), endPoint.y()));
	//}
	return QRect(m_curShapeParams.startPoint, m_curShapeParams.endPoint);
}


bool KShape::isValid()
{
	return (m_curShapeParams.startPoint.x() != m_curShapeParams.endPoint.x() || m_curShapeParams.startPoint.y() != m_curShapeParams.endPoint.y());
}

KShapeParams KShape::getCurShapeParams()
{
	m_curShapeParams.height = getHeight();
	m_curShapeParams.width = getWidth();
	return m_curShapeParams;
}

void KShape::setCurShapeParams(KShapeParams shape_params)
{
	m_curShapeParams = shape_params;
}

KShapeParams KShape::getPrevShapeParams()
{
	return m_prevShapeParams;
}

void KShape::setPrevShapeParams()
{
	m_prevShapeParams = m_curShapeParams;
}
