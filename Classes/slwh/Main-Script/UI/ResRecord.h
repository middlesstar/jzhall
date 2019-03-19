#ifndef __ResRecord_H__
#define __ResRecord_H__

#include "slwh/Base/UIBase.h"

#define RECORD_BOARD_MAX	8

class ResRecord :
	public UIBase
{
protected:
	std::map<int, std::string> m_map;

	struct Record
	{
		ImageView* animal = nullptr;
		ImageView* zhx = nullptr;
	};

	Record m_record[RECORD_BOARD_MAX];				//数组当做循环队列
	int m_listRear;									//循环队列尾

public:
	ResRecord();
	~ResRecord();

protected:
	void Start();

	void SetZHXTex(int index, enumGameZXH type);

public:
	//初始化记录信息接口  个位数为庄闲和  
	void InitResList(int* a);

	//添加到节点
	void Add(enumSpecialRs e1, enumAnimalColor e2, enumAnimalType e3, enumGameZXH e4);
	void Add(AnimalResult e);

	void setVisible(bool b);
	//动画属性
protected:
	const int List_AC_MAX = 50;						//动画帧
	int m_listAcIndex;								//当前动画帧
	AnimalResult m_animalRes;						//动画添加临时结果存储变量
	Vec2 m_ptEnd;									//起点坐标
	float offsetY;									//Y方向上的间距
	bool m_bIsVisible;								//是否显示
protected:
	void update(float t);

};

#endif


