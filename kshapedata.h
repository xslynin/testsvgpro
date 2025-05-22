#ifndef ___K_CANVAS_DATA_H__
#define ___K_CANVAS_DATA_H__

#include "ksubject.h"
#include "kshape.h"
#include <QString>
#include <vector>

// -------------------------------------------------------
// kcanvasdata.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/31
// 功能描述： 具体主题类，画布数据。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KShapeData : public KxSubject
{
public:
	KShapeData() = default;
	~KShapeData() = default;
	void registerObserver(std::shared_ptr<KObserver> observer) override;
	void removeObserver(std::shared_ptr<KObserver> observer) override;
	void notifyObservers() override;
	void setShapeData(KShapeParams shapeParams);

private:
	KShapeParams m_shapeParams;
	std::vector<std::shared_ptr<KObserver>> m_observers;
};

#endif  //___KCANVASDATA_H__