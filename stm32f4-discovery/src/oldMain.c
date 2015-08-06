/**
  ******************************************************************************
  * @file    IO_Toggle/main.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    19-September-2011
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

//jb hmmmfasdfa
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"
//hmmm

/** @addtogroup STM32F4_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup IO_Toggle
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
GPIO_InitTypeDef  GPIO_InitStructure;
CAN_InitTypeDef   CAN_InitStructure;//jb

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f4xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
        system_stm32f4xx.c file
     */


  
	 
  /* GPIOD Periph clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

  /* Configure PD12, PD13, PD14 and PD15 in output pushpull mode */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  CAN_StructInit(&CAN_InitStructure);
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
  CAN_InitStructure.CAN_TTCM = DISABLE; 
  CAN_InitStructure.CAN_ABOM  = DISABLE; 
  CAN_InitStructure.CAN_AWUM = DISABLE; 
  CAN_InitStructure.CAN_NART = DISABLE; 
  CAN_InitStructure.CAN_RFLM = DISABLE; 
  CAN_InitStructure.CAN_TXFP = DISABLE; 

  
  
  //GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9; //p59
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; //p59
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF ;// GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  

  
//1
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);//JB //for CAN1 
   CAN_DeInit(CAN1);
//2
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //JB //first arg is kind of a guess.
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_CAN1);
  //GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_CAN1);//JB //first arg is guess,//second is tricky,//but thrid is good GPIO_AF_CAN1 
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);//JB //first arg is guess,//second is tricky,//but thrid is good GPIO_AF_CAN1 
  
  //GPIO_Init(GPIOb, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  
//3
  //uint8_t CAN_Init(CAN_TypeDef* CANx, CAN_InitTypeDef* CAN_InitStruct)
  CAN_StructInit(&CAN_InitStructure);
  CAN_Init(CAN1, &CAN_InitStructure);

  
  while (1)
  {
    int i =0;
	CanTxMsg myMessage;
	myMessage.StdId = 0x711;
	myMessage.IDE = CAN_Id_Standard;
	myMessage.RTR = CAN_RTR_Data;
	myMessage.DLC =0;  //NO data //A free DLC, how nice
	
	//while(1){
		uint8_t mailBox = CAN_Transmit(CAN1,&myMessage);
		CAN_TransmitStatus(CAN1,mailBox); 
	//}
    /* PD12 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_12);
    
    /* Insert delay */
	for(i=0;i<10;i++)
	{
		Delay(0x3FFFFF);
    }
    /* PD13 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_13);
    
    /* Insert delay */
    Delay(0x3FFFFF);
  
    /* PD14 to be toggled */
    GPIO_SetBits(GPIOD, GPIO_Pin_14);
    
    /* Insert delay */
    Delay(0x3FFFFF);
    
    /* PD15 to be toggled */
//    GPIO_SetBits(GPIOD, GPIO_Pin_15);
    
    /* Insert delay */
    Delay(0x7FFFFF);
    
    GPIO_ResetBits(GPIOD, GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);
    
    /* Insert delay */
    Delay(0xFFFFFF);
  }
}

/**
  * @brief  Delay Function.
  * @param  nCount:specifies the Delay time length.
  * @retval None
  */
void Delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
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
{:
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
