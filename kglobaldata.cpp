#include "kglobaldata.h"
#include <Windows.h>
#include <QUrl> // For network request URL
#include <QDebug> // For debugging output


KGlobalData::KGlobalData(QObject *parent)
	: QObject(parent)
	, m_drawFlag(KDrawFlag::NoneDrawFlag)
	, m_colorFlag(KColorFlag::NoneColorFlag)
    , m_canvasColor("FFFFFF")
    , m_canvasWidth(800)
    , m_canvasHeight(600)
	, m_prevCanvasColor("FFFFFF")
    , m_prevCanvasWidth(800)
    , m_prevCanvasHeight(600)
    , m_scale(1.0)
	, m_borderWidth(2)
	, m_boderColor(Qt::gray)
	, m_fillColor(Qt::white)
	, m_penStyle(Qt::SolidLine)
    , m_networkManager(new QNetworkAccessManager(this)) // Initialize network manager
{
    //m_undoStack.setUndoLimit(20);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &KGlobalData::onButtonTipsReplyFinished);
    
    // 在初始化时从注册表加载配置
    loadConfigFromRegistry();
}

KGlobalData *KGlobalData::getGlobalDataIntance()
{
    static KGlobalData s_instance;
    static KGlobalData* pinstance = nullptr;
    if (pinstance == nullptr)
        pinstance = &s_instance;
    return pinstance;
}

// 从注册表加载配置
void KGlobalData::loadConfigFromRegistry()
{
    qDebug() << "KGlobalData: 从注册表加载配置...";
    
    // 加载配置
    if (m_config.loadConfig()) {
        // 更新全局数据
        m_canvasWidth = m_config.canvasWidth;
        m_canvasHeight = m_config.canvasHeight;
        m_canvasColor = QString::fromStdString(m_config.backgroundColor);
        
        // 同时更新前一次的值，以避免不必要的撤销栈操作
        m_prevCanvasWidth = m_canvasWidth;
        m_prevCanvasHeight = m_canvasHeight;
        m_prevCanvasColor = m_canvasColor;
        
        qDebug() << "KGlobalData: 配置加载成功 - 画布大小:" << m_canvasWidth << "x" << m_canvasHeight 
                 << "背景色:" << m_canvasColor;
    } else {
        qDebug() << "KGlobalData: 配置加载失败，使用默认值";
    }
}

// 保存配置到注册表
void KGlobalData::saveConfigToRegistry()
{
    qDebug() << "KGlobalData: 保存配置到注册表...";
    
    // 更新配置对象
    m_config.canvasWidth = m_canvasWidth;
    m_config.canvasHeight = m_canvasHeight;
    m_config.backgroundColor = m_canvasColor.toStdString();
    
    // 保存到注册表
    if (m_config.saveConfig()) {
        qDebug() << "KGlobalData: 配置保存成功";
    } else {
        qDebug() << "KGlobalData: 配置保存失败";
    }
}

void KGlobalData::setDrawFlag(KGlobalData::KDrawFlag drawflag)
{
    this->m_drawFlag = drawflag;
}

KGlobalData::KDrawFlag KGlobalData::getDrawFlag()
{
    return m_drawFlag;
}

void KGlobalData::setColorFlag(KGlobalData::KColorFlag colorflag)
{
    this->m_colorFlag = colorflag;
}

KGlobalData::KColorFlag KGlobalData::getColorFlag()
{
    return m_colorFlag;
}

void KGlobalData::setCanvasWidth(const int width)
{
    this->m_canvasWidth = width;
    // 当设置画布宽度时，保存配置到注册表
    saveConfigToRegistry();
}

void KGlobalData::setCanvasHeight(const int height)
{
    this->m_canvasHeight = height;
    // 当设置画布高度时，保存配置到注册表
    saveConfigToRegistry();
}

int KGlobalData::getPrevCanvasWidth() const
{
    return m_prevCanvasWidth;
}

int KGlobalData::getPrevCanvasHeight() const
{
    return m_prevCanvasHeight;
}

QString KGlobalData::getPrevCanvasColor()
{
    return m_prevCanvasColor;
}

void KGlobalData::setPrevCanvasWidth(const int width)
{
    m_prevCanvasWidth = width;
}

void KGlobalData::setPrevCanvasHeight(const int height)
{
    m_prevCanvasHeight = height;
}

void KGlobalData::setPrevCanvasColor(const QString& colorStr)
{
    m_prevCanvasColor = colorStr;
}

int KGlobalData::getCanvasWidth() const
{
    return this->m_canvasWidth;
}

int KGlobalData::getCanvasHeight() const
{
    return m_canvasHeight;
}

void KGlobalData::setCanvasColor(const QString& colorStr)
{
    this->m_canvasColor = colorStr;
    // 当设置画布颜色时，保存配置到注册表
    saveConfigToRegistry();
}

QString KGlobalData::getCanvasColor()
{
    return m_canvasColor;
}

void KGlobalData::setCanvaScale(qreal scale)
{
    m_scale = scale;
    
}

qreal KGlobalData::getCanvasScale() const
{
    return m_scale;
}

void KGlobalData::setBorderWidth(const int value)
{
    m_borderWidth = value;
}

int KGlobalData::getBorderWidth() const
{
    return m_borderWidth;
}

void KGlobalData::setBoderColor(const QColor color)
{
    this->m_boderColor = color;
}

QColor KGlobalData::getBoderColor()
{
    return m_boderColor;
}

void KGlobalData::setFillColor(const QColor color)
{
    this->m_fillColor = color;
}

QColor KGlobalData::getFillColor()
{
    return m_fillColor;
}

Qt::PenStyle KGlobalData::getPenStyle() const
{
	return m_penStyle;
}

void KGlobalData::setPenStyle(const Qt::PenStyle pen_style)
{
	m_penStyle = pen_style;
}

QUndoStack* KGlobalData::getUndoStack()
{
    return &m_undoStack;
}

// Fetch button tips from the API
void KGlobalData::fetchButtonTips()
{
    QUrl url("https://m1.apifoxmock.com/m1/6237106-5930859-default/app/buttontips");
    m_networkManager->get(QNetworkRequest(url));
    qDebug() << "Fetching button tips from:" << url.toString();
}

// Get tooltip for a specific button
QPair<QString, QString> KGlobalData::getButtonTip(const QString& buttonName) const
{
    if (m_buttonTips.contains(buttonName)) {
        return m_buttonTips.value(buttonName);
    }
    return qMakePair(QString(), QString()); // Return empty pair if not found
}

// Slot to handle network reply
void KGlobalData::onButtonTipsReplyFinished(QNetworkReply* reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);

        if (!jsonDoc.isNull() && jsonDoc.isObject()) {
            QJsonObject jsonObj = jsonDoc.object();
            for (auto it = jsonObj.begin(); it != jsonObj.end(); ++it) {
                if (it.value().isObject()) {
                    QJsonObject tipObj = it.value().toObject();
                    QString title = tipObj.value("title").toString();
                    QString text = tipObj.value("text").toString();
                    m_buttonTips.insert(it.key(), qMakePair(title, text));
                }
            }
            qDebug() << "Button tips fetched and parsed successfully.";
            emit buttonTipsFetched(); // Emit signal after fetching
        } else {
            qDebug() << "Failed to parse JSON response.";
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }
    reply->deleteLater();
}


KGlobalData::~KGlobalData()
{
    // 在析构时保存配置到注册表
    saveConfigToRegistry();
    
    // networkManager is parented to this, so it will be deleted automatically
}
