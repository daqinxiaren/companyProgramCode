/*
 * lh_diagno_api.c
 *
 *  Created on: Jul 13, 2021
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include "lh_diagno_api.h"


//10阶低通滤波
static float FILTER_NUM_PARA[11] = {
		0.000040885,
		0.00040885,
		0.0018,
		0.0049,
		0.0086,
		0.010,
		0.0086,
		0.0049,
		0.0018,
		0.00040885,
		0.000040885
};//分子

static float FILTER_DEN_PARA[11] = {
		1.000,
		-4.1281,
		8.5814,
		-11.3298,
		10.3637,
		-6.7906,
		3.2059,
		-1.0713,
		0.2415,
		-0.0331,
		0.0021
};//分母
