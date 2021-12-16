#ifndef ADDRESSBOOKMANAGER_H
#define ADDRESSBOOKMANAGER_H
#include"addressbook.h"
/**
 * @brief The AddressBookManager class
 * 用来管理所有通讯录的类
 */
class AddressBookManager
{
public:
    AddressBookManager();
    AddressBook* getAddressBook(int index);    //根据索引返回通讯录
    AddressBook* createAddressBook(QString name);      //创建新的通讯录，name为名字
    bool deleteAddressBook(int index);         //删除一张通讯录，index为索引，即在顶部Tab中的位置，从0开始
    bool addGroup(QString groupName,int bookIndex);     //向下标为bookIndex的通讯录中添加分组
    bool deleteGroup(int groupIndex,int bookIndex);     //在下标为bookIndex的通讯录中删除下标为groupIndex的分组
    //向下标为bookIndex的通讯录中下标为groupIndex的分组中添加联系人；groupIndex可不填，默认没有分组
    bool addPersonToBook(QString name,int age,int sex,QString phone,QString area,QString remark,int bookIndex,int groupIndex=-1);
    bool deletePersonFromBook(QString name,int bookIndex);  //在下标为index的通讯录中删除名字为name的联系人
    bool addPersonToGroup(QString name,int bookIndex,int groupIndex);   //将某个已存在的联系人添加到下标为groupIndex的分组中
    bool deletePersonFromGroup(QString name,int bookIndex,int groupIndex);  //在分组中删除联系人
    PersonInTree* searchPersonByName(QString name,int bookIndex);     //根据名字查找联系人
    PersonInTree* searchPersonByPhone(QString phone,int bookIndex);   //根据电话查找联系人
    QStringList getGroupList(int bookIndex);    //得到下标为bookIndex的通讯录的群组列表
    QStringList getPersonList(int bookIndex,int groupIndex=-1); //得到联系人列表，不传groupIndex则显示所有联系人
    QStringList searchPerson(int bookIndex,QString str);  //根据名字或电话查找联系人列表（含关键字序列的就算）
    bool saveAddressBook(int bookIndex,QString path);           //保存下标为bookIndex的通讯录，path为目标文件路径
    bool importAddressBook(QString path);       //导入路径path的文件

    AddressBook *books;     //程序中目前存在的通讯录链表
    int num;                //通讯录数量
};

#endif // ADDRESSBOOKMANAGER_H
