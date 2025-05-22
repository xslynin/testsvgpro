#ifndef __K_SHAPEFACTORY_H_
#define __K_SHAPEFACTORY_H_

#include <QObject>

#include "kglobaldata.h"
#include "krect.h"

class KShapeFactory : public QObject
{
	Q_OBJECT

public:
	KShapeFactory(QObject *parent);
	~KShapeFactory();
	static KShape* createShape(KGlobalData::KDrawFlag drawFlag);
};

#endif
