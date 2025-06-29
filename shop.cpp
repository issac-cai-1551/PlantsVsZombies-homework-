#include "shop.h"
#include <QPainter>

Shop::Shop()
    : QObject(), QGraphicsPixmapItem(nullptr) // 初始化两个基类
{
    setPixmap(QPixmap(":/res/GameRes/images/Shop.png"));
    // 构造函数实现
}

Shop::~Shop() {
    // 析构函数实现
}

QRectF Shop::boundingRect() const {
    // 返回商品的边界矩形
    return QRectF(0, 0, 200, 100); // 根据实际大小调整
}
void Shop::sunlightValue(){

}
//添加卡片
void Shop::addCard(){

}
//通过拖拽卡片实现种植
void Shop::drawPlant(){

}
