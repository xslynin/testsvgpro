#ifndef __K_MAINWINDOW_H_
#define __K_MAINWINDOW_H_

#include <QtWidgets/QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "kshapedata.h"
#include "kconfig.h" // Include KConfig header
#include "ui_kmainwindow.h"

class KShape;
class KToolBar;
class KSvgMainWindow;
class KMenuBar;
class KCanvasParamsBar;
class KShapeParamsBar;

class KMainWindow : public QWidget
{
    Q_OBJECT

public:
    KMainWindow(QWidget *parent = Q_NULLPTR);
    ~KMainWindow(); // Add destructor

    void initWindows();
    void initConnection();

private slots:
    void validateCanvasParams();
    void validateShapeParams();
    void updateSelectedShapeParamsToParamsBar(KShape* shape);
    void updateCanvasParamsToParamsBar();

private:
    void switchPenStyle(int index);
    Ui::KMainWindowClass ui;
    QVBoxLayout *m_pVLayout;
    QHBoxLayout *m_pHLayout;
    QVBoxLayout* m_pParamsBarVLayout;
    QVBoxLayout* m_pCenterVLayout;

    KToolBar *m_pToolBar;
    KSvgMainWindow *m_pSvgMainWin;
    KCanvasParamsBar *m_pCanvasParamsBar;
    KShapeParamsBar* m_pShapeParamsBar;
    KMenuBar* m_pMenuBar;

    KConfig m_config; // Add KConfig member
};

#endif
