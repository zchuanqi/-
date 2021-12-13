#ifndef PERSONINTREE_H
#define PERSONINTREE_H
#include<QString>
/**
 * @brief The PersonInTree class
 * 二叉排序树中使用的联系人类
 */
class Group;
class PersonInTree
{
public:
    PersonInTree(QString name,int age,int sex,QString phone,QString area,QString remark,Group *group);
    void setNewData(QString name,int age,int sex,QString phone,QString area,QString remark);

    QString name;           //联系人姓名
    int age;                //年龄
    int sex;                //性别，男1，女0
    QString phone;          //电话号码
    QString area;           //地区
    QString remark;         //备注
    Group *group;           //指向所属群组
    PersonInTree *lchild;   //左孩子
    PersonInTree *rchild;   //右孩子
    int lTag;               //0为左孩子，1为前驱
    int rTag;               //0为右孩子，1为后继
};

#endif // PERSONINTREE_H
