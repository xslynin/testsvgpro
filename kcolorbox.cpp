#include "kcolorbox.h"


KColorBox::KColorBox(const QString &title, const QString &rgbColorStr, KGlobalData::KColorFlag flag, QWidget *parent)
	: KParamBox(title, parent)
	, m_pColorBtn(Q_NULLPTR)
	, m_colorFlag(flag)
{
	m_pColorBtn = new QPushButton(this);
	m_pColorBtn->setMaximumSize(70, 50);
	m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(rgbColorStr));
	m_pVLayout->addWidget(m_pColorBtn); // ��ӵ�������

	(void)connect(m_pColorBtn, &QPushButton::clicked, this, &KColorBox::showPickColorDlg);
}

KColorBox::~KColorBox()
{
}

void KColorBox::mouseReleaseEvent(QMouseEvent* event)
{
	m_pColorBtn->click();
}

void KColorBox::showPickColorDlg()
{
	switch (m_colorFlag)
	{
	case KGlobalData::KColorFlag::NoneColorFlag:
		return;
	case KGlobalData::KColorFlag::CanvasFlag:
	{
		QString str = KGlobalData::getGlobalDataIntance()->getCanvasColor();
		QColor color = QColorDialog::getColor(QColor(str.toUInt(nullptr, 16)), nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);
		if (!color.isValid())
			return;
		str = QString::number(color.rgb(), 16); // �� 16���Ƶ���ɫֵת���� str �ַ���
		// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
		KGlobalData::getGlobalDataIntance()->setCanvasColor(str);
		m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(str));
	}
	break;
	case KGlobalData::KColorFlag::ShapeBorderFlag:
	{
		QColor shapeBorderColor = KGlobalData::getGlobalDataIntance()->getBoderColor();
		QColor colorShapeBorder = QColorDialog::getColor(shapeBorderColor, nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);
		if (!colorShapeBorder.isValid())
			return;
		// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
		KGlobalData::getGlobalDataIntance()->setBoderColor(colorShapeBorder);
		m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(colorShapeBorder.name().mid(1)));
	}
	break;
	case KGlobalData::KColorFlag::ShapeFillFlag:
	{
		QColor currentShapeFillColor = KGlobalData::getGlobalDataIntance()->getFillColor();
		QColor selectedShapeFillColor = QColorDialog::getColor(currentShapeFillColor, nullptr, QStringLiteral("ѡ����ɫ"), QColorDialog::ShowAlphaChannel);
		if (!selectedShapeFillColor.isValid())
			return;
		// ����ǰ��ɫֵ���浽ȫ�����ݶ�����
		KGlobalData::getGlobalDataIntance()->setFillColor(selectedShapeFillColor);
		m_pColorBtn->setStyleSheet(QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(selectedShapeFillColor.name().mid(1)));
	}
	break;
	}

	emit pickedColor(); // �����ź�	
}
