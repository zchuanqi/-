#include "addressbook.h"
#include"util.h"
#include<qdebug.h>

AddressBook::AddressBook(QString name)
{
    this->name=name;
    groups=NULL;            //群组链表初始为空
    for(int i=0;i<27;i++){
        //头结点左孩子为link，右孩子为线索化，头结点左右孩子回指自己；左孩子之后连接树的根结点
        hashTable[i]=new PersonInTree("",0,0,"","","",NULL);        //头结点不使用信息
        hashTable[i]->lTag=0;
        hashTable[i]->rTag=1;
        hashTable[i]->rchild=hashTable[i];
        hashTable[i]->lchild=hashTable[i];
    }
    num=0;
    groupsNum=0;
    next=NULL;
}

//析构函数，释放所有群组和联系人
AddressBook::~AddressBook()
{
    //释放群组
    if(groups!=NULL){
        Group *p=groups;
        Group *q;
        while(p!=NULL){
            q=p;
            p=p->next;
            delete(q);
        }
    }
    //释放所有联系人（中序遍历一遍，释放空间）
    if(num!=0){
        for(int i=0;i<27;i++){
            //释放每一个哈希值对应的树
            PersonInTree *T=hashTable[i];
            PersonInTree *p=T->lchild;
            PersonInTree *q;
            while(p!=T){
                while(p->lTag==0)   //找到起点（最左下）
                    p=p->lchild;
                while(p->rTag==1&&p->rchild!=T){    //访问后继结点，删除当前结点
                    q=p;
                    p=p->rchild;
                    delete(q);
                }
                q=p;
                p=p->rchild;
                delete(q);
            }
            //释放头结点
            delete(T);
        }
    }
}

//返回群组
Group *AddressBook::getGroup(int index)
{
    if(index<0||index>=groupsNum){        //索引不合法
        return NULL;
    }
    else{
        Group *p=groups;
        for(int i=0;i<index;i++){
            p=p->next;
        }
        return p;
    }
}

//返回群组下标
int AddressBook::getGroupIndex(QString name)
{
    Group *p=groups;
    int index=-1;
    while(p!=NULL){
        index++;
        if(p->name.compare(name)==0)
            return index;
        p=p->next;
    }
    return -1;
}

//新建群组
bool AddressBook::createGroup(QString name)
{
    Group *aGroup=new Group(name);
    if(aGroup==NULL){                   //新建失败，返回false
        return false;
    }
    if(this->groups==NULL){                   //若当前没有群组，则作为头结点
        this->groups=aGroup;
    }
    else{
        Group *p=this->groups;
        while(p->next!=NULL){                 //插入到群组链表的尾部
            p=p->next;
        }
        p->next=aGroup;
    }
    groupsNum++;
    return true;
}

//删除群组
bool AddressBook::deleteGroup(int index)
{
    if(groups==NULL)
        return false;
    //删除的是头结点
    if(index==0){
        Group *p=groups;
        groups=groups->next;
        delete(p);
        groupsNum--;
        return true;
    }
    Group *p=getGroup(index-1);     //待删除结点前一个
    if(p==NULL)
        return false;
    Group *q=p->next;               //待删除结点
    if(q==NULL)
        return false;
    p->next=q->next;
    delete(q);
    groupsNum--;
    return true;
}

//插入联系人到哈希表
bool AddressBook::createPerson(QString name, int age,int sex, QString phone, QString area, QString remark, int groupIndex)
{
    QChar firstLetter=Util::getFirstLetter(name);            //得到姓氏首字母
    Group *group=getGroup(groupIndex);  //所属群组
    PersonInTree *aPerson=new PersonInTree(name,age,sex,phone,area,remark,group);     //创建联系人
    if(aPerson==NULL){          //创建失败
        return false;
    }
    PersonInTree *tree=NULL;
    if(firstLetter.isLetter()){ //是字母
        if(firstLetter.isUpper())
            tree=hashTable[firstLetter.unicode()-64];  //在哈希表中找到首字母对应的二叉排序树
        else
            tree=hashTable[firstLetter.toUpper().unicode()-64];
    }
    else        //不是字母
        tree=hashTable[0];       //放到第一个哈希值中
    if(tree->lchild==tree){                 //树为空，直接作为根结点（头结点左孩子）
        tree->lchild=aPerson;
    }
    else{
        PersonInTree *temp=tree->lchild;    //从根结点开始搜索
        while(temp!=NULL){
            //待插入结点小于当前结点
            if(aPerson->name.compare(temp->name)<0){
                //当前结点左孩子为空，将待插入结点作为左孩子
                if(temp->lchild==NULL||temp->lTag==1){
                    temp->lchild=aPerson;
                    temp->lTag=0;
                    break;
                }
                //当前结点左孩子不为空，则左孩子作为下一个循环的结点
                else{
                    temp=temp->lchild;
                }
            }
            //待插入结点大于当前结点
            else{
                if(aPerson->name.compare(temp->name)==0)
                    aPerson->name=aPerson->name.append('1');
                //当前结点右孩子为空，将待插入结点作为右孩子
                if(temp->rchild==NULL||temp->rTag==1){
                    temp->rchild=aPerson;
                    temp->rTag=0;
                    break;
                }
                //当前结点右孩子不为空，则右孩子作为下一个循环的结点
                else{
                    temp=temp->rchild;
                }
            }
        }
    }
    threadTreeInOrder(tree);
    num++;
    //若有分组，则在分组中也添加
    if(group!=NULL)
        group->createPerson(aPerson->name,aPerson->age,aPerson->sex,aPerson->phone,aPerson->area,aPerson->remark,aPerson);
    return true;
}

//中序线索化二叉树
void AddressBook::threadTreeInOrder(PersonInTree *T)
{
    if(T->lchild==T){       //树为空
        return;
    }
    else{
        pre=T;              //前驱初始为头结点
        InThreading(T->lchild);
        pre->rchild=T;pre->rTag=1;  //最后一个结点线索化
        T->rchild=pre;
    }
}

//中序线索化
void AddressBook::InThreading(PersonInTree *p)
{
    if(p==NULL){
        return;
    }
    if(p->lchild!=NULL&&p->lTag==0){        //线索化左孩子（可能之前已经线索化过导致lchild不为空，所以要再判断lTag是否为0）
        InThreading(p->lchild);
    }
    if(p->lchild==NULL||p->lTag==1){        //前驱线索
        p->lTag=1;
        p->lchild=pre;
    }
    if(pre->rchild==NULL||p->rTag==1){      //后继线索
        pre->rTag=1;
        pre->rchild=p;
    }
    pre=p;
    if(p->rchild!=NULL&&p->rTag==0){        //线索化右孩子
        InThreading(p->rchild);
    }
}

//根据姓名查找联系人
PersonInTree *AddressBook::searchPerson(QString name)
{
    QChar firstLetter=Util::getFirstLetter(name);
    if(firstLetter.isLetter()){
        if(firstLetter.isUpper())
            treeIndex=firstLetter.unicode()-64;
        else
            treeIndex=firstLetter.toUpper().unicode()-64;
    }
    else
        treeIndex=0;
    PersonInTree *T=hashTable[treeIndex];    //根据首字母确定在哪棵树中
    if(T->lchild==T)    //树空
        return NULL;
    PersonInTree *p=T->lchild;  //从根结点开始找
    parent=p;
    while(p!=NULL){
        //与当前结点名字相同
        if(name.compare(p->name)==0){
            return p;
        }
        //小于当前结点，到左子树找
        else if(name.compare(p->name)<0){
            if(p->lTag==0){
                parent=p;
                p=p->lchild;
            }
            else
                p=NULL;
        }
        //大于当前结点，到右子树找
        else{
            if(p->rTag==0){
                parent=p;
                p=p->rchild;
            }
            else
                p=NULL;
        }
    }
    return NULL;
}

//根据电话查找联系人
PersonInTree *AddressBook::searchPersonByPhone(QString phone)
{
    for(int i=0;i<27;i++){
        //遍历哈希表下每棵树
        PersonInTree *T=hashTable[i];       //头结点
        PersonInTree *p=T->lchild;          //根结点
        while(p!=T){
            while(p->lTag==0)
                p=p->lchild;
            while(p->rTag==1&&p->rchild!=T){
                if(phone.compare(p->phone)==0)
                    return p;
                p=p->rchild;
            }
            if(phone.compare(p->phone)==0)
                return p;
            p=p->rchild;
        }
    }
    return NULL;
}

//删除联系人
bool AddressBook::deletePerson(QString name)
{
    PersonInTree *p=searchPerson(name);     //找到待删除联系人
    //没找到
    if(p==NULL)
        return false;
    PersonInTree *tree=hashTable[treeIndex];     //该联系人所在的二叉树
    if(p->group!=NULL)                           //将该联系人从分组中也删除
        p->group->deletePerson(name);
    //删除的是叶子结点
    if(p->lTag==1&&p->rTag==1){
        //该叶子结点也是根结点时
        if(p==tree->lchild){
            delete(p);
            tree->lchild=tree;//头结点回指自己，树空了
            tree->rchild=tree;
        }
        //该叶子节点是左孩子时
        else if(p==parent->lchild){
            delete(p);
            parent->lchild=NULL;
        }
        //该叶子节点是右孩子时
        else{
            delete(p);
            parent->rchild=NULL;
        }
    }
    //删除的结点只有左孩子
    else if(p->lTag==0&&p->rTag==1){
        //同时是根结点，左孩子顶替为根
        if(p==tree->lchild){
            tree->lchild=p->lchild;
            delete(p);
        }
        //该节点是左孩子
        else if(p==parent->lchild){
            parent->lchild=p->lchild;
            delete(p);
        }
        //该节点是右孩子
        else{
            parent->rchild=p->lchild;
            delete(p);
        }
    }
    //删除的结点只有右孩子
    else if(p->lTag==1&&p->rTag==0){
        //同时是根结点，右孩子顶替成为根
        if(p==tree->lchild){
            tree->lchild=p->rchild;
            delete(p);
        }
        //该节点是左孩子
        else if(p==parent->lchild){
            parent->lchild=p->rchild;
            delete(p);
        }
        //该节点是右孩子
        else{
            parent->rchild=p->rchild;
            delete(p);
        }
    }
    //删除的结点有两个孩子
    else{
        PersonInTree *successor=getSuccessor(p);
        //待删除结点是根结点
        if(p==tree->lchild){
            tree->lchild=successor;
            delete(p);
        }
        //该结点是左孩子
        else if(p==parent->lchild){
            parent->lchild=successor;
            delete(p);
        }
        //该结点是右孩子
        else{
            parent->rchild=successor;
            delete(p);
        }
    }
    //删除完重新线索化
    threadTreeInOrder(tree);
    num--;
    return true;
}

//deNode是有两个孩子的被删除结点，该函数是找到替换它的结点，并将其替换
PersonInTree *AddressBook::getSuccessor(PersonInTree *deNode)
{
    PersonInTree *successor=deNode->rchild; //目标结点（中序后继结点），从被删除结点的右孩子开始
    PersonInTree *sucParent=NULL;           //中序后继结点的父结点
    while(successor->lTag==0){          //找到待删除结点右孩子的最左下的结点，就是中序后继结点
        sucParent=successor;
        successor=successor->lchild;
    }
    if(successor!=deNode->rchild){      //中序后继不是待删除结点的右孩子
        //将中序后继结点的父节点指向中序后继结点的右孩子
        if(successor->rTag==0)
            sucParent->lchild=successor->rchild;
        else
            sucParent->lchild=NULL;
        //用中序后继结点替换被删除结点
        successor->rchild=deNode->rchild;
        successor->lchild=deNode->lchild;
        successor->lTag=0;
        successor->rTag=0;
    }
    else{
        successor->lchild=deNode->lchild;
        successor->rchild=NULL;
        successor->lTag=0;
        successor->rTag=0;
    }
    return successor;
}

//得到群组列表
QStringList AddressBook::getGroupNameList()
{
    QStringList list;
    Group *p=groups;
    while(p!=NULL){
        list.append(p->name);
        p=p->next;
    }
    return list;
}

//得到通讯录中所有联系人姓名构成的字符串列表
QStringList AddressBook::getPersonNameList()
{
    QStringList nameList;
    //遍历整个哈希表
    for(int i=0;i<27;i++){
        //遍历第i棵二叉排序树，利用线索
        PersonInTree *T=hashTable[i];
        PersonInTree *p=T->lchild;      //从根结点开始
        while(p!=T){
            while(p->lTag==0)       //找到最左下的结点，起点
                p=p->lchild;
            while(p->rTag==1&&p->rchild!=T){    //有线索就一直往后走
                nameList.append(p->name+" "+p->phone);
                p=p->rchild;
            }
            nameList.append(p->name+" "+p->phone);
            p=p->rchild;    //没线索，从右孩子开始继续走
        }
    }
    return nameList;
}

//根据姓名或电话得到联系人列表
QStringList AddressBook::searchPersons(QString str)
{
    QStringList list;
    //遍历哈希表，将姓名或电话与str匹配的联系人加入列表
    for(int i=0;i<27;i++){
        PersonInTree *T=hashTable[i];
        PersonInTree *p=T->lchild;
        while(p!=T){
            while(p->lTag==0)
                p=p->lchild;
            while(p->rTag==1&&p->rchild!=T){
                if(Util::KMP(p->name,str)||Util::KMP(p->phone,str)){
                    list.append(p->name+" "+p->phone);
                }
                p=p->rchild;
            }
            if(Util::KMP(p->name,str)||Util::KMP(p->phone,str)){
                list.append(p->name+" "+p->phone);
            }
            p=p->rchild;
        }
    }
    return list;
}
