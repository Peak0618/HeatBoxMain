#ifndef _COM_H_
#define _COM_H_
//------------------------------------------------------------------------------
//外部函数声明
extern void com_init(void);   //通讯初始化程序，在初始化程序中调用

extern void com_rx_init(void);     //通讯接收初始化程序，在主循环程序中调用

extern void com_rx_data_deal(void);     //通讯接收数据处理程序，在主循环程序中调用

extern void com_tx_init(void);     //通讯发送数据处理程序，在主循环程序中调用

extern void com_rx_delaytime(void);     //通讯接收延时程序，在1ms定时程序中调用

extern void com_tx_delaytime(void);     //通讯发送延时程序，在1ms定时程序中调用

extern void com_rx_end_delaytime(void); //通讯接收结束延时程序，在1ms定时程序中调用

extern void com_fault_delaytime(void);  //通讯故障延时程序，在1s定时程序中调用

extern void com_tx_int(void); //通讯发送中断程序

extern void com_rx_int(uint8_t rx_data);//通讯接收中断程序



//------------------------------------------------------------------------------
//外部变量声明//
extern flag_type flg_com;

          #define   bflg_com_tx_delaytime    flg_com.bits.bit0   //通讯发送延时标志
          #define   bflg_com_rx_delaytime    flg_com.bits.bit1   //通讯接收延时标志
          #define   bflg_com_allow_tx        flg_com.bits.bit2   //通讯允许发送标志
          #define   bflg_com_allow_rx        flg_com.bits.bit3   //通讯允许接收标志
          #define   bflg_com_tx_busy         flg_com.bits.bit4   //通讯发送忙标志
          #define   bflg_com_rx_busy         flg_com.bits.bit5   //通讯接收忙标志
          #define   bflg_com_tx_ok           flg_com.bits.bit6   //通讯发送成功标志
          #define   bflg_com_rx_ok           flg_com.bits.bit7   //通讯接收成功标志
          #define   bflg_com_rx_end          flg_com.bits.bit8   //通讯接收结束标志
          
          #define   bflg_com_fault           flg_com.bits.bit9   //通讯故障标志
          
          #define   bflg_com_type            flg_com.bits.bit10  //通讯类型标志     //peak  0:LED  1:液晶
          #define   bflg_com_type_ok         flg_com.bits.bit11  //通讯类型判断ok标志

          //peak add
          #define   bflg_lock_delaytimer     flg_com.bits.bit12  // 开锁的延时标志
//------------------------------------------------------------------------------
extern flag_type flg_read_write;
          #define   bflg_read_write_data     flg_read_write.bits.bit0      //读写数据标志
          #define   bflg_read_user_admin     flg_read_write.bits.bit1      //读用户管理员标志

//------------------------------------------------------------------------------
//外部变量 
extern uint8_t  guc_IC_access_state;               // 开锁的状态
extern uint16_t guc_flash_add;                     // 接收的IC卡注册已用的地址
extern uint16_t guc_temp_flash_add;                // 接收的IC卡注册临时保存的地址
extern uint8_t  guc_IC_erasure_tx_ok_count;        // 注销IC卡指令，在bflg_com_tx_ok中设置发送多次计数,防止丢包
extern int16_t  gss_liquid_led_button;              //液晶屏的开灯按钮  ：1开 0关
extern int16_t  gss_liquid_ozone_disinfect_button;  //液晶屏的启动杀菌按钮：1开 0关          
//------------------------------------------------------------------------------
#endif
