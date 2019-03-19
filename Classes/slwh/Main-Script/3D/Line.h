#ifndef __Line_H__
#define __Line_H__

#include "slwh/Base/ModelBase.h"
class Line :
	public ModelBase
{
private:
	float m_time;
	std::map<int, std::string> m_tex;
public:
	Line();
	~Line();
protected:
	void Start();
	void update(float delta);
};

#endif

