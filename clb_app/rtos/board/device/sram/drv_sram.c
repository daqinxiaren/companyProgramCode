/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-23     Malongwei    first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#include <sram_port.h>
#ifdef ENABLE_SRAM

#define DRV_DEBUG
#define LOG_TAG             "drv.sram"
#include <drv_log.h>

#ifdef ENABLE_MEMHEAP_AS_HEAP
static struct rt_memheap system_heap;
#endif

static SRAM_HandleTypeDef hsram;

static int rt_hw_sram_init(void)
{
	int result = OS_EOK;

	FSMC_NORSRAM_TimingTypeDef Timing =
	{
	        0 };

	/** Perform the SRAM2 memory initialization sequence
	 */
	hsram.Instance = FSMC_NORSRAM_DEVICE;
	hsram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
	/* hsram.Init */
	hsram.Init.NSBank = FSMC_NORSRAM_BANK3;
	hsram.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
	hsram.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
#if SRAM_DATA_WIDTH == 8
    hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_8;
#elif SRAM_DATA_WIDTH == 16
	hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
#else
    hsram.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_32;
#endif
	hsram.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
	hsram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
	hsram.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
	hsram.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
	hsram.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
	hsram.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
	hsram.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
	hsram.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
	hsram.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
	hsram.Init.PageSize = FSMC_PAGE_SIZE_NONE;

	/* Timing */
	Timing.AddressSetupTime = ADDRESS_SETUP_TIME;
	Timing.AddressHoldTime = ADDRESS_HOLD_TIME;
	Timing.DataSetupTime = DATA_SETUP_TIME;
	Timing.BusTurnAroundDuration = BUS_TURN_AROUND_DURATION;
	Timing.CLKDivision = CLK_DIVISION;
	Timing.DataLatency = DATA_LATENCY;
	Timing.AccessMode = ACCESS_MODE;
	/* ExtTiming */

	if (HAL_SRAM_Init(&hsram, &Timing, &Timing) != HAL_OK)
	{
		LOG_E("NOTO:\r\n");
		LOG_E("\tSRAM init\t\t\t [failed]!\r\n");
		result = -OS_ERROR;
	}
	else
	{

#ifdef ENABLE_MEMHEAP_AS_HEAP
		/* If ENABLE_MEMHEAP_AS_HEAP is enabled, SRAM is initialized to the heap */
		rt_memheap_init(&system_heap, "sram", (void*) SRAM_BANK_ADDR, SRAM_SIZE);
#endif
		LOG_E("NOTO:\r\n"); LOG_D("\tSRAM init\t\t\t [success]!, \t SRAM mapped at 0x%X, size is %d bytes, data width is %d\r\n", SRAM_BANK_ADDR, SRAM_SIZE, SRAM_DATA_WIDTH);
	}

	return result;
}
INIT_BOARD_EXPORT(rt_hw_sram_init);

#ifdef DRV_DEBUG
#ifdef FINSH_ENABLE_MSH
static int sram_test(void)
{
	int i = 0;
	uint32_t start_time = 0, time_cast = 0;
#if SRAM_DATA_WIDTH == 8
    char data_width = 1;
    uint8_t data = 0;
#elif SRAM_DATA_WIDTH == 16
	char data_width = 2;
	uint16_t data = 0;
#else
    char data_width = 4;
    uint32_t data = 0;
#endif

	/* write data */
	LOG_D("Writing the %ld bytes data, waiting....", SRAM_SIZE);
	start_time = rt_tick_get();
	for (i = 0; i < SRAM_SIZE / data_width; i++)
	{
#if SRAM_DATA_WIDTH == 8
        *(__IO uint8_t *)(SRAM_BANK_ADDR + i * data_width) = (uint8_t)0x55;
#elif SRAM_DATA_WIDTH == 16
		*(__IO uint16_t*) (SRAM_BANK_ADDR + i * data_width) = (uint16_t) 0x5555;
#else
        *(__IO uint32_t *)(SRAM_BANK_ADDR + i * data_width) = (uint32_t)0x55555555;
#endif
	}
	time_cast = rt_tick_get() - start_time;
	LOG_D("Write data success, total time: %d.%03dS.", time_cast / TICK_PER_SECOND,
			time_cast % TICK_PER_SECOND / ((TICK_PER_SECOND * 1 + 999) / 1000));

	/* read data */
	LOG_D("start Reading and verifying data, waiting....");
	for (i = 0; i < SRAM_SIZE / data_width; i++)
	{
#if SRAM_DATA_WIDTH == 8
        data = *(__IO uint8_t *)(SRAM_BANK_ADDR + i * data_width);
        if (data != 0x55)
        {
            LOG_E("SRAM test failed!");
            break;
        }
#elif SRAM_DATA_WIDTH == 16
		data = *(__IO uint16_t*) (SRAM_BANK_ADDR + i * data_width);
		if (data != 0x5555)
		{
			LOG_E("SRAM test failed!");
			break;
		}
#else
        data = *(__IO uint32_t *)(SRAM_BANK_ADDR + i * data_width);
        if (data != 0x55555555)
        {
            LOG_E("SRAM test failed!");
            break;
        }
#endif
	}

	if (i >= SRAM_SIZE / data_width)
	{
		LOG_D("SRAM test success!");
	}

	return OS_EOK;
}
MSH_CMD_EXPORT(sram_test, sram test);
#endif /* FINSH_ENABLE_MSH */
#endif /* DRV_DEBUG */
#endif /* ENABLE_SRAM */
