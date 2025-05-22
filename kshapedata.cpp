#include "kshapedata.h"
#include "kobserver.h"

void KShapeData::registerObserver(std::shared_ptr<KObserver> observer)
{
	m_observers.push_back(observer);
}

void KShapeData::removeObserver(std::shared_ptr<KObserver> observer)
{
	auto it = std::find(m_observers.begin(), m_observers.end(), observer);
	if (it != m_observers.end())
	{
		m_observers.erase(it);
	}
}

void KShapeData::notifyObservers()
{
	for (const auto &observer : m_observers)
	{
		observer->update(m_shapeParams);
	}
}

void KShapeData::setShapeData(KShapeParams shapeParams)
{
	m_shapeParams = shapeParams;
	notifyObservers();
}
