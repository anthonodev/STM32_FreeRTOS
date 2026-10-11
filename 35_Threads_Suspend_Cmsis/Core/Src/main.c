#include "main.h"
#include "cmsis_os.h"
#include "stdio.h"

#define STACK_SIZE 512

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

int uart2_write(int ch)
	{
	/*Make sure the transmit data register is empty*/
	while(!(USART2->SR & USART_SR_TXE)){}

	 /*Write to transmit data register*/
	USART2->DR	=  (ch & 0xFF);
	return ch;
	}

int __io_putchar(int ch)
	{
	uart2_write(ch);
	return ch;
	}

void vBlueLedControllerTask(void *pvParameters);
void vRedLedControllerTask(void *pvParameters);
void vOrangeLedControllerTask(void *pvParameters);
void vGreenLedControllerTask(void *pvParameters);

typedef uint32_t TaskProfiler_t;

TaskProfiler_t Orange_TaskProfiler, Red_TaskProfiler, Green_TaskProfiler, Blue_TaskProfiler;

osThreadId_t red_id, green_id, blue_id, orange_id;

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  //Initialize kernel
  osKernelInitialize();

  //Create threads
  osThreadAttr_t greenThreadAttribs = {
		  .name = "GreenTask",
		  .stack_size = STACK_SIZE,
		  .priority = osPriorityNormal
  };

  green_id = osThreadNew(vGreenLedControllerTask, NULL, &greenThreadAttribs);

  osThreadAttr_t redThreadAttribs = {
  		  .name = "RedTask",
  		  .stack_size = STACK_SIZE,
  		  .priority = osPriorityNormal
  };

  red_id = osThreadNew(vRedLedControllerTask, NULL, &redThreadAttribs);

  osThreadAttr_t orangeThreadAttribs = {
		  .name = "OrangeTask",
		  .stack_size = STACK_SIZE,
		  .priority = osPriorityNormal
	};

  orange_id = osThreadNew(vOrangeLedControllerTask, NULL, &orangeThreadAttribs);

  osThreadAttr_t blueThreadAttribs = {
		  .name = "BlueTask",
		  .stack_size = STACK_SIZE,
		  .priority = osPriorityNormal
	};

  blue_id = osThreadNew(vBlueLedControllerTask, NULL, &blueThreadAttribs);

  //Start kernel
  osKernelStart();

  while (1)
  {

  }

}

void vBlueLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		Blue_TaskProfiler++;
		for(i=0;i<700000;i++){}
	}
}

uint32_t suspend_monitor;

void vRedLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		Red_TaskProfiler++;
		for(i=0;i<700000;i++){}
		suspend_monitor++;
		if(suspend_monitor >= 49)
		{
			osThreadSuspend(blue_id);
		}
	}
}

void vOrangeLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		Orange_TaskProfiler++;
		for(i=0;i<700000;i++){}
	}
}

void vGreenLedControllerTask(void *pvParameters)
{
	int i;
	while(1)
	{
		Green_TaskProfiler++;
		for(i=0;i<700000;i++){}
	}
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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

