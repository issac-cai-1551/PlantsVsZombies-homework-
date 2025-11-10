#include "bullet.h"
#include"animate.h"
#include<QLineF>
int Bullet::bulletNum = 0;
int Bullet::delNum = 0;

Bullet::Bullet(QString objPath,int power)
    :MyObject(nullptr,objPath,Type::Bullet),power(power),speed(200)// p/s
{
    bulletNum++;
    qDebug()<<QString::number(bulletNum);
    Hz = 30;

    //不再在bullet类内管理子弹轨迹，而是交给拿枪的植物
    //类内负责子弹击中或出界逻辑
    QMetaObject::Connection colli_conn ;
    colli_conn = connect(this,&MyObject::Meet,this,[=](MyObject* obj){
        Zombie* zombie = dynamic_cast<Zombie*>(obj);
        if (zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead()) {
            DealZombie(zombie);
            isDead = true;//检验是否发生碰撞
            disconnect(colli_conn);  // 找到第一个僵尸后disconnect
        }
    });
    connect(this->timer,&QTimer::timeout,this,[=](){
        if (isDead) {
            if (scene()) {
                // moveTimer->stop();
                // // 只断开与moveTimer的连接，保留其他连接
                // moveTimer->disconnect(this);
                scene()->removeItem(this);
            }
            deleteLater();
            return;
        }

        // 超出场景边界时删除
        // 获取场景边界
        qreal sceneWidth = scene()->width();
        qreal sceneHeight = scene()->height();
        qreal buffer = 100; // 缓冲值，可根据需求调整

        //超出场景左、右、上、下边界
        bool outOfLeft = x() < -buffer;          // 左边界外
        bool outOfRight = x() > sceneWidth + buffer; // 右边界外
        bool outOfTop = y() < -buffer;           // 上边界外
        bool outOfBottom = y() > sceneHeight + buffer; // 下边界外

        if (outOfLeft || outOfRight || outOfTop || outOfBottom) {
            // moveTimer->stop();
            // // 只断开与moveTimer的连接，保留其他连接
            // moveTimer->disconnect(this);
            scene()->removeItem(this);

            deleteLater();
        }
    });
}
void Bullet::DealZombie(Zombie *zombie){
    if(zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead())
     zombie->beHeated(power,DieType::Normal);
}

//  void Bullet::advance(int phase)
// {
//     if (!phase) return;  // 跳过阶段0，只在阶段1执行更新

//     // 子弹向前移动
//     setPos(x() + speed/Hz, y());

//     // 检测碰撞
//     QList<QGraphicsItem*> items = collidingItems();

//     for (auto it = items.begin(); it != items.end(); ++it) {
//         // 修正：解引用迭代器获取QGraphicsItem*
//         Zombie* zombie = dynamic_cast<Zombie*>(*it);
//         if (zombie && zombie->getObjType() == Type::ZOMBIE && !zombie->IsDead()) {
//             DealZombie(zombie);

//             isDead = true;//检验是否发生碰撞
//             break;  // 找到第一个僵尸后退出循环
//         }
//     }

//     // 在循环外处理删除，确保安全
//     if (isDead) {
//         if (scene()) {
//             // moveTimer->stop();
//             // // 只断开与moveTimer的连接，保留其他连接
//             moveTimer->disconnect(this);
//             scene()->removeItem(this);
//         }
//         deleteLater();
//         return;
//     }

//     // 超出场景边界时删除（添加安全性检查）
//     if (scene() && x() > scene()->width() + 100) {
//         // moveTimer->stop();
//         // // 只断开与moveTimer的连接，保留其他连接
//         moveTimer->disconnect(this);
//         scene()->removeItem(this);

//         deleteLater();
//     }
// }

Bullet::~Bullet(){
    delNum++;
    qDebug()<<"out"+QString::number(delNum);
}




