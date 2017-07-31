#ifndef _BUZZ_C_
#define _BUZZ_C_


//-----------------------------------------------------------------------------------
//包含的文件声明
#include "h_type_define.h"
#include "r_cg_timer.h"
#include "h_protect.h"

#include "M_buzz.h"
#include "h_main.h"
//----------------------------------------------------------------------------------
//函数声明
void buzz_loop_tick(void);                //短滴循环
void buzz_loop_tick_delaytime(void);  //短滴响的时间
void buzz_one_tick(void);                 //长滴一声
void buzz_one_tick_delaytime(void);   //长滴响的时间
void buzz_always_sound(void);            //长响
void buzz_sound_priority(void);         //短滴循环、长滴一声、长响的优先级
void buzz_sound_cnt(void);



//---------------------------------------------------------------------------------
//变量定义
flag_type flg_buzz;

//------------------------------------------------------------------------------
int16_t   gss_buzz_loop_tick_delaytime;  
int16_t   gss_buzz_one_tick_delaytime;

uint8_t   guc_buzz_sound_cnt;            //自定义蜂鸣器响多少声，就赋值多少给他
uint8_t   guc_buzz_sound_cnt_delaytime;

/********************************
 测试用
********************************/


//---------------------------------------------------------------------------------
//具体函数

/*********************************************************************************
function: 蜂鸣器滴一声

之前问题及解决方案:         
         短滴:  在 1s 中调用   
       
         响多长时间不能控制，只是在几秒的定时中调用；
         
         solution:  bflg_alarm_buzz_output  的翻转在计时函数中调用
         
 功能描述: 控制蜂鸣器的开/关；
 
 输入接口: bflg_alarm_buzz_output   bflg_alarm_buzz_off_delaytime

 执行条件: 有报警、未按"蜂鸣取消"
 处理内容: 蜂鸣器的开关；

 输出接口: NO

 
 函数位置: 主函数中调用
*********************************************************************************/
void buzz_loop_tick(void)
{
    if (bflg_buzz_loop_tick_output == 1)
    {
        bflg_buzz_loop_tick_output = 0;    

        //if (bflg_alarm_buzz_off_delaytime == 0)   //未按"蜂鸣取消" 键，则响；
        //{
            R_TAU0_Channel3_Start();                //响的条件: 1、有报警 2、未按下"蜂鸣取消"键 
        //}		    
    }
    else
    {
        R_TAU0_Channel3_Stop();
    }	
}

/*********************************************************************************
功能描述: 蜂鸣器滴的短滴 声音长度的控制；现在设定响 1s;

输入接口: bflg_buzz_short_tick_set    检测到有短滴的标志置1，(有相应的报警时可置1)

执行条件: bflg_buzz_short_tick_set == 1 ;
处理内容: bflg_buzz_short_tick_output      进行翻转；或是置0


输出接口: bflg_buzz_short_tick_output  的值；


函数位置: 100ms 中调用;   

校正:   >= 1  ---->   > 1
*********************************************************************************/
void buzz_loop_tick_delaytime(void)
{
    if (bflg_buzz_loop_tick_set == 1)    
    {
        gss_buzz_loop_tick_delaytime++;
        if (gss_buzz_loop_tick_delaytime > 1)     //设定时间 100ms * 1 = 100 ms
        {
            //bflg_buzz_loop_tick_output = ~bflg_buzz_loop_tick_output;

            bflg_buzz_loop_tick_output = 1;
            gss_buzz_loop_tick_delaytime = 0;
        }
    }
    else
    {
        bflg_buzz_loop_tick_output = 0;
        gss_buzz_loop_tick_delaytime = 0;
    }
}


/*********************************************************************************
功能描述: 控制蜂鸣器的开/关；

输入接口: bflg_buzz_long_tick_output    

执行条件: 长按"蜂鸣取消" 5s
处理内容: 蜂鸣器的开关；

输出接口: NO


函数位置: 主函数中调用         
*********************************************************************************/
void buzz_one_tick(void)
{
    if (bflg_buzz_one_tick_set == 1)
    {
        if (bflg_buzz_one_tick_run == 0) 
        {
            bflg_buzz_one_tick_run = 1;
            R_TAU0_Channel3_Start();
        }
    }
}


/*********************************************************************************
功能描述: 蜂鸣器滴的长滴 声音长度的控制；现在设定响 3s;

输入接口: bflg_buzz_long_tick_set    检测到有长滴的标志置1，(报警时设置)

执行条件: bflg_buzz_long_tick_set == 1 ;
处理内容: bflg_buzz_long_tick_output 进行翻转；或是置0


输出接口: bflg_buzz_long_tick_output  的值；


函数位置: 100ms 中调用;       
*********************************************************************************/
void buzz_one_tick_delaytime(void)
{
    if (bflg_buzz_one_tick_set == 1)	  
    {
        gss_buzz_one_tick_delaytime++;
        if (gss_buzz_one_tick_delaytime >= 5)   // 100 * 30 = 3000ms = 3s
        {
            R_TAU0_Channel3_Stop();

            bflg_buzz_one_tick_run = 0；
            bflg_buzz_one_tick_set = 0;
            gss_buzz_one_tick_delaytime = 0;
        }
    }
}

/*********************************************************************************
function: 蜂鸣器长响

输入接口: 检测到有长响标志；



输出接口:NO

函数位置: 100ms 中调用
*********************************************************************************/
void buzz_always_sound(void)
{
    if (bflg_buzz_always_sound_set == 1)
    {      
        if (bfla_buzz_always_sound_run == 0)
        {
            R_TAU0_Channel3_Start();
            bfla_buzz_always_sound_run = 1;
        }
    }
    else
    {   
        bfla_buzz_always_sound_run = 0;
        R_TAU0_Channel3_Stop();
    }
}

/*********************************************************************************
功能描述:蜂鸣器长响、长滴、短滴的优先级顺序 
         在长响时，不能再有关闭蜂鸣器的行为，即不能再有滴滴声，否则声音会卡；

接口输入: 

处理内容: 判断此时要执行哪一个响的模式；

接口输出: NO

函数位置: 100ms 中调用
*********************************************************************************/
void buzz_sound_priority(void)
{
    if (bflg_test_mode == 0)                      //不在测试模式下才处理，在测试模式下不响
    {
        buzz_always_sound();                      // priority_1
        
        if (bflg_buzz_always_sound_set == 0)      //没有长响时
        {   
            buzz_one_tick();                      // priority_2
                                  
            if (bflg_buzz_one_tick_set == 0)      //没有长滴时
            {   //debug 包工注释
                //buzz_loop_tick();               // priority_3  因调试暂时注释
            }   
        } 
    }
}

/*********************************************************************************
功能描述:factory 指令的AT测试模式中的蜂鸣器响；


函数位置: 100ms定时器中----------ok

*********************************************************************************/
 void buzz_sound_cnt(void)
{    
    if(guc_buzz_sound_cnt > 0)
    {
        guc_buzz_sound_cnt_delaytime++;
        if(guc_buzz_sound_cnt_delaytime >= 10)  //1s
        {
            guc_buzz_sound_cnt--;
            guc_buzz_sound_cnt_delaytime = 0;
            
            bflg_buzz_one_tick_set = 1;         //响一次
        }
    }   
}


#endif
/***************************************ENG OF THE FILE*********************************************/
