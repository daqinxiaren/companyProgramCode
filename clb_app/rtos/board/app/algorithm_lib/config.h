/**
 * @file     fft_config.h
 * @details  配置算法参数，根据不同项目配置,郑州7号线项目
 * @author
 * @date
 * @version
**/

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* include */
#define MCU_ARM //若在STM32上跑，找不到此定义时须自行定义
#ifdef MCU_ARM
#include "arm_math.h" //根据代码路径配置
#else
#include "math.h"
#define PI					3.14159265358979f
#endif
/* macro */
/************************* IIR PARAM ****************************/
#define POLYGON_FILTER_NUM_1    0.0202110431311719
#define POLYGON_FILTER_NUM_2    -0.0618784014310059
#define POLYGON_FILTER_NUM_3    0.126361691958874
#define POLYGON_FILTER_NUM_4    -0.168293917341436
#define POLYGON_FILTER_NUM_5    0.191049889368274
#define POLYGON_FILTER_NUM_6    -0.168293917341436
#define POLYGON_FILTER_NUM_7    0.126361691958874
#define POLYGON_FILTER_NUM_8    -0.0618784014310059
#define POLYGON_FILTER_NUM_9    0.0202110431311719

#define POLYGON_FILTER_DEN_1    1
#define POLYGON_FILTER_DEN_2    -5.37851988464675
#define POLYGON_FILTER_DEN_3    13.9721707234815
#define POLYGON_FILTER_DEN_4    -22.3042602479832
#define POLYGON_FILTER_DEN_5    23.7373979438715
#define POLYGON_FILTER_DEN_6    -17.1538692718108
#define POLYGON_FILTER_DEN_7    8.20869031281336
#define POLYGON_FILTER_DEN_8    -2.37814731181059
#define POLYGON_FILTER_DEN_9    0.320944012772122

#define POLYGON_wheel_diameter          0.805		// 单位m	轮径，默认是0.805
#define POLYGON_Sample_rate             4096			//采样频率
#define POLYGON_threshold_prognosis     17
#define POLYGON_threshold_prewarning    22
#define POLYGON_threshold_warning       27
#define POLYGON_time_length             10			//堆栈空间的长度

/*********************** FFT INIT PARAM ****************************/
#define Diag_Num_ZXZC_Wheel     5     //轴箱轴承诊断与踏面诊断总数据长度
#define Diag_Num_DJZC           8     //电机诊断数据长度
#define Diag_Num_gear           1     //齿轮诊断数据长度
#define Diag_Num_CLXZC          8    //齿轮箱轴承诊断数据长度

/*阈值*/
#define threshold_NGB_prognosis    44
#define threshold_W_prognosis      49
#define threshold_Wheel_prognosis  54
#define threshold_gear_prognosis   54

#define threshold_NGB_prewarning   50
#define threshold_W_prewarning     55
#define threshold_Wheel_prewarning 60
#define threshold_gear_prewarning  60

#define threshold_NGB_warning      56
#define threshold_W_warning        61
#define threshold_Wheel_warning    65
#define threshold_gear_warning     65

/*轴箱轴承*/
#define ZXZC_low_limit_1           1.24
#define ZXZC_low_limit_2           0.9704
#define ZXZC_low_limit_3           0.83971
#define ZXZC_low_limit_4           0.046211
#define ZXZC_low_limit_5           0.1053
#define ZXZC_high_limit_1          1.3528
#define ZXZC_high_limit_2          1.0587
#define ZXZC_high_limit_3          0.91605
#define ZXZC_high_limit_4          0.050412
#define ZXZC_high_limit_5          0.1148
#define ZXZC_base_1           0.1
#define ZXZC_base_2           0.1
#define ZXZC_base_3           0.1
#define ZXZC_base_4           0.1
#define ZXZC_base_5           0.1
#define ZXZC_bia_1          (10)
#define ZXZC_bia_2          (10)
#define ZXZC_bia_3          (10)
#define ZXZC_bia_4          (10)
#define ZXZC_bia_5          (10)

/*电机轴承*/
#define DJ_low_limit_1           4.3444  //1-4为电机非驱动端轴承参数
#define DJ_low_limit_2           3.0796
#define DJ_low_limit_3           3.8467
#define DJ_low_limit_4           0.27997
#define DJ_low_limit_5           6.9474  //5-8为电机驱动端轴承参数
#define DJ_low_limit_6           5.201
#define DJ_low_limit_7           4.5978
#define DJ_low_limit_8           0.28894
#define DJ_high_limit_1          4.7393
#define DJ_high_limit_2          3.3596
#define DJ_high_limit_3          4.1964
#define DJ_high_limit_4          0.30542
#define DJ_high_limit_5          7.579
#define DJ_high_limit_6          5.6738
#define DJ_high_limit_7          5.0158
#define DJ_high_limit_8          0.31521
#define DJ_base_1           0.1
#define DJ_base_2           0.1
#define DJ_base_3           0.1
#define DJ_base_4           0.1
#define DJ_base_5           0.1
#define DJ_base_6           0.1
#define DJ_base_7           0.1
#define DJ_base_8           0.1
#define DJ_bia_1          (10)
#define DJ_bia_2          (10)
#define DJ_bia_3          (10)
#define DJ_bia_4          (10)
#define DJ_bia_5          (10)
#define DJ_bia_6          (10)
#define DJ_bia_7          (10)
#define DJ_bia_8          (10)

/*齿轮*/
#define gear_low_limit   11.4777
#define gear_high_limit  12.5132

#define gear_base  0.1
#define gear_bia   (10)

/*齿轮箱轴承*/ 
#define CLX_low_limit_1           6.9474  //1-4为电机侧轴承参数
#define CLX_low_limit_2           5.201
#define CLX_low_limit_3           4.5978
#define CLX_low_limit_4           0.28894
#define CLX_low_limit_5           6.1652  //5-8为车轮侧轴承参数
#define CLX_low_limit_6           4.6333
#define CLX_low_limit_7           3.8187
#define CLX_low_limit_8           0.28958
#define CLX_high_limit_1          7.579
#define CLX_high_limit_2          5.6738
#define CLX_high_limit_3          5.0158
#define CLX_high_limit_4          0.31521
#define CLX_high_limit_5          6.7257
#define CLX_high_limit_6          5.0545
#define CLX_high_limit_7          4.1658
#define CLX_high_limit_8          0.3159
#define CLX_base_1           0.1
#define CLX_base_2           0.1
#define CLX_base_3           0.1
#define CLX_base_4           0.1
#define CLX_base_5           0.1
#define CLX_base_6           0.1
#define CLX_base_7           0.1
#define CLX_base_8           0.1
#define CLX_bia_1          (10)
#define CLX_bia_2          (10)
#define CLX_bia_3          (10)
#define CLX_bia_4          (10)
#define CLX_bia_5          (10)
#define CLX_bia_6          (10)
#define CLX_bia_7          (10)
#define CLX_bia_8          (10)

// 预判门限值
#define ZC_PRO_INNER        44 //内环
#define ZC_PRO_OUTER        49 //外环
#define ZC_PRO_ROLLING      44 //滚动体
#define ZC_PRO_CAGE         44 //保持架
#define TM_PRO_CAGE         54 //踏面
// 预警
#define ZC_PRE_INNER        50 //内环
#define ZC_PRE_OUTER        55 //外环
#define ZC_PRE_ROLLING      50 //滚动体
#define ZC_PRE_CAGE         50 //保持架
#define TM_PRE_CAGE         60 //踏面
// 报警
#define ZC_WARN_INNER       56 //内环
#define ZC_WARN_OUTER       61 //外环
#define ZC_WARN_ROLLING     56 //滚动体
#define ZC_WARN_CAGE        56 //保持架
#define TM_WARN_CAGE        65 //踏面


/* type declaration */

#endif /*__CONFIG_H__*/

