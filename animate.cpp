#include "animate.h"

QHash<MyObject*,QPropertyAnimation*> Animate::moveAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::scaleAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::opacityAnim;

Animate::Animate(MyObject* target,GameScene* scene):
    type(0),
    m_duration_m(0),m_duration_s(0),m_duration_o(0)
    , m_speed_m(5),m_speed_s(5),m_speed_o(5),
    toPos(), toScale(0), toOpacity(0),
    m_shape_m(QEasingCurve::Linear),
    m_shape_s(QEasingCurve::Linear),
    m_shape_o(QEasingCurve::Linear),
    target(target), scene(scene)
{//
    if(!moveAnim.contains(target) || !scaleAnim.contains(target) || !opacityAnim.contains(target)){
        //仅在第一次target建立动画时绑定，后续无法绑定,管理哈希表，防止野指针
        QObject::connect(target,
                         &MyObject::needToDel,
                         [=](){
                             moveAnim.remove(target);
                             scaleAnim.remove(target);
                             opacityAnim.remove(target);
                         });
    }
    //管理哈希表，防止野指针
    if(moveAnim.contains(target))anim_m = moveAnim[target];
    else{
        anim_m = new QPropertyAnimation(target,"pos",target);
        moveAnim.insert(target,anim_m);
    }
    //
    if(scaleAnim.contains(target))anim_s = scaleAnim[target];
    else{
        anim_s = new QPropertyAnimation(target,"scale",target);
        scaleAnim.insert(target,anim_s);
    }
    //
    if(opacityAnim.contains(target))anim_o = opacityAnim[target];
    else{
        anim_o = new QPropertyAnimation(target,"opacity",target);
        opacityAnim.insert(target,anim_o);

    }

    //

}
Animate::~Animate(){
    //内存管理留给QT父指针制度
    // delete anim_m;
    // moveAnim.remove(target);
    // delete anim_s;
    // scaleAnim.remove(target);
    // delete anim_o;
    // opacityAnim.remove(target);
}
Animate& Animate::move(QPointF toPos,bool asDir){
    //暂停动画，防止错乱
    if(anim_m->state() == QAbstractAnimation::Running){
        anim_m->pause();
    }
    //重新设置各项属性
    anim_m->setStartValue(target->pos());//将当前位置作为动画开始
    //分两种情况，toPos是dir或者是dest
    if(asDir){//作为方向的情况
        this->toPos = target->pos() + toPos;
    }
    else{//作为目的地的情况
        this->toPos = toPos;
    }
    anim_m->setEndValue(this->toPos);
    anim_m->setEasingCurve(m_shape_m);
    //
    if(m_duration_m<=0){//可以用速度计算出来
        QPointF posVec = this->toPos - target->pos();
        m_duration_m = qRound(qSqrt(QPointF::dotProduct(posVec, posVec)) / m_speed_m);
    }

    anim_m->setDuration(m_duration_m);

    if (anim_m && anim_m->state() == QAbstractAnimation::Paused) {
        anim_m->resume();
    }
    else anim_m->start();
    return *this;
}
Animate& Animate::scale(qreal toScale){
    //暂停动画，防止错乱
    if(anim_s->state() == QAbstractAnimation::Running){
        anim_s->pause();
    }
    //重新设置各项属性
    anim_s->setStartValue(target->scale());//将当前位置作为动画开始
    this->toScale = toScale;

    anim_s->setEndValue(this->toScale);
    anim_s->setEasingCurve(m_shape_s);
    anim_s->setDuration(m_duration_s);

    if (anim_s && anim_s->state() == QAbstractAnimation::Paused) {
        anim_s->resume();
    }
    else anim_s->start();
    return *this;
}
Animate& Animate::fade(qreal toOpacity){
    //暂停动画，防止错乱
    if(anim_o->state() == QAbstractAnimation::Running){
        anim_o->pause();
    }
    //重新设置各项属性
    this->toOpacity = toOpacity;
    anim_o->setStartValue(target->opacity());//将当前位置作为动画开始

    anim_o->setEndValue(this->toOpacity);
    anim_o->setEasingCurve(m_shape_o);
    anim_o->setDuration(m_duration_o);

    if (anim_o && anim_o->state() == QAbstractAnimation::Paused) {
        anim_o->resume();
    }
    else anim_o->start();
    return *this;
}


Animate& Animate::replace(){
    return *this;
}
Animate& Animate::shape(enum AnimationType animType,QEasingCurve::Type shape){
    switch (animType) {
    case AnimationType::Move:
        m_shape_m = shape;
        break;
    case AnimationType::Scale:
        m_shape_s = shape;
        break;
    case AnimationType::Opacity:
        m_shape_o = shape;
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::speed(enum AnimationType animType,qreal speed){
    switch (animType) {
    case AnimationType::Move:
        m_speed_m = speed;
        break;
    case AnimationType::Scale:
        m_speed_s = speed;
        break;
    case AnimationType::Opacity:
        m_speed_o = speed;
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::duration(enum AnimationType animType,int duration){

    switch (animType) {
    case AnimationType::Move:
        m_duration_m = duration;
        break;
    case AnimationType::Scale:
        m_duration_s = duration;
        break;
    case AnimationType::Opacity:
        m_duration_o = duration;
        break;
    default:
        break;
    }
    return *this;
}

Animate& Animate::finish(enum AnimationType animType,std::function<void(void)> functor){
    switch (animType) {
    case AnimationType::Move:
        if(target->pos() == toPos){functor();}
        break;
    case AnimationType::Scale:
        if(target->scale() == toScale){functor();}
        break;
    case AnimationType::Opacity:
        if(target->opacity() == toOpacity){functor();}
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::finish(enum AnimationType animType,std::function<void(bool)>){
    return *this;
}

Animate& Animate::pause(enum AnimationType animType){
    //停止动画
    switch (animType) {
    case AnimationType::Move:
        if(anim_m->state() == QAbstractAnimation::Running)
        anim_m->pause();
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Running)
        anim_s->pause();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Running)
        anim_o->pause();
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::resume(enum AnimationType animType){
    //停止动画
    switch (animType) {
    case AnimationType::Move:
        if (anim_m && anim_m->state() == QAbstractAnimation::Paused)
        anim_m->resume();
        break;
    case AnimationType::Scale:
        if (anim_s && anim_s->state() == QAbstractAnimation::Paused)
        anim_s->resume();
        break;
    case AnimationType::Opacity:
        if (anim_o && anim_o->state() == QAbstractAnimation::Paused)
        anim_o->resume();
        break;
    default:
        break;
    }
    return *this;
}
void Animate::onTargetDestroyed(QObject* target){
    if (!target) return;

    MyObject* myTarget = qobject_cast<MyObject*>(target);
    if (!myTarget) return;
    // 从三个哈希表中移除对应条目
    qDebug()<<"ok";
    moveAnim.remove(myTarget);
    scaleAnim.remove(myTarget);
    opacityAnim.remove(myTarget);
}
