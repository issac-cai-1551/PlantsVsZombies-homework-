#ifndef SHOP_H
#define SHOP_H

#include <QObject>
#include <QGraphicsPixmapItem>

class Shop : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Shop();
    ~Shop() override;

    // 必须实现的虚函数
    QRectF boundingRect() const override;

    //显示阳光值
    void sunlightValue();
    //添加卡片
    void addCard();
    //拖拽种植
    void drawPlant();

signals:
    // 信号声明
};

#endif // SHOP_H
