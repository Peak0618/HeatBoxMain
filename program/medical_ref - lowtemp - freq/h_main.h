#ifndef _MAIN_H_
#define _MAIN_H_
//------------------------------------------------------------------------------
//�ⲿ��������//
extern void system_init(void);     //ϵͳ��ʼ������

extern void system_start(void);    //ϵͳ��������

extern void main_loop(void);  //��ѭ������

extern void timer_int(void);  //1ms��ʱ�жϳ���

extern void adc_int(void);    //ADת���жϳ���

extern void eeprom_write(uint16_t lus_addr, int16_t lss_eep_data);    //дeeprom����

extern int16_t eeprom_read(uint16_t lus_addr);                        //��eeprom����

extern void init_ram_para2(void);

extern void init_ram_para(void);

extern void eeprom2_read_deal(void); //eeprom2��ȡ��������ڳ�ʼ�������е���

extern void eeprom_read_deal(void);  //eeprom��ȡ��������ڳ�ʼ�������е���


//------------------------------------------------------------------------------
//�ⲿ��������//
extern flag_type flg_time;
          #define   bflg_1ms_reach           flg_time.bits.bit0  //1ms��ʱ����־
          #define   bflg_10ms_reach          flg_time.bits.bit1  //10ms��ʱ����׼
          #define   bflg_askfor_read_eeprom  flg_time.bits.bit2   //�����eeprom��־
          #define   bflg_read_eeprom_error   flg_time.bits.bit3   //��eeprom���־
          #define   bflg_eeprom_fault        flg_time.bits.bit4   //eeprom���ϱ�־
          #define   bflg_RL_output_delaytime flg_time.bits.bit5   //�̵��������ʱ��־
          
          #define   bflg_test_type      flg_time.bits.bit6   //���԰���ʾ���־
          #define   bflg_test_mode      flg_time.bits.bit7   //����ģʽ��־
          #define   bflg_test_ok        flg_time.bits.bit8   //������ɱ�־
          #define   bflg_test_mode_RTC  flg_time.bits.bit9   //����ģʽ�Ĳ���ʱ�ӵ�·
//------------------------------------------------------------------------------
#define   LOCK_OUT_PIN        P11.0     //����������                
#define   RL_ALM_OUT_PIN      P6.5      //Զ�̱����̵��������      =1 �Ǵ�Զ�̱�����  
//----------------------------------------------------------
//�°� �����ӵ� ��ɫ��
#define   BLUE_LED_PIN       P8.0      //��ɫ��
#define   GREEN_LED_PIN      P8.1      //��ɫ��
#define   RED_LED_PIN        P8.2      //��ɫ��--��������ģ�����pwm��

//------------------------------------------------------------------------------
#define   SOFT_VERSION        1000      //����汾��


//------------------------------------------------------------------------------
#endif

