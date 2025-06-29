#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QMainWindow>
#include "shop.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainScene;
}
QT_END_NAMESPACE

class MainScene : public QMainWindow
{
    Q_OBJECT

public:
    MainScene(QWidget *parent = nullptr);
    ~MainScene();

private:
    Ui::MainScene *ui;
    QGraphicsScene *scene;
    QGraphicsView *view;
};
#endif // MAINSCENE_H
