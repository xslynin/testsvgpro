#ifndef ___K_MENU_BAR_H__
#define ___K_MENU_BAR_H__

#include <QMenuBar>

// -------------------------------------------------------
// kmenubar.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/28
// 功能描述： 文件菜单栏。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

class KMenuBar : public QMenuBar
{
    Q_OBJECT

	friend class KMainWindow;
public:
    KMenuBar(QWidget* parent = Q_NULLPTR);

private:
    void initFileMenu();
    QMenu* m_fileMenu;

    QAction* m_creatFileAct;
    QAction* m_openSVGAct;
    QAction* m_saveAct;
    QAction* m_savePNGAct;
};

#endif  //___K_MENU_BAR_H__