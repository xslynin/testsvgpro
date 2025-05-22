#include "ktext.h"
#include "kglobaldata.h"

#include <qcoreevent.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QLineEdit>
#include <QDebug>


KText::KText(QObject* parent)
	: KShape(parent)
	, m_textEdit(new QLineEdit(Q_NULLPTR))
	, m_isEditing(false)
	, m_initText("hello!")
{
	m_textEdit->hide();
	m_textEdit->setText(m_initText);
	m_textEdit->installEventFilter(this);

	connect(m_textEdit, &QLineEdit::editingFinished, this, &KText::finishEditing);
}

KText::~KText()
{
	delete m_textEdit;
}

void KText::drawShape(QPaintDevice* parent)
{
	QPainter painter(parent);
	painter.setRenderHint(QPainter::Antialiasing);
	qreal scaleFactor = KGlobalData::getGlobalDataIntance()->getCanvasScale();
	painter.scale(scaleFactor, scaleFactor);

	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);

	m_boundingRect = getCorrectBoundingbox();
	int fontSize = calculateFontSizeForRect(QRect(getStartPoint(), getEndPoint()));
	if (m_isEditing == true)
	{
		m_textEdit->setGeometry(m_boundingRect);
		m_textEdit->show();
		m_textEdit->setFocus();
		m_textEdit->setStyleSheet("border: 1px dashed black;");
		QFont editFont = m_textEdit->font();
		editFont.setPointSize(fontSize);  // 使用与绘制文本相同的字体大小
		m_textEdit->setFont(editFont);
	}
	else
	{
		//qDebug() << "font size " << fontSize;
		// 设置字体
		QFont font = painter.font();
		font.setPointSize(fontSize);
		painter.setFont(font);
		painter.drawText(QRect(getStartPoint(), getEndPoint()), Qt::AlignCenter, m_textEdit->text());
	}
}

void KText::drawSave(QPainter& painter)
{
	QPen pen;
	pen.setColor(getBorderColor());
	pen.setStyle(getPenStyle());
	pen.setWidth(getBorderWidth());

	QBrush brush(getFillColor()); // 获取填充颜色
	painter.setBrush(brush);

	painter.setPen(pen);

	m_boundingRect = getCorrectBoundingbox();

	int fontSize = calculateFontSizeForRect(QRect(getStartPoint(), getEndPoint()));

	if (m_isEditing == true)
	{
		m_textEdit->setGeometry(m_boundingRect);
		m_textEdit->show();
		m_textEdit->setFocus();
		m_textEdit->setStyleSheet("border: 1px dashed black;");
	}
	else
	{
		//qDebug() << "font size " << fontSize;
		QFont font = painter.font();
		font.setPointSize(fontSize);
		painter.setFont(font);
		painter.drawText(QRect(getStartPoint(), getEndPoint()), Qt::AlignCenter, m_textEdit->text());
	}
}

void KText::setEditing()
{
	m_isEditing = true;
}

void KText::setText()
{
	m_textEdit->setText(m_initText);
}

void KText::setEditParent(QWidget* parent)
{
	m_textEdit->setParent(parent);
}


KShapeType KText::getShapeType() const
{
	return KShapeType::TextShapeType;
}

bool KText::isValid()
{
	return !m_textEdit->text().isEmpty();
}

bool KText::eventFilter(QObject* watched, QEvent* event)
{
	if (watched == m_textEdit)
	{
		if(event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
			{
				m_textEdit->editingFinished();
				return true;
			}
			else
			{
				return false;
			}
		}
		if (event->type() == QEvent::FocusOut) // 设置焦点事件，当点击其他位置，自动隐藏关闭
		{
			m_textEdit->editingFinished();
			return true;
		}
	}
	return KShape::eventFilter(watched, event);
}

void KText::startEditing(const QPoint& pos, QWidget *parent)
{
	int width = 150;
	int height = 50;

	m_textEdit->setParent(parent);
	m_isEditing = true;
	m_centerPoint = pos / KGlobalData::getGlobalDataIntance()->getCanvasScale();
	m_curShapeParams.startPoint = QPoint(m_centerPoint.x() - width / 2, m_centerPoint.y() - height / 2);
	m_curShapeParams.endPoint = QPoint(m_centerPoint.x() + width / 2, m_centerPoint.y() + height / 2);
}

void KText::finishEditing()
{
	m_isEditing = false;
	m_textEdit->hide();
}

QRect KText::getCorrectBoundingbox()
{
	QPoint left = m_curShapeParams.startPoint * KGlobalData::getGlobalDataIntance()->getCanvasScale();
	QPoint right = m_curShapeParams.endPoint * KGlobalData::getGlobalDataIntance()->getCanvasScale();
	if (left.y() > right.y() && left.x() < right.x())
	{
		return QRect(QPoint(left.x(), right.y()),
			QPoint(right.x(), left.y()));
	}
	if (left.x() > right.x() && left.y() < right.y())
	{
		return QRect(QPoint(right.x(), left.y()),
			QPoint(left.x(), right.y()));
	}
	if (left.x() > right.x() && left.y() > right.y())
	{
		return QRect(QPoint(right.x(), right.y()),
			QPoint(left.x(), left.y()));
	}
	return QRect(left, right);
}

int KText::calculateFontSizeForRect(const QRectF& rect)
{
	qreal textHeightRatio = 0.3;
	int fontSize = static_cast<int>(qAbs(rect.height()) * textHeightRatio);
	if (fontSize < 8)
		fontSize = 8;
	return fontSize;
}

void KText::writeToStream(QDataStream& out) const
{
	out << m_curShapeParams.startPoint << m_curShapeParams.endPoint
		<< m_curShapeParams.borderWidth << m_curShapeParams.borderColor
		<< static_cast<int>(m_curShapeParams.penStyle) << m_curShapeParams.fillColor
		<< m_boundingRect << m_centerPoint << m_isEditing
		<< m_textEdit->text();
		
}

void KText::readFromStream(QDataStream& in)
{
	int penStyleInt;
	in >> m_curShapeParams.startPoint >> m_curShapeParams.endPoint
		>> m_curShapeParams.borderWidth >> m_curShapeParams.borderColor
		>> penStyleInt >> m_curShapeParams.fillColor
		>> m_boundingRect >> m_centerPoint >> m_isEditing
		>> m_initText;
	m_curShapeParams.penStyle = static_cast<Qt::PenStyle>(penStyleInt);
}
