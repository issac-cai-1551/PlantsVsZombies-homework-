#include "mainscene.h"
#include "./ui_mainscene.h"
#include<QPainter>
#include<QPixmap>

MainScene::MainScene(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainScene)
{
    ui->setupUi(this);
    //显示主场景
    scene = new QGraphicsScene(this);
    scene->setSceneRect(150, 0, 900, 600);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setFixedSize(900,600);

    Shop *shop = new Shop;
    shop->setPos(290, 0);
    scene->addItem(shop);




    view = new QGraphicsView(scene, this);
    view->resize(902, 602);
    view->setRenderHint(QPainter::Antialiasing);
    view->setBackgroundBrush(QPixmap(":/res/GameRes/images/Background.jpg"));
    view->setCacheMode(QGraphicsView::CacheBackground);
    view->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    view->show();
}

MainScene::~MainScene()
{
    delete ui;
}
