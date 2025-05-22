#include "kcanvasparamsbar.h"

KCanvasParamsBar::KCanvasParamsBar(const QString &title,QWidget *parent)
	: KParamsBar(title,parent)
	, m_pWidthBox(Q_NULLPTR)
	, m_pHeightBox(Q_NULLPTR)
	, m_pScaleBox(Q_NULLPTR)
	, m_pColorBox(Q_NULLPTR)
{
	m_pWidthBox = new KValueBox(QStringLiteral("画布宽度"), KGlobalData::getGlobalDataIntance()->getCanvasWidth(), this);
	m_pHeightBox = new KValueBox(QStringLiteral("画布高度"), KGlobalData::getGlobalDataIntance()->getCanvasHeight(), this);
	m_pScaleBox = new KValueBox(QStringLiteral("缩放比例"), KGlobalData::getGlobalDataIntance()->getCanvasScale() * 100, this);
	m_pColorBox = new KColorBox(QStringLiteral("画布颜色"), KGlobalData::getGlobalDataIntance()->getCanvasColor(), KGlobalData::KColorFlag::CanvasFlag, this);

	// 添加到网格布局
	m_pGridLayout->addWidget(m_pWidthBox, 0, 0);// 第 0 行 第 0 列
	m_pGridLayout->addWidget(m_pHeightBox, 0, 1);// 第 0 行 第 1 列
	m_pGridLayout->addWidget(m_pScaleBox, 1, 0);
	m_pGridLayout->addWidget(m_pColorBox, 1, 1);

}

KCanvasParamsBar::~KCanvasParamsBar()
{
}
