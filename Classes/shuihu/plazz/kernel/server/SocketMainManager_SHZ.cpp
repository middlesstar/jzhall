#include "CServerItem_SHZ.h"

//������Ϣ
bool SHZCServerItem::OnSocketMainManager(int sub, void* data, int dataSize)
{
	switch (sub)
	{
	case SUB_GR_OPTION_CURRENT_SHZ:	//��ǰ����
		{
			////Ч�����
			//ASSERT(dataSize==sizeof(CMD_GR_OptionCurrent));
			//if (dataSize!=sizeof(CMD_GR_OptionCurrent)) return false;

			PLAZZ_PRINTF("%s",a_u8("TODO:CServerItem::OnSocketMainManager δ����\n"));
			////��Ϣ����
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
