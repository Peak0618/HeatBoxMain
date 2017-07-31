#ifndef _KEY_DISP_H_
#define _KEY_DISP_H_
//------------------------------------------------------------------------------
//外部函数声明
extern void read_key_deal(void);   //读键值处理程序，在主循环程序中调用

extern void led_disp1_deal(void);  //数码管1显示处理程序，在100ms定时程序中调用
extern void led_lamp_disp(void);

extern void led_disp2_deal(void);  //数码管2显示处理程序，在100ms定时程序中调用

extern void led_disp3_deal(void);  //数码管3显示处理程序，在100ms定时程序中调用

extern void lamp_disp_deal(void);  //灯显示处理程序，在100ms定时程序中调用


extern void read_key_delaytime(void);   //读键延时程序，在10ms定时程序中调用

//peak add
extern void  key_buzz_cancel(void);
extern void bat_con_check_again(void);     //  长按取消报警后，当再次检测到电池，恢复正常控制； 
extern void led_loop_delaytime(void);                 //LED闪烁标志
extern void led_disp1_flicker_delaytime(void);   //设定时数据闪烁
extern void view_error_code_delaytime(void);      //显示错误代码时间 
extern void key_factory_AT_test(void);
extern void  key_factory_init(void);                  //恢复出厂值
extern void system_self_test(void);                   //初始上电时的自检
extern void card_IC_deal(void);                         
extern void lock_open_delaytimer(void);  


//------------------------------------------------------------------------------
//外部变量声明
extern flag_type flg_disp;         
          #define   bflg_key_locked          flg_disp.bits.bit0  //按键锁定标志
          #define   bflg_key_long_valid      flg_disp.bits.bit1  //长按键有效标志
          #define   bflg_key_long_delaytime  flg_disp.bits.bit2  //长按键延时标志
          
          #define   bflg_disp1_val           flg_disp.bits.bit3  //数值显示有效标志
          #define   bflg_disp_minus          flg_disp.bits.bit4  //显示负数标志
          
          #define   bflg_read_key_delaytime  flg_disp.bits.bit5  //读键值延时标志
          
          #define   bflg_eep_init            flg_disp.bits.bit6  //EEP初始化标志       
          //peak  add
		  #define   bflg_key_long_buz_delaytime     flg_disp.bits.bit7   //长按 "蜂鸣取消" 按键延时标志 -在电池未连接时按下buzz键计时
		  #define   bflg_key_long_buz_valid         flg_disp.bits.bit8   // 长按 "蜂鸣取消" 按键有效标志
		  #define   flag_view_error_code            flg_disp.bits.bit9   // 显示错误代码的有效标志
		  #define   bflg_led_loop_set               flg_disp.bits.bit10  // led 闪烁的标志
		  #define   bflg_error_code_delaytime       flg_disp.bits.bit11  // 错误代码显示时间长度
		  #define   bflg_key_long_buz_valid_ok      flg_disp.bits.bit12  // 长按 5s 执行标记,是否解锁标志
		  #define   bflg_IC_register_state          flg_disp.bits.bit13  //是否进入注册状态
          #define   bflg_IC_erasure                 flg_disp.bits.bit14  //注销所有的IC卡，并没有发注销的命令，显示板判断BUZ按下5s后会自动注销，主板只是达到5s显示温度
          #define   bflg_IC_erasure_delaytime       flg_disp.bits.bit15
          
extern flag_type flg_disp1;   
        #define   bflg_disp1_flicker               flg_disp1.bits.bit0  //闪烁标记
        #define   bflg_set2_val                    flg_disp1.bits.bit1  //在user2模式中的标志
        #define   bflg_factory_val                 flg_disp1.bits.bit2  //在factory工厂模式中是否按set标志
        #define   bflg_IC_pass_CL1                 flg_disp1.bits.bit3  //清除IC注销的参数标志
        #define   bflg_factory_AT_test             flg_disp1.bits.bit4  //在factory工厂模式中AT指令要执行的标志；
        #define   bflg_factory_CL4                 flg_disp1.bits.bit5  //在factory工厂模式中CL4指令要执行的标志--恢复出厂设置
        #define   bflg_IC_erasure_order            flg_disp1.bits.bit6  //指令CL1、CL3的注销所有已经注册的IC卡指令标志
        #define   bflg_key_long_down_valid         flg_disp1.bits.bit7  //锁定状态时长按down键是否达到5s的标志
        #define   bflg_key_long_down_delaytime     flg_disp1.bits.bit8  //锁定状态按下down键，开始计时标志；
        #define   bflg_debug_val                   flg_disp1.bits.bit9  //海尔调试模式下显示参数值还是参数
        #define   bflg_prepare_deal                flg_disp1.bits.bit10 //长按set键解锁后，需要松开一下set键才可以继续操作；         
        #define   bflg_set1_val                    flg_disp1.bits.bit11 //在user1模式中的标志

extern flag_type flg_error_code_mark;   //错误代码已读标记
         #define   bflg_error_code_alarm_ALH_mark         flg_error_code_mark.bits.bit0   // 1
		 #define   bflg_error_code_alarm_ALL_mark         flg_error_code_mark.bits.bit1   // 2
         #define   bflg_error_code_alarm_power_fault_mark flg_error_code_mark.bits.bit2   // 3
         #define   bflg_error_code_alarm_TTEST_fault_mark flg_error_code_mark.bits.bit3   // 4
		 #define   bflg_error_code_alarm_door_open_mark   flg_error_code_mark.bits.bit4   // 5
         //*****************can't add,use'd .uword****************************************

extern flag_type   flg_save1;
        #define   bflg_TS_save                           flg_save1.bits.bit0   //TS参数可以保存的标志
        #define   bflg_ALH_save                          flg_save1.bits.bit1   
        #define   bflg_ALL_save                          flg_save1.bits.bit2
		 //------------------------------------------------------------------------------------------
        #define   bflg_user2_dA_save                     flg_save1.bits.bit3
        #define   bflg_user2_T1_save                     flg_save1.bits.bit4
        #define   bflg_user2_T2_save                     flg_save1.bits.bit5
        #define   bflg_user2_IC_save                     flg_save1.bits.bit6
        #define   bflg_user2_CL1_save                    flg_save1.bits.bit7
        //--------------------------------------------------------------------------------------------
        #define   bflg_factory_CL2_save                  flg_save1.bits.bit8
        #define   bflg_factory_Ct_save                   flg_save1.bits.bit9
        #define   bflg_factory_CU_save                   flg_save1.bits.bit10
        #define   bflg_factory_CA_save                   flg_save1.bits.bit11
        #define   bflg_factory_Fd_save                   flg_save1.bits.bit12
        #define   bflg_factory_AT_save                   flg_save1.bits.bit13
        #define   bflg_factory_SS_save                   flg_save1.bits.bit14
        #define   bflg_factory_AA_save                   flg_save1.bits.bit15
        
extern flag_type   flg_save2;
        #define   bflg_factory_Cd_save                   flg_save2.bits.bit0
        #define   bflg_factory_CL3_save                  flg_save2.bits.bit1
        #define   bflg_factory_CL4_save                  flg_save2.bits.bit2
        #define   bflg_factory_PS_save                   flg_save2.bits.bit3
        #define   bflg_factory_bS_save                   flg_save2.bits.bit4

        #define   bflg_self_test                         flg_save2.bits.bit5   //系统自检标志
        #define   bflg_self_test_flicker                 flg_save2.bits.bit6   //系统自检闪烁标志
        #define   bflg_self_test_sound                   flg_save2.bits.bit7   //系统自检响一声
        #define   bflg_user2_P6_save                     flg_save2.bits.bit8   //用户模式usb导出数据的模式
        //血液
        #define   bflg_t1_save                           flg_save2.bits.bit9    
        #define   bflg_r0_save                           flg_save2.bits.bit10   
        #define   bflg_r00_save                          flg_save2.bits.bit11    
        #define   bflg_s1_save                           flg_save2.bits.bit12   
        #define   bflg_s2_save                           flg_save2.bits.bit13   
//------------------------------------------------------------------------------
extern uint8_t guc_key_val;   //按键键值
extern uint8_t guc_led_buffer[12]; //数码管寄存器

#define   LED1      guc_led_buffer[0]
#define   LED2      guc_led_buffer[1]
#define   LED3      guc_led_buffer[2]
#define   LED4      guc_led_buffer[3]
#define   LED5      guc_led_buffer[4]
#define   LED6      guc_led_buffer[5]
#define   LED7      guc_led_buffer[6]
#define   LED8      guc_led_buffer[7]   //lamp
#define   LED9      guc_led_buffer[8]
#define   LED10     guc_led_buffer[9]
#define   LED11     guc_led_buffer[10]
#define   LED12     guc_led_buffer[11]  //lamp


//------------------------------------------------------------------------------
extern int16_t   gss_disp_see_buffer[6];



extern int16_t   gss_read_key_delaytimer; //读键值延时计时器

//------------------------------------------------------------------------------
#endif


