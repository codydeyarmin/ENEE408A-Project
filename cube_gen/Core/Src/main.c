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


/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/
__IO ITStatus UartReadyRX = RESET;
__IO ITStatus UartReadyTX = RESET;
UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;
#define NUMCOMMANDS                     5

/* Buffer used for transmission */
uint8_t aTxStartMessage[] = "\n\rGary is really cool\n\r408A";
uint8_t flushSequence[] = "408A";
int newCommands[NUMCOMMANDS];
int errorType = -1;

/* Buffer used for reception */
#define TIMEOUT                           5
#define RXBUFFERSIZE                      16
#define TXBUFFERSIZE                      30
uint8_t aRxBuffer[RXBUFFERSIZE];
uint8_t aTxBuffer[TXBUFFERSIZE];
uint8_t unpackBuffer[1 + (NUMCOMMANDS*3) + 1];

/* Size of Transmission buffer */
#define TXSTARTMESSAGESIZE               (strlen((char *) aTxStartMessage) - 1)
#define TXENDMESSAGESIZE                 (strlen((char *) aTxEndMessage) - 1)

/* Global contexts */
cw_pack_context pc;
cw_unpack_context uc;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_USART1_UART_Init(void);


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
	BSP_LED_Init(LED1); // RED LED
	BSP_LED_Init(LED2); // BLUE LED
	BSP_LED_Init(LED3); // GREEN LED

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_USART1_UART_Init();

	/* Configure timeout feature for UART */
//	HAL_UART_ReceiverTimeout_Config(&huart1, TIMEOUT);
//	HAL_UART_EnableReceiverTimeout(&huart1);

	/* Initialize LCD screen */
	LCD_Init();
	LCD_Clear();
	LCD_Set_Cursor(1, 1);
	LCD_Write_String("  Gary is  ");
	LCD_Set_Cursor(2, 1);
	LCD_Write_String("very cool!!!");

	/* MAIN CONTROL LOOP FOR COMMUNICATION */
	//cw_pack_context pc;
	//cw_unpack_context uc;
	//uint8_t aRxBuffer[RXBUFFERSIZE];
	//uint8_t *aRxBuffer;
	//int * newCommands;
	while (1) {
		BSP_LED_Off(LED2); // LED OFF

		// SEND TELEMETRY DATA
		pack_telemetry();
		send_telemetry();

		BSP_LED_On(LED2); // LED ON

		// RECEIVE DATA (if any)
		receive_commands();
		do_something_with_commands(newCommands);
	}
}

/* Pack telemetry data in order to send packed data to VR team */
void pack_telemetry() {
	// generate values for now (6 integers - ax, ay, az, gx, gy, gz)
	//cw_pack_context pc;
	char buffer[TXBUFFERSIZE];
	int num = 45;

	cw_pack_context_init(&pc, buffer, TXBUFFERSIZE, 0);
	cw_pack_array_size(&pc, 6);
	for (int i = 0; i < 6; i++) {
		cw_pack_unsigned(&pc, num);
		num = (num * 0x78282) % 1024;
	}

	//length = pc.current - pc.start;
	//if (length != 24) Error_Handler();
	//return pc;
}

/* Unpack data received ---> integer array*/
//void unpack_commands(uint8_t * recvBuffer) {
void unpack_commands() {
	// TODO - need to decide what the format of this stuff is
	//int receivedData[5]; // Controls: pitch, yaw, roll --- Gimble: left/right, up/down
	int * ptr = newCommands;
	int numItems = 0;
	//cw_unpack_context uc;
	cw_unpack_context_init(&uc, (char *) unpackBuffer, sizeof(unpackBuffer), 0);
	cw_unpack_next(&uc); // get array length
	numItems = uc.item.as.i64;
	//newCommands = uc.item.as.array;
	for (int i = 0; i < numItems; i++) {
		cw_unpack_next(&uc);
		if (uc.item.type == CWP_ITEM_POSITIVE_INTEGER) {
			*(ptr++) = uc.item.as.u64;
		} else if (uc.item.type == CWP_ITEM_NEGATIVE_INTEGER) {
			*(ptr++) = uc.item.as.i64;
		} else {
			errorType = 1;
			// TODO - figure out what exactly is breaking here
			break;
			//Error_Handler(); // error if not pos/neg integer
		}

		if ((int) uc.current == 0xcc || (int) uc.current == 0xd0) {
			cw_unpack_next(&uc);
		}
	}
	//return receivedData;
}

/* USART-TX: Send the packed telemetry */
void send_telemetry() {
	//uint8_t * sendBuffer;
	size_t length = pc.current - pc.start; // add 4 bytes to size for flush sequence

	// String operations
	//sendBuffer = malloc(length + 4);
	strncpy((char *) aTxBuffer, (char *) pc.start, length);
	strncpy((char *) aTxBuffer + length, (char *) flushSequence, 4);

	// Wait for current peripheral process to finish
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

	// send that shit
	BSP_LED_On(LED2);
	if(HAL_UART_Transmit_DMA(&huart1, (uint8_t*) aTxBuffer, length+4) != HAL_OK) {
		errorType = 2;
		Error_Handler(); /* Transfer error in transmission process */
	}

	// Wait for current peripheral process to finish
	//while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);
	while(UartReadyTX != SET);
	UartReadyTX = RESET;
}

/* USART-RX: Receive any incoming commands */
void receive_commands() {
	//uint8_t receivedData[RXBUFFERSIZE]
	int numWaited = 0;
	int offset = 0, nextOffset = 0;

	// Wait for current peripheral process to finish
	while (HAL_UART_GetState(&huart1) != HAL_UART_STATE_READY);

	BSP_LED_Off(LED3);
	if (HAL_UART_Receive_DMA(&huart1, (uint8_t *) aRxBuffer, RXBUFFERSIZE) != HAL_OK) {
		errorType = 3;
		return;
		//Error_Handler(); /* Transfer error in reception process */
	}

	while (UartReadyRX != SET);
	UartReadyRX = RESET;

	if (strlen((char *) aRxBuffer) > 4) {
		while(((int) *(aRxBuffer + offset)) != 0x95) {
			++offset;
		}
		strncpy(unpackBuffer, aRxBuffer+offset, 2+(NUMCOMMANDS*3));
		*(unpackBuffer+2+(NUMCOMMANDS*3)) = 0;
		unpack_commands();
		display_data_on_lcd();
	}
}

void display_data_on_lcd() {
	char lcd_buf[5];

	// TODO - turn commands into string - newCommands
	LCD_Init();
	LCD_Clear();

	LCD_Set_Cursor(1, 1);
	LCD_Write_String("Gx: ");
	sprintf(lcd_buf, "%d", newCommands[0]);
	LCD_Write_String(lcd_buf);

	LCD_Set_Cursor(1, 10);
	LCD_Write_String("Gy: ");
	sprintf(lcd_buf, "%d", newCommands[1]);
	LCD_Write_String(lcd_buf);

	LCD_Set_Cursor(2, 1);
	sprintf(lcd_buf, "%d", newCommands[2]);
	LCD_Write_String(lcd_buf);
	LCD_Set_Cursor(2, 7);
	//LCD_Write_String(" ");
	sprintf(lcd_buf, "%d", newCommands[3]);
	LCD_Write_String(lcd_buf);
	LCD_Set_Cursor(2, 13);
	//LCD_Write_String(" ");
	sprintf(lcd_buf, "%d", newCommands[4]);
	LCD_Write_String(lcd_buf);
}

void do_something_with_commands(int * commands) {
	// for @codydeyarmin to implement for drone control
	// for @VRteam and @Akiva to implement for gimble control
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK) {
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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_MSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
    Error_Handler();
  }
}

/**
  * @brief  Tx Transfer completed callback
  * @param  UartHandle: UART handle.
  * @note   This example shows a simple way to report end of DMA Tx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle) {
  /* Set transmission flag: transfer complete*/
  UartReadyTX = SET;
  /* Turn LED1 on: Transfer in transmission process is correct */
  BSP_LED_On(LED2);
}

/**
  * @brief  Rx Transfer completed callback
  * @param  UartHandle: UART handle
  * @note   This example shows a simple way to report end of DMA Rx transfer, and
  *         you can add your own implementation.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle) {
  /* Set transmission flag: transfer complete*/
  UartReadyRX = SET;

  /* Turn LED1 on: Transfer in reception process is correct */
  BSP_LED_On(LED3);
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void) {
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
  if (HAL_UART_DeInit(&huart1) != HAL_OK) {
	  Error_Handler();
  }
  if (HAL_UART_Init(&huart1) != HAL_OK) {
	  Error_Handler();
  }
}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) {
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
static void MX_GPIO_Init(void) {

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void){
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
