#ifndef ___K_PENT_H__
#define ___K_PENT_H__

// -------------------------------------------------------
// kpent.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/26
// 功能描述： 五边形绘制。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

#include "kshape.h"

class KPent : public KShape
{
    Q_OBJECT

public:
    explicit KPent(QObject* parent = Q_NULLPTR);
    virtual ~KPent();

    virtual void drawShape(QPaintDevice* parent = Q_NULLPTR) override;
    virtual void drawSave(QPainter& painter) override;
    virtual KShapeType getShapeType() const override;

private:
    QPolygonF calculatePentagonPoints();
};


#endif  //___K_PENT_H__


