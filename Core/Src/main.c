/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
SDRAM_HandleTypeDef hsdram1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_FMC_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void flash_leds(void) {
  GPIO_PinState s1, s2;

  while (1) {

    s1 = HAL_GPIO_ReadPin(PB5_BOOT0_S1_GPIO_Port, PB5_BOOT0_S1_Pin);
    s2 = HAL_GPIO_ReadPin(PA10_S2_GPIO_Port, PA10_S2_Pin);

    if (s1) {
      HAL_GPIO_TogglePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin);
    } else {
      HAL_GPIO_TogglePin(PG6_GREEN_LED_GPIO_Port, PG6_GREEN_LED_Pin);
    }

    HAL_Delay(s2 ? 125 : 250);
  }
}

/*
 * Go through all the memory, write the address, read the address,
 * and turn on an LED based on the result.
 */
void simple_sdram_test(void) {

  // First show the LEDs for a second.
  HAL_GPIO_WritePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(PG6_GREEN_LED_GPIO_Port, PG6_GREEN_LED_Pin, GPIO_PIN_SET);
  HAL_Delay(500);
  HAL_GPIO_WritePin(PG6_GREEN_LED_GPIO_Port, PG6_GREEN_LED_Pin, GPIO_PIN_RESET);


  void *sd_base = (void *)0xC000000;
  // 32MB RAM, at 2 bytes per write = 16M writes
  uint32_t sd_len = 0x10000; // 0x2000000;
  uint16_t val_offset = 0xD0F1; // My initials, whee
  uint32_t good = 0;

  void *cur = sd_base;

  // First write
  while (cur < sd_base + sd_len) {
    *((volatile uint16_t *)cur) = val_offset + (uint16_t)((uint32_t)cur & 0xFFFF);
    cur = (void *)((uint32_t)cur + 2); // Go up by words since our memory is 16 bits wide
  }

  // Then read
  // TODO

  // Print status
  while (1) {
    if (good) {
      HAL_GPIO_TogglePin(PG6_GREEN_LED_GPIO_Port, PG6_GREEN_LED_Pin);
    } else {
      HAL_GPIO_TogglePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin);
    }
    HAL_Delay(200);
  }
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FMC_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // flash_leds();
    simple_sdram_test();
  }
  /* USER CODE END 3 */
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
 * @brief  FMC SDRAM Mode definition register defines
 * From: https://github.com/ksoloti/ksoloti/blob/c6305dd5c92fde1ad0d5dcc49afcf5de4b7e3a58/firmware/sdram.h#L96
 */
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)


/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_9;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_13;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_16;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_2;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_1;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 3;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 2;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  // Code from: https://community.st.com/t5/stm32-mcus/how-to-set-up-the-fmc-peripheral-to-interface-with-the-sdram/ta-p/49457
  // See section 2.6 SDRAM Initialization sequence

  // See also Ksoloti code: https://github.com/ksoloti/ksoloti/blob/master/firmware/sdram.c#L223
  // See SDRAM_InitSequence()

  FMC_SDRAM_CommandTypeDef Command;
  /* Step 1 and Step 2 already done in HAL_SDRAM_Init() */
  /* Step 3: Configure a clock configuration enable command */
  Command.CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE; /* Set MODE bits to "001" */
  Command.CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1; /* configure the Target Bank bits */
  Command.AutoRefreshNumber      = 1;
  Command.ModeRegisterDefinition = 0;

  /* Wait until the SDRAM controller is ready */
  // while (FMC_GetFlagStatus(FMC_SDRAM_BANK1, FMC_FLAG_Busy) != RESET);
  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
  // Try FMC_SDRAM_SendCommand too?

  /* Step 4: Insert 100 us minimum delay - Min HAL Delay is 1ms */
  HAL_Delay(1);

  /* Step 5: Configure a PALL (precharge all) command */
  Command.CommandMode            = FMC_SDRAM_CMD_PALL; /* Set MODE bits to "010" */
  // while (FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET);
  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);

  /* Step 6: Configure an Auto Refresh command... twice */
  Command.CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE; /* Set MODE bits to "011" */
  Command.AutoRefreshNumber      = 4;
  // while (FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET);
  HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);
  // while (FMC_GetFlagStatus(FMC_Bank1_SDRAM, FMC_FLAG_Busy) != RESET);
  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);

  /* Step 7: Program the external memory mode register */
  Command.CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;/*set the MODE bits to "100" */
  Command.AutoRefreshNumber      = 1;
  // Command.ModeRegisterDefinition =  (uint32_t)0 | 0<<3 | 2<<4 | 0<<7 | 1<<9;
  Command.ModeRegisterDefinition = SDRAM_MODEREG_BURST_LENGTH_2 |
    SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL |
    SDRAM_MODEREG_CAS_LATENCY_2 |
    SDRAM_MODEREG_OPERATING_MODE_STANDARD |
    SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;
  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_SendCommand(&hsdram1, &Command, 0xfff);

  /* Step 8: Set the refresh rate counter - refer to section SDRAM refresh timer register in RM0455 */
  /* Per our SDRAM:
   *
   * self refresh time = 64ms
   * number of rows = 8192
   * SDRAM Clock = 84MHz (168MHz / 2)
   *
   * COUNT = [(SDRAM self refresh time / number of row) x  SDRAM CLK] – 20
   *       = [(64ms / 8192) * 84MHz] - 20
   *       = 656.25 - 20
   *       = 646.25
   *       = ~646
   */
  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_ProgramRefreshRate(&hsdram1, 646); // KSoloti uses 683

  for (int i = 0; i < 100000; i++);
  HAL_SDRAM_WriteProtection_Disable(&hsdram1);

  /* USER CODE END FMC_Init 2 */
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PG6_GREEN_LED_GPIO_Port, PG6_GREEN_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PG6_GREEN_LED_Pin */
  GPIO_InitStruct.Pin = PG6_GREEN_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PG6_GREEN_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PC6_RED_LED_Pin */
  GPIO_InitStruct.Pin = PC6_RED_LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PC6_RED_LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA10_S2_Pin */
  GPIO_InitStruct.Pin = PA10_S2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PA10_S2_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5_BOOT0_S1_Pin */
  GPIO_InitStruct.Pin = PB5_BOOT0_S1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(PB5_BOOT0_S1_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
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

  // Turn on RED LED and leave it
  HAL_GPIO_WritePin(PC6_RED_LED_GPIO_Port, PC6_RED_LED_Pin, GPIO_PIN_SET);

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
