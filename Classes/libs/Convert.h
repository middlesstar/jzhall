#ifndef _Convert_H_
#define _Convert_H_
#include <vector>
#include <string>

#include "cocos2d.h"

typedef std::vector<unsigned short> u2string;
typedef std::string					u1string;

/**
* 计算utf8字符串长度
* @param utf8 utf8字符串
* @return 字符串长度
*/
int utf8_len(const char *utf8);

int utf8_cmp(const char* str1, const char* str2);

/**
* 计算ucs2字符串长度
* @param ucs2 ucs2字符串
* @return 字符串长度
*/
int ucs2_len(const unsigned short* ucs2);

/**
* 将utf8字符串转换为ucs2字符串
* @param utf8 待转换的utf8字符串
* @return 转换后ucs2字符串
*/
u2string utf8_ucs2(const char* utf8);

/**
* 将ucs2字符串转换为utf8字符串
* @param ucs2 待转换的ucs2字符串
* @return 转换后utf8字符串
*/
u1string ucs2_utf8(const unsigned short *ucs2);

/**
* 将gbk字符串转换为utf8字符串
* @param gbk 待转换的gbk字符串
* @return 转换后utf8字符串
*/
u1string gbk_utf8(const char* gbk);

/**
* 将utf8字符串转换为gbk字符串
* @param utf8 待转换的utf8字符串
* @return 转换后gbk字符串
*/
u1string utf8_gbk(const char* utf8);

//////////////////////////////////////////////////////////////////////////
// 辅助函数

#define a_u8(str)	gbk_utf8(str).c_str()
#define u8_a(str)	utf8_gbk(str).c_str()

#define u2_8(ucs2)	ucs2_utf8((const unsigned short*)ucs2).c_str()
#define u8_2(utf8)	&utf8_ucs2(utf8)[0]
#define u8_2s(utf8)	utf8_ucs2(utf8)

/*---------------------------------分割线------------------------------------*/
//加入大厅公共
/**
* 计算utf8字符串长度
* @param utf8 utf8字符串
* @return 字符串长度
*/
int u8len(const std::string& str);

int u8cmp(const std::string& str1, const std::string& str2);

/**
* 计算ucs2字符串长度
* @param ucs2 ucs2字符串
* @return 字符串长度
*/
int u2len(const std::string& s);

/*UTF8转为gbk*/
std::string u2a(const std::string& s);
/*gbk转为UTF8*/
std::string a2u(const std::string& s);
/*gbk转为UTF16*/
std::string a2l(const std::string& s);
/*utf16转为UTF8*/
std::string l2u(const std::string& s);
/*UTF8转为utf16*/
std::string u2l(const std::string& s);

/*字母数字汉字外都是非法字符*/
bool isContainInvalidChar(const std::string &s);

int u8zhlen(const std::string &s);

bool isNumber(const std::string &s);

#endif // _Convert_H_