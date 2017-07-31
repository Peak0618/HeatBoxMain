#ifndef _BAT_CTRL_H_
#define _BAT_CTRL_H_
//------------------------------------------------------------------------------
//外部函数声明
extern void bat_charge_deal(void);      //电池充电处理程序，在主循环程序中调用

extern void bat_charge_delaytime(void); //电池充电延时程序，在1s定时程序中调用

extern void bat_charge_continue_delaytime(void);  //充电程序延时程序，在1min定时程序中调用

extern void bat_charge_space_delaytime(void);//电池充电间隔延时程序，在1min定时程序中调用

extern void bat_discharge_deal(void);   //电池放电处理程序，在主循环程序中调用


//------------------------------------------------------------------------------
//外部变量声明
extern flag_type flg_bat;

          #define   bflg_bat_type            flg_bat.bits.bit0   //电池类型标志  peak 0:铅酸 1:锂电
          #define   bflg_bat_charge_update   flg_bat.bits.bit1   //电池充电更新标志
          #define   bflg_bat_frist_charge    flg_bat.bits.bit2   //电池首次充电标志
          #define   bflg_bat_charging        flg_bat.bits.bit3   //电池充电标志        peak 0:不允许充电 1:允许充电
          #define   bflg_bat_charging_end    flg_bat.bits.bit4   //电池充电结束阶段标志
          
          #define   bflg_bat_charge_space_delaytime    flg_bat.bits.bit5   //电池充电间隔延时标志 //peak 充完电间隔30天再充电(充电的一个条件)
          
          #define   bflg_bat_discharging     flg_bat.bits.bit6   //电池放电标志  0:不放电  1:放电 初始化时为1；
//------------------------------------------------------------------------------
extern int16_t gss_bat_pwm_value;  //电池充电pwm占空比值
extern int16_t gss_bat_pwm_cnt;    //电池充电pwm占空比计数器

//------------------------------------------------------------------------------
#endif
