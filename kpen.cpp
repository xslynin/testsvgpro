#include "kpen.h"
#include <QPainterPath>
#include <qsvggenerator.h>

#include "kglobaldata.h"

KPen::KPen(QObject* parent)
	:KShape(parent)
{
}

KPen::~KPen()
{
}

void KPen::addPos(const QPoint& pos)
{
	if (m_posVector.isEmpty())
	{
		m_curShapeParams.startPoint = pos;
		m_curShapeParams.endPoint = pos;
	}
	else
	{
		if (pos.x() < m_curShapeParams.startPoint.x())
			m_curShapeParams.startPoint.rx() = pos.x();
		if (pos.y() < m_curShapeParams.startPoint.y())
			m_curShapeParams.startPoint.ry() = pos.y();
		if (pos.x() > m_curShapeParams.endPoint.x())
			m_curShapeParams.endPoint.rx() = pos.x();
		if (pos.y() > m_curShapeParams.endPoint.y())
			m_curShapeParams.endPoint.ry() = pos.y();
	}
	m_posVector.append(pos);

}

void KPen::move(QPoint offset)
{
	for (int i = 0; i < m_posVector.size(); ++i)
	{
		m_posVector[i] += offset;
	}
	KShape::move(offset);
}

KShapeType KPen::getShapeType() const
{
	return KShapeType::PenShapeType;
}

void KPen::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);

	// 与画布一起缩放
	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();

	QPainterPath path;
	auto pos = m_posVector.begin();

	path.moveTo(*pos);
	while (pos != m_posVector.end())
	{
		path.lineTo(*pos);
		pos++;
	}
	QPen pen;
	pen.setColor(getBorderColor());
	pen.setWidthF(getBorderWidth());
	pen.setStyle(getPenStyle());
	painter.setPen(pen);
	painter.scale(scaleFactor, scaleFactor);
	painter.drawPath(path);
}

void KPen::drawSave(QPainter& painter)
{
	QPainterPath path;
	painter.setRenderHint(QPainter::Antialiasing);
	auto pos = m_posVector.begin();

	path.moveTo(*pos);
	while (pos != m_posVector.end())
	{
		path.lineTo(*pos);
		pos++;
	}
	QPen pen;
	pen.setColor(getBorderColor());
	pen.setWidthF(getBorderWidth());
	pen.setStyle(getPenStyle());
	painter.setPen(pen);
	painter.drawPath(path);
}

void KPen::writeToStream(QDataStream& out) const
{
	out << m_curShapeParams.startPoint << m_curShapeParams.endPoint
		<< m_curShapeParams.borderWidth << m_curShapeParams.borderColor
		<< static_cast<int>(m_curShapeParams.penStyle) << m_curShapeParams.fillColor
		<< m_posVector;	
}

void KPen::readFromStream(QDataStream& in)
{
	int penStyleInt;
	in >> m_curShapeParams.startPoint >> m_curShapeParams.endPoint
		>> m_curShapeParams.borderWidth >> m_curShapeParams.borderColor
		>> penStyleInt >> m_curShapeParams.fillColor
		>> m_posVector;
	m_curShapeParams.penStyle = static_cast<Qt::PenStyle>(penStyleInt);
}

void KPen::penMove(QPoint offset)
{
	for (int i = 0; i < m_posVector.size(); ++i)
	{
		m_posVector[i] += offset;
	}
}
