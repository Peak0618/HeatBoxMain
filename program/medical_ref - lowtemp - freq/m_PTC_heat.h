#ifndef _M_PTC_HEAT_H_
#define _M_PTC_HEAT_H_

//�ⲿ����
extern void PTC_heat_power_on_delaytime(void);
extern void PTC_heat_control_deal(void);
extern void PTC_heat_fre_mode_judge(void);
extern void PTC_heat_RL_out(void);
extern void PTC_fre_RL_deal(void);




//------------------------------------------------------------------------------------------
//�ⲿ��־
extern flag_type bflg_PTC_heat; 
       #define   bflg_PTC_heat_power_on_delaytime  bflg_PTC_heat.bits.bit0  //PTC�ϵ���ʱ
       #define   bflg_PTC_heat_on                  bflg_PTC_heat.bits.bit1  //PTC�������б�־ 
       #define   bflg_pwm_level                    bflg_PTC_heat.bits.bit2  //PTCģ�������pwm��ƽ�ߵͱ�־   
       #define   bflg_PTC_fre_control              bflg_PTC_heat.bits.bit3  //PTC�Ƿ���Ƶ�ʿ���
       #define   bflg_PTC_alway_out                bflg_PTC_heat.bits.bit4  //һֱ���,Ϊ�˲�һֱ�򿪻�ر�
       #define   bflg_PTC_fre_out                  bflg_PTC_heat.bits.bit5  //Ƶ���������������
       #define   bflg_PTC_no_out                   bflg_PTC_heat.bits.bit6  //������� ����������

//------------------------------------------------------------------------------------------
//�ⲿ��������
extern uint8_t guc_PTC_heat_power_on_delaytime;   //PTC��ʼ�ϵ���ʱ



#endif
/**********************************END OF THE FILE*************************************************/
