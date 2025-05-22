#ifndef ___K_OBSERVER_H__
#define ___K_OBSERVER_H__

#include <QString>
// -------------------------------------------------------
// kobserver.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/31
// 功能描述： 观察者基类。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

struct KShapeParams;

class KObserver
{
public:
    virtual void update(KShapeParams shapeParams) = 0;
    virtual ~KObserver() = default;
};

#endif  //___K_OBSERVER_H__