#ifndef ___K_SHAPE_PARAMS_BAR_H__
#define ___K_SHAPE_PARAMS_BAR_H__

#include "kparamsbar.h"
#include "kcolorbox.h"
#include "kvaluebox.h"
#include "kcombobox.h"
#include "kcanvasmenu.h"
#include "kshapedata.h"


// -------------------------------------------------------
// kshapeparamsbar.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/28
// 功能描述： 绘制图形的参数设置。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KShapeParamsBar : public KParamsBar
{
	Q_OBJECT
		friend class KMainWindow;
public:
	KShapeParamsBar(const QString& title, QWidget* parent);
	~KShapeParamsBar();

private:
	KValueBox* m_pBorderWidthBox;
	KComboBox* m_pBorderStyleBox;
	KColorBox* m_pBorderColorBox;
	KColorBox* m_pFillColorBox;
	KValueBox* m_pShapeWidthBox;
	KValueBox* m_pShapeHidthBox;
	KShapeData m_shapeData;
};

#endif  //___KSHAPEPARAMSBAR_H__