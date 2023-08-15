/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2012-01-08     bernard      first version.
 * 2014-07-12     bernard      Add workqueue implementation.
 */

#ifndef __RT_DEVICE_H__
#define __RT_DEVICE_H__

#include <rtthread.h>

#include "ipc/ringbuffer.h"
#include "ipc/completion.h"
#include "ipc/dataqueue.h"
#include "ipc/workqueue.h"
#include "ipc/waitqueue.h"
#include "ipc/pipe.h"
#include "ipc/poll.h"
#include "ipc/ringblk_buf.h"

#ifdef __cplusplus
extern "C" {
#endif

#define RT_DEVICE(device)            ((rt_device_t)device)

#ifdef ENABLE_RTC
#include "drivers/rtc.h"
#ifdef ENABLE_ALARM
#include "drivers/alarm.h"
#endif
#endif /* ENABLE_RTC */

#ifdef ENABLE_SPI
#include "drivers/spi.h"
#endif /* ENABLE_SPI */

#ifdef ENABLE_MTD_NOR
#include "drivers/mtd_nor.h"
#endif /* ENABLE_MTD_NOR */

#ifdef ENABLE_MTD_NAND
#include "drivers/mtd_nand.h"
#endif /* ENABLE_MTD_NAND */

#ifdef ENABLE_USB_DEVICE
#include "drivers/usb_device.h"
#endif /* ENABLE_USB_DEVICE */

#ifdef ENABLE_USB_HOST
#include "drivers/usb_host.h"
#endif /* ENABLE_USB_HOST */

#ifdef ENABLE_SERIAL
#include "drivers/serial.h"
#endif /* ENABLE_SERIAL */

#ifdef ENABLE_I2C
#include "drivers/i2c.h"
#include "drivers/i2c_dev.h"

#ifdef ENABLE_I2C_BITOPS
#include "drivers/i2c-bit-ops.h"
#endif /* ENABLE_I2C_BITOPS */
#endif /* ENABLE_I2C */

#ifdef ENABLE_PHY
#include "drivers/phy.h"
#include "drivers/phy_mdio.h"
#endif /* ENABLE_PHY */

#ifdef ENABLE_SDIO
#include "drivers/mmcsd_core.h"
#include "drivers/sd.h"
#include "drivers/sdio.h"
#endif

#ifdef ENABLE_WDT
#include "drivers/watchdog.h"
#endif

#ifdef ENABLE_PIN
#include "drivers/pin.h"
#endif

#ifdef ENABLE_CAN
#include "drivers/can.h"
#endif

#ifdef ENABLE_HWTIMER
#include "drivers/hwtimer.h"
#endif

#ifdef ENABLE_AUDIO
#include "drivers/audio.h"
#endif

#ifdef ENABLE_CPUTIME
#include "drivers/cputime.h"
#endif

#ifdef ENABLE_ADC
#include "drivers/adc.h"
#endif

#ifdef ENABLE_DAC
#include "drivers/dac.h"
#endif

#ifdef ENABLE_PWM
#include "drivers/rt_drv_pwm.h"
#endif

#ifdef ENABLE_PM
#include "drivers/pm.h"
#endif

#ifdef ENABLE_WIFI
#include "drivers/wlan.h"
#endif

#ifdef MTD_ENABLE_NOR
#include "drivers/mtdnor.h"
#endif
#ifdef MTD_ENABLE_NAND
#include "drivers/mtdnand.h"
#endif

#ifdef ENABLE_HWCRYPTO
#include "drivers/crypto.h"
#endif

#ifdef ENABLE_PULSE_ENCODER
#include "drivers/pulse_encoder.h"
#endif

#ifdef ENABLE_INPUT_CAPTURE
#include "drivers/rt_inputcapture.h"
#endif

#ifdef __cplusplus
}
#endif

#endif /* __RT_DEVICE_H__ */
