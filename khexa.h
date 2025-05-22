#ifndef ___K_HEXA_H__
#define ___K_HEXA_H__
#include "kshape.h"

// -------------------------------------------------------
// khexa.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 绘制六边形。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KHexa : public KShape
{
	Q_OBJECT

public:
	KHexa(QObject* parent = Q_NULLPTR);
	~KHexa();
	virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
	void drawSave(QPainter& painter) override;
	virtual KShapeType getShapeType() const override;
	QPolygonF calculateHexaPoints();
};

#endif  //___K_HEXA_H__