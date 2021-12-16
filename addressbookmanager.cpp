#include "addressbookmanager.h"
#include<qdebug.h>
#include<QTemporaryFile>
#include<QTextStream>
#include"util.h"
#include<QTextCodec>

AddressBookManager::AddressBookManager()
{
    books=NULL;
    num=0;
}

//返回通讯录
AddressBook *AddressBookManager::getAddressBook(int index)
{
    if(index<0||index>=num){
        return NULL;
    }
    else{
        AddressBook *p=books;
        for(int i=0;i<index;i++){
            p=p->next;
        }
        return p;
    }
}

//创建新的通讯录，name为名字
AddressBook* AddressBookManager::createAddressBook(QString name)
{
    AddressBook *aBook=new AddressBook(name);   //新建的通讯录
    if(aBook==NULL){                            //创建失败，返回false
        return NULL;
    }
    if(this->books==NULL){                            //如果当前没有通讯录则直接赋值给头结点
        this->books=aBook;
    }
    else{
        AddressBook *p=this->books;             //从头开始遍历到最后一个，将新通讯录插入到最后一个结点后面
        while(p->next!=NULL){
            p=p->next;
        }
        p->next=aBook;
    }
    num++;
    return aBook;                                //添加成功，返回true
}

//删除一张通讯录，index为索引，即在顶部Tab中的位置，从0开始
bool AddressBookManager::deleteAddressBook(int index)
{
    if(books==NULL)
        return false;
    //删除的是头结点
    if(index==0){
        AddressBook *p=books;
        books=books->next;
        delete(p);
        num--;
        return true;
    }
    AddressBook *p=getAddressBook(index-1);     //待删除结点前一个
    if(p==NULL)
        return false;
    AddressBook *q=p->next;     //待删除结点
    if(q==NULL)
        return false;
    p->next=q->next;
    delete(q);
    num--;
    return true;
}

//向下标为bookIndex的通讯录中添加分组
bool AddressBookManager::addGroup(QString groupName, int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)      //通讯录不存在
        return false;
    if(book->createGroup(groupName))    //在该通讯录中创建新的分组，创建成功
        return true;
    else
        return false;
}

//在下标为index的通讯录中删除下标为groupIndex的分组
bool AddressBookManager::deleteGroup(int groupIndex, int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)      //通讯录不存在
        return false;
    if(book->deleteGroup(groupIndex))   //删除成功
        return true;
    else
        return false;
}

//向下标为bookIndex的通讯录中添加联系人，groupIndex为添加到的分组，默认为无
bool AddressBookManager::addPersonToBook(QString name, int age, int sex, QString phone, QString area, QString remark, int bookIndex, int groupIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)      //通讯录不存在
        return false;
    return book->createPerson(name,age,sex,phone,area,remark,groupIndex);
}

//在下标为index的通讯录中删除名字为name的联系人
bool AddressBookManager::deletePersonFromBook(QString name, int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)  //通讯录不存在
        return false;
    return book->deletePerson(name);    //从通讯录删除
}

//将某个已存在的联系人添加到下标为groupIndex的分组中
bool AddressBookManager::addPersonToGroup(QString name, int bookIndex, int groupIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return false;
    Group *group=book->getGroup(groupIndex);
    if(group==NULL)
        return false;
    PersonInTree *person=book->searchPerson(name);  //待添加联系人
    if(person==NULL)
        return false;
    group->createPerson(person->name,person->age,person->sex,person->phone,person->area,person->remark,person);
    return true;
}

//在分组中删除联系人
bool AddressBookManager::deletePersonFromGroup(QString name, int bookIndex, int groupIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return false;
    Group *group=book->getGroup(groupIndex);
    if(group==NULL)
        return false;
    return group->deletePerson(name);
}

//根据名字查找
PersonInTree *AddressBookManager::searchPersonByName(QString name, int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return NULL;
    return book->searchPerson(name);
}

//根据电话查找
PersonInTree *AddressBookManager::searchPersonByPhone(QString phone, int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return NULL;
    return book->searchPersonByPhone(phone);
}

//得到通讯录的群组列表
QStringList AddressBookManager::getGroupList(int bookIndex)
{
    AddressBook *book=getAddressBook(bookIndex);
    QStringList list;
    if(book==NULL)
        return list;
    list=book->getGroupNameList();
    return list;
}

//得到联系人列表，groupIndex不传或不合法代表显示全部联系人
QStringList AddressBookManager::getPersonList(int bookIndex, int groupIndex)
{
    QStringList list;
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return list;
    Group *group=book->getGroup(groupIndex);
    if(group!=NULL)
        list=group->getPersonNameList();
    else
        list=book->getPersonNameList();
    return list;
}

//根据名字或电话查找联系人列表（含关键字序列的就算）
QStringList AddressBookManager::searchPerson(int bookIndex,QString str)
{
    QStringList list;
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return list;
    list=book->searchPersons(str);
    return list;
}

//保存下标为bookIndex的通讯录，path为目标文件路径
bool AddressBookManager::saveAddressBook(int bookIndex, QString path)
{
    AddressBook *book=getAddressBook(bookIndex);
    if(book==NULL)
        return false;
    QFile file(path);   //文件
    if(!file.open(QIODevice::WriteOnly|QIODevice::Text))    //只写，文本格式打开
        return false;
    QTextStream stream(&file);  //文本IO流
    //将群组写入
    stream<<book->groupsNum<<endl;  //先写入群组数量
    Group *gp=book->groups;
    while(gp!=NULL){
        stream<<gp->name<<endl;
        gp=gp->next;
    }
    //遍历哈希表，将所有联系人信息写入
    stream<<book->num<<endl;        //联系人数量
    for(int i=0;i<27;i++){
        PersonInTree *T=book->hashTable[i];
        PersonInTree *p=T->lchild;
        while(p!=T){
            while(p->lTag==0)
                p=p->lchild;
            while(p->rTag==1&&p->rchild!=T){
                //写入联系人信息
                stream<<p->name<<endl;
                stream<<p->age<<endl;
                stream<<p->sex<<endl;
                stream<<p->phone<<endl;
                stream<<p->area<<endl;
                stream<<p->remark<<endl;
                //该联系人有群组，将群组名存入
                if(p->group!=NULL)
                    stream<<p->group->name<<endl;
                //该联系人无群组，写入"$"表示没有群组
                else
                    stream<<QString("$")<<endl;
                p=p->rchild;
            }
            stream<<p->name<<endl;
            stream<<p->age<<endl;
            stream<<p->sex<<endl;
            stream<<p->phone<<endl;
            stream<<p->area<<endl;
            stream<<p->remark<<endl;
            if(p->group!=NULL)
                stream<<p->group->name<<endl;
            else
                stream<<QString("$")<<endl;
            p=p->rchild;
        }
    }
    file.close();   //关闭
    //加密
    Util::encryption(path);
    return true;
}

//导入文件
bool AddressBookManager::importAddressBook(QString path)
{
    AddressBook *book=createAddressBook(path.split('/').last());  //创建通讯录
    if(book==NULL)
        return false;
    QList<QByteArray> dataList;
    Util::decode(path,dataList);    //解密，将信息都存入dataList中
    QTextCodec *tc=QTextCodec::codecForName("GBK");
    int groupNum=dataList.at(0).toInt();    //得到群组数
    int personNum=dataList.at(1+groupNum).toInt();
    //根据文件内容构建群组
    for(int i=1;i<1+groupNum;i++){
        book->createGroup(dataList.at(i));
    }
    //根据文件内容构建联系人
    for(int i=2+groupNum;i<2+groupNum+personNum*7;i=i+7){
        QString name=tc->toUnicode(dataList.at(i));
        int age=dataList.at(i+1).toInt();
        int sex=dataList.at(i+2).toInt();
        QString phone=tc->toUnicode(dataList.at(i+3));
        QString area=tc->toUnicode(dataList.at(i+4));
        QString remark=tc->toUnicode(dataList.at(i+5));
        QString groupName=tc->toUnicode(dataList.at(i+6));
        book->createPerson(name,age,sex,phone,area,remark,book->getGroupIndex(groupName));
    }
    return true;
}
