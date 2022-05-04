#ifndef __DHT11_H
#define __DHT11_H 
#include "sys.h"   

////IO��������											   
#define	DHT11_DQ_OUT PAout(15) //���ݶ˿�	PA15 
#define	DHT11_DQ_IN  PAin(15)  //���ݶ˿�	PA15

#define DHT11_GPIO_PORT    	GPIOA			              //GPIO�˿�
#define DHT11_GPIO_CLK 	    RCC_APB2Periph_GPIOA		//GPIO�˿�ʱ��
#define DHT11_GPIO_PIN		GPIO_Pin_15			        //���ӵ�SCLʱ���ߵ�GPIO

u8 DHT11_Init(void);//��ʼ��DHT11
u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
#endif















