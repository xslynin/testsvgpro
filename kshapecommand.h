#ifndef ___K_SHAPE_COMMAND_H__
#define ___K_SHAPE_COMMAND_H__

#include <QUndoCommand>
#include "kshape.h"

// -------------------------------------------------------
// kshapecommand.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/30
// 功能描述： 实现图形参数变化之后的redo，undo。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KShapeCommand : public QUndoCommand
{
public:
	KShapeCommand(KShapeParams shapeParms, KShape* shape = Q_NULLPTR, QUndoCommand* parent = Q_NULLPTR);
	~KShapeCommand();

	virtual void undo() override;
	virtual void redo() override;

private:
	KShape* m_shape;
	KShapeParams m_prevShapeParams;
	KShapeParams m_curShapeParams;
};

#endif  //___K_SHAPE_COMMAND_H__