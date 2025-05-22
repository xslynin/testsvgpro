#ifndef __K_PARAMBOX_H_
#define __K_PARAMBOX_H_

#include <QPushButton>
#include <QColorDialog>

#include "kparambox.h"

class KColorBox : public KParamBox
{
	Q_OBJECT

	friend class KMainWindow;
	friend class KShapeParamsBar;
public:
	KColorBox(const QString &title,const QString &rgbColorStr, KGlobalData::KColorFlag flag,
		QWidget *parent);
	~KColorBox();

	// ����ͷ��¼�,����ͷ�ʱ������ɫѡ��Ի���
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
signals:
	void pickedColor();
private slots:
	void showPickColorDlg();
private:
	QPushButton* m_pColorBtn;
	KGlobalData::KColorFlag m_colorFlag;
};
#endif