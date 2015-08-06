//ignor below JB john.broadbent.ky@gmail.com, CPU @8MHz APB @8MHz
// STM32F4 CAN1 TX Loop, requires RX Node, CPU @168MHz APB1 @42MHz - sourcer32@gmail.com
 
#include "stm32f4xx.h"
 
 #include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

//jb hmmmfasdfa
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_can.h"
//hmmm
 void Delay(__IO uint32_t nCount);

int main(void)
{
  GPIO_InitTypeDef      GPIO_InitStructure;
  RCC_ClocksTypeDef     RCC_Clocks;
  CAN_InitTypeDef       CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;
 
  CanTxMsg TxMessage;
 
  RCC_GetClocksFreq(&RCC_Clocks);
 
  /* CAN GPIOs configuration **************************************************/
  /* Enable GPIO clock */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
 
  /* Connect CAN pins */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);
 
  /* Configure CAN RX and TX pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
 
  /* CAN configuration ********************************************************/
  /* Enable CAN clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
 
  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);
 
  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
 
 
 
  /* quanta 1+6+7 = 14, 14 * 3 = 42, 42000000 / 42 = 1000000 */
  /* CAN Baudrate = 1Mbps (CAN clocked at 42 MHz) Prescale = 3 */
 
  /* Requires a clock with integer division into APB clock */
 
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq; // ? posible combos, I bet? // 1+6+7 = 14, 1+14+6 = 21, 1+15+5 = 21
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_7tq;
  //below is cleaver.  8MHz/14M = 0.571428571 = 0
  CAN_InitStructure.CAN_Prescaler = RCC_Clocks.PCLK1_Frequency / (14 * 1000000); // quanta by baudrate
 
  CAN_Init(CAN1, &CAN_InitStructure);
 
  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber = 0; // CAN1 [ 0..13]
 
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask; // IdMask or IdList
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit; // 16 or 32
 
  CAN_FilterInitStructure.CAN_FilterIdHigh      = 0x0000; // Everything, otherwise 11-bit in top bits
  CAN_FilterInitStructure.CAN_FilterIdLow       = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh  = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow   = 0x0000;
 
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0; // Rx
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
 
  CAN_FilterInit(&CAN_FilterInitStructure);
 
  // transmit */
  TxMessage.StdId = 0x123;
  TxMessage.ExtId = 0x00;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 8;
 
  TxMessage.Data[0] = 0x02;
  TxMessage.Data[1] = 0x11;
  TxMessage.Data[2] = 0x11;
  TxMessage.Data[3] = 0x11;
 
  while(1) // Do not want to exit
  {
    volatile uint32_t i;
    static int j = 0;
    uint8_t TransmitMailbox = 0;
 
    TxMessage.Data[4] = (j >>  0) & 0xFF; // Cycling
    TxMessage.Data[5] = (j >>  8) & 0xFF;
    TxMessage.Data[6] = (j >> 16) & 0xFF;
    TxMessage.Data[7] = (j >> 24) & 0xFF;
    j++;
 
    TransmitMailbox = CAN_Transmit(CAN1, &TxMessage);
 
    i = 0;
    while((CAN_TransmitStatus(CAN1, TransmitMailbox) != CANTXOK) && (i != 0xFFFFFF)) // Wait on Transmit
    {
      i++;
    }
  }
  Delay(0x7FFFFF);

  
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
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
 
  /* Infinite loop */
  while (1)
  {
  }
}
#endif