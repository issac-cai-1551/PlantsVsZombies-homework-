#ifndef BULLET_H
#define BULLET_H
#include"myobj.h"
#include "zombie.h"

class Zombie;
class Bullet : public MyObject
{
    Q_OBJECT
    int power;//子弹威力
    // QTimer *moveTimer;
    int Hz;
    double speed;
public:
    explicit Bullet(QString objPath,int power);

    int getHz(){return Hz;}
    double getSpeed(){return speed;}
    void setSpeed(double speed){speed = speed;}
    ~Bullet();
    static int bulletNum;
    static int delNum;

protected:
    virtual void DealZombie(Zombie *zombie);

signals:


};
#endif // BULLET_H
