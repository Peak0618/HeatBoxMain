#ifndef _KEY_DISP_C_
#define _KEY_DISP_C_
//------------------------------------------------------------------------------
//包含文件声明
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"

#include "h_type_define.h"
#include "h_key_disp.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_protect.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "r_cg_timer.h"
#include "h_com.h"
#include "h_main.h"
#include "m_key_operation.h"
#include "m_test.h"
//------------------------------------------------------------------------------
//函数声明
void key_IC_register_deal(void); //在注册状态下，再按其中的一些按键后的状态转换

void led_disp1_deal(void);       //数码管1显示处理程序，在100ms定时程序中调用
void led_lamp_disp(void);

void led_disp2_deal(void);       //数码管2显示处理程序，在100ms定时程序中调用

void led_disp3_deal(void);       //数码管3显示处理程序，在100ms定时程序中调用

void led1_disp_val(int16_t lss_disp_val);    //数码管1显示数值程序

void led2_disp_val(int16_t lss_disp_val);    //数码管2显示数值程序

void led3_disp_val(int16_t lss_disp_val);    //数码管3显示数值程序

void lamp_disp_deal(void);                   //灯显示处理程序，在100ms定时程序中调用



void read_key_delaytime(void);              //读键延时程序，在10ms定时程序中调用

//peak add
void  key_buzz_cancel(void);
void bat_con_check_again(void);              //长按取消报警后，当再次检测到电池，恢复正常控制；  extern 
void view_error_code(void);                    //显示错误代码
void view_error_code_delaytime(void);     //显示错误代码时间 
void led_loop_delaytime(void);               //led报警闪烁
void led_disp1_flicker_delaytime(void);  //设定时数据闪烁
void key_IC_password_deal(void);
void key_factory_AT_test(void);
void key_factory_init(void);                   //恢复出厂值
void system_self_test(void);                   //初始上电时的自检

void card_IC_deal(void);                         //锁的控制 
void lock_open_delaytimer(void);                //锁打开的时间控制

//------------------------------------------------------------------------------
//变量定义
flag_type flg_disp,flg_disp1,flg_error_code_mark,flg_save1,flg_save2;

//------------------------------------------------------------------------------
//led显示编码宏定义
//---------------------
//|        a          |
//|       ----        |
//|    f | g  | b     |
//|       ----        |
//|    e |    | c     |
//|       ---- .      |
//|        d    dp    |
//---------------------
//----------------------------------------------------------
//    dp   |  g   |  f   |  e   |  d   |  c   |  b   |  a
//   bit7  | bit6 | bit5 | bit4 | bit3 | bit2 | bit1 | bit0
//----------------------------------------------------------
#define   data_all_off   0x00    //都不亮
#define   data_dot       0x80    //点
#define   data_hyphen    0x40    //负号

#define   data_0         0x3F    //数字0
#define   data_1         0x06
#define   data_2         0x5B
#define   data_3         0x4F
#define   data_4         0x66
#define   data_5         0x6D
#define   data_6         0x7D
#define   data_7         0x07
#define   data_8         0x7F
#define   data_9         0x6F

#define   data_A         0x77
#define   data_b         0x7C
#define   data_C         0x39
#define   data_c         0x58
#define   data_d         0x5E
#define   data_E         0x79
#define   data_F         0x71
#define   data_G         0x3D
#define   data_H         0x76
#define   data_I         0x06    //peak 新加的大写I,和数字1的值一样
#define   data_i         0x04
#define   data_J         0x0E
#define   data_L         0x38
#define   data_N         0x37
#define   data_n         0x54
#define   data_o         0x5C
#define   data_P         0x73
#define   data_q         0x67
#define   data_r         0x50
#define   data_S         0x6D
#define   data_T         0x31
#define   data_t         0x78
#define   data_U         0x3E
#define   data_u         0x1C
#define   data_y         0x6E
//peak add
#define  data_E_dp       0XF9    // 显示 E 和 点；
#define  data_line       0X40    // 只显示 g 代表的那条横线 



//----------------------------------------------------------
//显示数组宏定义
uint8_t   guc_led_buffer[12]; //数码管寄存器  //peak LDE1~LED12 对应 [0] ~ [11]
//----------------------------------------------------------
const uint8_t guc_nmb_table[]=
{
  data_0  // 0
, data_1  // 1
, data_2  // 2
, data_3  // 3
, data_4  // 4
, data_5  // 5
, data_6  // 6
, data_7  // 7
, data_8  // 8
, data_9  // 9
};


//-----------------------------------------------------------------------------
uint8_t   guc_disp_rate;           //显示比率寄存器
int16_t   gss_disp_see_buffer[6];  //显示查询信息寄存器
int16_t   gss_read_key_delaytimer; //读键值延时计时器



int16_t    gss_user2_CL1_flicker_cnt;      //注销IC卡时CL1闪烁的次数
int16_t    gss_factory_AT_test_delaytime;  //测试AT指令时全亮的时间
int16_t    gss_factory_CL3_flicker_cnt;    //初始IC密码时CL3闪烁的次数
int16_t    gss_factory_CL4_flicker_cnt;    //恢复出厂设置CL4闪烁的次数


//peak add
int16_t   gss_error_code_delaytime;      //错误代码的显示延时时间
int16_t   gss_led_loop_delaytime;        //LED的闪烁频率设定
int16_t   guc_IC_register_delaytime;     //判断长按组合键的的时间是否达到进入注册状态
int16_t   gsc_IC_password_val;           //IC注册模式的密码值
int8_t    guc_IC_erasure_delaytime;      //IC注销时间判断
int8_t    gss_disp1_flicker_delaytime;   //"箱内温度设定" "高温报警设定" "低温报警设定" 闪烁延时
int8_t    guc_IC_password_cnt;           //进入IC注册模式时输入错误密码次数计数
int8_t    guc_password_cnt;              //进入工厂模式时输入错误密码次数计数

uint8_t   gss_self_test_delaytime;       //系统自检时的闪烁延时
uint8_t   gss_self_test_cnt;             //系统自检时的闪烁次数计数
uint8_t   guc_lock_delaytimer;           // IC卡开锁的打开的时间




/**********************************************************************************************************************************************
函数功能: 数码管显示内容
         LED1 bit0:上温      LED1 bit1:下温         LED1 bit2:220v电源       LED1 bit3:报警 
         LED1 bit4:开门      LED1 bit5:冷凝器脏       LED1 bit6:网络            LED1 bit7:NFC

         LED2~LED4：对应3位数码管

函数位置:  10ms定时中---------------------------------ok
***********************************************************************************************************************************************/
void led_disp1_deal(void)     
{        
    if (bflg_test_mode == 1)               //如果是测试模式
    {
		R_TAU0_Channel3_Stop();
        if(bflg_test_mode_RTC == 0)        //未按下buzz键
        {
	        if (bflg_test_ok == 0)         //如果未测试完成
	        {
	            guc_disp_rate = dot_0d001;
                LED1 = data_1;
                LED2 = data_0;
                LED3 = data_0;
                LED4 = data_0;

	            //led1_disp_val(SOFT_VERSION);
	        }
	        else                            //如果测试完成
	        {
	            if (guc_test_mode_code == 0)    
	            {
	                LED1 = data_P;
	                LED2 = data_A;
	                LED3 = data_S;
	                LED4 = data_S;
	            }
	            else                            //如果有测试故障
	            {
	                LED1 = data_all_off;
	                LED2 = data_E;
	                LED3 = guc_nmb_table[guc_test_mode_code / 10];
	                LED4 = guc_nmb_table[guc_test_mode_code % 10];  
	            }
	        }
        }
        else
        {
			LED1 = data_2;
			LED2 = data_0;
			LED3 = data_1;
			LED4 = data_7;
			//LED3 = guc_nmb_table[guc_RTC_r_year / 10];   //待+
			//LED4 = guc_nmb_table[guc_RTC_r_year % 10]; 
        }

        //数码管2-567
        led2_disp_val(gss_TA / 10);
        //数码管3 -91011
        led3_disp_val(gss_VAC);   
    }
    else if (guc_disp_mode == DISP_LOCKED)            //如果是锁定模式
    {
        if ((flag_view_error_code == 0))
        {
            if(flg_alarm.uword != 0) 
            {
	            if (guc_key_val == KEY_BUZ)                       				      
	            {
                    flag_view_error_code = 1;				
	            }
            }
            //-------------------------------------------------------
            guc_disp_rate = dot_0d1;
            
            if(guc_TUTD_dis_type == 0)               //显示和测试传感器的均值
            {
                led1_disp_val((gss_TTEST+gss_TDIS) >> 1); 
            }
            else if( guc_TUTD_dis_type == 1)         //测试传感器温度
            {
                led1_disp_val(gss_TTEST);                      
            }
            else if(guc_TUTD_dis_type == 2)          //显示传感器温度
            {
                led1_disp_val(gss_TDIS);          
            }
        }
        else
        {
		    view_error_code();      
        }
    }
    else if(guc_disp_mode == DISP_USER_SET)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_USER_SET_TS)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_S;  
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
            {
                LED2 = data_A;
                LED3 = data_L;
                LED4 = data_H; 
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
            {
                LED2 = data_A;
                LED3 = data_L;
                LED4 = data_L; 
            }
            else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
            {
                LED2 = data_all_off;
                LED3 = data_L;
                LED4 = data_t; 
            }   
            else if(gsc_disp_parm_type == DISP_USER_SET_dA)
            {
                LED2 = data_all_off;
                LED3 = data_d;
                LED4 = data_A; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_SET_S1)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_1; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_SET_S2)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_2; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_SET_TT)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_T; 
            } 
        }
        else
        {
            if(bflg_disp1_flicker == 1)         
            {
                if(bflg_parm_save == 1)                  
                {
                    if((gsc_disp_parm_type == DISP_USER_SET_TS) || 
                       (gsc_disp_parm_type == DISP_USER_SET_ALH)||
                       (gsc_disp_parm_type == DISP_USER_SET_ALL))
                    {
                        guc_disp_rate = dot_0d1;
                        led1_disp_val(gss_key_temp_value);
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_key_temp_value);
                    }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_USER_SET_TS) || 
                       (gsc_disp_parm_type == DISP_USER_SET_ALH)||
                       (gsc_disp_parm_type == DISP_USER_SET_ALL))
                    {
                        guc_disp_rate = dot_0d1;
                        led1_disp_val(gss_user_set_parm_value[gsc_disp_parm_type]);
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_user_set_parm_value[gsc_disp_parm_type]);
                    }
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }  
    }
    else if(guc_disp_mode == DISP_USB_TIME_SET)
    {
        
    }
    else if(guc_disp_mode == DISP_USER_MANAGE_PASSWORD)
    {
        led1_disp_val(gsc_user_manage_password_val);
    }
    else if(guc_disp_mode == DISP_USER_MANAGE)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
            {
                LED2 = data_all_off;
                LED3 = data_I;
                LED4 = data_C;  
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
            {
                LED2 = data_C;
                LED3 = data_L;
                LED4 = data_1; 
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
            {
                LED2 = data_all_off;
                LED3 = data_L;
                LED4 = data_t; 
            }
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
            {
                LED2 = data_all_off;
                LED3 = data_d;
                LED4 = data_A; 
            }   
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
            {
                LED2 = data_all_off;
                LED3 = data_F;
                LED4 = data_A; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_1; 
            }  
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
            {
                LED2 = data_all_off;
                LED3 = data_S;
                LED4 = data_2; 
            } 
            else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_T; 
            }  
        }
        else
        {
            if(bflg_disp1_flicker == 1)         //闪烁
            {
                if(bflg_parm_save == 1)                  
                {
                    guc_disp_rate = not_dot;
                    led1_disp_val(gss_key_temp_value);
                }
                else
                {
                    guc_disp_rate = not_dot;
                    led1_disp_val(gss_user_manage_parm_value[gsc_disp_parm_type]);
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }  
    }
    else if(guc_disp_mode == DISP_FACTORY_PASSWORD)
    {
        led1_disp_val(gsc_factory_password_val);      
    }
    else if(guc_disp_mode == DISP_FACTORY)
    {
        if(bflg_parm_dis_form == 0)
        {
            if(gsc_disp_parm_type == DISP_FAC_r0)
            {
                LED2 = data_all_off;
                LED3 = data_r;
                LED4 = data_0;  
            }
            if(gsc_disp_parm_type == DISP_FAC_r1)
            {
                LED2 = data_all_off;
                LED3 = data_r;
                LED4 = data_1;  
            }
            else if(gsc_disp_parm_type == DISP_FAC_Ct1)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_1; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_Ct2)
            {
                LED2 = data_C;
                LED3 = data_t;
                LED4 = data_2; 
            }
            else if(gsc_disp_parm_type == DISP_FAC_Cd)
            {
                LED2 = data_all_off;
                LED3 = data_C;
                LED4 = data_d; 
            }   
            else if(gsc_disp_parm_type == DISP_FAC_T1)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_1; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_T2)
            {
                LED2 = data_all_off;
                LED3 = data_T;
                LED4 = data_2; 
            }  
            else if(gsc_disp_parm_type == DISP_FAC_Fd)
            {
                LED2 = data_all_off;
                LED3 = data_F;
                LED4 = data_d; 
            }  
        }
        else
        {
            if(bflg_disp1_flicker == 1)         
            {
                if(bflg_parm_save == 1)                  
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0) || 
                       (gsc_disp_parm_type == DISP_FAC_r1) ||
                       (gsc_disp_parm_type == DISP_FAC_Ct1)||
                       (gsc_disp_parm_type == DISP_FAC_Ct2))
                    {
                        guc_disp_rate = dot_0d1;
                        led1_disp_val(gss_key_temp_value);
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Fd) //10min,20min,30min需级别*10
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_key_temp_value*10); 
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_key_temp_value);
                    }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0) || 
                       (gsc_disp_parm_type == DISP_FAC_r1) ||
                       (gsc_disp_parm_type == DISP_FAC_Ct1)||
                       (gsc_disp_parm_type == DISP_FAC_Ct2))
                    {
                        guc_disp_rate = dot_0d1;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]);
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Fd) //10min,20min,30min需级别*10
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]*10); 
                    }
                    else
                    {
                        guc_disp_rate = not_dot;
                        led1_disp_val(gss_factory_parm_value[gsc_disp_parm_type]);
                    }
                }
            }
            else
            {
                LED2 = data_all_off;
                LED3 = data_all_off;
                LED4 = data_all_off;
            }
        }  
    }
    else if(guc_disp_mode == DISP_INQUIRE)
    {
        if(bflg_key_inq_val == 0) //显示d1-d9
        {                
            LED2 = data_all_off;
            LED3 = data_d;
            LED4 = guc_nmb_table[gsc_disp_inq_mode+1];  
        }
        else                     //显示值
        {
            if(gsc_disp_inq_mode == DISP_INQ_d1)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TU);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d2)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TD);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d3)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TA);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d4)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TE);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d5)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_TC);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d6)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(gss_THW);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d7)
            {
                //湿度
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d8)
            {
                led1_disp_val(gss_VAC);          
            }
            else if(gsc_disp_inq_mode == DISP_INQ_d9)
            {
                guc_disp_rate = dot_0d1;
                led1_disp_val(guc_bat_Vout);          
            }
        }
    }
    else if(guc_disp_mode == DISP_IC_PASSWORD)
    {
        
    }
    else if(guc_disp_mode == DISP_IC_REGISTER)
    {
        
    }
}

/******************************************************************************************************************************************
函数功能：显示板上的led灯的显示

            LED1 bit0:上温      LED1 bit1:下温         LED1 bit2:电源       LED1 bit3:报警 
            LED1 bit4:开门      LED1 bit5:冷凝器脏       LED1 bit6:网络       LED1 bit7:NFC

            LED2~LED4：对应3位数码管

函数位置：10ms----------------------------------------未调
*******************************************************************************************************************************************/
void led_lamp_disp(void)
{
    if(bflg_test_mode == 0)
    {
         //---------------------------------------------上下温指示灯led1、led2
         if(guc_TUTD_dis_type == 0)       //均值
         {
             LED1 |= 0x03;
         }
         else if(guc_TUTD_dis_type == 1)   //上灯---测试温度
         {
             LED1 |= 0x01;                        
             LED1 &= ~0x02;
         }
         else if(guc_TUTD_dis_type == 2)   //下灯---显示传感器温度
         {
             LED1 &= ~0x01;                       
             LED1 |= 0x02;
         }
         //---------------------------------------------  电源指示灯 led3
         if(bflg_alarm_power_off == 1)
         {
             if(bflg_disp1_flicker == 1)
             {
                 LED1 |= 0X04;
             }
             else
             {
                 LED1 &= ~0x04; 
             }
         }
         else
         {
             LED1 |= 0X04;  //后加待测
         }
         //-----------------------------------------------报警指示灯led4
         if( flg_alarm.uword != 1)
         {
             if(bflg_disp1_flicker == 1)
             {
                 LED1 |= 0x08;
             }
             else
             {
                 LED1 &= ~0x08; 
             }
         }
         else
         {
             LED1 &= ~0x08; 
         }
         //-----------------------------------------------开门指示灯led5
         if(bflg_alarm_open_door == 1)
         {
             if(bflg_disp1_flicker == 1)
             {
                 LED1 |= 0X10;
             }
             else
             {
                 LED1 &= ~0x10; 
             }
         }
         else
         {
             LED1 &= ~0x10; 
         }
         //-----------------------------------------------冷凝器脏指示灯led6
        /* if(bflg_alarm_cond_dirty == 1)
         {
             if(bflg_disp1_flicker == 1)
             {
                 LED1 |= 0X20;
             }
             else
             {
                 LED1 &= ~0x20; 
             }
         }
         else
         {
             LED1 &= ~0x20; 
         }*/
         //-----------------------------------------------网络指示灯led7
         //待+
        
         //-----------------------------------------------NFC指示灯led8
         //待+
    }
}

//------------------------------------------------------------------------------
/*******************************************************************************
function: 中
          中间的LED要显示的内容
          
*******************************************************************************/
void led_disp2_deal(void)     //数码管2显示处理程序，在100ms定时程序中调用
{
    if (bflg_test_mode == 1)         //如果是测试模式
    {
        guc_disp_rate = not_dot;
        led2_disp_val(gss_TA / 10);
    }
    else
    {
        guc_disp_rate = not_dot;
        //debug
        //led2_disp_val(gss_VAC);              //peak 选择电压显示
        //led2_disp_val(guc_bat_Vout);
        //led2_disp_val(gss_adg_Temp[0]);
        //led2_disp_val(gss_comp2_freq / 10);    // 高温压机频率
    }      
}

//------------------------------------------------------------------------------
/*******************************************************************************
function: 右
-------------------显示依据-----------------------
 判断条件： 环温＞35℃（ EEP） 持续 30 秒（ EEP）；
 处理措施： 报警灯闪烁；
 恢复条件： 环温≤34℃（ EEP） 持续 30 秒（ EEP）；
		  （此时如果显示环温＜35℃， 则显示 35℃）
---------------------------------------------------

 bflg_prot2_THW_high_disp 在protect中选择 0/1 (现在的环温/设定的)
 
*******************************************************************************/
void led_disp3_deal(void)        //数码管3显示处理程序，在100ms定时程序中调用
{
    guc_disp_rate = not_dot;
    
    if (bflg_test_mode == 1)         //如果是测试模式
    {
        led3_disp_val(gss_VAC);   
    }
    //debug
    /*
    if (bflg_prot2_THW_high_disp == 0)   //peak 选择环温显示
    {
    	  //led3_disp_val(gss_THW);      //环温

    	  //peak add    	  
    	  //led3_disp_val(gss_TC);       //冷凝器
    	  //led3_disp_val(gss_TE);       //热交换器
    	  led3_disp_val(gss_Tbat1);      //电池温度
    }
    else
    {
    	  led3_disp_val(ram_para[num_prot2_THW_T]);  //peak 35°
    }
    */
    //led3_disp_val(guc_bat_Vin);
    //led3_disp_val(gss_bat_I);
    //led3_disp_val(gss_adg_Temp[1]);
    //led3_disp_val(gss_bat_pwm_value);
    //led3_disp_val(gss_adg_Temp2);
    //led3_disp_val(gss_THW);      //环温
    //led3_disp_val(gss_comp21_freq / 10);    //低压机转速
}
//------------------------------------------------------------------------------
/*******************************************************************************
 左面的数码管是4位数码管:
 四位数码管的1~4 位的分别显示:
*******************************************************************************/
void led1_disp_val(int16_t lss_disp_val)     //数码管1显示数值程序
{
    uint16_t lus_disp_abs_val;
   
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //----------------------------------------------------------------
    if ((lus_disp_abs_val / 1000) != 0)   //取第四位的值
    {
        lus_disp_abs_val /= 10;           //当是4位数时，会缩小10倍，这些参数需在功能书上标注
    }                                     //当数值高于4位时，不能正确处理

    //----------------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED2 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED2 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED3 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED3 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED3 = data_0 | data_dot;
        }
        else
        {
            if (LED2 == data_all_off)
            {
                LED3 = data_all_off;
            }
            else
            {
                LED3 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED4 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED3 == data_all_off)
            {
                LED3 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED4 = data_0;
    }
}

/*{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;      //显示负数标志   
        lus_disp_abs_val = (~lss_disp_val) + 1;    // peak 将负数的补码转换为我们想要的不带负号的原码
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------ LED1
    if ((lus_disp_abs_val / 1000) != 0)                  //peak 取商，取第四位的值
    {
        LED1 = guc_nmb_table[lus_disp_abs_val / 1000];   //peak 有值就显示
    }
    else
    {
        LED1 = data_all_off;                             //peak 没值就不亮
    }
    //------------------------------------------------------ LED2   
    lus_disp_abs_val %= 1000;                            // peak 取余，剩下的数值
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED2 = guc_nmb_table[lus_disp_abs_val / 100];
        //------------------------------
        if (guc_disp_rate == dot_0d01)                   //peak 判断LED2此位小数点是否开
        {
            LED2 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)                        //peak 判断前一位是否有负号
        {
            if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d01)                  //peak 判断为0时是否开小数点
        {
            LED2 = data_0 | data_dot;
        }
        else
        {
            if (LED1 == data_all_off)                   //peak 前一位没有值，此位为0没意义，也关掉
            {
                LED2 = data_all_off;
            }
            else
            {
                LED2 = data_0;
            }
        }
    }
    //------------------------------------------------------ LED3
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED3 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED3 |= data_dot;               //peak LED3的小数点也显示上
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED3 = data_0 | data_dot;
        }
        else
        {
            if (LED2 == data_all_off)
            {
                LED3 = data_all_off;
            }
            else
            {
                LED3 = data_0;
            }
        }
    }
    //------------------------------------------------------ LED4
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED4 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED3 == data_all_off)
            {
                LED3 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED2 == data_all_off)
            {
                LED2 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED1 == data_all_off)
            {
                LED1 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED4 = data_0;
    }
}*/
//------------------------------------------------------------------------------
/*******************************************************************************
 中间这个数码管为3位数码管
 
*******************************************************************************/
void led2_disp_val(int16_t lss_disp_val) //数码管2显示数值程序
{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED5 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED5 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED6 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED6 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED5 == data_all_off)
            {
                LED5 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED6 = data_0 | data_dot;
        }
        else
        {
            if (LED5 == data_all_off)
            {
                LED6 = data_all_off;
            }
            else
            {
                LED6 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED7 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED6 == data_all_off)
            {
                LED6 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED5 == data_all_off)
            {
                LED5 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED7 = data_0;
    }
}
//------------------------------------------------------------------------------
 /*******************************************************************************
 右面这个数码管是3位数码管  
*******************************************************************************/
void led3_disp_val(int16_t lss_disp_val) //数码管3显示数值程序
{
    uint16_t lus_disp_abs_val;
    
    if (lss_disp_val < 0)
    {
        bflg_disp_minus = 1;
        lus_disp_abs_val = (~lss_disp_val) + 1;
    }
    else
    {
        bflg_disp_minus = 0;
        lus_disp_abs_val = lss_disp_val;
    }
    //------------------------------------------------------
    if ((lus_disp_abs_val / 100) != 0)
    {
        LED9 = guc_nmb_table[lus_disp_abs_val / 100];
    }
    else
    {
        LED9 = data_all_off;
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 100;
    if ((lus_disp_abs_val / 10) != 0)
    {
        LED10 = guc_nmb_table[lus_disp_abs_val / 10];
        //------------------------------
        if (guc_disp_rate == dot_0d1)
        {
            LED10 |= data_dot;
        }
        //------------------------------
        if (bflg_disp_minus == 1)
        {
            if (LED9 == data_all_off)
            {
                LED9 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        if (guc_disp_rate == dot_0d1)
        {
            LED10 = data_0 | data_dot;
        }
        else
        {
            if (LED9 == data_all_off)
            {
                LED10 = data_all_off;
            }
            else
            {
                LED10 = data_0;
            }
        }
    }
    //------------------------------------------------------
    lus_disp_abs_val %= 10;
    if (lus_disp_abs_val != 0)
    {
        LED11 = guc_nmb_table[lus_disp_abs_val];
        if (bflg_disp_minus == 1)
        {
            if (LED10 == data_all_off)
            {
                LED10 = data_hyphen;
                bflg_disp_minus = 0;
            }
            else if (LED9 == data_all_off)
            {
                LED9 = data_hyphen;
                bflg_disp_minus = 0;
            }
        }
    }
    else
    {
        LED11 = data_0;
    }
}
/******************************************************************************
function: 1、低温压机运行灯的指示
          2、8个报警的闪烁指示
          3、新增的远程报警灯的指示

函数功能：10ms-------------------------------未调
******************************************************************************/
void lamp_disp_deal(void)        //灯显示处理程序，在100ms定时程序中调用
{
    //低温压机运行的指示灯
   


	//闪烁的8个灯
    //----------------------------------
    if (bflg_alarm_ALH == 1)             //peak  高温报警标志 1 (统计报警使用的数量)
    { 
        if (bflg_led_loop_set == 1)
        {                                
            LED12 |= 0x01;                 
        }
        else
        {
            LED12 &= ~0x01;
        }
    }
    else
    {
        LED12 &= ~0x01;
    }
    //----------------------------------
    if (bflg_alarm_ALL == 1)             //peak 低温报警标志 2
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x02;
        }
        else
        {
            LED12 &= ~0x02;
        }
    }
    else
    {
        LED12 &= ~0x02;
    }
    //----------------------------------
    if (bflg_alarm_power_off == 1)       //peak 断电报警标志 3
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x04;
        }
        else
        {
            LED12 &= ~0x04;
        }
    }
    else
    {
        LED12 &= ~0x04;
    }
    //----------------------------------
    if (bflg_alarm_open_door == 1)       //peak  开门报警标志 4 (面板标注_电压超标)
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x08;
        }
        else
        {
            LED12 &= ~0x08;
        }
    }
    else
    {
        LED12 &= ~0x08;
    }
    //----------------------------------
    if (bflg_alarm_THW_high == 1)        //peak 环温过高报警标志 5
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x10;
        }
        else
        {
            LED12 &= ~0x10;
        }
    }
    else
    {
        LED12 &= ~0x10;
    }
    //----------------------------------
    if (bflg_alarm_cond_dirty == 1)      //peak  冷凝器脏报警标志 6
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x20;
        }
        else
        {
            LED12 &= ~0x20;
        }
    }
    else
    {
        LED12 &= ~0x20;
    }               
   // 电池未连接 、电池电量低、电池插反这三种报警都是闪烁同一个灯
    if ((bflg_alarm_bat_discnnt == 1) || (bflg_alarm_bat_low == 1) || (bflg_alarm_bat_fed == 1))  
    {                                     //peak  蓄电池故障  7 、8 、9
        if ((bflg_led_loop_set == 1))  
        {
            LED12 |= 0x40;
        }
        else
        {
            LED12 &= ~0x40;
        }
    }
    else
    {
        LED12 &= ~0x40;
    }     
    //----------------------------------
    //主、环温、冷凝、热交换这4种传感器故障都是闪烁这个灯
    if (bflg_alarm_T_fault == 1)         //peak 传感器故障报警标志 10、11、12、13 
    {
        if (bflg_led_loop_set == 1)
        {
            LED12 |= 0x80;
        }
        else
        {
            LED12 &= ~0x80;
        }
    }
    else
    {
        LED12 &= ~0x80;
    }

    //远程报警灯
    //高温、低温、断电、开门 任意一种报警就开远程
    if(bflg_alarm_ALH == 1 ||  bflg_alarm_ALL == 1 || bflg_alarm_power_off == 1 || bflg_alarm_open_door == 1)
    {
        RL_ALM_OUT_PIN = 1;    //开
    }
    else
    {
        RL_ALM_OUT_PIN = 0;    //关
    }
}



//------------------------------------------------------------------------------
/*******************************************************************************
function:  主函数中每隔多长时间读取一次按键值，即这个函数 read_key_deal()
********************************************************************************/
void read_key_delaytime(void) //读键延时程序，在10ms定时程序中调用
{
    if (bflg_read_key_delaytime == 1)
    {
        gss_read_key_delaytimer++;
        if (gss_read_key_delaytimer >= 200)  // 200*10 = 2000ms = 2s
        {
            gss_read_key_delaytimer = 0;
            bflg_read_key_delaytime = 0;
        }
    }
}

//------------------------------------------------------------------------------
/*    |   The following code is added on my own
	  |
	  |
	  |
	  |    
	 \|/                                                                     */

/*===========================================================================
函数功能: 1、有报警时按下 buzz 键可30min内不再报警
          2、电池未连接时，长按buzz 5s,则在电池下一次插反之前不会报警；
        从原来函数中提取出的；

函数位置: 主循环 ----------------ok
===========================================================================*/
void  key_buzz_cancel(void)
{
    if (guc_key_val == KEY_SET)  
    {
        if (bflg_buzz_loop_tick_set == 1)         // 如果此时有蜂鸣器报警，但是按下了"蜂鸣取消"按键，则会延时30 min 不会响，
        {                                         // 30 min 后恢复正常控制；
           if (bflg_alarm_buzz_off_delaytime == 0)//若按下则要将这30min进行完后再计时
            {
                bflg_alarm_buzz_off_delaytime = 1;
                gss_alarm_buzz_off_delaytimer = 0;
            }       
        }
    }           
}


/********************************************************************************
function:            
          电池未连接，长按5s后，在下一次检测到电池这段时间内不响，
          
          当再次检测到电池，恢复正常控制；
函数位置: 主循环-------------------ok          
********************************************************************************/
void bat_con_check_again(void)    
{
	if (bflg_key_long_buz_valid == 1)        //有电池未连接，但设置了bS参数未1了
	{
		if (bflg_alarm_bat_discnnt == 0)     //如果又检测到电池
		{    
		     //debug 注释
			 bflg_key_long_buz_valid = 0;    //恢复正常控制
			 //gss_factory_bS = 0;           //临时注释
		}
	}
}

/******************************************************************************
function: 显示板未解锁时，按一下 “蜂鸣取消”键可查询故障代码如下:

函数位置； 在 led_disp1_deal(void) 中,在10ms定时器中--------ok
******************************************************************************/
void view_error_code(void)    
{ 
    /*
    bflg_alarm_ALH        
    bflg_alarm_ALL
    bflg_alarm_power_fault
    bflg_TTEST_fault
    bflg_alarm_open_door
    */
    
    if ((bflg_alarm_ALH == 1) && (bflg_error_code_alarm_ALH_mark ==0))        //1
    {
        bflg_error_code_delaytime = 1;

        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_1;
    }   
    else if ((bflg_alarm_ALL == 1) && (bflg_error_code_alarm_ALL_mark ==0))  //2 
    {
        bflg_error_code_delaytime = 1;
     
        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_2;
    }
    else if ((bflg_alarm_power_fault == 1) && (bflg_error_code_alarm_power_fault_mark ==0))    //3 
    {
        bflg_error_code_delaytime = 1;
    
        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_3;
    }
    else if ((bflg_TTEST_fault == 1) && (bflg_error_code_alarm_TTEST_fault_mark ==0))  //4
    {
        bflg_error_code_delaytime = 1;
     
        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_4;
    }
    else if ((bflg_alarm_open_door == 1) && (bflg_error_code_alarm_door_open_mark ==0))  //5
    {
        bflg_error_code_delaytime = 1;
    
        LED2 = data_E_dp;
        LED3 = data_0;
        LED4 = data_5;
    }
    else
    {
        flag_view_error_code = 0;                    // 可显示正常温度了
    
        flg_error_code_mark.uword = 0;
    }
}


/***********************************************************************************************
功能描述 :

函数位置: 在 100ms 中调用  ------ok
***********************************************************************************************/
void view_error_code_delaytime (void)
{
    if (bflg_error_code_delaytime == 1) 
    {
        gss_error_code_delaytime ++;
        if (gss_error_code_delaytime >= 20)       // 100 * 20 = 2s
        {
            bflg_error_code_delaytime = 0;
            gss_error_code_delaytime = 0;

            if ((bflg_alarm_ALH == 1) && (bflg_error_code_alarm_ALH_mark ==0))        //1
            {
                bflg_error_code_alarm_ALH_mark = 1;
            }   
            else if ((bflg_alarm_ALL == 1) && (bflg_error_code_alarm_ALL_mark ==0))     //2 
            {
                bflg_error_code_alarm_ALL_mark = 1;
            }
            else if ((bflg_alarm_power_fault == 1) && (bflg_error_code_alarm_power_fault_mark ==0))     //3 
            {
                bflg_error_code_alarm_power_fault_mark = 1;
            }
            else if ((bflg_TTEST_fault == 1) && (bflg_error_code_alarm_TTEST_fault_mark ==0))     //4
            {
                bflg_error_code_alarm_TTEST_fault_mark = 1;
            }
            else if ((bflg_alarm_open_door == 1) && (bflg_error_code_alarm_door_open_mark ==0))       //5
            {
                bflg_error_code_alarm_door_open_mark = 1;
            }
        }
    }
}



/*******************************************************************************************
功能描述: 指示灯闪烁时间标志的设定,可一直在这里循环

函数位置:100ms 定时器中 ---ok

校正: >=1  ----->  >1
*******************************************************************************************/
void led_loop_delaytime(void)    //--已用
{
    gss_led_loop_delaytime++;
	if (gss_led_loop_delaytime > 1)      // 100ms * 1 = 100ms 
	{
		gss_led_loop_delaytime = 0;
	    bflg_led_loop_set = ~bflg_led_loop_set;
	}
}

/*******************************************************************************************
功能描述: 在"箱内温度设定"、"高温报警设定"、"低温报警设定" 模式时，对应的数值要闪烁

函数位置:    100ms 定时器中----------ok
*******************************************************************************************/
void led_disp1_flicker_delaytime(void)
{
    gss_disp1_flicker_delaytime++;
    if(gss_disp1_flicker_delaytime > 5)
    {       
        gss_disp1_flicker_delaytime = 0;
        bflg_disp1_flicker = ~bflg_disp1_flicker;      
    }
}



/******************************************************************************
function: 在正常模式下，长按set+down 5s进入IC注册密码模式
          在IC注册密码模式时对该函数进行处理；密码正确进入注册模式；

          if(guc_disp1_mode == DISP1_IC_PASSWORD) 
          {
             key_IC_password_deal();
          }
          
函数位置:　  read_key_deal();     -----------------------OK    
******************************************************************************/
void key_IC_password_deal(void)  //按键密码处理程序
{
   /* if (guc_key_val == KEY_BUZ)
    {
        if(gsc_disp_user2_mode == DISP_USER2_CL1)    //如果是user2层的 IC注销参数时进入的
        {           
            guc_disp_mode = DISP_USER2;   
            //gsc_disp_user2_mode = DISP_USER2_dA;   //未锁定再次进入此层user2显示默认的参数
            bflg_set2_val = 0;                       //如果是在CL1 指令中返回的
        }
        else
        {
            guc_disp_mode = DISP_LOCKED;   
            
            bflg_IC_register_state = 0;              //后加 待验证
        }

    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_IC_password_val++;
        if (gsc_IC_password_val > 999)
        {
            gsc_IC_password_val = 0;
        }     
    }
    
    else if (guc_key_val == KEY_DOWN)
    {       
        gsc_IC_password_val--;
        if (gsc_IC_password_val < 0)
        {
            gsc_IC_password_val = 999;
        }      
    }
    
    else if (guc_key_val == KEY_SET)
    {       
        guc_IC_password_cnt++;
        if(guc_IC_password_cnt >= 3)
        {   
            bflg_IC_register_state = 0;           //清可进入IC密码状态
            gsc_IC_password_val = 0;
            guc_IC_password_cnt = 0;
            guc_disp_mode = DISP_LOCKED;
            gsc_disp_user1_mode = DISP_USER1_TS;    
            gsc_disp_user2_mode = DISP_USER2_dA; 
        }
        else
        {            
            if (gsc_IC_password_val == gss_user2_IC)      // 输入的密码是否和设置的一样
            {        
                guc_IC_password_cnt = 0;
                
                if(gsc_disp_user2_mode == DISP_USER2_CL1) //如果是user2层的 IC注销参数
                {
                    bflg_IC_erasure_order = 1;            //注销所有IC卡
                    
                    bflg_IC_pass_CL1 = 1;                 //CL1闪烁显示的标志
                }
                else
                {              
                    guc_disp_mode  = DISP_IC_REG;
                }
            }
        }        
    }*/
}

/*******************************************************************************
函数描述:  在IC注册时最左面的数码管的显示处理

函数位置:  read_key_deal() 中的 在其中 DISP1_IC_REG 状态下执行----------OK
*******************************************************************************/
void key_IC_register_deal(void)
{/*
    if(guc_key_val == KEY_BUZ)                // 按下SET键，退出并锁定 
    {
       bflg_IC_register_state = 0;            // 退出 IC注册状态
       guc_temp_flash_add = 0;
       guc_flash_add = 0;
       
       guc_disp_mode = DISP_LOCKED;            //回到上层的显示状态
    }*/
}

/*******************************************************************************
函数描述:  factory工厂模式的AT指令的测试
           
函数位置:  10ms定时器---------------------------------------ok
*******************************************************************************/
void key_factory_AT_test(void)
{
    LED1 = 0xff;
    LED2 = 0xff;
    LED3 = 0xff;
    LED4 = 0xff;
    LED5 = 0xff;
    LED6 = 0xff;
    LED7 = 0xff;
    LED8 = 0xff;
    LED9 = 0xff;
    LED10 = 0xff;
    LED11 = 0xff;
    LED12 = 0xff;    
    
    RL_ALM_OUT_PIN = 1;                       //远程报警灯开
    bflg_alarm_lamp = 1;                      //3位报警灯
    
    gss_factory_AT_test_delaytime++;
    if(gss_factory_AT_test_delaytime >= 600)  //10ms * 600 = 6s 后恢复正常
    {
        gss_factory_AT_test_delaytime = 0;
        bflg_factory_AT_test = 0;

        LED1 = 0x00;
        LED2 = 0x00;
        LED3 = 0x00;
        LED4 = 0x00;
        LED5 = 0x00;
        LED6 = 0x00;
        LED7 = 0x00;
        LED8 = 0x00;
        LED9 = 0x00;
        LED10 = 0x00;
        LED11 = 0x00;
        LED12 = 0x00;  
    }
}

/*******************************************************************************
函数描述:  CL4参数指令对应恢复出厂值功能
           
函数位置: 主循环---------------------------------------ok
*******************************************************************************/
void key_factory_init(void)
{
    if(bflg_factory_CL4 == 1)
    {
        bflg_factory_CL4 = 0;
        
        init_ram_para2();             // peak 烧写E2，新板子要先写一次；
        init_ram_para();
        
        eeprom2_read_deal();          //eeprom2读取处理程序
        eeprom_read_deal();           //eeprom读取处理程序
        /*
        gss_user_set_parm_value[DISP_USER_SET_TS] = ram_para[num_comp2_TS];
        gss_user_set_parm_value[DISP_USER_SET_ALH] = ram_para[num_comp2_ALH];
        gss_user_set_parm_value[DISP_USER_SET_ALL] = ram_para[num_comp2_ALL];
        //-------------------------------------------------
        gss_user_set_parm_value[DISP_USER_SET_dA]  = ram_para[num_door_alarm_delaytime];
        gss_user2_T1  = ram_para[num_usb_read_delaytime];
        //gss_user2_T2  未保存
        gss_user2_P6  = ram_para[num_usb_P6];
        gss_user2_IC  = ram_para[num_IC_password];
        //gss_user2_CL1 
        //-------------------------------------------------
        //gss_factory_CL2        
        gss_factory_Ct = ram_para[num_pt100_calibration];     
        gss_factory_parm_value[DISP_FAC_CU] = ram_para[num_voltage_calibration];  
        gss_factory_CA = ram_para[num_THW_calibration];  
        gss_factory_parm_value[DISP_FAC_Fd] = ram_para[num_buzz_alarm_delaytime];  
        //gss_factory_AT  
        gss_factory_SS = ram_para[num_TE_fault_shield];  
        gss_factory_AA = ram_para[num_THW_hight_alarm];  
        gss_factory_Cd = ram_para[num_comp_startup_delaytime];  
        //gss_factory_CL3 
        //gss_factory_CL4  
        gss_factory_PS =  ram_para[num_password_val];  
        */
    }
}

/*******************************************************************************
函数描述:  刚上电时自检，所有灯亮灭3s,响1声
           
函数位置:  10ms定时器----------------------------------ok
*******************************************************************************/
void system_self_test(void)
{
    if(bflg_self_test_sound == 0)
    {
        bflg_self_test_sound = 1;
        guc_buzz_sound_cnt = 1;     //响1声
    }

    gss_self_test_delaytime++;
    if(gss_self_test_delaytime >= 100)    //10ms*100 = 1s
    {
        gss_self_test_delaytime = 0;
        bflg_self_test_flicker = ~bflg_self_test_flicker;

        gss_self_test_cnt++;
        if(gss_self_test_cnt > 4 )
        {
            gss_self_test_cnt = 0;
            bflg_self_test = 0;
        }
    }


    if(bflg_self_test_flicker == 0)
    {   //全亮
        LED1 = 0xff;
        LED2 = 0xff;
        LED3 = 0xff;
        LED4 = 0xff;
        LED5 = 0xff;
        LED6 = 0xff;
        LED7 = 0xff;
        LED8 = 0xff;
        LED9 = 0xff;
        LED10 = 0xff;
        LED11 = 0xff;
        LED12 = 0xff; 
        
        bflg_alarm_lamp = 1;
    }
    else
    {   //全灭
        LED1 = 0x00;
        LED2 = 0x00;
        LED3 = 0x00;
        LED4 = 0x00;
        LED5 = 0x00;
        LED6 = 0x00;
        LED7 = 0x00;
        LED8 = 0x00;
        LED9 = 0x00;
        LED10 = 0x00;
        LED11 = 0x00;
        LED12 = 0x00; 

        bflg_alarm_lamp = 0;
    }
}

/*********************************************************************
函数描述: 判断是否打开IC卡

函数位置: 主函数中   ------ok
**********************************************************************/
void card_IC_deal(void)
{
 /*   if(guc_disp_mode != DISP_IC_REG)   //在IC卡注册状态时不进行开锁的动作
    {
        if(guc_IC_access_state == 1)
        {
            LOCK_OUT_PIN = 1;          //锁打开
            guc_IC_access_state = 0;

            bflg_lock_delaytimer = 1;  //开锁的延时标志
            guc_lock_delaytimer = 0;
        }
    }*/
}
/*********************************************************************
函数描述: 判断是否打开IC卡

函数位置: 1s 定时 -----ok
**********************************************************************/
void lock_open_delaytimer(void)
{
    if(bflg_lock_delaytimer == 1)
    {
        guc_lock_delaytimer++;
        if(guc_lock_delaytimer >= 10)    // 锁开10s
        {
            bflg_lock_delaytimer = 0;
            guc_lock_delaytimer = 0;

            LOCK_OUT_PIN = 0;
        }
    }
}


//----------------------------------------------------------------------------------
/* 
        /|\
         |
         |
         |
         |  The above is my own code
************************************************************************************/



#endif

