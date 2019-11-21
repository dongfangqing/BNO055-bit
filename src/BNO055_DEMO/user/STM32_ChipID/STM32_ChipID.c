#include "STM32_ChipID.h"



/*****************************************************
**�������ܣ���ȡSTM32оƬID��
**��	����ID��Ϊ96λ12Byte���ȣ����Ա��붨��Ϊ����Ԫ��Ϊu32�����ҳ��ȴ���3
**����	ֵ��0-----------��ȡ�ɹ�
**			1-----------��ȡʧ��
******************************************************/
u8 Get_ChipID(u32* CPU_ID)
{
//	u32 Dev_ID_Addr0,Dev_ID_Addr1,Dev_ID_Addr2;
	
	//��ȡоƬID
	CPU_ID[0]=*(vu32*)(0x1ffff7e8);
	CPU_ID[1]=*(vu32*)(0x1ffff7ec);
	CPU_ID[2]=*(vu32*)(0x1ffff7f0);
	
	if((CPU_ID[0] != 0)&&(CPU_ID[1] != 0)&&(CPU_ID[2] != 0))
		return 0;
	
	return 1;
}


/*****************************************************
**�������ܣ�WIFI�û�������㷨
**��	����Val = ((u8)CPU_ID[0]+(u8)CPU_ID[1]+(u8)CPU_ID[2])^0xFF
******************************************************/
u32 WIFI_UserNum_Val(u32* CPU_ID)
{
	u32 ret = 0;
	u8 i = 0;
	u8 buff[3] = {0};

	if(CPU_ID != (void*)0)
	{
		for(i = 0;i < 3;i++)
			buff[i] = (u8)CPU_ID[i];
		
		for(i = 0;i < 3;i++)
			ret += buff[i];
		
		ret ^= 0xff;
	}
	
	return ret;
}

