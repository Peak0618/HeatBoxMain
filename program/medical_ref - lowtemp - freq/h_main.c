#ifndef _MAIN_C_
#define _MAIN_C_

//---------new--------

//------------------------------------------------------------------------------
//包含文件声明
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"

#include "h_type_define.h"
#include "h_main.h"
#include "h_com.h"
#include "h_adg.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "h_rom_para2.h"
//#include "h_comp2.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "m_peripheral_control.h"
#include "m_key_operation.h"
#include "m_PTC_heat.h"
#include "m_test.h"
//------------------------------------------------------------------------------
//函数声明
void system_init(void);  //系统初始化程序

void switch_init(void);  //拨码开关初始化

void control_init(void); //控制初始化程序

void system_start(void); //系统启动程序

void main_loop(void);    //主循环程序

void timer_op(void);     //定时程序，在主循环程序中调用

void ad_convert_deal(void);   //ad转换处理程序，在1ms定时程序中调用

void timer_int(void);    //1ms定时中断程序

void init_ram_para2(void);

void init_ram_para(void);

void eeprom2_read_deal(void); //eeprom2读取处理程序，在初始化程序中调用

void eeprom_read_deal(void);  //eeprom读取处理程序，在初始化程序中调用

void delay_10us(uint16_t _10us);   //10us延时程序，在eeprom读写程序中调用

void delay_1ms(uint16_t _1ms);     //1ms延时程序，在eeprom读写程序中调用

void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data);      //写eeprom程序

int16_t eeprom_read(uint16_t lus_addr); //读eeprom程序

void output_deal(void);       //输出处理程序，在主循环程序中调用

void RL_output_deal(void);    //继电器输出处理程序，在主循环程序中调用

void RL_output_delaytime(void);    //继电器输出延时程序，在1ms定时程序中调用

void trip_lampblink_deal(void);    //故障灯闪烁程序，在10ms定时程序中调用

void input_pin_delaytime(void);    //输入引脚延时程序，在10ms定时程序中调用


//------------------------------------------------------------------------------
//485通讯控制模式宏定义
#define   COM_RX_MODE    (P1.0 = 0)     //接收模式
#define   COM_TX_MODE    (P1.0 = 1)     //发送模式

#define   NET_RX_MODE    (P0.4 = 0)     //接收模式
#define   NET_TX_MODE    (P0.4 = 1)     //发送模式




//------------------------------------------------------------------------------
//变量定义
flag_type flg_time;
//----------------------------------------------------------
uint8_t   guc_10ms_timer;     //10毫秒计时器
uint8_t   guc_100ms_timer;    //100ms定时器
uint8_t   guc_1s_timer;       //1s定时器
uint8_t   guc_1min_timer;     //1min定时器

uint8_t   guc_RL_output_delaytimer;     //继电器输出延时计时器

uint8_t   guc_trip_flash_cnt;      //故障灯闪烁计数器
uint8_t   guc_trip_flash_timer;    //故障灯闪烁计时器

uint8_t   guc_trip_code; //故障代码

int16_t   gss_tmp;
//----------------------------------------------------------
uint8_t   guc_door1_pin_high_delaytimer;     //门开关1引脚高延时计时器
uint8_t   guc_door1_pin_low_delaytimer;      //门开关1引脚低延时计时器

uint8_t   guc_door2_pin_high_delaytimer;     //门开关2引脚高延时计时器
uint8_t   guc_door2_pin_low_delaytimer;      //门开关2引脚低延时计时器

uint8_t   guc_bat_fed_pin_high_delaytimer;   //电池防插反引脚高延时计时器
uint8_t   guc_bat_fed_pin_low_delaytimer;    //电池防插反引脚低延时计时器
//----------------------------------------------------------

const int16_t gss_buzzer_register[10] = 
{
  900,    //1
  800,    //2
  900,    //3
  1000,   //4
  1100,   //5
  1200,   //6
  1300,   //7
  1400,   //8
  1500,   //9
  1600    //10
};
//------------------------------------------------------------------------------
//程序内容//
//------------------------------------------------------------------------------
void system_init(void)   //系统初始化程序
{
    int16_t    gss_E2_writed_flag;

    switch_init();            //拨码开关初始化
        
    gss_E2_writed_flag = eeprom_read(255);
    if(gss_E2_writed_flag != 0xdd)        //变频aa.双系统bb.血液cc.加温箱dd
    {        
        init_ram_para2();       
        init_ram_para();
        
        eeprom_write(255,0xdd);
    }
    
    //----------------------------------
    //init_ram_para2();            // peak 烧写E2，新板子要先写一次；
    //init_ram_para();
    
    eeprom2_read_deal(); //eeprom2读取处理程序
    eeprom_read_deal();  //eeprom读取处理程序
    
    key_parm_default_val();

    //----------------------------------
    R_ADC_Set_OperationOn();  //ad转换启动
    //----------------------------------
    com_init();     //通讯初始化程序
    //----------------------------------
    adg_init();     //ad模块初始化程序
    //----------------------------------
    control_init(); //控制初始化程序
    //----------------------------------
}
//------------------------------------------------------------------------------
void switch_init(void)       //拨码开关初始化
{
    if (SW18_IN_PIN == 1)          //peak拨码开关不动它，默认状态是高电平
    {
        bflg_bat_type = 0;         //铅酸电池    //peak 用此电池    
    }
    else
    {
        bflg_bat_type = 1;         //锂电池
    }
}
//------------------------------------------------------------------------------
void control_init(void)  //控制初始化程序
{
    bflg_self_test = 1;       //初始上电时系统自检标志
    //------------------------------------------------------
    //12V初始化关闭
    LOCK_OUT_PIN = 0;  //LOCK_OUT_PIN = 1; peak 原来的
    HTRMT_OUT_PIN = 0;
    HTRMXG_OUT_PIN = 0;
    HTRPHK_OUT_PIN = 0;
    //------------------------------------------------------
    TRIP_LAMP_PIN = 0;   //灯亮
    //------------------------------------------------------
    guc_100ms_timer = 100;
    guc_1s_timer = 100;
    guc_1min_timer = 60;
    //------------------------------------------------------
    guc_ad_calc_cnt = 0;
    bflg_ad_calc_ok = 0;
    //------------------------------------------------------
    BAT_DISCHARGE_PIN = 1;
    
    bflg_bat_frist_charge = 1;//首次上电进行电池充电
    //------------------------------------------------------
    bflg_adg_reset = 1;  //ad模块复位
    //------------------------------------------------------

 
    bflg_prot2_TA_first_delaytime = 1;
    gss_prot2_TA_first_delaytimer = 0;
    
    bflg_prot2_TC_first_delaytime = 1;
    gss_prot2_TC_first_delaytimer = 0;
    
    bflg_prot2_bat_low_delaytime = 1;
    gss_prot2_bat_low_delaytimer = 0;
    
    bflg_prot2_bat_space_delaytime = 1;
    gss_prot2_bat_space_delaytimer = 0;
    
    bflg_PTC_heat_power_on_delaytime = 1;     
    guc_PTC_heat_power_on_delaytime = 0;
    //----------------------------------
    //debug
    //bflg_bat_discharging = 1; //初始为放电状态 peak放在函数中了
    //----------------------------------
    //init_freq_ctrl();
}
//------------------------------------------------------------------------------
void system_start(void)  //系统启动程序
{
    R_TAU0_Channel0_Start();   //1ms定时启动
    
    R_ADC_Start();             //AD转换启动
    
    R_TAU0_Channel2_Start();
    
    R_TAU0_Channel5_Start();
    
    R_TAU1_Channel2_Start();
    R_TAU1_Channel3_Start();
}
//------------------------------------------------------------------------------
void main_loop(void)    
{   
    //------------------------------------------------------
    timer_op();                    //定时程序
    //------------------------------------------------------
    if (bflg_allow_ad_calc == 1)   //如果允许ad计算
    {
        bflg_allow_ad_calc = 0;
        ad_temp_calc();            //ad温度计算程序
    }
    //------------------------------------------------------
    if (bflg_com_type_ok == 1)
    {
        bflg_bat_type = bflg_com_type;  //得到电池类型 peak有通过拨码开关来选择电池类型的，但是通过通讯选择设置在后面，所以依据通讯选择；
    }

    if (bflg_test_mode == 1)       //如果是测试模式
    {
        test_mode_com_deal();      
        test_mode_output_deal();   
        test_mode_input_deal();    
    }
    else                      
    {
        fan_RL_out();
        led_RL_out();
        germicidal_lamp_RL_out();
        remote_RL_alarm_lamp();
        PTC_heat_RL_out();
        
        //--------------
        output_deal();           
        RL_output_deal();           
    }
    
    //------------------------------------------------------
    //通讯
    if (bflg_com_allow_tx == 1)    
    {
        bflg_com_allow_tx = 0;
        //------------------------------
        R_UART0_Start();
        COM_TX_MODE;
        com_tx_init();    
    }
    //----------------------------------
    if (bflg_com_tx_ok == 1)  
    {
        bflg_com_tx_ok = 0;
        guc_IC_erasure_tx_ok_count++;
        if(guc_IC_erasure_tx_ok_count >= 4)    //发送给主控板2次之后再清0,防止发送过去主控没有收到；
        {
            guc_IC_erasure_tx_ok_count = 0;
            bflg_IC_erasure_order = 0;
        }
        //------------------------------
        R_UART0_Stop();
    }
    //----------------------------------
    if (bflg_com_allow_rx == 1)     
    {
        bflg_com_allow_rx = 0;
        //------------------------------
        com_rx_init();    
        COM_RX_MODE;
        R_UART0_Start();
    }
    //----------------------------------
    if (bflg_com_rx_end == 1)  
    {
        bflg_com_rx_end = 0;
        //------------------------------
        R_UART0_Stop();
    }
    //----------------------------------
    if (bflg_com_rx_ok == 1)   
    {
        bflg_com_rx_ok = 0;
        //------------------------------
        R_UART0_Stop();
        com_rx_data_deal();    
    }



    //----------------------------------
    if (bflg_read_key_delaytime == 0)   //如果读键值时间间隔到
    {
        bflg_read_key_delaytime = 1;
        gss_read_key_delaytimer = 0;
        
        //read_key_deal();      //读键值处理程序
        key_buzz_cancel();    //报警取消30分钟
        
        mode_selection();
    }
    //----------------------------------
    if (bflg_ad_calc_ok == 1)
    {
        prot2_power_off_deal();    //断电保护处理程序
        
        port2_power_fault_deal();  //电源板故障处理程序
        
        prot2_bat_low_deal();      //电池电量低保护处理程序
        
        port2_T_fault_deal();      //传感器故障处理程序
               
        bat_T_hight_protect();     //电池温度过高判断peak
    }
    
    //----------------------------------
    //电池
    if (bflg_test_mode == 1)  //如果是测试模式
    {
        RL_CON_PIN = 1;
        gss_bat_pwm_value = 50;  
        TDR01 = gss_bat_pwm_value;
    }
    else if ((bflg_prot2_bat_fed_pin == 1) || (bflg_alarm_power_fault == 1) ||(bflg_alarm_bat_discnnt == 1)||
             (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat2_T_hight == 1)  )  
    {   
        bflg_bat_charging = 0;                     //若未达到充电情况则立即停充电不需10s了
        gss_bat_pwm_value = 0;
    }
   /* else if (bflg_comp2_power_on_delaytime == 0)   //上电延时到后才执行
    {         
        if (bflg_bat_charge_update == 1)           //每10s更新一次,充电情况
        {
            bflg_bat_charge_update = 0;
            bat_charge_deal();                     //电池充电处理程序
            TDR01 = gss_bat_pwm_value;             
         }
         
        //------------------------------
        bat_discharge_deal();                      //电池放电处理程序
    }*/


    //----------------------------------
    bat_con_check_again();  //peak add
    card_IC_deal();         //peak add

    //加温箱相关
    fan_control_deal();
    PTC_heat_fre_mode_judge();
    alarm_buzz_deal();
    germicidal_lamp_control_deal();    
}

//------------------------------------------------------------------------------
void timer_op(void)      //定时程序，在主循环程序中调用
{
    //------------------------------------------------------
    if (bflg_1ms_reach == 1)  //如果1ms定时到
    {
        bflg_1ms_reach = 0;
        guc_100ms_timer--;
        //--------------------------------------------------
        //1ms定时调用程序
        //--------------------------------------------------
        ad_convert_deal();    //ad转换处理程序
        //--------------------------------------------------
        com_rx_delaytime();   //通讯接收延时程序
        
        com_tx_delaytime();   //通讯发送延时程序
        
        com_rx_end_delaytime();    //通讯接收完成延时程序
        //--------------------------------------------------
        RL_output_delaytime();     //继电器输出延时程序
    }
    //------------------------------------------------------
    if (bflg_10ms_reach == 1)
    {
        bflg_10ms_reach = 0;
        guc_1s_timer--;
        //--------------------------------------------------
        //10ms定时调用程序
        trip_lampblink_deal();              //故障灯闪烁程序
        
        if(bflg_self_test == 1)             //初始上电时自检
        {   
            if(bflg_com_type_ok == 1)
            {              
                system_self_test();
            }
        }
        else if(bflg_factory_AT_test==1)    //在工厂参数测试指令中
        {
            key_factory_AT_test();
        }
        else
        {
            led_disp1_deal();          //数码管1显示处理程序
            
            //led_disp2_deal();          //数码管2显示处理程序
            
            //led_disp3_deal();          //数码管3显示处理程序
            
            //lamp_disp_deal();          //灯显示处理程序
            led_lamp_disp();
        }

        
        read_key_delaytime();          //读键延时程序
        
        input_pin_delaytime();         //输入引脚延时程序

        

    }
    //------------------------------------------------------
    if (guc_100ms_timer == 0)
    {
        guc_100ms_timer = 100;
        //--------------------------------------------------
        //100ms定时调用程序
        //key_locked_delaytime();         //按键锁定延时程序
        
        
        adg_data_delaytime();           //ad模块数据处理延时程序
        
        prot2_bat_space_delaytime();    //电池未连接检测间隔延时程序
        
        prot2_bat_discnnt_delaytime();  //电池未连接保护延时程序      //peak  原来给注释掉了---

		//peak add
		led_loop_delaytime();               //led闪烁标志
        led_disp1_flicker_delaytime();      //设定时数据闪烁
	    buzz_loop_tick_delaytime();         //滴滴声延时程序
        buzz_one_tick_delaytime();          //响一声延时        
        buzz_sound_priority();              //不同响声的优先级
        buzz_sound_cnt();
        view_error_code_delaytime();        //错误代码显示时间
        //--------------------------------------------------
        //血液相关
        key_locked_delaytime();
        key_set_long_delaytime();
        key_set_up_long_delaytime();
        key_up_down_long_delaytime();
        key_set_down_long_delaytime();
    }
    //------------------------------------------------------
    if (guc_1s_timer == 0)
    {
        guc_1s_timer = 100;
        guc_1min_timer--;
        //--------------------------------------------------
        //1s定时调用程序
        //--------------------------------------------------
        com_fault_delaytime();     //通讯故障延时程序
        //--------------------------------------------------
 
        //blood_freq_ctrl_deal();
        //TDR13 = gus_comp21_freq_cnt;
        
        test_mode_LED_deal();   
        //--------------------------------------------------
        heat_box_TDIS_high_delaytime();  //主传感器高温保护延时程序
        
        heat_box_TDIS_low_delaytime();   //主传感器低温保护延时程序
        //--------------------------------------------------
        prot2_THW_high_delaytime();//环温传感器过高保护延时程序
        //--------------------------------------------------
        port2_door_delaytime();    //门开关保护延时程序
        //--------------------------------------------------
        prot2_cond_dirty_delaytime();   //冷凝器脏保护延时程序
        //--------------------------------------------------
               
        
        //--------------------------------------------------
        bat_charge_delaytime();    //电池充电延时程序

		//peak add
		//buzz_loop_tick_delaytime();         //  滴滴声延时程序
		lock_open_delaytimer();
		//key_IC_register_delaytime();
		
		//加温箱相关
		PTC_heat_control_deal();
    }
    //------------------------------------------------------
    if (guc_1min_timer == 0)
    {
        guc_1min_timer = 60;
        //--------------------------------------------------
        //1min定时调用程序

        //--------------------------------------------------
        
        prot2_TC_first_delaytime();     //冷凝器脏首次保护延时程序
        
        prot2_bat_low_delaytime();      //电池电量低延迟程序
        
        alarm_buzz_off_delaytime();     //蜂鸣器报警取消延时程序
        //--------------------------------------------------
        bat_charge_continue_delaytime();     //充电程序延时程序
        
        bat_charge_space_delaytime();   //电池充电间隔延时程序
        //加温箱相关
        PTC_heat_power_on_delaytime();
        led_key_on_delaytime();
        germicidal_lamp_delaytime();
    }
}

//------------------------------------------------------------------------------
/*******************************************************************************
函数功能:  

函数位置: 1ms定时中

名词解析: ADCR: 转换结果寄存器
          ADCR  寄存器将 A/D 转换结果保持在其高 10 位（低 6 位固定为 0）;
                因为用的是10位ad转换；
          
          ADS: 模拟输入通道选择寄存器       
********************************************************************************/
void ad_convert_deal(void)    //ad转换处理程序，在1ms定时程序中调用
{
    gus_ad_val = (uint16_t)(ADCR >> 6U);
    //------------------------------------------------------
    ad_val_deal();  //ad值处理程序
    //------------------------------------------------------
    ADS = (uint8_t)(guc_ad_index + 2); //ad通道选择，peak因为硬件从第2通道开始的
    //------------------------------------------------------
    R_ADC_Start();  //启动ad转换 peak 每次都要开始还是每改变一次通道都要开始；
}
//------------------------------------------------------------------------------
void timer_int(void)     //1ms定时中断程序
{
    bflg_1ms_reach = 1;       //置1ms到标志位
    //----------------------------------
    guc_10ms_timer++;
    if (guc_10ms_timer >= 10) //10ms计时到
    {
        guc_10ms_timer = 0;
        bflg_10ms_reach = 1;
    }    
}
//------------------------------------------------------------------------------
void init_ram_para2(void)
{
    uint8_t i;
    for (i = 0; i < MAX_PARA_NUMBER2; i++)
    {
        //ram_para2[i] = EEPROM_init2[i][0];
        eeprom_write(i + 256, EEPROM_init2[i][0]);
    }
}
//------------------------------------------------------------------------------
void init_ram_para(void)
{
    uint8_t i;
    for (i = 0; i < MAX_PARA_NUMBER; i++)
    {
        //ram_para[i] = EEPROM_init[i][0];
        eeprom_write(i, EEPROM_init[i][0]);
    }
}
//------------------------------------------------------------------------------
void eeprom2_read_deal(void)  //eeprom2读取处理程序，在初始化程序中调用
{
    int16_t i;
    uint8_t luc_read_para_cnt;
    int16_t lss_verify_word;
    
    do
    {
        bflg_askfor_read_eeprom = 0;    //清请求读EEP标志
        //----------------------------------------
        for (i = 0; i < MAX_PARA_NUMBER2; i++)
        {
            ram_para2[i] = eeprom_read(i + 256); //从EEP读参数到RAM中
            R_WDT_Restart();
            
            if ((ram_para2[i] < EEPROM_init2[i][min_row])
               || (ram_para2[i] > EEPROM_init2[i][max_row]))
            {
                bflg_read_eeprom_error = 1;  //置读EEPROM错标志
                break;
            }
        }
        //--------------------------------------------------
        if (bflg_read_eeprom_error == 1)     //如果有读EEPROM错标志
        {
            bflg_read_eeprom_error = 0;      //清读EEPROM错标志
            
            luc_read_para_cnt++;   //读EEP次数加1
            if (luc_read_para_cnt >= 3)      //如果读3次错
            {
                //EEPROM故障处理
                bflg_eeprom_fault = 1;  //置EEP故障标志
                break;
            }
            else
            {
                bflg_askfor_read_eeprom = 1; //置请求读EEPROM标志
            }
        }
    }
    while (bflg_askfor_read_eeprom == 1);
}
//------------------------------------------------------------------------------
void eeprom_read_deal(void)   //eeprom读取处理程序，在初始化程序中调用
{
    int16_t i;
    uint8_t luc_read_para_cnt;
    int16_t lss_verify_word;
    
    do
    {
        bflg_askfor_read_eeprom = 0;    //清请求读EEP标志
        //----------------------------------------
        for (i = 0; i < MAX_PARA_NUMBER; i++)
        {
            ram_para[i] = eeprom_read(i);     //从EEP读参数到RAM中
            R_WDT_Restart();
            
            if ((ram_para[i] < EEPROM_init[i][min_row])
               || (ram_para[i] > EEPROM_init[i][max_row]))
            {
                bflg_read_eeprom_error = 1;  //置读EEPROM错标志
                break;
            }
        }
        //--------------------------------------------------
        if (bflg_read_eeprom_error == 1)     //如果有读EEPROM错标志
        {
            bflg_read_eeprom_error = 0;      //清读EEPROM错标志
            
            luc_read_para_cnt++;   //读EEP次数加1
            if (luc_read_para_cnt >= 3)      //如果读3次错
            {
                //EEPROM故障处理
                bflg_eeprom_fault = 1;  //置EEP故障标志
                break;
            }
            else
            {
                bflg_askfor_read_eeprom = 1; //置请求读EEPROM标志
            }
        }
    }
    while (bflg_askfor_read_eeprom == 1);
}
//------------------------------------------------------------------------------
void delay_10us(uint16_t _10us)    //10us延时程序，在eeprom读写程序中调用
{
    uint16_t i,j;
    for (i = 0; i < (_10us * 2); i++)
    {
        j = i * 3;
        R_WDT_Restart();
    }
}
//------------------------------------------------------------------------------
void delay_1ms(uint16_t _1ms)      //1ms延时程序，在eeprom读写程序中调用
{
    uint16_t i,j;
    for (i = 0; i < (_1ms * 159); i++)
    {
        j = i * 3;
        R_WDT_Restart();
    }
}
//------------------------------------------------------------------------------
void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data)    //写eeprom程序
{
    uint8_t luc_page_addr;
    uint8_t luc_unit_addr;
    uint8_t luc_driver_write;
    
    uint8_t luc_eep_data_H;
    uint8_t luc_eep_data_L;
    
    luc_page_addr = (uint8_t) (lus_addr * 2 / 256);
    luc_unit_addr = (uint8_t) (lus_addr * 2 % 256);
    luc_driver_write = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA0);
    
    luc_eep_data_H = (uint8_t) (((uint16_t) lss_eep_data) / 256);
    luc_eep_data_L = (uint8_t) (((uint16_t) lss_eep_data) % 256);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1) {}        //总线忙，则等待
    
    STT0 = 1;                      //产生start信号
    WREL0 = 0;                     //发送模式
    ACKE0 = 0U;                    //无ACK信号
    IICA0 = luc_driver_write;      //写驱动地址
    delay_10us(20);                //等待
    
    IICA0 = luc_unit_addr;         //单元地址
    delay_10us(20);                //等待
    
    IICA0 = luc_eep_data_H;            
    delay_10us(20);                //等待
    SPT0 = 1;                      //产生stop信号
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1){}         //总线忙，则等待
    
    STT0 = 1;                      //产生start信号
    WREL0 = 0;                     //发送模式
    ACKE0 = 0U;                    //无ACK信号
    IICA0 = luc_driver_write;      //写驱动地址
    delay_10us(20);                //等待
    
    IICA0 = (uint8_t)(luc_unit_addr + 1);     //单元地址
    delay_10us(20);                //等待
    
    IICA0 = luc_eep_data_L;
    delay_10us(20);                //等待
    SPT0 = 1;                      //产生stop信号
    //------------------------------------------------------
    delay_1ms(3);
}
//------------------------------------------------------------------------------
int16_t eeprom_read(uint16_t lus_addr)  //读eeprom程序
{
    uint8_t luc_page_addr;
    uint8_t luc_unit_addr;
    uint8_t luc_driver_write;
    uint8_t luc_driver_read;
    uint8_t luc_eep_data_H;
    uint8_t luc_eep_data_L;
    
    luc_page_addr = (uint8_t) (lus_addr * 2 / 256);
    luc_unit_addr = (uint8_t) (lus_addr * 2 % 256);
    luc_driver_write = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA0);
    luc_driver_read = (uint8_t) (( luc_page_addr << 1) & 0x0E | 0xA1);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1) {}        //总线忙，则等待
    
    STT0 = 1;                      //产生start信号
    WREL0 = 0;                     //发送模式
    ACKE0 = 0U;                    //无ACK信号
    IICA0 = luc_driver_write;      //写驱动地址
    delay_10us(20);                //等待
    
    IICA0 = luc_unit_addr;         //单元地址
    delay_10us(20);                //等待
    
    STT0 = 1;                      //产生start信号
    IICA0 = luc_driver_read;       //读驱动地址    
    delay_10us(20);                //等待
    WREL0 = 1;                     //接收模式
    
    delay_10us(20);                //等待
    luc_eep_data_H = IICA0;        //得到数据
    ACKE0 = 1;                     //产生ACK信号
    SPT0 = 1;                      //产生stop信号
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    R_WDT_Restart();
    while (IICBSY0 == 1){}         //总线忙，则等待
    
    STT0 = 1;                      //产生start信号
    WREL0 = 0;                     //发送模式
    ACKE0 = 0U;                    //无ACK信号
    IICA0 = luc_driver_write;      //写驱动地址
    delay_10us(20);                //等待
    
    IICA0 = (uint8_t)(luc_unit_addr + 1);     //单元地址
    delay_10us(20);                //等待
    
    STT0 = 1;                      //产生start信号
    IICA0 = luc_driver_read;       //读驱动地址
    delay_10us(20);                //等待
    WREL0 = 1;                     //接收模式
    
    delay_10us(20);                //等待
    luc_eep_data_L = IICA0;        //读数据
    ACKE0 = 1;                     //产生ACK信号
    SPT0 = 1;                      //产生stop信号
    //------------------------------------------------------
    delay_1ms(3);
    //------------------------------------------------------
    return ((int16_t) (luc_eep_data_H) * 256 + luc_eep_data_L);
}

//------------------------------------------------------------------------------
void output_deal(void)        //输出处理程序，在主循环程序中调用
{

}
//------------------------------------------------------------------------------
void RL_output_deal(void)     //继电器输出处理程序，在主循环程序中调用
{

}
//------------------------------------------------------------------------------
void RL_output_delaytime(void)     //继电器输出延时程序，在1ms定时程序中调用
{
    if (bflg_RL_output_delaytime == 1)
    {
        guc_RL_output_delaytimer++;
        if (guc_RL_output_delaytimer >= 200)
        {
            guc_RL_output_delaytimer = 0;
            bflg_RL_output_delaytime = 0;
        }
    }
}
//------------------------------------------------------------------------------
void trip_lampblink_deal(void)     //故障灯闪烁程序，在10ms定时程序中调用
{
    if (guc_trip_code != 0)       //如果不是跳停，则转
    {
        if (guc_trip_flash_cnt > 0)     //如果闪烁次数已经减到零，则开始3秒间隔延时
        {
            guc_trip_flash_timer++;
            if (guc_trip_flash_timer >= 30)  //每0.3秒闪烁一次
            {
                guc_trip_flash_timer = 0;
                TRIP_LAMP_PIN = ~TRIP_LAMP_PIN;
                guc_trip_flash_cnt--;
            }
        }
        else
        {
            TRIP_LAMP_PIN = 0;
            guc_trip_flash_timer++;
            if (guc_trip_flash_timer >= 200)   //2秒间隔延时
            {
                guc_trip_flash_timer = 0;
                guc_trip_flash_cnt = guc_trip_code;
                guc_trip_flash_cnt <<= 1;
            }
        }
    }
    else
    {
        TRIP_LAMP_PIN = 0;
        guc_trip_flash_timer = 0;
        guc_trip_flash_cnt = 0;
    }
}
/******************************************************************************************************************************
函数功能:     输入引脚的电平检测：1、开门 2、电池插反

函数位置：  10ms---------------------------------------ok          
******************************************************************************************************************************/
void input_pin_delaytime(void)      
{
    if (DOOR1_IN_PIN == 0)                   // 0关门
    {
        guc_door1_pin_high_delaytimer = 0;
        
        if (bflg_prot2_door1_pin == 1)
        {
            guc_door1_pin_low_delaytimer++;
            if (guc_door1_pin_low_delaytimer >= 100)   //1s
            {
                guc_door1_pin_low_delaytimer = 0;
                
                bflg_prot2_door1_pin = 0;
            }
        }
    }
    else                                      // 1开门      
    { 
        guc_door1_pin_low_delaytimer = 0;
        
        if (bflg_prot2_door1_pin == 0)
        {
            guc_door1_pin_high_delaytimer++;
            if (guc_door1_pin_high_delaytimer >= 100)  //1s
            {
                guc_door1_pin_high_delaytimer = 0;
                
                bflg_prot2_door1_pin = 1;
            }
        }
    }
    
    //------------------------------------------------------
	// 电池反接处理
    if (BAT_FED_IN_PIN == 1)  //电池未反接
    {
        guc_bat_fed_pin_high_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 1)       // 反接报警标志
        {
            guc_bat_fed_pin_low_delaytimer++;
            if (guc_bat_fed_pin_low_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_low_delaytimer = 0;
                
                bflg_alarm_bat_fed = 0;    
                //gus_trip_code1 &= ~0x0800;
            }  
        }
    }
    else     // 电池反接  
    {
        guc_bat_fed_pin_low_delaytimer = 0;
        
        if (bflg_alarm_bat_fed == 0)
        {
            guc_bat_fed_pin_high_delaytimer++;
            if (guc_bat_fed_pin_high_delaytimer >= 100) //1s
            {
                guc_bat_fed_pin_high_delaytimer = 0;
                
                bflg_alarm_bat_fed = 1;
                //gus_trip_code1 |= 0x0800;
            }
        }
    }
}

//------------------------------------------------------------------------------
#endif
