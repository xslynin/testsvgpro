#include "kcanvascommand.h"
#include "kglobaldata.h"

KCanvasCommand::KCanvasCommand(int preW, int preH, QString preC, int curW, int curH, QString curC, QUndoCommand* parent)
	: QUndoCommand(parent)
	, m_preWidth(preW)
	, m_preHeight(preH)
	, m_preColor(preC)
	, m_curWidth(curW)
	, m_curHeight(curH)
	, m_curColor(curC)
{
	KGlobalData::getGlobalDataIntance()->setPrevCanvasWidth(m_curWidth);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasHeight(m_curHeight);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasColor(m_curColor);
}

KCanvasCommand::~KCanvasCommand()
{
}

void KCanvasCommand::undo()
{
	KGlobalData::getGlobalDataIntance()->setCanvasWidth(m_preWidth);
	KGlobalData::getGlobalDataIntance()->setCanvasHeight(m_preHeight);
	KGlobalData::getGlobalDataIntance()->setCanvasColor(m_preColor);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasWidth(m_preWidth);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasHeight(m_preHeight);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasColor(m_preColor);
}

void KCanvasCommand::redo()
{
	KGlobalData::getGlobalDataIntance()->setCanvasWidth(m_curWidth);
	KGlobalData::getGlobalDataIntance()->setCanvasHeight(m_curHeight);
	KGlobalData::getGlobalDataIntance()->setCanvasColor(m_curColor);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasWidth(m_curWidth);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasHeight(m_curHeight);
	KGlobalData::getGlobalDataIntance()->setPrevCanvasColor(m_curColor);
}
