#include "ncentralview.h"

#include <QApplication>
#include <QDebug>

#define VIEW_CENTER viewport()->rect().center()
#define VIEW_WIDTH  viewport()->rect().width()
#define VIEW_HEIGHT viewport()->rect().height()

nCentralView::nCentralView(QGraphicsScene *scene,QWidget *parent):QGraphicsView(parent)
    ,m_scale(1.0),
    m_zoomDelta(0.1),
    m_translateSpeed(1.0),
    m_bMouseTranslate(false),
     m_translateButton(Qt::LeftButton)
{
   setScene(scene);
    setSceneRect(scene->sceneRect().x(),scene->sceneRect().y(),scene->width(),scene->height());
   //setSceneRect(0,0,scene->width(),scene->height());
   // 去掉滚动条
       setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
       setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
     //  setCursor(Qt::PointingHandCursor);
       setRenderHint(QPainter::Antialiasing);


}
nCentralView::~nCentralView()
{

}


void nCentralView::setTranslateSpeed(qreal speed)
{
    // 建议速度范围
        Q_ASSERT_X(speed >= 0.0 && speed <= 2.0,
                   "InteractiveView::setTranslateSpeed", "Speed should be in range [0.0, 2.0].");
        m_translateSpeed = speed;
}
qreal nCentralView::translateSpeed() const
{
      return m_translateSpeed;
}
void nCentralView::setZoomDelta(qreal delta)
{
    // 建议增量范围
        Q_ASSERT_X(delta >= 0.0 && delta <= 1.0,
                   "InteractiveView::setZoomDelta", "Delta should be in range [0.0, 1.0].");
        m_zoomDelta = delta;
}
qreal nCentralView::zoomDelta() const
{
     return m_zoomDelta;
}
void nCentralView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Up:
            translate(QPointF(0, -2));  // 上移
            break;
        case Qt::Key_Down:
            translate(QPointF(0, 2));  // 下移
            break;
        case Qt::Key_Left:
            translate(QPointF(-2, 0));  // 左移
            break;
        case Qt::Key_Right:
            translate(QPointF(2, 0));  // 右移
            break;
        case Qt::Key_Equal:  // 放大
            zoomIn();
            break;
        case Qt::Key_Minus:  // 缩小
            zoomOut();
            break;
        case Qt::Key_Space:  // 逆时针旋转
            rotate(-5);
            break;
        case Qt::Key_Enter:  // 顺时针旋转
        case Qt::Key_Return:
            rotate(5);
            break;
        default:
            QGraphicsView::keyPressEvent(event);

    }
}

// 平移
void nCentralView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bMouseTranslate){
            QPointF mouseDelta = mapToScene(event->pos()) - mapToScene(m_lastMousePos);
            translate(mouseDelta);
        }

        m_lastMousePos = event->pos();
     QGraphicsView::mouseMoveEvent(event);
}
void nCentralView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton) {
            // 当光标底下没有 item 时，才能移动
            QPointF point = mapToScene(event->pos());
            if (scene()->itemAt(point, transform()) == NULL)  {
                m_bMouseTranslate = true;
                m_lastMousePos = event->pos();
            }
        }
     QGraphicsView::mousePressEvent(event);
}
void nCentralView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == m_translateButton)
            m_bMouseTranslate = false;

     QGraphicsView::mouseReleaseEvent(event);
}

// 放大/缩小
void nCentralView::wheelEvent(QWheelEvent *event)
{
    // 滚轮的滚动量
        QPoint scrollAmount = event->angleDelta();
        // 正值表示滚轮远离使用者（放大），负值表示朝向使用者（缩小）
        scrollAmount.y() > 0 ? zoomIn() : zoomOut();
}

// 放大
void nCentralView::zoomIn()
{
    zoom(1 + m_zoomDelta);
}
// 缩小
void nCentralView::zoomOut()
{
    zoom(1 - m_zoomDelta);
}
void nCentralView::sratate()
{
    rotate(5);

}
void nCentralView::aratate()
{
    rotate(-5);

}
void nCentralView::vup()
{
  translate(QPointF(0, -2));
}
void nCentralView::vdown()
{
  translate(QPointF(0, 2));
}
void nCentralView::vleft()
{
  translate(QPointF(-2, 0));
}
void nCentralView::vright()
{
  translate(QPointF(2, 0));
}
void nCentralView::zoom(float scaleFactor)
{
    // 防止过小或过大
        qreal factor = transform().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();
        if (factor < 0.07 || factor > 100)
            return;

        scale(scaleFactor, scaleFactor);
        m_scale *= scaleFactor;
}
void nCentralView::translate(QPointF delta)
{
    // 根据当前 zoom 缩放平移数
        delta *= m_scale;
        delta *= m_translateSpeed;

        // view 根据鼠标下的点作为锚点来定位 scene
        setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
        QPoint newCenter(VIEW_WIDTH / 2 - delta.x(),  VIEW_HEIGHT / 2 - delta.y());
        centerOn(mapToScene(newCenter));

        // scene 在 view 的中心点作为锚点
        setTransformationAnchor(QGraphicsView::AnchorViewCenter);
}
