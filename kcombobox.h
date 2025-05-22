#ifndef ___K_COMBOBOX_H__
#define ___K_COMBOBOX_H__

#include "kparambox.h"

#include <QComboBox>

// -------------------------------------------------------
// kcombobox.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/27
// 功能描述： 下拉选择框。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KComboBox : public KParamBox
{
	Q_OBJECT

	friend class KMainWindow;
	friend class KShapeParamsBar;
public:
	KComboBox(const QString& title, QWidget* parent);
	~KComboBox();

	void addItemWithIcon(const QIcon& icon);
	
private:
	QComboBox* m_pBorderStyleComboBox;
	void initItemCount(const int count);
};

#endif  //___K_COMBOBOX_H__