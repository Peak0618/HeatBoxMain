#ifndef _COM_C_
#define _COM_C_
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
#include "h_com.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "h_rom_para2.h"
//#include "h_comp2.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak 
#include "h_main.h"
#include "m_key_operation.h"
#include "m_PTC_heat.h"
#include "m_peripheral_control.h"

//------------------------------------------------------------------------------
//函数声明
void com_init(void);//通讯初始化程序，在初始化程序中调用

void com_tx_init(void);  //通讯发送初始化程序，在主循环程序中调用

void com_rx_init(void);  //通讯接收初始化程序，在主循环程序中调用

void com_rx_data_deal(void);  //通讯接收数据处理程序，在主循环程序中调用

void com_tx_delaytime(void);  //通讯发送延时程序，在1ms定时程序中调用

void com_rx_delaytime(void);  //通讯接收延时程序，在1ms定时程序中调用

void com_rx_end_delaytime(void);   //通讯接收完成延时程序，在1ms定时程序中调用

void com_fault_delaytime(void);    //通讯故障延时程序，在1s定时程序中调用

void com_tx_int(void);   //通讯发送中断程序

void com_rx_int(uint8_t rx_data);  //通讯接收中断程序

unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen);    //进行CRC校验的程序



//------------------------------------------------------------------------------
//标志定义
flag_type flg_com;
flag_type flg_read_write;
//------------------------------------------------------------------------------
//变量定义
uint8_t   guc_com_tx_buffer[120];        //通讯发送缓存器
uint8_t   guc_com_rx_buffer[80];         //通讯接收缓存器

int16_t   gss_com_tx_delaytimer;        //通讯发送延时计时器
int16_t   gss_com_rx_delaytimer;        //通讯接收延时计时器

int16_t   gss_com_rx_end_delaytimer;    //通讯接收完标志
int16_t   gss_com_fault_delaytimer;     //通讯故障延时计时器

uint8_t   guc_com_tx_point;   //通讯发送索引
uint8_t   guc_com_rx_point;   //通讯接收索引

uint8_t   guc_com_type_cnt;   //通讯类型计数器

//peak add
uint8_t  guc_IC_access_state = 0;    // 接收的IC卡的指令

uint16_t guc_flash_add;              // 接收的IC卡注册已用的地址，用来显示有多少张卡注册
uint16_t guc_temp_flash_add;         // 接收的IC卡注册临时保存的地址，用来和现在的地址对比是否有新的IC卡注册，锁屏时间好重重新
uint8_t  guc_IC_erasure_tx_ok_count; // 注销IC卡指令，在bflg_com_tx_ok中设置发送多次计数,防止丢包


int16_t gss_liquid_led_button;              //液晶屏的开灯按钮  ：1开 0关
int16_t gss_liquid_ozone_disinfect_button;  //液晶屏的启动杀菌按钮：1开 0关
int16_t gss_pwm_adjust;                     //对PTC模块输出pwm占空比的调节

#define   WRITE_STATE_CNT     50

#define   READ_USER_CNT       30
#define   READ_ADMIN_CNT      30

#define   LIQU_ROK_SDELAY     20   //液晶屏接收成功后，延时xms再发送
//------------------------------------------------------------------------------
//程序内容
//------------------------------------------------------------------------------
/*******************************************************************************
function:  初始化时(仅一次)，为了刚运行时打开发送中断，发送数据；
    

*******************************************************************************/
void com_init(void) //通讯初始化程序，在初始化程序中调用
{
    bflg_com_tx_delaytime = 1;     //peak 延时时间到，打开发送中断，发送数据；
    gss_com_tx_delaytimer = 3000;
    
    bflg_com_rx_delaytime = 0;
    gss_com_rx_delaytimer = 0;
    
    bflg_read_write_data = 1; //首次写数据
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 在每一次发送数据时，对其初始化；
         LED: 发送显示板上所有要显示的内容(txt)

         初始化内容:
           1、发送的查询指令(固定)
           2、发送的设定数据---->|a、 "工作状态"、"报警"、"故障指示"、"电压"、"环温"
                                 |b、"温度设定值" "高温报警设定值" "低温报警设定值"

           a---根据主控板本身的数据采集来判断；
           
           b---是显示板按键设置好的值； 过程如下
               显示板按键设置后--->发送给主板-->主板再将此值发送给显示板显示；  
               
********************************************************************************/
void com_tx_init(void)   //通讯发送初始化程序，在主循环程序中调用
{
    int8_t i;
    word_type gss_tmp;
    word_type com_crc;
    //----------------------------------------------------------LED屏-----------------------------------
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if (bflg_read_write_data == 0)  //如果是读数据  //peak 读按键值
        {
            guc_com_tx_buffer[0] = 0x00;     //设备地址
            guc_com_tx_buffer[1] = 0x03;     //读命令
            
            guc_com_tx_buffer[2] = 0x00;
            guc_com_tx_buffer[3] = 0x07;     //起始地址7
            
            guc_com_tx_buffer[4] = 0x00;
            guc_com_tx_buffer[5] = 2;        //地址个数
            //----------------------------------------------
            com_crc.uword = CRC16(guc_com_tx_buffer, 6);
            guc_com_tx_buffer[6] = com_crc.ubyte.low;
            guc_com_tx_buffer[7] = com_crc.ubyte.high;
        }
        else   //如果是写数据   //peak 设定数码管、LED显示内容
        {
            guc_com_tx_buffer[0] = 0x00;     //设备地址
            guc_com_tx_buffer[1] = 0x10;     //写命令
            
            guc_com_tx_buffer[2] = 0x00;
            guc_com_tx_buffer[3] = 0x00;     //起始地址0
            
            guc_com_tx_buffer[4] = 0x00;
            guc_com_tx_buffer[5] = 22;        //地址个数  peak 要写几个寄存器，每个寄存器占2个字节；7*2 = 14 个字节；
            
            guc_com_tx_buffer[6] = 44;       //字节数    peak [7]--[20] = 14 具体的内容

            for (i = 0; i < 12; i++)         // peak [7]~[18] = 12 ;
            {
                guc_com_tx_buffer[i + 7] = guc_led_buffer[i];    //peak 数码管寄存器
            }           
            
            //----------------------------------------------
            if(bflg_IC_erasure_order == 1)  //如有注销指令,注销IC卡指令_在bflg_com_tx_ok中设置发送多次计数,防止丢包
            {               
                guc_com_tx_buffer[19] = 1;
            }
            else
            {
                guc_com_tx_buffer[19] = 0;
            }
            
            if (bflg_alarm_lamp == 0)         //peak 显示板上的三位报警灯 
            {
                //guc_com_tx_buffer[19] = 0;
                guc_com_tx_buffer[20] = 0x00;
            }
            else
            {
                //guc_com_tx_buffer[19] = 0;
                guc_com_tx_buffer[20] = 0x07;     //peak 显示板上面的 "报警" 的三个灯 
            }
        

            //----------------------------------------------
            //com_crc.uword = CRC16(guc_com_tx_buffer, 21);
            //guc_com_tx_buffer[21] = com_crc.ubyte.low;
            //guc_com_tx_buffer[22] = com_crc.ubyte.high;

            com_crc.uword = CRC16(guc_com_tx_buffer, 51);  //21
            guc_com_tx_buffer[51] = com_crc.ubyte.low;     //21
            guc_com_tx_buffer[52] = com_crc.ubyte.high;    //22            
        }
    }
//---------------------------------------------------------------------液晶屏------------------
    else  //如果是液晶屏通讯
    {
        if (bflg_read_write_data == 0)  //如果是读数据
        {
            if (bflg_read_user_admin == 0)   //如果是读用户信息
            {
                guc_com_tx_buffer[0] = 0x01;      //设备地址
                guc_com_tx_buffer[1] = 0x03;      //读命令
                
                guc_com_tx_buffer[2] = 0xA4;
                guc_com_tx_buffer[3] = 0x1F;      //起始地址8100
                
                guc_com_tx_buffer[4] = READ_USER_CNT;
                guc_com_tx_buffer[5] = 0x00;       //地址个数
                //------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    //如果是读参数信息
            {
                guc_com_tx_buffer[0] = 0x01;      //设备地址
                guc_com_tx_buffer[1] = 0x03;      //读命令
                
                guc_com_tx_buffer[2] = 0xD6;
                guc_com_tx_buffer[3] = 0x1F;      //起始地址8150
                
                guc_com_tx_buffer[4] = READ_ADMIN_CNT;
                guc_com_tx_buffer[5] = 0x00;       //地址个数
                //------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
        }
        else   //如果是写数据
        {
            guc_com_tx_buffer[0] = 0x01;     //设备地址
            guc_com_tx_buffer[1] = 0x10;     //写命令
            
            guc_com_tx_buffer[2] = 0x40;
            guc_com_tx_buffer[3] = 0x1F;     //起始地址10000
            
            guc_com_tx_buffer[4] = WRITE_STATE_CNT;
            guc_com_tx_buffer[5] = 0x00;    //地址个数
            
            guc_com_tx_buffer[6] = (WRITE_STATE_CNT * 2);   //字节数
            //----------------------------------------------
            {
                //debug
                //gss_TTEST = -1997;
                
                gss_tmp.sword = gss_TTEST;      //测试传感器温度
                guc_com_tx_buffer[7] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[8] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gss_TDIS;      //显示传感器温度
                guc_com_tx_buffer[9] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[10] = gss_tmp.ubyte.high;
                //------------------------------------------
                if(bflg_PTC_heat_on == 0)     //PTC加热模块状态
                {
                    guc_com_tx_buffer[11] = 0;
                    guc_com_tx_buffer[12] = 0;
                }
                else
                {
                    guc_com_tx_buffer[11] = 1;
                    guc_com_tx_buffer[12] = 0;
                }
                //------------------------------------------
                if(bflg_fan_on == 0)            //内风机状态
                {
                    guc_com_tx_buffer[13] = 0;
                    guc_com_tx_buffer[14] = 0;
                }
                else
                {
                    guc_com_tx_buffer[13] = 1;
                    guc_com_tx_buffer[14] = 0;
                }
                //------------------------------------------
                if(bflg_led_on == 0)            //照明灯状态
                {
                    guc_com_tx_buffer[15] = 0;
                    guc_com_tx_buffer[16] = 0;
                }
                else
                {
                    guc_com_tx_buffer[15] = 1;
                    guc_com_tx_buffer[16] = 0;
                }
                //------------------------------------------
                if(bflg_germicidal_lamp_on == 0)  //杀菌灯状态
                {
                    guc_com_tx_buffer[17] = 0;
                    guc_com_tx_buffer[18] = 0;
                }
                else
                {
                    guc_com_tx_buffer[17] = 1;
                    guc_com_tx_buffer[18] = 0;
                }
                //------------------------------------------
                if (DOOR1_IN_PIN == 0)                   // 0关门
                {
                    guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0;                
                }
                else
                {
                    guc_com_tx_buffer[19] = 1;
                    guc_com_tx_buffer[20] = 0;
                }
               /* if (bflg_prot2_door1_pin == 0)    //开门状态
                {
                    guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0;
                }
                else
                {
                    guc_com_tx_buffer[19] = 1;
                    guc_com_tx_buffer[20] = 0;
                }*/
                
                gss_tmp.sword = gus_trip_code1;   //报警状态信息1
                guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gus_trip_code2;   //报警状态信息2
                guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gss_VAC;          //电源电压
                guc_com_tx_buffer[25] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[26] = gss_tmp.ubyte.high;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[27] = 0;
                guc_com_tx_buffer[28] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[29] = 0;
                guc_com_tx_buffer[30] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[31] = 0;
                guc_com_tx_buffer[32] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[33] = 0;
                guc_com_tx_buffer[34] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[35] = 0;
                guc_com_tx_buffer[36] = 0;
                //------------------------------------------
                //
                guc_com_tx_buffer[37] = 0;
                guc_com_tx_buffer[38] = 0;
                //------------------------------------------
                //------------------------------------------
                //预留
                guc_com_tx_buffer[39] = 0;
                guc_com_tx_buffer[40] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[41] = 0;
                guc_com_tx_buffer[42] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[43] = 0;
                guc_com_tx_buffer[44] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[45] = 0;
                guc_com_tx_buffer[46] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[47] = 0;
                guc_com_tx_buffer[48] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[49] = 0;
                guc_com_tx_buffer[50] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[51] = 0;
                guc_com_tx_buffer[52] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[53] = 0;
                guc_com_tx_buffer[54] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[55] = 0;
                guc_com_tx_buffer[56] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[57] = 0;
                guc_com_tx_buffer[58] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[59] = 0;
                guc_com_tx_buffer[60] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[61] = 0;
                guc_com_tx_buffer[62] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[63] = 0;
                guc_com_tx_buffer[64] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[65] = 0;
                guc_com_tx_buffer[66] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[67] = 0;
                guc_com_tx_buffer[68] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[69] = 0;
                guc_com_tx_buffer[70] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[71] = 0;
                guc_com_tx_buffer[72] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[73] = 0;
                guc_com_tx_buffer[74] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[75] = 0;
                guc_com_tx_buffer[76] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[77] = 0;
                guc_com_tx_buffer[78] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[79] = 0;
                guc_com_tx_buffer[80] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[81] = 0;
                guc_com_tx_buffer[82] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[83] = 0;
                guc_com_tx_buffer[84] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[85] = 0;
                guc_com_tx_buffer[86] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[87] = 0;
                guc_com_tx_buffer[88] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[89] = 0;
                guc_com_tx_buffer[90] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[91] = 0;
                guc_com_tx_buffer[92] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[93] = 0;
                guc_com_tx_buffer[94] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[95] = 0;
                guc_com_tx_buffer[96] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[97] = 0;
                guc_com_tx_buffer[98] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[99] = 0;
                guc_com_tx_buffer[100] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[101] = 0;
                guc_com_tx_buffer[102] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[103] = 0;
                guc_com_tx_buffer[104] = 0;
                //------------------------------------------
                //预留
                guc_com_tx_buffer[105] = 0;
                guc_com_tx_buffer[106] = 0;
            }
            //----------------------------------------------
            com_crc.uword = CRC16(guc_com_tx_buffer, ((WRITE_STATE_CNT * 2) + 7));   //107
            guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7] = com_crc.ubyte.low;
            guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7 + 1] = com_crc.ubyte.high;
        }
    }
    //------------------------------------------------------
    guc_com_tx_point = 0;
    bflg_com_tx_busy = 1;
    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];
}
//------------------------------------------------------------------------------
/******************************************************************************
function:  在每次接收数据时，对其初始化；
           接收时，将要接收到缓存清空，准备接收新数据；

******************************************************************************/
void com_rx_init(void)   //通讯接收初始化程序，在主循环程序中调用
{
    uint8_t i;
    
    for (i = 0; i < 65; i++)  //20
    {
        guc_com_rx_buffer[i] = 0;  //清接收缓存器
    }
    
    bflg_com_rx_busy = 0;
    guc_com_rx_point = 0;
}
//------------------------------------------------------------------------------
/*******************************************************************************
function : 接收成功后，对数据进行处理 ；

           bflg_read_write_data--- 主机发送给显示板按照:发送查询指令、发送设定值
      这两个操作循环发送的方式；

        因为主板需要不停的判断此时按下的按键，比如增加"高温报警设定"后要改变判断的阀值了；
            显示板也需要知道此时主板有哪些故障，来进行显示；
        所以读/写要循环执行；
********************************************************************************/
void com_rx_data_deal(void)   //通讯接收数据处理程序，在主循环程序中调用
{
    uint8_t i,msb_byte = 0,lsb_byte = 0;
    uint16_t lus_tmp;
    word_type com_crc;
    word_type gss_tmp;
    //------------------------------------------------------
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据  //peak 读按键的值
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 7);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[7])
             && (com_crc.ubyte.high == guc_com_rx_buffer[8]))
            {
                bflg_com_type_ok = 1;   //通讯类型判断ok
                
                bflg_com_tx_delaytime = 1;        //置发送延时标志
                gss_com_tx_delaytimer = 10;       //发送延时计时器赋值
                
                bflg_com_fault = 0;               //清通讯故障标志
                gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                //------------------------------------------
                //数据处理
                bflg_read_write_data = 1;    //变为写数据
                
                if (guc_key_val != guc_com_rx_buffer[3])
                {
                    bflg_read_key_delaytime = 0; //peak 若键值有改变，则不需要等待一段时间后在判断键值，而是直接处理 read_key_deal()
                    gss_read_key_delaytimer = 0;
                }
                guc_key_val = guc_com_rx_buffer[3];    

                //注册状态时显示的IC已用注册的地址，显示需转换为卡片的张数 = xx/4;
                guc_flash_add = 0;
                msb_byte = guc_com_rx_buffer[4];
                lsb_byte = guc_com_rx_buffer[5];
                guc_flash_add = ((guc_flash_add | msb_byte)<<8) | lsb_byte;
                 
                //peak add 开锁指令
                if ((guc_com_rx_buffer[6] & 0x80) != 0)
                {
                      guc_IC_access_state = 1;
                }
                else
                {
                      guc_IC_access_state = 0;
                }
                //------------------------------------------
                //测试版本显示板确认
                if ((guc_com_rx_buffer[6] & 0x40) != 0)
                {
                	  bflg_test_type = 1;
                }
                else
                {
                	  bflg_test_type = 0;
                }
            }
        }
        else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据  //peak 设定数码管、LED显示  打算IC卡在此接收
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 6);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak 如果CRC校验正确
             && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
            {

               //peak add  改
               //guc_IC_access_state = guc_com_rx_buffer[2];



            
                bflg_com_type_ok = 1;           //通讯类型判断ok      //peak ??
                
                bflg_com_tx_delaytime = 1;      //置发送延时标志      //peak 之后时间到将再次允许发送
                gss_com_tx_delaytimer = 10;     //发送延时计时器赋值
                
                bflg_com_fault = 0;             //清通讯故障标志      //peak ?? 未用
                gss_com_fault_delaytimer = 0;   //清通讯故障计时器
                //------------------------------------------
                //数据处理
                bflg_read_write_data = 0;       //变为读数据          //peak 
            }
        }
    }
    else  //如果是液晶屏通讯
    {
        if ((guc_com_rx_buffer[1] == 0x03))  //如果是读数据
        {
            if (bflg_read_user_admin == 0)   //如果是读用户信息
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_USER_CNT * 2) + 3));
                if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_USER_CNT * 2) + 3])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_USER_CNT * 2) + 4]))
                {
                    bflg_com_type_ok = 1;    //通讯类型判断ok
                    
                    bflg_com_tx_delaytime = 1;         //置发送延时标志
                    gss_com_tx_delaytimer = LIQU_ROK_SDELAY;        //发送延时计时器赋值
                    
                    bflg_com_fault = 0;                //清通讯故障标志
                    gss_com_fault_delaytimer = 0;      //清通讯故障计时器
                    //--------------------------------------
                    //数据处理
                    //bflg_read_write_data = 1;     //变为写数据
                    bflg_read_user_admin = 1;       //变为读管理员信息
                    //-----------------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                    gss_user_set_parm_value[DISP_USER_SET_TS] = gss_tmp.sword; //得到设定温度 ok
                    //ram_para[num_comp2_TS] = gss_user_set_parm_value[DISP_USER_SET_TS];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                    gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_tmp.sword;//得到高温报警温度 ok
                    //ram_para[num_comp2_ALH] = gss_user_set_parm_value[DISP_USER_SET_ALH];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                    gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_tmp.sword;//得到高温报警温度 ok
                    //ram_para[num_comp2_ALL] = gss_user_set_parm_value[DISP_USER_SET_ALL];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[10];   //得到语言选择--未用
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[12];   //得到温度单位选择--未用
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                    gss_user_set_parm_value[DISP_USER_SET_Lt] = gss_tmp.sword;  //照明灯延时设置 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                    gss_user_set_parm_value[DISP_USER_SET_S1] = gss_tmp.sword;  //杀菌模式 ok 待+
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                    gss_user_set_parm_value[DISP_USER_SET_S2] = gss_tmp.sword; //杀菌时间设置 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                    gss_user_set_parm_value[DISP_USER_SET_dA] = gss_tmp.sword;  //开门报警延时时间设置 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                    gss_liquid_led_button = gss_tmp.sword;              //箱内照明灯控制按钮 ok 待+

                    if(gss_liquid_led_button == 1)
                    {
                        bflg_led_on = 1;
                    }
                    else
                    {
                        bflg_led_on = 0;
                    }
                    //led_control_deal();
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                    gss_liquid_ozone_disinfect_button = gss_tmp.sword;  //臭氧杀菌启动按钮 ok 待+
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[25];   //年 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[27];   //月 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[28];      
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[29];   //日 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[30];  
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[31];   //时 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[32];  
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[33];   //分 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[34];     
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[35];   //秒 待+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[36];                      
                }
            }
            else    //如果是读管理员信息
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_ADMIN_CNT * 2) + 3));
                if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 3])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 4]))
                {
                    bflg_com_type_ok = 1;    //通讯类型判断ok
                    
                    bflg_com_tx_delaytime = 1;         //置发送延时标志
                    gss_com_tx_delaytimer = LIQU_ROK_SDELAY;        //发送延时计时器赋值
                    
                    bflg_com_fault = 0;                //清通讯故障标志
                    gss_com_fault_delaytimer = 0;      //清通讯故障计时器
                    //--------------------------------------
                    //数据处理
                    bflg_read_write_data = 1;     //变为写数据
                    bflg_read_user_admin = 0;     //变为读用户信息
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                    gss_factory_parm_value[DISP_FAC_r0] = gss_tmp.sword;     //控制温差 ok
                    //ram_para[num_prot2_TA_first_delaytime] = gss_tmp.sword;   
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                    gss_factory_parm_value[DISP_FAC_Ct1] = gss_tmp.sword; 
                    //ram_para[num_prot2_door_delaytime] = gss_tmp.sword;     //测试传感器校准值 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                    gss_factory_parm_value[DISP_FAC_Ct2] = gss_tmp.sword; 
                    //ram_para[num_comp2_VAC_ref] = gss_tmp.sword;      //显示传感器校准值 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                    gss_factory_parm_value[DISP_FAC_Cd] = gss_tmp.sword; 
                    //ram_para[num_comp2_THW_ref] = gss_tmp.sword;      //开机PTC启动延时时间 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                    gss_factory_parm_value[DISP_FAC_T1] = gss_tmp.sword; 
                    //ram_para[num_prot2_THW_T] = gss_tmp.sword;        //故障PTC开机时间 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                    gss_factory_parm_value[DISP_FAC_T2] = gss_tmp.sword; 
                    //ram_para[num_comp2_prot_T] = gss_tmp.sword;       //故障PTC停机时间 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                    gss_factory_parm_value[DISP_FAC_r1] = gss_tmp.sword; 
                    //ram_para[num_comp2_PT100_shield] = gss_tmp.sword; //档位切换温差 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                    gss_factory_parm_value[DISP_FAC_Ct3] = gss_tmp.sword; 
                    //ram_para[num_comp2_THW_shield] = gss_tmp.sword;   //市电校准值 ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                    gss_pwm_adjust  = gss_tmp.sword;                    //pwm占空比
                }
            }
        }
        else if (guc_com_rx_buffer[1] == 0x10)    //如果是写数据
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 6);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[6])
             && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
            {
                bflg_com_type_ok = 1;        //通讯类型判断ok
                
                bflg_com_tx_delaytime = 1;   //置发送延时标志
                gss_com_tx_delaytimer = LIQU_ROK_SDELAY;  //发送延时计时器赋值
                
                bflg_com_fault = 0;               //清通讯故障标志
                gss_com_fault_delaytimer = 0;     //清通讯故障计时器
                //------------------------------------------
                //数据处理
                bflg_read_write_data = 0;   //变为读数据
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_rx_delaytime 置1后，延时gss_com_rx_delaytimer 设定的时间后，
           可以打开中断允许接收；
*******************************************************************************/
void com_rx_delaytime(void)     //通讯接收延时程序，在1ms定时程序中调用
{
    if (bflg_com_rx_delaytime == 1)
    {
        gss_com_rx_delaytimer--;
        if (gss_com_rx_delaytimer <= 0)
        {
            gss_com_rx_delaytimer = 0;
            bflg_com_rx_delaytime = 0;
            
            bflg_com_allow_rx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_tx_delaytime 此标志置1后，延时gss_com_tx_delaytimer 设定的时间
           后，打开发送中断，允许发送；
          
*******************************************************************************/
void com_tx_delaytime(void)     //通讯发送延时程序，在1ms定时程序中调用
{
    if (bflg_com_tx_delaytime == 1)
    {
        gss_com_tx_delaytimer--;
        if (gss_com_tx_delaytimer <= 0)
        {
            gss_com_tx_delaytimer = 0;
            bflg_com_tx_delaytime = 0;
            
            bflg_com_allow_tx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: 接收数据时，多长时间后，仍未正确接收到数据，或是没有接收完，
          则停止数据的接收，也不对接收的数据进行任何的处理；
          
*******************************************************************************/
void com_rx_end_delaytime(void) //通讯接收完成延时程序，在1ms定时程序中调用
{
    if (bflg_com_rx_busy == 1)  //如果当前接收忙
    {
        gss_com_rx_end_delaytimer++;
        if (gss_com_rx_end_delaytimer >= 50)
        {
            gss_com_rx_end_delaytimer = 0;
            bflg_com_rx_busy = 0;
            
            bflg_com_rx_end = 1;
            
            bflg_com_tx_delaytime = 1;
            gss_com_tx_delaytimer = 10;
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 没有对发生故障时的情况下进行相应的处理；
            

函数位置:1s定时器中-------------------------ok
******************************************************************************/
void com_fault_delaytime(void)     //通讯故障延时程序，在1s定时程序中调用
{
    if (bflg_com_fault == 0)                //peak 这个判断了并没有起作用
    {
        gss_com_fault_delaytimer++;
        if (gss_com_fault_delaytimer >= 30)  //30
        {
            gss_com_fault_delaytimer = 0;
            bflg_com_fault = 1;
        }
    }
    //----------------------------------------------
    //通讯类型再判断
    if (bflg_com_type_ok == 0)               //通讯正确后 = 1；不正确则为0；
    {
        guc_com_type_cnt++;                  //peak 开始通讯时，发送5次数据
        if (guc_com_type_cnt >= 5)
        {                                   
            bflg_com_type = ~bflg_com_type;  //peak 清0 ???
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function :中断中处理；
          发送要发送的内容----> |1、发送读显示板数据的指令  (8个字节)
                                |2、发送向显示板写的数据    (23个字节)
    
******************************************************************************/
void com_tx_int(void)    //通讯发送中断程序
{
    if (bflg_com_tx_busy == 1)
    {
        if (bflg_com_type == 0)    //如果是LED显示屏通讯
        {
            if (bflg_read_write_data == 0)   //如果是读数据
            {
                if (guc_com_tx_point < 8)
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                }
                else
                {
                    guc_com_tx_point = 0;    //清发送字节索引
                    bflg_com_tx_busy = 0;    //清发送忙标志
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                    gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 100;
                }
            }
            else    //如果是写数据
            {
                if (guc_com_tx_point < 53) //23
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                }
                else
                {
                    guc_com_tx_point = 0;    //清发送字节索引
                    bflg_com_tx_busy = 0;    //清发送忙标志
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //置通讯接收延时标志
                    gss_com_rx_delaytimer = 5;    //通讯接收延时计时器赋值
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 100;
                }
            }
        }
        else   //如果是液晶屏通讯
        {
            if (bflg_read_write_data == 0)   //如果是读数据
            {
                if (guc_com_tx_point < 8)
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                }
                else
                {
                    guc_com_tx_point = 0;    //清发送字节索引
                    bflg_com_tx_busy = 0;    //清发送忙标志
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //置com通讯接收延时标志
                    gss_com_rx_delaytimer = 5;    //com通讯接收延时计时器赋值
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 300; //1000
                }
            }
            else    //如果是写数据
            {
                if (guc_com_tx_point < ((WRITE_STATE_CNT * 2) + 9))   //109
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //发送数据
                }
                else
                {
                    guc_com_tx_point = 0;     //清发送字节索引
                    bflg_com_tx_busy = 0;     //清发送忙标志
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;     //置通讯接收延时标志
                    gss_com_rx_delaytimer = 5;     //通讯接收延时计时器赋值
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 300; //1000
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: 中断接收的数据

          *****LED****
          如果收到的是发送完查询命令后返回的值:
          共收到9个字节。功能书中可以无限扩展的，但是此程序发送查询设定为固定的     
      查询2个寄存器，所以返回时也是固定的4个字节(一个寄存器返回2个字节);
      
       ----------------------------------------   
          如果收到的是发送完设定命令后返回的值:
          共收到8个字节，功能书中就是8个；

*******************************************************************************/
void com_rx_int(uint8_t rx_data)   //通讯接收中断程序
{
    uint8_t luc_com_rx_byte;  //接收字节临时缓冲区
    
    gss_com_rx_end_delaytimer = 0; //清接收完延时计时器
    //--------------------------------------------------
    luc_com_rx_byte = rx_data;
    
    if (bflg_com_type == 0)   //如果是LED显示屏通讯
    {
        if ((bflg_com_rx_busy == 0) && (luc_com_rx_byte == 0x00))     //如果无接收忙，且从地址正确
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //如果接收忙
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 9))       //如果是查询命令，且接收数据完毕
            {
                guc_com_rx_point = 0;   //清接收字节索引
                bflg_com_rx_busy = 0;   //清接收忙标志
                bflg_com_rx_ok = 1;     //置接收成功标志
            }
            else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //如果是多个设定命令，且接收到字节数数据帧
            {
                guc_com_rx_point = 0;    //清接收字节索引
                bflg_com_rx_busy = 0;    //清接收忙标志
                bflg_com_rx_ok = 1;      //置接收成功标志
            }
        }
    }
    else  //如果是液晶屏通讯
    {
        if ((bflg_com_rx_busy == 0) && (luc_com_rx_byte == 0x01))     //如果无接收忙，且从地址正确
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //如果接收忙
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if (guc_com_rx_buffer[1] == 0x10)     //如果是多个设定命令
            {
                if (guc_com_rx_point >= 8)   //接收到字节数数据帧
                {
                    guc_com_rx_point = 0;    //清接收字节索引
                    bflg_com_rx_busy = 0;    //清接收忙标志
                    bflg_com_rx_ok = 1;      //置接收成功标志
                }
            }
            else if (guc_com_rx_buffer[1] == 0x03)     //如果是读信息
            {
                if (bflg_read_user_admin == 0)    //如果是读用户信息
                {
                    if (guc_com_rx_point >= ((READ_USER_CNT * 2) + 5))      //65
                    {
                        guc_com_rx_point = 0;     //清接收字节索引
                        bflg_com_rx_busy = 0;     //清接收忙标志
                        bflg_com_rx_ok = 1;       //置接收成功标志
                    }
                }
                else     //如果是读管理员信息
                {
                    if (guc_com_rx_point >= ((READ_ADMIN_CNT * 2) + 5))     //65
                    {
                        guc_com_rx_point = 0;     //清接收字节索引
                        bflg_com_rx_busy = 0;     //清接收忙标志
                        bflg_com_rx_ok = 1;       //置接收成功标志
                    }
                }
            }
            else     //收到的元素[1]非约定的则抛弃
            {
                
            }
        }
    }
}
//------------------------------------------------------------------------------
unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen)    //进行CRC校验的程序
{
    unsigned int uchCRC;
    unsigned int uIndex_x, uIndex_y;
    
    uchCRC = 0xFFFF;
    
    if (usDataLen > 0)
    {
        for (uIndex_x = 0; uIndex_x <= (usDataLen - 1); uIndex_x++)
        {
            uchCRC = uchCRC ^ (unsigned int) (*puchmsg);
            puchmsg++;
            
            for (uIndex_y = 0; uIndex_y <= 7; uIndex_y++)
            {
                if ((uchCRC & 0x0001) != 0)
                {
                    uchCRC = (uchCRC >> 1) ^ 0xA001;
                }
                else
                {
                    uchCRC = uchCRC >> 1;
                }
            }
        }
    }
    else
    {
        uchCRC = 0;
    }
    
    return uchCRC;
}

//---------------------------------------------------------------------
//peak add




//------------------------------------------------------------------------------
#endif
