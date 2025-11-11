#include "mainscene.h"
#include "./ui_mainscene.h"
#include<QList>
#include<QVariantList>
#include<QPalette>
#include<QFile>
#include"animate.h"

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene),scene(nullptr),view(nullptr)
{
    //设置标题
    setWindowTitle("Plants Fight Zombies");
    setAcceptDrops(true);

    ui->setupUi(this);
    //显示主场景
    setFixedSize(900,600);

    scene = new GameScene(this);
    view = new QGraphicsView(scene,this);
    //游戏结束后清空场景元素


    //settingsMenu
    QPushButton *menu_btn = new QPushButton("Menu",this);
    settingsMenu = new SettingsMenu(this);
    scene->setMenu(settingsMenu);
    connect(menu_btn,&QPushButton::clicked,this,[=](){
        settingsMenu->exec();
    });
    QPalette palette_menubtn;
    palette_menubtn.setBrush(QPalette::Button,QBrush(QPixmap(":/res/GameRes/images/Button.png")));
    menu_btn->setPalette(palette_menubtn);


    GamePre();
}





void MainScene::GamePre(){


    scene->GamePre();

    scene->setSceneRect(150, 0, 900, 600);

    view->setFixedSize(902, 602);
    view->setRenderHint(QPainter::Antialiasing);

    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->show();


}


MainScene::~MainScene()
{

    delete ui;
}
