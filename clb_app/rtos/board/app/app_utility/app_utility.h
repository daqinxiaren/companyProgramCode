/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

#ifndef __APP_UTILITY_H__
#define __APP_UTILITY_H__

/** includes */
#include <string.h>
#include <rtthread.h>
#include <net_config.h>
#include <lwip/sockets.h>
#include "stdint.h"
#include "../app_board_config/app_board_config.h"

/** macro */
#define MIN(a, b)               ((a) > (b) ? (b) : (a))
#define MIN3(a, b, c)           MIN(MIN(a, b), c)
#define MAX(a, b)               ((a) < (b) ? (b) : (a))
#define MAX3(a, b, c)           MAX(MAX((a), (b)), c)
#define MAX4(a, b, c, d)        MAX(MAX3((a), (b), (c)), d)
#define MAX5(a, b, c, d, e)     MAX(MAX4((a), (b), (c), (d)), e)
#define MAX6(a, b, c, d, e, f)  MAX(MAX5((a), (b), (c), (d), (e)), f)

#define DEF_MARK_BIT(b)         ((uint32_t)1 << (b))
#define DEF_MARK_BITS(b)        (((uint32_t)1 << (b)) - 1)

#define DATA16(h, l)            (((h) << 8) | (l))
#define DATA32(hh, hl, lh, ll)  ((DATA16(hh, hl) << 8) | DATA16(lh, ll))
#define DATA16_H(d)             ((uint8_t)((d) >> 8))
#define DATA16_L(d)             ((uint8_t)(d))

#define STRUCT_OFFSET(S, e)     (uint32_t)(&((S *)0)->e)

#define STR_LEN(s)              (sizeof(s) - 1)

/** type declaration */

/** variable declaration */

/** function declaration */
uint16_t modbus_crc(uint8_t *data, uint16_t size);
uint8_t check_xor_sum (void *p_data, uint32_t len, uint8_t in_data);
uint8_t hex_compare(uint8_t *data1, uint8_t *data2, uint32_t size);
uint8_t non_empty(uint8_t *data, uint32_t size);
uint16_t sum_check_16(uint8_t *data, uint16_t crclen);

#endif /* __APP_UTILITY_H__ */

