#ifndef _M_KEY_OPERATION_H_
#define _M_KEY_OPERATION_H_

//外部函数
extern void mode_selection(void);
extern void key_locked_delaytime(void);   
extern void key_set_long_delaytime(void);
extern void key_set_up_long_delaytime(void);
extern void key_up_down_long_delaytime(void);
extern void key_set_down_long_delaytime(void);
extern void key_parm_default_val(void);



//按键值
#define   KEY_NONE              0x00     //无按键按下
#define   KEY_SET               0x08     //设定
#define   KEY_UP                0x10     //上调
#define   KEY_DOWN              0x01     //下调
#define   KEY_BUZ               0x02     //报警测试
#define   KEY_LAMP              0x04     //灯
#define   KEY_SET_AND_UP        0x18     //设定+上调  工厂
#define   KEY_UP_AND_DOWN       0x11     //上调+下调  查询
#define   KEY_SET_AND_DOWN      0x09     // IC密码



//主干_层
extern uint8_t   guc_disp_mode;
#define   DISP_LOCKED                  0   //锁定模式
#define   DISP_USER_SET                1   //用户设定层
#define   DISP_USB_TIME_SET            2   //usb时间设置
#define   DISP_USER_MANAGE_PASSWORD    3   //用户管理层密码
#define   DISP_USER_MANAGE             4   //用户管理层
#define   DISP_FACTORY_PASSWORD        5   //工厂密码模式
#define   DISP_FACTORY                 6   //工厂模式
#define   DISP_INQUIRE                 7   //查询模式
#define   DISP_IC_PASSWORD             8   //IC卡密码模式
#define   DISP_IC_REGISTER             9   //IC卡注册模式

extern int8_t   gsc_disp_parm_type;
//分支_用户设定层参数
#define   DISP_USER_SET_TS                  0   //箱内温度设定值            magnify 10 使用时不需放大10
#define   DISP_USER_SET_ALH                 1   //高温报警值              magnify 10
#define   DISP_USER_SET_ALL                 2   //低温报警值              magnify 10
#define   DISP_USER_SET_Lt                  3   //箱内灯延时关闭时间 or 取消延时
#define   DISP_USER_SET_dA                  4   //开门蜂鸣器报警延时
#define   DISP_USER_SET_S1                  5   //杀菌模式
#define   DISP_USER_SET_S2                  6   //最大杀菌时间
#define   DISP_USER_SET_TT                  7   //温度单位
#define   DISP_USER_SET_SUM                 8   //用户设定层参数总个数

//分支_用户管理层参数
#define   DISP_USER_MANAGE_IC               0  //ic卡注册/用户管理层密码修改
#define   DISP_USER_MANAGE_CL1              1  //ic卡注销
#define   DISP_USER_MANAGE_Lt               2  //箱内照明灯延时关闭时间or取消关闭延时
#define   DISP_USER_MANAGE_dA               3  //开门蜂鸣器报警延时
#define   DISP_USER_MANAGE_FA               4  //蜂鸣器音量
#define   DISP_USER_MANAGE_S1               5  //杀菌模式
#define   DISP_USER_MANAGE_S2               6  //最大杀菌时间
#define   DISP_USER_MANAGE_TT               7  //温度单位
#define   DISP_USER_MANAGE_SUM              8  //管理层参数总数

//分支_工厂参数
#define  DISP_FAC_r0                        0   //控制温差                magnify 10
#define  DISP_FAC_r1                        1   //档位切换温差
#define  DISP_FAC_Ct1                       2   //测试传感器校准值            magnify 10
#define  DISP_FAC_Ct2                       3   //显示传感器校准值            magnify 10
#define  DISP_FAC_Cd                        4   //初始上电PTC开启延时
#define  DISP_FAC_T1                        5   //故障时PCT开机时间
#define  DISP_FAC_T2                        6   //故障时PCT停机时间
#define  DISP_FAC_Fd                        7   //蜂鸣器取消后再启动间隔时间-使用需*10
#define  DISP_FAC_Ct3                       8   //市电电压校准值----因后期加，LED屏的没有用，只在液晶屏用
#define  DISP_FAC_SUM                       9   //工厂层参数总数


//分支_查询参数
extern int8_t  gsc_disp_inq_mode;
#define  DISP_INQ_d1                        0   //上传感器
#define  DISP_INQ_d2                        1   //下传感器
#define  DISP_INQ_d3                        2   //控制传感器
#define  DISP_INQ_d4                        3   //化霜传感器
#define  DISP_INQ_d5                        4   //冷凝传感器
#define  DISP_INQ_d6                        5   //环温传感器
#define  DISP_INQ_d7                        6   //环境湿度
#define  DISP_INQ_d8                        7   //市电电压
#define  DISP_INQ_d9                        8   //电池电量



//外部标志
extern flag_type bflg_key_operation;
       #define   bflg_key_set_long_valid          bflg_key_operation.bits.bit0   //长按set键有效
       #define   bflg_key_set_long_delaytime      bflg_key_operation.bits.bit1   //长按set键计时
       #define   bflg_key_set_up_long_valid       bflg_key_operation.bits.bit2   //长按set+up键有效
       #define   bflg_key_set_up_long_delaytime   bflg_key_operation.bits.bit3   //长按set+up键计时
       #define   bflg_key_up_down_long_valid      bflg_key_operation.bits.bit4   //长按up+down键有效
       #define   bflg_key_up_down_long_delaytime  bflg_key_operation.bits.bit5   //长按up+down键计时
       #define   bflg_key_set_down_long_valid     bflg_key_operation.bits.bit6   //长按set+down键有效
       #define   bflg_key_set_down_long_delaytime bflg_key_operation.bits.bit7   //长按set+down键计时
       #define   bflg_parm_dis_form                bflg_key_operation.bits.bit8  //显示参数标识 or 参数值
       
       #define   bflg_key_inq_val                 bflg_key_operation.bits.bit10  //查询层进入参数值
       
extern flag_type bflg_key_operation_save;
       #define   bflg_parm_save              bflg_key_operation_save.bits.bit0   //是否对参数保存



//变量定义
extern uint8_t   guc_key_val;                      //按键键值
extern uint8_t   guc_TUTD_dis_type;                //选择显示上温 或下温或上下温平均值
extern uint8_t   guc_key_set_long_delaytimer;      //长按set键计时
extern uint8_t   guc_key_set_up_long_delaytimer;   //长按set+up键计时
extern uint8_t   guc_key_up_down_long_delaytimer;  //长按up+down键计时
extern uint8_t   guc_key_set_down_long_delaytimer; //长按set+down键计时
extern uint8_t   guc_key_locked_delaytimer;        //按键无操作锁定延时
extern uint8_t   guc_disp_mode;                    //总的UI模式

extern int8_t    gsc_user_manage_password_val;     //用户管理密码值
extern int8_t    gsc_factory_password_val;         //工厂密码值
extern int8_t    gsc_ic_password_val;              //ic卡密码值
extern int8_t    gsc_disp_parm_type;               //user设定层的不同参数
extern int8_t    gsc_disp_inq_mode;                //查询模式中不同参数

extern int16_t   gss_key_temp_value;               //操作时的临时值

extern int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];       
extern int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM]; 
extern int16_t   gss_factory_parm_value[DISP_FAC_SUM]; 













#endif
/*********************************************END OF THE FILE********************************************************/

