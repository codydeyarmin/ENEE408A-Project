/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32l4xx_nucleo_144.h"
#include <string.h>

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USER CODE BEGIN PV */
/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\r ****UART-Hyperterminal communication based on DMA****\n\r Enter 10 characters using keyboard :\n\r 408A";
uint8_t aTxEndMessage[] = "\n\r Example Finished\n\r";

/* Buffer used for reception */
#define RXBUFFERSIZE                      10
uint8_t aRxBuffer[RXBUFFERSIZE];

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE               (strlen((char *) aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (strlen((char *) aTxEndMessage) - 1)

/* Size of Reception buffer */


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void) {
	/* MCU Configuration--------------------------------------------------------*/
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* Configure the system clock */
	SystemClock_Config();
	BSP_LED_Init(LED2); // BLUE

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();

	//while (1);

	while (1) {
		/*##-1- Start the transmission process #####################################*/
		/* User start transmission data through "TxBuffer" buffer */
		if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)aTxStartMessage, TXSTARTMESSAGESIZE)!= HAL_OK) {
			Error_Handler(); /* Transfer error in transmission process */
		}

		/*##-2- Put UART peripheral in reception process ###########################*/
		/* Any data received will be stored in "aRxBuffer" buffer : the number max of
		   data received is 10 */
		//if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) {
		//	Error_Handler(); /* Transfer error in reception process */
		//}

		/*##-3- Wait for the end of the transfer ###################################*/
		/*  Before starting a new communication transfer, you need to check the current
			state of the peripheral; if it's busy you need to wait for the end of current
			transfer before starting a new one.
			For simplicity reasons, this example is just waiting till the end of the
			transfer, but application may perform other tasks while transfer operation
			is ongoing. */
		while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

		/*##-4- Send the received Buffer ###########################################*/
		//if (HAL_UART_Transmit_DMA(&huart1, (uint8_t *)aRxBuffer, RXBUFFERSIZE) != HAL_OK) {
		//	Error_Handler(); /* Transfer error in transmission process */
		//}

		/*##-5- Wait for the end of the transfer ###################################*/
		/*  Before starting a new communication transfer, you need to check the current
			state of the peripheral; if it's busy you need to wait for the end of current
			transfer before starting a new one.
			For simplicity reasons, this example is just waiting till the end of the
			transfer, but application may perform other tasks while transfer operation
			is ongoing. */
		//while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

		/*##-6- Send the End Message ###############################################*/
		//if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*)aTxEndMessage, TXENDMESSAGESIZE)!= HAL_OK) {
		//	Error_Handler(); /* Transfer error in transmission process */
		//}

		/*##-7- Wait for the end of the transfer ###################################*/
		//while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

		/* Turn on LED2 if test passes then enter infinite loop */
		BSP_LED_On(LED2); // BLUE
		HAL_Delay(100);
		BSP_LED_Off(LED2); // BLUE
	}
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  //huart1.Init.BaudRate = 38400;
  huart1.Init.BaudRate = 74880;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  //huart1.Init.Parity = UART_PARITY_ODD;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel4_IRQn);
  /* DMA1_Channel5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
