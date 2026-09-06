/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "monitor.h"
#include "events.h"
#include "keep_alive.h"
#include "watchdog.h"
#include "comm.h"
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
/* USER CODE BEGIN Variables */
osThreadId_t monitorTaskHandle;
osThreadId_t eventTaskHandle;
osThreadId_t keepAliveTaskHandle;
osThreadId_t watchdogTaskHandle;
osThreadId_t commTaskHandle;

const osThreadAttr_t monitorTask_attributes = {
    .name       = "monitorTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t)osPriorityNormal
};

const osThreadAttr_t eventTask_attributes = {
    .name       = "eventTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t)osPriorityNormal
};

const osThreadAttr_t keepAliveTask_attributes = {
    .name       = "keepAliveTask",
    .stack_size = 256 * 4,
    .priority   = (osPriority_t)osPriorityNormal
};

const osThreadAttr_t watchdogTask_attributes = {
    .name       = "watchdogTask",
    .stack_size = 256 * 4,
    .priority   = (osPriority_t)osPriorityNormal
};
const osThreadAttr_t commTask_attributes = {
    .name       = "commTask",
    .stack_size = 512 * 4,
    .priority   = (osPriority_t)osPriorityAboveNormal
};
/* USER CODE END Variables */
/* Definitions for defaultTask */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void)
{
    /* USER CODE BEGIN Init */
    Event_Init();
    KeepAlive_Init();
    Monitor_Init();
    Comm_Init();
    /* USER CODE END Init */

    /* USER CODE BEGIN RTOS_MUTEX */
    /* USER CODE END RTOS_MUTEX */

    /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* USER CODE END RTOS_SEMAPHORES */

    /* USER CODE BEGIN RTOS_TIMERS */
    /* USER CODE END RTOS_TIMERS */

    /* USER CODE BEGIN RTOS_QUEUES */
    /* USER CODE END RTOS_QUEUES */

    /* USER CODE BEGIN RTOS_THREADS */
    monitorTaskHandle   = osThreadNew(Monitor_Task,   NULL, &monitorTask_attributes);
    eventTaskHandle     = osThreadNew(Event_Task,     NULL, &eventTask_attributes);
    keepAliveTaskHandle = osThreadNew(KeepAlive_Task, NULL, &keepAliveTask_attributes);
    watchdogTaskHandle  = osThreadNew(Watchdog_Task,  NULL, &watchdogTask_attributes);
    commTaskHandle      = osThreadNew(Comm_Task,      NULL, &commTask_attributes);
    /* USER CODE END RTOS_THREADS */

    /* USER CODE BEGIN RTOS_EVENTS */
    /* USER CODE END RTOS_EVENTS */
}

/* USER CODE BEGIN Header_StartDefaultTask */
/* USER CODE END Header_StartDefaultTask */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

