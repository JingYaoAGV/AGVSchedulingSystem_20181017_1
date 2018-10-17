#ifndef NCENTRALVIEW_H
#define NCENTRALVIEW_H


#include <QGraphicsView>
#include <QKeyEvent>
class nCentralView : public QGraphicsView
{
     Q_OBJECT
public:
     explicit nCentralView(QGraphicsScene *scene,QWidget *parent=0);
    ~nCentralView();
    // 平移速度
    void setTranslateSpeed(qreal speed);
    qreal translateSpeed() const;
    // 缩放的增量
    void setZoomDelta(qreal delta);
    qreal zoomDelta() const;
protected:
    // 上/下/左/右键向各个方向移动、加/减键进行缩放、空格/回车键旋转
    void keyPressEvent(QKeyEvent *event) override;
    // 平移
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    // 放大/缩小
    void wheelEvent(QWheelEvent *event) override;
public slots:
    void zoomIn();  // 放大
    void zoomOut();  // 缩小
    void sratate();
    void aratate();
    void vup();
    void vdown();
    void vleft();
    void vright();
    void zoom(float scaleFactor); // 缩放 - scaleFactor：缩放的比例因子
    void translate(QPointF delta);  // 平移
private:
    Qt::MouseButton m_translateButton;  // 平移按钮
    qreal m_translateSpeed;  // 平移速度
    qreal m_zoomDelta;  // 缩放的增量
    bool m_bMouseTranslate;  // 平移标识
    QPoint m_lastMousePos;  // 鼠标最后按下的位置
    qreal m_scale;  // 缩放值
};

#endif // NCENTRALVIEW_H
