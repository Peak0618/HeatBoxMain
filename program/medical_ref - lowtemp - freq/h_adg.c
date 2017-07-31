/******************************************************************************
概述:
     此文件是对获取外部的热电偶、精密电阻的AD值进行处理；
******************************************************************************/

#ifndef _ADG_C_
#define _ADG_C_
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
#include "h_adg.h"
#include "h_main.h"
#include "h_ad.h"

//------------------------------------------------------------------------------
//函数声明
void adg_init(void);     //ad模块初始化程序，在初始化程序中调用

void adg_data_delaytime(void);//ad模块数据处理延时程序，在100ms定时程序中调用

int16_t get_adg_temp(uint16_t lus_ad);  //根据ad值查表得到温度值1

int16_t get_adg_temp2(uint16_t lus_ad); //根据ad值查表得到温度值2

void adg_iic_int(void);       //ad模块iic模拟中断程序，在定时中断调用

uint8_t TEST(uint8_t luc_data, uint8_t luc_bit);

uint8_t SET(uint8_t luc_bit);

uint8_t CLR(uint8_t luc_bit);

//------------------------------------------------------------------------------
//变量定义
//------------------------------------------------------------------------------
//输入输出口定义
#define   SCLK_OUT_PIN        P4.3
#define   CLK_OUT_PIN         P4.4
#define   CS_OUT_PIN          P4.5
#define   DIN_OUT_PIN         P4.6
#define   DOUT_OUT_PIN        P4.7

#define   EN_OUT_PIN          P12.0
#define   WR_OUT_PIN          P14.0
#define   CSB_OUT_PIN         P14.1
#define   CSA_OUT_PIN         P14.2
#define   A0_OUT_PIN          P10.2
#define   A1_OUT_PIN          P0.1
#define   A2_OUT_PIN          P0.0
#define   A3_OUT_PIN          P14.5
//------------------------------------------------------------------------------
//命令字定义
#define   WRITE_MODE_ORDER    0x08
#define   WRITE_MODE_REG1     0x20
#define   WRITE_MODE_REG2     0x08

#define   WRITE_CONFIG_ORDER  0x10
#define   WRITE_CONFIG_REG1   0x45
#define   WRITE_CONFIG_REG2   0x90

#define   WRITE_CONFIG2_ORDER 0x10
#define   WRITE_CONFIG2_REG1  0x00
#define   WRITE_CONFIG2_REG2  0x81

#define   READ_STATE_ORDER    0x40
#define   READ_DATA_ORDER     0x58

#define   H_LEVEL             1
#define   L_LEVEL             0

//------------------------------------------------------------------------------
//标志定义
flag_type flg_adg;
//------------------------------------------------------------------------------
uint8_t   guc_write_state;    //写状态阶段
uint8_t   guc_read_state;     //读状态阶段

uint8_t   guc_adg_bit;

uint8_t   guc_state_buffer;   //状态寄存器
word_type gus_data_buffer;    //数据寄存器

int16_t   gss_adg_data_delaytimer; //ad模块数据处理延时计时器

uint16_t  gus_adg_buffer[10];      //ad模块寄存器
uint32_t  gul_adg_sum_buffer[10];  //ad模块累加和寄存器
uint16_t  gus_adg_aver_buffer[10]; //ad模块平均值寄存器
uint8_t   guc_adg_cnt;
uint8_t   guc_adg_index;

uint16_t  gus_adg2_buffer;         //ad模块2寄存器
uint32_t  gul_adg2_sum_buffer;     //ad模块2累加和寄存器
uint16_t  gus_adg2_aver_buffer;    //ad模块2平均值寄存器

uint8_t   guc_write_config_cnt;    //写配置寄存器次数

//------------------------------------------------------------------------------
#define   TEMP_TAB_LENGTH     300  //热电偶温度表
#define   TEMP2_TAB_LENGTH    140  //热电偶温度表
//------------------------------------------------------------------------------
// peak 热电偶对应的温度值
const int16_t temp_for_adg[] =
{
  -200
, -199
, -198
, -197
, -196
, -195
, -194
, -193
, -192
, -191
, -190
, -189
, -188
, -187
, -186
, -185
, -184
, -183
, -182
, -181
, -180
, -179
, -178
, -177
, -176
, -175
, -174
, -173
, -172
, -171
, -170
, -169
, -168
, -167
, -166
, -165
, -164
, -163
, -162
, -161
, -160
, -159
, -158
, -157
, -156
, -155
, -154
, -153
, -152
, -151
, -150
, -149
, -148
, -147
, -146
, -145
, -144
, -143
, -142
, -141
, -140
, -139
, -138
, -137
, -136
, -135
, -134
, -133
, -132
, -131
, -130
, -129
, -128
, -127
, -126
, -125
, -124
, -123
, -122
, -121
, -120
, -119
, -118
, -117
, -116
, -115
, -114
, -113
, -112
, -111
, -110
, -109
, -108
, -107
, -106
, -105
, -104
, -103
, -102
, -101
, -100
, -99
, -98
, -97
, -96
, -95
, -94
, -93
, -92
, -91
, -90
, -89
, -88
, -87
, -86
, -85
, -84
, -83
, -82
, -81
, -80
, -79
, -78
, -77
, -76
, -75
, -74
, -73
, -72
, -71
, -70
, -69
, -68
, -67
, -66
, -65
, -64
, -63
, -62
, -61
, -60
, -59
, -58
, -57
, -56
, -55
, -54
, -53
, -52
, -51
, -50
, -49
, -48
, -47
, -46
, -45
, -44
, -43
, -42
, -41
, -40
, -39
, -38
, -37
, -36
, -35
, -34
, -33
, -32
, -31
, -30
, -29
, -28
, -27
, -26
, -25
, -24
, -23
, -22
, -21
, -20
, -19
, -18
, -17
, -16
, -15
, -14
, -13
, -12
, -11
, -10
, -9
, -8
, -7
, -6
, -5
, -4
, -3
, -2
, -1
, 0
, 1
, 2
, 3
, 4
, 5
, 6
, 7
, 8
, 9
, 10
, 11
, 12
, 13
, 14
, 15
, 16
, 17
, 18
, 19
, 20
, 21
, 22
, 23
, 24
, 25
, 26
, 27
, 28
, 29
, 30
, 31
, 32
, 33
, 34
, 35
, 36
, 37
, 38
, 39
, 40
, 41
, 42
, 43
, 44
, 45
, 46
, 47
, 48
, 49
, 50
, 51
, 52
, 53
, 54
, 55
, 56
, 57
, 58
, 59
, 60
, 61
, 62
, 63
, 64
, 65
, 66
, 67
, 68
, 69
, 70
, 71
, 72
, 73
, 74
, 75
, 76
, 77
, 78
, 79
, 80
, 81
, 82
, 83
, 84
, 85
, 86
, 87
, 88
, 89
, 90
, 91
, 92
, 93
, 94
, 95
, 96
, 97
, 98
, 99
, 100
};
//------------------------------------------------------------------------------
//peak 热电偶对应的AD值
const int16_t adg_to_temp[] =
{
  28075 
, 28088 
, 28101 
, 28115 
, 28128 
, 28142 
, 28156 
, 28170 
, 28184 
, 28198 
, 28213 
, 28228 
, 28242 
, 28257 
, 28272 
, 28286 
, 28301 
, 28317 
, 28332 
, 28347 
, 28363 
, 28378 
, 28394 
, 28410 
, 28426 
, 28442 
, 28458 
, 28474 
, 28491 
, 28507 
, 28523 
, 28540 
, 28557 
, 28574 
, 28591 
, 28608 
, 28625 
, 28643 
, 28661 
, 28678 
, 28696 
, 28713 
, 28731 
, 28749 
, 28767 
, 28785 
, 28804 
, 28822 
, 28840 
, 28859 
, 28878 
, 28897 
, 28916 
, 28935 
, 28954 
, 28973 
, 28993 
, 29012 
, 29031 
, 29051 
, 29071 
, 29091 
, 29111 
, 29131 
, 29151 
, 29171 
, 29192 
, 29212 
, 29233 
, 29254 
, 29275 
, 29296 
, 29317 
, 29337 
, 29358 
, 29380 
, 29402 
, 29423 
, 29444 
, 29466 
, 29488 
, 29510 
, 29532 
, 29554 
, 29577 
, 29599 
, 29621 
, 29644 
, 29666 
, 29689 
, 29712 
, 29735 
, 29758 
, 29781 
, 29804 
, 29828 
, 29851 
, 29875 
, 29898 
, 29922 
, 29946 
, 29970 
, 29993 
, 30018 
, 30042 
, 30066 
, 30091 
, 30115 
, 30140 
, 30165 
, 30189 
, 30214 
, 30239 
, 30264 
, 30289 
, 30314 
, 30340 
, 30366 
, 30391 
, 30417 
, 30442 
, 30468 
, 30494 
, 30520 
, 30546 
, 30572 
, 30598 
, 30625 
, 30652 
, 30678 
, 30704 
, 30731 
, 30758 
, 30785 
, 30812 
, 30839 
, 30866 
, 30894 
, 30921 
, 30948 
, 30976 
, 31003 
, 31031 
, 31059 
, 31087 
, 31115 
, 31143 
, 31171 
, 31199 
, 31228 
, 31256 
, 31285 
, 31313 
, 31342 
, 31371 
, 31400 
, 31429 
, 31458 
, 31487 
, 31516 
, 31545 
, 31575 
, 31604 
, 31633 
, 31663 
, 31693 
, 31723 
, 31753 
, 31783 
, 31813 
, 31843 
, 31873 
, 31903 
, 31934 
, 31964 
, 31995 
, 32025 
, 32056 
, 32087 
, 32117 
, 32149 
, 32180 
, 32211 
, 32242 
, 32273 
, 32305 
, 32336 
, 32367 
, 32399 
, 32431 
, 32463 
, 32495 
, 32526 
, 32558 
, 32590 
, 32622 
, 32655 
, 32687 
, 32719 
, 32752 
, 32784 
, 32817 
, 32850 
, 32883 
, 32915 
, 32948 
, 32981 
, 33013 
, 33047 
, 33080 
, 33113 
, 33146 
, 33179 
, 33212 
, 33245 
, 33279 
, 33312 
, 33346 
, 33380 
, 33414 
, 33447 
, 33481 
, 33515 
, 33549 
, 33583 
, 33617 
, 33652 
, 33686 
, 33720 
, 33754 
, 33789 
, 33824 
, 33858 
, 33893 
, 33928 
, 33963 
, 33997 
, 34032 
, 34067 
, 34103 
, 34138 
, 34173 
, 34208 
, 34244 
, 34279 
, 34315 
, 34351 
, 34386 
, 34422 
, 34458 
, 34494 
, 34530 
, 34566 
, 34602 
, 34638 
, 34674 
, 34711 
, 34747 
, 34784 
, 34820 
, 34857 
, 34894 
, 34931 
, 34967 
, 35004 
, 35041 
, 35078 
, 35115 
, 35152 
, 35189 
, 35227 
, 35264 
, 35302 
, 35339 
, 35376 
, 35414 
, 35452 
, 35490 
, 35527 
, 35566 
, 35604 
, 35642 
, 35680 
, 35718 
, 35756 
, 35795 
, 35833 
, 35871 
, 35910 
, 35948 
, 35987 
, 36026 
, 36065 
, 36103 
, 36142 
, 36181 
, 36220 
, 36259 
, 36298 
, 36338 
, 36377 
, 36416 
, 36455 
, 36495 
, 36534 
, 36574 
, 36614 
, 36653 
, 36693 
, 36732 
, 36773 
, 36813 
, 36852 
, 36893 
, 36933 
, 36972 
, 37013 
, 37053 
, 37093 
, 37133 
, 37174 
, 37215 
, 37255 
, 37296 
, 37336 
, 37377 
, 37418 
, 37458 
, 37500 
, 37541 
, 37582 
, 37623 
, 37664 
, 37705 
, 37746 
, 37787 
, 37828 
, 37870 
, 37911 
, 37953 
, 37995 
, 38036 
, 38078 
, 38120 
, 38161 
, 38203 
, 38245 
, 38287 
, 38329 
, 38371 
, 38413 
, 38455 
, 38497 
, 38539 
, 38582 
, 38624 
, 38666 
, 38709 
, 38751 
, 38794 
, 38837 
, 38879 
, 38921 
, 38964 
, 39007 
, 39050 
, 39093 
, 39136 
, 39179 
, 39222 
, 39265 
, 39309 
, 39352 
, 39395 
, 39438 
, 39481 
, 39525 
, 39569 
, 39612 
, 39656 
, 39700 
, 39743 
, 39787 
, 39830 
, 39874 
, 39918 
, 39962 
, 40006 
, 40050 
, 40094 
, 40138 
, 40182 
, 40226 
, 40270 
, 40315 
, 40359 
, 40404 
, 40448 
, 40493 
, 40537 
, 40582 
};
//------------------------------------------------------------------------------
// peak 精密电阻对应的温度值
const int16_t temp2_for_adg[] =
{
  -40 
, -39 
, -38 
, -37 
, -36 
, -35 
, -34 
, -33 
, -32 
, -31 
, -30 
, -29 
, -28 
, -27 
, -26 
, -25 
, -24 
, -23 
, -22 
, -21 
, -20 
, -19 
, -18 
, -17 
, -16 
, -15 
, -14 
, -13 
, -12 
, -11 
, -10 
, -9 
, -8 
, -7 
, -6 
, -5 
, -4 
, -3 
, -2 
, -1 
, 0 
, 1 
, 2 
, 3 
, 4 
, 5 
, 6 
, 7 
, 8 
, 9 
, 10 
, 11 
, 12 
, 13 
, 14 
, 15 
, 16 
, 17 
, 18 
, 19 
, 20 
, 21 
, 22 
, 23 
, 24 
, 25 
, 26 
, 27 
, 28 
, 29 
, 30 
, 31 
, 32 
, 33 
, 34 
, 35 
, 36 
, 37 
, 38 
, 39 
, 40 
, 41 
, 42 
, 43 
, 44 
, 45 
, 46 
, 47 
, 48 
, 49 
, 50 
, 51 
, 52 
, 53 
, 54 
, 55 
, 56 
, 57 
, 58 
, 59 
, 60 
, 61 
, 62 
, 63 
, 64 
, 65 
, 66 
, 67 
, 68 
, 69 
, 70 
, 71 
, 72 
, 73 
, 74 
, 75 
, 76 
, 77 
, 78 
, 79 
, 80 
, 81 
, 82 
, 83 
, 84 
, 85 
, 86 
, 87 
, 88 
, 89 
, 90 
, 91 
, 92 
, 93 
, 94 
, 95 
, 96 
, 97 
, 98 
, 99 
, 100 
};
//------------------------------------------------------------------------------
//peak  精密电阻对应的AD值
const int16_t adg_to_temp2[] =
{
  48531
, 47778
, 47062
, 46381
, 45732
, 45116
, 44532
, 43974
, 43444
, 42941
, 42461
, 42007
, 41574
, 41163
, 40774
, 40402
, 40048
, 39713
, 39396
, 39093
, 38805
, 38532
, 38272
, 38025
, 37791
, 37568
, 37356
, 37153
, 36960
, 36777
, 36603
, 36437
, 36279
, 36129
, 35986
, 35850
, 35721
, 35597
, 35479
, 35367
, 35260
, 35159
, 35061
, 34968
, 34880
, 34796
, 34715
, 34639
, 34565
, 34495
, 34428
, 34365
, 34303
, 34245
, 34189
, 34136
, 34085
, 34037
, 33990
, 33945
, 33903
, 33862
, 33823
, 33785
, 33750
, 33715
, 33682
, 33651
, 33620
, 33591
, 33564
, 33537
, 33512
, 33487
, 33463
, 33441
, 33419
, 33398
, 33378
, 33359
, 33341
, 33323
, 33306
, 33290
, 33274
, 33259
, 33244
, 33230
, 33216
, 33204
, 33191
, 33179
, 33167
, 33156
, 33145
, 33135
, 33125
, 33116
, 33106
, 33097
, 33089
, 33080
, 33072
, 33065
, 33057
, 33050
, 33043
, 33036
, 33030
, 33024
, 33017
, 33012
, 33006
, 33000
, 32995
, 32990
, 32985
, 32980
, 32976
, 32971
, 32967
, 32963
, 32959
, 32955
, 32951
, 32947
, 32944
, 32940
, 32937
, 32934
, 32931
, 32928
, 32925
, 32922
, 32919
, 32916
, 32914
, 32911
, 32909
, 32906
, 32904
};
//------------------------------------------------------------------------------
//程序内容//
//------------------------------------------------------------------------------
void adg_init(void)      //ad模块初始化程序，在初始化程序中调用
{
    guc_write_state = 1;
    guc_adg_bit = 0;
    
    CLK_OUT_PIN = 0;
    CS_OUT_PIN = 1;
    SCLK_OUT_PIN = 1;
    DIN_OUT_PIN = 1;
    //----------------------------------
    EN_OUT_PIN = 0;
    WR_OUT_PIN = 0;
    CSB_OUT_PIN = 0;
    CSA_OUT_PIN = 0;
    A3_OUT_PIN = 0;
    A2_OUT_PIN = 0;
    A1_OUT_PIN = 0;
    A0_OUT_PIN = 0;
    //----------------------------------
    bflg_write_read_adg = 1;
    guc_read_state = 1;
    guc_adg_cnt = 9;
    guc_adg_index = 9;
}
//------------------------------------------------------------------------------
void adg_data_delaytime(void) //ad模块数据处理延时程序，在100ms定时程序中调用
{
    gss_adg_data_delaytimer++;
    if (gss_adg_data_delaytimer >= 2)
    {
        gss_adg_data_delaytimer = 0;
        
        if (bflg_adg_data_ok == 0)
        {
            if (bflg_adg_channel == 0)  //如果是通道1
            {
                if (bflg_write_read_adg == 0)
                {
                    if (bflg_write_config_adg == 1)
                    {
                        guc_write_config_cnt++;
                        if (guc_write_config_cnt >= 5)
                        {
                            guc_write_config_cnt = 0;
                            bflg_write_config_adg = 0;
                        }
                    }
                    else
                    {
                        if (guc_write_state == 1)
                        {
                            bflg_write_read_adg = 1;
                            guc_read_state = 1;
                        }
                    }
                }
                else
                {
                    if (guc_read_state == 1)
                    {
                        bflg_write_read_adg = 0;
                        guc_write_state = 1;
                        
                        gus_adg_buffer[guc_adg_index] = gus_data_buffer.uword;
                        
                        if (bflg_test_mode == 0)
                        {
                            if (guc_adg_cnt > 0)
                            {
                                gul_adg_sum_buffer[guc_adg_index] += gus_adg_buffer[guc_adg_index];
                            }
                            
                            guc_adg_cnt++;
                            if (guc_adg_cnt >= 9)
                            {
                                guc_adg_cnt = 0;
                                bflg_write_config_adg = 1;
                                
                                gus_adg_aver_buffer[guc_adg_index] = (uint16_t)(gul_adg_sum_buffer[guc_adg_index] >> 3);
                                gul_adg_sum_buffer[guc_adg_index] = 0;
                                
                                gss_adg_Temp[guc_adg_index] = gss_adg_Temp2 - get_adg_temp(gus_adg_aver_buffer[guc_adg_index]);
                                //gss_adg_Temp[guc_adg_index] = get_adg_temp(gus_adg_aver_buffer[guc_adg_index]);
                                
                                guc_adg_index++;
                                if (guc_adg_index >= 10)
                                {
                                    guc_adg_index = 0;
                                    
                                    bflg_adg_channel = 1;
                                }
                            }
                        }
                        else  //如果是测试模式
                        {
                            bflg_write_config_adg = 1;
                            gss_adg_Temp[guc_adg_index] = gss_adg_Temp2 - get_adg_temp(gus_adg_buffer[guc_adg_index]);
                            
                            guc_adg_index++;
                            if (guc_adg_index >= 10)
                            {
                                guc_adg_index = 0;
                                
                                bflg_adg_channel = 1;
                                    
                                bflg_test_ok = 1;  //测试完成标志
                            }
                        }
                        //--------------------------------------
                        if (guc_adg_index == 0)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 0;
                        }
                        else if (guc_adg_index == 1)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 1;
                        }
                        else if (guc_adg_index == 2)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 1;
                            A0_OUT_PIN = 0;
                        }
                        else if (guc_adg_index == 3)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 1;
                            A0_OUT_PIN = 1;
                        }
                        else if (guc_adg_index == 4)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 1;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 0;
                        }
                        else if (guc_adg_index == 5)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 1;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 1;
                        }
                        else if (guc_adg_index == 6)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 1;
                            A1_OUT_PIN = 1;
                            A0_OUT_PIN = 0;
                        }
                        else if (guc_adg_index == 7)
                        {
                            A3_OUT_PIN = 0;
                            A2_OUT_PIN = 1;
                            A1_OUT_PIN = 1;
                            A0_OUT_PIN = 1;
                        }
                        else if (guc_adg_index == 8)
                        {
                            A3_OUT_PIN = 1;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 0;
                        }
                        else if (guc_adg_index == 9)
                        {
                            A3_OUT_PIN = 1;
                            A2_OUT_PIN = 0;
                            A1_OUT_PIN = 0;
                            A0_OUT_PIN = 1;
                        }
                    }
                }
            }
            else    //如果是通道2
            {
                if (bflg_write_read_adg == 0)
                {
                    if (bflg_write_config_adg == 1)
                    {
                        guc_write_config_cnt++;
                        if (guc_write_config_cnt >= 5)
                        {
                            guc_write_config_cnt = 0;
                            bflg_write_config_adg = 0;
                        }
                    }
                    else
                    {
                        if (guc_write_state == 1)
                        {
                            bflg_write_read_adg = 1;
                            guc_read_state = 1;
                        }
                    }
                }
                else
                {
                    if (guc_read_state == 1)
                    {
                        bflg_write_read_adg = 0;
                        guc_write_state = 1;
                        
                        gus_adg2_buffer = gus_data_buffer.uword;
                        
                        if (guc_adg_cnt > 0)
                        {
                            gul_adg2_sum_buffer += gus_adg2_buffer;
                        }
                        
                        guc_adg_cnt++;
                        if (guc_adg_cnt >= 9)
                        {
                            guc_adg_cnt = 0;
                            bflg_write_config_adg = 1;
                            
                            gus_adg2_aver_buffer = (uint16_t)(gul_adg2_sum_buffer >> 3);
                            gul_adg2_sum_buffer = 0;
                            
                            gss_adg_Temp2 = get_adg_temp2(gus_adg2_aver_buffer);
                            
                            bflg_adg_channel = 0;
                        }
                    }
                }
            }
            
            bflg_adg_data_ok = 1;
        }
    }
}
//------------------------------------------------------------------------------
int16_t get_adg_temp(uint16_t lus_ad)   //根据ad值查表得到温度值1
{
    int16_t lss_temp_index;
    lss_temp_index = (TEMP_TAB_LENGTH >> 1);
    
    if (lus_ad > adg_to_temp[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP_TAB_LENGTH)
            {
                lss_temp_index = TEMP_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad < adg_to_temp[lss_temp_index])
            {
                lss_temp_index--;
                break;
            }
        }
        while (lus_ad > adg_to_temp[lss_temp_index]);
    }
    else if (lus_ad < adg_to_temp[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad > adg_to_temp[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad < adg_to_temp[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    return temp_for_adg[lss_temp_index];
}
//------------------------------------------------------------------------------
int16_t get_adg_temp2(uint16_t lus_ad)      //根据ad值查表得到温度值2
{
    int16_t lss_temp_index;
    lss_temp_index = (TEMP2_TAB_LENGTH >> 1);
    
    if (lus_ad > adg_to_temp2[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < adg_to_temp2[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad > adg_to_temp2[lss_temp_index]);
    }
    else if (lus_ad < adg_to_temp2[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP2_TAB_LENGTH)
            {
                lss_temp_index = TEMP2_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > adg_to_temp2[lss_temp_index])
            {
                lss_temp_index--;
                break;
            }
        }
        while (lus_ad < adg_to_temp2[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    return temp2_for_adg[lss_temp_index];
}
//------------------------------------------------------------------------------
void adg_iic_int(void)        //ad模块iic模拟中断程序，在定时中断调用
{
    if (bflg_adg_data_ok == 1)
    {
        CS_OUT_PIN = L_LEVEL;
        SCLK_OUT_PIN = ~SCLK_OUT_PIN;
        //--------------------------------------------------
        if (bflg_adg_reset == 1)   //如果是ad模块复位
        {
            DIN_OUT_PIN = H_LEVEL;
            
            if (SCLK_OUT_PIN == L_LEVEL)
            {
                guc_adg_bit++;
                //if (guc_adg_bit >= 32)
                if (guc_adg_bit >= 40)
                {
                    guc_adg_bit = 0;
                    CS_OUT_PIN = H_LEVEL;
                    bflg_adg_reset = 0;
                    bflg_adg_data_ok = 0;
                }
            }
        }
        else if (bflg_adg_channel == 0) //如果是通道1
        {
            if (bflg_write_read_adg == 0)    //如果是写ad模块标志
            {
                if (bflg_write_config_adg == 1)   //如果是写配置ad模块
                {
                    if (guc_write_state == 1)     //如果是写配置寄存器命令
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG_ORDER, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 2;   //写模式寄存器1
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 2)     //如果是写配置寄存器1
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG_REG1, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 3;   //写模式寄存器2
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 3)     //如果是写配置寄存器2
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG_REG2, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 1;   //写模式寄存器阶段1
                                CS_OUT_PIN = H_LEVEL;
                                
                                bflg_adg_data_ok = 0;
                            }
                        }
                    }
                }
                else    //如果是写模式ad模块
                {
                    if (guc_write_state == 1)   //如果是写模式寄存器命令
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_ORDER, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 2; //写模式寄存器1
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 2)   //如果是写模式寄存器1
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_REG1, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 3; //写模式寄存器2
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 3)   //如果是写模式寄存器2
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_REG2, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 1; //写模式寄存器阶段1
                                CS_OUT_PIN = H_LEVEL;
                                
                                bflg_adg_data_ok = 0;
                            }
                        }
                    }
                }
            }
            else   //如果是读ad模块标志
            {
                if (guc_read_state == 1)   //如果是读状态寄存器命令
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            DIN_OUT_PIN = TEST(READ_STATE_ORDER, (uint8_t) (7 - guc_adg_bit));
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 2;  //读状态寄存器
                            CS_OUT_PIN = H_LEVEL;
                            DIN_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 2)    //如果是读状态寄存器
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                guc_state_buffer |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                guc_state_buffer &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 3;  //读数据寄存器命令
                            CS_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 3)    //如果是读数据寄存器命令
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            DIN_OUT_PIN = TEST(READ_DATA_ORDER, (uint8_t) (7 - guc_adg_bit));
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 4;  //读数据寄存器1
                            CS_OUT_PIN = H_LEVEL;
                            DIN_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 4)    //如果是读数据寄存器1
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                gus_data_buffer.ubyte.high |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                gus_data_buffer.ubyte.high &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 5;  //读数据寄存器2
                        }
                    }
                }
                else if (guc_read_state == 5)    //如果是读数据寄存器2
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                gus_data_buffer.ubyte.low |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                gus_data_buffer.ubyte.low &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 1;  //读数据寄存器1
                            CS_OUT_PIN = H_LEVEL;
                            
                            bflg_adg_data_ok = 0;
                        }
                    }
                }
            }
        }
        else   //如果是通道2
        {
            if (bflg_write_read_adg == 0)    //如果是写ad模块标志
            {
                if (bflg_write_config_adg == 1)   //如果是写配置ad模块
                {
                    if (guc_write_state == 1)     //如果是写配置寄存器命令
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG2_ORDER, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 2;   //写模式寄存器1
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 2)     //如果是写配置寄存器1
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG2_REG1, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 3;   //写模式寄存器2
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 3)     //如果是写配置寄存器2
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_CONFIG2_REG2, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 1;   //写模式寄存器阶段1
                                CS_OUT_PIN = H_LEVEL;
                                
                                bflg_adg_data_ok = 0;
                            }
                        }
                    }
                }
                else    //如果是写模式ad模块
                {
                    if (guc_write_state == 1)   //如果是写模式寄存器命令
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_ORDER, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 2; //写模式寄存器1
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 2)   //如果是写模式寄存器1
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_REG1, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 3; //写模式寄存器2
                                CS_OUT_PIN = H_LEVEL;
                            }
                        }
                    }
                    else if (guc_write_state == 3)   //如果是写模式寄存器2
                    {
                        if (SCLK_OUT_PIN == L_LEVEL)
                        {
                            if (guc_adg_bit < 8)
                            {
                                DIN_OUT_PIN = TEST(WRITE_MODE_REG2, (uint8_t) (7 - guc_adg_bit));
                                guc_adg_bit++;
                            }
                        }
                        else
                        {
                            if (guc_adg_bit == 8)
                            {
                                guc_adg_bit = 0;
                                guc_write_state = 1; //写模式寄存器阶段1
                                CS_OUT_PIN = H_LEVEL;
                                
                                bflg_adg_data_ok = 0;
                            }
                        }
                    }
                }
            }
            else   //如果是读ad模块标志
            {
                if (guc_read_state == 1)   //如果是读状态寄存器命令
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            DIN_OUT_PIN = TEST(READ_STATE_ORDER, (uint8_t) (7 - guc_adg_bit));
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 2;  //读状态寄存器
                            CS_OUT_PIN = H_LEVEL;
                            DIN_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 2)    //如果是读状态寄存器
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                guc_state_buffer |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                guc_state_buffer &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 3;  //读数据寄存器命令
                            CS_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 3)    //如果是读数据寄存器命令
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            DIN_OUT_PIN = TEST(READ_DATA_ORDER, (uint8_t) (7 - guc_adg_bit));
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 4;  //读数据寄存器1
                            CS_OUT_PIN = H_LEVEL;
                            DIN_OUT_PIN = H_LEVEL;
                        }
                    }
                }
                else if (guc_read_state == 4)    //如果是读数据寄存器1
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                gus_data_buffer.ubyte.high |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                gus_data_buffer.ubyte.high &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 5;  //读数据寄存器2
                        }
                    }
                }
                else if (guc_read_state == 5)    //如果是读数据寄存器2
                {
                    if (SCLK_OUT_PIN == L_LEVEL)
                    {
                        if (guc_adg_bit < 8)
                        {
                            if (DOUT_OUT_PIN == H_LEVEL)
                            {
                                gus_data_buffer.ubyte.low |= SET((uint8_t) (7 - guc_adg_bit));
                            }
                            else
                            {
                                gus_data_buffer.ubyte.low &= CLR((uint8_t) (7 - guc_adg_bit));
                            }
                            guc_adg_bit++;
                        }
                    }
                    else
                    {
                        if (guc_adg_bit == 8)
                        {
                            guc_adg_bit = 0;
                            guc_read_state = 1;  //读数据寄存器1
                            CS_OUT_PIN = H_LEVEL;
                            
                            bflg_adg_data_ok = 0;
                        }
                    }
                }
            }
        }
    }
    else
    {
        CS_OUT_PIN = H_LEVEL; //选通不使能
        SCLK_OUT_PIN = H_LEVEL;
    }
}
//------------------------------------------------------------------------------
uint8_t TEST(uint8_t luc_data, uint8_t luc_bit)
{
    luc_data >>= luc_bit;
    luc_data &= 0x01;
    
    if (luc_data == 0)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
//------------------------------------------------------------------------------
uint8_t SET(uint8_t luc_bit)
{
    uint8_t luc_data;
    
    luc_data = 0x01;
    luc_data <<= luc_bit;
    
    return luc_data;
}
//------------------------------------------------------------------------------
uint8_t CLR(uint8_t luc_bit)
{
    uint8_t luc_data;
    
    luc_data = 0x01;
    luc_data <<= luc_bit;
    luc_data = (uint8_t) (~luc_data);
    
    return luc_data;
}
//------------------------------------------------------------------------------
#endif

