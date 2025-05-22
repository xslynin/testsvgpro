#include "kshapeparamsbar.h"
#include "kcanvasparamsbar.h"
#include "kcurrentshapedatadisplay.h"

KShapeParamsBar::KShapeParamsBar(const QString& title, QWidget* parent)
	: KParamsBar(title, parent)
	, m_pBorderWidthBox(Q_NULLPTR)
	, m_pBorderStyleBox(Q_NULLPTR)
	, m_pBorderColorBox(Q_NULLPTR)
	, m_pFillColorBox(Q_NULLPTR)

{
	m_pBorderWidthBox = new KValueBox(QStringLiteral("边框宽度"), KGlobalData::getGlobalDataIntance()->getBorderWidth(), this);
	m_pBorderStyleBox = new KComboBox(QStringLiteral("边框样式"), this);
	m_pBorderColorBox = new KColorBox(QStringLiteral("边框颜色"), KGlobalData::getGlobalDataIntance()->getBoderColor().name().mid(1), KGlobalData::KColorFlag::ShapeBorderFlag, this);
	m_pFillColorBox = new KColorBox(QStringLiteral("填充颜色"), KGlobalData::getGlobalDataIntance()->getFillColor().name().mid(1), KGlobalData::KColorFlag::ShapeFillFlag, this);
	m_pShapeWidthBox = new KValueBox(QStringLiteral("图形宽度"), 0, this);
	m_pShapeHidthBox = new KValueBox(QStringLiteral("图形高度"), 0, this);

	// 给边框线条样式初始化
	QIcon icon0(":/icons/borderStyle/solidline.png");
	QIcon icon1(":/icons/borderStyle/dotline.png");
	QIcon icon2(":/icons/borderStyle/dashline.png");
	QIcon icon3(":/icons/borderStyle/dashdotline.png");
	QIcon icon4(":/icons/borderStyle/dashdotdotline.png");
	m_pBorderStyleBox->addItemWithIcon(icon0);
	m_pBorderStyleBox->addItemWithIcon(icon1);
	m_pBorderStyleBox->addItemWithIcon(icon2);
	m_pBorderStyleBox->addItemWithIcon(icon3);
	m_pBorderStyleBox->addItemWithIcon(icon4);

	// 添加到网格布局
	m_pGridLayout->addWidget(m_pBorderWidthBox, 1, 0);
	m_pGridLayout->addWidget(m_pBorderStyleBox, 1, 1);
	m_pGridLayout->addWidget(m_pBorderColorBox, 2, 0);
	m_pGridLayout->addWidget(m_pFillColorBox, 2, 1);
	m_pGridLayout->addWidget(m_pShapeWidthBox, 3, 0);
	m_pGridLayout->addWidget(m_pShapeHidthBox, 3, 1);


	auto curCanvasDataDisplay = std::make_shared<KCurrentShapeDataDisplay>(
		m_pBorderWidthBox->m_pParamEdit, m_pBorderStyleBox->m_pBorderStyleComboBox,
		m_pBorderColorBox->m_pColorBtn, m_pFillColorBox->m_pColorBtn,
		m_pShapeWidthBox->m_pParamEdit, m_pShapeHidthBox->m_pParamEdit
		);
	m_shapeData.registerObserver(curCanvasDataDisplay);
}

KShapeParamsBar::~KShapeParamsBar()
{
}
