/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "cwpack.h"
#include "cwpack_internals.h"
#include "stm32l4xx_nucleo_144.h"
#include "LCD16x2.h"
#include <string.h>
#include <stdio.h>

/* Function Prototypes */
void Error_Handler(void);
void pack_telemetry();
void unpack_commands();
void send_telemetry();
void receive_commands();
void do_something_with_commands(int *);
void display_data_on_lcd();

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
