#ifndef ___K_SUBJECT_H__
#define ___K_SUBJECT_H__

#include <memory>

// -------------------------------------------------------
// ksubject.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/31
// 功能描述： 主题接口。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KObserver;

class KxSubject
{
public:
    virtual void registerObserver(std::shared_ptr<KObserver> observer) = 0;
    virtual void removeObserver(std::shared_ptr<KObserver> observer) = 0;
    virtual void notifyObservers() = 0;
    virtual ~KxSubject() = default;
};
#endif  //___KSUBJECT_H__