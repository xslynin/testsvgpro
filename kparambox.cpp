#include "kparambox.h"

KParamBox::KParamBox(const QString &title,QWidget *parent)
	: QWidget(parent)
	, m_title(title)
	, m_pVLayout(Q_NULLPTR)
	, m_pTitleLabel(Q_NULLPTR)
{
	setMinimumSize(70, 70);
	setMaximumSize(70, 70);

	setFocusPolicy(Qt::ClickFocus); // ͨ��������ȡ�����¼�
	setAttribute(Qt::WA_StyledBackground, true);// �Զ���ؼ���ʽ����Ч

	setStyleSheet("background-color:#3f3f3c");

	m_pVLayout = new QVBoxLayout(this);
	m_pVLayout->setMargin(0); // ������߾�

	m_pTitleLabel = new QLabel(this);
	m_pTitleLabel->setMaximumHeight(20);// �������߶�
	m_pTitleLabel->setAlignment(Qt::AlignVCenter); // ���ö���
	m_pTitleLabel->setStyleSheet("background-color:#3f3f3c; font-size:13px; color:#cccccc; font-family: Microsoft YaHei; padding: 0px 5px");
	m_pTitleLabel->setText(m_title);

	m_pVLayout->addWidget(m_pTitleLabel);
}

KParamBox::~KParamBox()
{
}

void KParamBox::enterEvent(QEvent* event)
{
	setCursor(Qt::PointingHandCursor); // ���������ʽ
}

void KParamBox::leaveEvent(QEvent* event)
{
	setCursor(Qt::ArrowCursor); // ���������ʽ
}
