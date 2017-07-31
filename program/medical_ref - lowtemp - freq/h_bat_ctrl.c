#ifndef _BAT_CTRL_C_
#define _BAT_CTRL_C_
//------------------------------------------------------------------------------
//�����ļ�����
#include "h_type_define.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_protect.h"
#include "h_bat_ctrl.h"

//------------------------------------------------------------------------------
//��������
void bat_charge_deal(void);   //��س�紦���������ѭ�������е���

void bat_charge_delaytime(void);   //��س����ʱ������1s��ʱ�����е���

void bat_charge_continue_delaytime(void);    //��������ʱ������1min��ʱ�����е���

void bat_charge_space_delaytime(void);  //��س������ʱ������1min��ʱ�����е���

void bat_discharge_deal(void);     //��طŵ紦���������ѭ�������е���

//------------------------------------------------------------------------------
//��������
flag_type flg_bat;

int16_t   gss_bat_charge_delaytimer;    //��س����ʱ��ʱ��

int16_t   gss_bat_pwm_value;  //��س��pwmռ�ձ�ֵ
int16_t   gss_bat_pwm_cnt;    //��س��pwmռ�ձȼ�����

int16_t   gss_charge_continue_delaytimer;    //��س�������ʱ��ʱ��

uint16_t  gus_bat_charge_space_delaytimer;   //��س������ʱ��ʱ��
//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
/******************************************************************************
function: ��Ǧ���� �� ﮵�� �ĳ�����

����λ��: ��ѭ��-��updateӰ��Ҳ��ÿ10s���һ��----------------------ok          
******************************************************************************/
void bat_charge_deal(void)    //��س�紦���������ѭ�������е���
{
    //����е�ز巴�����Դ����ϣ�����δ���Ӷ�ֹͣ���,����¶ȹ���Ҳֹͣ���
    if ((bflg_prot2_bat_fed_pin == 1) || (bflg_alarm_power_fault == 1) ||(bflg_alarm_bat_discnnt == 1)||
        (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat2_T_hight == 1))  
    {   
        bflg_bat_charging = 0;
        gss_bat_pwm_value = 0;
    }
    else
    {
        if (bflg_bat_frist_charge == 1) //������״ε�س��
        {
            bflg_bat_frist_charge = 0;  //���״ε�س���־
            
            bflg_bat_charging = 1;  //��س������
            bflg_bat_charging_end = 0;
            gss_bat_pwm_value = 0;
            
            bflg_bat_charge_space_delaytime = 0;
            gus_bat_charge_space_delaytimer = 0;
            
            bflg_prot2_bat_low_delaytime = 1;       //peak 1 �״��ϵ�ʱ֮�󲻻�����  ��.c��5��//��Ҳ�򿪣�ֻҪ�Ǹտ�ʼ������1��Ϊ���жϵ�ص�����
            gss_prot2_bat_low_delaytimer = 0;
        }
        else
        {
            if (bflg_bat_type == 0)   //�����Ǧ����
            {
                if (bflg_bat_charging == 1)     //�����ǰ�ڳ��
                {
                    if (bflg_bat_charging_end == 0)
                    {
                        if (guc_bat_Vout < ram_para2[num_bat1_charge_V1])     //13.5V//��һ�׶�
                        {
                            if (gss_bat_I > ram_para2[num_bat1_charge_I1])    //610mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat1_charge_I2])    //590mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat1_charge_V2])     //14.3V
                        {
                            if (gss_bat_I > ram_para2[num_bat1_charge_I3])         //270mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat1_charge_I4])    //250mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else
                        {
                            bflg_bat_charging_end = 1;
                        }
                    }
                    else    //����ǳ������׶�
                    {
                        if (guc_bat_Vout > ram_para2[num_bat1_charge_V4])     //13.9V
                        {
                            gss_bat_pwm_value--;
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat1_charge_V5])     //13.7V
                        {
                            gss_bat_pwm_value++;
                        }
                    }
                }
                else   //�����ǰδ���
                {
                    if (guc_bat_Vout < ram_para2[num_bat1_charge_V3])    //12.4V
                    {
                        bflg_bat_charging = 1;  //��س������
                        bflg_bat_charging_end = 0;
                        gss_bat_pwm_value = 0;
                        
                        bflg_bat_charge_space_delaytime = 0;
                        gus_bat_charge_space_delaytimer = 0;
                        
                        bflg_prot2_bat_low_delaytime = 1;          //peak 2  <12.4vʱ���еĳ�� 
                        gss_prot2_bat_low_delaytimer = 0;          //peak  ֮ǰΪ���ڳ��״̬�²��ܼ������͵Ĺ��ϣ�����ע�͵������ʱ�ɼ�⣻//�ֿ���
                    } 
                    else
                    {
                        if (bflg_bat_charge_space_delaytime == 0)  //peak ���30�쵽
                        {
                            bflg_bat_charging = 1;   //��س������
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                            
                            bflg_prot2_bat_low_delaytime = 1;   //peak 3  ���30��ĳ��
                            gss_prot2_bat_low_delaytimer = 0;
                        }
                        else
                        {
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                        }
                    }
                }
                //------------------------------
                if (gss_bat_pwm_value < 0)
                {
                    gss_bat_pwm_value = 0;
                }
                else if (gss_bat_pwm_value > 200)
                {
                    gss_bat_pwm_value = 200;
                }
            }
            else  //�����﮵��
            {
                if (bflg_bat_charging == 1)     //�����ǰ�ڳ��
                {
                    if (bflg_bat_charging_end == 0)
                    {
                        if (guc_bat_Vout < ram_para2[num_bat2_charge_V1])     //12.0V//��һ�׶�
                        {
                            if (gss_bat_I > ram_para2[num_bat2_charge_I1])    //710mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat2_charge_I2])    //690mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat2_charge_V2])     //12.4V
                        {
                            if (gss_bat_I > ram_para2[num_bat2_charge_I3])         //330mA
                            {
                                gss_bat_pwm_value--;
                            }
                            else if (gss_bat_I < ram_para2[num_bat2_charge_I4])    //310mA
                            {
                                gss_bat_pwm_value++;
                            }
                        }
                        else
                        {
                            bflg_bat_charging_end = 1;
                        }
                    }
                    else    //����ǳ������׶�
                    {
                        if (guc_bat_Vout > ram_para2[num_bat2_charge_V4])     //12.9V
                        {
                            gss_bat_pwm_value--;
                        }
                        else if (guc_bat_Vout < ram_para2[num_bat2_charge_V5])     //12.7V
                        {
                            gss_bat_pwm_value++;
                        }
                    }
                }
                else   //�����ǰδ���
                {
                    if (guc_bat_Vout < ram_para2[num_bat2_charge_V3])    //11.2V
                    {
                        bflg_bat_charging = 1;  //��س������
                        bflg_bat_charging_end = 0;
                        gss_bat_pwm_value = 0;
                        
                        bflg_bat_charge_space_delaytime = 0;
                        gus_bat_charge_space_delaytimer = 0;
                        
                        bflg_prot2_bat_low_delaytime = 1;       //peak 4
                        gss_prot2_bat_low_delaytimer = 0;
                    }
                    else
                    {
                        if (bflg_bat_charge_space_delaytime == 0)
                        {
                            bflg_bat_charging = 1;   //��س������
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                            
                            bflg_prot2_bat_low_delaytime = 1;   //peak 5
                            gss_prot2_bat_low_delaytimer = 0;
                        }
                        else
                        {
                            bflg_bat_charging_end = 0;
                            gss_bat_pwm_value = 0;
                        }
                    }
                }
                //------------------------------
                if (gss_bat_pwm_value < 0)
                {
                    gss_bat_pwm_value = 0;
                }
                else if (gss_bat_pwm_value > 250)
                {
                    gss_bat_pwm_value = 250;
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ÿ���1s �Ե�س����е�״������һ�β�ѯ��
          ����� bat_charge_deal();
          
����λ��: 1s��ʱ����------------------------ok          
********************************************************************************/
void bat_charge_delaytime(void)    //��س����ʱ������1s��ʱ�����е���
{
    gss_bat_charge_delaytimer++;
    if (gss_bat_charge_delaytimer >= 10)     //10s
    {
        gss_bat_charge_delaytimer = 0;
        
        bflg_bat_charge_update = 1;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: Ǧ���س��36��Сʱ�����
          ﮵�س��7Сʱ�������
         
*******************************************************************************/
void bat_charge_continue_delaytime(void)     //��������ʱ������1min��ʱ�����е���
{
    if (bflg_bat_charging == 1)
    {
        gss_charge_continue_delaytimer++;
        if (bflg_bat_type == 0)    //�����Ǧ����
        {
            if (gss_charge_continue_delaytimer >= (ram_para2[num_bat1_charge_continue_delaytime] * 60))     //36Сʱ
            {
                gss_charge_continue_delaytimer = 0;
                
                bflg_bat_charging = 0;
                bflg_bat_charge_space_delaytime = 1;
                gus_bat_charge_space_delaytimer = 0;
            }
        }
        else   //�����﮵��
        {
            if (gss_charge_continue_delaytimer >= (ram_para2[num_bat2_charge_continue_delaytime] * 60))     //7Сʱ
            {
                gss_charge_continue_delaytimer = 0;
                
                bflg_bat_charging = 0;
                bflg_bat_charge_space_delaytime = 1;
                gus_bat_charge_space_delaytimer = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��س���һ�ε磬�ڵ�30���ٳ�(�ڼ�����������ﵽ��Ҳ�ܳ��)

*******************************************************************************/
void bat_charge_space_delaytime(void)   //��س������ʱ������1min��ʱ�����е���
{
    if (bflg_bat_charge_space_delaytime == 1)
    {
        gus_bat_charge_space_delaytimer++;
        if (bflg_bat_type == 0)    //�����Ǧ����
        {
            if (gus_bat_charge_space_delaytimer >= (ram_para2[num_bat1_charge_sapce_delaytime] * 24 * 60))    //30��
            {
                gus_bat_charge_space_delaytimer = 0;
                bflg_bat_charge_space_delaytime = 0;
            }
        }
        else   //�����﮵��
        {
            if (gus_bat_charge_space_delaytimer >= (ram_para2[num_bat2_charge_sapce_delaytime] * 24 * 60))    //30��
            {
                gus_bat_charge_space_delaytimer = 0;
                bflg_bat_charge_space_delaytime = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function:   ��⵽��Դ��ѹ��80V ��Ϊ�ϵ�״̬�� bflg_alarm_power_off = 1��

            bflg_bat_discharging = 0  ���ֹͣ�ŵ�

            bflg_bat_discharging = 1  ��ʼΪ�ŵ�״̬

����λ�ã� ��ѭ��--------------------�ϵ���ʱ�������------------------ok           
******************************************************************************/
void bat_discharge_deal(void)      //��طŵ紦���������ѭ�������е���
{
///*
    //��ز巴������¶ȹ��߲��ŵ磻peak���δ�����Ƿ�Ҳ��?
    if((bflg_alarm_bat_fed == 1) || (bflg_alarm_bat1_T_hight == 1) || (bflg_alarm_bat1_T_hight ==1))                             
    {
        bflg_bat_discharging = 0;
    }     
    else if (bflg_alarm_power_off == 1) //����Ƕϵ�״̬
    {
        if (bflg_bat_type == 0)    //�����Ǧ����
        {
            if (guc_bat_Vout <= ram_para2[num_bat1_discharge_off_V])   //10.5V
            {
                bflg_bat_discharging = 0;
            }
            else
            {                
                bflg_bat_discharging = 1;
            }
        }
        else   //�����﮵��
        {
            if (guc_bat_Vout <= ram_para2[num_bat2_discharge_off_V])   //9.0V
            {
                bflg_bat_discharging = 0;
            }
            else
            {
                bflg_bat_discharging = 1;
            }
        }
    }
    else
    {        
        bflg_bat_discharging = 1;
    }
   //*/
}

//------------------------------------------------------------------------------
#endif
