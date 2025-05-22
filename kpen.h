#ifndef ___K_PEN_H__
#define ___K_PEN_H__

#include "kshape.h"
// -------------------------------------------------------
// kpen.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 铅笔自由线。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KPen : public KShape
{
	Q_OBJECT
public:
	KPen(QObject* parent = Q_NULLPTR);
	~KPen();

	void addPos(const QPoint& pos);

	virtual void move(QPoint offset) override;

	virtual KShapeType getShapeType() const override;
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void drawSave(QPainter& painter) override;
	virtual void writeToStream(QDataStream& out) const override;
	virtual void readFromStream(QDataStream& in) override;
	void penMove(QPoint offset);

private:
	QVector<QPoint> m_posVector;
};

#endif  //___K_PEN_H__


