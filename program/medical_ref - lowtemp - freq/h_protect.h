#ifndef _PROTECT_H_
#define _PROTECT_H_
//------------------------------------------------------------------------------
//外部函数声明

extern void prot2_TA_first_delaytime(void);  //主传感器首次高低温保护延时程序，在1min定时程序中调用

extern void heat_box_TDIS_high_delaytime(void);   //主传感器高温保护延时程序，在1s定时程序中调用

extern void heat_box_TDIS_low_delaytime(void);    //主传感器低温保护延时程序，在1s定时程序中调用

extern void prot2_power_off_deal(void);      //断电保护处理程序，在主循环程序中调用

extern void port2_power_fault_deal(void);    //电源板故障处理程序，在主循环程序中调用

extern void prot2_bat_discnnt_delaytime(void);    //电池未连接保护延时程序，在100ms定时程序中调用

extern void prot2_bat_space_delaytime(void);      //电池未连接检测间隔延时程序，在100ms定时程序中调用

extern void prot2_bat_low_deal(void);   //电池电量低保护处理程序，在主循环程序中调用

extern void prot2_bat_low_delaytime(void);   //电池电量低延迟程序，在1min定时程序中调用

extern void port2_door_delaytime(void);      //门开关保护延时程序，在1s定时程序中调用

extern void port2_T_fault_deal(void);        //传感器故障处理程序，在主循环程序中调用

extern void prot2_THW_high_delaytime(void);  //环温传感器过高保护延时程序，在1s定时程序中调用

extern void prot2_TC_first_delaytime(void);  //冷凝器脏首次保护延时程序，在1min定时程序中调用

extern void prot2_cond_dirty_delaytime(void);//冷凝器脏保护延时程序，在1s定时程序中调用

extern void prot2_volt_deal(void);      //电压超标报警保护处理程序，在主循环程序中调用

extern void alarm_lamp_deal(void);      //报警灯处理程序，在主循环程序中调用

extern void alarm_buzz_deal(void);      //蜂鸣器报警处理程序，在主循环程序中调用

extern void alarm_buzz_off_delaytime(void);       //蜂鸣器报警取消延时程序，在1min定时程序中调用

extern void bat_T_hight_protect(void);               //电池温度高判断程序


//------------------------------------------------------------------------------
//外部标志声明
extern flag_type flg_alarm;                  
          #define   bflg_alarm_ALH           flg_alarm.bits.bit0    //高温报警标志        
          #define   bflg_alarm_ALL           flg_alarm.bits.bit1    //低温报警标志         
          #define   bflg_alarm_power_fault   flg_alarm.bits.bit2    //电源板故障报警标志   
          #define   bflg_TTEST_fault         flg_alarm.bits.bit3    //测试传感器故障
          #define   bflg_alarm_open_door     flg_alarm.bits.bit4    //开门报警标志       
          //不能在添加其他位了，因为使用了 flg_alarm.uword
          //--xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

          
//------------------------------------------------------------------------------
 extern flag_type flg_alarm1;                                         
         #define   bflg_alarm_bat_fed      flg_alarm1.bits.bit0   //电池接反报警标志    长响
         #define   bflg_alarm_bat_low      flg_alarm1.bits.bit1   //电池电量低报警标志  不响
         #define   bflg_alarm_THW_high     flg_alarm1.bits.bit2   //环温过高报警标志    不响
         #define   bflg_alarm_power_off    flg_alarm1.bits.bit3  //断电报警标志         
         #define   bflg_alarm_bat_discnnt  flg_alarm1.bits.bit4  //电池未连接报警标志   
         #define   bflg_alarm_T_fault	   flg_alarm1.bits.bit5  //传感器故障报警标志: 主、环温、冷凝、热交换这4种传感器任意一种有故障这个就有故障
         #define   bflg_alarm_cond_dirty   flg_alarm1.bits.bit6  //冷凝器脏报警标志	          


         
//------------------------------------------------------------------------------
extern flag_type flg_alarm2;  
        #define   bflg_alarm_volt          flg_alarm2.bits.bit0       //电压超标报警标志  
        #define   bflg_alarm_bat1_T_hight  flg_alarm2.bits.bit1       //电池1温度过高       
        #define   bflg_alarm_bat2_T_hight  flg_alarm2.bits.bit2       //电池2温度过高


//------------------------------------------------------------------------------
extern flag_type flg_prot2;
          
          #define   bflg_alarm_lamp          flg_prot2.bits.bit0 //报警灯标志
          #define   bflg_alarm_buzz          flg_prot2.bits.bit1 //蜂鸣器报警标志
          #define   bflg_alarm_buzz_output   flg_prot2.bits.bit2 //蜂鸣器输出标志
          #define   bflg_alarm_output        flg_prot2.bits.bit3 //远程报警输出标志
          
          #define   bflg_prot2_door1_pin     flg_prot2.bits.bit4 //门开关1引脚状态标志
          #define   bflg_prot2_door2_pin     flg_prot2.bits.bit5 //门开关2引脚状态标志
          #define   bflg_prot2_bat_fed_pin   flg_prot2.bits.bit6 //锂电池防插反标志
          
          #define   bflg_prot2_TA_first_delaytime flg_prot2.bits.bit7 //主传感器首次保护延时标志
          #define   bflg_prot2_TC_first_delaytime flg_prot2.bits.bit8 //冷凝器脏首次保护延时标志
          
          #define   bflg_alarm_buzz_off_delaytime flg_prot2.bits.bit9 //蜂鸣器报警取消延时标志  =1表示已按下"蜂鸣取消"键可30min内不响
          
          #define   bflg_prot2_THW_high_disp      flg_prot2.bits.bit10//环温过高温度显示标志
          
          #define   bflg_prot2_bat_low_delaytime         flg_prot2.bits.bit11   //电池电量低延时标志    //peak 初始上电对电池充电24小时；
          #define   bflg_prot2_bat_space_delaytime       flg_prot2.bits.bit12   //电池未连接检测间隔延时标志
          #define   bflg_prot2_bat_space_reach           flg_prot2.bits.bit13   //电池未连接检测间隔延时到标志

		  /****peak add*********/
          #define   bflg_alarm_bat_low_led              flg_prot2.bits.bit14     // 检测到电池电量低时，指示灯只是闪烁，不蜂鸣；


//macro
//gus_trip_code1
#define   H_T_CODE       0x0001   //bit0  高温报警
#define   L_T_CODE       0x0002   //bit1  低温报警
#define   TTSET_S_CODE   0x0004   //bit2  测试传感器故障报警 S = sensor
#define   TDIS_S_CODE    0x0008   //bit3  显示传感器故障报警
#define   DOOR_OP_CODE   0x0010   //bit4  开门报警
#define   DC_F_CODE      0x0020   //bit5  开关电源故障报警
//#define   VAC_OFF_CODE   0x0040   //bit6  断电报警--无电池

//gus_trip_code2
//#define   AIR_L_CODE     0x0001   //bit0  空气低温报警

//------------------------------------------------------------------------------
#define   THW_FAULT_CODE      1
#define   TF_FAULT_CODE       2
#define   TC_FAULT_CODE       3
#define   TD_FAULT_CODE       4
#define   TE_FAULT_CODE       5
#define   TA_FAULT_CODE       6
//------------------------------------------------------------------------------
extern int16_t   gss_prot2_TA_first_delaytimer;   //主传感器首次保护延时计时器
extern int16_t   gss_prot2_TC_first_delaytimer;   //冷凝器脏首次保护延时计时器

extern int16_t   gss_prot2_bat_space_delaytimer;  //电池未连接检测间隔延时计时器

extern int16_t   gss_prot2_bat_low_delaytimer;    //电池电量低延时计时器

extern int16_t   gss_alarm_buzz_off_delaytimer;   //蜂鸣器报警取消延时计时器

extern uint16_t  gus_trip_code1;   //故障代码1
extern uint16_t  gus_trip_code2;   //故障代码2




//------------------------------------------------------------------------------
#endif
