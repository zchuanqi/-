#ifndef UTIL_H
#define UTIL_H
#include<QChar>
#include<QString>
/**
 * @brief The Util class
 * 工具类，实现程序用到的辅助操作
 */
class Util
{
public:
    Util();
    static QChar getFirstLetter(QString str);       //获取字符串首字母
    static QString encryption(QString str);         //对str加密，返回加密后的字符串
    static QString decode(QString str);             //对str解密，返回解密后的字符串
};

#endif // UTIL_H
