#include "CServerItem_SHZ.h"

//������Ϣ
bool SHZCServerItem::OnSocketMainMatch(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//���ò�ѯ
	case SUB_GR_MATCH_FEE_SHZ:		return OnSocketSubMatchFee(data,dataSize);
		//��������
	case SUB_GR_MATCH_NUM_SHZ:		return OnSocketSubMatchNum(data,dataSize); 
		//����״̬
	case SUB_GR_MATCH_STATUS_SHZ:	return OnSocketSubMatchStatus(data, dataSize);
		//��������
	case SUB_GR_MATCH_DESC_SHZ:		return OnSocketSubMatchDesc(data, dataSize);
	}
	return true;
}

//��������
bool SHZCServerItem::OnSocketSubMatchFee(void* data, int dataSize)
{
	//����Ч��
	ASSERT(dataSize==sizeof(dword));
	if(dataSize!=sizeof(dword)) return false;

	dword dwMatchFee=*(dword*)data;
	if(dwMatchFee>0)
	{
		char szDescribe[256]={0};
		sprintf(szDescribe,a_u8("�������۳������� %ld ��Ϸ�ң�ȷ��Ҫ������"),dwMatchFee);
		PLAZZ_PRINTF("%s\n", szDescribe);
		if (mIStringMessageSink)
		{
			if (mIStringMessageSink->InsertPromptString(szDescribe, DLG_MB_YES|DLG_MB_NO)==DLG_MB_YES)
				SendSocketData(MDM_GR_MATCH_SHZ,SUB_GR_MATCH_FEE_SHZ,&dwMatchFee,sizeof(dword));
		}
	}
	return true;
}

//��������
bool SHZCServerItem::OnSocketSubMatchNum(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	mTableViewFrame.mMatchWaittingCount=packet.read4Byte();
	mTableViewFrame.mMatchTotalUser=packet.read4Byte();
	//mTableViewFrame.InvalidateRect(0);
	return true;
}

//����״̬
bool SHZCServerItem::OnSocketSubMatchStatus(void* data, int dataSize)
{
	//Ч�����
	ASSERT(dataSize==sizeof(byte));
	if(dataSize!=sizeof(byte)) return false;

	mTableViewFrame.SetMatchStatus(*(byte*)data);

	return true;
}

//��������
bool SHZCServerItem::OnSocketSubMatchDesc(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	u2string str;
	for (int i = 0; i < 4; ++i)
	{
		str.resize(64+1, '\0');
		packet.readUTF16(&str[0], 64);
		strncpy(mTableViewFrame.mMatchDesc.szTitle[i], u2_8(&str[0]), countarray(mTableViewFrame.mMatchDesc.szTitle[i]));
	}

	for (int i = 0; i < 4; ++i)
	{
		str.resize(64+1, '\0');
		packet.readUTF16(&str[0], 64);
		strncpy(mTableViewFrame.mMatchDesc.szDescribe[i], u2_8(&str[0]), countarray(mTableViewFrame.mMatchDesc.szDescribe[i]));
	}

	mTableViewFrame.mMatchDesc.crTitleColor = packet.read4Byte();
	mTableViewFrame.mMatchDesc.crDescribeColor = packet.read4Byte();

	return true;
	////����Ч��
	//ASSERT(dataSize==sizeof(CMD_GR_MatchDesc));
	//if(dataSize!=sizeof(CMD_GR_MatchDesc)) return false;

	////��������
	//CMD_GR_MatchDesc *pMatchDesc=(CMD_GR_MatchDesc*)data;

	////��ֵ
	//for(byte i=0; i<MAX_MATCH_DESC; i++)
	//{
	//	tstrcpyn(mTableViewFrame.mMatchDesc.szTitle[i], pMatchDesc->szTitle[i],countarray(pMatchDesc->szTitle[i]));
	//	tstrcpyn(mTableViewFrame.mMatchDesc.szDescribe[i], pMatchDesc->szDescribe[i],countarray(pMatchDesc->szDescribe[i]));				
	//}
	//mTableViewFrame.mMatchDesc.crTitleColor=pMatchDesc->crTitleColor;
	//mTableViewFrame.mMatchDesc.crDescribeColor=pMatchDesc->crDescribeColor;

	//return true;
}
