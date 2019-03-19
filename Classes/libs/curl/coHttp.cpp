#include "CoHttp.h"
#include "cocos2d.h"
#include "curl/curl.h"
#define CURL_PRINT cocos2d::log

#ifdef _WIN32
#define LLSTRING "%I64d"
#else
#define LLSTRING "%lld"
#endif

class CurlGloablInit
{
public:
	CurlGloablInit()
	{

		//CURL_PRINT("CurlGloablInit");
		curl_global_init(CURL_GLOBAL_ALL);
		//CURL_PRINT("curl xxx:%s", curl_version());
	}
	~CurlGloablInit()
	{
		//CURL_PRINT("!CurlGloablInit");
		curl_global_cleanup();
	}
}; // CurlGloablInit

CurlGloablInit __gCurlGloablInit;

struct ProgressData
{
	long long file_size;
	long long file_now_size;

	void* userdata;
	PROGRESS_HANDLE fnProgress;
};

//////////////////////////////////////////////////////////////////////////
// get

/**
 * 写数据回调
 * @param ptr		数据地址
 * @param size		数据大小
 * @param nmemb		数据个数
 * @param userdata	用户数据 (通过 CURLOPT_WRITEDATA 传入数据)
 * @return size_t	返回写入数据大小
 *   如果回调函数返回的数据长度与传递给它的长度不一致
 *  （即返回长度 != size * nmemb）
 *   libcurl将会终止操作，并返回一个错误代码。
 */
static size_t get_write(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	std::string* pstr = (std::string*)userdata;
	pstr->insert(pstr->size(), (char*)ptr);
	return size*nmemb;
} // get_write

/**
 * get 方式执行操作
 * @param url			地址
 * @param [in]string	返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
static int doGet(const char* url, std::string& ret)
{
	CURL* curl = curl_easy_init();
	
	if (curl == 0)
	{
		CURL_PRINT("[doGet] curl_easy_init failed");
		return -1;
	}
	// url
	curl_easy_setopt(curl, CURLOPT_URL, url);
	// 开启跟随重定向
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// 写入处理
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, get_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
	//查看通信过程,调试
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[doGet] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // doGet


//////////////////////////////////////////////////////////////////////////
// post

/**
 * 写数据回调
 * @param ptr		数据地址
 * @param size		数据大小
 * @param nmemb		数据个数
 * @param userdata	用户数据 (通过 CURLOPT_WRITEDATA 传入数据)
 * @return size_t	返回写入数据大小
 *   如果回调函数返回的数据长度与传递给它的长度不一致
 *  （即返回长度 != size * nmemb）
 *   libcurl将会终止操作，并返回一个错误代码。
 */
static size_t post_write(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	std::string* pstr = (std::string*)userdata;
	pstr->insert(pstr->size(), (char*)ptr);
	return size*nmemb;
} // post_write

/**
 * post 方式执行操作
 * @param url			地址
 * @param postdata		要传输的数据 (key1=value1&key2=value2&...)
 * @param [in]string	返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
static int doPost(const char* url, const char* postdata, std::string& ret)
{
	CURL* curl = curl_easy_init();

	if (curl == 0)
	{
		CURL_PRINT("[doPost] curl_easy_init failed");
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, url);
	
	// post方式
	curl_easy_setopt(curl, CURLOPT_POST, true); 
	// 发送的post数据
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS,postdata);
	
	// 开启跟随重定向
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// 写入处理
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, post_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ret);
	// 这个选项很常用 用来在屏幕上显示对服务器相关操作返回的信息 
	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1); 
	//执行操作
	CURLcode res = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[doPost] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // doPost

//////////////////////////////////////////////////////////////////////////
// 下载文件

/**
 * 获取下载文件大小
 */
long long download_get_file_length(const char* url)
{
	double file_length = -1;

	CURL* curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url);	
	curl_easy_setopt(curl, CURLOPT_HEADER, 1);	//只需要header头
	curl_easy_setopt(curl, CURLOPT_NOBODY, 1);	//不要body
	// 开启跟随重定向
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

	if (curl_easy_perform(curl) == CURLE_OK)
		curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &file_length);
	
	curl_easy_cleanup(curl);

	return (long long)file_length;
}

/**
 * 写数据回调
 * @param ptr		数据地址
 * @param size		数据大小
 * @param nmemb		数据个数
 * @param userdata	用户数据 (通过 CURLOPT_WRITEDATA 传入数据)
 * @return size_t	返回写入数据大小
 *   如果回调函数返回的数据长度与传递给它的长度不一致
 *  （即返回长度 != size * nmemb）
 *   libcurl将会终止操作，并返回一个错误代码。
 */
static size_t download_write(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	return fwrite(ptr, size, nmemb, (FILE*)userdata);
} // download_write

/**
 * 进度回调
 * @param userdata	用户数据 (通过 CURLOPT_PROGRESSDATA 传入数据)
 * @param dltotal	下载数据总大小
 * @param dlnow		下载当前大小
 * @param ultotal	上传数据总大小
 * @param ulnow		上传当前大小
 * @return int		
 */
static int download_progress(void* userdata, double dltotal, double dlnow, double ultotal, double ulnow)
{
	ProgressData* data = (ProgressData*)userdata;
	return data->fnProgress(data->userdata, dltotal+data->file_now_size, dlnow+data->file_now_size, ultotal, ulnow);
} // download_progress

/**
 * 下载文件
 * @param url		远程文件地址
 * @param filepath	文件路径
 * @param PROGRESS	文件进度
 * @param userdata	传入PROGRESS
 * @return int		返回值(-2:创建文件失败 -1:curl初始失败, 其他:(CURLE_OK, ...)
 */
static int download(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0)
{
	// 获取限制文件大小
	long long file_size = download_get_file_length(url);
	CURL_PRINT("[download] download file size:" LLSTRING, file_size);

	FILE* file = fopen(filepath, "ab+");
	
	if (file == 0)
	{
		CURL_PRINT("[download] open file failed:%s", filepath);
		return -2;
	}

	// 文件大小
	fseek(file, 0, SEEK_END);
	curl_off_t local_file_size = ftell(file);
	rewind(file);
	CURL_PRINT("[download] download local file size:" LLSTRING, local_file_size);

	if (local_file_size >= file_size)
	{
		fclose(file);
		CURL_PRINT("[download] already complete");
		return CURLE_OK;
	}

	CURL* curl = curl_easy_init();

	if (curl == 0)
	{
		fclose(file);
		CURL_PRINT("[download] curl_easy_init failed");
		return -1;
	}
	
	//访问地址
	curl_easy_setopt(curl, CURLOPT_URL, url);
	// 写数据
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, download_write);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
	
	// 开启跟随重定向
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// 设置文件续传的位置给libcurl
	curl_easy_setopt(curl, CURLOPT_RESUME_FROM_LARGE, local_file_size);
	
	// 文件进度
	ProgressData data;
	data.file_now_size	= local_file_size;
	data.file_size		= file_size;
	data.userdata		= userdata;
	data.fnProgress		= fnProgress;
	if (fnProgress != 0)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION, download_progress);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data); //数据
	}

	//执行操作
	CURLcode res = curl_easy_perform(curl);
	//执行完清理
	curl_easy_cleanup(curl);
	//关闭文件
	fclose(file);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[download] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // download

//////////////////////////////////////////////////////////////////////////
// 上传文件

/**
 * 写数据回调
 * @param ptr		数据地址
 * @param size		数据大小
 * @param nmemb		数据个数
 * @param userdata	用户数据 (通过 CURLOPT_WRITEDATA 传入数据)
 * @return size_t	返回写入数据大小
 *   如果回调函数返回的数据长度与传递给它的长度不一致
 *  （即返回长度 != size * nmemb）
 *   libcurl将会终止操作，并返回一个错误代码。
 */
static size_t upload_read(void* ptr, size_t size, size_t nmemb, void* userdata)
{
	return fread(ptr, size, nmemb, (FILE*)userdata);
} // upload_read

/**
 * 进度回调
 * @param userdata	用户数据 (通过 CURLOPT_PROGRESSDATA 传入数据)
 * @param dltotal	下载数据总大小
 * @param dlnow		下载当前大小
 * @param ultotal	上传数据总大小
 * @param ulnow		上传当前大小
 * @return int		
 */
static int upload_progress(void* userdata, double dltotal, double dlnow, double ultotal, double ulnow)
{
	ProgressData* data = (ProgressData*)userdata;
	return data->fnProgress(data->userdata, dltotal, dlnow, ultotal+data->file_now_size, ulnow+data->file_now_size);
} // upload_progress


static int upload_by_put(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0)
{
	FILE* file = fopen(filepath, "rb");

	if (file == 0)
	{
		CURL_PRINT("[upload] open file failed:%s", filepath);
		return -2;
	}

	// 文件大小
	fseek(file, 0, SEEK_END);
	long long filesize = ftell(file);
	rewind(file);

	CURL* curl = curl_easy_init();

	if (curl == 0)
	{
		fclose(file);
		CURL_PRINT("[upload] curl_easy_init failed");
		return -1;
	}

	//访问地址
	curl_easy_setopt(curl, CURLOPT_URL, url);
	//设置PUT标志
	curl_easy_setopt(curl, CURLOPT_PUT, 1L);
	//设置上传标志
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	//设置上传文件大小
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, filesize);
	//设置处理函数
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload_read);
	curl_easy_setopt(curl, CURLOPT_READDATA, file);
	// 文件进度
	ProgressData data;
	data.file_now_size	= 0;
	data.file_size		= filesize;
	data.userdata		= userdata;
	data.fnProgress		= fnProgress;
	if (fnProgress != 0)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION, upload_progress);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data); //数据
	}

	//执行操作
	CURLcode res = curl_easy_perform(curl);
	//执行完清理
	curl_easy_cleanup(curl);

	//关闭文件
	fclose(file);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[upload] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // upload_by_put


static int upload_by_formdata(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0)
{
	FILE* file = fopen(filepath, "rb");

	if (file == 0)
	{
		CURL_PRINT("[upload] open file failed:%s", filepath);
		return -2;
	}

	// 文件大小
	fseek(file, 0, SEEK_END);
	long long filesize = ftell(file);
	rewind(file);
	fclose(file);

	CURL* curl = curl_easy_init();

	if (curl == 0)
	{
		CURL_PRINT("[upload_formdata] curl_easy_init failed");
		return -1;
	}

	//访问地址
	curl_easy_setopt(curl, CURLOPT_URL, url);
	// 开启跟随重定向
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	// 文件进度
	ProgressData data;
	data.file_now_size	= 0;
	data.file_size		= filesize;
	data.userdata		= userdata;
	data.fnProgress		= fnProgress;
	if (fnProgress != 0)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION, upload_progress);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data); //数据
	}

	// 构造httppost
	struct curl_httppost* formpost = 0;
	struct curl_httppost* lastptr = 0;
	curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "reqformat", CURLFORM_PTRCONTENTS, "plain", CURLFORM_END);
	curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "file", CURLFORM_FILE, filepath, CURLFORM_END);
	curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

	//执行操作
	CURLcode res = curl_easy_perform(curl);
	//执行完清理
	curl_formfree(formpost);
	curl_easy_cleanup(curl);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[upload] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // upload_by_formdata


static int upload(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress = 0, void* userdata = 0)
{
	// 不成功(要改服务端???)
	//return upload_by_put(url, filepath, fnProgress, userdata);		
	return upload_by_formdata(url, filepath, fnProgress, userdata);

	// 不成功(要改服务端???)
	FILE* file = fopen(filepath, "rb");

	if (file == 0)
	{
		CURL_PRINT("[upload] open file failed:%s", filepath);
		return -2;
	}

	// 文件大小
	fseek(file, 0, SEEK_END);
	long long filesize = ftell(file);
	rewind(file);

	CURL* curl = curl_easy_init();

	if (curl == 0)
	{
		fclose(file);
		CURL_PRINT("[upload] curl_easy_init failed");
		return -1;
	}

	//访问地址
	curl_easy_setopt(curl, CURLOPT_URL, url);
	//设置上传标志
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
	//设置上传文件大小
	curl_easy_setopt(curl, CURLOPT_INFILESIZE_LARGE, filesize);
	//设置处理函数
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, upload_read);
	curl_easy_setopt(curl, CURLOPT_READDATA, file);
	
	// 文件进度
	ProgressData data;
	data.file_now_size	= 0;
	data.file_size		= filesize;
	data.userdata		= userdata;
	data.fnProgress		= fnProgress;
	if (fnProgress != 0)
	{
		curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
		curl_easy_setopt(curl,CURLOPT_PROGRESSFUNCTION, upload_progress);
		curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &data); //数据
	}
	//执行操作
	CURLcode res = curl_easy_perform(curl);
	//执行完清理
	curl_easy_cleanup(curl);
	
	//关闭文件
	fclose(file);

	if (res != CURLE_OK)
	{
		CURL_PRINT("[upload] curl error:[%i] %s ", res, curl_easy_strerror(res));
	}

	return res;
} // upload

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * get 方式执行操作
 * @param url			地址
 * @param [in]rep		返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
int coHttpGet(const char* url, std::string& rep)
{
	return doGet(url, rep);
} // coHttpGet

/**
 * post 方式执行操作
 * @param url			地址
 * @param postdata		要传输的数据 (key1=value1&key2=value2&...)
 * @param [in]rep		返回数据
 * @return int			操作结果(CURLE_OK, ...)
 */
int coHttpPost(const char* url, const char* postdata, std::string& rep)
{
	return doPost(url, postdata, rep);
} // coHttpPost

/**
 * 下载文件
 * @param url		远程文件地址
 * @param filepath	文件路径
 * @param fnProgress	文件进度
 * @param userdata	fnProgress 的第一个参数
 * @return int		返回值(-2:创建文件失败 -1:curl初始失败, 其他:(CURLE_OK, ...)
 */
int coHttpDownload(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress, void* userdata)
{
	return download(url, filepath, fnProgress, userdata);
} // coHttpDownload

/**
 * 上传文件
 * @param url			上传地址
 * @param filepath		文件路径
 * @param fnProgress	文件进度
 * @param userdata		fnProgress 的第一个参数
 * @return int			(-2:打开文件失败 -1:curl创建失败 其他:(CURLE_OK, ...)
 */
int coHttpUpload(const char* url, const char* filepath, PROGRESS_HANDLE fnProgress, void* userdata)
{
	return upload(url, filepath, fnProgress, userdata);
} // coHttpUpload
