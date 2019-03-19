//各种结构体定义
//子弹结构
#ifndef SHZ__GameStruct__H_
#define SHZ__GameStruct__H_

#include "shuihu/GameHeader_SHZ.h"

//类型图片
struct SHZTypeImgStruct
{
	  //类型图
	  int m_typeid;
	  bool m_have;
	  //状态图
	  int m_index;
	  //当前图片状态,0白,1黑白
	  int m_type;
	  //行数
	  int m_Rows;
	  //列数
	  int m_Cons;
	  //是否有圈
	  bool m_havequan;
	  //循环次数
	  int  m_RoundCount;
	  //
	  bool  m_CanAction;

};
//NPC结构
struct NpcBossStruct_SHZ
{
	  //NPC状态,高兴,失落,气愤等
      int m_faceState;
	  //当前图片序列
	  int m_ImgIndex;
	  //

	  //
};
class SHZCGameSceneInterface 
{

public:

	//切换比倍界面
	virtual void OnChangeBibeiView()=0;

	//切换主界面
	virtual void OnChangeGameView(int iCreditScore)=0;

	//切换玛丽界面
	virtual void OnChangeMlView()=0;

};
#endif