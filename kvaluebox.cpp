#include "kvaluebox.h"
#include <QRegExpValidator>

KValueBox::KValueBox(const QString &title,const qreal val,QWidget *parent)
	: KParamBox(title,parent)
	, m_pParamEdit(Q_NULLPTR)
{
	m_pParamEdit = new QLineEdit(this);
	m_pParamEdit->setText(QString::number(val));
	m_pParamEdit->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
	m_pParamEdit->setStyleSheet("background-color:#3f3f3c; font-size:20px; color:#4f80ff; font-family: Microsoft YaHei; font-weight: bold; border-width:0; border-style:outset");
	m_pParamEdit->setMaxLength(4);

	QRegExp reg("[2-9][0-9]*|[1-9][0-9]+$"); // 正则表达式  改为接受大于1的参数
	QValidator* validator = new QRegExpValidator(reg, m_pParamEdit);
	m_pParamEdit->setValidator(validator); // 设置正则表达式

	m_pVLayout->addWidget(m_pParamEdit);// 加入到布局中 m_pVLayout 设置在父类中
}

KValueBox::~KValueBox()
{
}

void KValueBox::focusInEvent(QFocusEvent* event)
{
	m_pParamEdit->selectAll();// 全选文本
}
