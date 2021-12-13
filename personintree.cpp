#include "personintree.h"
#include"group.h"
PersonInTree::PersonInTree(QString name,int age,int sex,QString phone,QString area,QString remark,Group *group)
{
    this->name=name;
    this->age=age;
    this->sex=sex;
    this->phone=phone;
    this->area=area;
    this->remark=remark;
    this->group=group;
    lchild=NULL;
    rchild=NULL;
    lTag=0;
    rTag=0;
}

//设置联系人信息
void PersonInTree::setNewData(QString name, int age, int sex, QString phone, QString area, QString remark)
{
    this->name=name;
    this->age=age;
    this->sex=sex;
    this->phone=phone;
    this->area=area;
    this->remark=remark;
}
