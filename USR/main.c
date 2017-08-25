#include "pbdata.h"

void RCC_Configuration(void);
void GPIO_COM_Configuration(void);
void NVIC_Configuration(void);
void USART_Configuration(void);

SD_Error Status=SD_OK;
extern SD_CardInfo SDCardInfo;

FATFS fs;
FIL fdst;
int res;
UINT brw;

int fputc(int ch,FILE *f)
{
	USART_SendData(USART3,(u8)ch);
	while(USART_GetFlagStatus(USART3,USART_FLAG_TXE)==RESET);
	return ch;
}

int main(void)
{
   u8 Buf[]="yo yo yo yo rap of China! www.zxkjmcu.com";
   u8 Rev[128] = {0};
    
   RCC_Configuration();	//系统时钟初始化
   GPIO_COM_Configuration();//端口初始化
   USART_Configuration();
   NVIC_Configuration();

   Status=SD_Init();

   if(Status==SD_OK)
   {
   		printf("SD卡初始化成功!\r\n");
   }
   else
   {
   		printf("SD卡初始化失败!\r\n");
   }
   
   if(f_mount(&fs,"0:",1) == FR_NO_FILESYSTEM){
       printf("没有文件系统。。\r\n");
       if(f_mkfs("0:",0,0) == FR_OK)printf("创建文件系统成功！\r\n");
   }
   
   res=f_open(&fdst,"0:/dk.txt",FA_OPEN_EXISTING|FA_WRITE);

   if(res==FR_OK)
   {
   	 	res=f_write(&fdst,Buf,sizeof(Buf),&brw);
		f_close(&fdst);

		printf("\r\n文件打开成功\r\n");
		//res=f_open(&fdst,"0:/zx01.txt",FA_OPEN_EXISTING|FA_READ);
        f_puts("HEHEDA", &fdst);
		while(1)
		{
			brw=0;
		//	memset(Rev,0,sizeof(Rev));
			res=f_read(&fdst,Rev,sizeof(Rev),&brw);
			printf("\r\n %s",Buf);
			if(res||brw==0) break;		
		}

		f_close(&fdst);
   }
   
   while(1);	
}

void RCC_Configuration(void)
{
    SystemInit();//72m
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
}

void GPIO_COM_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;	//COM
    
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;//TX
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;//RX
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

void NVIC_Configuration(void)
{
   	NVIC_InitTypeDef NVIC_InitStructure; 

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); 

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; 
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
}

void USART_Configuration(void)
{
    USART_InitTypeDef  USART_InitStructure;

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;

	USART_Init(USART3,&USART_InitStructure);
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART3,ENABLE);
	USART_ClearFlag(USART3,USART_FLAG_TC);
}



