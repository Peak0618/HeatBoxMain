#ifndef _PROTECT_C_
#define _PROTECT_C_
//------------------------------------------------------------------------------
//�����ļ�����
#include "h_type_define.h"
#include "h_ad.h"
#include "h_rom_para2.h"
#include "h_key_disp.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak add
#include "M_buzz.h"
#include "h_main.h"
#include "m_key_operation.h"
//------------------------------------------------------------------------------
//��������
void prot2_TA_first_delaytime(void);    //���������״θߵ��±�����ʱ������1min��ʱ�����е���

void heat_box_TDIS_high_delaytime(void);     //�����������±�����ʱ������1s��ʱ�����е���

void heat_box_TDIS_low_delaytime(void);      //�����������±�����ʱ������1s��ʱ�����е���

void prot2_power_off_deal(void);   //�ϵ籣�������������ѭ�������е���

void port2_power_fault_deal(void); //��Դ����ϴ����������ѭ�������е���

void prot2_bat_discnnt_delaytime(void); //���δ���ӱ�����ʱ������100ms��ʱ�����е���

void prot2_bat_space_delaytime(void);   //���δ���Ӽ������ʱ������100ms��ʱ�����е���

void prot2_bat_low_deal(void);     //��ص����ͱ��������������ѭ�������е���

void prot2_bat_low_delaytime(void);//��ص������ӳٳ�����1min��ʱ�����е���

void port2_door_delaytime(void);   //�ſ��ر�����ʱ������1s��ʱ�����е���

void port2_T_fault_deal(void);     //���������ϴ����������ѭ�������е���

void prot2_THW_high_delaytime(void);    //���´��������߱�����ʱ������1s��ʱ�����е���

void prot2_TC_first_delaytime(void);    //���������״α�����ʱ������1min��ʱ�����е���

void prot2_cond_dirty_delaytime(void);  //�������ౣ����ʱ������1s��ʱ�����е���

void prot2_volt_deal(void);   //��ѹ���걨�����������������ѭ�������е���

void alarm_lamp_deal(void);   //�����ƴ����������ѭ�������е���

void alarm_buzz_deal(void);   //���������������������ѭ�������е���

void alarm_buzz_off_delaytime(void);    //����������ȡ����ʱ������1min��ʱ�����е���

void alarm_output_deal(void);                //Զ�̱��������������ѭ�������е���
void bat_T_hight_protect(void);             //����¶ȹ����ж�



//------------------------------------------------------------------------------
//��������
flag_type flg_alarm,flg_alarm1,flg_alarm2,flg_prot2;

//peak add
flag_type   memory;

//------------------------------------------------------------------------------
int16_t   gss_heat_box_TDIS_high_delaytimer; //�����������±�����ʱ��ʱ��
int16_t   gss_heat_box_TDIS_low_delaytimer;  //�����������±�����ʱ��ʱ��
int16_t   gss_prot2_TA_first_delaytimer;//���������״α�����ʱ��ʱ��

int16_t   gss_prot2_bat_discnnt_delaytimer;  //���δ���ӱ�����ʱ��ʱ��
int16_t   gss_prot2_bat_space_delaytimer;    //���δ���Ӽ������ʱ��ʱ��
int16_t   gss_prot2_bat_low_delaytimer;      //��ص�������ʱ��ʱ��

int16_t   gss_prot2_door_delaytimer;    //�ſ��ر�����ʱ��ʱ��

int16_t   gss_prot2_THW_high_delaytimer;     //���´��������߱�����ʱ��ʱ��

int16_t   gss_prot2_TC_first_delaytimer;//���������״α�����ʱ��ʱ��
int16_t   gss_prot2_cond_dirty_delaytimer;   //����������ʱ��ʱ��

int16_t   gss_alarm_buzz_off_delaytimer;     //����������ȡ����ʱ��ʱ��

uint16_t  gus_trip_code1;     //���ϴ���1
uint16_t  gus_trip_code2;     //���ϴ���2

/******************************************************************************************************************************************
�������ܣ�  ��ʼ�ϵ�3Сʱ�����жϸ��±�����

����λ�ã�1min----------------------------��Ϊ�����䲻��--δ��      
******************************************************************************************************************************************/
void prot2_TA_first_delaytime(void)     
{
    if (bflg_prot2_TA_first_delaytime == 1)
    {
        gss_prot2_TA_first_delaytimer++;
        if (gss_prot2_TA_first_delaytimer >= ram_para[num_prot2_TA_first_delaytime])      //180����
        {
            gss_prot2_TA_first_delaytimer = 0;
            bflg_prot2_TA_first_delaytime = 0;
        }
    }
}
/********************************************************************************************************************************
function : ��ʾ���������±���---������
      

����λ�ã�1s-----------------------------------------------ok
*********************************************************************************************************************************/
void heat_box_TDIS_high_delaytime(void)     
{
    if (bflg_alarm_ALH == 0)
    {
        if (gss_TDIS >= gss_user_set_parm_value[DISP_USER_SET_ALH])      
        {
            gss_heat_box_TDIS_high_delaytimer++;
            if (gss_heat_box_TDIS_high_delaytimer >= 60)   //60s
            {
                gss_heat_box_TDIS_high_delaytimer = 0;
                bflg_alarm_ALH = 1;
                gus_trip_code1 |= H_T_CODE;    
            }
        }
        else
        {
            gss_heat_box_TDIS_high_delaytimer = 0;
        }
    }
    else                                     
    {
        if (gss_TDIS < gss_user_set_parm_value[DISP_USER_SET_ALH]  - 10)   // -1��
        {
            gss_heat_box_TDIS_high_delaytimer++;
            if (gss_heat_box_TDIS_high_delaytimer >= 60)                       //60s
            {
                gss_heat_box_TDIS_high_delaytimer = 0;
                bflg_alarm_ALH = 0;
                gus_trip_code1 &= ~H_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_high_delaytimer = 0;
        }
    }
}
/*****************************************************************************************************************************************
�������ܣ���ʾ���������±����ĳ����ͻָ�---������

����λ�ã�1s------------------------------------------ok
*****************************************************************************************************************************************/
void heat_box_TDIS_low_delaytime(void)  
{
    if (bflg_alarm_ALL == 0)
    {
        if (gss_TDIS <= gss_user_set_parm_value[DISP_USER_SET_ALL])
        {
            gss_heat_box_TDIS_low_delaytimer++;
            if (gss_heat_box_TDIS_low_delaytimer >= 60)     //60s
            {
                gss_heat_box_TDIS_low_delaytimer = 0;
                bflg_alarm_ALL = 1;
                gus_trip_code1 |= L_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_low_delaytimer = 0;
        }
    }
    else
    {
        if (gss_TDIS > gss_user_set_parm_value[DISP_USER_SET_ALL] + 10)    // 1��
        {
            gss_heat_box_TDIS_low_delaytimer++;
            if (gss_heat_box_TDIS_low_delaytimer >= 60)    //60
            {
                gss_heat_box_TDIS_low_delaytimer = 0;
                bflg_alarm_ALL = 0;
                gus_trip_code1 &= ~L_T_CODE;
            }
        }
        else
        {
            gss_heat_box_TDIS_low_delaytimer = 0;
        }
    }
}
/*******************************************************************************
function : �ϵ籨���ĳ����ͻָ�

�ж������� ��⵽��Դ��ѹ��80V
�����ʩ�� ��������˸�� �������죬 Զ�̱��������
�ָ������� ��⵽��Դ��ѹ��90V �ָ���

*******************************************************************************/
void prot2_power_off_deal(void)    //�ϵ籣�������������ѭ�������е���
{
    if (gss_VAC <= ram_para2[num_power_fautl_VAC1])         //80V
    {
        bflg_alarm_power_off = 1;
        //gus_trip_code1 |= VAC_OFF_CODE; 
    }
    else if (gss_VAC >= ram_para2[num_power_fautl_VAC2])    //90V
    {
        bflg_alarm_power_off = 0;
        //gus_trip_code1 &= ~VAC_OFF_CODE;
    }
}
/****************************************************************************************************************************************
�������� : 12v��Դ����ϵĳ����ͻָ�


����λ��:  ��ѭ��-----------------------------ok
****************************************************************************************************************************************/
void port2_power_fault_deal(void)     
{
    if (guc_12V < ram_para2[num_power_off_12V])  //9v
    {
        if (bflg_alarm_power_off == 0)           //peak ��220û�жϵ�������,12v�Ǵ�220ת�����ģ�
        {
            bflg_alarm_power_fault = 1;
            gus_trip_code1 |= DC_F_CODE;
        }
        else
        {
            bflg_alarm_power_fault = 0;
            gus_trip_code1 &= ~DC_F_CODE;
        }
    }
    else if (guc_12V > ram_para2[num_power_on_12V]) //11v
    {
        bflg_alarm_power_fault = 0;
        gus_trip_code1 &= ~DC_F_CODE;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ���δ���ӹ��ϵĳ����ͻָ�

��Ϊ������:
1�� �ǳ��״̬ʱ
�ж������� ����ص�ѹ��2.0V��
�����ʩ�� ��������˸�� �������죻
�ָ������� δ���ʱ�� ����ص�ѹ��5.0V

2�� ���״̬ʱ
�ж������� ����ص�ѹ��9.0V��
�����ʩ�� ��������˸�� �������죻
�ָ������� δ���ʱ�� ����ص�ѹ��10.0V

********************************************************************************/
void prot2_bat_discnnt_delaytime(void)  //���δ���ӱ�����ʱ������100ms��ʱ�����е���
{
    if (bflg_bat_charging == 0)              //�����ǰΪδ���״̬
    {
        if (bflg_alarm_bat_discnnt == 0)     //�����ǰ�޵��δ���ӹ���
        {
            if (guc_bat_Vout_instant < ram_para2[num_port2_bat_discnnt_V1])   //peak ��Ϊ2v ��
            {
                bflg_alarm_bat_discnnt = 1;
                //gus_trip_code1 |= 0x1000;
            }
        }
        else   //�����ǰ�е��δ���ӹ���
        {
            if (guc_bat_Vout_instant > ram_para2[num_port2_bat_discnnt_V2])   //5.0V
            {
                bflg_alarm_bat_discnnt = 0;
                //gus_trip_code1 &= ~0x1000;
            }
        }
    }
    else   //�����ǰΪ���״̬
    {
        if (bflg_alarm_bat_discnnt == 0)     //�����ǰ�޵��δ���ӹ���
        {
            //if (guc_bat_Vout_instant < 90)//ram_para2[num_port2_bat_discnnt_V1]) //peak ������Ϊ 9v
            if ((guc_bat_Vout_instant < 90) || (guc_bat_Vout_instant > 151))       //peak���ʱ�����<9v��>15.1v���ж�Ϊ�Ͽ����
            {
                bflg_alarm_bat_discnnt = 1;
                //gus_trip_code1 |= 0x1000;
            }
        }
        else   //�����ǰ�е��δ���ӹ���
        {
            if (guc_bat_Vout_instant > 100)//ram_para2[num_port2_bat_discnnt_V2])       //peak ������Ϊ 10v
            {
                bflg_alarm_bat_discnnt = 0;
                //gus_trip_code1 &= ~0x1000;
            }
        }
    }
}

//------------------------------------------------------------------------------
/*******************************************************************************
function : ò��û��~~
********************************************************************************/
void prot2_bat_space_delaytime(void)    //���δ���Ӽ������ʱ������100ms��ʱ�����е���
{
    if (bflg_prot2_bat_space_delaytime == 1)
    {
        gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 200) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 0;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
    else
    {
    	  gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 50) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 1;
            
            //bflg_prot2_bat_space_reach = 1;
        }
    }
    //----------------------------------
    /*if (bflg_prot2_bat_space_reach == 1)
    {
        gss_prot2_bat_space_delaytimer++;
        if (gss_prot2_bat_space_delaytimer >= 50) //5s
        {
            gss_prot2_bat_space_delaytimer = 0;
            bflg_prot2_bat_space_delaytime = 1;
            
            bflg_prot2_bat_space_reach = 0;
        }
    }*/
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��ص����͹��ϵĳ����ͻָ�

�ж�����:�����ڵ�س��24Сʱ���ҵ�����ӵ�������жϵģ�


�ָ�����:1�����δ���ӣ�2����ѹ>��ֵ  (3���ڳ��ʱ���ܼ���ѱ�ȥ�� )
*******************************************************************************/
void prot2_bat_low_deal(void)      //��ص����ͱ��������������ѭ�������е���
{    
    //debug ��������ʱ��ȥ��ע��
    if ((bflg_prot2_bat_low_delaytime == 0) && (bflg_alarm_bat_discnnt == 0))   //���24Сʱ���ҵ��δ����//peak û��δ����
	{   //�����������޵��δ���ӱ���
        if (bflg_bat_type == 0)    //�����Ǧ����
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V1])   //10.5V
            {
                bflg_alarm_bat_low = 1;
                //gus_trip_code1 |= 0x2000;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V2])   //11.0V
            {
                bflg_alarm_bat_low = 0;
                //gus_trip_code1 &= ~0x2000;
            }
        }
        else   //�����﮵��
        {
            if (guc_bat_Vout < ram_para2[num_bat_low_V3])   //9.0V
            {
                bflg_alarm_bat_low = 1;
                //gus_trip_code1 |= 0x2000;
            }
            else if (guc_bat_Vout > ram_para2[num_bat_low_V4])   //9.5V
            {
                bflg_alarm_bat_low = 0;
                //gus_trip_code1 &= ~0x2000;
            }
        }
    }
    else
    {
        bflg_alarm_bat_low = 0;
        //gus_trip_code1 &= ~0x2000;
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: ��ʼ�ϵ���24Сʱ���ٶԵ�ص����ͽ��м��---peak ���˾�������
          Ӧ��һֱ�Ե�ص����ͽ��м�⣻
******************************************************************************/
void prot2_bat_low_delaytime(void) //��ص������ӳٳ�����1min��ʱ�����е���
{
    if (bflg_prot2_bat_low_delaytime == 1)
    {
        gss_prot2_bat_low_delaytimer++;
        if (gss_prot2_bat_low_delaytimer >= (ram_para2[num_prot2_bat_low_delaytime] * 60)) //24Сʱ
        {
            gss_prot2_bat_low_delaytimer = 0;
            bflg_prot2_bat_low_delaytime = 0;
        }
    }
}

/*******************************************************************************************************************************
��������: ���ű����ĳ����ͻָ�


����λ�ã�1s--------------------------------------------ok
*******************************************************************************************************************************/
void port2_door_delaytime(void)    
{
    if ((bflg_prot2_door1_pin == 1) || (bflg_prot2_door2_pin == 1))
    {
        if (bflg_alarm_open_door == 0)
        {
            gss_prot2_door_delaytimer++;
            //if (gss_prot2_door_delaytimer >= (ram_para[num_prot2_door_delaytime] * 60))  //5����
            if (gss_prot2_door_delaytimer >= (gss_user_set_parm_value[DISP_USER_SET_dA] * 60))        //5���� �ÿɵ���gss_user_set_parm_value[DISP_USER_SET_dA]�滻֮ǰ�Ĺ̶�ֵ
            {   
                gss_prot2_door_delaytimer = 0;
                
                bflg_alarm_open_door = 1;
                gus_trip_code1 |= DOOR_OP_CODE;
            }
        }
    }
    else
    {
        gss_prot2_door_delaytimer = 0;
        bflg_alarm_open_door = 0;
        gus_trip_code1 &= ~DOOR_OP_CODE;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ���������ϱ����ĳ����ͻָ�:

�ж������� �������������� ������������ ���´������� �Ƚ���������������ʱ��
�����ʩ�� ��������˸�� �������죬 Զ�̱��������
�ָ������� ������������ ������������ ���´������� �Ƚ�����������������ʱ�� �ָ���

********************************************************************************/
void port2_T_fault_deal(void)      //���������ϴ����������ѭ�������е���
{
    if (bflg_THW_fault == 1)    //peak ���±���
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TE_fault == 1) //peak �Ƚ������¶ȴ��������ϱ�־        
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TC_fault == 1) //peak �������¶ȴ��������ϱ�־
    {
        bflg_alarm_T_fault = 1;
    }
    else if (bflg_TA_fault == 1) //peak �����������ϱ�־
    {
        bflg_alarm_T_fault = 1;
    }
    else
    {
        bflg_alarm_T_fault = 0;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ���¹��߱����ĳ����ͻָ�

�ж������� ���£�35�棨 EEP�� ���� 30 �루 EEP����
�����ʩ�� ��������˸��
�ָ������� ���¡�34�棨 EEP�� ���� 30 �루 EEP����
         ����ʱ�����ʾ���£�35�棬 ����ʾ 35�棩

********************************************************************************/
void prot2_THW_high_delaytime(void)     //���´��������߱�����ʱ������1s��ʱ�����е���
{/*
    if (bflg_alarm_THW_high == 0)
    {
        //if (gss_THW > ram_para[num_prot2_THW_T])     //35ԭ����       
        if (gss_THW > gss_factory_AA)                  //�ɵ�
        {
            gss_prot2_THW_high_delaytimer++;
            if (gss_prot2_THW_high_delaytimer >= ram_para2[num_prot2_THW_high_delaytime]) //30
            {
                gss_prot2_THW_high_delaytimer = 0;
                bflg_alarm_THW_high = 1;
                gus_trip_code1 |= 0x0400;
            }
        }
        else
        {
            gss_prot2_THW_high_delaytimer = 0;
        }
        
        bflg_prot2_THW_high_disp = 0;
    }
    else
    {
        //if (gss_THW <= (ram_para[num_prot2_THW_T] + ram_para2[num_prot2_THW_Tdelt]))     //34ԭ����
        if (gss_THW <= (gss_factory_AA + ram_para2[num_prot2_THW_Tdelt]))                  //�ɵ�
        {
            bflg_prot2_THW_high_disp = 1;
            
            gss_prot2_THW_high_delaytimer++;
            if (gss_prot2_THW_high_delaytimer >= ram_para2[num_prot2_THW_high_delaytime]) //30
            {
                gss_prot2_THW_high_delaytimer = 0;
                bflg_alarm_THW_high = 0;
                gus_trip_code1 &= ~0x0400;
                
                bflg_prot2_THW_high_disp = 0;
            }
        }
        else
        {
            gss_prot2_THW_high_delaytimer = 0;
            
            bflg_prot2_THW_high_disp = 0;
        }
    }*/
}
//------------------------------------------------------------------------------
/******************************************************************************
function: �������౨����һ������--��ʼ�ϵ� 2 Сʱ���жϣ�Ϊ����ĺ�������
******************************************************************************/
void prot2_TC_first_delaytime(void)     //���������״α�����ʱ������1min��ʱ�����е���
{
    if (bflg_prot2_TC_first_delaytime == 1)
    {
        gss_prot2_TC_first_delaytimer++;
        if (gss_prot2_TC_first_delaytimer >= ram_para2[num_prot2_TC_first_delaytime])     //120
        {
            gss_prot2_TC_first_delaytimer = 0;
            bflg_prot2_TC_first_delaytime = 0;
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: �������౨���ĳ����ͻָ�

�ж�������1�� ��ʼ�ϵ� 2 Сʱ���жϣ�
          2�� �������������¶�-���´������¶ȡ�13����� 5 ���ӣ�

�����ʩ�� ��������˸�� �������죻

�ָ������� �������������¶�-���´������¶ȣ�10�� ���� 5 ����;

******************************************************************************/
void prot2_cond_dirty_delaytime(void)   //�������ౣ����ʱ������1s��ʱ�����е���
{
    if (bflg_prot2_TC_first_delaytime == 0)          //��ʼ�ϵ羭��2Сʱ��
    {
        if (bflg_alarm_cond_dirty == 0)              //û�б���ʱ���ж��Ƿ��б���
        {
            if ((gss_TC - gss_THW) >= ram_para2[num_prot2_cond_dirty_TC1])   //13��
            {
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s = 5min
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 1;
                    //gus_trip_code1 |= 0x0200;
                }
            }
            else                                    
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
        else                                          //�б���ʱ���ж��Ƿ�������    
        {
            if ((gss_TC - gss_THW) < ram_para2[num_prot2_cond_dirty_TC2])    //10��   peakԭ������TC1
            {
                gss_prot2_cond_dirty_delaytimer++;
                if (gss_prot2_cond_dirty_delaytimer >= ram_para2[num_prot2_cond_dirty_delaytime])   //300s
                {
                    gss_prot2_cond_dirty_delaytimer = 0;
                    
                    bflg_alarm_cond_dirty = 0;
                    //gus_trip_code1 &= ~0x0200;
                }
            }
            else
            {
                gss_prot2_cond_dirty_delaytimer = 0;
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��ѹ���걨���ĳ����ͻָ�

1�� ����� 220V ��Դ
	�ж������� ��Դ��ѹ��184V�� ���Դ��ѹ��253V��
	�����ʩ�� ��������˸�� �������죻
	�ָ������� 194V�ܵ�Դ��ѹ��243V ʱ��
	
2�� ����� 115V ��Դ
	�ж������� ��Դ��ѹ��93V�� ���Դ��ѹ��136V��
	�����ʩ�� ��������˸�� �������죻
	�ָ������� 98V �ܵ�Դ��ѹ��131V ʱ��

********************************************************************************/
void prot2_volt_deal(void)    //��ѹ���걨�����������������ѭ�������е���
{
   /*if (bflg_comp2_volt_type == 0) //�����220V��Դ
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V1]) || (gss_VAC >= ram_para2[num_prot2_volt_V2]))       //184V    253V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V3]) && (gss_VAC <= ram_para2[num_prot2_volt_V4]))   //194V   243V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }
    else       //�����115V��Դ
    {
        if ((gss_VAC <= ram_para2[num_prot2_volt_V5]) || (gss_VAC >= ram_para2[num_prot2_volt_V6]))        //93V   136V
        {
            bflg_alarm_volt = 1;
            gus_trip_code1 |= 0x8000;
        }
        else if ((gss_VAC >= ram_para2[num_prot2_volt_V7]) && (gss_VAC <= ram_para2[num_prot2_volt_V8]))   //98V   131V
        {
            bflg_alarm_volt = 0;
            gus_trip_code1 &= ~0x8000;
        }
    }*/
}

/******************************************************************************
function: 1����ʾ���ϵĵ�3λ�������Ƿ���ʾ�Ĵ���;
          2��ֻҪ������һ�ֱ�������ʾ���ϵ�3λ�����ƶ��᳣����

          bflg_alarm_lamp = 1 ��ʾ��ʾ�����"����"����������һֱ����
******************************************************************************/
void alarm_lamp_deal(void)             //�����ƴ����������ѭ�������е���
{
    if (((flg_alarm.uword & 0xff) != 0)||     //�б�����־Ϊ1��
        ((flg_alarm1.uword & 0xff) != 0))
    {        
        bflg_alarm_lamp = 1;
    }
    else
    {
        bflg_alarm_lamp = 0;
    }
}

/*****************************************************************************************************************************
function: ���� ������ ���� --������

����λ�ã���ѭ������������������������ok
*****************************************************************************************************************************/
void alarm_buzz_deal(void)    //���������������������ѭ�������е���
{   
    if (bflg_test_mode == 0)         //���ڲ���ģʽ�²Ŵ���      
    {
        flag_type memory_temp ;      

        memory_temp.uword = memory.uword;  //memory_temp�洢��һ�εĹ��� 
                                           //flg_alarm ���µĹ��ϣ������жϷ������ε���  
        memory.uword= flg_alarm.uword;     //memory ��flg_alarm���µĹ��ϸ�����ֵ���������ϴεĴ洢�Ĺ���ֵ memory_temp�Աȣ��ж��Ƿ����µĹ��ϱ仯��������'����ȡ��'�����������30�ڲ���

       //--------------------------------------------------------------------------------
       
       if((flg_alarm.uword  == 0)||                                        //û�б���
          (bflg_alarm_buzz_off_delaytime == 1))//||                        //�б�����������"����ȡ��"
          //((flg_alarm.uword == 0x0008 )&&(bflg_key_long_buz_valid == 1))) //ֻ�е��δ���ӱ��������ǳ���"����ȡ��"5s��(����Ϊ���ò���bs = 1)
        {                                                                   
            bflg_buzz_loop_tick_set = 0;                                   //����3��������ε�          

            if(memory_temp.uword != memory.uword)                          //���µı���ʱ���尴"����ȡ��"30min���죬�ֿ�ʼ��
            {
                bflg_alarm_buzz_off_delaytime = 0;
                gss_alarm_buzz_off_delaytimer = 0;
            }
        }           
        else
        {
            bflg_buzz_loop_tick_set = 1;                                    //��������ε���
        }

        
        /*        //��ز巴 �������޵��
        if (bflg_alarm_bat_fed == 1)               //peak ��ؽӷ�������־ 6
        {
            bflg_buzz_always_sound_set = 1;        //peak ����������
        }
        else
        {
            bflg_buzz_always_sound_set = 0;
        }	*/
    }
}

/*******************************************************************************
function:   ���ⱨ����������ʱ�� ���¡�����ȡ�������� ���������죬 
            30 ���Ӻ�ָ��������ƣ� 
            
����λ��: ��1min��ʱ�����е���      -----------ok        
********************************************************************************/
void alarm_buzz_off_delaytime(void)     
{
    if (bflg_alarm_buzz_off_delaytime == 1)
    {
        gss_alarm_buzz_off_delaytimer++;
        //if (gss_alarm_buzz_off_delaytimer >= ram_para[num_alarm_buzz_off_delaytime])     //30����
        if (gss_alarm_buzz_off_delaytimer >= gss_factory_parm_value[DISP_FAC_Fd]*10)                               //��Ϊ�ɵ�
        {
            gss_alarm_buzz_off_delaytimer = 0;
            bflg_alarm_buzz_off_delaytime = 0;
        }
    }
}


/**********************************************************************************
�������� : ����¶ȹ��߱��� ,cn22��cn23�����ӵ�صĿڶ�һ��

            ������¶ȡ�65�棨EEP��ʱ��ֹͣ��س�ŵ磻 
            ������¶ȣ�55�棨EEP��ʱ���ָ���س�ŵ硣

��ע:  cn22 cn23�����ڶ������¶ȹ��ߵı�������ȻǦ����ֻ������û���¶ȴ�������
       ������û�н�ʱ��ad�ɼ�Ϊ1024����ʱ�¶�Ϊ0��������ߣ�

����λ��: ��ѭ����-----------------------------------------------ok
**********************************************************************************/
void bat_T_hight_protect(void)
{
    //bat1
    if (gss_Tbat1 >= ram_para2[num_bat2_discharge_off_T])       //65��   ���1 ��2���������׼
    {
        bflg_alarm_bat1_T_hight = 1;
    }
    else if (gss_Tbat1 <= ram_para2[num_bat2_discharge_on_T])   //55��
    {
        bflg_alarm_bat1_T_hight = 0;
    }

    //bat2
    //--------------------------------------------------------------------
    if (gss_Tbat2 >= ram_para2[num_bat2_discharge_off_T])       //65��
    {
        bflg_alarm_bat2_T_hight = 1;
    }
    else if (gss_Tbat2 <= ram_para2[num_bat2_discharge_on_T])   //55��
    {
        bflg_alarm_bat2_T_hight = 0;
    }

}


//------------------------------------------------------------------------------
#endif
