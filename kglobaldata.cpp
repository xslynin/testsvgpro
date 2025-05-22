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
}

KGlobalData *KGlobalData::getGlobalDataIntance()
{
    static KGlobalData s_instance;
    static KGlobalData* pinstance = nullptr;
    if (pinstance == nullptr)
        pinstance = &s_instance;
    return pinstance;
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
}

void KGlobalData::setCanvasHeight(const int height)
{
    this->m_canvasHeight = height;
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
    // networkManager is parented to this, so it will be deleted automatically
}
