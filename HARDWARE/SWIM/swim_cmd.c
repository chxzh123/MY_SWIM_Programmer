#include "swim_cmd.h"
#include "usb_lib.h"
#include "SEGGER_SYSVIEW.h"

uint8_t USB_Rx_Buffer[64];
uint8_t USB_RX_DAT=0,//����Ѿ��յ����ݣ�������һ����������꣩
USB_RX_CMD_BUSY=0,//���Ϊ1���ʾ��һ�����û������ɣ�0�����
LONG_CMD_BUSY=0;//�����������64�ֽڣ�һ��usb�����ĳ�����ı�ǣ�һ��Ϊдһ�����ݣ�
uint16_t USB_Rx_Cnt; //���ճ���
uint8_t CMD_READY=0;//���ڻظ�09ָ������������־λ �����0 �������1Ϊ�Ǿ��� 2Ϊ���ٷְٷǾ���  4Ϊ����
uint16_t CMD_READY_num=0;//���ڻظ�09ָ������������־λ ��CMD_READYΪ2��ʱ�򣬷Ǿ���������ȼ�¼(�Ѵ������)

uint8_t SWIM_EnterProgMode_EN=0;//����ʱ��ʹ�� ���Ϊ1��ִ�н���ʱ��
uint8_t SWIM_WOTF_DAT_EN=0;//д������ʹ��

uint8_t WOTF_DAT_Buffer[1024];
uint16_t WOTF_DAT_num=0;
uint16_t WOTF_DAT_num_left=0;//ʣ����Ҫ�������
uint32_t WOTF_DAT_ADDR=0;



uint8_t USB_Tx_Buffer[64];
uint8_t ROTF_DAT_Buffer[6*1024];//ÿ�����6K�Ķ�ȡ��
uint8_t SWIM_ROTF_DAT_EN=0;//��������ʹ��


uint16_t ROTF_DAT_num=0;
uint16_t ROTF_DAT_num_left=0;//ʣ����Ҫ�������
uint32_t ROTF_DAT_ADDR=0;

void GET_USB_CMD_Data(uint8_t bEpAddr,uint8_t bEpNum)//�ϵ�ص�����
{
uint16_t i;
	if(USB_RX_CMD_BUSY)//��һ�����û�б�����
	{
		USB_RX_DAT=1;
	}
	else
	{
		USB_Rx_Cnt = USB_SIL_Read(bEpAddr, USB_Rx_Buffer);	//�õ�USB���յ������ݼ��䳤��  							
		SetEPRxValid(bEpNum);//ʹ�ܶ˵�����ݽ���
		USB_RX_CMD_BUSY=1;
		USB_RX_DAT=0;
		if(LONG_CMD_BUSY)//һ�δ�����
		{

			if(WOTF_DAT_num_left>USB_Rx_Cnt)
			{
				for(i=0;i<USB_Rx_Cnt;i++)
				{
					WOTF_DAT_Buffer[WOTF_DAT_num-WOTF_DAT_num_left]=USB_Rx_Buffer[i];		
					WOTF_DAT_num_left--;								
				}						
			}
			else
			{
				for(i=0;i<USB_Rx_Cnt;i++)
				{
					WOTF_DAT_Buffer[WOTF_DAT_num-WOTF_DAT_num_left]=USB_Rx_Buffer[i];		
					WOTF_DAT_num_left--;								
				}							
				LONG_CMD_BUSY=0;
				SWIM_WOTF_DAT_EN=1;//usb������ɣ���ʼд���豸
			}
			USB_RX_CMD_BUSY=0;
		}		
		
	}
}

void SWIM_Process_USB_CMD(void)
{
	uint16_t i;
	if(USB_RX_CMD_BUSY)//���������
	{
		if(LONG_CMD_BUSY==0)//һ���ܴ���
		{
			if(USB_Rx_Buffer[0]==0xf4)
			{
				switch(USB_Rx_Buffer[1])
				{
					case 0x02:
					{
						USB_Tx_Buffer[0]=0x00;
						USB_Tx_Buffer[1]=0x01;
						USB_Tx_Buffer[2]=0x02;
						USB_Tx_Buffer[3]=0x07;
						USB_Tx_Buffer[4]=0x00;
						USB_Tx_Buffer[5]=0x00;
						USB_Tx_Buffer[6]=0x00;
						USB_Tx_Buffer[7]=0x00;
						SEND_Data_To_USB(USB_Tx_Buffer,8);						
						break;
					}					
					case 0x03:
					{	
						if(USB_Rx_Buffer[2]==0x00)
						{
							SWIM_Set_Low_Speed();
						}
						if(USB_Rx_Buffer[2]==0x01&&USB_Rx_Buffer[3]==0x01)
						{
							SWIM_Set_High_Speed();
						}
						CMD_READY=0;						
						break;
					}
					case 0x04:
					{	
						SWIM_EnterProgMode_EN=1;
						CMD_READY=1;						
						break;
					}
					case 0x05:
					{	
						SWIM_SRST();
						CMD_READY=0;						
						break;
					}
					case 0x06:
					{	
						CMD_READY=SWIM_Communication_Reset();					
						break;
					}
					case 0x07:
					{	
						SWIM_RST_LOW;
						CMD_READY=0;						
						break;
					}
					case 0x08:
					{
						SWIM_RST_HIGH;
						CMD_READY=0;						
						break;
					}
					case 0x09:
					{
						if(CMD_READY==0||CMD_READY==1||CMD_READY==4)	
						{
							SEGGER_SYSVIEW_Print("9-1");
							USB_Tx_Buffer[0]=CMD_READY;
							USB_Tx_Buffer[1]=0x00;
							USB_Tx_Buffer[2]=0x00;
							USB_Tx_Buffer[3]=0x00;
							SEND_Data_To_USB(USB_Tx_Buffer,4);														
						}
						else if(CMD_READY==2)	
						{
							USB_Tx_Buffer[0]=0x01;
							USB_Tx_Buffer[3]=0x00;
							if(CMD_READY_num<0x100)
							{
								USB_Tx_Buffer[1]=CMD_READY_num;
								USB_Tx_Buffer[2]=0x00;												
							}
							else
							{
								USB_Tx_Buffer[1]=CMD_READY_num&0x00ff;
								USB_Tx_Buffer[2]=(CMD_READY_num>>8)&0x00ff;								
							}
							SEGGER_SYSVIEW_Print("9-2");
							SEND_Data_To_USB(USB_Tx_Buffer,4);								
						}							
						break;
					}
					case 0x0a://д
					{
						CMD_READY=1;	
						WOTF_DAT_num=(USB_Rx_Buffer[3]&0x00ff)|((USB_Rx_Buffer[2]<<8)&0xff00);
						WOTF_DAT_ADDR=(USB_Rx_Buffer[7]&0x000000ff)|((USB_Rx_Buffer[6]<<8)&0x0000ff00)|((USB_Rx_Buffer[5]<<16)&0x00ff0000);
						if(WOTF_DAT_num>8)
						{
							LONG_CMD_BUSY=1;
							WOTF_DAT_num_left=WOTF_DAT_num;
							for(i=0;i<8;i++)
							{
								WOTF_DAT_Buffer[WOTF_DAT_num-WOTF_DAT_num_left]=USB_Rx_Buffer[8+i];		
								WOTF_DAT_num_left--;								
							}
							CMD_READY=2;
						}
						else
						{							
							SWIM_WOTF(WOTF_DAT_ADDR, WOTF_DAT_num, &USB_Rx_Buffer[8]);
							CMD_READY=0;
						}							
							
						break;
					}
					case 0x0b://��
					{
						ROTF_DAT_num=(USB_Rx_Buffer[3]&0x00ff)|((USB_Rx_Buffer[2]<<8)&0xff00);
						ROTF_DAT_ADDR=(USB_Rx_Buffer[7]&0x000000ff)|((USB_Rx_Buffer[6]<<8)&0x0000ff00)|((USB_Rx_Buffer[5]<<16)&0x00ff0000);
						ROTF_DAT_num_left=ROTF_DAT_num;
						SWIM_ROTF_DAT_EN=1;
						CMD_READY=2;							
						break;
					}	
					case 0x0c://�� ���ݻش�
					{
						while(ROTF_DAT_num_left)
						{
							if(ROTF_DAT_num_left>64)
							{								
								SEND_Data_To_USB(&ROTF_DAT_Buffer[ROTF_DAT_num-ROTF_DAT_num_left],64);
								ROTF_DAT_num_left-=64;
							}
							else
							{
								SEND_Data_To_USB(&ROTF_DAT_Buffer[ROTF_DAT_num-ROTF_DAT_num_left],ROTF_DAT_num_left);
								ROTF_DAT_num_left=0;
							}					
						}							
						break;
					}	
					case 0x0d:
					{
						USB_Tx_Buffer[0]=0x00;
						USB_Tx_Buffer[1]=0x18;
						SEND_Data_To_USB(USB_Tx_Buffer,2);														
						break;
					}					
					default: break;
				}		
			}
			else if(USB_Rx_Buffer[0]==0xf5)
			{
				if(USB_Rx_Buffer[1]==0x00)
				{
					USB_Tx_Buffer[0]=0x00;
					USB_Tx_Buffer[1]=0x01;
					SEND_Data_To_USB(USB_Tx_Buffer,2);
				}
			}
			USB_RX_CMD_BUSY=0;
		}

	}
}


void SWIM_Service(void)
{
	SWIM_Process_USB_CMD();
	if(SWIM_EnterProgMode_EN==1)
	{
		CMD_READY=SWIM_EnterProgMode_Time_Wheel(SWIM_Process_USB_CMD);//�ڽ���ʱ��������Ӽ������	
		SWIM_EnterProgMode_EN=0;
	}

	if(SWIM_WOTF_DAT_EN==1)
	{
		CMD_READY_num=0;	
		CMD_READY=SWIM_WOTF_LONG_DAT_Time_Wheel(WOTF_DAT_ADDR, WOTF_DAT_num, WOTF_DAT_Buffer, &CMD_READY_num,SWIM_Process_USB_CMD);//��дʱ��������Ӽ������	(��Ҫ�Ǽ��09����)
		SWIM_WOTF_DAT_EN=0;
	}
	
	if(SWIM_ROTF_DAT_EN==1)
	{
		CMD_READY_num=0;	
		CMD_READY=SWIM_ROTF_LONG_DAT_Time_Wheel(ROTF_DAT_ADDR, ROTF_DAT_num, ROTF_DAT_Buffer, &CMD_READY_num,SWIM_Process_USB_CMD);//�ڶ�ʱ��������Ӽ������	(��Ҫ�Ǽ��09����)
		SWIM_ROTF_DAT_EN=0;
	}
		
	if(USB_RX_DAT==1)GET_USB_CMD_Data(EP2_OUT,ENDP2);
	
}




