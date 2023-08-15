/*
 * Copyright (c) 2019 Winner Microelectronics Co., Ltd.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2019-07-10     Ernest       1st version
 */

#ifndef __DRV_CRYPTO_H__
#define __DRV_CRYPTO_H__

#include "drv_config.h"

#ifdef ENABLE_HWCRYPTO

#ifdef __cplusplus
extern "C" {
#endif

int l4_hw_crypto_device_init(void);

#ifdef __cplusplus
}
#endif

#endif

#endif /* __DRV_CRYPTO_H__ */
