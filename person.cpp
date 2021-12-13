#include "person.h"
#include "group.h"
Person::Person(QString name,int age,int sex,QString phone,QString area,QString remark,Group *group)
{
    this->name=name;
    this->age=age;
    this->sex=sex;
    this->phone=phone;
    this->area=area;
    this->remark=remark;
    this->group=group;
    this->inTree=NULL;
    next=NULL;
}

//设置联系人信息
void Person::setNewData(QString name, int age,int sex, QString phone, QString area, QString remark)
{
    this->name=name;
    this->age=age;
    this->sex=sex;
    this->phone=phone;
    this->area=area;
    this->remark=remark;
}
