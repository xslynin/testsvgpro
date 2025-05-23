# 画布缩放功能的架构优化建议

基于对您的SVG编辑器代码的分析，我发现当前的画布缩放功能使用了观察者模式，但实现方式可能存在一些性能和实时性的问题。以下是我对架构的改进建议：

## 当前实现分析

目前的实现方式：
1. 在`KSvgMainWindow::wheelEvent`中处理鼠标滚轮事件
2. 通过`KGlobalData`单例存储缩放值
3. 使用信号槽机制(`scaleChanged`信号)将缩放值更新到参数栏
4. 参数栏编辑时通过`validateCanvasParams`更新画布

这种实现存在以下问题：
- 信号槽机制在高频滚轮事件中可能导致性能开销
- 数据流向不够直接，需要经过多个类的传递
- 缩放操作和UI更新之间存在延迟
- 缩放中心点固定，没有以鼠标位置为中心进行缩放

## 改进方案

### 1. 使用MVVM架构替代观察者模式

MVVM(Model-View-ViewModel)架构比传统观察者模式更适合此类UI交互：

```cpp
// KCanvasViewModel类 - 作为画布和参数栏之间的桥梁
class KCanvasViewModel : public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    
public:
    qreal scale() const { return m_scale; }
    
    void setScale(qreal scale) {
        if (qAbs(m_scale - scale) < 0.001)
            return;
        m_scale = scale;
        emit scaleChanged(m_scale);
    }
    
signals:
    void scaleChanged(qreal scale);
    
private:
    qreal m_scale = 1.0;
};
```

### 2. 使用属性绑定实现双向同步

```cpp
// 在KMainWindow中初始化
void KMainWindow::initBindings() {
    // 双向绑定：画布缩放 <-> 参数栏
    connect(m_viewModel, &KCanvasViewModel::scaleChanged, this, [this](qreal scale) {
        // 更新参数栏UI，不触发信号
        m_pCanvasParamsBar->m_pScaleBox->blockSignals(true);
        m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->setText(QString::number(scale * 100));
        m_pCanvasParamsBar->m_pScaleBox->blockSignals(false);
        
        // 更新画布大小
        updateCanvasSize();
    });
    
    // 参数栏编辑时更新模型
    connect(m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit, &QLineEdit::editingFinished, this, [this]() {
        qreal scale = m_pCanvasParamsBar->m_pScaleBox->m_pParamEdit->text().toDouble() / 100;
        m_viewModel->setScale(scale);
    });
}
```

### 3. 优化滚轮缩放实现

```cpp
void KSvgMainWindow::wheelEvent(QWheelEvent* event) {
    if (QApplication::keyboardModifiers() == Qt::ControlModifier) {
        // 获取鼠标在画布上的位置作为缩放中心点
        QPointF mousePos = event->posF();
        QPointF viewportCenter = QPointF(viewport()->width() / 2.0, viewport()->height() / 2.0);
        QPointF scenePosBeforeScale = mapToScene(mousePos.toPoint());
        
        // 计算新的缩放值
        qreal scaleFactor = (event->delta() > 0) ? 1.05 : 0.95;
        qreal newScale = m_viewModel->scale() * scaleFactor;
        
        // 限制缩放范围
        newScale = qBound(0.1, newScale, 5.0);
        
        // 更新模型中的缩放值（会触发UI更新）
        m_viewModel->setScale(newScale);
        
        // 调整视图以保持鼠标位置不变
        QPointF scenePosAfterScale = mapToScene(mousePos.toPoint());
        QPointF delta = scenePosAfterScale - scenePosBeforeScale;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() + delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() + delta.y());
        
        event->accept();
    } else {
        QScrollArea::wheelEvent(event);
    }
}
```

### 4. 使用QML/QtQuick实现更流畅的缩放效果

如果您愿意考虑更现代的UI技术，可以使用QML/QtQuick替代传统的QWidget：

```qml
// CanvasView.qml
Rectangle {
    id: canvasContainer
    
    property real scale: 1.0
    
    Canvas {
        id: canvas
        anchors.centerIn: parent
        width: canvasWidth * scale
        height: canvasHeight * scale
        
        transform: Scale {
            origin.x: canvas.width / 2
            origin.y: canvas.height / 2
            xScale: scale
            yScale: scale
        }
        
        // 绘制代码...
    }
    
    MouseArea {
        anchors.fill: parent
        onWheel: function(wheel) {
            if (wheel.modifiers & Qt.ControlModifier) {
                var scaleFactor = wheel.angleDelta.y > 0 ? 1.05 : 0.95;
                var newScale = scale * scaleFactor;
                scale = Math.min(Math.max(0.1, newScale), 5.0);
                
                // 通知C++后端
                backend.setCanvasScale(scale);
            }
        }
    }
}
```

### 5. 使用Direct2D/OpenGL加速渲染

对于大型SVG或频繁缩放操作，可以考虑使用硬件加速渲染：

```cpp
class KGLCanvas : public QOpenGLWidget, protected QOpenGLFunctions {
public:
    void setScale(qreal scale) {
        m_scale = scale;
        update(); // 触发重绘
    }
    
protected:
    void initializeGL() override {
        initializeOpenGLFunctions();
        // 初始化OpenGL资源
    }
    
    void paintGL() override {
        // 使用OpenGL渲染SVG内容
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glScalef(m_scale, m_scale, 1.0);
        
        // 渲染图形...
    }
    
private:
    qreal m_scale = 1.0;
};
```

## 总结建议

1. **采用MVVM架构**：将画布状态(包括缩放)集中在ViewModel中管理，替代分散的观察者模式
2. **使用属性绑定**：实现UI和数据的自动双向同步，减少手动同步代码
3. **优化缩放算法**：以鼠标位置为中心进行缩放，提供更自然的用户体验
4. **考虑使用QML**：对于复杂的UI交互，QML提供更现代和高效的实现方式
5. **硬件加速**：对于大型SVG或频繁缩放操作，考虑使用OpenGL或Direct2D加速渲染
6. **减少中间层**：直接连接画布和参数栏，减少通过全局单例的数据传递

这些改进将显著提高画布缩放的性能和实时性，同时使代码结构更清晰、更易于维护。