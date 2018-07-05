/**
  ******************************************************************************
  * @file    EXTI/EXTI_Example/main.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    24-July-2014
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stdbool.h"
#include "stdio.h"
/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Example
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
	
/* Private function prototypes -----------------------------------------------*/
static void USART_Config(void);
static void EXTI0_Config(void);
#ifdef USE_STM32072B_EVAL 
//static void EXTI2_3_Config(void);
#endif
static void EXTI4_15_Config(void);
__IO bool exflag=0;
__IO uint8_t btflag = 0;
/* Private functions ---------------------------------------------------------*/
void Delay(__IO uint32_t nCount);
void GPIOinit(void);
uint8_t scan(uint8_t line);
static __IO uint32_t TimingDelay;
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f0xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f0xx.c file
     */

  /* Initialize LEDs mounted on EVAL board */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
	USART_Config();
	GPIOinit();
	
	while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}
  
		
	if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
  /* Configure PA0 in interrupt mode */
  EXTI0_Config();

  EXTI4_15_Config();

	printf("Ready!");

  /* Infinite loop */
  while (1)
  {
		//PortC 5 G,6 W,7 G for scan
		if(btflag != 0)
		{
			switch (btflag)
			{
				case 5:
					printf("%c",scan(5));
				  btflag = 0;
				break;
				case 6:
					printf("%c",scan(6));
				  btflag = 0;
				break;
				case 7:
					printf("%c",scan(7));
				  btflag = 0;
				break;
				default:
					btflag = 0;
					break;
			}
		}
  }
}
	
uint8_t scan(uint8_t line)
{
  if(line == 5)
	{
		GPIOA->BRR = GPIO_Pin_10;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_10;
			return '1';
		}
		GPIOA->BSRR = GPIO_Pin_10;
		GPIOA->BRR = GPIO_Pin_11;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_11;
			return '4';
		}
		GPIOA->BSRR = GPIO_Pin_11;
		GPIOA->BRR = GPIO_Pin_12;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_12;
			return '7';
		}
		GPIOA->BSRR = GPIO_Pin_12;
		GPIOA->BRR = GPIO_Pin_13;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_13;
			return '*';
		}
		GPIOA->BSRR = GPIO_Pin_13;
	}
	if(line == 6)
	{
		GPIOA->BRR = GPIO_Pin_10;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_10;
			return '2';
		}
		GPIOA->BSRR = GPIO_Pin_10;
		GPIOA->BRR = GPIO_Pin_11;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_11;
			return '5';
		}
		GPIOA->BSRR = GPIO_Pin_11;
		GPIOA->BRR = GPIO_Pin_12;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_12;
			return '8';
		}
		GPIOA->BSRR = GPIO_Pin_12;
		GPIOA->BRR = GPIO_Pin_13;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_13;
			return '0';
		}
		GPIOA->BSRR = GPIO_Pin_13;
	}
	if(line == 7)
	{
		GPIOA->BRR = GPIO_Pin_10;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_10;
			return '3';
		}
		GPIOA->BSRR = GPIO_Pin_10;
		GPIOA->BRR = GPIO_Pin_11;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_11;
			return '6';
		}
		GPIOA->BSRR = GPIO_Pin_11;
		GPIOA->BRR = GPIO_Pin_12;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_12;
			return '9';
		}
		GPIOA->BSRR = GPIO_Pin_12;
		GPIOA->BRR = GPIO_Pin_13;
		if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7) == RESET)
		{
			GPIOA->BSRR = GPIO_Pin_13;
			return '#';
		}
		GPIOA->BSRR = GPIO_Pin_13;
	}
	return 0;
}

static void USART_Config(void)
{ 
  USART_InitTypeDef USART_InitStructure;
  
  /* USARTx configured as follow:
  - BaudRate = 115200 baud  
  - Word Length = 8 Bits
  - Stop Bit = 1 Stop Bit
  - Parity = No Parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until transmit data register is empty */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TXE) == RESET)
  {}

  return ch;
}

void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

/**
  * @brief  Configure PA0 in interrupt mode
  * @param  None
  * @retval None
  */
static void EXTI0_Config(void)
{
  /* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PA0 pin as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI0 Line to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

  /* Configure EXTI0 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line0;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI0 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x00;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configure PE3, PE0in interrupt mode
  * @param  None
  * @retval None
  */

/**
  * @brief  Configure PC13 in interrupt mode
  * @param  None
  * @retval None
  */
static void EXTI4_15_Config(void)
{
  /* Enable GPIOC clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* Configure PC8 and PC13 pins as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  
   /* Connect EXTI8 Line to PC8 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource5);
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource6);
  /* Connect EXTI13 Line to PC13 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource7);
  
  /* Configure EXTI8 line */
  EXTI_InitStructure.EXTI_Line =  EXTI_Line5 |EXTI_Line6 |EXTI_Line7;  
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
  /* Configure EXTI13 line */
  /* Enable and set EXTI4_15 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_15_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPriority = 0x02;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

}
void GPIOinit(void)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* Configure PC10 and PC11 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 ;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIOA->BSRR = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;//PA10 R,11 Br,12 O,13 Y
}
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
