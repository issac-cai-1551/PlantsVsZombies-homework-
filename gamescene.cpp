#include "gamescene.h"
#include"animate.h"

GameScene::GameScene(QObject *parent)
    : QGraphicsScene{parent}
{

}

void GameScene::move(MyObject* target,QPointF& dest){

}

void GameScene::plant(){

}
void GameScene::PlantAreaGenerate(){
    //打开配置文件
    settings->beginGroup("MapInfo");
    //
    for(int i=0;i<5;i++){
        QVariantList MapRow = settings->value(QString("row%1").arg(i)).toList();
        QVariantList MowerRow = settings->value(QString("MowerRow")).toList();
        //Mower
        if(MowerRow[i].toInt()){
            Mower *mower = new Mower();
            mower->setPos(QPointF(150 +105 ,120) + QPointF(-20 ,94*i));
            addItem(mower);
            connect(this,&GameScene::GameOver,mower,&MyObject::GameOver);
        }
        for(int j=0;j<9;j++){
            enum LandType landType = LandType::None;
            switch (MapRow[j].toInt()) {
            case 0:{
                landType = LandType::Nomal;
                break;
            }
            case 1:{
                landType = LandType::DryLand;
                break;
            }
            default:
                break;
            }
            PlantArea *area = new PlantArea(i,j,landType);
            area->setPos(QPointF(150 +105 ,90) + QPointF(area->w()*j , area->h()*i));//81,94
            //连接向日葵生成的阳光
            connect(area,&PlantArea::sunlightProduced,this,[=](SunLight *sunlight){
                //仅仅收集阳光使用bool(int)
                bool (Shop::*func)(int) = &Shop::sunlightValueShow;
                connect(sunlight,&SunLight::sunlightCollected,shop,func);
                connect(this,&GameScene::GameOver,sunlight,&QGraphicsObject::deleteLater);
            });
            //买并种植植物消耗阳光
            connect(area,&PlantArea::needToPlant,this,[=](int cost,enum PlantType plantType){
                //如果阳光足够
                if(shop->sunlightValueShow(-cost,plantType)){
                    area->plant(plantType);
                }
            });
            addItem(area);
            connect(this,&GameScene::GameOver,area,&PlantArea::GameOver);

        }
    }
    //
    settings->endGroup();
}
void GameScene::ZombieGenerate(){
    int gen = QRandomGenerator::global()->bounded(0,11);//随机0到4
    int offsetX = QRandomGenerator::global()->bounded(0,100);//避免僵尸同时出现，用距离控制时间
    QPointF start(this->width()+200+offsetX,300);
    Zombie *zombie=nullptr;
    //随机决定僵尸种类
    if(gen<3)
        zombie = new NomalZombie();
    else if(gen<5){
        zombie = new ScreenZombie();
    }
    else if(gen<7){
        zombie = new BucketZombie();
    }
    else if(gen<9){
        zombie = new ConeZombie();
    }
    else if(gen<11){
        zombie = new FootballZombie();
    }
    if(zombie)
    {
        zombie->setPos(start);
        addItem(zombie);
        //处理僵尸行走
        // connect(moveTimer,&QTimer::timeout,zombie,[=](){
        //     zombie->proceed();
        // });
        Animate(zombie).speed(AnimationType::Move,zombie->getSpeed()).move(QPointF(-900,0));
        //处理僵尸胜利的请款
        connect(zombie,&Zombie::zombieSuccess,this,[=](){

        });
        connect(this,&GameScene::GameOver,zombie,&MyObject::GameOver);
    }
}
void GameScene::cardAvailable(){
    if(selectPlant)
    {
        selectPlant->addCard(":/res/GameRes/images/WallNut.png");
        selectPlant->addCard(":/res/GameRes/images/Peashooter.png");
        selectPlant->addCard(":/res/GameRes/images/SunFlower.png");
        selectPlant->addCard(":/res/GameRes/images/CherryBomb.png");
        selectPlant->addCard(":/res/GameRes/images/PotatoMine.png");
        selectPlant->addCard(":/res/GameRes/images/SnowPea.png");
    }
}
void GameScene::GameStart(){

}
void GameScene::GamePre(){

}



