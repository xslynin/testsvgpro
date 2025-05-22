#include "kdeletecommand.h"

KDeleteCommand::KDeleteCommand(QList<KShape*>* isSelectedShapelist, QList<KShape*>* shapelist, QList<KShape*>* deletedShapeList, QUndoCommand* parent)
	: QUndoCommand(parent)
	, m_isSelectedShapelist(isSelectedShapelist)
	, m_shapeList(shapelist)
	, m_deletedShapeList(deletedShapeList)
{
	for (KShape* shape : *isSelectedShapelist)
	{
		m_record.append(shape);
	}
}

KDeleteCommand::~KDeleteCommand()
{
}

void KDeleteCommand::undo()
{
	
	for (auto shape : m_record)
	{
		if (!m_shapeList->contains(shape))
		{
			m_shapeList->prepend(shape);
		}
		m_deletedShapeList->removeOne(shape);
	}
}

void KDeleteCommand::redo()
{
	for (auto shape : m_record)
	{
		m_shapeList->removeOne(shape);
		if (!m_deletedShapeList->contains(shape))
		{
			m_deletedShapeList->append(shape);
		}
	}
}
