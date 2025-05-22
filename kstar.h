#ifndef ___K_STAR_H__
#define ___K_STAR_H__

#include "kshape.h"

// -------------------------------------------------------
// kstar.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 绘制五角星。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KStar : public KShape
{
	Q_OBJECT

public:
	KStar(QObject* parent = Q_NULLPTR);
	~KStar();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void drawSave(QPainter& painter) override;
	virtual KShapeType getShapeType() const override;
private:
	QPolygonF generateStar();
	
};

#endif  //___K_STAR_H__