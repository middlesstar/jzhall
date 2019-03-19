#ifndef _CoHttp_H_
#define _CoHttp_H_

#include "curl/curl.h"
#include <string>

typedef int (*PROGRESS_HANDLE)(void* userdata, double dltotal, double dlnow, double ultotal, double ulnow);

/**
 * get 方式执行操作
 * @param url			地址
 * @param [in]rep		返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
int coHttpGet(const char* url, std::string& rep);

/**
 * post 方式执行操作
 * @param url			地址
 * @param postdata		要传输的数据 (key1=value1&key2=value2&...)
 * @param [in]rep		返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
int coHttpPost(const char* url, const char* postdata, std::string& rep);

/**
 * 下载文件
 * @param url		远程文件地址
 * @param filepath	文件路径
 * @param fnProgress	文件进度
 * @param userdata	fnProgress 的第一个参数
 * @return int		返回值(-2:创建文件失败 -1:curl初始失败, 其他:(CURLE_OK, ...)
 */
int coHttpDownload(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0);

/**
 * 上传文件
 * @param url			上传地址
 * @param filepath		文件路径
 * @param fnProgress	文件进度
 * @param userdata		fnProgress 的第一个参数
 * @return int			(-2:打开文件失败 -1:curl创建失败 其他:(CURLE_OK, ...)
 */
int coHttpUpload(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0);

#endif // _CoHttp_H_