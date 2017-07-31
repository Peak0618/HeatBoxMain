#ifndef _M_PERIPHERAL_CONTROL_H_
#define _M_PERIPHERAL_CONTROL_H_


//函数声明
extern void fan_control_deal(void);
extern void fan_RL_out(void);
extern void led_control_deal(void);
extern void led_key_on_delaytime(void);
extern void led_RL_out(void);
extern void germicidal_lamp_control_deal(void);
extern void germicidal_lamp_delaytime(void);
extern void germicidal_lamp_RL_out(void);
extern void remote_RL_alarm_lamp(void);



//外部标志
extern flag_type bflg_peripheral_control; 
       #define   bflg_fan_on                       bflg_peripheral_control.bits.bit0  //风机开关标志
       #define   bflg_germicidal_lamp_on           bflg_peripheral_control.bits.bit1  //杀菌灯开关标志
       #define   bflg_led_on                       bflg_peripheral_control.bits.bit2  //led灯开关标志
       #define   bflg_led_on_delaytime             bflg_peripheral_control.bits.bit3  //按键开了的灯延时
       #define   bflg_germicidal_lamp_on_delaytime bflg_peripheral_control.bits.bit4  //杀菌灯开延时


//----------------------------------------------------------
//继电器输出口宏定义
#define   RL_COMP1_OUT_PIN    P7.4      //压缩机1继电器输出口     
#define   RL_COMP2_OUT_PIN    P7.5      //压缩机2继电器输出口      
       
#define   RL_FAN1_OUT_PIN     P7.3      //风机1继电器输出口           RL7  bflg_outfan20_running
#define   RL_FAN2_OUT_PIN     P4.2      //风机2继电器输出口         
#define   RL_HTRMXG_OUT_PIN   P4.1      //毛细管加热丝继电器输出口  
#define   RL_HTRGK_OUT_PIN    P13.0     //柜口加热丝继电器输出口    
#define   RL_DOWN_OUT_PIN     P15.6     //降压继电器输出口          
#define   RL_UP_OUT_PIN       P10.0     //升压继电器输出口          
       //----------------------------------------------------------
       //12V输出口宏定义
#define   HTRMT_OUT_PIN       P11.1     //门体加热丝输出口   
#define   HTRMXG_OUT_PIN      P14.6     //毛细管加热丝输出口 
#define   HTRPHK_OUT_PIN      P14.7     //平衡口加热丝输出口 
       //----------------------------------------------------------
#define   TRIP_LAMP_PIN       P7.7      //故障灯输出口       
#define   BUZZ_OUT_PIN        P3.1      //蜂鸣器输出口       
       //----------------------------------------------------------
#define   TFT_OUT_PIN         P10.1     //液晶屏电源输出口   
       //----------------------------------------------------------
#define   RL_CON_PIN          P1.5      //电池充电控制输出口    // peak 控制电池的充电脚
#define   BAT_PWM_OUT_PIN     P1.6      //电池充电PWM输出口   
#define   BAT_DISCHARGE_PIN   P1.7      //电池供电输出口     
       //----------------------------------------------------------
       //开关输入口宏定义
#define   DOOR1_IN_PIN        P5.2      //门开关1输入口  
       
#define   BAT_FED_IN_PIN      P0.5      //电池插反输入口  
       //----------------------------------------------------------
       //拨码开关输入口宏定义
#define   SW11_IN_PIN         P5.1      //拨码开关输入口
#define   SW12_IN_PIN         P5.0      //拨码开关输入口
#define   SW13_IN_PIN         P3.0      //拨码开关输入口
#define   SW14_IN_PIN         P8.7      //拨码开关输入口
#define   SW15_IN_PIN         P8.6      //拨码开关输入口
#define   SW16_IN_PIN         P8.5      //拨码开关输入口
#define   SW17_IN_PIN         P8.4      //拨码开关输入口
#define   SW18_IN_PIN         P8.3      //拨码开关输入口
       
#define   SW21_IN_PIN         P0.6      //拨码开关输入口
#define   SW22_IN_PIN         P7.0      //拨码开关输入口
#define   SW23_IN_PIN         P7.1      //拨码开关输入口
#define   SW24_IN_PIN         P7.2      //拨码开关输入口
       










#endif 
