#ifndef __K_CANVAS_H_
#define __K_CANVAS_H_

#include <QWidget>
#include <QSvgRenderer>
#include <qundostack.h>

#include "kshape.h"
#include "kglobaldata.h"

enum class KTransType
{
	None = 0,
	TopLeft, 
	Top,
	TopRight,
	Left,
	Contains,// 全部 
	Right,
	BottomLeft,
	Bottom,
	BottomRight
};

class KShape;
class KGlobalData;

class KCanvas : public QWidget
{
	Q_OBJECT
	friend class KMainWindow;
	friend class KSvgMainWindow;
public:
	KCanvas(QWidget *parent = Q_NULLPTR);
	~KCanvas();

	virtual void paintEvent(QPaintEvent* event) override;
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override; // 编辑文本框使用

	KShape* getCurrentShape(const QPoint &pos);// 获取当前位置图形
	QList <KShape*>& getShapeList();
	KTransType getTransType(const QPoint &pos); // 获取移动类型
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType, const QPoint& pos);

	// 新建、导入、保存
	void createCanvas();
	void loadSvg();
	void saveToSvg();
	void saveToPng();

signals:
	void shapeParamChanged(KShape* shape);
	void canvasParamsChanged();
	void isSelectedShape();

private:
	int findShapeIndex(KShape* shapeToFind) const;
	QSvgRenderer* m_renderer; // 导入svg渲染

	KShape* m_pCurrentShape;
	QList <KShape*> m_pShapeList;// 存储当前画布中的图形
	QList <KShape*> m_isSelectedShapeList; //被选中的图形的容器，可一个一个选中图形
	QList <KShape*> m_pdeletedShapeList; // 存放删除图形的容器，在析构函数位置释放内存
	QPoint m_lastPos;// 记录前一次的位置
	KTransType m_TransType;// 记录移动类型
	
	bool m_isLPress;// 标记鼠标左键
	bool m_isDrawing;// 是否绘图
	bool m_isSelected;// 是否为选中
	bool m_isRender; // 是否加载svg

	KShapeClipboard* m_pClipBoard; // 剪切板
	QPoint m_clickedPos; //当前鼠标选中位置，供复制粘贴单个图形指定位置使用
};

#endif