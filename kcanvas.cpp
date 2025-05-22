#include "kcanvas.h"
#include "kshapefactory.h"
#include "kpen.h"
#include "ktext.h"
#include "kshapecommand.h"
#include "kaddcommand.h"
#include "kclipboard.h"

#include <QMouseEvent>
#include <QDebug>
#include <QSvgGenerator>
#include <QMessageBox>
#include <QSvgRenderer>
#include <QFileDialog>
#include <QShortcut>

KCanvas::KCanvas(QWidget* parent)
	: QWidget(parent)
	, m_renderer(Q_NULLPTR)
	, m_pCurrentShape(Q_NULLPTR)
	, m_TransType(KTransType::None)
	, m_isDrawing(false)
	, m_isLPress(false)
	, m_isSelected(false)
	, m_isRender(false)
	, m_pClipBoard(new KShapeClipboard(this))
{
	setAttribute(Qt::WA_StyledBackground, true);

	// 设置背景颜色为白色
	setStyleSheet("background-color:#FFFFFF");

	KGlobalData::getGlobalDataIntance()->setCanvasColor("FFFFFF");
	resize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight());

	setMouseTracking(true);//设置鼠标追踪
	setFocusPolicy(Qt::ClickFocus);// 单击获取焦点
}

KCanvas::~KCanvas()
{
	for (auto shape : m_pShapeList)
		delete shape;
	for (auto shape : m_pdeletedShapeList)
		delete shape;
	m_pShapeList.clear();
	m_pdeletedShapeList.clear();
	m_pClipBoard->clear(); // 析构的时候清除剪切板
}

void KCanvas::createCanvas()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, QString::fromLocal8Bit("新建画布"), QString::fromLocal8Bit("确定新建画布吗？请注意此操作会清空当前内容！"),
		QMessageBox::Yes | QMessageBox::No);
	if (reply == QMessageBox::No)
		return;

	// 释放内存
	for (auto shape : m_pShapeList)
		delete shape;
	for (auto shape : m_pdeletedShapeList)
		delete shape;
	m_pShapeList.clear();
	m_pdeletedShapeList.clear();
	m_isSelectedShapeList.clear();
	KGlobalData::getGlobalDataIntance()->getUndoStack()->clear();
	m_pClipBoard->clear();

	delete m_renderer;
	m_renderer = Q_NULLPTR;
	update();
}

void KCanvas::loadSvg()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		QString::fromLocal8Bit("选择SVG文件"),
		"", // 默认目录可以为空，或者指定一个初始目录  
		"SVG 文件 (*.svg);;所有文件 (*.*)"); // 文件过滤器  


	if (fileName.isEmpty())
		return;

	if (m_renderer == Q_NULLPTR)
		m_renderer = new QSvgRenderer(this);

	if (!m_pShapeList.isEmpty())
	{
		for (auto shape : m_pShapeList)
			delete shape;
		m_pShapeList.clear();
	}

	// 加载 SVG 文件
	m_renderer->load(fileName);
	if (!m_renderer->isValid())
	{
		qDebug() << "Error: SVG file is invalid or could not be loaded.";
		return;
	}

	// 重新绘制窗口
	update();
}

void KCanvas::saveToSvg()
{
	QString fileName = QFileDialog::getSaveFileName(nullptr, QString::fromLocal8Bit("导出为SVG文件"), "svg.svg");
	if (fileName.isEmpty())
		return;

	QString svgBackgroundColor = KGlobalData::getGlobalDataIntance()->getCanvasColor();

	// 创建 QSvgGenerator 对象并配置
	QSvgGenerator svgGenerator;
	svgGenerator.setFileName(fileName);
	svgGenerator.setSize(QSize(KGlobalData::getGlobalDataIntance()->getCanvasWidth(), 
		KGlobalData::getGlobalDataIntance()->getCanvasHeight()));  // 设置 SVG 图像大小
	svgGenerator.setViewBox(QRect(0, 0, KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight())); // 设置 SVG 视图框


	QPainter painter;
	painter.begin(&svgGenerator);

	painter.fillRect(QRect(0, 0, svgGenerator.viewBox().width(), svgGenerator.viewBox().height()), Qt::white);

	// 遍历图形列表并绘制每个图形
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawSave(painter);
			it++;
		}
	}
	painter.end();
}

void KCanvas::saveToPng()
{

	QString fileName = QFileDialog::getSaveFileName(nullptr, QString::fromLocal8Bit("导出为PNG文件"), "svg.png");
	if (fileName.isEmpty())
		return;
	// 创建 QImage 对象并配置大小
	QImage image(KGlobalData::getGlobalDataIntance()->getCanvasWidth(),
		KGlobalData::getGlobalDataIntance()->getCanvasHeight(),
		QImage::Format_ARGB32);

	QPainter painter(&image);
	painter.fillRect(image.rect(), Qt::white); // 填充白色背景

	// 遍历图形列表并绘制每个图形
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawSave(painter);
			it++;
		}
	}

	painter.end();

	// 保存 QImage 到文件
	if (!image.save(fileName)) 
	{
		qDebug() << "Failed to save PNG file:" << fileName;
	}
}

// 绘图事件处理
void KCanvas::paintEvent(QPaintEvent* event)
{
	qDebug() << QString::fromLocal8Bit("1-当前选中图形数量： ") << m_isSelectedShapeList.size();
	qDebug() << QString::fromLocal8Bit("2-删除列表图形数量： ") << m_pdeletedShapeList.size();
	qDebug() << QString::fromLocal8Bit("3-当前所有图形数量： ") << m_pShapeList.size();
	qDebug() << QString::fromLocal8Bit("--------------------------------------");
	if (m_renderer != Q_NULLPTR)
	{
		QPainter painter(this);
		m_renderer->render(&painter, this->rect());
	}

#if 0
	if (!m_pShapeList->isEmpty())
	{
			//遍历图形对象的  List
		for (QList<KShape*>::iterator it = m_pShapeList->begin();
								it != m_pShapeList->end(); ++it)
			(*it)->drawShape(this);// 绘制
	}
#endif
	if (!m_pShapeList.isEmpty())
	{
		auto it = m_pShapeList.rbegin();
		while (it != m_pShapeList.rend())
		{
			(*it)->drawShape(this);
			it++;
		}
	}

	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_isDrawing) // 图形绘制标志为 true，则表示正在绘制该图形，此时需要不断重新绘制
			m_pCurrentShape->drawShape(this);
	}
	if (!m_isSelectedShapeList.isEmpty())
	{
		for (auto shape : m_isSelectedShapeList)
		{
			shape->drawOutLine(this);
		}
	}
}

void KCanvas::mousePressEvent(QMouseEvent* event)
{
	QPoint mousePos = event->pos();
	mousePos.setX(mousePos.x() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	mousePos.setY(mousePos.y() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	qDebug() << QString::fromLocal8Bit("鼠标位置 ： ") << mousePos;
	//获取当前需要绘制的图形类型
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;
	
	if (event->buttons() == Qt::LeftButton)
	{
		m_isLPress = true;// 记录鼠标左键已经点击
		bool ctrlPressed = event->modifiers() & Qt::ControlModifier; //检查ctrl状态
		if (flag == KGlobalData::KDrawFlag::MouseDrawFlag)//如果是选中图形事件
		{
			m_pCurrentShape = getCurrentShape(mousePos);
			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (!ctrlPressed && m_isSelectedShapeList.size() == 1)
					m_isSelectedShapeList.clear(); // 如果 Ctrl 没有按下或者列表只有一个，清空之前选中的图形列表

				if (ctrlPressed)
				{
					if (!m_isSelectedShapeList.contains(m_pCurrentShape))
						m_isSelectedShapeList.append(m_pCurrentShape); //添加图形到选中图形列表
				}
				if (!m_isSelectedShapeList.contains(m_pCurrentShape))
					m_isSelectedShapeList.append(m_pCurrentShape); //添加图形到选中图形列表

				m_lastPos = mousePos;// 当前位置移动的最后位置(会更新)
				m_isSelected = true;// 设置选中标志
				emit shapeParamChanged(m_pCurrentShape); // 发送选中图形参数，更新UI
				m_TransType = getTransType(mousePos);// 获取移动类型
			}
			else
			{
				m_isSelected = false;
				if (!ctrlPressed)
					m_isSelectedShapeList.clear();
			}
		}
		else
		{ 
			m_isSelected = false;
			m_pCurrentShape = KShapeFactory::createShape(KGlobalData::getGlobalDataIntance()->getDrawFlag());

			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
					qobject_cast<KPen*>(m_pCurrentShape)->addPos(mousePos);
				else if (flag == KGlobalData::KDrawFlag::TextDrawFlag)
					m_pCurrentShape->setStartPoint(event->pos());
				else
					m_pCurrentShape->setStartPoint(mousePos);
			}
		}
	}
	else // 右键显示菜单
	{
		m_pCurrentShape = getCurrentShape(mousePos);
		if (m_pCurrentShape != Q_NULLPTR)
			emit isSelectedShape();
	}
	update();
}

void KCanvas::mouseMoveEvent(QMouseEvent* event)
{
	QPoint mousePos = event->pos();
	mousePos.setX(mousePos.x() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	mousePos.setY(mousePos.y() / KGlobalData::getGlobalDataIntance()->getCanvasScale());

	//qDebug() << QString::fromLocal8Bit("鼠标位置 ： ") << mousePos;
	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();
	if (flag == KGlobalData::KDrawFlag::NoneDrawFlag)
		return;

	KTransType transType = getTransType(mousePos);
	updateCusorStyle(flag, transType);//选中时设置鼠标选中样式

	if (event->buttons() != Qt::LeftButton)//bug : 在不点击鼠标左键并在在选中状态，移动鼠标会移动图形
			return;


	if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)
	{
		if (m_isLPress && !m_isDrawing)
			m_isDrawing = true;
		if (m_pCurrentShape != Q_NULLPTR)
		{
			if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
				qobject_cast<KPen*>(m_pCurrentShape)->addPos(mousePos);
			else
				m_pCurrentShape->setEndPoint(mousePos);
		}
	}
	else
	{
		if (m_pCurrentShape == Q_NULLPTR)
			return;
		dragMoveShape(transType, mousePos);
		emit shapeParamChanged(m_pCurrentShape);
	}

	update();
}

void KCanvas::mouseReleaseEvent(QMouseEvent* event)
{
	QPoint mousePos = event->pos();
	mousePos.setX(mousePos.x() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	mousePos.setY(mousePos.y() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	m_clickedPos = mousePos; //记录点击的位置，方便复制粘贴

	KGlobalData::KDrawFlag flag = KGlobalData::getGlobalDataIntance()->getDrawFlag();

	if (m_isLPress) // 鼠标左键按下后释放，则表示移动或者绘图完毕
	{
		if (flag != KGlobalData::KDrawFlag::MouseDrawFlag)// 如果是绘图
		{
			if (m_pCurrentShape != Q_NULLPTR)
			{
				if (flag == KGlobalData::KDrawFlag::PenDrawFlag)
					qobject_cast<KPen*>(m_pCurrentShape)->addPos(mousePos);
				else if (flag == KGlobalData::KDrawFlag::TextDrawFlag)
					qobject_cast<KText*>(m_pCurrentShape)->startEditing(event->pos(), this);
				else
					m_pCurrentShape->setEndPoint(mousePos);
				
				if (m_pCurrentShape->isValid())// 判断图形矩形是否有效
				{

					KShapeParams curparams1 = m_pCurrentShape->getCurShapeParams();
					m_pCurrentShape->setCurShapeParams(curparams1);
					m_pCurrentShape->setPrevShapeParams(); // 设置前一步状态

					m_pShapeList.prepend(m_pCurrentShape);

					// 添加图形的command 方便回退
					KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KAddCommand(m_pCurrentShape, 
						&m_pShapeList, &m_pdeletedShapeList));
					
					m_pCurrentShape = Q_NULLPTR;
					//m_isSelected = true;
					//m_isDrawing = false;
				}
				else
				{
					delete m_pCurrentShape;
					m_pCurrentShape = Q_NULLPTR;
				}
			}
		}

		else
		{
			if (m_pCurrentShape!= Q_NULLPTR)
			{
				KShapeParams curparams = m_pCurrentShape->getCurShapeParams();
				KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KShapeCommand(curparams, m_pCurrentShape));
			}
		}
		m_isLPress = false;
		m_isDrawing = false;
		m_TransType = KTransType::None;
	}

	update();
}

void KCanvas::mouseDoubleClickEvent(QMouseEvent* event)
{
	QPoint mousePos = event->pos();
	mousePos.setX(mousePos.x() / KGlobalData::getGlobalDataIntance()->getCanvasScale());
	mousePos.setY(mousePos.y() / KGlobalData::getGlobalDataIntance()->getCanvasScale());

	if (m_pCurrentShape != Q_NULLPTR)
	{
		if (m_pCurrentShape->getShapeType() == KShapeType::TextShapeType)
			qobject_cast<KText*>(m_pCurrentShape)->setEditing();
	}

	update();
}

KShape* KCanvas::getCurrentShape(const QPoint &pos)
{
	for (QList<KShape*>::iterator it = m_pShapeList.begin();
		it != m_pShapeList.end(); ++it)
	{
		if ((*it)->getShapeRect().contains(pos))
			return *it;
	}

	return Q_NULLPTR;
}

QList<KShape*>& KCanvas::getShapeList()
{
	return m_pShapeList;
}

KTransType KCanvas::getTransType(const QPoint& pos)
{
	if (!m_pCurrentShape)
		return KTransType::None;

	// 获取图形矩形
	QRect rect = m_pCurrentShape->getShapeRect();
	//qDebug() << QString::fromLocal8Bit("当前鼠标位置： ") << pos;
	if (!rect.contains(pos))
		return KTransType::None;
	
	//qDebug() << "rect = " << rect.topLeft().x();
	//qDebug() << "rect = " << rect.topLeft().y();

	// 判断是整体移动，局部拖拽移动
	if (qAbs(pos.x() - rect.topLeft().x()) < 5 
			&& qAbs(pos.y() - rect.topLeft().y()) < 5)
		return KTransType::TopLeft;

	if (qAbs(rect.topRight().x() - pos.x()) < 5 
			&& qAbs(pos.y() - rect.topRight().y()) < 5)
		return KTransType::TopRight;

	if (qAbs(rect.bottomRight().x() - pos.x()) < 5 
			&& qAbs(rect.bottomRight().y() - pos.y()) < 5)
		return KTransType::BottomRight;

	if (qAbs(pos.x() - rect.bottomLeft().x()) < 5 
			&& qAbs(rect.bottomLeft().y() - pos.y()) < 5)
		return KTransType::BottomLeft;

	if (qAbs(pos.y() - rect.top()) < 5)
		return KTransType::Top;

	if (qAbs(rect.right() - pos.x()) < 5)
		return KTransType::Right;

	if (qAbs(rect.bottom() - pos.y()) < 5 )
		return KTransType::Bottom;

	if (qAbs(pos.x() - rect.left()) < 5 )
		return KTransType::Left;

	return KTransType::Contains;
}

void KCanvas::updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType)
{
	// 如果已经选中
	if (m_isSelected
		&& flag == KGlobalData::KDrawFlag::MouseDrawFlag
		&& transType != KTransType::None)
	{
		//TODO：变换图标,设置不同的鼠标样式,根据点击位置，设置不同的鼠标样式
		if (transType == KTransType::TopLeft || transType == KTransType::BottomRight)
			setCursor(Qt::SizeFDiagCursor);
		else if (transType == KTransType::TopRight || transType == KTransType::BottomLeft)
			setCursor(Qt::SizeBDiagCursor);
		else if (transType == KTransType::Top || transType == KTransType::Bottom)
			setCursor(Qt::SizeVerCursor);
		else if (transType == KTransType::Left || transType == KTransType::Right)
			setCursor(Qt::SizeHorCursor);

		else if (transType == KTransType::Contains)
			setCursor(Qt::SizeAllCursor);
	}
	else
		unsetCursor();
}

void KCanvas::dragMoveShape(KTransType transType,const QPoint &pos)
{
	if (m_pCurrentShape == nullptr)
		return;

	/*
		在移动过程中，分为两种移动方式
			(1) 整体移动  contains
			(2) 基于某一个方向的拖拽移动

	*/
	switch (m_TransType)
	{
	case KTransType::None:
		return;
	case KTransType::TopLeft:
		// 如果是左上角，则当前位置为新的矩形起始位置
		m_pCurrentShape->moveTopLeft(pos);
		break;
	case KTransType::Top:
		// 如果是向上移动，则修改起始位置的 y 坐标
		m_pCurrentShape->moveTop(pos);
		break;
	case KTransType::TopRight:
		m_pCurrentShape->moveTopRight(pos);
		break;
	case KTransType::Left:
		m_pCurrentShape->moveLeft(pos);
		break;
	case KTransType::Contains: // 整体移动
	{
		QPoint newpos = pos - m_lastPos;
		if (m_isSelectedShapeList.size() > 1)
		{
			for (auto shape : m_isSelectedShapeList)
			{
				shape->move(newpos);
			}
		}
		else
		{
			m_pCurrentShape->move(newpos);
		}
		m_lastPos = pos;
	}
	break;
	case KTransType::Right:
		m_pCurrentShape->moveRight(pos);
		break;
	case KTransType::BottomLeft:
		m_pCurrentShape->moveBottomLeft(pos);
		break;
	case KTransType::Bottom:
		m_pCurrentShape->moveBottom(pos);
		break;
	case KTransType::BottomRight:
		m_pCurrentShape->moveBottomRight(pos);
		break;
	default:
		break;
	}
}

int KCanvas::findShapeIndex(KShape* shapeToFind) const
{
	int index = -1; 

	QList<KShape*>::const_iterator it;
	for (it = m_pShapeList.begin(); it != m_pShapeList.end(); ++it) 
	{
		if (*it == shapeToFind) 
		{
			index = std::distance(m_pShapeList.begin(), it);  //index
			break;  
		}
	}

	return index;
}