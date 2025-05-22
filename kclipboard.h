#ifndef ___K_CLIPBOARD_H__
#define ___K_CLIPBOARD_H__

#include "kglobaldata.h"

#include <QObject>
#include <QClipboard>
#include <QGuiApplication>

// -------------------------------------------------------
// kclipboard.h
// 创建者： 肖朝洋
// 创建时间： 2024/7/29
// 功能描述： 复制、剪切、粘贴。
// Copyright 2024 Kingsoft
// --------------------------------------------------------

enum class KShapeType;
class KShape;
class KDrawFlag;
class KGlobalData;

class KShapeClipboard : public QObject
{
    Q_OBJECT

public:
    KShapeClipboard(QObject* parent = Q_NULLPTR);
    ~KShapeClipboard();

    void copy(QList<KShape*>& shapeList);
    bool contains();
    QList<KShape*> getData(const QPoint& point,QWidget* parent);
    void clear();

private:
    void switchType(KShapeType shapeType);
    QClipboard* clipboard;
    KShapeType m_shapeType;
    KGlobalData::KDrawFlag m_drawFlag;

    bool isSigleShape; // 目前只是支持单个图形复制，可指定粘贴位置，多个图形，统一设置偏移量
};
#endif  //___KCLIPBOARD_H__