#include "kmenubar.h"
#include <QDebug>

KMenuBar::KMenuBar(QWidget* parent)
	: QMenuBar(parent)
{
    // 设置字体大小
    QFont menuFont;
    menuFont.setPointSize(14);  
    setFont(menuFont);
    setStyleSheet("QMenuBar { color: #F5F5DC; }"
        "QMenuBar:selected { color: black; }"
        "QMenu { background-color: white; color: black; selection-background-color: gray; selection-color: black; }"
        "QMenu::item:selected { background-color: gray; }" // 选中项（可能是通过键盘或程序选中的）  
    );  

    m_fileMenu = addMenu(QString::fromLocal8Bit("文件"));
    initFileMenu();
}

void KMenuBar::initFileMenu()
{
    // 添加动作到文件菜单
    m_creatFileAct = new QAction(QString::fromLocal8Bit("新建文件"), this);
    m_creatFileAct->setShortcuts(QKeySequence::New);
    m_creatFileAct->setStatusTip(QString::fromLocal8Bit("创建新的画布"));
    m_fileMenu->addAction(m_creatFileAct);

    m_openSVGAct = new QAction(QString::fromLocal8Bit("打开SVG"), this);
    m_openSVGAct->setShortcuts(QKeySequence::Open);
    m_openSVGAct->setStatusTip(tr("打开SVG"));
    m_fileMenu->addAction(m_openSVGAct);

    m_saveAct = new QAction(QString::fromLocal8Bit("保存"), this);
    m_saveAct->setShortcuts(QKeySequence::Save);
    m_saveAct->setStatusTip(QString::fromLocal8Bit("保存当前文件"));
    m_fileMenu->addAction(m_saveAct);

    m_savePNGAct = new QAction(QString::fromLocal8Bit("导出PNG"), this);
    m_savePNGAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));  // 设置快捷键为 Ctrl + E
    m_savePNGAct->setStatusTip(QString::fromLocal8Bit("当前图形导出"));
    m_fileMenu->addAction(m_savePNGAct);
}
