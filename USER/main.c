#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "usart.h"
#include "swim.h"
#include "swim_cmd.h"

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"	


#include "SEGGER_SYSVIEW.h"


 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��
	//SEGGER_SYSVIEW_Conf();//��ʼ���������
	uart_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	//LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	//TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms 

	delay_ms(500);
	USB_Port_Set(0); 	//USB�ȶϿ�
	delay_ms(100);
	USB_Port_Set(1);	//USB�ٴ�����
 	Set_USBClock();   
 	USB_Interrupts_Config();    
 	USB_Init();	

	 
	 SWIM_Init();
//	 SWIM_EnterProgMode();
//	 SWIM_CUT_OFF();
   	while(1)
	{
		SWIM_Service();
	   	
	}
}
