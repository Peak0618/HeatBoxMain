#ifndef _M_PTC_HEAT_H_
#define _M_PTC_HEAT_H_

//外部函数
extern void PTC_heat_power_on_delaytime(void);
extern void PTC_heat_control_deal(void);
extern void PTC_heat_fre_mode_judge(void);
extern void PTC_heat_RL_out(void);
extern void PTC_fre_RL_deal(void);




//------------------------------------------------------------------------------------------
//外部标志
extern flag_type bflg_PTC_heat; 
       #define   bflg_PTC_heat_power_on_delaytime  bflg_PTC_heat.bits.bit0  //PTC上电延时
       #define   bflg_PTC_heat_on                  bflg_PTC_heat.bits.bit1  //PTC加热运行标志 
       #define   bflg_pwm_level                    bflg_PTC_heat.bits.bit2  //PTC模块输出的pwm电平高低标志   
       #define   bflg_PTC_fre_control              bflg_PTC_heat.bits.bit3  //PTC是否是频率控制
       #define   bflg_PTC_alway_out                bflg_PTC_heat.bits.bit4  //一直输出,为了不一直打开或关闭
       #define   bflg_PTC_fre_out                  bflg_PTC_heat.bits.bit5  //频率输出，。。。。
       #define   bflg_PTC_no_out                   bflg_PTC_heat.bits.bit6  //不输出， 。。。。。

//------------------------------------------------------------------------------------------
//外部变量声明
extern uint8_t guc_PTC_heat_power_on_delaytime;   //PTC初始上电延时



#endif
/**********************************END OF THE FILE*************************************************/
