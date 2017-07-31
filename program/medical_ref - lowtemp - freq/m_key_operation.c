/**************************************************************************************************** 
overview:20170329
                按键的操作:
                            1、通过按键选择进入不同的模式
                            2、通过按键对参数进行调节
*****************************************************************************************************/

//包含的文件
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "h_type_define.h"
#include "m_key_operation.h"
#include "h_rom_para2.h"
#include "h_main.h"
//函数声明
void mode_selection(void);
void key_locked_deal(void);
void key_locked_delaytime(void);   
void key_set_long_delaytime(void);
void key_set_up_long_delaytime(void);
void key_up_down_long_delaytime(void);
void key_set_down_long_delaytime(void);
void key_user_set_deal(void);
void key_user_manage_password_deal(void);
void key_user_manage_deal(void);
void key_factory_password_deal();
void key_factory_deal(void);
void key_inquire_deal(void);
void key_ic_password_deal(void);
void key_ic_register_deal(void);
void key_parm_default_val(void);


//标志定义
flag_type bflg_key_operation,bflg_key_operation_save;




//宏


//变量定义
uint8_t   guc_key_val;                      //按键键值
uint8_t   guc_TUTD_dis_type;                //选择显示上温 或下温或上下温平均值
uint8_t   guc_key_set_long_delaytimer;      //长按set键计时
uint8_t   guc_key_set_up_long_delaytimer;   //长按set+up键计时
uint8_t   guc_key_up_down_long_delaytimer;  //长按up+down键计时
uint8_t   guc_key_set_down_long_delaytimer; //长按set+down键计时
uint8_t   guc_key_locked_delaytimer;        //按键无操作锁定延时

int8_t    gsc_user_manage_password_val;     //用户管理密码值
int8_t    gsc_factory_password_val;         //工厂密码值
int8_t    gsc_ic_password_val;              //ic卡密码值

uint8_t   guc_disp_mode;                    //总的UI模式
int8_t    gsc_disp_parm_type;               //不同的参数类型,所有的参数都用这个
int16_t   gss_key_temp_value;               //参数的临时值,所有的参数都用这个

int8_t    gsc_disp_inq_mode;                //查询模式中不同参数

int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];        //用户设定参数值   
int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM];  //用户管理参数值   
int16_t   gss_factory_parm_value[DISP_FAC_SUM];              //工厂参数值  


/**********************************************************************************************************************************************
函数功能: 模式选择,通过按键选择进入不同的模式


函数位置: 主循环(读取按键延时)----------------------------------------------ok
***********************************************************************************************************************************************/
void mode_selection(void)
{    
    if (bflg_test_type == 1)                     //如果是测试版显示板
    {
        if (guc_key_val == 0x20)                 //SET---超低温的设置键和血液的不同
        {
            bflg_test_mode = 1;                  //置测试模式标志
        } 
        else if(guc_key_val == 0x40)             //BUZZ键
        {
        	bflg_test_mode_RTC = 1;
        	//test_mode_RTC_circuit();
        }
    }
    else if(guc_disp_mode == DISP_LOCKED)
    {
        key_locked_deal();      //锁定时按键操作
    }
    else if(guc_disp_mode == DISP_USER_SET)
    {
        key_user_set_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE_PASSWORD)
    {
    	key_user_manage_password_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE)
    {
    	key_user_manage_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY_PASSWORD)
    {
        key_factory_password_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY)
    {
        key_factory_deal();
    }
    else if(guc_disp_mode == DISP_INQUIRE)
    {
        key_inquire_deal();
    }
    else if(guc_disp_mode == DISP_IC_PASSWORD)
    {
        key_ic_password_deal();
    }
    else if(guc_disp_mode == DISP_IC_REGISTER)
    {
        key_ic_register_deal();
    }
}

/**********************************************************************************************************************************************
函数功能: 锁定模式下按键的操作


函数位置: mode_selection(即主循环)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_locked_deal(void)
{
    //上下温和平均值显示切换
    if(guc_key_val == KEY_UP)             //按up切换上温和上下温
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 1;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    
    if(guc_key_val == KEY_DOWN)            //按down切换下温和上下温
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 2;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    //--------------------------------------------------------------------------------------------------
    if (guc_key_val == KEY_SET)          //长按set键5s进入USER SET
    {
        if (bflg_key_set_long_valid == 0)           
        {
            if (bflg_key_set_long_delaytime == 0)    
            {
                bflg_key_set_long_delaytime = 1;     
                guc_key_set_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_long_valid = 0;
            
            guc_disp_mode = DISP_USER_SET;
            gsc_disp_parm_type = DISP_USER_SET_TS;
            gss_key_temp_value = 0;
            bflg_parm_save = 0;
            bflg_parm_dis_form = 0;
        }
    }
    //--------------------------------------------------------------------------------------------------
    else if(guc_key_val == KEY_SET_AND_UP)  //长按设定+上调 5s进入工厂密码
    {
        if (bflg_key_set_up_long_valid == 0)           
        {
            if (bflg_key_set_up_long_delaytime == 0)    
            {
                bflg_key_set_up_long_delaytime = 1;     
                guc_key_set_up_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_up_long_valid = 0;
            
            guc_disp_mode = DISP_FACTORY_PASSWORD;       
            gsc_factory_password_val = 0;       
        }
    }
    //--------------------------------------------------------------------------------------------------
    else if(guc_key_val == KEY_UP_AND_DOWN)  //长按上调+下调 5s进入查询模式
    {
        if (bflg_key_up_down_long_valid == 0)           
        {
            if (bflg_key_up_down_long_delaytime == 0)    
            {
                bflg_key_up_down_long_delaytime = 1;     
                guc_key_up_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_up_down_long_valid = 0;
            
            guc_disp_mode = DISP_INQUIRE;   
            gsc_disp_inq_mode = 0;
            bflg_key_inq_val = 0;
        }
    }    
    //--------------------------------------------------------------------------------------------------    
    else if(guc_key_val == KEY_SET_AND_DOWN)  //长按设置+下调 5s进入IC密码模式
    {
        if (bflg_key_set_down_long_valid == 0)           
        {
            if (bflg_key_set_down_long_delaytime == 0)    
            {
                bflg_key_set_down_long_delaytime = 1;     
                guc_key_set_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_down_long_valid = 0;
            
            //guc_disp_mode = DISP_IC_PASSWORD;     //暂时未用屏蔽
            gsc_ic_password_val = 0;
        }
    }
    else
    {
        bflg_key_set_long_delaytime = 0;     
        guc_key_set_long_delaytimer = 0;

        bflg_key_set_up_long_delaytime = 0;     
        guc_key_set_up_long_delaytimer = 0;

        bflg_key_up_down_long_delaytime = 0;     
        guc_key_up_down_long_delaytimer = 0;

        bflg_key_set_down_long_delaytime = 0;     
        guc_key_set_down_long_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
函数功能: 20s 按键没有动作进入锁定状态


函数位置:  100ms -----------------------------------------ok  
***********************************************************************************************************************************************/
void key_locked_delaytime(void)   
{
    if (guc_key_val == KEY_NONE)
    {
        guc_key_locked_delaytimer++;
        if (guc_key_locked_delaytimer >= 200)    //200*100ms=20s
        {
            guc_key_locked_delaytimer = 0;
            guc_disp_mode = DISP_LOCKED;         
        }
    }
    else
    {
        guc_key_locked_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
函数功能: 锁定模式下长按set键延时


函数位置: 100ms定时器----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_long_delaytime(void)
{
    if (bflg_key_set_long_delaytime == 1)
    {
        guc_key_set_long_delaytimer++;
        if (guc_key_set_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_long_delaytimer = 0;
            bflg_key_set_long_delaytime = 0;
            
            bflg_key_set_long_valid = 1;       
        }
    }
}
/**********************************************************************************************************************************************
函数功能: 锁定模式下长按set+up键延时


函数位置: 100ms定时器----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_up_long_delaytime(void)
{
    if (bflg_key_set_up_long_delaytime == 1)
    {
        guc_key_set_up_long_delaytimer++;
        if (guc_key_set_up_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_up_long_delaytimer = 0;
            bflg_key_set_up_long_delaytime = 0;
            
            bflg_key_set_up_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 锁定模式下长按up+down键延时


函数位置: 100ms定时器----------------------------------------------ok
***********************************************************************************************************************************************/
void key_up_down_long_delaytime(void)
{
    if (bflg_key_up_down_long_delaytime == 1)
    {
        guc_key_up_down_long_delaytimer++;
        if (guc_key_up_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_up_down_long_delaytimer = 0;
            bflg_key_up_down_long_delaytime = 0;
            
            bflg_key_up_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 锁定模式下长按set+down键延时


函数位置: 100ms定时器----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_down_long_delaytime(void)
{
    if (bflg_key_set_down_long_delaytime == 1)
    {
        guc_key_set_down_long_delaytimer++;
        if (guc_key_set_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_down_long_delaytimer = 0;
            bflg_key_set_down_long_delaytime = 0;
            
            bflg_key_set_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
函数功能: 用户设定层模式下按键的操作

使用注意：进入此层时需
         1、   guc_disp_mode = DISP_USER_SET;
         2、   gsc_disp_parm_type = 0;  默认显示首个参数
         3、   gss_key_temp_value = 0;      临时参数为0
         4、   bflg_parm_save = 0;          默认不保存
         5、   bflg_parm_dis_form = 0；      默认显示参数类型

条件:  此所有参数的步进值都是1，所以才能统一         
         
函数位置: mode_selection(即主循环)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_set_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_parm_dis_form = 0;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                    if(gsc_disp_parm_type > DISP_USER_SET_TT)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_TS;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_SET_TS)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_TT;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    if(gss_user_set_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_user_set_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save
                        if(gsc_disp_parm_type == DISP_USER_SET_TS)
                        {
                            ram_para[num_user_set_TS] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_TS, ram_para[num_user_set_TS]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                        {
                            ram_para[num_user_set_ALH] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_ALH, ram_para[num_user_set_ALH]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                        {
                            ram_para[num_user_set_ALL] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_ALL, ram_para[num_user_set_ALL]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                        {
                            ram_para[num_user_set_Lt] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_Lt, ram_para[num_user_set_Lt]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                        {
                            ram_para[num_user_set_dA] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_dA, ram_para[num_user_set_dA]);
                        }   
                        else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                        {
                            ram_para[num_user_set_S1] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_S1, ram_para[num_user_set_S1]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                        {
                            ram_para[num_user_set_S2] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_S2, ram_para[num_user_set_S2]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                        {
                            ram_para[num_user_set_TT] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_TT, ram_para[num_user_set_TT]);
                        }                         
                    }  
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value++; 
                }
                else
                {
                    gss_key_temp_value++;         

                    if(gsc_disp_parm_type == DISP_USER_SET_TS)
                    {
                        if(gss_key_temp_value > 500)    //50℃  magnify 10
                        {
                            gss_key_temp_value = 500;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                    {
                        if(gss_key_temp_value > 510)     //51℃  magnify 10
                        {
                            gss_key_temp_value = 510;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                    {
                        if(gss_key_temp_value > (gss_user_set_parm_value[DISP_USER_SET_TS] - 10))    
                        {
                            gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS] - 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                    {
                        if(gss_key_temp_value > 30)     //min
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                    {
                        if(gss_key_temp_value > 30)      //min
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                    {
                        if(gss_key_temp_value > 1)  
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                    {
                        if(gss_key_temp_value > 999)    
                        {
                            gss_key_temp_value = 999;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                    {
                        if(gss_key_temp_value > 1)    
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value--; 
                }
                else
                {
                    gss_key_temp_value--;         

                    if(gsc_disp_parm_type == DISP_USER_SET_TS)
                    {
                        if(gss_key_temp_value < 260)    //26℃  magnify 10
                        {
                            gss_key_temp_value = 260;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                    {
                        if(gss_key_temp_value < (gss_user_set_parm_value[DISP_USER_SET_TS]+10))      
                        {
                            gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS]+10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                    {
                        if(gss_key_temp_value < 250)    
                        {
                            gss_key_temp_value = 250;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                    {
                        if(gss_key_temp_value < -1)     //min
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                    {
                        if(gss_key_temp_value < 1)      //min
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                    {
                        if(gss_key_temp_value < 0)  
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                    {
                        if(gss_key_temp_value < -1)    
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                    {
                        if(gss_key_temp_value < 0)    
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                }
            }
        }
    }
}


/**********************************************************************************************************************************************
函数功能: 从用户设定层进入用户管理层时的密码操作

使用注意: 进入此层应
          1、  guc_disp_mode = DISP_USER_MANAGE_PASSWORD;
          2、  gsc_user_manage_password_val = 0;  

函数位置: mode_selection(即主循环)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_user_manage_password_val++;
        if (gsc_user_manage_password_val > 99)
        {
            gsc_user_manage_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_user_manage_password_val--;
        if (gsc_user_manage_password_val < 0)
        {
            gsc_user_manage_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_user_manage_password_val == 6)                //暂时用默认
        {
            guc_disp_mode = DISP_USER_MANAGE; 
            gsc_disp_parm_type = 0;
            gss_key_temp_value = 0;    
            bflg_parm_save = 0;       
            bflg_parm_dis_form = 0；   
        }
    }
}


/**********************************************************************************************************************************************
函数功能: 用户管理层的按键操作

使用注意：进入此层时需
         1、   guc_disp_mode = DISP_USER_MANAGE;
         2、   gsc_disp_parm_type = 0;     默认显示首个参数
         3、   gss_key_temp_value = 0;     临时参数为0
         4、   bflg_parm_save = 0;         默认不保存
         5、   bflg_parm_dis_form = 0；     默认显示参数类型

条件:  此所有参数的步进值都是1，所以才能统一    


函数位置: mode_selection(即主循环)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_USER_MANAGE_TT)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_IC;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_MANAGE_IC)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_TT;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)  //special
                    {
                         
                    }
                    else if(gss_user_manage_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_user_manage_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save_wait
                        /*
                        ram_para[num_usb_read_delaytime] = gss_user2_T1;                                                                    //!!!!!!!
                        eeprom_write(num_usb_read_delaytime, ram_para[num_usb_read_delaytime]); 
                        */
                        //eeprom_write(num_user_set_TS, gss_user_set_parm_value[DISP_USER_SET_TS]); 
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value++; 
                }
                else
                {
                    gss_key_temp_value++;         

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                    {
                        if(gss_key_temp_value > 999)    
                        {
                            gss_key_temp_value = 999;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                    {
                        // wait
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                    {
                        if(gss_key_temp_value > 60)    
                        {
                            gss_key_temp_value = 60;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                    {
                        if(gss_key_temp_value > 30)     
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                    {
                        if(gss_key_temp_value > 3)     
                        {
                            gss_key_temp_value = 3;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                    {
                        if(gss_key_temp_value > 999)     
                        {
                            gss_key_temp_value = 999;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }                      
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value--; 
                }
                else
                {
                    gss_key_temp_value--;         

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                    {
                        if(gss_key_temp_value < 0)    
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                    {
                        // wait
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                    {
                        if(gss_key_temp_value < -1)    
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                    {
                        if(gss_key_temp_value < 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                    {
                        if(gss_key_temp_value < 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                    {
                        if(gss_key_temp_value < 0)     
                        {
                            gss_key_temp_value = 0;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                    {
                        if(gss_key_temp_value < 0)     
                        {
                            gss_key_temp_value = 0;
                        }
                    }                      
                }
            }
        }
    }
}


/**********************************************************************************************************************************************
函数功能: 工厂密码模式下的操作


函数位置: mode_selection(即主循环)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_factory_password_val++;
        if (gsc_factory_password_val > 99)
        {
            gsc_factory_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_factory_password_val--;
        if (gsc_factory_password_val < 0)
        {
            gsc_factory_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_factory_password_val == 6)                //暂时用默认
        {
            guc_disp_mode = DISP_FACTORY; 
            gsc_disp_parm_type = 0;   
            gss_key_temp_value = 0;      
            bflg_parm_save = 0;          
            bflg_parm_dis_form = 0；     
        }
    }
}


/**********************************************************************************************************************************************
函数功能: 工厂模式下的按键操作


函数位置: mode_selection(即主循环)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_FAC_Fd)
                    {
                        gsc_disp_parm_type = DISP_FAC_r0;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_FAC_r0)
                    {
                        gsc_disp_parm_type = DISP_FAC_Fd;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    //have special
                    if(gss_factory_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_factory_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save_wait
                        if(gsc_disp_parm_type == DISP_FAC_r0)
                        {
                            ram_para[num_fac_r0] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_r0, ram_para[num_fac_r0]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_r1)
                        {
                            ram_para[num_fac_r1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_r1, ram_para[num_fac_r1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                        {
                            ram_para[num_fac_Ct1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Ct1, ram_para[num_fac_Ct1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                        {
                            ram_para[num_fac_Ct2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Ct2, ram_para[num_fac_Ct2]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Cd)
                        {
                            ram_para[num_fac_Cd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Cd, ram_para[num_fac_Cd]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_T1)
                        {
                            ram_para[num_fac_T1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_T1, ram_para[num_fac_T1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_T2)
                        {
                            ram_para[num_fac_T2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_T2, ram_para[num_fac_T2]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Fd)
                        {
                            ram_para[num_fac_Fd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Fd, ram_para[num_fac_Fd]);
                        }                        
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)          //刚进入时先将原来的值赋给临时值
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];

                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1)) //这两个参数步进0.5
                    {
                        gss_key_temp_value += 5; 
                    }
                    else
                    {
                        gss_key_temp_value++; 
                    }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value += 5; 
                     }
                     else
                     {
                         gss_key_temp_value++; 
                     }

                    if(gsc_disp_parm_type == DISP_FAC_r0)
                    {
                        if(gss_key_temp_value > 100)     // 10℃ magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_r1)
                    {
                        if(gss_key_temp_value > 300)    // 30℃ magnify 10
                        {
                            gss_key_temp_value = 300;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                    {
                        if(gss_key_temp_value > 100)    //10℃ magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                    {
                        if(gss_key_temp_value > 100)     //10℃ magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Cd)
                    {
                        if(gss_key_temp_value > 15)     //min
                        {
                            gss_key_temp_value = 15;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T1)
                    {
                        if(gss_key_temp_value > 10)     //min
                        {
                            gss_key_temp_value = 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T2)
                    {
                        if(gss_key_temp_value > 10)    //min  
                        {
                            gss_key_temp_value = 10;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_FAC_Fd)
                    {
                        if(gss_key_temp_value > 3)    //级别 使用时*10 
                        {
                            gss_key_temp_value = 3;
                        }
                    }   
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];
                    
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value -= 5; 
                     }
                     else
                     {
                         gss_key_temp_value--; 
                     }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value -= 5; 
                     }
                     else
                     {
                         gss_key_temp_value--; 
                     }

                    if(gsc_disp_parm_type == DISP_FAC_r0)
                    {
                        if(gss_key_temp_value < -100)    // -10℃ magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_r1)
                    {
                        if(gss_key_temp_value < 10)    // 1℃ magnify 10
                        {
                            gss_key_temp_value = 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                    {
                        if(gss_key_temp_value < -100)    //-10℃ magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                    {
                        if(gss_key_temp_value < -100)     //-10℃ magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Cd)
                    {
                        if(gss_key_temp_value < 0)        //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T1)
                    {
                        if(gss_key_temp_value < 0)       //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T2)
                    {
                        if(gss_key_temp_value < 0)       //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_FAC_Fd)
                    {
                        if(gss_key_temp_value < 1)      //级别
                        {
                            gss_key_temp_value = 1;
                        }
                    }   
                }
            }
        }
    }
}



/**********************************************************************************************************************************************
函数功能: 工厂模式下的按键操作


函数位置: mode_selection(即主循环)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_inquire_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_key_inq_val = 0;
    }
    else
    {
        if(bflg_key_inq_val == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_inq_mode++;
                    if(gsc_disp_inq_mode > DISP_INQ_d9)
                    {
                        gsc_disp_inq_mode = DISP_INQ_d1;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_inq_mode--;
                    if(gsc_disp_inq_mode < DISP_INQ_d1)

                    {
                        gsc_disp_inq_mode = DISP_INQ_d9;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 0;
            }
        }
    }
}

/**********************************************************************************************************************************************
函数功能:IC密码模式下的按键操作


函数位置: mode_selection(即主循环)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_ic_password_val++;
        if (gsc_ic_password_val > 99)
        {
            gsc_ic_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_ic_password_val--;
        if (gsc_ic_password_val < 0)
        {
            gsc_ic_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_ic_password_val == 8)                //暂时用默认
        {
            guc_disp_mode = DISP_IC_REGISTER;                       
            gsc_ic_password_val = 0;                 
        }
    }
}

/**********************************************************************************************************************************************
函数功能:IC注册模式下的按键操作


函数位置: mode_selection(即主循环)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_register_deal(void)
{
    if(guc_key_val == KEY_BUZ)                
    {
        //bflg_IC_register_state = 0;         // 退出 IC注册状态
        //guc_temp_flash_add = 0;
        //guc_flash_add = 0;

        guc_disp_mode = DISP_LOCKED;           
    }     
}

/**********************************************************************************************************************************************
函数功能: 用户设定参数、用户管理参数、工厂参数的默认值（以后要从E2读取）


函数位置: 初始化中---------------------------------------------ok
***********************************************************************************************************************************************/
void key_parm_default_val(void)
{   
    //用户设定参数
    gss_user_set_parm_value[DISP_USER_SET_TS]  = ram_para[num_user_set_TS];  //magnify 用到时不需*10了
    gss_user_set_parm_value[DISP_USER_SET_ALH] = ram_para[num_user_set_ALH]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_ALL] = ram_para[num_user_set_ALL]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_Lt]  = ram_para[num_user_set_Lt];
    gss_user_set_parm_value[DISP_USER_SET_dA]  = ram_para[num_user_set_dA];
    gss_user_set_parm_value[DISP_USER_SET_S1]  = ram_para[num_user_set_S1];
    gss_user_set_parm_value[DISP_USER_SET_S2]  = ram_para[num_user_set_S2];
    gss_user_set_parm_value[DISP_USER_SET_TT]  = ram_para[num_user_set_TT];

    
    //工厂参数
    gss_factory_parm_value[DISP_FAC_r0]  = ram_para[num_fac_r0];   //magnify  
    gss_factory_parm_value[DISP_FAC_r1]  = ram_para[num_fac_r1];   //magnify  
    gss_factory_parm_value[DISP_FAC_Ct1] = ram_para[num_fac_Ct1];  //magnify
    gss_factory_parm_value[DISP_FAC_Ct2] = ram_para[num_fac_Ct2];  //magnify
    gss_factory_parm_value[DISP_FAC_Cd]  = ram_para[num_fac_Cd]; 
    gss_factory_parm_value[DISP_FAC_T1]  = ram_para[num_fac_T1]; 
    gss_factory_parm_value[DISP_FAC_T2]  = ram_para[num_fac_T2]; 
    gss_factory_parm_value[DISP_FAC_Fd]  = ram_para[num_fac_Fd];    //级别 使用时*10     

    /*  血液
    //用户设定参数
    gss_user_set_parm_value[DISP_USER_SET_TS]  = 360;  //magnify 用到时不需*10了
    gss_user_set_parm_value[DISP_USER_SET_ALH] = 400;  //magnify
    gss_user_set_parm_value[DISP_USER_SET_ALL] = 300;  //magnify
    gss_user_set_parm_value[DISP_USER_SET_Lt]  = 2;
    gss_user_set_parm_value[DISP_USER_SET_dA]  = 5;
    gss_user_set_parm_value[DISP_USER_SET_S1]  = 1;
    gss_user_set_parm_value[DISP_USER_SET_S2]  = 30;
    gss_user_set_parm_value[DISP_USER_SET_TT]  = 0;
    
    //用户管理参数----加温箱未用，因为充血液移植的所以保留
    gss_user_manage_parm_value[DISP_USER_MANAGE_IC]  = 8;    
    gss_user_manage_parm_value[DISP_USER_MANAGE_CL1] = 0;
    gss_user_manage_parm_value[DISP_USER_MANAGE_Lt]  = 8;   
    gss_user_manage_parm_value[DISP_USER_MANAGE_dA]  = 5;  
    gss_user_manage_parm_value[DISP_USER_MANAGE_FA]  = 3;   
    gss_user_manage_parm_value[DISP_USER_MANAGE_S1]  = 1;
    gss_user_manage_parm_value[DISP_USER_MANAGE_S2]  = 60;  
    gss_user_manage_parm_value[DISP_USER_MANAGE_TT]  = 0;

    //工厂参数
    gss_factory_parm_value[DISP_FAC_r0]  = 10;   //magnify  
    gss_factory_parm_value[DISP_FAC_r1]  = 100;  //magnify  
    gss_factory_parm_value[DISP_FAC_Ct1] = 0;    //magnify
    gss_factory_parm_value[DISP_FAC_Ct2] = 0;    //magnify
    gss_factory_parm_value[DISP_FAC_Cd] = 0; 
    gss_factory_parm_value[DISP_FAC_T1] = 1; 
    gss_factory_parm_value[DISP_FAC_T2] = 3; 
    gss_factory_parm_value[DISP_FAC_Fd] = 1;     //级别 使用时*10 
    */
}



/*******************************************END OF THE FILE*************************************************************/



