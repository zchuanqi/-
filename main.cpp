#include "widget.h"
#include"addressbookmanager.h"
#include <QApplication>
#include<QDebug>
#include<QTemporaryFile>
#include<QFileInfo>
#include"util.h"

void printPerson(PersonInTree *p)
{
    qDebug()<<p->name<<";"<<p->age<<";"<<p->sex<<";"<<p->area<<";"<<p->phone;
    if(p->group!=NULL)
        qDebug()<<p->group->name;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    AddressBookManager manager;
//    manager.createAddressBook("book1");
//    manager.createAddressBook("book2");
//    manager.addGroup("群组1",0);
//    manager.addGroup("group2",1);
//    qDebug()<<manager.addPersonToBook("hhhhhhhhh",19,1,"46516518","ssss","sss",0,0);
//    qDebug()<<manager.addPersonToBook("hha",19,1,"18203908985","河南济源","无",0,0);
//    qDebug()<<manager.addPersonToBook("赵彦翔1111",19,1,"18203908985","河南济源","无",0,0);
//    qDebug()<<manager.addPersonToBook("肖炜力",19,1,"123456789","福建莆田","无",0,0);
//    qDebug()<<manager.addPersonToBook("叶子",19,0,"12346789","重庆","无",0);
//    qDebug()<<manager.addPersonToBook("kzh",19,1,"7541561","河南济源","无",0);
//    qDebug()<<manager.addPersonToBook("222",19,1,"18203908985","河南济源","无",0);
//    manager.saveAddressBook(0,"D:/test");
    qDebug()<<"import test:"<<manager.importAddressBook("D:/test");
    qDebug()<<manager.getPersonList(manager.num-1);
    qDebug()<<manager.getAddressBook(manager.num-1)->name;
    qDebug()<<manager.getGroupList(manager.num-1);

    return a.exec();
}
