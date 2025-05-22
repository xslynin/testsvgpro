#include "kaddcommand.h"

KAddCommand::KAddCommand(KShape* createdShape, QList<KShape*>* shapelist, QList<KShape*>* deletedShapeList, QUndoCommand* parent)
	: QUndoCommand(parent)
	, m_curCreatedShape(createdShape)
	, m_shapeList(shapelist)
	, m_deletedShapeList(deletedShapeList)
{
	m_record = m_curCreatedShape;
}

KAddCommand::~KAddCommand()
{
}

void KAddCommand::undo()
{
	m_shapeList->removeOne(m_record);
	if (!m_deletedShapeList->contains(m_record))
	{
		m_deletedShapeList->append(m_record);
	}
}

void KAddCommand::redo()
{
	if (!m_shapeList->contains(m_record))
	{
		m_shapeList->prepend(m_record);
	}
	m_deletedShapeList->removeOne(m_record);
}
