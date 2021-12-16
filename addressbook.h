#ifndef ADDRESSBOOK_H
#define ADDRESSBOOK_H
#include"group.h"
#include"personintree.h"
#include<QStack>
#include<QChar>
#include<QString>
/**
 * @brief The AddressBook class
 * 通讯录类
 */
class AddressBook
{
private:
    void threadTreeInOrder(PersonInTree *T);    //中序线索化一棵二叉排序树
    void InThreading(PersonInTree *p);          //中序线索化
    PersonInTree* getSuccessor(PersonInTree* deNode);   //deNode是有两个孩子的被删除结点，该函数是找到替换它的结点，并将其替换

    PersonInTree *pre;      //线索化时使用的前驱结点
    PersonInTree *parent;   //保存在searchPerson()函数中查找的结点的父结点，以便找到结点后对其父节点操作
    int treeIndex;          //保存searchPerson()函数中操作的树的索引
public:
    AddressBook(QString name);
    ~AddressBook();     //析构函数中释放所包含的群组、联系人
    Group* getGroup(int index);         //返回群组，index是下标
    int getGroupIndex(QString name);         //返回群组下标，name是名字
    bool createGroup(QString name);     //新建群组
    bool deleteGroup(int index);        //删除群组
    bool createPerson(QString name,int age,int sex,QString phone,QString area,QString remark,int groupIndex=-1);   //groupIndex为其目标群组索引，没有默认-1
    bool deletePerson(QString name);    //删除联系人
    PersonInTree* searchPerson(QString name); //根据姓名查找联系人
    PersonInTree* searchPersonByPhone(QString phone);   //根据电话查找联系人
    QStringList searchPersons(QString str);     //根据姓名或电话得到联系人列表
    QStringList getGroupNameList();     //得到群组列表
    QStringList getPersonNameList();    //得到通讯录中所有联系人姓名构成的字符串列表

    QString name;           //通讯录名称
    Group *groups;          //群组的链表，存放通讯录中的群组
    PersonInTree *hashTable[27];  //存放所有联系人的哈希表,每个指针连接一棵二叉排序树（带头结点）
    int num;                //所有联系人数量
    int groupsNum;          //群组数量
    AddressBook *next;      //指向下一张通讯录
};

#endif // ADDRESSBOOK_H
