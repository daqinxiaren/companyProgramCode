/*
 * fault_hand.h
 *
 *  Created on: Jul 1, 2021
 *      Author: root
 */

#ifndef _FAULT_HAND_H_
#define _FAULT_HAND_H_
#include <rtconfig.h>

struct qzq_st
{
	uint8_t qzq1_rest_cnt;
	uint8_t qzq2_rest_cnt;
	uint8_t qzq3_rest_cnt;
	uint8_t qzq4_rest_cnt;

};

struct eth_st
{
	uint8_t self_eth_err;
	uint32_t eth_comm_cnt;
};

struct fault_data
{
	struct qzq_st qzqs;
	struct eth_st ethst;
};

extern struct fault_data faults;

void qzq_fault_deal(uint8_t flag);

void eth_fault_deal(void);

void fault_hand_data_init(void);

#endif /* APP_INCLUDE_FAULT_HAND_H_ */
