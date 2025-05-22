#ifndef __K_LINE_H__
#define __K_LINE_H__

#include "kshape.h"

// -------------------------------------------------------
// kline.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 直线。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KLine : public KShape
{
	Q_OBJECT

public:
	KLine(QObject* parent = Q_NULLPTR);
	~KLine();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	virtual void drawSave(QPainter& painter) override;
	virtual KShapeType getShapeType();
};

#endif  //__K_LINE_H__


