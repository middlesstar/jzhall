#ifndef  _RecordManager_H_
#define  _RecordManager_H_

#include "shark/Base/ViewBase.h"
#include "OneOfRecord.h"


class  RecordManager : public ViewBase
{
public:
	RecordManager();
	virtual ~RecordManager();

protected:
	void setContent();
public:
	void updateBoard(int code = 0);            //更新展示板

	void judgePos(int pos);

protected:
	OneOfRecord 	m_oneofRecord[12];
	Node*           pMainNode;
private:
	int  newNum;                         //记录新一局展示板动物的位置
};

#endif 
