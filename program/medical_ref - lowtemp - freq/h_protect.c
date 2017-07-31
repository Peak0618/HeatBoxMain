#ifndef _PROTECT_C_
#define _PROTECT_C_
//------------------------------------------------------------------------------
//包含文件声明
#include "h_type_define.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_key_disp.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "h_main.h"
#include "m_key_operation.h"
//------------------------------------------------------------------------------
//函数声明
void prot2_TA_first_delaytime(void);    //主传感器首次高低温保护延时程序，在1min定时程序中调用

void heat_box_TDIS_high_delaytime(void);     //主传感器高温保护延时程序，在1s定时程序中调用

void heat_box_TDIS_low_delaytime(void);      //主传感器低温保护延时程序，在1s定时程序中调用

void prot2_power_off_deal(void);   //断电保护处理程序，在主循环程序中调用

void port2_power_fault_deal(void); //电源板故障处理程序，在主循环程序中调用

void prot2_bat_discnnt_delaytime(void); //电池未连接保护延时程序，在100ms定时程序中调用

void prot2_bat_space_delaytime(void);   //电池未连接检测间隔延时程序，在100ms定时程序中调用

void prot2_bat_low_deal(void);     //电池电量低保护处理程序，在主循环程序中调用

void prot2_bat_low_delaytime(void);//电池电量低延迟程序，在1min定时程序中调用

void port2_door_delaytime(void);   //门开关保护延时程序，在1s定时程序中调用

void port2_T_fault_deal(void);     //传感器故障处理程序，在主循环程序中调用

void prot2_THW_high_delaytime(void);    //环温传感器过高保护延时程序，在1s定时程序中调用

void prot2_TC_first_delaytime(void);    //冷凝器脏首次保护延时程序，在1min定时程序中调用

void prot2_cond_dirty_delaytime(void);  //冷凝器脏保护延时程序，在1s定时程序中调用

void prot2_volt_deal(void);   //电压超标报警保护处理程序，在主循环程序中调用

void alarm_lamp_deal(void);   //报警灯处理程序，在主循环程序中调用

void alarm_buzz_deal(void);   //蜂鸣器报警处理程序，在主循环程序中调用

void alarm_buzz_off_delaytime(void);    //蜂鸣器报警取消延时程序，在1min定时程序中调用

void alarm_output_deal(void);                //远程报警处理程序，在主循环程序中调用
void bat_T_hight_protect(void);             //电池温度过高判断



//------------------------------------------------------------------------------
//变量定义
flag_type flg_alarm,flg_alarm1,flg_alarm2,flg_prot2;

//peak add
flag_type   memory;

//------------------------------------------------------------------------------
int16_t   gss_heat_box_TDIS_high_delaytimer; //主传感器高温保护延时计时器
int16_t   gss_heat_box_TDIS_low_delaytimer;  //主传感器低温保护延时计时器
int16_t   gss_prot2_TA_first_delaytimer;//主传感器首次保护延时计时器

int16_t   gss_prot2_bat_discnnt_delaytimer;  //电池未连接保护延时计时器
int16_t   gss_prot2_bat_space_delaytimer;    //电池未连接检测间隔延时计时器
int16_t   gss_prot2_bat_low_delaytimer;      //电池电量低延时计时器

int16_t   gss_prot2_door_delaytimer;    //门开关保护延时计时器

int16_t   gss_prot2_THW_high_delaytimer;     //环温传感器过高保护延时计时器

int16_t   gss_prot2_TC_first_delaytimer;//冷凝器脏首次保护延时计时器
int16_t   gss_prot2_cond_dirty_delaytimer;   //冷凝器脏延时计时器

int16_t   gss_alarm_buzz_off_delaytimer;     //蜂鸣器报警取消延时计时器

uint16_t  gus_trip_code1;     //故障代码1
uint16_t  gus_trip_code2;     //故障代码2

/******************************************************************************************************************************************
函数功能：  初始上电3小时后再判断高温报警，

函数位置：1min----------------------------因为加温箱不用--未调      
******************************************************************************************************************************************/
void prot2_TA_first_delaytime(void)     
{
    if (bflg_prot2_TA_first_delaytime == 1)
    {
        gss_prot2_TA_first_delaytimer++;
        if (gss_prot2_TA_first_delaytimer >= ram_para[num_prot2_TA_first_delaytime])      //180分钟
        {
            gss_prot2_TA_first_delaytimer = 0;
            bflg_prot2_TA_first_delaytime = 0;
        }
    }
}
/********************************************************************************************************************************
function : 显示传感器高温报警---加温箱
      

函数位置：1s-----------------------------------------------ok
*********************************************************************************************************************************/
void heat_box_TDIS_high_delaytime(void)     
{
    if (bflg_alarm_ALH == 0)
    {
        if (gss_TDIS >= gss_user_set_parm_value[DISP_USER_SET_ALH])      
        {
            gss_heat_box_TDIS_high_delaytimer++;
            if (gss_heat_box_TDIS_high_delaytimer >= 60)   //60s
            {
                gss_heat_box_TDIS_high_delaytimer = 0;
                bflg_alarm_ALH = 1;
                gus_trip_code1 |= H_T_CODE;    
            }
        }
        else
        {
            gss_heat_box_TDIS_high_delaytimer = 0;
        }
    }
    else                                     
    {
        if (gss_TDIS < gss_user_set_parm_value[DISP_USER_SET_ALH]  - 10)   // -1℃
        {
            gss_heat_box_TDIS_high_delaytimer++;
            if (gss_heat_box_TDIS_high_delaytimer >= 60)                       //60s
            {
                gss_heat_box_TDIS_high_delaytimer = 0;
                bflg_alarm_ALH = 0;
                gus_trip_code1 &= ~H_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_high_delaytimer = 0;
        }
    }
}
/*****************************************************************************************************************************************
函数功能：显示传感器低温报警的成立和恢复---加温箱

函数位置：1s------------------------------------------ok
*****************************************************************************************************************************************/
void heat_box_TDIS_low_delaytime(void)  
{
    if (bflg_alarm_ALL == 0)
    {
        if (gss_TDIS <= gss_user_set_parm_value[DISP_USER_SET_ALL])
        {
            gss_heat_box_TDIS_low_delaytimer++;
            if (gss_heat_box_TDIS_low_delaytimer >= 60)     //60s
            {
                gss_heat_box_TDIS_low_delaytimer = 0;
                bflg_alarm_ALL = 1;
                gus_trip_code1 |= L_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_low_delaytimer = 0;
        }
    }
    else
    {
        if (gss_TDIS > gss_user_set_parm_value[DISP_USER_SET_ALL] + 10)    // 1℃
        {
            gss_heat_box_TDIS_low_delaytimer++;
            if (gss_heat_box_TDIS_low_delaytimer >= 60)    //60
            {
                gss_heat_box_TDIS_low_delaytimer = 0;
                bflg_alarm_ALL = 0;
                gus_trip_code1 &= ~L_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_low_delaytimer = 0;
        }
    }
}
/*******************************************************************************
function : 断电报警的成立和恢复

判断条件： 检测到电源电压＜80V
处理措施： 报警灯闪烁， 蜂鸣器响， 远程报警输出；
恢复条件： 检测到电源电压＞90V 恢复；

*******************************************************************************/
void prot2_power_off_deal(void)    //断电保护处理程序，在主循环程序中调用
{
    if (gss_VAC <= ram_para2[num_power_fautl_VAC1])         //80V
    {
        bflg_alarm_power_off = 1;
        //gus_trip_code1 |= VAC_OFF_CODE; 
    }
    else if (gss_VAC >= ram_para2[num_power_fautl_VAC2])    //90V
    {
        bflg_alarm_power_off = 0;
        //gus_trip_code1 &= ~VAC_OFF_CODE;
    }
}
/****************************************************************************************************************************************
函数功能 : 12v电源板故障的成立和恢复


函数位置:  主循环-----------------------------ok
****************************************************************************************************************************************/
void port2_power_fault_deal(void)     
{
    if (guc_12V < ram_para2[num_power_off_12V])  //9v
    {
        if (bflg_alarm_power_off == 0)           //peak 在220没有断电的情况下,12v是从220转换来的；
        {
            bflg_alarm_power_fault = 1;
            gus_trip_code1 |= DC_F_CODE;
        }
        else
        {
            bflg_alarm_power_fault = 0;
            gus_trip_code1 &= ~DC_F_CODE;
        }
    }
    else if (guc_12V > ram_para2[num_power_on_12V]) //11v
    {
        bflg_alarm_power_fault = 0;
        gus_trip_code1 &= ~DC_F_CODE;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 电池未连接故障的成立和恢复

下为功能书:
1． 非充电状态时
判断条件： 检测电池电压＜2.0V；
处理措施： 报警灯闪烁， 蜂鸣器响；
恢复条件： 未充电时， 检测电池电压＞5.0V

2． 充电状态时
判断条件： 检测电池电压＜9.0V；
处理措施： 报警灯闪烁， 蜂鸣器响；
恢复条件： 未充电时， 检测电池电压＞10.0V

********************************************************************************/
void prot2_bat_discnnt_delaytime(void)  //电池未连接保护延时程序，在100ms定时程序中调用
{
    if (bflg_bat_charging == 0)              //如果当前为未充电状态
    {
        if (bflg_alarm_bat_discnnt == 0)     //如果当前无电池未连接故障
        {
            if (guc_bat_Vout_instant < ram_para2[num_port2_bat_discnnt_V1])   //peak 改为2v 了
            {
                bflg_alarm_bat_discnnt = 1;
                //gus_trip_code1 |= 0x1000;
            }
        }
        else   //如果当前有电池未连接故障
        {
            if (guc_bat_Vout_instant > ram_para2[num_port2_bat_discnnt_V2])   //5.0V
            {
                bflg_alarm_bat_discnnt = 0;
                //gus_trip_code1 &= ~0x1000;
            }
        }
    }
    else   //如果当前为充电状态
    {
        if (bflg_alarm_bat_discnnt == 0)     //如果当前无电池未连接故障
        {
            //if (guc_bat_Vout_instant < 90)//ram_para2[num_port2_bat_discnnt_V1]) //peak 功能书为 9v
            if ((guc_bat_Vout_instant < 90) || (guc_bat_Vout_instant > 151))       //peak充电时若检测<9v或>15.1v则判定为断开电池
            {
                bflg_alarm_bat_discnnt = 1;
                //gus_trip_code1 |= 0x1000;
            }
        }
        else   //如果当前有电池未连接故障
        {
            if (guc_bat_Vout_instant > 100)//ram_para2[num_port2_bat_discnnt_V2])       //peak 功能书为 10v
            {
                bflg_alarm_bat_discnnt = 0;
                //gus_trip_code1 &= ~0x1000;
            }
        }
    }
}

//------------------------------------------------------------------------------
/*******************************************************************************
function : 貌似没用~~
********************************************************************************/
void prot2_bat_space_delaytime(void)    //电池未连接检测间隔延时程序，在100ms定时程序中调用
{
    if (bflg_prot2_bat_space_delaytime == 1)
    {
        gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 200) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 0;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
    else
    {
    	  gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 50) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 1;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
    //----------------------------------
    /*if (bflg_prot2_bat_space_reach == 1)
    {
        gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 50) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 1;
            
            bflg_prot2_bat_space_reach = 0;
        }
    }*/
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 电池电量低故障的成立和恢复

判断条件:都是在电池充电24小时后，且电池连接的情况下判断的；


恢复条件:1、电池未连接，2、电压>阀值  (3、在充电时不能检测已被去掉 )
*******************************************************************************/
void prot2_bat_low_deal(void)      //电池电量低保护处理程序，在主循环程序中调用
{    
    //debug 正常运行时，去掉注释
    if ((bflg_prot2_bat_low_delaytime == 0) && (bflg_alarm_bat_discnnt == 0))   //充电24小时后，且电池未连接//peak 没有未连接
	{   //充电结束，且无电池未连接报警
        if (bflg_bat_type == 0)    //如果是铅酸电池
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V1])   //10.5V
            {
                bflg_alarm_bat_low = 1;
                //gus_trip_code1 |= 0x2000;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V2])   //11.0V
            {
                bflg_alarm_bat_low = 0;
                //gus_trip_code1 &= ~0x2000;
            }
        }
        else   //如果是锂电池
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V3])   //9.0V
            {
                bflg_alarm_bat_low = 1;
                //gus_trip_code1 |= 0x2000;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V4])   //9.5V
            {
                bflg_alarm_bat_low = 0;
                //gus_trip_code1 &= ~0x2000;
            }
        }
    }
    else
    {
        bflg_alarm_bat_low = 0;
        //gus_trip_code1 &= ~0x2000;
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 初始上电充电24小时后再对电池电量低进行检测---peak 个人觉不合理
          应该一直对电池电量低进行检测；
******************************************************************************/
void prot2_bat_low_delaytime(void) //电池电量低延迟程序，在1min定时程序中调用
{
    if (bflg_prot2_bat_low_delaytime == 1)
    {
        gss_prot2_bat_low_delaytimer++;
        if (gss_prot2_bat_low_delaytimer >= (ram_para2[num_prot2_bat_low_delaytime] * 60)) //24小时
        {
            gss_prot2_bat_low_delaytimer = 0;
            bflg_prot2_bat_low_delaytime = 0;
        }
    }
}

/*******************************************************************************************************************************
函数功能: 开门报警的成立和恢复


函数位置：1s--------------------------------------------ok
*******************************************************************************************************************************/
void port2_door_delaytime(void)    
{
    if ((bflg_prot2_door1_pin == 1) || (bflg_prot2_door2_pin == 1))
    {
        if (bflg_alarm_open_door == 0)
        {
            gss_prot2_door_delaytimer++;
            //if (gss_prot2_door_delaytimer >= (ram_para[num_prot2_door_delaytime] * 60))  //5分钟
            if (gss_prot2_door_delaytimer >= (gss_user_set_parm_value[DISP_USER_SET_dA] * 60))        //5分钟 用可调的gss_user_set_parm_value[DISP_USER_SET_dA]替换之前的固定值
            {   
                gss_prot2_door_delaytimer = 0;
                
                bflg_alarm_open_door = 1;
                gus_trip_code1 |= DOOR_OP_CODE;
            }
        }
    }
    else
    {
        gss_prot2_door_delaytimer = 0;
        bflg_alarm_open_door = 0;
        gus_trip_code1 &= ~DOOR_OP_CODE;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 传感器故障报警的成立和恢复:

判断条件： 当有主传感器、 冷凝传感器、 环温传感器、 热交换器传感器故障时；
处理措施： 报警灯闪烁， 蜂鸣器响， 远程报警输出；
恢复条件： 当主传感器、 冷凝传感器、 环温传感器、 热交换器传感器都正常时， 恢复；

********************************************************************************/
void port2_T_fault_deal(void)      //传感器故障处理程序，在主循环程序中调用
{
    if (bflg_THW_fault == 1)    //peak 环温报警
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TE_fault == 1) //peak 热交换器温度传感器故障标志        
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TC_fault == 1) //peak 冷凝器温度传感器故障标志
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TA_fault == 1) //peak 主传感器故障标志
    {
        bflg_alarm_T_fault = 1;
    }
    else
    {
        bflg_alarm_T_fault = 0;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 环温过高报警的成立和恢复

判断条件： 环温＞35℃（ EEP） 持续 30 秒（ EEP）；
处理措施： 报警灯闪烁；
恢复条件： 环温≤34℃（ EEP） 持续 30 秒（ EEP）；
         （此时如果显示环温＜35℃， 则显示 35℃）

********************************************************************************/
void prot2_THW_high_delaytime(void)     //环温传感器过高保护延时程序，在1s定时程序中调用
{/*
    if (bflg_alarm_THW_high == 0)
    {
        //if (gss_THW > ram_para[num_prot2_THW_T])     //35原来的       
        if (gss_THW > gss_factory_AA)                  //可调
        {
            gss_prot2_THW_high_delaytimer++;
            if (gss_prot2_THW_high_delaytimer >= ram_para2[num_prot2_THW_high_delaytime]) //30
            {
                gss_prot2_THW_high_delaytimer = 0;
                bflg_alarm_THW_high = 1;
                gus_trip_code1 |= 0x0400;
            }
        }
        else
        {
            gss_prot2_THW_high_delaytimer = 0;
        }
        
        bflg_prot2_THW_high_disp = 0;
    }
    else
    {
        //if (gss_THW <= (ram_para[num_prot2_THW_T] + ram_para2[num_prot2_THW_Tdelt]))     //34原来的
        if (gss_THW <= (gss_factory_AA + ram_para2[num_prot2_THW_Tdelt]))                  //可调
        {
            bflg_prot2_THW_high_disp = 1;
            
            gss_prot2_THW_high_delaytimer++;
            if (gss_prot2_THW_high_delaytimer >= ram_para2[num_prot2_THW_high_delaytime]) //30
            {
                gss_prot2_THW_high_delaytimer = 0;
                bflg_alarm_THW_high = 0;
                gus_trip_code1 &= ~0x0400;
                
                bflg_prot2_THW_high_disp = 0;
            }
        }
        else
        {
            gss_prot2_THW_high_delaytimer = 0;
            
            bflg_prot2_THW_high_disp = 0;
        }
    }*/
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 冷凝器脏报警的一个条件--初始上电 2 小时后判断，为下面的函数服务
******************************************************************************/
void prot2_TC_first_delaytime(void)     //冷凝器脏首次保护延时程序，在1min定时程序中调用
{
    if (bflg_prot2_TC_first_delaytime == 1)
    {
        gss_prot2_TC_first_delaytimer++;
        if (gss_prot2_TC_first_delaytimer >= ram_para2[num_prot2_TC_first_delaytime])     //120
        {
            gss_prot2_TC_first_delaytimer = 0;
            bflg_prot2_TC_first_delaytime = 0;
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 冷凝器脏报警的成立和恢复

判断条件：1、 初始上电 2 小时后判断：
          2、 冷凝器传感器温度-环温传感器温度≥13℃持续 5 分钟；

处理措施： 报警灯闪烁， 蜂鸣器响；

恢复条件： 冷凝器传感器温度-环温传感器温度＜10℃ 持续 5 分钟;

******************************************************************************/
void prot2_cond_dirty_delaytime(void)   //冷凝器脏保护延时程序，在1s定时程序中调用
{
    if (bflg_prot2_TC_first_delaytime == 0)          //初始上电经过2小时了
    {
        if (bflg_alarm_cond_dirty == 0)              //没有报警时，判断是否有报警
        {
            if ((gss_TC - gss_THW) >= ram_para2[num_prot2_cond_dirty_TC1])   //13℃
            {
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s = 5min
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 1;
                    //gus_trip_code1 |= 0x0200;
                }
            }
            else                                    
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
        else                                          //有报警时，判断是否解除报警    
        {
            if ((gss_TC - gss_THW) < ram_para2[num_prot2_cond_dirty_TC2])    //10℃   peak原来还是TC1
            {
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 0;
                    //gus_trip_code1 &= ~0x0200;
                }
            }
            else
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 电压超标报警的成立和恢复

1． 如果是 220V 电源
	判断条件： 电源电压≤184V， 或电源电压≥253V；
	处理措施： 报警灯闪烁， 蜂鸣器响；
	恢复条件： 194V≤电源电压≤243V 时；
	
2． 如果是 115V 电源
	判断条件： 电源电压≤93V， 或电源电压≥136V；
	处理措施： 报警灯闪烁， 蜂鸣器响；
	恢复条件： 98V ≤电源电压≤131V 时；

********************************************************************************/
void prot2_volt_deal(void)    //电压超标报警保护处理程序，在主循环程序中调用
{
   /*if (bflg_comp2_volt_type == 0) //如果是220V电源
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V1]) || (gss_VAC >= ram_para2[num_prot2_volt_V2]))       //184V    253V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V3]) && (gss_VAC <= ram_para2[num_prot2_volt_V4]))   //194V   243V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }
    else       //如果是115V电源
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V5]) || (gss_VAC >= ram_para2[num_prot2_volt_V6]))        //93V   136V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V7]) && (gss_VAC <= ram_para2[num_prot2_volt_V8]))   //98V   131V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }*/
}

/******************************************************************************
function: 1、显示板上的的3位报警灯是否显示的处理;
          2、只要有任意一种报警，显示板上的3位报警灯都会常亮；

          bflg_alarm_lamp = 1 表示显示面板上"报警"的那三个灯一直亮；
******************************************************************************/
void alarm_lamp_deal(void)             //报警灯处理程序，在主循环程序中调用
{
    if (((flg_alarm.uword & 0xff) != 0)||     //有报警标志为1的
        ((flg_alarm1.uword & 0xff) != 0))
    {        
        bflg_alarm_lamp = 1;
    }
    else
    {
        bflg_alarm_lamp = 0;
    }
}

/*****************************************************************************************************************************
function: 处理 蜂鸣器 报警 --加温箱

函数位置；主循环－－－－－－－－－－－ok
*****************************************************************************************************************************/
void alarm_buzz_deal(void)    //蜂鸣器报警处理程序，在主循环程序中调用
{   
    if (bflg_test_mode == 0)         //不在测试模式下才处理      
    {
        flag_type memory_temp ;      

        memory_temp.uword = memory.uword;  //memory_temp存储上一次的故障 
                                           //flg_alarm 最新的故障，用来判断蜂鸣器滴滴响  
        memory.uword= flg_alarm.uword;     //memory 将flg_alarm最新的故障赋给它值，用来和上次的存储的故障值 memory_temp对比，判断是否有新的故障变化，若按下'蜂鸣取消'键，则可以清30内不响

       //--------------------------------------------------------------------------------
       
       if((flg_alarm.uword  == 0)||                                        //没有报警
          (bflg_alarm_buzz_off_delaytime == 1))//||                        //有报警，但按下"蜂鸣取消"
          //((flg_alarm.uword == 0x0008 )&&(bflg_key_long_buz_valid == 1))) //只有电池未连接报警，但是长按"蜂鸣取消"5s了(现在为设置参数bs = 1)
        {                                                                   
            bflg_buzz_loop_tick_set = 0;                                   //以上3种情况不滴滴          

            if(memory_temp.uword != memory.uword)                          //有新的报警时，清按"蜂鸣取消"30min不响，又开始响
            {
                bflg_alarm_buzz_off_delaytime = 0;
                gss_alarm_buzz_off_delaytimer = 0;
            }
        }           
        else
        {
            bflg_buzz_loop_tick_set = 1;                                    //其他情况滴滴响
        }

        
        /*        //电池插反 加温箱无电池
        if (bflg_alarm_bat_fed == 1)               //peak 电池接反报警标志 6
        {
            bflg_buzz_always_sound_set = 1;        //peak 蜂鸣器长响
        }
        else
        {
            bflg_buzz_always_sound_set = 0;
        }	*/
    }
}

/*******************************************************************************
function:   任意报警蜂鸣器响时， 按下“蜂鸣取消”键， 蜂鸣器不响， 
            30 分钟后恢复正常控制； 
            
函数位置: 1min定时程序中调用      -----------ok        
********************************************************************************/
void alarm_buzz_off_delaytime(void)     
{
    if (bflg_alarm_buzz_off_delaytime == 1)
    {
        gss_alarm_buzz_off_delaytimer++;
        //if (gss_alarm_buzz_off_delaytimer >= ram_para[num_alarm_buzz_off_delaytime])     //30分钟
        if (gss_alarm_buzz_off_delaytimer >= gss_factory_parm_value[DISP_FAC_Fd]*10)                               //改为可调
        {
            gss_alarm_buzz_off_delaytimer = 0;
            bflg_alarm_buzz_off_delaytime = 0;
        }
    }
}


/**********************************************************************************
功能描述 : 电池温度过高保护 ,cn22和cn23两个接电池的口都一样

            当电池温度≥65℃（EEP）时，停止电池充放电； 
            当电池温度＜55℃（EEP）时，恢复电池充放电。

备注:  cn22 cn23两个口都作了温度过高的保护，虽然铅酸电池只有正负没有温度传感器，
       但是在没有接时候ad采集为1024，此时温读为0，不会过高；

函数位置: 主循环中-----------------------------------------------ok
**********************************************************************************/
void bat_T_hight_protect(void)
{
    //bat1
    if (gss_Tbat1 >= ram_para2[num_bat2_discharge_off_T])       //65℃   电池1 、2都用这个标准
    {
        bflg_alarm_bat1_T_hight = 1;
    }
    else if (gss_Tbat1 <= ram_para2[num_bat2_discharge_on_T])   //55℃
    {
        bflg_alarm_bat1_T_hight = 0;
    }

    //bat2
    //--------------------------------------------------------------------
    if (gss_Tbat2 >= ram_para2[num_bat2_discharge_off_T])       //65℃
    {
        bflg_alarm_bat2_T_hight = 1;
    }
    else if (gss_Tbat2 <= ram_para2[num_bat2_discharge_on_T])   //55℃
    {
        bflg_alarm_bat2_T_hight = 0;
    }

}


//------------------------------------------------------------------------------
#endif
