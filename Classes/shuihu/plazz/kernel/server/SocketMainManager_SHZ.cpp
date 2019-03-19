#include "CServerItem_SHZ.h"

//管理消息
bool SHZCServerItem::OnSocketMainManager(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	case SUB_GR_OPTION_CURRENT_SHZ:	//当前配置
		{
			////效验参数
			//ASSERT(dataSize==sizeof(CMD_GR_OptionCurrent));
			//if (dataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			PLAZZ_PRINTF("%s",a_u8("TODO:CServerItem::OnSocketMainManager 未处理\n"));
			////消息处理
			//if (m_pDlgServerManager!=0)
			//{
			//	CMD_GR_OptionCurrent * pOptionCurrent=(CMD_GR_OptionCurrent *)data;
			//	m_pDlgServerManager->SetServerType(m_wServerType);
			//	m_pDlgServerManager->SetServerOptionInfo(pOptionCurrent->ServerOptionInfo,pOptionCurrent->dwRuleMask);
			//}

			return true;
		}
	}

	return true;
}
