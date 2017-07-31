#ifndef _M_PERIPHERAL_CONTROL_H_
#define _M_PERIPHERAL_CONTROL_H_


//��������
extern void fan_control_deal(void);
extern void fan_RL_out(void);
extern void led_control_deal(void);
extern void led_key_on_delaytime(void);
extern void led_RL_out(void);
extern void germicidal_lamp_control_deal(void);
extern void germicidal_lamp_delaytime(void);
extern void germicidal_lamp_RL_out(void);
extern void remote_RL_alarm_lamp(void);



//�ⲿ��־
extern flag_type bflg_peripheral_control; 
       #define   bflg_fan_on                       bflg_peripheral_control.bits.bit0  //������ر�־
       #define   bflg_germicidal_lamp_on           bflg_peripheral_control.bits.bit1  //ɱ���ƿ��ر�־
       #define   bflg_led_on                       bflg_peripheral_control.bits.bit2  //led�ƿ��ر�־
       #define   bflg_led_on_delaytime             bflg_peripheral_control.bits.bit3  //�������˵ĵ���ʱ
       #define   bflg_germicidal_lamp_on_delaytime bflg_peripheral_control.bits.bit4  //ɱ���ƿ���ʱ


//----------------------------------------------------------
//�̵�������ں궨��
#define   RL_COMP1_OUT_PIN    P7.4      //ѹ����1�̵��������     
#define   RL_COMP2_OUT_PIN    P7.5      //ѹ����2�̵��������      
       
#define   RL_FAN1_OUT_PIN     P7.3      //���1�̵��������           RL7  bflg_outfan20_running
#define   RL_FAN2_OUT_PIN     P4.2      //���2�̵��������         
#define   RL_HTRMXG_OUT_PIN   P4.1      //ëϸ�ܼ���˿�̵��������  
#define   RL_HTRGK_OUT_PIN    P13.0     //��ڼ���˿�̵��������    
#define   RL_DOWN_OUT_PIN     P15.6     //��ѹ�̵��������          
#define   RL_UP_OUT_PIN       P10.0     //��ѹ�̵��������          
       //----------------------------------------------------------
       //12V����ں궨��
#define   HTRMT_OUT_PIN       P11.1     //�������˿�����   
#define   HTRMXG_OUT_PIN      P14.6     //ëϸ�ܼ���˿����� 
#define   HTRPHK_OUT_PIN      P14.7     //ƽ��ڼ���˿����� 
       //----------------------------------------------------------
#define   TRIP_LAMP_PIN       P7.7      //���ϵ������       
#define   BUZZ_OUT_PIN        P3.1      //�����������       
       //----------------------------------------------------------
#define   TFT_OUT_PIN         P10.1     //Һ������Դ�����   
       //----------------------------------------------------------
#define   RL_CON_PIN          P1.5      //��س����������    // peak ���Ƶ�صĳ���
#define   BAT_PWM_OUT_PIN     P1.6      //��س��PWM�����   
#define   BAT_DISCHARGE_PIN   P1.7      //��ع��������     
       //----------------------------------------------------------
       //��������ں궨��
#define   DOOR1_IN_PIN        P5.2      //�ſ���1�����  
       
#define   BAT_FED_IN_PIN      P0.5      //��ز巴�����  
       //----------------------------------------------------------
       //���뿪������ں궨��
#define   SW11_IN_PIN         P5.1      //���뿪�������
#define   SW12_IN_PIN         P5.0      //���뿪�������
#define   SW13_IN_PIN         P3.0      //���뿪�������
#define   SW14_IN_PIN         P8.7      //���뿪�������
#define   SW15_IN_PIN         P8.6      //���뿪�������
#define   SW16_IN_PIN         P8.5      //���뿪�������
#define   SW17_IN_PIN         P8.4      //���뿪�������
#define   SW18_IN_PIN         P8.3      //���뿪�������
       
#define   SW21_IN_PIN         P0.6      //���뿪�������
#define   SW22_IN_PIN         P7.0      //���뿪�������
#define   SW23_IN_PIN         P7.1      //���뿪�������
#define   SW24_IN_PIN         P7.2      //���뿪�������
       










#endif 
