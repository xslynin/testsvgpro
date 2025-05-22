#ifndef __K_SHAPE_H_
#define __K_SHAPE_H_

#include <QObject>
#include <QPoint>
#include <QPaintDevice>
#include <QPainter>
#include <QPen>

class QSvgGenerator;
class KShapeClipboard;

enum class KShapeType
{
	None = 0,
	PenShapeType,
	LineShapeType,
	CircleShapeType,
	RectShapeType,
	PentShapeType,
	HexaShapeType,
	StarShapeType,
	TextShapeType,
};

// 图形参数结构体
struct KShapeParams
{
	QPoint startPoint;// 起始坐标
	QPoint endPoint; // 结束坐标
	int width;
	int height;
	int borderWidth;
	QColor borderColor;
	Qt::PenStyle penStyle;
	QColor fillColor;

	// 成员函数来检查两个对象是否相等
	bool operator==(const KShapeParams& other) const
	{
		return startPoint == other.startPoint &&
			endPoint == other.endPoint &&
			width == other.width &&
			height == other.height &&
			borderWidth == other.borderWidth &&
			borderColor == other.borderColor &&
			penStyle == other.penStyle &&
			fillColor == other.fillColor;
	}
};

class KShape : public QObject
{
	Q_OBJECT
public:
	KShape(QObject *parent = Q_NULLPTR);
	~KShape();

	// 重运算符，用于将KShape数据写入到 QDataStream,方便复制粘贴。
	// 文本和铅笔子类需要重写，因为信息不一致
	virtual void writeToStream(QDataStream& out) const;
	virtual void readFromStream(QDataStream& in);
	friend QDataStream& operator<<(QDataStream& out, const KShape& shape)
	{
		shape.writeToStream(out);
		return out;
	}

	friend QDataStream& operator>>(QDataStream& in, KShape& shape)
	{
		shape.readFromStream(in);
		return in;
	}

	// 在子类中重写，实现不同类型的图形的绘制
	virtual void drawShape(QPaintDevice *parent = Q_NULLPTR) = 0;
	virtual void drawSave(QPainter& painter) = 0; // 保存使用

	virtual void move(QPoint offset);
	virtual void moveTop(QPoint pos);
	virtual void moveBottom(QPoint pos);
	virtual void moveLeft(QPoint pos);
	virtual void moveRight(QPoint pos);
	virtual void moveTopLeft(QPoint pos);
	virtual void moveTopRight(QPoint pos);
	virtual void moveBottomLeft(QPoint pos);
	virtual void moveBottomRight(QPoint pos);
	virtual KShapeType getShapeType() const;

	QPoint getStartPoint();
	QPoint getEndPoint();

	void setStartPoint(const QPoint &point);
	void setEndPoint(const QPoint &point);

	// 画笔
	void setBorderWidth(const int value);
	int getBorderWidth() const;

	void setBorderColor(QColor color);
	void setPenStyle(Qt::PenStyle style);

	QColor getBorderColor() const;
	Qt::PenStyle getPenStyle() const;

	void setFillColor(QColor color);
	QColor getFillColor() const;

	int getWidth() const;
	void setWidth(const int w);

	int getHeight() const;
	void setHeight(const int h);

	void drawOutLine(QPaintDevice* parent = Q_NULLPTR);
	QRect getShapeRect() const;
	virtual bool isValid();

	KShapeParams getCurShapeParams();
	void setCurShapeParams(KShapeParams shape_params);

	KShapeParams getPrevShapeParams();
	void setPrevShapeParams();
protected:
	KShapeParams m_prevShapeParams; // 存储前置状态，撤回使用
	KShapeParams m_curShapeParams; // 存储当前状态
};

#endif
