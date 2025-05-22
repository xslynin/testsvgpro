#ifndef ___K_CANVAS_MENU_H__
#define ___K_CANVAS_MENU_H__

// -------------------------------------------------------
// kcanvasmenu.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/29
// 功能描述： 画布的右键菜单栏。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QPoint>

class KCanvasMenu : public QObject
{
    Q_OBJECT

	friend class KSvgMainWindow;
    friend class KMainWindow;
public:
    explicit KCanvasMenu(QWidget* parent = nullptr);
    ~KCanvasMenu();
    void showMenu(const QPoint& pos);

    bool getIsSelectedShape();
    void setIsSelectedShape(bool ok);

private:
    void initializeMenu();
    void updateActions();
    bool m_isSelectedShape;

    QWidget* m_parentWidget;

    QMenu* m_menu;
    QAction* m_selectAllAct;
    QAction* m_undoAct;
    QAction* m_redoAct;
    QAction* m_cutAct;
    QAction* m_copyAct;
    QAction* m_pasteAct;
    QAction* m_copyAndPasteAct;
    QAction* m_deleteAct;
    QAction* m_upLayerAct;
    QAction* m_downLayerAct;
    QAction* m_upFrontLayerAct;
    QAction* m_downBackLayerAct;
};

#endif  //___K_CANVAS_MENU_H__


