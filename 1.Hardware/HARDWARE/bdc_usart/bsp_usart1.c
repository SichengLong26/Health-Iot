#include "bsp_usart1.h"
void USART1_Config(void)//���ô���
{
		GPIO_InitTypeDef GPIO_InitStructure;//GPIO��ʼ���ṹ��
		USART_InitTypeDef USART_InitStructure;//���ڳ�ʼ���ṹ��
		
		/* config USART1 clock */
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);//������1��ʱ��
		
		/* USART1 GPIO config */
		/* Configure USART1 Tx (PA.09) as alternate function push-pull */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//����1��ͨ��PA9���������
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//GPIOģʽΪƬ���������
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//�����ٶ�Ϊ50MHZ
		GPIO_Init(GPIOA, &GPIO_InitStructure);  // PA�ڳ�ʼ��  
		/* Configure USART1 Rx (PA.10) as input floating */
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//����1��ͨ��PA10��������ģ�
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//����Ϊ�������뷽ʽ
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��PA��
			
		/* USART1 mode config */
		USART_InitStructure.USART_BaudRate = 115200;//���ڲ�����Ϊ115200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//���ݳ���Ϊ8λ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//1��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No ;//��У��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ��������λ
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//ȫ˫������he����ģʽ
		USART_Init(USART1, &USART_InitStructure); //���ڳ�ʼ��
		USART_Cmd(USART1, ENABLE);//����ʹ��

}

/**********************************************************************/



///�ض���c�⺯��printf��USART1
int fputc(int ch, FILE *f)//FILE *fΪһ���ļ��������ݴ�������
{
		/* ����һ���ֽ����ݵ�USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

///�ض���c�⺯��scanf��USART1
int fgetc(FILE *f)
{
		/* �ȴ�����1�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
