#include "dabHelper.h"

DABHelper::DABHelper(QObject *parent) : QObject(parent)
{

}

void DABHelper::changeMousePos(int x, int y){
    QPoint curent = QCursor::pos();
    QCursor::setPos(curent.x()+x,curent.y()+y);
}
