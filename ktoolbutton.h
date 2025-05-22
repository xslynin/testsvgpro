#ifndef __KTOOLBUTTON_H_
#define __KTOOLBUTTON_H_

#include <QRadioButton>
#include <QtSvg/QSvgRenderer>
#include <QPainter>
#include <QToolTip> // Include QToolTip for displaying tooltips
#include <QTimer> // Include QTimer for delayed tooltip display

#include "kglobaldata.h"

class KToolButton : public QRadioButton
{
	Q_OBJECT
public:
	KToolButton(KGlobalData::KDrawFlag drawflag,QWidget *parent);
	~KToolButton();

	void initToolButton();

	virtual void paintEvent(QPaintEvent* event) override;// 绘图事件
	virtual void enterEvent(QEvent* event) override; // 进入组件区域
	virtual void leaveEvent(QEvent* event) override; // 离开组件区域
	virtual void mouseReleaseEvent(QMouseEvent* e) override;// 鼠标释放事件


private slots:
	void onToggledButton(bool checked);
    void updateTooltip(); // Slot to update tooltip text
    void showTooltip(); // Slot to show tooltip after a delay

private:
	QString m_iconName;//svg 图标名称
	QSvgRenderer* m_pSvgRenderer;//处理 svg 图标/图片的对象指针
	KGlobalData::KDrawFlag m_drawFlag;//记录绘图类型
    QString m_buttonIdentifier; // Identifier string for fetching tooltip
    QTimer m_tooltipTimer; // Timer for delayed tooltip display
};

#endif
