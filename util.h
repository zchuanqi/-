#ifndef UTIL_H
#define UTIL_H
#include<QChar>
#include<QString>
#include<QTemporaryFile>
/**
 * @brief The Util class
 * 工具类，实现程序用到的辅助操作
 */
class Util
{
private:
    static void getNext(QString p,int next[]);   //得到next数组
public:
    Util();
    static QChar getFirstLetter(QString str);       //获取字符串首字母
    static void encryption(QString path);         //加密路径为path的文本文件
    static void decode(QString path,QList<QByteArray> &dataList);  //解密文件，并将内容存到dataList中
    static bool KMP(QString name,QString search);   //按姓名搜索时的匹配算法
};

#endif // UTIL_H
