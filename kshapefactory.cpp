#include "kshapefactory.h"
#include "kline.h"
#include "kcircle.h"
#include "khexa.h"
#include "kpen.h"
#include "kpent.h"
#include "kstar.h"
#include "ktext.h"


KShapeFactory::KShapeFactory(QObject *parent)
	: QObject(parent)
{
}

KShapeFactory::~KShapeFactory()
{
}

KShape* KShapeFactory::createShape(KGlobalData::KDrawFlag drawFlag)
{
	switch(drawFlag)
	{
	case KGlobalData::KDrawFlag::RectDrawFlag:
		return new KRect;
	case KGlobalData::KDrawFlag::LineDrawFlag:
		return new KLine;
	case KGlobalData::KDrawFlag::CircleDrawFlag:
		return new KCircle;
	case KGlobalData::KDrawFlag::PenDrawFlag:
		return new KPen;
	case KGlobalData::KDrawFlag::PentDrawFlag:
		return new KPent;
	case KGlobalData::KDrawFlag::HexaDrawFlag:
		return new KHexa;
	case KGlobalData::KDrawFlag::StarDrawFlag:
		return new KStar;
	case KGlobalData::KDrawFlag::TextDrawFlag:
		return new KText;
	default:
		break;
	}

}
