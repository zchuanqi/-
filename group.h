#ifndef GROUP_H
#define GROUP_H
#include<QString>
#include<QStringList>
#include"person.h"
/**
 * @brief The Group class
 * 群组类
 */
class Group
{
private:
    Person *pre;    //保存在getPerson()函数中找到的联系人的前驱结点
public:
    Group(QString name);
    Person* getPerson(QString name);       //根据姓名，得到联系人
    Person* getPerson(int index);          //根据下标，得到联系人
    QStringList getPersonNameList();    //返回联系人的姓名集合，用于控件使用
    void setGroupName(QString newName); //设置新的群组名
    bool createPerson(QString name,int age,int sex,QString phone,QString area,QString remark,PersonInTree* inTree);      //在该群组插入联系人
    bool deletePerson(QString name);       //删除联系人，name为名字
    bool deletePerson(int index);          //删除联系人，index为下标
    ~Group();                           //在析构函数中释放该群组所有联系人的空间

    QString name;       //群组名称
    Person *persons;    //存放该群组的联系人的链表
    int num;            //该群组联系人数量
    Group *next;        //指向下一个群组
};

#endif // GROUP_H
