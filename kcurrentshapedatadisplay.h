#ifndef ___K_CURRENT_SHAPE_DATA_DISPLAY_H__
#define ___K_CURRENT_SHAPE_DATA_DISPLAY_H__

#include <QLineEdit>
#include <QPushButton>

#include "kobserver.h"

// -------------------------------------------------------
// kcurrentshapedatadisplay.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/31
// 功能描述： 画布参数的更新显示--UI。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

struct KShapeParams;

class KCurrentShapeDataDisplay : public KObserver
{
public:
	KCurrentShapeDataDisplay(QLineEdit* borderWidthLineEdit, QComboBox* penStyleBox, QPushButton* borderColor,
		QPushButton* fillColor, QLineEdit* WLineEdit, QLineEdit* HLineEdit);
	virtual void update(KShapeParams shapeParams) override;

private:
	QLineEdit* m_borderWidthLineEdit;
	QComboBox* m_penStyleBox;
	QPushButton* m_borderColorBtn;
	QPushButton* m_fillColorBtn;
	QLineEdit* m_shapeWLineEdit;
	QLineEdit* m_shapeHLineEdit;
};

#endif  //___K_CURRENT_SHAPE_DATADISPLAY_H__