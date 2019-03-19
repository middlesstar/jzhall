#ifndef SHZ__Utils_H_
#define SHZ__Utils_H_

#include <string>

//////////////////////////////////////////////////////////////////////////
typedef void (*HTTPCALLBACK)(int ret, const std::string& rep, void* userdata);

struct SHttpRequest_SHZ
{
	std::string		url;
	std::string		post;
	std::string		notify;
	HTTPCALLBACK	callback;
	void*			userdata;
	SHttpRequest_SHZ()
		: callback(0)
		, userdata(0)
	{}
};

void doHttpPost_SHZ(const SHttpRequest_SHZ& request);

int  RandomArea_SHZ(int nLen, ... );


std::string strrpl_SHZ(const std::string& src, const std::string& sMatch, const std::string& sReplace);
bool translateXML_SHZ(const char* oldKey, const char* newKey);
#endif // _Utils_H_