#include "dominator.h"


Dominator::Dominator():MyObject(nullptr,QString(":/res/GameRes/images/muliBoki.gif"),Type::Dominator)
{
    dialog = new DialogBox(this);
    QVector<QString> btnStrs;
    btnStrs.push_back("hahahaha");
    dialog->setDialog("MuliMuli......",btnStrs);
}
void Dominator::setCurrentGif(){

}
