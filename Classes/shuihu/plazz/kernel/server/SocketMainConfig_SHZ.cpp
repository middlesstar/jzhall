#include "CServerItem_SHZ.h"

//������Ϣ
bool SHZCServerItem::OnSocketMainConfig(int sub, void* data, int dataSize)
{
	switch (sub)
	{
		//�б�����
	case SUB_GR_CONFIG_COLUMN_SHZ:		return OnSocketSubConfigColumn(data, dataSize);
		//��������
	case SUB_GR_CONFIG_SERVER_SHZ:		return OnSocketSubConfigServer(data, dataSize);
		//��������
	case SUB_GR_CONFIG_PROPERTY_SHZ:	return OnSocketSubConfigProperty(data, dataSize);
		//�������Ȩ��	
	case SUB_GR_CONFIG_USER_RIGHT_SHZ:	return OnSocketSubConfigUserRight(data, dataSize);
		//�������
	case SUB_GR_CONFIG_FINISH_SHZ:		return OnSocketSubConfigFinish(data, dataSize);	
	}

	//�������
	ASSERT(FALSE);
	return true;
}

//�б�����
bool SHZCServerItem::OnSocketSubConfigColumn(void* data, int dataSize)
{
	//��������
	PACKET_AIDE_DATA(data);
	byte cbColumnCount = packet.readByte();

	//u2string str;

	//for (int i = 0; i < cbColumnCount; ++i)
	//{
	//	tagColumnItem ColumnItem;
	//	ColumnItem.cbColumnWidth = packet.readByte();
	//	ColumnItem.cbDataDescribe = packet.readByte();
	//	str.resize(16+1,'\0');
	//	packet.readUTF16(&str[0], 16);
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigColumn();

	return true;

	////��������
	//CMD_GR_ConfigColumn * pConfigColumn=(CMD_GR_ConfigColumn *)data;
	//word wHeadSize=sizeof(CMD_GR_ConfigColumn)-sizeof(pConfigColumn->ColumnItem);

	////Ч�����
	//ASSERT((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))==dataSize);
	//if ((wHeadSize+pConfigColumn->cbColumnCount*sizeof(pConfigColumn->ColumnItem[0]))!=dataSize) return false;

	//////������Ϣ
	////m_cbColumnCount=__min(pConfigColumn->cbColumnCount,CountArray(m_ColumnItem));
	////memcpy(m_ColumnItem,pConfigColumn->ColumnItem,sizeof(tagColumnItem)*m_cbColumnCount);

	//////�����б�
	////m_UserListControl.SetColumnDescribe(pConfigColumn->ColumnItem,pConfigColumn->cbColumnCount);
	//
	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRConfigColumn();

	//return true;
}

//��������
bool SHZCServerItem::OnSocketSubConfigServer(void* data, int dataSize)
{
	//Ч������
	ASSERT(dataSize==10);
	if (dataSize<10) return false;

	PACKET_AIDE_DATA(data);

	//��������
	mServerAttribute.wTableCount=packet.read2Byte();
	mServerAttribute.wChairCount=packet.read2Byte();
	mServerAttribute.wServerType=packet.read2Byte();
	mServerAttribute.dwServerRule=packet.read4Byte();
	//m_UserListControl.SetServerRule(m_dwServerRule);

	if (!mTableViewFrame.ConfigTableFrame(
		mServerAttribute.wTableCount,
		mServerAttribute.wChairCount,
		mServerAttribute.dwServerRule,
		mServerAttribute.wServerType,
		mServerAttribute.wServerID))
	{
		IntermitConnect(false);
		return false;
	}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigServer();

	return true;

	////Ч������
	//ASSERT(dataSize==sizeof(CMD_GR_ConfigServer));
	//if (dataSize<sizeof(CMD_GR_ConfigServer)) return false;

	////��Ϣ����
	//CMD_GR_ConfigServer * pConfigServer=(CMD_GR_ConfigServer *)data;

	////��������
	//mServerAttribute.wServerType=pConfigServer->wServerType;
	//mServerAttribute.dwServerRule=pConfigServer->dwServerRule;
	//mServerAttribute.wTableCount=pConfigServer->wTableCount;
	//mServerAttribute.wChairCount=pConfigServer->wChairCount;
	////m_UserListControl.SetServerRule(m_dwServerRule);
	//
	//if (!mTableViewFrame.ConfigTableFrame(
	//	mServerAttribute.wTableCount,
	//	mServerAttribute.wChairCount,
	//	mServerAttribute.dwServerRule,
	//	mServerAttribute.wServerType,
	//	mServerAttribute.wServerID))
	//{
	//	IntermitConnect(false);
	//	return false;
	//}

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRConfigServer();

	//return true;
}

//��������
bool SHZCServerItem::OnSocketSubConfigProperty(void* data, int dataSize)
{
	////��������
	//PACKET_AIDE_DATA(data);
	//byte cbPropertyCount=packet.readByte();

	//for (int i = 0; i < cbPropertyCount; ++i)
	//{
	//	tagPropertyInfo PropertyInfo;
	//	PropertyInfo.wIndex = packet.read2Byte();
	//	PropertyInfo.wDiscount = packet.read2Byte();
	//	PropertyInfo.wIssueArea = packet.read2Byte();
	//	PropertyInfo.lPropertyGold = packet.read8Byte();
	//	PropertyInfo.dPropertyCash = packet.read8Byte();
	//	PropertyInfo.lSendLoveLiness = packet.read8Byte();
	//	PropertyInfo.lRecvLoveLiness = packet.read8Byte();
	//}

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigProperty();

	return true;

	////��������
	//CMD_GR_ConfigProperty * pConfigProperty=(CMD_GR_ConfigProperty *)data;
	//word wHeadSize=sizeof(CMD_GR_ConfigProperty)-sizeof(pConfigProperty->PropertyInfo);

	////Ч�����
	//ASSERT((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))==dataSize);
	//if ((wHeadSize+pConfigProperty->cbPropertyCount*sizeof(pConfigProperty->PropertyInfo[0]))!=dataSize) return false;

	////������Ϣ
	////m_cbPropertyCount=__min(pConfigProperty->cbPropertyCount,CountArray(m_PropertyInfo));
	////memcpy(m_PropertyInfo,pConfigProperty->PropertyInfo,sizeof(tagPropertyInfo)*m_cbPropertyCount);

	////���õ���
	////m_GamePropertyCtrl.SetGamePropertyInfo(m_PropertyInfo,m_cbPropertyCount,PT_ISSUE_AREA_SERVER);
	//
	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRConfigProperty();

	//return true;
}

//�������Ȩ��	
bool SHZCServerItem::OnSocketSubConfigUserRight(void* data, int dataSize)
{
	PACKET_AIDE_DATA(data);

	//���Ȩ��
	mUserAttribute.dwUserRight=packet.read4Byte();

	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigUserRight();

	return true;
	////Ч������
	//ASSERT(dataSize>=sizeof(CMD_GR_ConfigUserRight));
	//if (dataSize<sizeof(CMD_GR_ConfigUserRight)) return false;

	////��Ϣ����
	//CMD_GR_ConfigUserRight * pConfigUserRight=(CMD_GR_ConfigUserRight *)data;

	////���Ȩ��
	//mUserAttribute.dwUserRight=pConfigUserRight->dwUserRight;

	//if (mIServerItemSink)
	//	mIServerItemSink->OnGRConfigUserRight();

	//return true;
}

//�������
bool SHZCServerItem::OnSocketSubConfigFinish(void* data, int dataSize)
{
	if (mIServerItemSink)
		mIServerItemSink->OnGRConfigFinish();
	return true;
}
