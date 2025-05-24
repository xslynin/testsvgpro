#include "ktoolbutton.h"
#include <QMouseEvent> // Include QMouseEvent for mouseReleaseEvent
#include <QDebug> // For debugging

KToolButton::KToolButton(KGlobalData::KDrawFlag drawflag, QWidget *parent)
	: QRadioButton(parent)
	, m_pSvgRenderer(nullptr)
	, m_drawFlag(drawflag)
{
    // Map draw flag to button identifier for tooltip fetching
    switch (static_cast<int>(m_drawFlag))
    {
    case static_cast<int>(KGlobalData::KDrawFlag::MouseDrawFlag):
        m_buttonIdentifier = "selectionbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::PenDrawFlag):
        m_buttonIdentifier = "freehandlinedrawbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::LineDrawFlag):
        m_buttonIdentifier = "linedrawbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::RectDrawFlag):
        m_buttonIdentifier = "rectdrawbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::CircleDrawFlag):
        // No direct match in provided JSON, using a placeholder
        m_buttonIdentifier = "circlebutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::PentDrawFlag):
        m_buttonIdentifier = "pentagondrawbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::HexaDrawFlag):
        m_buttonIdentifier = "quadrilateraldrawbutton"; // Assuming Hexa is quadrilateral
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::StarDrawFlag):
        m_buttonIdentifier = "stardrawbutton";
        break;
    case static_cast<int>(KGlobalData::KDrawFlag::TextDrawFlag):
        // No direct match in provided JSON, using a placeholder
        m_buttonIdentifier = "textbutton";
        break;
    default:
        m_buttonIdentifier = ""; // No tooltip for unknown flags
        break;
    }

	initToolButton();

    // Connect signal to update tooltip when tips are fetched
    connect(KGlobalData::getGlobalDataIntance(), &KGlobalData::buttonTipsFetched, this, &KToolButton::updateTooltip);

    // Configure tooltip timer
    m_tooltipTimer.setInterval(500); // 500ms delay
    m_tooltipTimer.setSingleShot(true);
    connect(&m_tooltipTimer, &QTimer::timeout, this, &KToolButton::showTooltip);
}

KToolButton::~KToolButton()
{
    delete m_pSvgRenderer;
}

void KToolButton::initToolButton()
{
	switch (static_cast<int>(m_drawFlag))
	{
	case static_cast<int>(KGlobalData::KDrawFlag::MouseDrawFlag):
		m_iconName = "mouse.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::PenDrawFlag):
		m_iconName = "pen.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::LineDrawFlag):
		m_iconName = "line.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::RectDrawFlag):
		m_iconName = "rect.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::CircleDrawFlag):
		m_iconName = "circle.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::PentDrawFlag):
		m_iconName = "pent.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::HexaDrawFlag):
		m_iconName = "hexa.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::StarDrawFlag):
		m_iconName = "star.svg";
		break;
	case static_cast<int>(KGlobalData::KDrawFlag::TextDrawFlag):
		m_iconName = "text.svg";
		break;
	default:
		break;
	}

	m_pSvgRenderer = new QSvgRenderer(":/icons/normal/" + m_iconName);
	this->setMinimumSize(32, 32);

	//toggled :在按钮状态发生变化时,发出此信号
	(void)connect(this, &KToolButton::toggled,this,&KToolButton::onToggledButton);

    // Initial tooltip update
    updateTooltip();
}


void KToolButton::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	m_pSvgRenderer->render(&painter,QRectF(0,0,32,32));

}

void KToolButton::enterEvent(QEvent* event)
{
	if (!isChecked())//判断按钮是否选中
		m_pSvgRenderer->load(":/icons/hover/" + m_iconName);//如果没有选中，加载 hover 的图标

	setCursor(Qt::PointingHandCursor); //设置光标样式

	update();// 触发绘图事件

    // Start timer to show tooltip
    if (!m_buttonIdentifier.isEmpty()) {
        m_tooltipTimer.start();
    }

    QRadioButton::enterEvent(event); // Call base class implementation
}

void KToolButton::leaveEvent(QEvent* event)
{
	if (!isChecked())
		m_pSvgRenderer->load(":/icons/normal/" + m_iconName);

	setCursor(Qt::ArrowCursor);
	update();

    // Stop timer and hide tooltip
    m_tooltipTimer.stop();
    QToolTip::hideText();

    QRadioButton::leaveEvent(event); // Call base class implementation
}


void KToolButton::mouseReleaseEvent(QMouseEvent* e)
{
	setChecked(true);// 设置当前按钮为选中状态
    QRadioButton::mouseReleaseEvent(e); // Call base class implementation
}

void KToolButton::onToggledButton(bool checked)
{
	if (isChecked())
	{
		m_pSvgRenderer->load(":/icons/selected/" + m_iconName);
		KGlobalData::getGlobalDataIntance()->setDrawFlag(m_drawFlag);
	}
	else
		m_pSvgRenderer->load(":/icons/normal/" + m_iconName);
	update();
}

// Slot to update tooltip text
void KToolButton::updateTooltip()
{
    if (!m_buttonIdentifier.isEmpty()) {
        QPair<QString, QString> tip = KGlobalData::getGlobalDataIntance()->getButtonTip(m_buttonIdentifier);
        if (!tip.first.isEmpty() || !tip.second.isEmpty()) {
            // Format tooltip text as "Title\nText"
            QString tooltipText = tip.first;
            if (!tip.first.isEmpty() && !tip.second.isEmpty()) {
                tooltipText += "\n";
            }
            tooltipText += tip.second;
            setToolTip(tooltipText);
            qDebug() << "Tooltip updated for" << m_buttonIdentifier << ":" << tooltipText;
        } else {
            setToolTip(""); // Clear tooltip if no data found
            qDebug() << "No tooltip data found for" << m_buttonIdentifier;
        }
    } else {
        setToolTip(""); // Clear tooltip if no identifier
    }
}

// Slot to show tooltip after a delay
void KToolButton::showTooltip()
{
    if (!toolTip().isEmpty()) {
        QToolTip::showText(mapToGlobal(QPoint(width(), 0)), toolTip(), this);
    }
}
