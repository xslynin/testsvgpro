#ifndef __K_VALUE_BOX_H_
#define __K_VALUE_BOX_H_

#include <QLineEdit>

#include "kparambox.h"

class KValueBox : public KParamBox
{
	Q_OBJECT

	friend class KMainWindow;
	friend class KShapeParamsBar;
public:
	KValueBox(const QString &title,const qreal value,QWidget *parent);
	~KValueBox();
	virtual void focusInEvent(QFocusEvent* event) override;
private:
	QLineEdit *m_pParamEdit;
};

#endif
