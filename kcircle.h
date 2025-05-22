#ifndef ___K_CIRCLE_H__
#define ___K_CIRCLE_H__
#include "kshape.h"

// -------------------------------------------------------
// kcircle.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 圆。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KCircle : public KShape
{
	Q_OBJECT

public:
	KCircle(QObject* parent = Q_NULLPTR);
	~KCircle();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void drawSave(QPainter& painter) override;
	virtual KShapeType getShapeType() const override;
};
#endif  //___KCIRCLE_H__


