#include "ktoolbar.h"
#include "kglobaldata.h"

KToolBar::KToolBar(QWidget *parent)
	: QWidget(parent)
	, m_pVLayout(nullptr)
	, m_pBtnGroup(nullptr)
	, m_pMouseBtn(nullptr)
	, m_pPenBtn(nullptr)
	, m_pLineBtn(nullptr)
	, m_pRectBtn(nullptr)
	, m_pCircleBtn(nullptr)
	, m_pTextBtn(nullptr)
{
	setMaximumWidth(50);// 设置最大宽度为 50
	setAttribute(Qt::WA_StyledBackground, true);// 自定义控件样式表生效
    
    // Fetch button tips when the toolbar is created
    KGlobalData::getGlobalDataIntance()->fetchButtonTips();

	setStyleSheet("background-color:#000000");// 设置样式表

	//垂直布局对象
	m_pVLayout = new QVBoxLayout(this);
	m_pVLayout->setAlignment(Qt::AlignVCenter | Qt::AlignTop);// 设置对齐方式
	m_pVLayout->setSpacing(25);// 设置布局器中的组件的间距

	m_pBtnGroup = new QButtonGroup(this);
	m_pBtnGroup->setExclusive(true);// 设置互斥，只能有一个按钮处于 checked 状态

	// 创建 自定义按钮
	m_pMouseBtn = new KToolButton(KGlobalData::KDrawFlag::MouseDrawFlag, this);
	m_pPenBtn = new KToolButton(KGlobalData::KDrawFlag::PenDrawFlag, this);
	m_pLineBtn = new KToolButton(KGlobalData::KDrawFlag::LineDrawFlag, this);
	m_pRectBtn = new KToolButton(KGlobalData::KDrawFlag::RectDrawFlag, this);
	m_pCircleBtn = new KToolButton(KGlobalData::KDrawFlag::CircleDrawFlag, this);
	m_pPentBtn = new KToolButton(KGlobalData::KDrawFlag::PentDrawFlag, this);
	m_pHexaBtn = new KToolButton(KGlobalData::KDrawFlag::HexaDrawFlag, this);
	m_pStarBtn = new KToolButton(KGlobalData::KDrawFlag::StarDrawFlag, this);
	m_pTextBtn = new KToolButton(KGlobalData::KDrawFlag::TextDrawFlag, this);

	// 将按钮添加到 按钮组中
	m_pBtnGroup->addButton(m_pMouseBtn);
	m_pBtnGroup->addButton(m_pPenBtn);
	m_pBtnGroup->addButton(m_pLineBtn);
	m_pBtnGroup->addButton(m_pRectBtn);
	m_pBtnGroup->addButton(m_pCircleBtn);
	m_pBtnGroup->addButton(m_pPentBtn);
	m_pBtnGroup->addButton(m_pHexaBtn);
	m_pBtnGroup->addButton(m_pStarBtn);
	m_pBtnGroup->addButton(m_pTextBtn);

	// 将自定义按钮添加到布局器中
	m_pVLayout->addWidget(m_pMouseBtn);
	m_pVLayout->addWidget(m_pPenBtn);
	m_pVLayout->addWidget(m_pLineBtn);
	m_pVLayout->addWidget(m_pRectBtn);
	m_pVLayout->addWidget(m_pCircleBtn);
	m_pVLayout->addWidget(m_pPentBtn);
	m_pVLayout->addWidget(m_pHexaBtn);
	m_pVLayout->addWidget(m_pStarBtn);
	m_pVLayout->addWidget(m_pTextBtn);
	
	// 将布局器添加到 qwidget 中
	setLayout(m_pVLayout);
}


KToolBar::~KToolBar()
{
}
