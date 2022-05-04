/*
1.��Ŀ���ƣ������콢��DHT11ģ��STM32F103RCT6���Գ���
2.��ʾģ�飺���ڷ�������
3.ʹ�����: keil5 for ARM
4.������λ������
5.��Ŀ��ɣ�DHT11ģ��
6.��Ŀ���ܣ����ڷ��ز�������ʪ�����ݣ�������9600.
7.��Ҫԭ������ο�DHT11�����ֲ�
8.�����ַ��https://lssz.tmall.com ���Ա��������������콢�ꡱ
10.��Ȩ�����������콢�����г��������������Ȩ�����뱾���Ʒ���׳��ۣ��벻Ҫ����������׷���䷨�����Σ�
���߶���:
	VCC--3.3V/5V
	GND--GND
	DATA--PA15
*/
#include "sys.h"
#include "delay.h"
#include "DHT11.h"
#include "stdio.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "oled.h"
#include "bsp_adc.h"
#include "bsp_usart1.h"
#include "usart.h"
//����Э���
#include "onenet.h"
//�����豸
#include "esp8266.h"
#include "cJSON.h"

extern __IO uint16_t ADC_ConvertedValue;
float ADC_ConvertedValueLocal;

const char *devSubTopic[] = {"/iot/943/sub"};
const char devPubTopic[] = "/iot/943/pub";
unsigned short timeCount = 0;	//���ͼ������
unsigned char *dataPtr = NULL;
char PUB_BUF[256]; //�ϴ����ݵ�buf
u8 LED_Status = 0;

int main(void)
{		
	LED0=1;
	u8 temperature;
	u8 humidity;
	float smoke;
	int t=0;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
	delay_init();
	Usart1_Init(115200); //debug����
	Usart2_Init(115200); //esp8266����
	DHT11_Init();
	OLED_Init();			//��ʼ��OLED  
	OLED_Clear(); 
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	ESP8266_Init();
	ADC1_Init();
	while(OneNet_DevLink())			//����OneNET
		delay_ms(500);	
	OneNet_Subscribe(devSubTopic, 1);
	
	while(1)
	{
		ADC_ConvertedValueLocal = (float) ADC_ConvertedValue/4096*3.3;
		OLED_Clear();
		DHT11_Read_Data(&temperature, &humidity);	//��ȡ��ʪ��ֵ
		smoke=MQ2_GetPPM();
		UsartPrintf(USART1,"�¶�:%d��\r\nʪ��:%d%%RH\r\nŨ��:%.2f\r\n", temperature, humidity,smoke);
		t=temperature;
		OLED_ShowString(0,0,"Temperature");
		OLED_ShowNum(83,0,t,3,16);//��ʾ�¶�
		OLED_ShowString(0,3,"Humidity");
		OLED_ShowNum(83,3,humidity,3,16);//��ʾ�¶�
		
		if(++timeCount >= 20)									//���ͼ��5s
		{
			LED_Status = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
			UsartPrintf(USART_DEBUG, "OneNet_Publish\r\n");
			sprintf(PUB_BUF,"{\"Temp\":%d,\"Hum\":%d,\"Led\":%d,\"Smoke\":%.2f}", temperature, humidity,LED_Status?0:1,smoke);
			OneNet_Publish(devPubTopic, PUB_BUF);
			timeCount = 0;
			ESP8266_Clear();
		}
		
		LED0 = LED_Status;
		dataPtr = ESP8266_GetIPD(3); // 8266������Ϣ
		if(dataPtr != NULL)
			OneNet_RevPro(dataPtr);
		
		delay_ms(500);
	}
}


