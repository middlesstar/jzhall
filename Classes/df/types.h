#ifndef _types_H_
#define _types_H_

//////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdio.h>
#include <string>
#include <set>
#include <map>
#include <unordered_map>
#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <deque>
#include <iomanip>
#include <list>
#include <sstream>

//#include <basetyps.h>
#include <assert.h>
#include <stdarg.h>

#ifndef ASSERT
#define ASSERT(f) assert(f)
#endif

///////////////////////////////////////////////////////////////////////////////////////

typedef signed char			int8;
typedef signed short		int16;
typedef signed int			int32;
typedef long long			int64;

typedef unsigned char		uint8;
typedef unsigned short		uint16;
typedef unsigned int		uint32;
typedef unsigned long long	uint64;

typedef unsigned char		byte;
typedef unsigned int		uint;
typedef unsigned short		word;


typedef unsigned int		dword;
typedef long long			longlong;
typedef unsigned long long	handle;
#define TRUE				1
#define FALSE				0

//typedef wchar_t				wchar;

//#ifdef  _UNICODE
//	typedef std::wstring	tstring;
//	typedef wchar			tchar;
//#else
//	typedef std::string		tstring;
//	typedef char			tchar;
//#endif

#ifdef _WIN32
#define LLSTRING "%I64d"
#define ULLSTRING "%U64d"
#else
#define LLSTRING "%lld"
#define ULLSTRING "%llu"
#endif

#define makeword(a, b)      ((word)(((byte)(((longlong)(a)) & 0xff)) | ((word)((byte)(((longlong)(b)) & 0xff))) << 8))
#define makelong(a, b)      ((long)(((word)(((longlong)(a)) & 0xffff)) | ((dword)((word)(((longlong)(b)) & 0xffff))) << 16))
#define loword(l)           ((word)(((longlong)(l)) & 0xffff))
#define hiword(l)           ((word)((((longlong)(l)) >> 16) & 0xffff))
#define lobyte(w)           ((byte)(((longlong)(w)) & 0xff))
#define hibyte(w)           ((byte)((((longlong)(w)) >> 8) & 0xff))

//////////////////////////////////////////////////////////////////////////
// 功能函数
inline std::string toString(unsigned int value)
{
	static char szTemp[256];
	sprintf(&szTemp[0], "%u", value);
	return szTemp;
}

inline std::string toString(int value)
{
	static char szTemp[256];
	sprintf(&szTemp[0], "%d", value);
	return szTemp;
}

inline std::string toString(float value)
{
	static char szTemp[256];
	sprintf(&szTemp[0], "%.1f", value);
	return szTemp;
}

inline std::string toString(int64 value)
{
	static char szTemp[256];
	sprintf(&szTemp[0], LLSTRING, value);
	return szTemp;
}

static std::string stringFormatC(const char* format, ...)
{
	static char szTemp[1024];
	va_list vlist;
	va_start(vlist, format);
	vsprintf(&szTemp[0], format, vlist);
	va_end(vlist);
	return szTemp;
} // 

#define stringFormat(format, ...) stringFormatC(format.c_str(), ##__VA_ARGS__)

template<typename T>
inline T tmin(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
inline T tmax(const T& a, const T& b)
{
	return a < b ? b : a;
}

//
//inline const std::wstring& toStringW(int value)
//{
//	static std::wstring str;
//	str.resize(128, L'\0');
//	_snwprintf(&str[0], 128, L"%d", value);
//	return str;
//}
//
//static std::string w2s(const std::wstring& ws)
//{
//	std::string curLocale = setlocale(LC_ALL, 0);        // curLocale = "C";
//	setlocale(LC_ALL, "chs");
//	size_t len = ws.size() * 2 + 1;
//	std::string result;
//	result.resize(len, '\0');
//
//	wcstombs_s(&len, &result[0], len, ws.c_str(), len);
//	setlocale(LC_ALL, curLocale.c_str());
//	return result;
//}
//
//static std::wstring s2w(const std::string& s)
//{
//	std::string curLocale = setlocale(LC_ALL, "chs"); 
//	size_t len = s.size() + 1;
//	std::wstring result;
//	result.resize(len, L'\0');
//	mbstowcs_s(&len, &result[0], len, s.c_str(),len);
//	setlocale(LC_ALL, curLocale.c_str());
//	return result;
//}

//////////////////////////////////////////////////////////////////////////
// 宏定义
#define countarray(ary)		(sizeof(ary)/sizeof(ary[0]))
#define zeromemory(x, size)	memset(x, 0, size)
//
//#ifdef  _UNICODE
//	#define t2s(text)		w2s(text)
//	#define t2w(text)		tstring(text)
//	#define s2t(text)		s2w(text)
//	#define w2t(text)		tstring(text)
//	#define T_T(str)		L##str
//	#define toString		toStringW
//
//	#define tstrcpyn(dst, src, len)	wcscpy_s(dst, len, src) 
//	#define tstrcpy(dst, src)		wcscpy(dst, src) 
//	#define tstrlen(str)			wcslen(str)
//	#define tstrcmp(str1, str2)		wcscmp(str1, str2)
//	
//#else
//	#define t2s(text)		tstring(text)
//	#define t2w(text)		s2w(text)
//	#define s2t(text)		tstring(text)
//	#define w2t(text)		s2w(text)
//	#define T_T(str)		str
//	#define toString		toStringA
//
//	#define tstrcpyn(dst, src, len)	strcpy_s(dst, len, src) 
//	#define tstrcpy(dst, src)		strcpy(dst, src) 
//	#define tstrlen(str)			strlen(str)
//	#define tstrcmp(str1, str2)		strcmp(str1, str2)
//#endif



///////////////////////////////////////////////////////////////////////////////////////

#endif // _types_H_
