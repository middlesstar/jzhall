#ifndef  _WindowDefine_H_
#define  _WindowDefine_H_

//此文件定义所有窗口编号
enum eWindowType
{
	EWT_Login = 0,				//登录
	EWT_LoginAccount,			//账号登录
	EWT_Service,				//客服
	EWT_Hall,					//大厅

	EWT_Shop,					//商城
	EWT_MoneyTree,				//摇钱树
	EWT_FreeMoney,				//免费金币
	EWT_Rank,					//排行榜
	EWT_Promote,				//推广赚金
	EWT_Exchange,				//兑奖
	EWT_VipGift,				//VIP礼包
	EWT_Bank,					//银行
	EWT_Mail,					//邮件
	EWT_Setting,				//设置

	EWT_Personal,				//个人设置
	EWT_TicketSend,				//赠送船票
	EWT_Sign,					//每日挑战
	EWT_DialyPrize,				//每日抽奖
	EWT_WeiXin,					//微信公众号
	EWT_Comment,				//好评有礼
	EWT_Dole,					//救济金
	EWT_Expedition,				//远征
	EWT_Activity,				//活动

	EWT_Share,					//分享界面
	EWT_TreeUpgrade,			//摇钱树升级
	EWT_Chat,					//聊天界面
	EWT_SendRedPacket,			//聊天发红包界面
	EWT_PacketResult,			//聊天发红包结果界面

	//todosl
	EWT_Update,                //更新界面

	EWT_MAX
};

#endif // _WindowDefine_H_

