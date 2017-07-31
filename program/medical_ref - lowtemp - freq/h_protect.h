#ifndef _PROTECT_H_
#define _PROTECT_H_
//------------------------------------------------------------------------------
//�ⲿ��������

extern void prot2_TA_first_delaytime(void);  //���������״θߵ��±�����ʱ������1min��ʱ�����е���

extern void heat_box_TDIS_high_delaytime(void);   //�����������±�����ʱ������1s��ʱ�����е���

extern void heat_box_TDIS_low_delaytime(void);    //�����������±�����ʱ������1s��ʱ�����е���

extern void prot2_power_off_deal(void);      //�ϵ籣�������������ѭ�������е���

extern void port2_power_fault_deal(void);    //��Դ����ϴ����������ѭ�������е���

extern void prot2_bat_discnnt_delaytime(void);    //���δ���ӱ�����ʱ������100ms��ʱ�����е���

extern void prot2_bat_space_delaytime(void);      //���δ���Ӽ������ʱ������100ms��ʱ�����е���

extern void prot2_bat_low_deal(void);   //��ص����ͱ��������������ѭ�������е���

extern void prot2_bat_low_delaytime(void);   //��ص������ӳٳ�����1min��ʱ�����е���

extern void port2_door_delaytime(void);      //�ſ��ر�����ʱ������1s��ʱ�����е���

extern void port2_T_fault_deal(void);        //���������ϴ����������ѭ�������е���

extern void prot2_THW_high_delaytime(void);  //���´��������߱�����ʱ������1s��ʱ�����е���

extern void prot2_TC_first_delaytime(void);  //���������״α�����ʱ������1min��ʱ�����е���

extern void prot2_cond_dirty_delaytime(void);//�������ౣ����ʱ������1s��ʱ�����е���

extern void prot2_volt_deal(void);      //��ѹ���걨�����������������ѭ�������е���

extern void alarm_lamp_deal(void);      //�����ƴ����������ѭ�������е���

extern void alarm_buzz_deal(void);      //���������������������ѭ�������е���

extern void alarm_buzz_off_delaytime(void);       //����������ȡ����ʱ������1min��ʱ�����е���

extern void bat_T_hight_protect(void);               //����¶ȸ��жϳ���


//------------------------------------------------------------------------------
//�ⲿ��־����
extern flag_type flg_alarm;                  
          #define   bflg_alarm_ALH           flg_alarm.bits.bit0    //���±�����־        
          #define   bflg_alarm_ALL           flg_alarm.bits.bit1    //���±�����־         
          #define   bflg_alarm_power_fault   flg_alarm.bits.bit2    //��Դ����ϱ�����־   
          #define   bflg_TTEST_fault         flg_alarm.bits.bit3    //���Դ���������
          #define   bflg_alarm_open_door     flg_alarm.bits.bit4    //���ű�����־       
          //�������������λ�ˣ���Ϊʹ���� flg_alarm.uword
          //--xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

          
//------------------------------------------------------------------------------
 extern flag_type flg_alarm1;                                         
         #define   bflg_alarm_bat_fed      flg_alarm1.bits.bit0   //��ؽӷ�������־    ����
         #define   bflg_alarm_bat_low      flg_alarm1.bits.bit1   //��ص����ͱ�����־  ����
         #define   bflg_alarm_THW_high     flg_alarm1.bits.bit2   //���¹��߱�����־    ����
         #define   bflg_alarm_power_off    flg_alarm1.bits.bit3  //�ϵ籨����־         
         #define   bflg_alarm_bat_discnnt  flg_alarm1.bits.bit4  //���δ���ӱ�����־   
         #define   bflg_alarm_T_fault	   flg_alarm1.bits.bit5  //���������ϱ�����־: �������¡��������Ƚ�����4�ִ���������һ���й���������й���
         #define   bflg_alarm_cond_dirty   flg_alarm1.bits.bit6  //�������౨����־	          


         
//------------------------------------------------------------------------------
extern flag_type flg_alarm2;  
        #define   bflg_alarm_volt          flg_alarm2.bits.bit0       //��ѹ���걨����־  
        #define   bflg_alarm_bat1_T_hight  flg_alarm2.bits.bit1       //���1�¶ȹ���       
        #define   bflg_alarm_bat2_T_hight  flg_alarm2.bits.bit2       //���2�¶ȹ���


//------------------------------------------------------------------------------
extern flag_type flg_prot2;
          
          #define   bflg_alarm_lamp          flg_prot2.bits.bit0 //�����Ʊ�־
          #define   bflg_alarm_buzz          flg_prot2.bits.bit1 //������������־
          #define   bflg_alarm_buzz_output   flg_prot2.bits.bit2 //�����������־
          #define   bflg_alarm_output        flg_prot2.bits.bit3 //Զ�̱��������־
          
          #define   bflg_prot2_door1_pin     flg_prot2.bits.bit4 //�ſ���1����״̬��־
          #define   bflg_prot2_door2_pin     flg_prot2.bits.bit5 //�ſ���2����״̬��־
          #define   bflg_prot2_bat_fed_pin   flg_prot2.bits.bit6 //﮵�ط��巴��־
          
          #define   bflg_prot2_TA_first_delaytime flg_prot2.bits.bit7 //���������״α�����ʱ��־
          #define   bflg_prot2_TC_first_delaytime flg_prot2.bits.bit8 //���������״α�����ʱ��־
          
          #define   bflg_alarm_buzz_off_delaytime flg_prot2.bits.bit9 //����������ȡ����ʱ��־  =1��ʾ�Ѱ���"����ȡ��"����30min�ڲ���
          
          #define   bflg_prot2_THW_high_disp      flg_prot2.bits.bit10//���¹����¶���ʾ��־
          
          #define   bflg_prot2_bat_low_delaytime         flg_prot2.bits.bit11   //��ص�������ʱ��־    //peak ��ʼ�ϵ�Ե�س��24Сʱ��
          #define   bflg_prot2_bat_space_delaytime       flg_prot2.bits.bit12   //���δ���Ӽ������ʱ��־
          #define   bflg_prot2_bat_space_reach           flg_prot2.bits.bit13   //���δ���Ӽ������ʱ����־

		  /****peak add*********/
          #define   bflg_alarm_bat_low_led              flg_prot2.bits.bit14     // ��⵽��ص�����ʱ��ָʾ��ֻ����˸����������


//macro
//gus_trip_code1
#define   H_T_CODE       0x0001   //bit0  ���±���
#define   L_T_CODE       0x0002   //bit1  ���±���
#define   TTSET_S_CODE   0x0004   //bit2  ���Դ��������ϱ��� S = sensor
#define   TDIS_S_CODE    0x0008   //bit3  ��ʾ���������ϱ���
#define   DOOR_OP_CODE   0x0010   //bit4  ���ű���
#define   DC_F_CODE      0x0020   //bit5  ���ص�Դ���ϱ���
//#define   VAC_OFF_CODE   0x0040   //bit6  �ϵ籨��--�޵��

//gus_trip_code2
//#define   AIR_L_CODE     0x0001   //bit0  �������±���

//------------------------------------------------------------------------------
#define   THW_FAULT_CODE      1
#define   TF_FAULT_CODE       2
#define   TC_FAULT_CODE       3
#define   TD_FAULT_CODE       4
#define   TE_FAULT_CODE       5
#define   TA_FAULT_CODE       6
//------------------------------------------------------------------------------
extern int16_t   gss_prot2_TA_first_delaytimer;   //���������״α�����ʱ��ʱ��
extern int16_t   gss_prot2_TC_first_delaytimer;   //���������״α�����ʱ��ʱ��

extern int16_t   gss_prot2_bat_space_delaytimer;  //���δ���Ӽ������ʱ��ʱ��

extern int16_t   gss_prot2_bat_low_delaytimer;    //��ص�������ʱ��ʱ��

extern int16_t   gss_alarm_buzz_off_delaytimer;   //����������ȡ����ʱ��ʱ��

extern uint16_t  gus_trip_code1;   //���ϴ���1
extern uint16_t  gus_trip_code2;   //���ϴ���2




//------------------------------------------------------------------------------
#endif
