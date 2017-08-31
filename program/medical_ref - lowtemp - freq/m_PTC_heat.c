/**************************************************************************************************** 
overview:20170406
         此文件是对PTC加热模块的控制；1、PTC模块的加热控制 --CN29(HTR-CAP) (P41)
                                      
                                       
*****************************************************************************************************/
//------------------------------------------------------------------------------------------
//包含的文件
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"

#include "h_type_define.h"
#include "h_ad.h"
#include "m_PTC_heat.h"
#include "m_key_operation.h"
#include "h_main.h"
#include "h_protect.h"
#include "h_com.h"
//------------------------------------------------------------------------------------------
//函数声明
void PTC_heat_power_on_delaytime(void);
void PTC_heat_control_deal(void);
void PTC_heat_fre_mode_judge(void);
void PTC_heat_RL_out(void);
void PTC_fre_RL_deal(void);



//------------------------------------------------------------------------------------------
//macro
#define     PWM_PERIOD            33333    //周期设定为33333
#define     PWM_HIGH_LEVEL_CNT    0x208d   //pwm输出高电平计时    --30HZ的25%占空比
#define     PWM_LOW_LEVEL_CNT     0x61a8   //pwm输出低电平计时
#define     PWM_HIGH_LEVEL        1        //pwm为高电平时标志
#define     PWM_LOW_LEVEL         0        //pwm为低电平时标志
#define     PTC_OUT_ON            0        //开PTC -因为管子0是开
#define     PTC_OUT_OFF           1        //关PTC
#define     RL_PTC_OUT_PIN        P4.1     //加热模块引脚输出


//------------------------------------------------------------------------------------------
//标志定义
flag_type bflg_PTC_heat; 


//------------------------------------------------------------------------------------------
//变量定义 
uint8_t guc_PTC_heat_power_on_delaytime;   //PTC初始上电延时
uint8_t guc_PTC_heat_fault_on_delaytime;   //测试传感器故障时PTC开/停时间

/*********************************************************************************************************************************************
函数功能: 初始上电PTC开启延时

函数位置: 1min定时器中 --------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_power_on_delaytime(void)
{
    if(bflg_PTC_heat_power_on_delaytime == 1)
    {
        guc_PTC_heat_power_on_delaytime++;
        if(guc_PTC_heat_power_on_delaytime >= gss_factory_parm_value[DISP_FAC_Cd]) //min
        {
            guc_PTC_heat_power_on_delaytime = 0;
            bflg_PTC_heat_power_on_delaytime = 0; 
        }
    }
}

/*********************************************************************************************************************************************
函数功能: PTC加热模块的控制

函数位置: 1s定时器中 --------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_control_deal(void)
{
    if(bflg_PTC_heat_power_on_delaytime == 0)     //延时结束
    {
        if(bflg_prot2_door1_pin == 0)             //关门
        { 
            if(bflg_TTEST_fault == 1)             //故障
            {
                if(bflg_PTC_heat_on == 1)
                {
                    guc_PTC_heat_fault_on_delaytime++;
                    if(guc_PTC_heat_fault_on_delaytime >= gss_factory_parm_value[DISP_FAC_T1]*60) // min * 60 =s
                    {
                        guc_PTC_heat_fault_on_delaytime = 0;
                        bflg_PTC_heat_on = 0;
                    }
                }
                else
                {
                    guc_PTC_heat_fault_on_delaytime++;
                    if(guc_PTC_heat_fault_on_delaytime >= gss_factory_parm_value[DISP_FAC_T2]*60) 
                    {
                        guc_PTC_heat_fault_on_delaytime = 0;
                        bflg_PTC_heat_on = 1;
                    }
                }
            }
            else
            {
                if(bflg_PTC_heat_on == 1)
                {
                    if(gss_TTEST >= gss_user_set_parm_value[DISP_USER_SET_TS] + gss_factory_parm_value[DISP_FAC_r0])
                    {
                        bflg_PTC_heat_on = 0;
                    }
                }
                else
                {
                    if(gss_TTEST <= gss_user_set_parm_value[DISP_USER_SET_TS] - gss_factory_parm_value[DISP_FAC_r0])
                    {
                        bflg_PTC_heat_on = 1;
                    } 
                }
            }
        }
        else 
        {
            bflg_PTC_heat_on = 0;
        }
    }
}

/*********************************************************************************************************************************************
函数功能:判断是否按频率pwm方式控制

       CN5第一脚就是P82，原来三色灯的红灯
       要求：33Hz 25%占空比      
       T= 1/30s = 33333us                     
       高电平占的时间：25% * T 
       低电平占的时间：33333 - 25% * T 
       
函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_fre_mode_judge(void)
{
    if(bflg_PTC_heat_on == 1)
    {        
        if(gss_user_set_parm_value[DISP_USER_SET_TS] - gss_TTEST >= gss_factory_parm_value[DISP_FAC_r1])
        {
            bflg_PTC_fre_control = 0;   //不需频率控制

            //debug + 调试频率输出
            //bflg_PTC_fre_control = 1;
        }
        else
        {
            bflg_PTC_fre_control = 1;   //需频率控制
        }
    }
    else
    {        
        bflg_PTC_fre_control = 0;       //不需频率控制
    }
}

/*********************************************************************************************************************************************
函数功能: PTC的实际输出：1、一直输出 2、频率输出 3、不输出
       
函数位置: 主循环--------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_RL_out(void)
{    
    if(bflg_PTC_heat_on == 1)             //开机
    {  
        RL_PTC_OUT_PIN = 1;               //CN29这个端口只其供电作用，刘晓阳现在测试使用
        //-------------------------------
        if(bflg_PTC_fre_control == 0)     
        {  
            if(bflg_PTC_alway_out == 0)
            {
                bflg_PTC_alway_out = 1;   //一直输出
                bflg_PTC_fre_out = 0;
                bflg_PTC_no_out = 0;
                
                RED_LED_PIN = PTC_OUT_ON;  
                R_TAU0_Channel6_Stop();  
            }
        }
        else
        {
            if(bflg_PTC_fre_out == 0)
            {
                bflg_PTC_alway_out = 0;   
                bflg_PTC_fre_out = 1;     //频率输出
                bflg_PTC_no_out = 0;
                R_TAU0_Channel6_Start();  
            }
        }
    }
    else
    {
        
        RL_PTC_OUT_PIN = 0;
        //-------------------------
        if(bflg_PTC_no_out == 0)
        {
            bflg_PTC_alway_out = 0;   
            bflg_PTC_fre_out = 0;     
            bflg_PTC_no_out = 1;         //不输出
            
            RED_LED_PIN = PTC_OUT_OFF;     
            R_TAU0_Channel6_Stop();  
        } 
    }
}

/*********************************************************************************************************************************************
函数功能: PTC模块的频率控制时的处理
          要求：输出30HZ,占空比为25%----->所以周期为33333us(定时器6底层配置为33333us)
                                          25%*33333 = 8333 = 0x208d
                                          33333 - 8333 = 0x61a8

          处理方式：每一次进中断时翻转pwm电平，也重新装载下次进中断的时间(即高低电平的时间)
       
函数位置: 定时器6系统中断--------------------------ok
**********************************************************************************************************************************************/
void PTC_fre_RL_deal(void)
{
    if(bflg_pwm_level == PWM_HIGH_LEVEL)                  //进中断pwm为高时，则要变为低电平
    {
        bflg_pwm_level = PWM_LOW_LEVEL;
        
        //TDR06 = PWM_LOW_LEVEL_CNT;                       //重新设定定时中断周期
        TDR06 = (uint16_t)(33333 - ((PWM_PERIOD * gss_pwm_adjust)/100)); //调试占空比+，后期定了删掉
        TS0 |= _0040_TAU_CH6_START_TRG_ON;                 //重启定时器，可将TCR重新装载TDR的值并能重新计数

        RED_LED_PIN = PTC_OUT_OFF;                         //关 
    }
    else
    {
        bflg_pwm_level = PWM_HIGH_LEVEL;
        
        //TDR06 = PWM_HIGH_LEVEL_CNT;  
        TDR06 =  (uint16_t)((PWM_PERIOD * gss_pwm_adjust)/100);
        TS0 |= _0040_TAU_CH6_START_TRG_ON;  

        RED_LED_PIN = PTC_OUT_ON;             //开     
    } 
}
/**********************************END OF THE FILE*************************************************/
