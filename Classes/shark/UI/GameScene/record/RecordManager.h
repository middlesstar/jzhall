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
	void updateBoard(int code = 0);            //����չʾ��

	void judgePos(int pos);

protected:
	OneOfRecord 	m_oneofRecord[12];
	Node*           pMainNode;
private:
	int  newNum;                         //��¼��һ��չʾ�嶯���λ��
};

#endif 
