#include "animate.h"

QHash<MyObject*,QPropertyAnimation*> Animate::moveAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::scaleAnim;
QHash<MyObject*,QPropertyAnimation*> Animate::opacityAnim;

//注意：：每次调用都意味着要给出完整动画，不能想当然与之前的动画关联（Animate使用指针版除外）
Animate::Animate(MyObject* target,GameScene* scene):
    type(0),
    m_duration_m(0),m_duration_s(0),m_duration_o(0)
    , m_speed_m(20),m_speed_s(20),m_speed_o(20),
    toPos(), toScale(0), toOpacity(0),
    m_shape_m(QEasingCurve::Linear),
    m_shape_s(QEasingCurve::Linear),
    m_shape_o(QEasingCurve::Linear),
    target(target), scene(scene),
    interval_m(50),cnt_m(0),
    interval_s(50),cnt_s(0),
    interval_o(50),cnt_o(0)
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
    // QObject::connect(anim_m,&QPropertyAnimation::valueChanged,[=](){
    //     cnt_m = (cnt_m+1) % interval_m;
    //     if(cnt_m==0)emit m_timeout();
    // });

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
        anim_m->stop();
    }
    //重新设置各项属性
    QPointF currPos = target->pos();

    //分两种情况，toPos是dir或者是dest
    this->toPos = asDir ? currPos + toPos : toPos;

    //
    //可以用速度计算出来
    if(m_duration_m<=0)
    {
        QPointF posVec = this->toPos - currPos;
        m_duration_m = std::max(1,qRound(qSqrt(QPointF::dotProduct(posVec, posVec)) / m_speed_m) * 1000);// p/s
    }

    anim_m->setStartValue(currPos);
    anim_m->setEndValue(this->toPos);
    anim_m->setDuration(m_duration_m);
    anim_m->setEasingCurve(m_shape_m);

    if (anim_m->state() == QAbstractAnimation::Paused) {
        // qDebug()<<"pause resume"<<currPos;
        anim_m->resume();
    } else if(anim_m->state() == QAbstractAnimation::Stopped) {
        // qDebug()<<"stop resume"<<currPos;
        anim_m->start();
    }
    m_duration_m = 0;//确保下次用speed计算
    return *this;
}
Animate& Animate::scale(qreal toScale){
    //暂停动画，防止错乱
    if(anim_s->state() == QAbstractAnimation::Running){
        anim_s->pause();
    }
    //重新设置各项属性
    qreal currScale = target->scale();
    this->toScale = toScale;

    if (anim_s->state() == QAbstractAnimation::Running) {
        //动画正在运行,直接更新属性，不停止
        anim_s->setEndValue(this->toScale);
        anim_s->setDuration(m_duration_s);
        anim_s->setEasingCurve(m_shape_s);
    } else  {
        //从头开始
        anim_s->setStartValue(currScale);
        anim_s->setEndValue(this->toScale);
        anim_s->setDuration(m_duration_s);
        anim_s->setEasingCurve(m_shape_s);

        anim_s->start();
    }
    return *this;
}
Animate& Animate::fade(qreal toOpacity){
    //暂停动画，防止错乱
    if(anim_o->state() == QAbstractAnimation::Running){
        anim_o->pause();
    }
    //重新设置各项属性
    this->toOpacity = toOpacity;
    qreal currOpacity = target->opacity();

    if (anim_o->state() == QAbstractAnimation::Running) {
        //动画正在运行,直接更新属性，不停止
        anim_o->setEndValue(this->toOpacity);
        anim_o->setDuration(m_duration_o);
        anim_o->setEasingCurve(m_shape_o);
    } else  {
        //从头开始
        anim_o->setStartValue(currOpacity);
        anim_o->setEndValue(this->toOpacity);
        anim_o->setDuration(m_duration_o);
        anim_o->setEasingCurve(m_shape_o);

        anim_o->start();
    }
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
Animate& Animate::setInterval(enum AnimationType animType,int interval){
    switch (animType) {
    case AnimationType::Move:
        interval_m = interval;
        break;
    case AnimationType::Scale:
        interval_s = interval;;
        break;
    case AnimationType::Opacity:
        interval_o = interval;;
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
        if(target->pos() == toPos && anim_m->state()==QAbstractAnimation::Stopped){functor();}
        break;
    case AnimationType::Scale:
        if(target->scale() == toScale && anim_s->state()==QAbstractAnimation::Stopped){functor();}
        break;
    case AnimationType::Opacity:
        if(target->opacity() == toOpacity && anim_o->state()==QAbstractAnimation::Stopped){functor();}
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
        {


            anim_m->pause();
            qDebug()<<"pause22"<<anim_m->state();
        }
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Running)
        anim_s->pause();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Running)
        anim_o->pause();
        break;
    case AnimationType::All:
        this->pause(AnimationType::Move);
        this->pause(AnimationType::Scale);
        this->pause(AnimationType::Opacity);
        break;
    default:
        break;
    }
    return *this;
}
Animate& Animate::stop(enum AnimationType animType){
    //停止动画
    switch (animType) {
    case AnimationType::Move:
        if(anim_m->state() == QAbstractAnimation::Running)
            anim_m->stop();
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Running)
            anim_s->stop();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Running)
            anim_o->stop();
        break;
    case AnimationType::All:
        this->stop(AnimationType::Move);
        this->stop(AnimationType::Scale);
        this->stop(AnimationType::Opacity);
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
        qDebug()<<"resume"<<anim_s->state();

        if(anim_s->state() == QAbstractAnimation::Paused)
        {


            anim_s->resume();
        }
        if(anim_s->state()==QAbstractAnimation::Stopped)
            anim_s->start();
        break;
    case AnimationType::Scale:
        if(anim_s->state() == QAbstractAnimation::Paused)
            anim_s->resume();
        if(anim_s->state()==QAbstractAnimation::Stopped)
            anim_s->start();
        break;
    case AnimationType::Opacity:
        if(anim_o->state() == QAbstractAnimation::Paused)
            anim_o->resume();
        if(anim_o->state()==QAbstractAnimation::Stopped)
            anim_o->start();
        break;
    case AnimationType::All:
        this->resume(AnimationType::Move);
        this->resume(AnimationType::Scale);
        this->resume(AnimationType::Opacity);
    default:
        break;
    }
    return *this;
}
Animate& Animate::trigger(enum AnimationType animType,std::function<void(void)> functor,int interval){

    if(target){
        functor();
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
