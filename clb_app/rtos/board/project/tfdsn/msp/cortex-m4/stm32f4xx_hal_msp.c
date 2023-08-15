/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file         stm32f4xx_hal_msp.c
 * @brief        This file provides code for the MSP Initialization
 *               and de-Initialization codes.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "rtconfig.h"
#if  defined(STM32F4XX)
#include "drv_common.h"
#include "stm32f4xx_hal.h"
#ifdef LH_TFDSN
#include "tfdsn_io_config.h"
#endif
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
/**
 * Initializes the Global MSP.
 */
void HAL_MspInit(void)
{
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END MspInit 0 */

	__HAL_RCC_SYSCFG_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();

	/* System interrupt init*/

	/* USER CODE BEGIN MspInit 1 */

	/* USER CODE END MspInit 1 */
}

#if defined(ENABLE_CAN) && (defined(ENABLE_CAN1) || defined(ENABLE_CAN1))
static uint32_t HAL_RCC_CAN1_CLK_ENABLED = 0;

/**
 * @brief CAN MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
#if defined(ENABLE_CAN1)
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* Peripheral clock enable */
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1)
    {
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    GPIO_InitStruct.Pin = CAN1_RX_PIN|CAN1_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN1;
    HAL_GPIO_Init(CAN1_GPIO_BANK, &GPIO_InitStruct);
#if defined(ENABLE_CAN1_IRQ)
    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(CAN1_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_TX_IRQn);

    HAL_NVIC_SetPriority(CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX0_IRQn);

    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);

    HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
#endif
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
#endif
#if defined(ENABLE_CAN2)
#if defined(ENABLE_CAN1)
  else
#endif
	  if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspInit 0 */

  /* USER CODE END CAN2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_CAN2_CLK_ENABLE();
    HAL_RCC_CAN1_CLK_ENABLED++;
    if(HAL_RCC_CAN1_CLK_ENABLED==1)
    {
      __HAL_RCC_CAN1_CLK_ENABLE();
    }

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    GPIO_InitStruct.Pin = CAN2_RX_PIN|CAN2_TX_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_CAN2;
    HAL_GPIO_Init(CAN2_GPIO_BANK, &GPIO_InitStruct);

#if defined(ENABLE_CAN2_IRQ)
    /* CAN2 interrupt Init */
    HAL_NVIC_SetPriority(CAN2_TX_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN2_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_SetPriority(CAN2_SCE_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN2_SCE_IRQn);
#endif
  /* USER CODE BEGIN CAN2_MspInit 1 */

  /* USER CODE END CAN2_MspInit 1 */
  }
#endif

}

/**
 * @brief CAN MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hcan: CAN handle pointer
 * @retval None
 */
void HAL_CAN_MspDeInit(CAN_HandleTypeDef *hcan)
{
#if defined(ENABLE_CAN1)
  if(hcan->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0)
    {
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN1 GPIO Configuration
    PA11     ------> CAN1_RX
    PA12     ------> CAN1_TX
    */
    HAL_GPIO_DeInit(CAN1_GPIO_BANK, CAN1_RX_PIN|CAN1_TX_PIN);
#if defined(ENABLE_CAN1_IRQ)
    /* CAN1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN1_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN1_SCE_IRQn);
#endif
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
#endif
#if defined(ENABLE_CAN2)
#if defined(ENABLE_CAN1)
  else
#endif
  if(hcan->Instance==CAN2)
  {
  /* USER CODE BEGIN CAN2_MspDeInit 0 */

  /* USER CODE END CAN2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN2_CLK_DISABLE();
    HAL_RCC_CAN1_CLK_ENABLED--;
    if(HAL_RCC_CAN1_CLK_ENABLED==0){
      __HAL_RCC_CAN1_CLK_DISABLE();
    }

    /**CAN2 GPIO Configuration
    PB12     ------> CAN2_RX
    PB13     ------> CAN2_TX
    */
    HAL_GPIO_DeInit(CAN2_GPIO_BANK, CAN2_RX_PIN|CAN2_TX_PIN);
#if defined(ENABLE_CAN2_IRQ)
    /* CAN2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(CAN2_TX_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX0_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_RX1_IRQn);
    HAL_NVIC_DisableIRQ(CAN2_SCE_IRQn);
#endif
  /* USER CODE BEGIN CAN2_MspDeInit 1 */

  /* USER CODE END CAN2_MspDeInit 1 */
  }
#endif
}
#endif //(ENABLE_CAN)

#if defined(ENABLE_ETH)
/**
 * @brief ETH MSP Initialization
 * This function configures the hardware resources used in this example
 * @param heth: ETH handle pointer
 * @retval None
 */
void HAL_ETH_MspInit(ETH_HandleTypeDef *heth)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
	if (heth->Instance == ETH)
	{
		/* USER CODE BEGIN ETH_MspInit 0 */

		/* USER CODE END ETH_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_ETH_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOA_CLK_ENABLE();
#if defined(LH_TFDSN_CLB)
		/**ETH GPIO Configuration
		 PC1     ------> ETH_MDC
		 PA1     ------> ETH_REF_CLK
		 PA2     ------> ETH_MDIO
		 PA7     ------> ETH_CRS_DV
		 PC4     ------> ETH_RXD0
		 PC5     ------> ETH_RXD1
		 PG11     ------> ETH_TX_EN
		 PG13     ------> ETH_TXD0
		 PG14     ------> ETH_TXD1
		 */
		__HAL_RCC_GPIOG_CLK_ENABLE();
#elif defined(LH_TFDSN_CLB)
	    /**ETH GPIO Configuration
	    PC1     ------> ETH_MDC
	    PA1     ------> ETH_REF_CLK
	    PA2     ------> ETH_MDIO
	    PA7     ------> ETH_CRS_DV
	    PC4     ------> ETH_RXD0
	    PC5     ------> ETH_RXD1
	    PB11     ------> ETH_TX_EN
	    PB12     ------> ETH_TXD0
	    PB13     ------> ETH_TXD1
	    */
		__HAL_RCC_GPIOB_CLK_ENABLE();
#endif

		GPIO_InitStruct.Pin = ETH_MDC | ETH_RXD0 | ETH_RXD1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(ETH_GPIO_BANKC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = ETH_REF_CLK | ETH_MDIO | ETH_CRS_DV;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(ETH_GPIO_BANKA, &GPIO_InitStruct);

#if defined(LH_TFDSN_CLB)
		GPIO_InitStruct.Pin = ETH_TX_EN | ETH_TXD0 | ETH_TXD1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(ETH_GPIO_BANKG, &GPIO_InitStruct);
#elif defined(LH_TFDSN_ZXB)
		GPIO_InitStruct.Pin = ETH_TX_EN | ETH_TXD0 | ETH_TXD1;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF11_ETH;
		HAL_GPIO_Init(ETH_GPIO_BANKB, &GPIO_InitStruct);
#endif

#if defined(ENABLE_ETH_IRQ)
	/* ETH interrupt Init */
	HAL_NVIC_SetPriority(ETH_IO_IRQ, 0, 0);
	HAL_NVIC_EnableIRQ(ETH_IO_IRQ);
	HAL_NVIC_SetPriority(ETH_IO_WKUP_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ETH_IO_WKUP_IRQn);
#endif
		/* USER CODE BEGIN ETH_MspInit 1 */
		/* USER CODE END ETH_MspInit 1 */
	}

}

/**
 * @brief ETH MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param heth: ETH handle pointer
 * @retval None
 */
void HAL_ETH_MspDeInit(ETH_HandleTypeDef *heth)
{
	if (heth->Instance == ETH)
	{
		/* USER CODE BEGIN ETH_MspDeInit 0 */

		/* USER CODE END ETH_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ETH_CLK_DISABLE();

		/**ETH GPIO Configuration
		 PC1     ------> ETH_MDC
		 PA1     ------> ETH_REF_CLK
		 PA2     ------> ETH_MDIO
		 PA7     ------> ETH_CRS_DV
		 PC4     ------> ETH_RXD0
		 PC5     ------> ETH_RXD1
		 PG11     ------> ETH_TX_EN
		 PG13     ------> ETH_TXD0
		 PG14     ------> ETH_TXD1
		 */
		HAL_GPIO_DeInit(ETH_GPIO_BANKC, ETH_MDC | ETH_RXD0 | ETH_RXD1);

		HAL_GPIO_DeInit(ETH_GPIO_BANKA, ETH_REF_CLK | ETH_MDIO | ETH_CRS_DV);

#if defined(LH_TFDSN_CLB)
		HAL_GPIO_DeInit(ETH_GPIO_BANKG, ETH_TX_EN | ETH_TXD0 | ETH_TXD1);
#elif defined(LH_TFDSN_ZXB)
		HAL_GPIO_DeInit(ETH_GPIO_BANKB, ETH_TX_EN | ETH_TXD0 | ETH_TXD1);
#endif

#if defined(ENABLE_ETH_IRQ)
	/* ETH interrupt DeInit */
	HAL_NVIC_DisableIRQ(ETH_IO_IRQ);
	HAL_NVIC_DisableIRQ(ETH_IO_WKUP_IRQn);
#endif
		/* USER CODE BEGIN ETH_MspDeInit 1 */

		/* USER CODE END ETH_MspDeInit 1 */
	}

}
#endif //ENABLE_ETH

#ifdef ENABLE_RTC
/**
* @brief RTC MSP Initialization
* This function configures the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_RTC_ENABLE();
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }

}

/**
* @brief RTC MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hrtc: RTC handle pointer
* @retval None
*/
void HAL_RTC_MspDeInit(RTC_HandleTypeDef* hrtc)
{
  if(hrtc->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }

}
#endif




#if defined(ENABLE_I2C)

#ifdef ENABLE_I2C1_RX_DMA
extern DMA_HandleTypeDef hdma_i2c1_rx;
#endif
#ifdef ENABLE_I2C1_TX_DMA
extern DMA_HandleTypeDef hdma_i2c1_tx;
#endif
/**
* @brief I2C MSP Initialization
* This function configures the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* Peripheral clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();

#if defined(ENABLE_I2C1_RX_DMA)
    /* I2C1 DMA Init */
    /* I2C1_RX Init */
    hdma_i2c1_rx.Instance = DMA1_Stream0;
    hdma_i2c1_rx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_rx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(hi2c,hdmarx,hdma_i2c1_rx);
#endif

#if defined(ENABLE_I2C1_TX_DMA)
    /* I2C1_TX Init */
    hdma_i2c1_tx.Instance = DMA1_Stream6;
    hdma_i2c1_tx.Init.Channel = DMA_CHANNEL_1;
    hdma_i2c1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode = DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_i2c1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_i2c1_tx) != HAL_OK)
    {
      Error_Handler();
    }
    __HAL_LINKDMA(hi2c,hdmatx,hdma_i2c1_tx);
#endif

#if defined(ENABLE_I2C1_IRQ)
    /* I2C1 interrupt Init */
    HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
#endif
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }

}

/**
* @brief I2C MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hi2c: I2C handle pointer
* @retval None
*/
void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
  if(hi2c->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();

    /**I2C1 GPIO Configuration
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6);

    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_7);

#if defined(ENABLE_I2C1_RX_DMA)
    /* I2C1 DMA DeInit */
    HAL_DMA_DeInit(hi2c->hdmarx);
    HAL_DMA_DeInit(hi2c->hdmatx);
#endif

#if defined(ENABLE_I2C1_IRQ)
    /* I2C1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
#endif
  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }

}

#endif //#if defined(ENABLE_I2C)

#if defined(ENABLE_SDIO)
#if defined(SDIO_ENABLE_DMA)
extern DMA_HandleTypeDef hdma_sdio;
#endif
/**
 * @brief SD MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hsd: SD handle pointer
 * @retval None
 */
void HAL_SD_MspInit(SD_HandleTypeDef *hsd)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
	if (hsd->Instance == SDIO)
	{
		/* USER CODE BEGIN SDIO_MspInit 0 */

		/* USER CODE END SDIO_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_SDIO_CLK_ENABLE();

		__HAL_RCC_GPIOC_CLK_ENABLE();
		__HAL_RCC_GPIOD_CLK_ENABLE();
		/**SDIO GPIO Configuration
		 PC8     ------> SDIO_D0
		 PC9     ------> SDIO_D1
		 PC10     ------> SDIO_D2
		 PC11     ------> SDIO_D3
		 PC12     ------> SDIO_CK
		 PD2     ------> SDIO_CMD
		 */
		GPIO_InitStruct.Pin = SDIO_D0 | SDIO_D1 | SDIO_D2 | SDIO_D3 | SDIO_CK;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
		HAL_GPIO_Init(SDIO_GPIO_BANKC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = SDIO_CMD;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF12_SDIO;
		HAL_GPIO_Init(SDIO_GPIO_BANKD, &GPIO_InitStruct);
#if defined(SDIO_ENABLE_DMA)
    /* SDIO DMA Init */
    /* SDIO Init */
    hdma_sdio.Instance = DMA2_Stream3;
    hdma_sdio.Init.Channel = DMA_CHANNEL_4;
    hdma_sdio.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_sdio.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_sdio.Init.MemInc = DMA_MINC_ENABLE;
    hdma_sdio.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    hdma_sdio.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    hdma_sdio.Init.Mode = DMA_PFCTRL;
    hdma_sdio.Init.Priority = DMA_PRIORITY_LOW;
    hdma_sdio.Init.FIFOMode = DMA_FIFOMODE_ENABLE;
    hdma_sdio.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL;
    hdma_sdio.Init.MemBurst = DMA_MBURST_INC4;
    hdma_sdio.Init.PeriphBurst = DMA_PBURST_INC4;
    if (HAL_DMA_Init(&hdma_sdio) != HAL_OK)
    {
      Error_Handler();
    }

    /* Several peripheral DMA handle pointers point to the same DMA handle.
     Be aware that there is only one stream to perform all the requested DMAs. */
    /* Be sure to change transfer direction before calling
     HAL_SD_ReadBlocks_DMA or HAL_SD_WriteBlocks_DMA. */
    __HAL_LINKDMA(hsd,hdmarx,hdma_sdio);
    __HAL_LINKDMA(hsd,hdmatx,hdma_sdio);
#endif

#if defined(SDIO_ENABLE_IRQ)
    /* SDIO interrupt Init */
    HAL_NVIC_SetPriority(SDIO_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SDIO_IRQn);
#endif
		/* USER CODE BEGIN SDIO_MspInit 1 */

		/* USER CODE END SDIO_MspInit 1 */
	}

}

/**
 * @brief SD MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hsd: SD handle pointer
 * @retval None
 */
void HAL_SD_MspDeInit(SD_HandleTypeDef *hsd)
{
	if (hsd->Instance == SDIO)
	{
		/* USER CODE BEGIN SDIO_MspDeInit 0 */

		/* USER CODE END SDIO_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_SDIO_CLK_DISABLE();

		/**SDIO GPIO Configuration
		 PC8     ------> SDIO_D0
		 PC9     ------> SDIO_D1
		 PC10     ------> SDIO_D2
		 PC11     ------> SDIO_D3
		 PC12     ------> SDIO_CK
		 PD2     ------> SDIO_CMD
		 */
		HAL_GPIO_DeInit(SDIO_GPIO_BANKC, SDIO_D0 | SDIO_D1 | SDIO_D2 | SDIO_D3 | SDIO_CK);

		HAL_GPIO_DeInit(SDIO_GPIO_BANKD, SDIO_CMD);
#if defined(SDIO_ENABLE_DMA)
    /* SDIO DMA DeInit */
    HAL_DMA_DeInit(hsd->hdmarx);
    HAL_DMA_DeInit(hsd->hdmatx);
#endif

#if defined(SDIO_ENABLE_IRQ)
    /* SDIO interrupt DeInit */
    HAL_NVIC_DisableIRQ(SDIO_IRQn);
#endif
		/* USER CODE BEGIN SDIO_MspDeInit 1 */

		/* USER CODE END SDIO_MspDeInit 1 */
	}

}

#endif //defined(ENABLE_SDIO)

#if defined(ENABLE_SPI)

#if defined (SPI1_ENABLE_DMA)
		#if defined (SPI1_ENABLE_RX_DMA)
		extern DMA_HandleTypeDef hdma_spi1_rx;
		#endif

		#if defined (SPI1_ENABLE_TX_DMA)
		extern DMA_HandleTypeDef hdma_spi1_tx;
		#endif
	#endif
/**
 * @brief SPI MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
	if (hspi->Instance == SPI1)
	{
		/* USER CODE BEGIN SPI1_MspInit 0 */

		/* USER CODE END SPI1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_SPI1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOB_CLK_ENABLE();
		/**SPI1 GPIO Configuration
		 PA5     ------> SPI1_SCK
		 PA6     ------> SPI1_MISO
		 PB5     ------> SPI1_MOSI
		 */
		GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

#if defined (SPI1_ENABLE_RX_DMA)
    /* SPI1 DMA Init */
    /* SPI1_RX Init */
    hdma_spi1_rx.Instance = DMA2_Stream0;
    hdma_spi1_rx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_spi1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_rx.Init.Mode = DMA_NORMAL;
    hdma_spi1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmarx,hdma_spi1_rx);
#endif
#if defined (SPI1_ENABLE_TX_DMA)
    /* SPI1_TX Init */
    hdma_spi1_tx.Instance = DMA2_Stream5;
    hdma_spi1_tx.Init.Channel = DMA_CHANNEL_3;
    hdma_spi1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_spi1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_spi1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_spi1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_spi1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_spi1_tx.Init.Mode = DMA_NORMAL;
    hdma_spi1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_spi1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_spi1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(hspi,hdmatx,hdma_spi1_tx);
#endif

#if defined (SPI1_ENABLE_IRQ)
    /* SPI1 interrupt Init */
    HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(SPI1_IRQn);
#endif
		/* USER CODE BEGIN SPI1_MspInit 1 */

		/* USER CODE END SPI1_MspInit 1 */
	}

}

/**
 * @brief SPI MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param hspi: SPI handle pointer
 * @retval None
 */
void HAL_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	if (hspi->Instance == SPI1)
	{
		/* USER CODE BEGIN SPI1_MspDeInit 0 */

		/* USER CODE END SPI1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_SPI1_CLK_DISABLE();

		/**SPI1 GPIO Configuration
		 PA5     ------> SPI1_SCK
		 PA6     ------> SPI1_MISO
		 PB5     ------> SPI1_MOSI
		 */
		HAL_GPIO_DeInit(SPI1_GPIO_BANKA, SPI1_SCK | SPI1_MISO);

		HAL_GPIO_DeInit(SPI1_GPIO_BANKB, SPI1_MOSI);
#if defined (SPI1_ENABLE_RX_DMA)
    /* SPI1 DMA DeInit */
    HAL_DMA_DeInit(hspi->hdmarx);
#endif

#if defined (SPI1_ENABLE_TX_DMA)
    HAL_DMA_DeInit(hspi->hdmatx);
#endif

#if defined (SPI1_ENABLE_IRQ)
    /* SPI1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(SPI1_IRQn);
#endif
		/* USER CODE BEGIN SPI1_MspDeInit 1 */

		/* USER CODE END SPI1_MspDeInit 1 */
	}

}

#endif //#if defined(ENABLE_SPI)

//void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
//{
//#ifdef ENABLE_TIM1
//
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//  if(htim_base->Instance==TIM1)
//  {
//  /* USER CODE BEGIN TIM1_MspInit 0 */
//
//  /* USER CODE END TIM1_MspInit 0 */
//    /* Peripheral clock enable */
//    __HAL_RCC_TIM1_CLK_ENABLE();
//#if 1
//    __HAL_RCC_GPIOA_CLK_ENABLE();
//
//    /**TIM1 GPIO Configuration
//    PA9     ------> TIM1_CH2
//    */
//    GPIO_InitStruct.Pin = TIM1_CH1_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(TIM1_CH1_BANK, &GPIO_InitStruct);
//#endif
//
////#ifdef ENABLE_TIM1_IRQ
//    /* TIM1 interrupt Init */
//    HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
//    HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
//    HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
////#endif
//
//  /* USER CODE BEGIN TIM1_MspInit 1 */
//
//  /* USER CODE END TIM1_MspInit 1 */
//  }
//#endif
//
//#if 0
//#ifdef ENABLE_TIM2
//#ifdef ENABLE_TIM1
//  else
//#endif
//  if(htim_base->Instance==TIM2)
//  {
//
//  }
//#endif
//
//#ifdef ENABLE_TIM3
//#ifdef ENABLE_TIM2
//  else
//#endif
//  if(htim_base->Instance==TIM3)
//  {
//
//  }
//#endif
//#endif
//
//}
//
///**
// * @brief TIM_Base MSP De-Initialization
// * This function freeze the hardware resources used in this example
// * @param htim_base: TIM_Base handle pointer
// * @retval None
// */
//void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim_base)
//{
//#ifdef ENABLE_TIM1
//  if(htim_base->Instance==TIM1)
//  {
//  /* USER CODE BEGIN TIM1_MspDeInit 0 */
//
//  /* USER CODE END TIM1_MspDeInit 0 */
//    /* Peripheral clock disable */
//	  __HAL_RCC_TIM1_CLK_ENABLE();
//
//#if defined(TFDSN_ZXB)
//    /**TIM1 GPIO Configuration
//    PA9     ------> TIM1_CH2
//    */
//    HAL_GPIO_DeInit(TIM1_CH1_BANK, TIM1_CH1_PIN);
//#elif  defined(TFDSN_CLB)
//    /**TIM1 GPIO Configuration
//    PA8     ------> TIM1_CH1
//    */
//
//    HAL_GPIO_DeInit(TIM1_CH1_BANK, TIM1_CH1_PIN);
//#endif
//
//#ifdef ENABLE_TIM1_IRQ
//    /* TIM1 interrupt DeInit */
//    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
//    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
//
//#endif
//
//  /* USER CODE BEGIN TIM1_MspDeInit 1 */
//
//  /* USER CODE END TIM1_MspDeInit 1 */
//  }
//#endif
//#if 0
//#ifdef ENABLE_TIM2
//#ifdef ENABLE_TIM1
//  else
//#endif
//  if(htim_base->Instance==TIM2)
//  {
//	  /* USER CODE BEGIN TIM1_MspDeInit 0 */
//
//	   /* USER CODE END TIM1_MspDeInit 0 */
//	     /* Peripheral clock disable */
//	     __HAL_RCC_TIM1_CLK_DISABLE();
//
//	     /**TIM1 GPIO Configuration
//	     PA9     ------> TIM1_CH2
//	     */
//	     HAL_GPIO_DeInit(TIM2_CH1_BANK, TIM2_CH1_PIN);
//
//	 #ifdef ENABLE_TIM2_IRQ
//	     /* TIM1 interrupt DeInit */
//	     HAL_NVIC_DisableIRQ(TIM2_TRG_COM_TIM11_IRQn);
//	     HAL_NVIC_DisableIRQ(TIM2_CC_IRQn);
//	 #endif
//
//	   /* USER CODE BEGIN TIM1_MspDeInit 1 */
//
//	   /* USER CODE END TIM1_MspDeInit 1 */
//  }
//#endif
//
//#ifdef ENABLE_TIM3
//#ifdef ENABLE_TIM2
//  else
//#endif
//  if(htim_base->Instance==TIM3)
//  {
//	  /* USER CODE BEGIN TIM1_MspDeInit 0 */
//
//	   /* USER CODE END TIM1_MspDeInit 0 */
//	     /* Peripheral clock disable */
//	     __HAL_RCC_TIM3_CLK_DISABLE();
//
//	     /**TIM1 GPIO Configuration
//	     PA9     ------> TIM1_CH2
//	     */
//	     HAL_GPIO_DeInit(TIM1_CH3_BANK, TIM3_CH1_PIN);
//
//	 #ifdef ENABLE_TIM1_IRQ
//	     /* TIM1 interrupt DeInit */
//	     HAL_NVIC_DisableIRQ(TIM3_TRG_COM_TIM11_IRQn);
//	     HAL_NVIC_DisableIRQ(TIM3_CC_IRQn);
//	 #endif
//
//	   /* USER CODE BEGIN TIM1_MspDeInit 1 */
//
//	   /* USER CODE END TIM1_MspDeInit 1 */
//  }
//#endif
//#endif
//
//}
//
//void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
//{
//  GPIO_InitTypeDef GPIO_InitStruct = {0};
//
//
//  if(htim->Instance==TIM1)
//  {
//  /* USER CODE BEGIN TIM1_MspPostInit 0 */
//
//  /* USER CODE END TIM1_MspPostInit 0 */
//
//    //__HAL_RCC_GPIOA_CLK_ENABLE();
//	  PWM_CH1_BANK_CLK;
//    /**TIM1 GPIO Configuration
//    PA8     ------> TIM1_CH1
//    */
//    GPIO_InitStruct.Pin = PWM_CH1_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = GPIO_NOPULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(PWM_CH1_BANK, &GPIO_InitStruct);
//
//  /* USER CODE BEGIN TIM1_MspPostInit 1 */
//
//  /* USER CODE END TIM1_MspPostInit 1 */
//  }
//
//}

#if defined(ENABLE_TIM)
/**
* @brief TIM_Base MSP Initialization
* This function configures the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* htim_base)
{

	//GPIO_InitTypeDef GPIO_InitStruct = {0};
#ifdef ENABLE_TIM1
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspInit 0 */

  /* USER CODE END TIM1_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM1_CLK_ENABLE();

//    __HAL_RCC_GPIOA_CLK_ENABLE();
//    /**TIM1 GPIO Configuration
//    PA9     ------> TIM1_CH2
//    */
//    GPIO_InitStruct.Pin = TIM1_CH1_PIN;
//    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
//    GPIO_InitStruct.Pull = TIM1_CH1_PIN_PULL;
//    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
//    HAL_GPIO_Init(TIM1_CH1_BANK, &GPIO_InitStruct);

//#ifdef ENABLE_TIM1_IRQ
    /* TIM1 interrupt Init */
    //HAL_NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn, 0, 0);
    //HAL_NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
   // HAL_NVIC_SetPriority(TIM1_CC_IRQn, 0, 0);
   // HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
//#endif

  /* USER CODE BEGIN TIM1_MspInit 1 */

  /* USER CODE END TIM1_MspInit 1 */
  }
#endif

#ifdef ENABLE_TIM2
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspInit 0 */

  /* USER CODE END TIM2_MspInit 0 */
    /* Peripheral clock enable */
    __HAL_RCC_TIM2_CLK_ENABLE();
    /* TIM2 interrupt Init */
    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspInit 1 */

  /* USER CODE END TIM2_MspInit 1 */
  }
#endif

}

void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
#ifdef ENABLE_PWM1
  if(htim->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspPostInit 0 */

  /* USER CODE END TIM1_MspPostInit 0 */

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM1 GPIO Configuration
    PA8     ------> TIM1_CH1
    */
    GPIO_InitStruct.Pin = TIM1_CH1_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = TIM1_CH1_PIN_PULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
    HAL_GPIO_Init(TIM1_CH1_BANK, &GPIO_InitStruct);

  /* USER CODE BEGIN TIM1_MspPostInit 1 */

  /* USER CODE END TIM1_MspPostInit 1 */
  }
#endif

}
/**
* @brief TIM_Base MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param htim_base: TIM_Base handle pointer
* @retval None
*/
void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* htim_base)
{
#ifdef ENABLE_TIM1
  if(htim_base->Instance==TIM1)
  {
  /* USER CODE BEGIN TIM1_MspDeInit 0 */

  /* USER CODE END TIM1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM1_CLK_DISABLE();

    /**TIM1 GPIO Configuration
    PA9     ------> TIM1_CH2
    */
    //HAL_GPIO_DeInit(TIM1_CH1_BANK, TIM1_CH1_PIN);

	#ifdef ENABLE_TIM1_IRQ
    /* TIM1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
    HAL_NVIC_DisableIRQ(TIM1_CC_IRQn);
	#endif

  /* USER CODE BEGIN TIM1_MspDeInit 1 */

  /* USER CODE END TIM1_MspDeInit 1 */
  }
#endif

#ifdef ENABLE_TIM2
  if(htim_base->Instance==TIM2)
  {
  /* USER CODE BEGIN TIM2_MspDeInit 0 */

  /* USER CODE END TIM2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM2_CLK_DISABLE();

    /* TIM2 interrupt DeInit */
    HAL_NVIC_DisableIRQ(TIM2_IRQn);
  /* USER CODE BEGIN TIM2_MspDeInit 1 */

  /* USER CODE END TIM2_MspDeInit 1 */
  }
#endif

}
#endif

#if defined(ENABLE_SERIAL)

#if defined(BSP_UART1_RX_ENABLE_DMA)
extern DMA_HandleTypeDef hdma_usart1_rx;
#endif

#if defined(BSP_UART1_TX_ENABLE_DMA)
extern DMA_HandleTypeDef hdma_usart1_tx;
#endif

/**
 * @brief UART MSP Initialization
 * This function configures the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
	if (huart->Instance == USART1)
	{
		/* USER CODE BEGIN USART1_MspInit 0 */

		/* USER CODE END USART1_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_USART1_CLK_ENABLE();

		__HAL_RCC_GPIOA_CLK_ENABLE();
		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		GPIO_InitStruct.Pin = USART1_TX_PIN | USART1_RX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
		HAL_GPIO_Init(USART1_GPIO_BANKA, &GPIO_InitStruct);

#if defined(BSP_UART1_RX_ENABLE_DMA)
    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA2_Stream2;
    hdma_usart1_rx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(huart,hdmarx,hdma_usart1_rx);
#endif

#if defined(BSP_UART1_TX_ENABLE_DMA)
    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA2_Stream7;
    hdma_usart1_tx.Init.Channel = DMA_CHANNEL_4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    hdma_usart1_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(huart,hdmatx,hdma_usart1_tx);
#endif

#if defined(ENABLE_UART1_IRQ)
    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
#endif
		/* USER CODE BEGIN USART1_MspInit 1 */

		/* USER CODE END USART1_MspInit 1 */
	}

}

/**
 * @brief UART MSP De-Initialization
 * This function freeze the hardware resources used in this example
 * @param huart: UART handle pointer
 * @retval None
 */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		/* USER CODE BEGIN USART1_MspDeInit 0 */

		/* USER CODE END USART1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_USART1_CLK_DISABLE();

		/**USART1 GPIO Configuration
		 PA9     ------> USART1_TX
		 PA10     ------> USART1_RX
		 */
		HAL_GPIO_DeInit(USART1_GPIO_BANKA, USART1_TX_PIN | USART1_RX_PIN);

#if defined(BSP_UART1_RX_ENABLE_DMA)
    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(huart->hdmarx);
#endif

#if defined(BSP_UART1_TX_ENABLE_DMA)
    HAL_DMA_DeInit(huart->hdmatx);
#endif

#if defined(ENABLE_UART1_IRQ)
    /* USART1 interrupt DeInit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
#endif
		/* USER CODE BEGIN USART1_MspDeInit 1 */

		/* USER CODE END USART1_MspDeInit 1 */
	}

}

#endif // defined(ENABLE_SERIAL)

/**
 * @brief WWDG MSP Initialization
 * This function configures the hardware resources used in this example
 * @param hwwdg: WWDG handle pointer
 * @retval None
 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
	if (hwwdg->Instance == WWDG)
	{
		/* USER CODE BEGIN WWDG_MspInit 0 */

		/* USER CODE END WWDG_MspInit 0 */
		/* Peripheral clock enable */
		__HAL_RCC_WWDG_CLK_ENABLE();
		/* USER CODE BEGIN WWDG_MspInit 1 */

		/* USER CODE END WWDG_MspInit 1 */
	}

}

#if defined(ENABLE_SRAM) || defined(ENABLE_FSMC_ADC7606)
static uint32_t FSMC_Initialized = 0;

static void HAL_FSMC_MspInit(void)
{
	/* USER CODE BEGIN FSMC_MspInit 0 */

	/* USER CODE END FSMC_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };
	if (FSMC_Initialized)
	{
		return;
	}
	FSMC_Initialized = 1;

	/* Peripheral clock enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_FSMC_CLK_ENABLE();

	/** FSMC GPIO Configuration
	 PE3   ------> FSMC_A19
	 PE4   ------> FSMC_A20

	 PF0   ------> FSMC_A0
	 PF1   ------> FSMC_A1
	 PF2   ------> FSMC_A2
	 PF3   ------> FSMC_A3
	 PF4   ------> FSMC_A4
	 PF5   ------> FSMC_A5
	 PF12   ------> FSMC_A6
	 PF13   ------> FSMC_A7
	 PF14   ------> FSMC_A8
	 PF15   ------> FSMC_A9

	 PG0   ------> FSMC_A10
	 PG1   ------> FSMC_A11

	 PE7   ------> FSMC_D4
	 PE8   ------> FSMC_D5
	 PE9   ------> FSMC_D6
	 PE10   ------> FSMC_D7
	 PE11   ------> FSMC_D8
	 PE12   ------> FSMC_D9
	 PE13   ------> FSMC_D10
	 PE14   ------> FSMC_D11
	 PE15   ------> FSMC_D12

	 PD8   ------> FSMC_D13
	 PD9   ------> FSMC_D14
	 PD10   ------> FSMC_D15
	 PD11   ------> FSMC_A16
	 PD12   ------> FSMC_A17
	 PD13   ------> FSMC_A18
	 PD14   ------> FSMC_D0
	 PD15   ------> FSMC_D1

	 PG2   ------> FSMC_A12
	 PG3   ------> FSMC_A13
	 PG4   ------> FSMC_A14
	 PG5   ------> FSMC_A15

	 |	PG6   ------> FSMC_INT2
	 | PG7   ------> FSMC_INT3

	 PD0   ------> FSMC_D2
	 PD1   ------> FSMC_D3
	 PD4   ------> FSMC_NOE
	 PD5   ------> FSMC_NWE

	 |	PD6   ------> FSMC_NWAIT
	 |	PD7   ------> FSMC_NE1

	 PG10   ------> FSMC_NE3
	 PE0   ------> FSMC_NBL0
	 PE1   ------> FSMC_NBL1
	 */
	GPIO_InitStruct.Pin = FSMC_A19_PIN | FSMC_A20_PIN | FSMC_D4_PIN | FSMC_D5_PIN | FSMC_D6_PIN | FSMC_D7_PIN | FSMC_D8_PIN
	        | FSMC_D9_PIN | FSMC_D10_PIN | FSMC_D11_PIN | FSMC_D12_PIN | FSMC_NBL0_PIN | FSMC_NBL1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(FSMC_GPIO_BANKE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FSMC_A0_PIN | FSMC_A1_PIN | FSMC_A2_PIN | FSMC_A3_PIN | FSMC_A4_PIN | FSMC_A5_PIN | FSMC_A6_PIN
	        | FSMC_A7_PIN | FSMC_A8_PIN | FSMC_A9_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(FSMC_GPIO_BANKF, &GPIO_InitStruct);

	//HAL_GPIO_DeInit(GPIOG, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_10);
	GPIO_InitStruct.Pin = FSMC_A10_PIN | FSMC_A11_PIN | FSMC_A12_PIN | FSMC_A13_PIN | FSMC_A14_PIN | FSMC_A15_PIN
	        | FSMC_INT2_PIN | FSMC_INT3_PIN | FSMC_NE3_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(FSMC_GPIO_BANKG, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = FSMC_D13_PIN | FSMC_D14_PIN | FSMC_D15_PIN | FSMC_A16_PIN | FSMC_A17_PIN | FSMC_A18_PIN | FSMC_D0_PIN
	        | FSMC_D1_PIN | FSMC_D2_PIN | FSMC_D3_PIN | FSMC_NOE_PIN | FSMC_NWE_PIN | FSMC_NWAIT_PIN | FSMC_NE1_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF12_FSMC;
	HAL_GPIO_Init(FSMC_GPIO_BANKD, &GPIO_InitStruct);

	/* USER CODE BEGIN FSMC_MspInit 1 */

	/* USER CODE END FSMC_MspInit 1 */
}

#if defined(ENABLE_FSMC_ADC7606)
void HAL_AD7606_MspInit(void)
{
	GPIO_InitTypeDef GPIO_InitStruct =
	{
	        0 };

	//HAL_FSMC_MspInit();
	/* USER CODE BEGIN MspInit 0 */

	/* USER CODE END _MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/**
	 * AD7606 GPIO Configuration
	 PB0     ------> AD7606_BUSY1
	 PA3     ------> AD7606_RESET
	 PB1     ------> AD7606_CONVST
	 ...     ------> AD_RANGE
	 ...     ------> AD_OS0//
	 ...     ------> AD_OS1//
	 ...     ------> AD_OS2//
	 ...     ------> AD_OS2//
	 PG12    ------> AD7606_CS//
	 */

	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = AD7606_BUSY;
	HAL_GPIO_Init(AD7606_BUSY_IO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = AD7606_RESET;
	HAL_GPIO_Init(AD7606_RESET_IO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = AD7606_CONVST;
	HAL_GPIO_Init(AD7606_CONVST_IO_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	GPIO_InitStruct.Pin = AD7606_CS;
	GPIO_InitStruct.Alternate = AD7606_CS_IO_ALT;
	HAL_GPIO_Init(AD7606_CS_IO_PORT, &GPIO_InitStruct);

	HAL_GPIO_WritePin(AD7606_CS_IO_PORT, AD7606_CS, GPIO_PIN_SET);
	/* USER CODE BEGIN _MspInit 1 */

	/* USER CODE END _MspInit 1 */
}
#endif

void HAL_SRAM_MspInit(SRAM_HandleTypeDef *hsram)
{
	/* USER CODE BEGIN SRAM_MspInit 0 */

	/* USER CODE END SRAM_MspInit 0 */
	HAL_FSMC_MspInit();

#if defined(ENABLE_FSMC_ADC7606)
	HAL_AD7606_MspInit();
#endif
	/* USER CODE BEGIN SRAM_MspInit 1 */

	/* USER CODE END SRAM_MspInit 1 */
}

static uint32_t FSMC_DeInitialized = 0;

static void HAL_FSMC_MspDeInit(void)
{
	/* USER CODE BEGIN FSMC_MspDeInit 0 */

	/* USER CODE END FSMC_MspDeInit 0 */
	if (FSMC_DeInitialized)
	{
		return;
	}
	FSMC_DeInitialized = 1;
	/* Peripheral clock enable */
	__HAL_RCC_FSMC_CLK_DISABLE();

	/** FSMC GPIO Configuration
	 PE3   ------> FSMC_A19
	 PE4   ------> FSMC_A20
	 PF0   ------> FSMC_A0
	 PF1   ------> FSMC_A1
	 PF2   ------> FSMC_A2
	 PF3   ------> FSMC_A3
	 PF4   ------> FSMC_A4
	 PF5   ------> FSMC_A5
	 PF12   ------> FSMC_A6
	 PF13   ------> FSMC_A7
	 PF14   ------> FSMC_A8
	 PF15   ------> FSMC_A9
	 PG0   ------> FSMC_A10
	 PG1   ------> FSMC_A11
	 PE7   ------> FSMC_D4
	 PE8   ------> FSMC_D5
	 PE9   ------> FSMC_D6
	 PE10   ------> FSMC_D7
	 PE11   ------> FSMC_D8
	 PE12   ------> FSMC_D9
	 PE13   ------> FSMC_D10
	 PE14   ------> FSMC_D11
	 PE15   ------> FSMC_D12
	 PD8   ------> FSMC_D13
	 PD9   ------> FSMC_D14
	 PD10   ------> FSMC_D15
	 PD11   ------> FSMC_A16
	 PD12   ------> FSMC_A17
	 PD13   ------> FSMC_A18
	 PD14   ------> FSMC_D0
	 PD15   ------> FSMC_D1
	 PG2   ------> FSMC_A12
	 PG3   ------> FSMC_A13
	 PG4   ------> FSMC_A14
	 PG5   ------> FSMC_A15
	 PD0   ------> FSMC_D2
	 PD1   ------> FSMC_D3
	 PD4   ------> FSMC_NOE
	 PD5   ------> FSMC_NWE
	 PG9   ------> FSMC_NE2
	 PG10   ------> FSMC_NE3
	 PE0   ------> FSMC_NBL0
	 PE1   ------> FSMC_NBL1
	 */
	HAL_GPIO_DeInit(FSMC_GPIO_BANKE,
	        FSMC_A19_PIN | FSMC_A20_PIN | FSMC_D4_PIN | FSMC_D5_PIN | FSMC_D6_PIN | FSMC_D7_PIN | FSMC_D8_PIN | FSMC_D9_PIN
	                | FSMC_D10_PIN | FSMC_D11_PIN | FSMC_D12_PIN | FSMC_NBL0_PIN | FSMC_NBL1_PIN);

	HAL_GPIO_DeInit(FSMC_GPIO_BANKF,
	        FSMC_A0_PIN | FSMC_A1_PIN | FSMC_A2_PIN | FSMC_A3_PIN | FSMC_A4_PIN | FSMC_A5_PIN | FSMC_A6_PIN | FSMC_A7_PIN
	                | FSMC_A8_PIN | FSMC_A9_PIN);

	HAL_GPIO_DeInit(FSMC_GPIO_BANKG,
	FSMC_A10_PIN | FSMC_A11_PIN | FSMC_A12_PIN | FSMC_A13_PIN | FSMC_A14_PIN | FSMC_A15_PIN | FSMC_INT2_PIN | FSMC_INT3_PIN    //| GPIO_PIN_9
	        | FSMC_NE3_PIN);

	HAL_GPIO_DeInit(FSMC_GPIO_BANKD,
	        FSMC_D13_PIN | FSMC_D14_PIN | FSMC_D15_PIN | FSMC_A16_PIN | FSMC_A17_PIN | FSMC_A18_PIN | FSMC_D0_PIN | FSMC_D1_PIN
	                | FSMC_D2_PIN | FSMC_D3_PIN | FSMC_NOE_PIN | FSMC_NWE_PIN | FSMC_NWAIT_PIN | FSMC_NE1_PIN);

	/* USER CODE BEGIN FSMC_MspDeInit 1 */

	/* USER CODE END FSMC_MspDeInit 1 */
}

#if defined(ENABLE_FSMC_ADC7606)
void HAL_AD7606_MspDeInit(void)
{

	//HAL_FSMC_MspDeInit();
	/* Peripheral clock disable */
	__HAL_RCC_GPIOA_CLK_DISABLE();
	__HAL_RCC_GPIOB_CLK_DISABLE();
	__HAL_RCC_GPIOC_CLK_DISABLE();
	__HAL_RCC_GPIOG_CLK_DISABLE();
	/**
	 * AD7606 GPIO Configuration
	 PB0     ------> AD7606_BUSY1
	 PA3     ------> AD7606_RESET
	 PB1     ------> AD7606_CONVST
	 ...     ------> AD_RANGE
	 ...     ------> AD_OS0//
	 ...     ------> AD_OS1//
	 ...     ------> AD_OS2//
	 ...     ------> AD_OS2//
	 PG12    ------> AD7606_CS//
	 */

	HAL_GPIO_DeInit(AD7606_BUSY_IO_PORT, AD7606_BUSY);

	HAL_GPIO_DeInit(AD7606_RESET_IO_PORT, AD7606_RESET);

	HAL_GPIO_DeInit(AD7606_CONVST_IO_PORT, AD7606_CONVST);

	HAL_GPIO_DeInit(AD7606_CS_IO_PORT, AD7606_CS);

	/* USER CODE BEGIN _MspInit 1 */

	/* USER CODE END _MspInit 1 */
}
#endif

void HAL_SRAM_MspDeInit(SRAM_HandleTypeDef *hsram)
{
	/* USER CODE BEGIN SRAM_MspDeInit 0 */

	/* USER CODE END SRAM_MspDeInit 0 */
	HAL_FSMC_MspDeInit();

#if defined(ENABLE_FSMC_ADC7606)
	HAL_AD7606_MspDeInit();
#endif
	/* USER CODE BEGIN SRAM_MspDeInit 1 */

	/* USER CODE END SRAM_MspDeInit 1 */
}

#endif //#if defined(ENABLE_SRAM) || defined(ENABLE_FSMC_ADC7606)
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
