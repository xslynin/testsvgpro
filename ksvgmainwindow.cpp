#include "ksvgmainwindow.h"
#include "kclipboard.h"
#include "kshapecommand.h"
#include "kdeletecommand.h"
#include "kaddcommand.h"
#include "kcanvasmenu.h"

#include <QApplication>
#include <QWheelEvent>
#include <QDebug>



KSvgMainWindow::KSvgMainWindow(QWidget *parent)
	: QScrollArea(parent)
	, m_pCanvas(Q_NULLPTR)
{
	setFrameShape(QFrame::NoFrame); // �����ޱ߿�

	// �����ݳ�����ʾ����ʱ���ֹ�����
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setStyleSheet("QScrollArea{background-color:#3f3f3c;}");
	setAlignment(Qt::AlignCenter);
	
	m_pCanvas = new KCanvas;
	setWidget(m_pCanvas);// ���õ�ǰ�ؼ�Ϊ����������ӿؼ�
	m_canvasMenu = new KCanvasMenu(this);
	initCanvasMenuConnection();
	initPenCursor();
}

KSvgMainWindow::~KSvgMainWindow()
{
}

void KSvgMainWindow::initCanvasMenuConnection()
{
	connect(m_canvasMenu->m_selectAllAct, &QAction::triggered, this, &KSvgMainWindow::selectAllShape);
	connect(m_canvasMenu->m_cutAct, &QAction::triggered, this, &KSvgMainWindow::cutCurrentShape);
	connect(m_canvasMenu->m_copyAct, &QAction::triggered, this, &KSvgMainWindow::copyCurrentShape);
	connect(m_canvasMenu->m_pasteAct, &QAction::triggered, this, &KSvgMainWindow::pasteCurrentShape);
	connect(m_canvasMenu->m_copyAndPasteAct, &QAction::triggered, this, &KSvgMainWindow::copyAndPasteCurrentShape);
	connect(m_canvasMenu->m_deleteAct, &QAction::triggered, this, &KSvgMainWindow::deleteCurrentShape);
	connect(m_canvasMenu->m_upLayerAct, &QAction::triggered, this, &KSvgMainWindow::upLayer);
	connect(m_canvasMenu->m_downLayerAct, &QAction::triggered, this, &KSvgMainWindow::downLayer);
	connect(m_canvasMenu->m_upFrontLayerAct, &QAction::triggered, this, &KSvgMainWindow::upFrontLayer);
	connect(m_canvasMenu->m_downBackLayerAct, &QAction::triggered, this, &KSvgMainWindow::downBackLayer);
	connect(m_canvasMenu->m_undoAct, &QAction::triggered, this, &KSvgMainWindow::shapeUndo);
	connect(m_canvasMenu->m_redoAct, &QAction::triggered, this, &KSvgMainWindow::shapeRedo);
	connect(m_pCanvas, &KCanvas::isSelectedShape, this, [this]()
		{
			m_canvasMenu->m_isSelectedShape = true;
		});
}

void KSvgMainWindow::initPenCursor()
{
	QPixmap penPixmap(":/icons/pen.png");
	QPoint hotSpot(0, penPixmap.height());
	penCursor = QCursor(penPixmap, hotSpot.x(), hotSpot.y());
}

void KSvgMainWindow::setCustorStyle(KGlobalData::KDrawFlag drawFlag)
{
	switch (drawFlag)
	{
	case KGlobalData::KDrawFlag::PenDrawFlag:
		setCursor(penCursor);
		//setCursor(Qt::ArrowCursor);
		break;
	case KGlobalData::KDrawFlag::LineDrawFlag:
	case KGlobalData::KDrawFlag::RectDrawFlag:
	case KGlobalData::KDrawFlag::CircleDrawFlag:
	case KGlobalData::KDrawFlag::PentDrawFlag:
	case KGlobalData::KDrawFlag::HexaDrawFlag:
	case KGlobalData::KDrawFlag::StarDrawFlag:

		setCursor(Qt::CrossCursor);
		break;

	case KGlobalData::KDrawFlag::TextDrawFlag:
		setCursor(Qt::IBeamCursor);
		break;
	default:
		setCursor(Qt::ArrowCursor);
		break;
	}
}

void KSvgMainWindow::enterEvent(QEvent* event)
{
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	setCustorStyle(flag);
	QScrollArea::enterEvent(event);
}

void KSvgMainWindow::wheelEvent(QWheelEvent* event)
{
	if (QApplication::keyboardModifiers() == Qt::ControlModifier)
	{
		qreal initscale = 1.0;
		qreal scale = 1.0;
		if (event->delta() > 0)
		{
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() + 0.05;
			initscale = 1.0 + 0.05;
		}
		else
		{
			scale = KGlobalData::getGlobalDataIntance()->getCanvasScale() - 0.05;
			initscale = 1 - 0.05;
		}


		qint32 width1 = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
		qint32 height1 = KGlobalData::getGlobalDataIntance()->getCanvasHeight();

		m_pCanvas->resize(width1 * scale, height1 * scale);
		//�������ݵ�ȫ�ֱ���
		KGlobalData::getGlobalDataIntance()->setCanvaScale(scale);
		emit scaleChanged(scale); // ���������ڵĲ��������������ű���

		//QPoint oldCenter
		//qDebug() << " widget center :" << m_pCanvas->geometry().center(); //�������ģ�����ı�

		QPoint topLeft = m_pCanvas->mapToParent(QPoint(0, 0));
		QPoint bottomRight = m_pCanvas->mapToParent(QPoint(m_pCanvas->width(), m_pCanvas->height()));

		qDebug() << " widget topLeft  :" << topLeft;
		qDebug() << " widget bottomRight :" << bottomRight;

		m_pCanvas->update();
	}
}

void KSvgMainWindow::selectAllShape()
{
	m_pCanvas->m_isSelectedShapeList.clear();
	for (auto shape : m_pCanvas->m_pShapeList)
	{
		m_pCanvas->m_isSelectedShapeList.push_back(shape);
	}
}

void KSvgMainWindow::cutCurrentShape()
{
	m_pCanvas->m_pClipBoard->copy(m_pCanvas->m_isSelectedShapeList);
	deleteCurrentShape(); // ɾ��������ֱ�ӳ���
}

void KSvgMainWindow::copyCurrentShape()
{
	m_pCanvas->m_pClipBoard->copy(m_pCanvas->m_isSelectedShapeList);
}

void KSvgMainWindow::pasteCurrentShape()
{
	for (auto shape : m_pCanvas->m_pClipBoard->getData(m_pCanvas->m_clickedPos, m_pCanvas))
	{
		m_pCanvas->m_pShapeList.append(shape);
		m_pCanvas->m_pCurrentShape = shape;

		KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KAddCommand(m_pCanvas->m_pCurrentShape,
			&m_pCanvas->m_pShapeList, &m_pCanvas->m_pdeletedShapeList));
	}
	update();
}

void KSvgMainWindow::copyAndPasteCurrentShape()
{
	copyCurrentShape();
	pasteCurrentShape();
}

void KSvgMainWindow::deleteCurrentShape()
{
	// ɾ��ͼ�ε�ʱ�򣬰ѻ����б��ָ���Ƴ����ڴ���ɾ��ͼ���б�����������ʱ����ͷ�
	if (!m_pCanvas->m_isSelectedShapeList.isEmpty())
	{
		for (auto shape : m_pCanvas->m_isSelectedShapeList)
		{
			m_pCanvas->m_pShapeList.removeOne(shape);
			KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KDeleteCommand(&m_pCanvas->m_isSelectedShapeList, 
				&m_pCanvas->m_pShapeList,&m_pCanvas->m_pdeletedShapeList));
		}
	}
	m_pCanvas->m_isSelectedShapeList.clear();
	m_pCanvas->update();
}

void KSvgMainWindow::keyPressEvent(QKeyEvent* event)
{
	QWidget::keyReleaseEvent(event);
	// ��鲢�����ݼ�  
	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_A && m_canvasMenu->m_selectAllAct && m_canvasMenu->m_selectAllAct->isEnabled())
		m_canvasMenu->m_selectAllAct->trigger(); // Ctrl+A ȫѡ  

	if (event->matches(QKeySequence::Undo) && m_canvasMenu->m_undoAct && m_canvasMenu->m_undoAct->isEnabled())
		m_canvasMenu->m_undoAct->trigger(); // ����  

	if (event->matches(QKeySequence::Redo) && m_canvasMenu->m_redoAct && m_canvasMenu->m_redoAct->isEnabled())
		m_canvasMenu->m_redoAct->trigger(); // ����  

	if (event->matches(QKeySequence::Cut) && m_canvasMenu->m_cutAct && m_canvasMenu->m_cutAct->isEnabled())
		m_canvasMenu->m_cutAct->trigger(); // ����  

	if (event->matches(QKeySequence::Copy) && m_canvasMenu->m_copyAct && m_canvasMenu->m_copyAct->isEnabled())
		m_canvasMenu->m_copyAct->trigger(); // ����  

	if (event->matches(QKeySequence::Paste) && m_canvasMenu->m_pasteAct && m_canvasMenu->m_pasteAct->isEnabled())
		m_canvasMenu->m_pasteAct->trigger(); // ճ��  

	if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == Qt::Key_D && m_canvasMenu->m_copyAndPasteAct && m_canvasMenu->m_copyAndPasteAct->isEnabled())
		m_canvasMenu->m_copyAndPasteAct->trigger(); // Ctrl+Shift+D ���Ʋ�ճ��  

	if (event->key() == Qt::Key_Delete && m_canvasMenu->m_deleteAct && m_canvasMenu->m_deleteAct->isEnabled())
		m_canvasMenu->m_deleteAct->trigger(); // ɾ��  

	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Up && m_canvasMenu->m_upLayerAct && m_canvasMenu->m_upLayerAct->isEnabled())
		m_canvasMenu->m_upLayerAct->trigger(); // Ctrl+Up ����һ��  

	if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Down && m_canvasMenu->m_downLayerAct && m_canvasMenu->m_downLayerAct->isEnabled())
		m_canvasMenu->m_downLayerAct->trigger(); // Ctrl+Down ����һ��  

	if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == Qt::Key_Up && m_canvasMenu->m_upFrontLayerAct && m_canvasMenu->m_upFrontLayerAct->isEnabled())
		m_canvasMenu->m_upFrontLayerAct->trigger(); // Ctrl+Shift+Up ���ڶ���  

	if (event->modifiers() == (Qt::ControlModifier | Qt::ShiftModifier) && event->key() == Qt::Key_Down && m_canvasMenu->m_downBackLayerAct && m_canvasMenu->m_downBackLayerAct->isEnabled())
		m_canvasMenu->m_downBackLayerAct->trigger(); // Ctrl+Shift+Down ���ڵײ� 
	update();
}

void KSvgMainWindow::upLayer()
{
	int index = m_pCanvas->findShapeIndex(m_pCanvas->m_pCurrentShape);
	if (index != -1)
	{
		if (index < m_pCanvas->m_pShapeList.size() && index > 0)
		{
			m_pCanvas->m_pShapeList.swap(index, index - 1);
		}
	}

}

void KSvgMainWindow::downLayer()
{
	int index = m_pCanvas->findShapeIndex(m_pCanvas->m_pCurrentShape);
	if (index != -1)
	{
		if (index < m_pCanvas->m_pShapeList.size() - 1 && index >= 0)
		{
			m_pCanvas->m_pShapeList.swap(index, index + 1);
		}
	}
}

void KSvgMainWindow::upFrontLayer()
{
	int index = m_pCanvas->findShapeIndex(m_pCanvas->m_pCurrentShape);
	if (index != -1)
	{
		if (index < m_pCanvas->m_pShapeList.size() && index > 0)
		{
			m_pCanvas->m_pShapeList.insert(0, m_pCanvas->m_pShapeList.takeAt(index));
		}
	}
}

void KSvgMainWindow::downBackLayer()
{
	int index = m_pCanvas->findShapeIndex(m_pCanvas->m_pCurrentShape);
	if (index != -1)
	{
		if (index < m_pCanvas->m_pShapeList.size() - 1 && index >= 0)
		{
			m_pCanvas->m_pShapeList.append(m_pCanvas->m_pShapeList.takeAt(index));
		}
	}
}

void KSvgMainWindow::shapeUndo()
{
	m_pCanvas->m_isSelectedShapeList.clear();
	KGlobalData::getGlobalDataIntance()->getUndoStack()->undo();
	emit m_pCanvas->canvasParamsChanged();
}

void KSvgMainWindow::shapeRedo()
{
	KGlobalData::getGlobalDataIntance()->getUndoStack()->redo();
	emit m_pCanvas->canvasParamsChanged();
}