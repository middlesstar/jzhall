#include "CServerItem_SHZ.h"

//������Ϣ
bool SHZCServerItem::OnSocketMainInsure(int sub, void* data, int dataSize)
{
	//Ч��״̬
	ASSERT(data!=0);
	if (data==0) return true;

	//��Ϸ����
	byte *pActivityGame = (byte*)data;
	if(*pActivityGame==TRUE)
	{
		return OnSocketMainGameFrame(MDM_GR_INSURE_SHZ,sub,data,dataSize);
	}

	////��Ϣ����
	//ASSERT(m_pDlgInsureServer!=0);
	//if (m_pDlgInsureServer==0) return true;
	//if (m_pDlgInsureServer->OnServerInsureMessage(MDM_GR_INSURE_SHZ,sub,data,dataSize)==false)
	//{
	//	ASSERT(FALSE);
	//	return false;
	//}

	return true;
}
