#ifndef ___K_CANVAS_COMMAND_H__
#define ___K_CANVAS_COMMAND_H__

#include <QUndoCommand>

// -------------------------------------------------------
// kcanvascommand.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/30
// 功能描述： 画布尺寸、颜色的redo、undo命令。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KCanvasCommand : public QUndoCommand
{
public:
	KCanvasCommand(int preW, int preH, QString preC, int curW, int curH, QString curC, QUndoCommand* parent = Q_NULLPTR);
	~KCanvasCommand();

	virtual void undo() override;
	virtual void redo() override;
private:
	int m_preWidth;
	int m_preHeight;
	int m_curWidth;
	int m_curHeight;

	QString m_preColor;
	QString m_curColor;
};
#endif  //___KCANVASCOMMAND_H__


