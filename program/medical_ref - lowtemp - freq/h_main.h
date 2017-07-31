#ifndef _MAIN_H_
#define _MAIN_H_
//------------------------------------------------------------------------------
//外部函数声明//
extern void system_init(void);     //系统初始化程序

extern void system_start(void);    //系统启动程序

extern void main_loop(void);  //主循环程序

extern void timer_int(void);  //1ms定时中断程序

extern void adc_int(void);    //AD转换中断程序

extern void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data);    //写eeprom程序

extern int16_t eeprom_read(uint16_t lus_addr);                        //读eeprom程序

extern void init_ram_para2(void);

extern void init_ram_para(void);

extern void eeprom2_read_deal(void); //eeprom2读取处理程序，在初始化程序中调用

extern void eeprom_read_deal(void);  //eeprom读取处理程序，在初始化程序中调用


//------------------------------------------------------------------------------
//外部变量声明//
extern flag_type flg_time;
          #define   bflg_1ms_reach           flg_time.bits.bit0  //1ms计时到标志
          #define   bflg_10ms_reach          flg_time.bits.bit1  //10ms计时到标准
          #define   bflg_askfor_read_eeprom  flg_time.bits.bit2   //请求读eeprom标志
          #define   bflg_read_eeprom_error   flg_time.bits.bit3   //读eeprom错标志
          #define   bflg_eeprom_fault        flg_time.bits.bit4   //eeprom故障标志
          #define   bflg_RL_output_delaytime flg_time.bits.bit5   //继电器输出延时标志
          
          #define   bflg_test_type      flg_time.bits.bit6   //测试版显示板标志
          #define   bflg_test_mode      flg_time.bits.bit7   //测试模式标志
          #define   bflg_test_ok        flg_time.bits.bit8   //测试完成标志
          #define   bflg_test_mode_RTC  flg_time.bits.bit9   //测试模式的测试时钟电路
//------------------------------------------------------------------------------
#define   LOCK_OUT_PIN        P11.0     //电磁锁输出口                
#define   RL_ALM_OUT_PIN      P6.5      //远程报警继电器输出口      =1 是打开远程报警灯  
//----------------------------------------------------------
//新版 新增加的 三色灯
#define   BLUE_LED_PIN       P8.0      //绿色灯
#define   GREEN_LED_PIN      P8.1      //蓝色灯
#define   RED_LED_PIN        P8.2      //红色灯--用作加热模块控制pwm口

//------------------------------------------------------------------------------
#define   SOFT_VERSION        1000      //软件版本号


//------------------------------------------------------------------------------
#endif

