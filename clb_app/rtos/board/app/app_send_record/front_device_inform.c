/**
 * @file
 * @details
 * @author
 * @date
 * @version
**/

/* include */
#include "app_send_record.h"
#include "app_paras_save.h"

/* macro */
#define FRONT_TEMP_SHIFT         DEF_TEMP_SHIFT
#define FRONT_TEMP_DEFAULT       DEF_TEMP_DEFAULT //发送上报列控的默认值
#define TEMP_INVALID             DEF_TEMP_DEFAULT //温度无有效时的默认值
#define FRONT_TEMP_85            85 //85°温度是18b20的默认值，若中途出现，则丢掉此温度
#define WAIT_TEMP_TIME           600

/* function declaration */
/**
 * @brief
 * @param
 * @return
 * @note   前置器信息中的温度buffer初始化为默认温度
**/
static void front_temp_init(void)
{
    app_send_record_env.all_front_inform.front_dev1_inform.bearing_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.bearing_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.bearing_inform.dev3_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.bearing_inform.dev4_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.motor_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.motor_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.gear_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.gear_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev1_inform.env_temp = FRONT_TEMP_DEFAULT;

    app_send_record_env.all_front_inform.front_dev2_inform.bearing_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.bearing_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.bearing_inform.dev3_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.bearing_inform.dev4_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.motor_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.motor_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.gear_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.gear_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev2_inform.env_temp = FRONT_TEMP_DEFAULT;

    app_send_record_env.all_front_inform.front_dev3_inform.bearing_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.bearing_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.bearing_inform.dev3_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.bearing_inform.dev4_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.motor_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.motor_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.gear_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.gear_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev3_inform.env_temp = FRONT_TEMP_DEFAULT;

    app_send_record_env.all_front_inform.front_dev4_inform.bearing_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.bearing_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.bearing_inform.dev3_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.bearing_inform.dev4_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.motor_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.motor_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.gear_inform.dev1_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.gear_inform.dev2_inform.temp = FRONT_TEMP_DEFAULT;
    app_send_record_env.all_front_inform.front_dev4_inform.env_temp = FRONT_TEMP_DEFAULT;
}
/**
 * @brief
 * @param
 * @return
 * @note   前置器所有信息初始化为0, 处理板发送到记录板的协议初始化为0
**/
static inline void front_device_inform_init(void)
{
	memset(&app_send_record_env.all_front_inform, 0, sizeof(send_all_front_t));
    memset(&app_send_record_env.clb_record_data, 0, sizeof(clb_record_protocol_t));
    front_temp_init();
}

/**
 * @brief
 * @param
 * @return
 * @note
	前置器状态获取逻辑：
	如果大于APP_SEND_RECORD_FRONT_TIMEOUT_CNT时间没有接收到前置器发送的消息，则前置器状态暂时设置为异常，
	如果4个前置器都异常则重启处理板，否则重启2次前置器后判定最终前置器状态为异常。
**/
static inline void front_device_get_front_state(void)
{
#define front_dev1_state  app_send_record_env.all_front_inform.front_dev1_inform
#define front_dev2_state  app_send_record_env.all_front_inform.front_dev2_inform
#define front_dev3_state  app_send_record_env.all_front_inform.front_dev3_inform
#define front_dev4_state  app_send_record_env.all_front_inform.front_dev4_inform
#define proxi_status proximitor_data.proximitor_status
	paras_t *paras = app_paras_get();
	uint8_t all_state = 0; //所有前置器都故障时，该标志为1

	//前置器1
	//printf("******* front state**** 1:%d 2:%d 3:%d 4:%d\n",
	//		proxi_status.proximitor1_comm_status,proxi_status.proximitor2_comm_status,
	//		proxi_status.proximitor3_comm_status,proxi_status.proximitor4_comm_status);
	proxi_status.proximitor1_comm_status++;
	if(proxi_status.proximitor1_comm_status > APP_SEND_RECORD_FRONT_TIMEOUT_CNT)
	{
		if(proxi_status.proximitor1_comm_status > 200)//deal overturn
		{
			proxi_status.proximitor1_comm_status = APP_SEND_RECORD_FRONT_TIMEOUT_CNT;
		}
		front_dev1_state.front_state = 1;
		memset(proximitor_data.proximitor_data1.temp_orignal, 0, 100);
	}
	else {
		front_dev1_state.front_power_on = proxi_status.proximitor1_power_on;
		front_dev1_state.front_state = 0;
	}
	//前置器2
	proxi_status.proximitor2_comm_status++;
	if(proxi_status.proximitor2_comm_status > APP_SEND_RECORD_FRONT_TIMEOUT_CNT)
	{
		if(proxi_status.proximitor2_comm_status > 200)
		{
			proxi_status.proximitor2_comm_status = APP_SEND_RECORD_FRONT_TIMEOUT_CNT;
		}
		front_dev2_state.front_state = 1;
		memset(proximitor_data.proximitor_data2.temp_orignal, 0, 100);
	}
	else {
		front_dev2_state.front_power_on = proxi_status.proximitor2_power_on;
		front_dev2_state.front_state = 0;
	}

	//前置器3 中间车 或者3个前置的主机
	if(!app_paras_save_env.host_flag || (DEF_QZQ_HOST_NUM_3==app_paras_save_env.host_qzq_num))
	{
		proxi_status.proximitor3_comm_status++;
		if(proxi_status.proximitor3_comm_status > APP_SEND_RECORD_FRONT_TIMEOUT_CNT)
		{
			if(proxi_status.proximitor3_comm_status > 200)
			{
				proxi_status.proximitor3_comm_status = APP_SEND_RECORD_FRONT_TIMEOUT_CNT;
			}
			front_dev3_state.front_state = 1;
			memset(proximitor_data.proximitor_data3.temp_orignal, 0, 100);
		}
		else {
			front_dev3_state.front_power_on = proxi_status.proximitor3_power_on;
			front_dev3_state.front_state = 0;
		}
	}
	else //头尾车
	{
		memset(proximitor_data.proximitor_data3.temp_orignal, 0, 100);
		front_dev3_state.front_power_on = proxi_status.proximitor3_power_on;
		front_dev3_state.front_state = 0;
	}

	//前置器4 中间车
	if(!app_paras_save_env.host_flag)
	{
		proxi_status.proximitor4_comm_status++;
		if(proxi_status.proximitor4_comm_status > APP_SEND_RECORD_FRONT_TIMEOUT_CNT)
		{
			if(proxi_status.proximitor4_comm_status > 200)
			{
				proxi_status.proximitor4_comm_status = APP_SEND_RECORD_FRONT_TIMEOUT_CNT;
			}
			front_dev4_state.front_state = 1;
			memset(proximitor_data.proximitor_data4.temp_orignal, 0, 100);
		}
		else {
			front_dev4_state.front_power_on = proxi_status.proximitor4_power_on;
			front_dev4_state.front_state = 0;
		}
	}
	else //头尾车
	{
		memset(proximitor_data.proximitor_data4.temp_orignal, 0, 100);
		front_dev4_state.front_power_on = proxi_status.proximitor4_power_on;
		front_dev4_state.front_state = 0;
	}

	/* 前置器故障处理逻辑 */
	//中间车
	if(!app_paras_save_env.host_flag)
	{
		all_state = front_dev1_state.front_state && front_dev2_state.front_state && \
				   front_dev3_state.front_state && front_dev4_state.front_state;
	}
	else if (DEF_QZQ_HOST_NUM_3==app_paras_save_env.host_qzq_num)
	{
		all_state = front_dev1_state.front_state && front_dev2_state.front_state && \
				   front_dev3_state.front_state;
	}
	else
	{
		all_state = front_dev1_state.front_state && front_dev2_state.front_state;
	}

	if(all_state)
	{
		if(!paras->sys_para.dev_para.reset_flag)
		{
			rt_kprintf("**all qzq no connect first rest clb** \n");
			qzq_fault_deal(0);//reset clb
		}
	}
	else
	{
		if(front_dev1_state.front_state && (faults.qzqs.qzq1_rest_cnt < 2))
		{
			qzq_fault_deal(1);//reset qzq1
			faults.qzqs.qzq1_rest_cnt++;
			proximitor_data.proximitor_status.proximitor1_comm_status = 0;
			proximitor_data.proximitor_status.proximitor2_power_on = 0;
			front_dev1_state.front_state = 0;
		}
		if(front_dev2_state.front_state && (faults.qzqs.qzq2_rest_cnt < 2))//can't create socket
		{
			qzq_fault_deal(2);//reset qzq2
			faults.qzqs.qzq2_rest_cnt++;
			proximitor_data.proximitor_status.proximitor2_comm_status = 0;
			proximitor_data.proximitor_status.proximitor2_power_on = 0;
			front_dev2_state.front_state = 0;
		}
		if (!app_paras_save_env.host_flag || (DEF_QZQ_HOST_NUM_3==app_paras_save_env.host_qzq_num))
		{
			if(front_dev3_state.front_state && (faults.qzqs.qzq3_rest_cnt < 2))
			{
				qzq_fault_deal(3);//reset qzq3
				faults.qzqs.qzq3_rest_cnt++;
				proximitor_data.proximitor_status.proximitor3_comm_status = 0;
				proximitor_data.proximitor_status.proximitor3_power_on = 0;
				front_dev3_state.front_state = 0;
			}
		}
		if(!app_paras_save_env.host_flag)
		{
			if(front_dev4_state.front_state && (faults.qzqs.qzq4_rest_cnt < 2))
			{
				qzq_fault_deal(4);//reset qzq4
				faults.qzqs.qzq4_rest_cnt++;
				proximitor_data.proximitor_status.proximitor4_comm_status = 0;
				proximitor_data.proximitor_status.proximitor4_power_on = 0;
				front_dev4_state.front_state = 0;
			}
		}
	}

#undef front_dev1_state
#undef front_dev2_state
#undef front_dev3_state
#undef front_dev4_state
#undef proxi_status
}

/**
 * @brief
 * @param
 * @return
 * @note   传感器处理
**/
static inline void sensor_deal_method(vib_sensor_status_p target,union tmp_t *tmp,vib_sensor_status_t vir,temp_sensor_status_t temp)
{
	target->vib_sensor1 = vir.vib_sensor1 & temp.temp_sensor1;
	target->vib_sensor2 = vir.vib_sensor2 & temp.temp_sensor2;
	target->vib_sensor3 = vir.vib_sensor3 & temp.temp_sensor3;
	target->vib_sensor4 = vir.vib_sensor4 & temp.temp_sensor4;

	tmp->bits.st_1 = vir.vib_sensor1 | (temp.temp_sensor1 << 1);
	tmp->bits.st_2 = vir.vib_sensor2 | (temp.temp_sensor2 << 1);
	tmp->bits.st_3 = vir.vib_sensor3 | (temp.temp_sensor3 << 1);
	tmp->bits.st_4 = vir.vib_sensor4 | (temp.temp_sensor4 << 1);
}

/**
 * @brief
 * @param
 * @return
 * @note   传感器故障闪报处理
**/
static inline void sensor_err_flash_alram(uint8_t *src_buf,uint8_t qzq_id)
{
	static uint32_t sensor_err_continous_cnt[16]={0,0,0,0};
	uint8_t cnt_j;uint8_t deal_bit=4;
	qzq_id=qzq_id-1;
	for(cnt_j=0;cnt_j<deal_bit;cnt_j++)
	{
		if((src_buf[0] & (1<<cnt_j))== (1<<cnt_j))
		{
			sensor_err_continous_cnt[4*qzq_id+cnt_j] ++;
		}
		else
		{
			sensor_err_continous_cnt[4*qzq_id+cnt_j] = 0;
		}

		if(sensor_err_continous_cnt[4*qzq_id+cnt_j]>20)
		{
			src_buf[0]|=(1<<cnt_j);			//确认传感器故障
		}
		else
		{
			src_buf[0] &=(~(1<<cnt_j));
		}
	}
}

/**
 * @brief  将温度copy到发送到记录板的buffer中
 * @param
 * @return
 * @note
**/
static void copy_temp_to_sendbuff(int8_t* dts_buff, int8_t source_data, uint16_t* wait_buff)
{
	if (source_data==TEMP_INVALID)
	{
		(*wait_buff) ++;
		if(!app_paras_save_env.temp_flag)
		{
			*dts_buff = FRONT_TEMP_DEFAULT;
		}
	}
	else if (source_data!=FRONT_TEMP_85)
	{
		*wait_buff = 0;
		*dts_buff = source_data + FRONT_TEMP_SHIFT;
	}
	if (*wait_buff > WAIT_TEMP_TIME)
	{
		rt_kprintf("temp error  wait  timeout(10min) ------- reboot clb\n");
		*wait_buff = 0;
		if(!app_paras_save_env.clb_reboot)
		{
			rt_hw_cpu_reset();//reset clb
		}
	}
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
static inline int8_t parsing_temperature(uint16_t * data, uint8_t len)
{
	OS_ASSERT(data != OS_NULL);
	int sum;
	char temp;
	uint16_t avail_data[10];
	int avail_num = 0; //用于存放可用温度的数量：针对每秒钟采集的10个温度中，有一部分为-127另一部分又是正常温度的情况，避免异常温度出现
	int sun_num = 0; //用于存放求和温度的数量：针对丢掉温度与中心温度相差5°以上丢掉的处理

	for(int i = 0; i < len; i++)
	{
		if(0==data[i]) continue; //0:是传感器未采到温度时读数，2120:是传感器给85°时的读数
		avail_data[avail_num] = data[i];
		avail_num ++;
	}
	bubble_sort(avail_data,avail_num);//排序

	sum = 0;
	temp = 0;
//	rt_kprintf("parsing_temperature------ %d %d %d %d %d %d %d %d %d %d\n",data[0],data[1],data[2],data[3],data[4],data[5],data[6],data[7],data[8],data[9]);
	//去掉最大最小值求和再求平均值
	for(int i = 0; i < avail_num; i++)
	{
		if ((avail_data[i]>(avail_data[avail_num/2]+50)) || (avail_data[i]<(avail_data[avail_num/2]-50))) continue; //丢掉与中心温度差别5°以上的值
		sum += avail_data[i];
		sun_num ++;
	}
	if (sun_num)
	{
		sum /= sun_num;
		sum -= 1270;
		sum = sum / 10;
		temp = (int8_t)sum ;
	}
	else //若没有有效温度值，直接给出默认温度值；
	{
		temp = (int8_t)TEMP_INVALID;
	}

	return temp;
}

/**
 * @brief
 * @param
 * @return
 * @note   前置器信息包括轴箱传感器，齿端电机轴承传感器，小齿轮箱车轮侧轴承传感器
**/
static inline void front_device_get_front_data(uint8_t front_num)
{
//#define front_inform app_send_record_env.all_front_inform.front_dev1_inform
//#define front_data proximitor_data.proximitor_data1
	send_front_t* front_inform = NULL;
	proximitor_data_t*          front_data = NULL;
	static temp_wait_t temp_wait[4] = {0};
	uint8_t front_subscript = 0;//前置器对应buffer的下标

	if((front_num < 1) || (front_num > 4)) //
	{
		return ;
	}
	else if(front_num == 1)
	{
		front_inform = &app_send_record_env.all_front_inform.front_dev1_inform;
		front_data = &proximitor_data.proximitor_data1;
	}
	else if(front_num == 2)
	{
		front_inform = &app_send_record_env.all_front_inform.front_dev2_inform;
		front_data = &proximitor_data.proximitor_data2;
	}
	else if(front_num == 3)
	{
		front_inform = &app_send_record_env.all_front_inform.front_dev3_inform;
		front_data = &proximitor_data.proximitor_data3;
	}
	else if(front_num == 4)
	{
		front_inform = &app_send_record_env.all_front_inform.front_dev4_inform;
		front_data = &proximitor_data.proximitor_data4;
	}
	else {
		return ;
	}

	vib_sensor_status_t vib_sensor_status;
	union tmp_t sensorast;

	memset(&vib_sensor_status, 0, sizeof(vib_sensor_status_t));
	sensorast.byte=0;
	sensor_deal_method(&vib_sensor_status, &sensorast, front_data->vib_sensor_status,
			             front_data->temp_sensor_status);
	sensor_err_flash_alram((uint8_t *)&vib_sensor_status, front_num);
	front_subscript = front_num - 1;
	//中间车 或 3个前置器主机的2，3号前置器
	if(!app_paras_save_env.host_flag || ((front_num>=2)&&(DEF_QZQ_HOST_NUM_3==app_paras_save_env.host_qzq_num)))
	{
		if(front_inform->front_state==0 && front_inform->front_power_on==1)
		{
			front_inform->bearing_inform.dev1_inform.state = vib_sensor_status.vib_sensor1;
			if(front_inform->bearing_inform.dev1_inform.state == 0) {
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
						              (parsing_temperature(front_data->temp_orignal[0],10)),\
						              &temp_wait[front_subscript].zxzc_temp[0]);
			}
			else {
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
						              &temp_wait[front_subscript].zxzc_temp[0]);
			}
			front_inform->bearing_inform.dev2_inform.state = vib_sensor_status.vib_sensor2;
			if(front_inform->bearing_inform.dev2_inform.state == 0) {
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
										(parsing_temperature(front_data->temp_orignal[1],10)),\
										&temp_wait[front_subscript].zxzc_temp[1]);
			}
			else {
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
						              &temp_wait[front_subscript].zxzc_temp[1]);
			}
			front_inform->gear_inform.dev1_inform.state = vib_sensor_status.vib_sensor3;
			if(front_inform->gear_inform.dev1_inform.state == 0) {
				copy_temp_to_sendbuff((int8_t*)&front_inform->gear_inform.dev1_inform.temp,\
										(parsing_temperature(front_data->temp_orignal[2],10)),\
										&temp_wait[front_subscript].clx_temp[0]);
			}
			else {
				copy_temp_to_sendbuff((int8_t*)&front_inform->gear_inform.dev1_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
						              &temp_wait[front_subscript].clx_temp[0]);
			}
			front_inform->motor_inform.dev1_inform.state = vib_sensor_status.vib_sensor4;
			if(front_inform->motor_inform.dev1_inform.state == 0) {
				copy_temp_to_sendbuff((int8_t*)&front_inform->motor_inform.dev1_inform.temp,\
										(parsing_temperature(front_data->temp_orignal[3],10)),\
										&temp_wait[front_subscript].dj_temp[0]);
			}
			else {
				copy_temp_to_sendbuff((int8_t*)&front_inform->motor_inform.dev1_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
						              &temp_wait[front_subscript].dj_temp[0]);
			}
			copy_temp_to_sendbuff((int8_t*)&front_inform->env_temp,\
									(parsing_temperature(front_data->temp_orignal[4],10)),\
									&temp_wait[front_subscript].env_temp);
		}
		else
		{
			front_inform->bearing_inform.dev1_inform.state = 0;
			copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
								  FRONT_TEMP_DEFAULT,\
					              &temp_wait[front_subscript].zxzc_temp[0]);
			front_inform->bearing_inform.dev2_inform.state = 0;
			copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
								  FRONT_TEMP_DEFAULT,\
					              &temp_wait[front_subscript].zxzc_temp[1]);
			front_inform->gear_inform.dev1_inform.state = 0;
			copy_temp_to_sendbuff((int8_t*)&front_inform->gear_inform.dev1_inform.temp,\
								  FRONT_TEMP_DEFAULT,\
					              &temp_wait[front_subscript].clx_temp[0]);
			front_inform->motor_inform.dev1_inform.state = 0;
			copy_temp_to_sendbuff((int8_t*)&front_inform->motor_inform.dev1_inform.temp,\
								  FRONT_TEMP_DEFAULT,\
					              &temp_wait[front_subscript].dj_temp[0]);
		}
	}
	else //头尾车
	{
		if(((front_num<=2)&&(DEF_QZQ_HOST_NUM_2==app_paras_save_env.host_qzq_num)) ||
				((front_num<=1)&&(DEF_QZQ_HOST_NUM_3==app_paras_save_env.host_qzq_num)))/*qzq1*/
		{
			if(front_inform->front_state == 0 && front_inform->front_power_on==1)
			{
				front_inform->bearing_inform.dev1_inform.state = vib_sensor_status.vib_sensor1;
				if(front_inform->bearing_inform.dev1_inform.state == 0) {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
											(parsing_temperature(front_data->temp_orignal[0],10)),\
											&temp_wait[front_subscript].zxzc_temp[0]);
				}
				else {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
											FRONT_TEMP_DEFAULT,\
											&temp_wait[front_subscript].zxzc_temp[0]);
				}
				front_inform->bearing_inform.dev2_inform.state = vib_sensor_status.vib_sensor2;
				if(front_inform->bearing_inform.dev2_inform.state == 0) {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
											(parsing_temperature(front_data->temp_orignal[1],10)),\
											&temp_wait[front_subscript].zxzc_temp[1]);
				}
				else {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
											FRONT_TEMP_DEFAULT,\
											&temp_wait[front_subscript].zxzc_temp[1]);
				}
				front_inform->bearing_inform.dev3_inform.state = vib_sensor_status.vib_sensor3;
				if(front_inform->bearing_inform.dev3_inform.state == 0) {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev3_inform.temp,\
											(parsing_temperature(front_data->temp_orignal[2],10)),\
											&temp_wait[front_subscript].zxzc_temp[2]);
				}
				else {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev3_inform.temp,\
											FRONT_TEMP_DEFAULT,\
											&temp_wait[front_subscript].zxzc_temp[2]);
				}
				front_inform->bearing_inform.dev4_inform.state = vib_sensor_status.vib_sensor4;
				if(front_inform->bearing_inform.dev4_inform.state == 0) {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev4_inform.temp,\
											(parsing_temperature(front_data->temp_orignal[3],10)),\
											&temp_wait[front_subscript].zxzc_temp[3]);
				}
				else {
					copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev4_inform.temp,\
											FRONT_TEMP_DEFAULT,\
											&temp_wait[front_subscript].zxzc_temp[3]);
				}
				copy_temp_to_sendbuff((int8_t*)&front_inform->env_temp,\
										(parsing_temperature(front_data->temp_orignal[4],10)),\
										&temp_wait[front_subscript].env_temp);
			}
			else
			{
				front_inform->bearing_inform.dev1_inform.state = 0;
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev1_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
									  &temp_wait[front_subscript].zxzc_temp[0]);
				front_inform->bearing_inform.dev2_inform.state = 0;
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev2_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
									  &temp_wait[front_subscript].zxzc_temp[1]);
				front_inform->bearing_inform.dev3_inform.state = 0;
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev3_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
									  &temp_wait[front_subscript].zxzc_temp[2]);
				front_inform->bearing_inform.dev4_inform.state = 0;
				copy_temp_to_sendbuff((int8_t*)&front_inform->bearing_inform.dev4_inform.temp,\
									  FRONT_TEMP_DEFAULT,\
									  &temp_wait[front_subscript].zxzc_temp[3]);
				front_inform->env_temp = FRONT_TEMP_DEFAULT;
			}
		}
	}
//#undef front_inform
//#undef front_data
}

/**
 * @brief
 * @param
 * @return
 * @note   前置器信息包括轴箱传感器，齿端电机轴承传感器，小齿轮箱车轮侧轴承传感器
**/
static inline void front_device_get_front_inform(void)
{
	front_device_get_front_data(1);
	front_device_get_front_data(2);
	front_device_get_front_data(3);
	front_device_get_front_data(4);
}

















