#ifndef _BAT_CTRL_C_
#define _BAT_CTRL_C_
//------------------------------------------------------------------------------
//包含文件声明
#include "h_type_define.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_protect.h"
#include "h_bat_ctrl.h"

//------------------------------------------------------------------------------
//函数声明
void bat_charge_deal(void);   //电池充电处理程序，在主循环程序中调用

void bat_charge_delaytime(void);   //电池充电延时程序，在1s定时程序中调用

void bat_charge_continue_delaytime(void);    //充电程序延时程序，在1min定时程序中调用

void bat_charge_space_delaytime(void);  //电池充电间隔延时程序，在1min定时程序中调用

void bat_discharge_deal(void);     //电池放电处理程序，在主循环程序中调用

//------------------------------------------------------------------------------
//变量定义
flag_type flg_bat;

int16_t   gss_bat_charge_delaytimer;    //电池充电延时计时器

int16_t   gss_bat_pwm_value;  //电池充电pwm占空比值
int16_t   gss_bat_pwm_cnt;    //电池充电pwm占空比计数器

int16_t   gss_charge_continue_delaytimer;    //电池充电持续延时计时器

uint16_t  gus_bat_charge_space_delaytimer;   //电池充电间隔延时计时器
//------------------------------------------------------------------------------
//程序内容
//------------------------------------------------------------------------------
/******************************************************************************
function: 对铅酸电池 和 锂电池 的充电管理

函数位置: 主循环-受update影响也是每10s检测一次----------------------ok          
******************************************************************************/
void bat_charge_deal(void)    //电池充电处理程序，在主循环程序中调用
{
    //如果有电池插反，或电源板故障，或电池未连接都停止充电,电池温度过高也停止充电
    if ((bflg_prot2_bat_fed_pin == 1) || (bflg_alarm_power_fault == 1) ||(bflg_alarm_bat_discnnt == 1)||
        (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat2_T_hight == 1))  
    {   
        bflg_bat_charging = 0;
        gss_bat_pwm_value = 0;
    }
    else
    {
        if (bflg_bat_frist_charge == 1) //如果是首次电池充电
        {
            bflg_bat_frist_charge = 0;  //清首次电池充电标志
            
            bflg_bat_charging = 1;  //电池充电启动
            bflg_bat_charging_end = 0;
            gss_bat_pwm_value = 0;
            
            bflg_bat_charge_space_delaytime = 0;
            gus_bat_charge_space_delaytimer = 0;
            
            bflg_prot2_bat_low_delaytime = 1;       //peak 1 首次上电时之后不会再有  此.c共5处//别处也打开，只要是刚开始充电就置1，为了判断电池电量低
            gss_prot2_bat_low_delaytimer = 0;
        }
        else
        {
            if (bflg_bat_type == 0)   //如果是铅酸电池
            {
                if (bflg_bat_charging == 1)     //如果当前在充电
                {
                    if (bflg_bat_charging_end == 0)
                    {
                        if (guc_bat_Vout < ram_para2[num_bat1_charge_V1])     //13.5V//第一阶段
                        {
                            if (gss_bat_I > ram_para2[num_bat1_charge_I1])    //610mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat1_charge_I2])    //590mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat1_charge_V2])     //14.3V
                        {
                            if (gss_bat_I > ram_para2[num_bat1_charge_I3])         //270mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat1_charge_I4])    //250mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else
                        {
                            bflg_bat_charging_end = 1;
                        }
                    }
                    else    //如果是充电结束阶段
                    {
                        if (guc_bat_Vout > ram_para2[num_bat1_charge_V4])     //13.9V
                        {
                            gss_bat_pwm_value--;
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat1_charge_V5])     //13.7V
                        {
                            gss_bat_pwm_value++;
                        }
                    }
                }
                else   //如果当前未充电
                {
                    if (guc_bat_Vout < ram_para2[num_bat1_charge_V3])    //12.4V
                    {
                        bflg_bat_charging = 1;  //电池充电启动
                        bflg_bat_charging_end = 0;
                        gss_bat_pwm_value = 0;
                        
                        bflg_bat_charge_space_delaytime = 0;
                        gus_bat_charge_space_delaytimer = 0;
                        
                        bflg_prot2_bat_low_delaytime = 1;          //peak 2  <12.4v时进行的充电 
                        gss_prot2_bat_low_delaytimer = 0;          //peak  之前为了在充电状态下不能检测电量低的故障，现在注释掉，充电时可检测；//又开了
                    } 
                    else
                    {
                        if (bflg_bat_charge_space_delaytime == 0)  //peak 间隔30天到
                        {
                            bflg_bat_charging = 1;   //电池充电启动
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                            
                            bflg_prot2_bat_low_delaytime = 1;   //peak 3  间隔30天的充电
                            gss_prot2_bat_low_delaytimer = 0;
                        }
                        else
                        {
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                        }
                    }
                }
                //------------------------------
                if (gss_bat_pwm_value < 0)
                {
                    gss_bat_pwm_value = 0;
                }
                else if (gss_bat_pwm_value > 200)
                {
                    gss_bat_pwm_value = 200;
                }
            }
            else  //如果是锂电池
            {
                if (bflg_bat_charging == 1)     //如果当前在充电
                {
                    if (bflg_bat_charging_end == 0)
                    {
                        if (guc_bat_Vout < ram_para2[num_bat2_charge_V1])     //12.0V//第一阶段
                        {
                            if (gss_bat_I > ram_para2[num_bat2_charge_I1])    //710mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat2_charge_I2])    //690mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat2_charge_V2])     //12.4V
                        {
                            if (gss_bat_I > ram_para2[num_bat2_charge_I3])         //330mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat2_charge_I4])    //310mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else
                        {
                            bflg_bat_charging_end = 1;
                        }
                    }
                    else    //如果是充电结束阶段
                    {
                        if (guc_bat_Vout > ram_para2[num_bat2_charge_V4])     //12.9V
                        {
                            gss_bat_pwm_value--;
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat2_charge_V5])     //12.7V
                        {
                            gss_bat_pwm_value++;
                        }
                    }
                }
                else   //如果当前未充电
                {
                    if (guc_bat_Vout < ram_para2[num_bat2_charge_V3])    //11.2V
                    {
                        bflg_bat_charging = 1;  //电池充电启动
                        bflg_bat_charging_end = 0;
                        gss_bat_pwm_value = 0;
                        
                        bflg_bat_charge_space_delaytime = 0;
                        gus_bat_charge_space_delaytimer = 0;
                        
                        bflg_prot2_bat_low_delaytime = 1;       //peak 4
                        gss_prot2_bat_low_delaytimer = 0;
                    }
                    else
                    {
                        if (bflg_bat_charge_space_delaytime == 0)
                        {
                            bflg_bat_charging = 1;   //电池充电启动
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                            
                            bflg_prot2_bat_low_delaytime = 1;   //peak 5
                            gss_prot2_bat_low_delaytimer = 0;
                        }
                        else
                        {
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                        }
                    }
                }
                //------------------------------
                if (gss_bat_pwm_value < 0)
                {
                    gss_bat_pwm_value = 0;
                }
                else if (gss_bat_pwm_value > 250)
                {
                    gss_bat_pwm_value = 250;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 每间隔1s 对电池充电进行的状况进行一次查询；
          会调用 bat_charge_deal();
          
函数位置: 1s定时器中------------------------ok          
********************************************************************************/
void bat_charge_delaytime(void)    //电池充电延时程序，在1s定时程序中调用
{
    gss_bat_charge_delaytimer++;
    if (gss_bat_charge_delaytimer >= 10)     //10s
    {
        gss_bat_charge_delaytimer = 0;
        
        bflg_bat_charge_update = 1;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 铅酸电池充电36个小时后结束
          锂电池充电7小时后结束；
         
*******************************************************************************/
void bat_charge_continue_delaytime(void)     //充电程序延时程序，在1min定时程序中调用
{
    if (bflg_bat_charging == 1)
    {
        gss_charge_continue_delaytimer++;
        if (bflg_bat_type == 0)    //如果是铅酸电池
        {
            if (gss_charge_continue_delaytimer >= (ram_para2[num_bat1_charge_continue_delaytime] * 60))     //36小时
            {
                gss_charge_continue_delaytimer = 0;
                
                bflg_bat_charging = 0;
                bflg_bat_charge_space_delaytime = 1;
                gus_bat_charge_space_delaytimer = 0;
            }
        }
        else   //如果是锂电池
        {
            if (gss_charge_continue_delaytimer >= (ram_para2[num_bat2_charge_continue_delaytime] * 60))     //7小时
            {
                gss_charge_continue_delaytimer = 0;
                
                bflg_bat_charging = 0;
                bflg_bat_charge_space_delaytime = 1;
                gus_bat_charge_space_delaytimer = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 电池充完一次电，在等30天再充(期间若别的条件达到，也能充电)

*******************************************************************************/
void bat_charge_space_delaytime(void)   //电池充电间隔延时程序，在1min定时程序中调用
{
    if (bflg_bat_charge_space_delaytime == 1)
    {
        gus_bat_charge_space_delaytimer++;
        if (bflg_bat_type == 0)    //如果是铅酸电池
        {
            if (gus_bat_charge_space_delaytimer >= (ram_para2[num_bat1_charge_sapce_delaytime] * 24 * 60))    //30天
            {
                gus_bat_charge_space_delaytimer = 0;
                bflg_bat_charge_space_delaytime = 0;
            }
        }
        else   //如果是锂电池
        {
            if (gus_bat_charge_space_delaytimer >= (ram_para2[num_bat2_charge_sapce_delaytime] * 24 * 60))    //30天
            {
                gus_bat_charge_space_delaytimer = 0;
                bflg_bat_charge_space_delaytime = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function:   检测到电源电压＜80V 则为断电状态， bflg_alarm_power_off = 1；

            bflg_bat_discharging = 0  电池停止放电

            bflg_bat_discharging = 1  初始为放电状态

函数位置； 主循环--------------------上电延时过后可用------------------ok           
******************************************************************************/
void bat_discharge_deal(void)      //电池放电处理程序，在主循环程序中调用
{
///*
    //电池插反、电池温度过高不放电；peak电池未连接是否也放?
    if((bflg_alarm_bat_fed == 1) || (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat1_T_hight ==1))                             
    {
        bflg_bat_discharging = 0;
    }     
    else if (bflg_alarm_power_off == 1) //如果是断电状态
    {
        if (bflg_bat_type == 0)    //如果是铅酸电池
        {
            if (guc_bat_Vout <= ram_para2[num_bat1_discharge_off_V])   //10.5V
            {
                bflg_bat_discharging = 0;
            }
            else
            {                
                bflg_bat_discharging = 1;
            }
        }
        else   //如果是锂电池
        {
            if (guc_bat_Vout <= ram_para2[num_bat2_discharge_off_V])   //9.0V
            {
                bflg_bat_discharging = 0;
            }
            else
            {
                bflg_bat_discharging = 1;
            }
        }
    }
    else
    {        
        bflg_bat_discharging = 1;
    }
   //*/
}

//------------------------------------------------------------------------------
#endif
