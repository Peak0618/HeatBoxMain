#ifndef _KEY_DISP_H_
#define _KEY_DISP_H_
//------------------------------------------------------------------------------
//�ⲿ��������
extern void read_key_deal(void);   //����ֵ�����������ѭ�������е���

extern void led_disp1_deal(void);  //�����1��ʾ���������100ms��ʱ�����е���
extern void led_lamp_disp(void);

extern void led_disp2_deal(void);  //�����2��ʾ���������100ms��ʱ�����е���

extern void led_disp3_deal(void);  //�����3��ʾ���������100ms��ʱ�����е���

extern void lamp_disp_deal(void);  //����ʾ���������100ms��ʱ�����е���


extern void read_key_delaytime(void);   //������ʱ������10ms��ʱ�����е���

//peak add
extern void  key_buzz_cancel(void);
extern void bat_con_check_again(void);     //  ����ȡ�������󣬵��ٴμ�⵽��أ��ָ��������ƣ� 
extern void led_loop_delaytime(void);                 //LED��˸��־
extern void led_disp1_flicker_delaytime(void);   //�趨ʱ������˸
extern void view_error_code_delaytime(void);      //��ʾ�������ʱ�� 
extern void key_factory_AT_test(void);
extern void  key_factory_init(void);                  //�ָ�����ֵ
extern void system_self_test(void);                   //��ʼ�ϵ�ʱ���Լ�
extern void card_IC_deal(void);                         
extern void lock_open_delaytimer(void);  


//------------------------------------------------------------------------------
//�ⲿ��������
extern flag_type flg_disp;         
          #define   bflg_key_locked          flg_disp.bits.bit0  //����������־
          #define   bflg_key_long_valid      flg_disp.bits.bit1  //��������Ч��־
          #define   bflg_key_long_delaytime  flg_disp.bits.bit2  //��������ʱ��־
          
          #define   bflg_disp1_val           flg_disp.bits.bit3  //��ֵ��ʾ��Ч��־
          #define   bflg_disp_minus          flg_disp.bits.bit4  //��ʾ������־
          
          #define   bflg_read_key_delaytime  flg_disp.bits.bit5  //����ֵ��ʱ��־
          
          #define   bflg_eep_init            flg_disp.bits.bit6  //EEP��ʼ����־       
          //peak  add
		  #define   bflg_key_long_buz_delaytime     flg_disp.bits.bit7   //���� "����ȡ��" ������ʱ��־ -�ڵ��δ����ʱ����buzz����ʱ
		  #define   bflg_key_long_buz_valid         flg_disp.bits.bit8   // ���� "����ȡ��" ������Ч��־
		  #define   flag_view_error_code            flg_disp.bits.bit9   // ��ʾ����������Ч��־
		  #define   bflg_led_loop_set               flg_disp.bits.bit10  // led ��˸�ı�־
		  #define   bflg_error_code_delaytime       flg_disp.bits.bit11  // ���������ʾʱ�䳤��
		  #define   bflg_key_long_buz_valid_ok      flg_disp.bits.bit12  // ���� 5s ִ�б��,�Ƿ������־
		  #define   bflg_IC_register_state          flg_disp.bits.bit13  //�Ƿ����ע��״̬
          #define   bflg_IC_erasure                 flg_disp.bits.bit14  //ע�����е�IC������û�з�ע���������ʾ���ж�BUZ����5s����Զ�ע��������ֻ�Ǵﵽ5s��ʾ�¶�
          #define   bflg_IC_erasure_delaytime       flg_disp.bits.bit15
          
extern flag_type flg_disp1;   
        #define   bflg_disp1_flicker               flg_disp1.bits.bit0  //��˸���
        #define   bflg_set2_val                    flg_disp1.bits.bit1  //��user2ģʽ�еı�־
        #define   bflg_factory_val                 flg_disp1.bits.bit2  //��factory����ģʽ���Ƿ�set��־
        #define   bflg_IC_pass_CL1                 flg_disp1.bits.bit3  //���ICע���Ĳ�����־
        #define   bflg_factory_AT_test             flg_disp1.bits.bit4  //��factory����ģʽ��ATָ��Ҫִ�еı�־��
        #define   bflg_factory_CL4                 flg_disp1.bits.bit5  //��factory����ģʽ��CL4ָ��Ҫִ�еı�־--�ָ���������
        #define   bflg_IC_erasure_order            flg_disp1.bits.bit6  //ָ��CL1��CL3��ע�������Ѿ�ע���IC��ָ���־
        #define   bflg_key_long_down_valid         flg_disp1.bits.bit7  //����״̬ʱ����down���Ƿ�ﵽ5s�ı�־
        #define   bflg_key_long_down_delaytime     flg_disp1.bits.bit8  //����״̬����down������ʼ��ʱ��־��
        #define   bflg_debug_val                   flg_disp1.bits.bit9  //��������ģʽ����ʾ����ֵ���ǲ���
        #define   bflg_prepare_deal                flg_disp1.bits.bit10 //����set����������Ҫ�ɿ�һ��set���ſ��Լ���������         
        #define   bflg_set1_val                    flg_disp1.bits.bit11 //��user1ģʽ�еı�־

extern flag_type flg_error_code_mark;   //��������Ѷ����
         #define   bflg_error_code_alarm_ALH_mark         flg_error_code_mark.bits.bit0   // 1
		 #define   bflg_error_code_alarm_ALL_mark         flg_error_code_mark.bits.bit1   // 2
         #define   bflg_error_code_alarm_power_fault_mark flg_error_code_mark.bits.bit2   // 3
         #define   bflg_error_code_alarm_TTEST_fault_mark flg_error_code_mark.bits.bit3   // 4
		 #define   bflg_error_code_alarm_door_open_mark   flg_error_code_mark.bits.bit4   // 5
         //*****************can't add,use'd .uword****************************************

extern flag_type   flg_save1;
        #define   bflg_TS_save                           flg_save1.bits.bit0   //TS�������Ա���ı�־
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

        #define   bflg_self_test                         flg_save2.bits.bit5   //ϵͳ�Լ��־
        #define   bflg_self_test_flicker                 flg_save2.bits.bit6   //ϵͳ�Լ���˸��־
        #define   bflg_self_test_sound                   flg_save2.bits.bit7   //ϵͳ�Լ���һ��
        #define   bflg_user2_P6_save                     flg_save2.bits.bit8   //�û�ģʽusb�������ݵ�ģʽ
        //ѪҺ
        #define   bflg_t1_save                           flg_save2.bits.bit9    
        #define   bflg_r0_save                           flg_save2.bits.bit10   
        #define   bflg_r00_save                          flg_save2.bits.bit11    
        #define   bflg_s1_save                           flg_save2.bits.bit12   
        #define   bflg_s2_save                           flg_save2.bits.bit13   
//------------------------------------------------------------------------------
extern uint8_t guc_key_val;   //������ֵ
extern uint8_t guc_led_buffer[12]; //����ܼĴ���

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



extern int16_t   gss_read_key_delaytimer; //����ֵ��ʱ��ʱ��

//------------------------------------------------------------------------------
#endif


