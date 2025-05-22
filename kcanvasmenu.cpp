#include "kcanvasmenu.h"
#include "kglobaldata.h"

KCanvasMenu::KCanvasMenu(QWidget* parent)
    : QObject(parent)
	, m_parentWidget(parent)
	, m_isSelectedShape(false)
{
	m_parentWidget->setContextMenuPolicy(Qt::CustomContextMenu); // 自定义上下文菜单
    m_parentWidget->setFocusPolicy(Qt::StrongFocus); // 强焦点，即该部件可以通过键盘操作获得焦点。
	connect(m_parentWidget, &QWidget::customContextMenuRequested, this, &KCanvasMenu::showMenu);
    initializeMenu();
}

KCanvasMenu::~KCanvasMenu()
{
}

void KCanvasMenu::initializeMenu()
{
    m_menu = new QMenu(m_parentWidget);
    m_menu->setStyleSheet("QMenu { background-color: #3f3f3c;color: white; }"
        "QMenu::item:selected { background-color: #555555; color: white; }"
        "QMenu::item:disabled { color: gray; }");

    // 在初始化代码或者构造函数中进行实例化和设置
    m_selectAllAct = new QAction(QString::fromLocal8Bit("全选"), this);
    m_selectAllAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_A));

    m_undoAct = new QAction(QString::fromLocal8Bit("撤销(undo)"), this);
    m_undoAct->setShortcut(QKeySequence::Undo);

    m_redoAct = new QAction(QString::fromLocal8Bit("重做(redo)"), this);
    m_redoAct->setShortcut(QKeySequence::Redo);

    m_cutAct = new QAction(QString::fromLocal8Bit("剪切"), this);
    m_cutAct->setShortcut(QKeySequence::Cut);

    m_copyAct = new QAction(QString::fromLocal8Bit("复制"), this);
    m_copyAct->setShortcut(QKeySequence::Copy);

    m_pasteAct = new QAction(QString::fromLocal8Bit("粘贴"), this);
    m_pasteAct->setShortcut(QKeySequence::Paste);

    m_copyAndPasteAct = new QAction(QString::fromLocal8Bit("复制并粘贴"), this);
    m_copyAndPasteAct->setShortcut(QKeySequence::fromString("Ctrl+Shift+D")); // 自定义快捷键示例

    m_deleteAct = new QAction(QString::fromLocal8Bit("删除"), this);
    m_deleteAct->setShortcut(QKeySequence::Delete);

    m_upLayerAct = new QAction(QString::fromLocal8Bit("置上一层"), this);
    m_upLayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Up));

    m_downLayerAct = new QAction(QString::fromLocal8Bit("置下一层"), this);
    m_downLayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Down));

    m_upFrontLayerAct = new QAction(QString::fromLocal8Bit("置于顶层"), this);
    m_upFrontLayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Up));

    m_downBackLayerAct = new QAction(QString::fromLocal8Bit("置于底层"), this);
    m_downBackLayerAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_Down));

    m_menu->addAction(m_selectAllAct);
    m_menu->addAction(m_undoAct);
    m_menu->addAction(m_redoAct);
    m_menu->addAction(m_cutAct);
    m_menu->addAction(m_copyAct);
    m_menu->addAction(m_pasteAct);
    m_menu->addAction(m_copyAndPasteAct);
    m_menu->addAction(m_deleteAct);
    m_menu->addSeparator(); // 可以添加分隔线来分组不同的操作
    m_menu->addAction(m_upLayerAct);
    m_menu->addAction(m_downLayerAct);
    m_menu->addAction(m_upFrontLayerAct);
    m_menu->addAction(m_downBackLayerAct);
}

void KCanvasMenu::updateActions()
{
    if (KGlobalData::getGlobalDataIntance()->getUndoStack()->count() == 0)
    {
        m_undoAct->setEnabled(false);
        m_redoAct->setEnabled(false);
    }
    else
    {
        m_undoAct->setEnabled(true);
        m_redoAct->setEnabled(true);
    }
	if (m_isSelectedShape == true)
	{
        m_selectAllAct->setEnabled(true);
        m_cutAct->setEnabled(true);
        m_copyAct->setEnabled(true);
        m_pasteAct->setEnabled(true);
        m_copyAndPasteAct->setEnabled(true);
        m_deleteAct->setEnabled(true);
        m_upLayerAct->setEnabled(true);
        m_downLayerAct->setEnabled(true);
        m_upFrontLayerAct->setEnabled(true);
        m_downBackLayerAct->setEnabled(true);
	}
	else
	{
        m_selectAllAct->setEnabled(true);
        m_cutAct->setEnabled(false);
        m_copyAct->setEnabled(false);
        m_pasteAct->setEnabled(true);
        m_copyAndPasteAct->setEnabled(false);
        m_deleteAct->setEnabled(false);
        m_upLayerAct->setEnabled(false);
        m_downLayerAct->setEnabled(false);
        m_upFrontLayerAct->setEnabled(false);
        m_downBackLayerAct->setEnabled(false);
	}
}

void KCanvasMenu::showMenu(const QPoint& pos)
{
    updateActions();
    m_menu->exec(m_parentWidget->mapToGlobal(pos));
    m_isSelectedShape = false;
}

bool KCanvasMenu::getIsSelectedShape()
{
    return m_isSelectedShape;
}

void KCanvasMenu::setIsSelectedShape(bool ok)
{
    m_isSelectedShape = ok;
}