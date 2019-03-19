#ifndef  _CustomMessage_H_
#define  _CustomMessage_H_

/*
* 此文件定义窗口消息,此文件定义的消息一般用于在服务器返回某个协议后，刷新多个窗口的场景
* 要刷新的窗口自己注册消息，以免在很多地方使用HallGameScene::updateWindow()方法
* 如果只有少量(1个)窗口要刷新，建议使用HallGameScene::updateWindow()方法
*/

#define Hall_Message_Refresh_Coin			"Hall_Message_Refresh_Coin"			//刷新金币显示
#define Hall_Message_Refresh_Gold			"Hall_Message_Refresh_Gold"			//刷新元宝显示
#define Hall_Message_Refresh_Ticket			"Hall_Message_Refresh_Ticket"		//刷新船票显示
#define Hall_Message_Refresh_NickName		"Hall_Message_Refresh_NickName"		//刷新昵称显示
#define Hall_Message_Refresh_VIP			"Hall_Message_Refresh_VIP"			//刷新vip显示

#endif // _CustomMessage_H_

