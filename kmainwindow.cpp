#include "kmainwindow.h"
#include <QLineEdit>
#include <QComboBox>

#include "kglobaldata.h" // 使用 KGlobalData 来管理配置
#include "ktoolbar.h"
#include "ksvgmainwindow.h"
#include "kcanvasparamsbar.h"
#include "kshapeparamsbar.h"
#include "kmenubar.h"
#include "kshapecommand.h"
#include "kcanvascommand.h"
#include "kcurrentshapedatadisplay.h"

KMainWindow::KMainWindow(QWidget *parent)
    : QWidget(parent)
    ,m_pVLayout(Q_NULLPTR)
    ,m_pHLayout(Q_NULLPTR)
    ,m_pSvgMainWin(Q_NULLPTR)
    ,m_pToolBar(Q_NULLPTR)
{
    ui.setupUi(this);
    setWindowIcon(QIcon(":/icons/svg.png"));

    // KGlobalData 在其构造函数中已经从注册表加载了配置，这里不需要重复加载

    // 从 KGlobalData 获取窗口配置
    m_config.loadConfig(); // 仍然需要加载窗口位置和大小的配置

    // Set window geometry from loaded config
    if (m_config.windowWidth > 0 && m_config.windowHeight > 0) {
        setGeometry(m_config.windowX, m_config.windowY, m_config.windowWidth, m_config.windowHeight);
    } else {
        // Set default window size if no config is loaded
        resize(800, 600);
    }


    initWindows();
    initConnection();
}

KMainWindow::~KMainWindow()
{
    // Save configuration on close
    m_config.windowX = x();
    m_config.windowY = y();
    m_config.windowWidth = width();
    m_config.windowHeight = height();
    m_config.saveConfig();
    
    // 确保 KGlobalData 的配置也保存到注册表
    KGlobalData::getGlobalDataIntance()->saveConfigToRegistry();
}


void KMainWindow::initWindows()
{
    m_pVLayout = new QVBoxLayout(this);
    m_pVLayout->setSpacing(0);
    m_pVLayout->setMargin(0);

    m_pHLayout = new QHBoxLayout(this);
    m_pHLayout->setSpacing(0);

    m_pParamsBarVLayout = new QVBoxLayout(this);
    m_pParamsBarVLayout->setSpacing(0);

    m_pCenterVLayout = new QVBoxLayout(this);
    m_pCenterVLayout->setSpacing(0);

    m_pToolBar = new KToolBar(this);
    m_pSvgMainWin = new KSvgMainWindow(this);
    m_pCanvasParamsBar = new KCanvasParamsBar(QStringLiteral("画布参数"),this);
    m_pShapeParamsBar= new KShapeParamsBar(QStringLiteral("图形参数"), this);
    m_pMenuBar = new KMenuBar(this);

    // Set initial canvas parameters from loaded config
    KGlobalData::getGlobalDataIntance()->setCanvasWidth(m_config.canvasWidth);
    KGlobalData::getGlobalDataIntance()->setCanvasHeight(m_config.canvasHeight);
    KGlobalData::getGlobalDataIntance()->setCanvasColor(QString::fromStdString(m_config.backgroundColor));


    // 布局
    m_pCenterVLayout->addWidget(m_pMenuBar);
    m_pCenterVLayout->addWidget(m_pSvgMainWin);

    m_pParamsBarVLayout->addWidget(m_pCanvasParamsBar);
    m_pParamsBarVLayout->addWidget(m_pShapeParamsBar);
    m_pParamsBarVLayout->setStretchFactor(m_pCanvasParamsBar, 1);
    m_pParamsBarVLayout->setStretchFactor(m_pShapeParamsBar, 1);
    m_pParamsBarVLayout->addStretch(8);

    m_pHLayout->addWidget(m_pToolBar);
    m_pHLayout->addLayout(m_pCenterVLayout);
    m_pHLayout->addLayout(m_pParamsBarVLayout);

    m_pVLayout->addLayout(m_pHLayout);
    setLayout(m_pVLayout);
 }

void KMainWindow::initConnection()
{
    connect(m_pSvgMainWin, &KSvgMainWindow::scaleChanged, this, [this](qreal value)
        {
            m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->setText(QString::number(value * 100));
        });
    (void)connect(m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pCanvasParamsBar->m_pColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateCanvasParams);
    (void)connect(m_pShapeParamsBar->m_pBorderWidthBox->m_pParamEdit, &QLineEdit::editingFinished, this, &KMainWindow::validateShapeParams);
    (void)connect(m_pShapeParamsBar->m_pBorderStyleBox->m_pBorderStyleComboBox, qOverload<int>(&QComboBox::currentIndexChanged), this, &KMainWindow::switchPenStyle);
    (void)connect(m_pShapeParamsBar->m_pBorderColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateShapeParams);
    (void)connect(m_pShapeParamsBar->m_pFillColorBox, &KColorBox::pickedColor, this, &KMainWindow::validateShapeParams);

    (void)connect(m_pSvgMainWin->m_pCanvas, &KCanvas::shapeParamChanged, this, &KMainWindow::updateSelectedShapeParamsToParamsBar);
    (void)connect(m_pSvgMainWin->m_pCanvas, &KCanvas::canvasParamsChanged, this, &KMainWindow::updateCanvasParamsToParamsBar);

    (void)connect(m_pMenuBar->m_creatFileAct, &QAction::triggered, m_pSvgMainWin->m_pCanvas, &KCanvas::createCanvas);
    (void)connect(m_pMenuBar->m_openSVGAct, &QAction::triggered, m_pSvgMainWin->m_pCanvas, &KCanvas::loadSvg);
    (void)connect(m_pMenuBar->m_saveAct, &QAction::triggered, m_pSvgMainWin->m_pCanvas, &KCanvas::saveToSvg);
    (void)connect(m_pMenuBar->m_savePNGAct, &QAction::triggered, m_pSvgMainWin->m_pCanvas, &KCanvas::saveToPng);
}

void KMainWindow::validateCanvasParams()
{
	qint32 width = m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->text().toInt();
    qint32 height = m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit->text().toInt();
    qreal scale = m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->text().toDouble() / 100;
    QString canvasC = KGlobalData::getGlobalDataIntance()->getCanvasColor();

    KGlobalData::getGlobalDataIntance()->setCanvasWidth(width);
    KGlobalData::getGlobalDataIntance()->setCanvasHeight(height);
    KGlobalData::getGlobalDataIntance()->setCanvaScale(scale);

    m_pSvgMainWin->m_pCanvas->resize(width, height);
    m_pSvgMainWin->m_pCanvas->resize(width * scale, height * scale);
    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:#%1").arg(canvasC)); // Use updated canvasC

    int preW = KGlobalData::getGlobalDataIntance()->getPrevCanvasWidth();
    int preH = KGlobalData::getGlobalDataIntance()->getPrevCanvasHeight();
    QString preC = KGlobalData::getGlobalDataIntance()->getPrevCanvasColor();
    if (preW == width && preH == height && preC == canvasC)
        return;
    // 画布操作入栈
    KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KCanvasCommand(KGlobalData::getGlobalDataIntance()->getPrevCanvasWidth(),
        KGlobalData::getGlobalDataIntance()->getPrevCanvasHeight(), KGlobalData::getGlobalDataIntance()->getPrevCanvasColor(),
        width, height, KGlobalData::getGlobalDataIntance()->getCanvasColor()));

    // 不再需要手动更新 m_config，KGlobalData 会在程序关闭时保存配置到注册表
}

void KMainWindow::validateShapeParams()
{
    Qt::PenStyle penStyle = KGlobalData::getGlobalDataIntance()->getPenStyle();
    qint32 borderWidth = m_pShapeParamsBar->m_pBorderWidthBox->m_pParamEdit->text().toInt();
    QColor borderColor = KGlobalData::getGlobalDataIntance()->getBoderColor();
    QColor fillColor = KGlobalData::getGlobalDataIntance()->getFillColor();
    KGlobalData::getGlobalDataIntance()->setBorderWidth(borderWidth);
    KGlobalData::getGlobalDataIntance()->setBoderColor(borderColor);
    KGlobalData::getGlobalDataIntance()->setFillColor(fillColor);
    KGlobalData::getGlobalDataIntance()->setPenStyle(penStyle);

    // 选中当前形状也可改变样式
    if (m_pSvgMainWin->m_pCanvas->m_pCurrentShape != Q_NULLPTR)
    {
        m_pSvgMainWin->m_pCanvas->m_pCurrentShape->setBorderColor(borderColor);
        m_pSvgMainWin->m_pCanvas->m_pCurrentShape->setBorderWidth(borderWidth);
        m_pSvgMainWin->m_pCanvas->m_pCurrentShape->setFillColor(fillColor);
        m_pSvgMainWin->m_pCanvas->m_pCurrentShape->setPenStyle(penStyle);
        m_pSvgMainWin->m_pCanvas->update();
        KShapeParams curParams = m_pSvgMainWin->m_pCanvas->m_pCurrentShape->getCurShapeParams();
        KShapeParams prevParams = m_pSvgMainWin->m_pCanvas->m_pCurrentShape->getPrevShapeParams();
        if (curParams == prevParams)
            return;
        KGlobalData::getGlobalDataIntance()->getUndoStack()->push(new KShapeCommand(curParams, m_pSvgMainWin->m_pCanvas->m_pCurrentShape));
        
    }
}

void KMainWindow::updateSelectedShapeParamsToParamsBar(KShape* shape)
{
    KShapeParams curShapeParams = shape->getCurShapeParams();
    m_pShapeParamsBar->m_shapeData.setShapeData(curShapeParams); // 观察者，更新UI

    KGlobalData::getGlobalDataIntance()->setBoderColor(curShapeParams.borderColor);
    KGlobalData::getGlobalDataIntance()->setFillColor(curShapeParams.fillColor);
    KGlobalData::getGlobalDataIntance()->setBorderWidth(curShapeParams.borderWidth);
    KGlobalData::getGlobalDataIntance()->setPenStyle(curShapeParams.penStyle);
}

void KMainWindow::updateCanvasParamsToParamsBar()
{
    qint32 width = KGlobalData::getGlobalDataIntance()->getCanvasWidth();
    qint32 height = KGlobalData::getGlobalDataIntance()->getCanvasHeight();
    QString str = KGlobalData::getGlobalDataIntance()->getCanvasColor();
    qreal scale = KGlobalData::getGlobalDataIntance()->getCanvasScale();
    m_pSvgMainWin->m_pCanvas->resize(width * scale, height * scale);
    m_pCanvasParamsBar->m_pWidthBox->m_pParamEdit->setText(QString::number(width));
    m_pCanvasParamsBar->m_pHeightBox->m_pParamEdit->setText(QString::number(height));
    m_pCanvasParamsBar->m_pColorBox->m_pColorBtn->setStyleSheet(
        QString("border-width: 0px; background-color:#%1; border-style:outset;margin:5px").arg(str));

    m_pSvgMainWin->m_pCanvas->setStyleSheet(QString("background-color:#%1").arg(str));

    // 不再需要手动更新 m_config，KGlobalData 会在程序关闭时保存配置到注册表
}

void KMainWindow::switchPenStyle(const int index)
{
    switch (index)
    {
    case 0:
        KGlobalData::getGlobalDataIntance()->setPenStyle(Qt::SolidLine);
        break;
    case 1:
        KGlobalData::getGlobalDataIntance()->setPenStyle(Qt::DotLine);
        break;
    case 2:
        KGlobalData::getGlobalDataIntance()->setPenStyle(Qt::DashLine);
        break;
    case 3:
        KGlobalData::getGlobalDataIntance()->setPenStyle(Qt::DashDotLine);
        break;
    case 4:
        KGlobalData::getGlobalDataIntance()->setPenStyle(Qt::DashDotDotLine);
        break;
    }
    validateShapeParams();
}
