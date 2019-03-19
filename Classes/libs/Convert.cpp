/************************************************************************/
/* http://blog.csdn.net/coollofty/article/details/8058859               */
/************************************************************************/
#include "Convert.h"

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "sdk/libiconv/include/iconv.h"
#endif

#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iconv.h"
#endif
//////////////////////////////////////////////////////////////////////////

/**
* 计算utf8字符串长度
* @param utf8 utf8字符串
* @return 字符串长度
*/
int utf8_len(const char *utf8)
{
	int i = 0, j = 0;
	while (utf8[i]) {
		if ((utf8[i] & 0xc0) != 0x80) j++;
		i++;
	}
	return j;
}

int utf8_cmp(const char* str1, const char* str2)
{
	unsigned int len1 = (unsigned int)strlen((char*)str1);
	unsigned int len2 = (unsigned int)strlen((char*)str2);
	unsigned int len = (len1<len2) ? len1 : len2;

	int ret = memcmp(str1, str2, len);
	if (ret == 0){
		if (len1 > len2) ret = 1;
		else if (len1 < len2) ret = -1;
	}
	return ret;
}

/**
* 计算ucs2字符串长度
* @param ucs2 ucs2字符串
* @return 字符串长度
*/
int ucs2_len(const unsigned short* ucs2)
{
	int i = 0;
	while (ucs2[i] != 0)
	{
		char c0 = ucs2[i] & 0xff;
		char c1 = ucs2[i] >> 8 & 0xff;

		if (c0 == '\0' && c1 == '\0')
			break;
		++i;
	}

	return i;
}

/////////////////////////////////////////////////////////////////////////////
static int code_convert(const char *from_charset, const char *to_charset, const char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	//memset(outbuf,0,outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (-1 == iconv(cd, pin, &inlen, pout, &outlen)) return -1;
	iconv_close(cd);
	return 0;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	//memset(outbuf,0,outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (-1 == iconv(cd, (char**)pin, &inlen, pout, &outlen)) return -1;
	iconv_close(cd);
	return 0;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	iconv_t cd;
	const char *temp = inbuf;
	const char **pin = &temp;
	char **pout = &outbuf;
	//memset(outbuf,0,outlen);
	cd = iconv_open(to_charset, from_charset);
	if (cd == 0) return -1;
	if (-1 == iconv(cd, (char**)pin, &inlen, pout, &outlen)) return -1;
	iconv_close(cd);
	return 0;
#endif
	return -1;
}

/*UTF8转为gbk*/
std::string u2a(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("utf-8", "gbk", inbuf, inlen, &strRet[0], strRet.size()))
		return inbuf;

	return strRet;
}

/*gbk转为UTF8*/
std::string a2u(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("gbk", "utf-8", inbuf, inlen, &strRet[0], strRet.size()))
		return inbuf;
	return strRet;
}

/*gbk转为UTF16*/
std::string a2l(const char *inbuf)
{
	size_t inlen = strlen(inbuf);
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("gbk", "UTF-16LE", inbuf, inlen, &strRet[0], strRet.size()))
		return inbuf;
	return strRet;
}


/*utf16转为UTF8*/
std::string l2u(const char *inbuf)
{
	size_t inlen = ucs2_len((const unsigned short*)&inbuf[0]);
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("UTF-16LE", "utf-8", inbuf, inlen * 2, &strRet[0], strRet.size()))
		return inbuf;

	return strRet;
}

///*UTF8转为utf16*/
//std::string u2l(const char *inbuf)
//{
//	size_t inlen = ucs2_len((const unsigned short*)&inbuf[0]);
//	std::string strRet;
//    strRet.resize(inlen * 2 + 2);
//
//    if(!code_convert("utf-8", "UTF-16LE", inbuf, inlen, &strRet[0], strRet.size()) == 0)
//    {
//       return "";
//    }
//	return strRet;
//}

/*UTF8转为utf16*/
std::string u2l(const char *inbuf)
{
	return a2l(utf8_gbk(inbuf).c_str());
}

/////////////////////////////////////////////////////////////////////////////
/**
* 将utf8字符转换成ucs2字符
* @param utf8	待转换的utf8字符串
* @param wchar	转换后的ucs2字符
* @return int 转换所需utf8字符个数
*/
inline int utf8_ucs2_one(const char* utf8, unsigned short& wchar)
{
	//首字符的Ascii码大于0xC0才需要向后判断，否则，就肯定是单个ANSI字符了
	unsigned char firstCh = utf8[0];
	if (firstCh >= 0xC0)
	{
		//根据首字符的高位判断这是几个字母的UTF8编码
		int afters, code;
		if ((firstCh & 0xE0) == 0xC0)
		{
			afters = 2;
			code = firstCh & 0x1F;
		}
		else if ((firstCh & 0xF0) == 0xE0)
		{
			afters = 3;
			code = firstCh & 0xF;
		}
		else if ((firstCh & 0xF8) == 0xF0)
		{
			afters = 4;
			code = firstCh & 0x7;
		}
		else if ((firstCh & 0xFC) == 0xF8)
		{
			afters = 5;
			code = firstCh & 0x3;
		}
		else if ((firstCh & 0xFE) == 0xFC)
		{
			afters = 6;
			code = firstCh & 0x1;
		}
		else
		{
			wchar = firstCh;
			return 1;
		}

		//知道了字节数量之后，还需要向后检查一下，如果检查失败，就简单的认为此UTF8编码有问题，或者不是UTF8编码，于是当成一个ANSI来返回处理
		for (int k = 1; k < afters; ++k)
		{
			if ((utf8[k] & 0xC0) != 0x80)
			{
				//判断失败，不符合UTF8编码的规则，直接当成一个ANSI字符返回
				wchar = firstCh;
				return 1;
			}

			code <<= 6;
			code |= (unsigned char)utf8[k] & 0x3F;
		}

		wchar = code;
		return afters;
	}
	else if (firstCh == 0 || firstCh == '\0')
	{
		wchar = '\0' << 8 | '\0';
		return 0;
	}

	wchar = firstCh;
	return 1;
}

/**
* 将utf8字符串转换为ucs2字符串
* @param utf8 待转换的utf8字符串
* @return 转换后ucs2字符串
*/
u2string utf8_ucs2(const char* utf8)
{
	u2string ret;
	int utf8len = utf8_len(utf8);

	ret.resize(utf8len * 2 + 1, '\0');

	u1string xx = u2l(utf8);
	int size = xx.size();
	memcpy(&ret[0], &xx[0], size);
	return ret;
}

/**
* 将ucs2字符转换成utf8字符
* @param wchar	待转换的ucs2字符串
* @param utf8	转换后的utf8字符数组
* @return int 转换所得utf8字符个数
*/
inline int ucs2_utf8_one(unsigned short wchar, char *utf8)
{
	if (wchar == 0)
	{
		*utf8 = '\0';
		return 0;
	}

	int len = 0;
	if (wchar < 0xC0)
	{
		utf8[len++] = (char)wchar;
	}
	else if (wchar < 0x800)
	{
		utf8[len++] = 0xc0 | (wchar >> 6);
		utf8[len++] = 0x80 | (wchar & 0x3f);
	}
	else
	{
		utf8[len++] = 0xe0 | (wchar >> 12);
		utf8[len++] = 0x80 | ((wchar >> 6) & 0x3f);
		utf8[len++] = 0x80 | (wchar & 0x3f);
	}

	return len;
}

/**
* 将ucs2字符串转换为utf8字符串
* @param ucs2 待转换的ucs2字符串
* @return 转换后utf8字符串
*/
u1string ucs2_utf8(const unsigned short *ucs2)
{
	u1string ret;
	int ucs2len = ucs2_len(ucs2);

	ret.resize(ucs2len * 4 + 1, '\0');

	int count = 0;
	for (int i = 0; i < ucs2len; ++i)
	{
		int len = ucs2_utf8_one(ucs2[i], &ret[count]);
		if (len == 0)
			break;
		count += len;
	}

	ret[count] = '\0';
	return ret;
}



/**
* 将gbk字符串转换为utf8字符串
* @param gbk 待转换的mbcs字符串
* @return 转换后utf8字符串
*/
u1string gbk_utf8(const char* gbk)
{
	return a2u(gbk);
}

/**
* 将utf8字符串转换为mbcs字符串
* @param utf8 待转换的utf8字符串
* @return 转换后mbcs字符串
*/
u1string utf8_gbk(const char* utf8)
{
	return u2a(utf8);
}

/*---------------------------------分割线------------------------------------*/
/**
* 计算utf8字符串长度
* @param utf8 utf8字符串
* @return 字符串长度
*/
int u8len(const std::string& str)
{
	int i = 0, j = 0;
	while (str[i]) {
		if ((str[i] & 0xc0) != 0x80) j++;
		i++;
	}
	return j;
}

int u8cmp(const std::string& str1, const std::string& str2)
{
	unsigned int len1 = str1.size();
	unsigned int len2 = str2.size();
	unsigned int len = (len1<len2) ? len1 : len2;

	int ret = memcmp(&str1[0], &str2[0], len);
	if (ret == 0){
		if (len1 > len2) ret = 1;
		else if (len1 < len2) ret = -1;
	}
	return ret;
}

/**
* 计算ucs2字符串长度
* @param ucs2 ucs2字符串
* @return 字符串长度
*/
int u2len(const std::string& s)
{
	const unsigned short* ucs2 = (const unsigned short*)&s[0];

	int i = 0;
	while (ucs2[i] != 0)
	{
		char c0 = ucs2[i] & 0xff;
		char c1 = ucs2[i] >> 8 & 0xff;

		if (c0 == '\0' && c1 == '\0')
			break;
		++i;
	}

	return i;
}

/*UTF8转为gbk*/
std::string u2a(const std::string& s)
{
	size_t inlen = s.size();
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("utf-8", "gbk", &s[0], inlen, &strRet[0], strRet.size()))
		return s;

	return strRet;
}

/*gbk转为UTF8*/
std::string a2u(const std::string& s)
{
	size_t inlen = s.size();
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("gbk", "utf-8", &s[0], inlen, &strRet[0], strRet.size()))
		return s;
	return strRet;
}

/*gbk转为UTF16*/
std::string a2l(const std::string& s)
{
	size_t inlen = s.size();
	std::string strRet;
	strRet.resize(inlen * 2 + 2);

	if (code_convert("gbk", "UTF-16LE", &s[0], inlen, &strRet[0], strRet.size()))
		return s;
	return strRet;
}

/*utf16转为UTF8*/
std::string l2u(const std::string& s)
{
	size_t inlen = u2len(s);
	std::string strRet;
	strRet.resize(inlen * 4 + 1, '\0');

	if (code_convert("UTF-16LE", "utf-8", &s[0], inlen * 2, &strRet[0], strRet.size()))
		return s;
	return strRet;
}

/*UTF8转为utf16*/
std::string u2l(const std::string& s)
{
	return a2l(u2a(s));
}

int u8zhlen(const std::string &s)
{
	std::string strRet = u2a(s);
	int len = strRet.size();
	short high, low;
	unsigned int code;
	std::string ss;
	int count = 0;
	for (int i = 0; i<len; i++)
	{
		if (strRet[i] == 0) {
			break;
		}
		if (strRet[i] >= 0 || i == len - 1)
		{
			count++;
		}
		else
		{
			//计算编码
			high = (short)(strRet[i] + 256);
			low = (short)(strRet[i + 1] + 256);
			code = high * 256 + low;

			//获取字符
			ss = "";
			ss += strRet[i];
			ss += strRet[i + 1];
			i++;
			count += 2;
		}
	}
	return count;

}

bool isNumber(const std::string &s)
{
	std::string strRet = u2a(s);
	int len = strRet.size();
	std::string ss;
	for (int i = 0; i<len; i++)
	{
		if (strRet[i] == 0) {
			break;
		}
		if (strRet[i] >= 0 || i == len - 1)
		{
			if ('0' <= strRet[i] && strRet[i] <= '9') {
				printf("%c >> no\n", strRet[i]);   //ASCii字符
			}
			else{

				printf("%c >> no\n", strRet[i]);
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	return true;

}

/*字母数字汉字外都是非法字符*/
bool isContainInvalidChar(const std::string &s)
{
	std::string strRet = u2a(s);
	int len = strRet.size();
	short high, low;
	unsigned int code;
	std::string ss;
	for (int i = 0; i<len; i++)
	{
		if (strRet[i] == 0) {
			break;
		}
		if (strRet[i] >= 0 || i == len - 1)
		{
			if (('a' <= strRet[i] && strRet[i] <= 'z') || ('A' <= strRet[i] && strRet[i] <= 'Z') || ('0' <= strRet[i] && strRet[i] <= '9')) {
				printf("%c >> no\n", strRet[i]);   //ASCii字符
			}
			else{

				printf("%c >> no\n", strRet[i]);
				return true;
			}
		}
		else
		{
			//计算编码
			high = (short)(strRet[i] + 256);
			low = (short)(strRet[i + 1] + 256);
			code = high * 256 + low;

			//获取字符
			ss = "";
			ss += strRet[i];
			ss += strRet[i + 1];
			i++;

			printf("%s >> 0x%x", ss.c_str(), code);
			if ((code >= 0xB0A1 && code <= 0xF7FE) || (code >= 0x8140 && code <= 0xA0FE) || (code >= 0xAA40 && code <= 0xFEA0))
			{
				printf(" yes\n");
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}