#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "swim.h"



 int main(void)
 {	
	delay_init();	    	 //��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	//LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	//TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	 SWIM_Init();
	 SWIM_EnterProgMode();
   	while(1)
	{
	   
	}
}
