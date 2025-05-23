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
	Contains,// ȫ�� 
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
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override; // �༭�ı���ʹ��

	KShape* getCurrentShape(const QPoint &pos);// ��ȡ��ǰλ��ͼ��
	QList <KShape*>& getShapeList();
	KTransType getTransType(const QPoint &pos); // ��ȡ�ƶ�����
	void updateCusorStyle(KGlobalData::KDrawFlag flag, KTransType transType);
	void dragMoveShape(KTransType transType, const QPoint& pos);

	// �½������롢����
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
	QSvgRenderer* m_renderer; // ����svg��Ⱦ

	KShape* m_pCurrentShape;
	QList <KShape*> m_pShapeList;// �洢��ǰ�����е�ͼ��
	QList <KShape*> m_isSelectedShapeList; //��ѡ�е�ͼ�ε���������һ��һ��ѡ��ͼ��
	QList <KShape*> m_pdeletedShapeList; // ���ɾ��ͼ�ε�����������������λ���ͷ��ڴ�
	QPoint m_lastPos;// ��¼ǰһ�ε�λ��
	KTransType m_TransType;// ��¼�ƶ�����
	
	bool m_isLPress;// ���������
	bool m_isDrawing;// �Ƿ��ͼ
	bool m_isSelected;// �Ƿ�Ϊѡ��
	bool m_isRender; // �Ƿ����svg

	KShapeClipboard* m_pClipBoard; // ���а�
	QPoint m_clickedPos; //��ǰ���ѡ��λ�ã�������ճ������ͼ��ָ��λ��ʹ��
};

#endif