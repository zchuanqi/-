#include "group.h"

Group::Group(QString name)
{
    this->name=name;
    persons=NULL;   //联系人链表初始为空
    num=0;
    next=NULL;
}

//析构函数中释放该群组所有联系人的空间
Group::~Group()
{
    if(persons!=NULL){
        Person *p=persons;
        Person *q;
        while(p!=NULL){
            q=p;
            p=p->next;
            q->inTree->group=NULL;      //让通讯录中对应的该联系人分组为空
            delete(q);
        }
    }
}

//返回联系人
Person *Group::getPerson(QString name)
{
    Person *p=persons;
    pre=p;
    while(p!=NULL){
        if(p->name.compare(name)==0){
            return p;
        }
        pre=p;
        p=p->next;
    }
    return NULL;
}

//返回联系人，index为下标
Person *Group::getPerson(int index)
{
    if(index<0||index>=num)
        return NULL;
    Person *p=persons;
    for(int i=0;i<index;i++)
        p=p->next;
    return p;
}

//返回该群组的联系人姓名集合
QStringList Group::getPersonNameList()
{
    QStringList nameList;
    Person *p=persons;
    while(p!=NULL){
        nameList.append(p->name);
        p=p->next;
    }
    return nameList;
}

//设置群组名
void Group::setGroupName(QString newName)
{
    this->name=newName;
}

//在该群组插入联系人
bool Group::createPerson(QString name, int age,int sex, QString phone, QString area, QString remark,PersonInTree* inTree)
{
    Person *aPerson=new Person(name,age,sex,phone,area,remark,this);
    aPerson->inTree=inTree;
    if(aPerson==NULL){
        return false;
    }
    if(persons==NULL){                      //头结点为空，直接作为头结点
        persons=aPerson;
    }
    else{
        Person *p=persons;                              //从头结点开始
        Person *pre=p;    //用来记录当前结点的前驱
        while((p->name.compare(aPerson->name)<0)&&p!=NULL){     //当前结点小于待插入结点时继续后移（关键字是name）
            pre=p;
            p=p->next;
        }
        if(pre==p){                 //pre等于p说明第一个结点就＞＝待插入结点，那么将待插入结点作为头结点
            persons=aPerson;
            persons->next=p;
        }
        else{
            aPerson->next=p;        //将待插入结点插入到pre结点后
            pre->next=aPerson;
        }
    }
    num++;
    return true;
}

//删除联系人，name为名字
bool Group::deletePerson(QString name)
{
    if(persons==NULL)
        return false;
    Person *p=getPerson(name);
    //不存在
    if(p==NULL)
        return false;
    //删除的是头结点
    if(pre==p){
        persons=persons->next;
        delete(p);
        num--;
        return true;
    }
    //是其他结点
    pre->next=p->next;
    delete(p);
    num--;
    return true;
}

//删除联系人，index为下标
bool Group::deletePerson(int index)
{
    if(num<=0)
        return false;
    //删头结点
    if(index==0){
        Person *p=persons;
        persons=persons->next;
        num--;
        return true;
    }
    //其他结点
    Person *p=getPerson(index-1);   //待删除结点前一个结点
    if(p==NULL)
        return false;
    Person *q=p->next;      //待删除结点
    if(q==NULL)
        return false;
    p->next=q->next;
    delete(q);
    num--;
    return true;
}
