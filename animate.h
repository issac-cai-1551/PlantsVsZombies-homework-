#ifndef ANIMATE_H
#define ANIMATE_H

#include <QWidget>
#include"gamescene.h"
#include<QEasingCurve>
#include<QPropertyAnimation>
#include<QParallelAnimationGroup>
#include<QSequentialAnimationGroup>

class Animate;

enum class AnimationType{
    Move,
    Scale,
    Opacity
};

class Animate
{
    QPropertyAnimation *anim_m;
    QPropertyAnimation* anim_s;
    QPropertyAnimation* anim_o;
    //
    int type;
    int m_duration_m;
    int m_duration_s;
    int m_duration_o;
    qreal m_speed_m;
    qreal m_speed_s;
    qreal m_speed_o;
    QPointF dir;
    QPointF toPos;
    qreal toScale, toOpacity;
    QEasingCurve::Type m_shape_m;
    QEasingCurve::Type m_shape_s;
    QEasingCurve::Type m_shape_o;
    //
    MyObject* target;
    QGraphicsScene *scene;
    //detect event interval
    int interval_m;
    int cnt_m;
    int interval_s;
    int cnt_s;
    int interval_o;
    int cnt_o;
    Animate & setInterval(enum AnimationType animType,int interval);
    //使用QHash管理target的动画实例
    static QHash<MyObject*,QPropertyAnimation*> moveAnim;
    static QHash<MyObject*,QPropertyAnimation*> scaleAnim;
    static QHash<MyObject*,QPropertyAnimation*> opacityAnim;
public:
    Animate(MyObject* target,GameScene* scene = nullptr);
    ~Animate();
    Animate &move(QPointF toPos,bool asDir=true);
    Animate &scale(qreal toScale);
    Animate &fade(qreal toOpacity);

    Animate &replace();
    Animate &shape(enum AnimationType animType,QEasingCurve::Type shape);
    Animate &speed(enum AnimationType animType,qreal speed);
    Animate &duration(enum AnimationType animType,int duration);

    //需要知道是哪种动画停止
    Animate &finish(enum AnimationType animType,std::function<void(void)> functor);
    Animate &finish(enum AnimationType animType,std::function<void(bool)> functor = [](bool) {});

    Animate &pause(enum AnimationType animType);//暂停动画
    Animate &resume(enum AnimationType animType);//继续动画
    Animate &stop(enum AnimationType animType);//停zi动画

    //连接target会在动画过程中触发的函数
    Animate &trigger(enum AnimationType animType,std::function<void(void)> functor,int interval = 50);

signals:
    void m_timeout();
    void s_timeout();
    void o_timeout();
public slots:
    static void onTargetDestroyed(QObject* target);
};

#endif // ANIMATE_H
