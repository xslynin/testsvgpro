#include "kcurrentshapedatadisplay.h"

#include <QComboBox>

#include "kshape.h"

KCurrentShapeDataDisplay::KCurrentShapeDataDisplay(QLineEdit* borderWidthLineEdit, QComboBox* penStyleBox, QPushButton* borderColor,
	QPushButton* fillColor, QLineEdit* WLineEdit, QLineEdit* HLineEdit)
	: m_borderWidthLineEdit(borderWidthLineEdit)
	, m_penStyleBox(penStyleBox)
	, m_borderColorBtn(borderColor)
	, m_fillColorBtn(fillColor)
	, m_shapeWLineEdit(WLineEdit)
	, m_shapeHLineEdit(HLineEdit)
{
}
void KCurrentShapeDataDisplay::update(KShapeParams shapeParams)
{
	int index = 0;
    switch (shapeParams.penStyle)
	{
	case Qt::SolidLine:
	    index = 0;
	    break;
	case Qt::DotLine:
	    index = 1;
	    break;
	case Qt::DashLine:
	    index = 2;
	    break;
	case Qt::DashDotLine:
	    index = 3;
	    break;
	case Qt::DashDotDotLine:
	    index = 4;
	    break;
	}
	m_borderWidthLineEdit->setText(QString::number(shapeParams.borderWidth));
	m_penStyleBox->setCurrentIndex(index);
	m_borderColorBtn->setStyleSheet(
		QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(shapeParams.borderColor.name().mid(1)));
	m_fillColorBtn->setStyleSheet(
		QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(shapeParams.fillColor.name().mid(1)));
	m_shapeHLineEdit->setText(QString::number(shapeParams.height));
	m_shapeWLineEdit->setText(QString::number(shapeParams.width));
}