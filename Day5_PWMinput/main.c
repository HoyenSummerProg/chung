/**
  ******************************************************************************
  * @file    TIM/TIM_7PWMOutputs/main.c  
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

/** @addtogroup STM32F0xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup TIM_7PWMOutputs
  * @{
  */
static __IO uint32_t TimingDelay;

void Delay(__IO uint32_t nTime);
void RCC_Configuration(void);
void TIM_Configuration(void);
void GPIO_Configuration(void);;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  volatile int i;
  int n = 1;
  uint16_t brightness = 0;      

  RCC_Configuration();
  TIM_Configuration();
  GPIO_Configuration();

  
  while(1)  // Do not exit
  {
    if (((brightness + n) >= 3000) || ((brightness + n) <= 0))
      n = -n; // if  brightness maximum/maximum change direction
    
    brightness += n;
    
    // TIM4->CCR3 = brightness - 1;
    // TIM4->CCR4 = brightness - 1;
    
    //Light LEDs in turn
		TIM3->CCR1 = brightness - 1; // set brightness
            
		TIM3->CCR2 = brightness - 1; // set brightness
      
		TIM3->CCR3 = brightness - 1; // set brightness
            
		TIM3->CCR4 = brightness - 1; // set brightness
              
    for(i=0;i<10000;i++)//delay
		{}
  }
 
  
}   
/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
   RCC_AHBPeriphClockCmd(  RCC_AHBPeriph_GPIOC , ENABLE );
   RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3, ENABLE );
}

/**
  * @brief  configure the PD12~15 to Timers
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure
 
//    GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_0);
//    GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_0);
      

    // Setup Blue & Green LED on STM32-Discovery Board to use PWM.
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9; //PD12->LED3 PD13->LED4 PD14->LED5 PD15->LED6
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;            // Alt Function - Push Pull
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init( GPIOC, &GPIO_InitStructure );  
}

/**
  * @brief  configure the TIM4 for PWM mode
  * @param  None
  * @retval None
  */
void TIM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef TIM_OCInitStruct;

    // Let PWM frequency equal 100Hz.
    // Let period equal 1000. Therefore, timer runs from zero to 1000. Gives 0.1Hz resolution.
    // Solving for prescaler gives 240.
    TIM_TimeBaseStructInit( &TIM_TimeBaseInitStruct );
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV4;
    TIM_TimeBaseInitStruct.TIM_Period = 1680-1;   
    TIM_TimeBaseInitStruct.TIM_Prescaler = 500 - 1; 
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;    
    TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct );
    
    TIM_OCStructInit( &TIM_OCInitStruct );
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
    
    // Initial duty cycle equals 0%. Value can range from zero to 65535.
    //TIM_Pulse = TIM4_CCR1 register (16 bits)
    TIM_OCInitStruct.TIM_Pulse = 65535; //(0=Always Off, 65535=Always On)
 
		TIM_OC1Init( TIM3, &TIM_OCInitStruct ); // Channel 3  LED
    TIM_OC2Init( TIM3, &TIM_OCInitStruct ); // Channel 4  LED
    TIM_OC3Init( TIM3, &TIM_OCInitStruct ); // Channel 3  LED
    TIM_OC4Init( TIM3, &TIM_OCInitStruct ); // Channel 4  LED
 
    TIM_Cmd( TIM3, ENABLE );
}
