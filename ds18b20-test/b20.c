#include "stm32f0xx.h"  
#include "b20.h"  
  
   
  
#define EnableINT()    
#define DisableINT()   
  
#define DS_PORT GPIOA  
#define DS_DQIO GPIO_Pin_1  
#define DS_RCC_PORT RCC_APB1Periph_USART2
#define DS_PRECISION 0x7f //1f=9bis,3f=10bits,5f=11bits,7f=12bits  7f's precsion is 0.0625;  
#define DS_AlarmTH 0x64  
#define DS_AlarmTL 0x8a  
#define DS_CONVERT_TICK 1000  
  
#define ResetDQ() GPIO_ResetBits(DS_PORT,DS_DQIO)  
#define SetDQ() GPIO_SetBits(DS_PORT,DS_DQIO)  
#define GetDQ() GPIO_ReadInputDataBit(DS_PORT,DS_DQIO)  
   
   
//static  unsigned  char  TempX_TAB[16]={0x00,0x01,0x01,0x02,0x03,0x03,0x04,0x04,0x05,0x06,0x06,0x07,0x08,0x08,0x09,0x09};  
  
  
void  Delay_us(uint32_t Nus)   
{    
 SysTick->LOAD=Nus*9;           //time count up         
 SysTick->CTRL|=0x01;              //time count down      
 while (!(SysTick->CTRL&(1<<16)));  //wait time count to 0
 SysTick->CTRL=0X00000000;         //close count
 SysTick->VAL=0X00000000;          //clear count  
}    
  
   
  
unsigned  char  ResetDS18B20( void )  
{  
 unsigned  char  resport;  
 SetDQ();  
 Delay_us(50);  
   
 ResetDQ();  
 Delay_us(500);   //500us (wait 480us~960us)  
 SetDQ();  
 Delay_us(40);   //40us  
 //resport = GetDQ();  
 while (GetDQ());  
 Delay_us(500);   //500us  
 SetDQ();  
 return  resport;  
}  
  
void  DS18B20WriteByte(unsigned  char  Dat)  
{  
 unsigned  char  i;  
 for (i=0;i<8;i++)  
 {  
   ResetDQ();      //Send the number to the data line within 15us, the DS18B20 reads at 15-60u 
  Delay_us(5);     //5us  
  if (Dat & 0x01)  
   SetDQ();  
  else  
   ResetDQ();  
  Delay_us(65);     //65us  
  SetDQ();  
  Delay_us(2);     //More than 1 us between  2 bits  
  Dat >>= 1;   
 }   
}  
  
  
unsigned  char  DS18B20ReadByte( void )  
{  
 unsigned  char  i,Dat;  
 SetDQ();  
 Delay_us(5);  
 for (i=0;i<8;i++)  
 {  
   Dat >>= 1;  
	 
/**	From the beginning of the read sequence to the sampling signal line must be within 15u
	  ,and the sampling should be arranged as far as possible at the end of 15u */
    ResetDQ();      
	 
  Delay_us(5);    //5us  
  SetDQ();  
  Delay_us(5);    //5us  
  if (GetDQ())  
    Dat|=0x80;  
  else  
   Dat&=0x7f;    
  Delay_us(65);    //65us  
  SetDQ();  
 }  
 return  Dat;  
}  
  
  
void  ReadRom(unsigned  char  *Read_Addr)  
{  
 unsigned  char  i;  
  
 DS18B20WriteByte(ReadROM);  
    
 for (i=0;i<8;i++)  
 {  
  *Read_Addr=DS18B20ReadByte();  
  Read_Addr++;  
 }  
}  
  
  
void  DS18B20Init(unsigned  char  Precision,unsigned  char  AlarmTH,unsigned  char  AlarmTL)  
{  
 DisableINT();  
 ResetDS18B20();  
 DS18B20WriteByte(SkipROM);   
 DS18B20WriteByte(WriteScratchpad);  
 DS18B20WriteByte(AlarmTL);  
 DS18B20WriteByte(AlarmTH);  
 DS18B20WriteByte(Precision);  
  
 ResetDS18B20();  
 DS18B20WriteByte(SkipROM);   
 DS18B20WriteByte(CopyScratchpad);  
 EnableINT();  
  
 while (!GetDQ());   //Waiting for replication to complete
}  
  
  
void  DS18B20StartConvert( void )  
{  
 DisableINT();  
 ResetDS18B20();  
 DS18B20WriteByte(SkipROM);   
 DS18B20WriteByte(StartConvert);   
 EnableINT();  
}  
  
void  DS18B20_Configuration( void )  
{  
 GPIO_InitTypeDef GPIO_InitStructure;  
   
 RCC_APB2PeriphClockCmd(DS_RCC_PORT, ENABLE);  
  
 GPIO_InitStructure.GPIO_Pin = DS_DQIO;  
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //let pin output 
 GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//output Open Drain Mode
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
 GPIO_Init(DS_PORT, &GPIO_InitStructure);  
}  
  
  
void  ds18b20_init( void )  
{  
 DS18B20_Configuration();  
 DS18B20Init(DS_PRECISION, DS_AlarmTH, DS_AlarmTL);  
 DS18B20StartConvert();  
}  
  
  
float ds18b20_read(void)
{
	unsigned char DL, DH;
	unsigned short TemperatureData;//2 bytes space
	float Temperature;
	float symbol = 1;
	DisableINT();
	DS18B20StartConvert();
	ResetDS18B20();
	DS18B20WriteByte(SkipROM); 
	DS18B20WriteByte(ReadScratchpad);
	DL = DS18B20ReadByte();
	DH = DS18B20ReadByte(); 
	EnableINT();

	TemperatureData = DH;
	TemperatureData <<= 8;
	TemperatureData |= DL;
	
	//if top 5 bits is 1,the temp is negative
	if(TemperatureData > 0xF800)
	{
		TemperatureData = ~(TemperatureData - 1);
		symbol= -1;
	}
	
	Temperature = (float)symbol*((float)TemperatureData * 0.0625); //Resolution 0.0625

	return  Temperature;
}
