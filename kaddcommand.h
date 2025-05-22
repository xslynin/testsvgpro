#ifndef ___K_ADD_COMMAND_H__
#define ___K_ADD_COMMAND_H__

#include <QUndoCommand>

// -------------------------------------------------------
// kaddcommand.h
// 创建者： 肖朝洋
// 创建时间： 2024/8/1
// 功能描述： 添加图形的撤退、前进命令。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KShape;

class KAddCommand : public QUndoCommand
{
public:
	KAddCommand(KShape* createdShape, QList<KShape*>* shapelist, QList<KShape*>* deletedShapeList, QUndoCommand* parent = Q_NULLPTR);
	~KAddCommand();

	virtual void undo() override;
	virtual void redo() override;

private:
	KShape* m_curCreatedShape;
	QList<KShape*>* m_shapeList;
	QList<KShape*>* m_deletedShapeList;
	KShape* m_record;
};
#endif  //___K_ADD_COMMAND_H__


