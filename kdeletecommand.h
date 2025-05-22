#ifndef ___K_DELETE_COMMAND_H__
#define ___K_DELETE_COMMAND_H__

#include <QUndoCommand>

// -------------------------------------------------------
// kdeletecommand.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/31
// 功能描述： 删除的撤退、前进命令。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KShape;

class KDeleteCommand : public QUndoCommand
{
public:
	KDeleteCommand(QList<KShape*>* isSelectedShapelist, QList<KShape*>* shapelist, QList<KShape*>* deletedShapeList, QUndoCommand* parent = Q_NULLPTR);
	~KDeleteCommand();

	virtual void undo() override;
	virtual void redo() override;

private:
	QList<KShape*>* m_isSelectedShapelist;
	QList<KShape*>* m_shapeList;
	QList<KShape*>* m_deletedShapeList;
	QList<KShape*> m_record;
};

#endif  //___KDELETECOMMAND_H__


