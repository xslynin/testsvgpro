#ifndef __K_GLOBAL_DATA__
#define __K_GLOBAL_DATA__

#include "kshape.h"

#include <QObject>
#include <QColor>
#include <QUndoStack>
#include <QMap> // For storing tooltip data
#include <QPair> // For storing title and text
#include <QString> // For string data
#include <QNetworkAccessManager> // For network requests
#include <QNetworkReply> // For network replies
#include <QJsonDocument> // For JSON parsing
#include <QJsonObject> // For JSON parsing
#include <QJsonValue> // For JSON parsing


class KGlobalData : public QObject
{
	Q_OBJECT

public:
	enum class KDrawFlag
	{
		NoneDrawFlag = 0,
		MouseDrawFlag,	
		PenDrawFlag,
		LineDrawFlag,
		RectDrawFlag,
		CircleDrawFlag,
		PentDrawFlag,
		HexaDrawFlag,
		StarDrawFlag,
		TextDrawFlag
	};

	enum class KColorFlag //颜色标志的枚举，用于判断设置的颜色是属于画布还是图形参数，主要用于colorbox判断
	{
		NoneColorFlag = 0,
		CanvasFlag,
		ShapeBorderFlag,
		ShapeFillFlag,
	};

	KGlobalData(QObject *parent = Q_NULLPTR);
	~KGlobalData();

	static KGlobalData *getGlobalDataIntance();

	void setDrawFlag(KGlobalData::KDrawFlag drawflag);
	KDrawFlag getDrawFlag();

	void setColorFlag(KGlobalData::KColorFlag colorflag);
	KColorFlag getColorFlag();

	// 画布参数
	void setPrevCanvasWidth(const int width);
	void setPrevCanvasHeight(const int height);
	void setPrevCanvasColor(const QString& colorStr);

	int getPrevCanvasWidth() const;
	int getPrevCanvasHeight() const;
	QString getPrevCanvasColor();

	void setCanvasWidth(const int width);
	void setCanvasHeight(const int height);

	int getCanvasWidth() const;
	int getCanvasHeight() const;

	void setCanvasColor(const QString & colorStr);
	QString getCanvasColor();

	void setCanvaScale(qreal scale);
	qreal getCanvasScale() const;

	// 图形参数
	void setBorderWidth(const int value);
	int getBorderWidth() const;

	void setBoderColor(const QColor color);
	QColor getBoderColor();
	void setFillColor(const QColor color);
	QColor getFillColor();

	Qt::PenStyle getPenStyle() const;
	void setPenStyle(const Qt::PenStyle pen_style);

	QUndoStack* getUndoStack();

    // Tooltip data
    void fetchButtonTips();
    QPair<QString, QString> getButtonTip(const QString& buttonName) const;

signals:
    void buttonTipsFetched(); // Signal to indicate tips are ready

private slots:
    void onButtonTipsReplyFinished(QNetworkReply* reply);

private:
	KGlobalData(const KGlobalData &other) = delete;
	KGlobalData(const KGlobalData &&other) = delete;
	void operator=(const KGlobalData) = delete;

	KDrawFlag m_drawFlag;
	KColorFlag m_colorFlag;

	// 画布的参数，待优化，可以设计成结构体存储。
	int m_canvasWidth;
	int m_canvasHeight;
	QString m_canvasColor;
	int m_prevCanvasWidth;
	int m_prevCanvasHeight;
	QString m_prevCanvasColor;
	qreal m_scale;

	// 参数栏的全局变量，当不选中图形的时候，可以调整，下一次绘制使用此参数绘制
	int m_borderWidth;
	QColor m_boderColor;
	QColor m_fillColor;
	Qt::PenStyle m_penStyle;

	// 全局的栈，用来存储前进从、撤退命令
	QUndoStack m_undoStack;//撤退、前进操作的栈

    QNetworkAccessManager* m_networkManager; // Network manager for fetching tips
    QMap<QString, QPair<QString, QString>> m_buttonTips; // Map to store button tips
};
#endif
