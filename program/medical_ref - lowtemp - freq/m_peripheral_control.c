/*************************************************************************************************************
overview:20170406
    此文件为控制外围的负载； 1、风机    --CN29(FAN2)              P42
                             2、照明灯  --CN17(1.2脚)             P111
                             3、杀菌灯  --CN17(3.4脚)和CN29(FAN1) P146 和 P73 用2个
                             
*************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "h_type_define.h"
#include "h_ad.h"
#include "m_key_operation.h"
#include "m_peripheral_control.h"
#include "m_PTC_heat.h"
#include "h_protect.h"
#include "h_main.h"
#include "h_com.h"
//----------------------------------------------------------------------------------------------------------
//函数声明
void fan_control_deal(void);
void fan_RL_out(void);
void led_control_deal(void);
void led_key_on_delaytime(void);
void led_RL_out(void);
void germicidal_lamp_control_deal(void);
void germicidal_lamp_delaytime(void);
void germicidal_lamp_RL_out(void);
void remote_RL_alarm_lamp(void);



//宏定义
#define   DOOR_IN_PIN              P5.2        //门开关输入口  
#define   FAN_RL_OUT_PIN           P4.2        //风机输出引脚
#define   LED_RL_OUT_PIN           P11.1       //照明灯输出引脚
#define   GERMICIDAL_RL_OUT_PIN_1  P14.6       //杀菌灯输出引脚1
#define   GERMICIDAL_RL_OUT_PIN_2  P7.3        //杀菌灯输出引脚2

#define   DOOR_OPEN                1           
#define   DOOR_CLOSE               0  

//标志定义
flag_type bflg_peripheral_control; 

//变量
uint8_t guc_led_on_delaytime;                 //按键开灯延时
uint8_t guc_germicidal_lamp_on_delaytime;     //杀菌灯开延时


/*********************************************************************************************************************************************
函数功能: 风机控制

函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void fan_control_deal(void)
{
    if(DOOR_IN_PIN == DOOR_OPEN)
    {
        bflg_fan_on = 0;
    }
    else
    {
        bflg_fan_on = 1;
       /*
        if(bflg_PTC_heat_on == 1)//和PTC相关的去掉，只和开关门相关
        {
            bflg_fan_on = 1;
        }
        else
        {
            bflg_fan_on = 0;
        }
        */
    }
}

/*********************************************************************************************************************************************
函数功能: 风机控制输出引脚

函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void fan_RL_out(void)
{
    if(bflg_fan_on == 1)
    {
        FAN_RL_OUT_PIN = 1;
    }
    else
    {
        FAN_RL_OUT_PIN =0;
    }
}

/*********************************************************************************************************************************************
函数功能: 照明灯控制

函数位置: 主循环(读按键延时中)--------------------------ok
**********************************************************************************************************************************************/
void led_control_deal(void)
{
    if(gss_liquid_led_button == 1)
    {
        bflg_led_on = 1;
    }
    else
    {
        bflg_led_on = 0;
    }
        
   /* if(bflg_germicidal_lamp_on == 1)
    {
        bflg_led_on = 0;
    }
    else
    {
        if(DOOR_IN_PIN == DOOR_OPEN)   //照明灯暂时只随门开关动作，灯按键给杀菌灯用
        {
            bflg_led_on = 1;
        }
        else
        {
            bflg_led_on = 0;
        }
        */
        
        /*  //注释的这部分是正式的照明灯控制,先不用
        if(guc_key_val == KEY_LAMP)      //有灯按键按下
        {
            if(bflg_led_on == 1)
            {
                bflg_led_on = 0;
                bflg_led_on_delaytime = 0;
                guc_led_on_delaytime = 0;
            }
            else
            {
                bflg_led_on = 1;
                bflg_led_on_delaytime = 1;
                guc_led_on_delaytime = 0;
            }
        }
        else
        {
            if(DOOR_IN_PIN == DOOR_OPEN)
            {
                bflg_led_on = 1;
            }
            else
            {
                if(bflg_led_on_delaytime == 0)
                {
                    bflg_led_on = 0;
                }
            }
        }
    }*/
}

/*********************************************************************************************************************************************
函数功能: 照明灯按键开灯延时(若只是按键开没有按键关则延时后关闭)

函数位置: 1min定时器中-------------------------ok
**********************************************************************************************************************************************/
void led_key_on_delaytime(void)
{
    if(gss_user_set_parm_value[DISP_USER_SET_Lt] != -1)
    {
        if(bflg_led_on_delaytime == 1)
        {
            guc_led_on_delaytime++;
            if(guc_led_on_delaytime >= gss_user_set_parm_value[DISP_USER_SET_Lt])  //2min
            {
                guc_led_on_delaytime = 0;
                bflg_led_on_delaytime = 0;
            }
        }
    }
}

/*********************************************************************************************************************************************
函数功能: 照明灯输出引脚

函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void led_RL_out(void)
{
    if(bflg_led_on == 1)
    {
        LED_RL_OUT_PIN = 1;
    }
    else
    {
        LED_RL_OUT_PIN = 0;
    }
}

/*********************************************************************************************************************************************
函数功能: 杀菌灯控制

函数位置: 主循环(按键延时中)-------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_control_deal(void)
{
    if(gss_liquid_ozone_disinfect_button == 1)
    {
        bflg_germicidal_lamp_on = 1;
    }
    else
    {
        bflg_germicidal_lamp_on = 0;
    }
    /*
    if(bflg_led_on == 1)
    {
        bflg_germicidal_lamp_on = 0;
    }
    else
    {
        if(guc_key_val == KEY_LAMP)          //暂时用照明灯按键替代
        {
            if(bflg_germicidal_lamp_on == 1)
            {
                bflg_germicidal_lamp_on = 0;
                bflg_germicidal_lamp_on_delaytime = 0;
                guc_germicidal_lamp_on_delaytime = 0;
            }
            else
            {
                bflg_germicidal_lamp_on = 1;
                bflg_germicidal_lamp_on_delaytime = 1;
                guc_germicidal_lamp_on_delaytime = 0;
            }
        }*/
        /*
        else
        {
            if(DOOR_IN_PIN == DOOR_OPEN)
            {
                if(bflg_germicidal_lamp_on_delaytime == 0)
                {
                    bflg_germicidal_lamp_on = 0;
                }
            }
            else
            {   
                if(bflg_germicidal_lamp_on_delaytime == 0)
                {
                    bflg_germicidal_lamp_on = 1;
                    bflg_germicidal_lamp_on_delaytime = 1;
                    guc_germicidal_lamp_on_delaytime = 0;
                }
            }
        }
    }*/
}
/*********************************************************************************************************************************************
函数功能: 杀菌灯延时

函数位置: 1min定时器--------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_delaytime(void)
{
    if(gss_user_set_parm_value[DISP_USER_SET_S2] != -1)
    {
        if(bflg_germicidal_lamp_on_delaytime == 1)
        {
            guc_germicidal_lamp_on_delaytime++;
            if(guc_germicidal_lamp_on_delaytime >= gss_user_set_parm_value[DISP_USER_SET_S2])  //min
            {
                guc_germicidal_lamp_on_delaytime = 0;
                bflg_germicidal_lamp_on_delaytime = 0;
        
                bflg_germicidal_lamp_on = 0;
            }
        }
    }
}

/*********************************************************************************************************************************************
函数功能: 杀菌灯引脚输出

函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_RL_out(void)
{
    if(bflg_germicidal_lamp_on == 1)
    {
        GERMICIDAL_RL_OUT_PIN_1 = 1;
        GERMICIDAL_RL_OUT_PIN_2 = 1;
    }
    else
    {
        GERMICIDAL_RL_OUT_PIN_1 = 0;
        GERMICIDAL_RL_OUT_PIN_2 = 0;
    }
}

/**********************************************************************************************************************************************
函数功能: 远程报警灯

函数位置:  主循环----------------------------------------------ok
***********************************************************************************************************************************************/
void remote_RL_alarm_lamp(void)
{
    if(flg_alarm.uword != 0)
    {
        bflg_alarm_output = 1; //通讯传输状态
        RL_ALM_OUT_PIN = 1;    //开
    }
    else
    {
        bflg_alarm_output = 0;
        RL_ALM_OUT_PIN = 0;    //关
    }
}

/**********************************END OF THE FILE*************************************************/
