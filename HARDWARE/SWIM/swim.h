#ifndef _SWIM_H_
#define _SWIM_H_

#include "sys.h"
#include "delay.h"

///////////////swim_pin/////////////////
//swim
#define GPIO_SWIM_PORT 						GPIOB

#define GPIO_SWIM_OUT_PIN         GPIO_Pin_8		//Tim4_CH3ӳ�䵽PB.8��ΪSWIM���
#define GPIO_SWIM_IN1_PIN					GPIO_Pin_7		//PB7,PB9 ���߶�����ΪSWIM����
#define GPIO_SWIM_IN2_PIN				  GPIO_Pin_9		//Tim4_CH2 PB7// Tim4_CH4 PB9

#define SWIM_OUT_PIN_OD 					GPIO_SWIM_PORT->CRH &= ~GPIO_CRH_CNF8_1  //CNFλ 01Ϊ��©
#define SWIM_OUT_PIN_AFOD				  GPIO_SWIM_PORT->CRH |= GPIO_CRH_CNF8	 //CNFλ 11����Ϊ��©

#define SWIM_HIGH		  						GPIO_SWIM_PORT->BSRR = GPIO_SWIM_OUT_PIN		 
#define SWIM_LOW   								GPIO_SWIM_PORT->BRR = GPIO_SWIM_OUT_PIN


#define SWIM_IN1  								GPIO_ReadInputDataBit(GPIO_SWIM_PORT, GPIO_SWIM_IN1_PIN)
#define SWIM_IN2	  							GPIO_ReadInputDataBit(GPIO_SWIM_PORT, GPIO_SWIM_IN2_PIN)
#define SWIM_IN1_Q  							GPIO_SWIM_PORT->IDR & GPIO_SWIM_IN1_PIN
#define SWIM_IN2_Q  							GPIO_SWIM_PORT->IDR & GPIO_SWIM_IN2_PIN


//rst
#define GPIO_SWIM_RST_PORT 				GPIOB
#define GPIO_SWIM_RSTOUT_PIN			GPIO_Pin_6		//��λ�� pb6

#define SWIM_RST_HIGH  						GPIO_SWIM_RST_PORT->BSRR = GPIO_SWIM_RSTOUT_PIN		 
#define SWIM_RST_LOW   						GPIO_SWIM_RST_PORT->BRR = GPIO_SWIM_RSTOUT_PIN	





////////////////////////////////////////




///////////////swim_cmd/////////////////
#define SWIM_CMD_SRST					0x00	 //��λ
#define SWIM_CMD_ROTF					0x01	 //SWIM ���ٶ�
#define SWIM_CMD_WOTF					0x02	 //SWIM ����д

#define SWIM_SYNC_CYCLES				128


////////////////////////////////////////


///////////////dma/////////////////
#define SWIN_DMA_DAT_IN_BUF_SIZE 32

//#define SWIN_DMA_DAT_ENABLE(num){\
//DMA1_Channel4->CCR &= (uint16_t)(~DMA_CCR1_EN);\
//DMA1_Channel4->CNDTR = num;\
//TIM4->CR1 |= TIM_CR1_CEN;\
//DMA1_Channel4->CCR |= DMA_CCR1_EN;\
//\
//}

#define SWIN_DMA_DAT_ENABLE(num){\
DMA1_Channel1->CCR &= (uint16_t)(~DMA_CCR1_EN);\
DMA1_Channel1->CNDTR = num;\
TIM4->CR1 |= TIM_CR1_CEN;\
DMA1_Channel1->CCR |= DMA_CCR1_EN;\
\
}
////////////////////////////////////////


////////
void SWIM_Init(void);
uint8_t SWIM_EnterProgMode(void);



////////
void SWIM_GPIO_Init(void);




////////
#endif
