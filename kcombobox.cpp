#include "kcombobox.h"

KComboBox::KComboBox(const QString& title, QWidget* parent)
	: KParamBox(title, parent)
	, m_pBorderStyleComboBox(Q_NULLPTR)
{
	m_pBorderStyleComboBox = new QComboBox(this);
	m_pBorderStyleComboBox->setMaximumSize(70, 50);
	m_pBorderStyleComboBox->setStyleSheet("QComboBox::drop-down { subcontrol-origin: padding; subcontrol-position: bottom right; }");
	m_pVLayout->addWidget(m_pBorderStyleComboBox); // 添加到布局器
}

KComboBox::~KComboBox()
{
}

void KComboBox::addItemWithIcon(const QIcon& icon)
{
	m_pBorderStyleComboBox->setIconSize(QSize(64, 64));
	m_pBorderStyleComboBox->addItem(icon,"");
}

void KComboBox::initItemCount(const int count)
{
	m_pBorderStyleComboBox->clear();
	for (int i = 0; i < count; ++i) 
	{
		QString itemText = QString("Item %1").arg(i + 1);
		m_pBorderStyleComboBox->addItem(itemText);
	}
}
