#ifndef PERSON_H
#define PERSON_H
#include<QString>
#include"personintree.h"
/**
 * @brief The Person class
 * 顺序存储时使用的联系人类
 */
class Group;
class Person
{
public:
    Person(QString name,int age,int sex,QString phone,QString area,QString remark,Group *group=NULL);
    void setNewData(QString name,int age,int sex,QString phone,QString area,QString remark);

    QString name;           //联系人姓名
    int age;                //年龄
    int sex;                //性别，男1，女0
    QString phone;          //电话号码
    QString area;           //地区
    QString remark;         //备注
    Group *group;           //指向所属群组
    PersonInTree *inTree;   //指向它在通讯录的哈希表中的自己
    Person *next;           //指向下一个联系人
};
#endif // PERSON_H
