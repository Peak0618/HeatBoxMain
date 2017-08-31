/**************************************************************************************************** 
overview:20170406
         ���ļ��Ƕ�PTC����ģ��Ŀ��ƣ�1��PTCģ��ļ��ȿ��� --CN29(HTR-CAP) (P41)
                                      
                                       
*****************************************************************************************************/
//------------------------------------------------------------------------------------------
//�������ļ�
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_timer.h"

#include "h_type_define.h"
#include "h_ad.h"
#include "m_PTC_heat.h"
#include "m_key_operation.h"
#include "h_main.h"
#include "h_protect.h"
#include "h_com.h"
//------------------------------------------------------------------------------------------
//��������
void PTC_heat_power_on_delaytime(void);
void PTC_heat_control_deal(void);
void PTC_heat_fre_mode_judge(void);
void PTC_heat_RL_out(void);
void PTC_fre_RL_deal(void);



//------------------------------------------------------------------------------------------
//macro
#define     PWM_PERIOD            33333    //�����趨Ϊ33333
#define     PWM_HIGH_LEVEL_CNT    0x208d   //pwm����ߵ�ƽ��ʱ    --30HZ��25%ռ�ձ�
#define     PWM_LOW_LEVEL_CNT     0x61a8   //pwm����͵�ƽ��ʱ
#define     PWM_HIGH_LEVEL        1        //pwmΪ�ߵ�ƽʱ��־
#define     PWM_LOW_LEVEL         0        //pwmΪ�͵�ƽʱ��־
#define     PTC_OUT_ON            0        //��PTC -��Ϊ����0�ǿ�
#define     PTC_OUT_OFF           1        //��PTC
#define     RL_PTC_OUT_PIN        P4.1     //����ģ���������


//------------------------------------------------------------------------------------------
//��־����
flag_type bflg_PTC_heat; 


//------------------------------------------------------------------------------------------
//�������� 
uint8_t guc_PTC_heat_power_on_delaytime;   //PTC��ʼ�ϵ���ʱ
uint8_t guc_PTC_heat_fault_on_delaytime;   //���Դ���������ʱPTC��/ͣʱ��

/*********************************************************************************************************************************************
��������: ��ʼ�ϵ�PTC������ʱ

����λ��: 1min��ʱ���� --------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_power_on_delaytime(void)
{
    if(bflg_PTC_heat_power_on_delaytime == 1)
    {
        guc_PTC_heat_power_on_delaytime++;
        if(guc_PTC_heat_power_on_delaytime >= gss_factory_parm_value[DISP_FAC_Cd]) //min
        {
            guc_PTC_heat_power_on_delaytime = 0;
            bflg_PTC_heat_power_on_delaytime = 0; 
        }
    }
}

/*********************************************************************************************************************************************
��������: PTC����ģ��Ŀ���

����λ��: 1s��ʱ���� --------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_control_deal(void)
{
    if(bflg_PTC_heat_power_on_delaytime == 0)     //��ʱ����
    {
        if(bflg_prot2_door1_pin == 0)             //����
        { 
            if(bflg_TTEST_fault == 1)             //����
            {
                if(bflg_PTC_heat_on == 1)
                {
                    guc_PTC_heat_fault_on_delaytime++;
                    if(guc_PTC_heat_fault_on_delaytime >= gss_factory_parm_value[DISP_FAC_T1]*60) // min * 60 =s
                    {
                        guc_PTC_heat_fault_on_delaytime = 0;
                        bflg_PTC_heat_on = 0;
                    }
                }
                else
                {
                    guc_PTC_heat_fault_on_delaytime++;
                    if(guc_PTC_heat_fault_on_delaytime >= gss_factory_parm_value[DISP_FAC_T2]*60) 
                    {
                        guc_PTC_heat_fault_on_delaytime = 0;
                        bflg_PTC_heat_on = 1;
                    }
                }
            }
            else
            {
                if(bflg_PTC_heat_on == 1)
                {
                    if(gss_TTEST >= gss_user_set_parm_value[DISP_USER_SET_TS] + gss_factory_parm_value[DISP_FAC_r0])
                    {
                        bflg_PTC_heat_on = 0;
                    }
                }
                else
                {
                    if(gss_TTEST <= gss_user_set_parm_value[DISP_USER_SET_TS] - gss_factory_parm_value[DISP_FAC_r0])
                    {
                        bflg_PTC_heat_on = 1;
                    } 
                }
            }
        }
        else 
        {
            bflg_PTC_heat_on = 0;
        }
    }
}

/*********************************************************************************************************************************************
��������:�ж��Ƿ�Ƶ��pwm��ʽ����

       CN5��һ�ž���P82��ԭ����ɫ�Ƶĺ��
       Ҫ��33Hz 25%ռ�ձ�      
       T= 1/30s = 33333us                     
       �ߵ�ƽռ��ʱ�䣺25% * T 
       �͵�ƽռ��ʱ�䣺33333 - 25% * T 
       
����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_fre_mode_judge(void)
{
    if(bflg_PTC_heat_on == 1)
    {        
        if(gss_user_set_parm_value[DISP_USER_SET_TS] - gss_TTEST >= gss_factory_parm_value[DISP_FAC_r1])
        {
            bflg_PTC_fre_control = 0;   //����Ƶ�ʿ���

            //debug + ����Ƶ�����
            //bflg_PTC_fre_control = 1;
        }
        else
        {
            bflg_PTC_fre_control = 1;   //��Ƶ�ʿ���
        }
    }
    else
    {        
        bflg_PTC_fre_control = 0;       //����Ƶ�ʿ���
    }
}

/*********************************************************************************************************************************************
��������: PTC��ʵ�������1��һֱ��� 2��Ƶ����� 3�������
       
����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void PTC_heat_RL_out(void)
{    
    if(bflg_PTC_heat_on == 1)             //����
    {  
        RL_PTC_OUT_PIN = 1;               //CN29����˿�ֻ�乩�����ã����������ڲ���ʹ��
        //-------------------------------
        if(bflg_PTC_fre_control == 0)     
        {  
            if(bflg_PTC_alway_out == 0)
            {
                bflg_PTC_alway_out = 1;   //һֱ���
                bflg_PTC_fre_out = 0;
                bflg_PTC_no_out = 0;
                
                RED_LED_PIN = PTC_OUT_ON;  
                R_TAU0_Channel6_Stop();  
            }
        }
        else
        {
            if(bflg_PTC_fre_out == 0)
            {
                bflg_PTC_alway_out = 0;   
                bflg_PTC_fre_out = 1;     //Ƶ�����
                bflg_PTC_no_out = 0;
                R_TAU0_Channel6_Start();  
            }
        }
    }
    else
    {
        
        RL_PTC_OUT_PIN = 0;
        //-------------------------
        if(bflg_PTC_no_out == 0)
        {
            bflg_PTC_alway_out = 0;   
            bflg_PTC_fre_out = 0;     
            bflg_PTC_no_out = 1;         //�����
            
            RED_LED_PIN = PTC_OUT_OFF;     
            R_TAU0_Channel6_Stop();  
        } 
    }
}

/*********************************************************************************************************************************************
��������: PTCģ���Ƶ�ʿ���ʱ�Ĵ���
          Ҫ�����30HZ,ռ�ձ�Ϊ25%----->��������Ϊ33333us(��ʱ��6�ײ�����Ϊ33333us)
                                          25%*33333 = 8333 = 0x208d
                                          33333 - 8333 = 0x61a8

          ����ʽ��ÿһ�ν��ж�ʱ��תpwm��ƽ��Ҳ����װ���´ν��жϵ�ʱ��(���ߵ͵�ƽ��ʱ��)
       
����λ��: ��ʱ��6ϵͳ�ж�--------------------------ok
**********************************************************************************************************************************************/
void PTC_fre_RL_deal(void)
{
    if(bflg_pwm_level == PWM_HIGH_LEVEL)                  //���ж�pwmΪ��ʱ����Ҫ��Ϊ�͵�ƽ
    {
        bflg_pwm_level = PWM_LOW_LEVEL;
        
        //TDR06 = PWM_LOW_LEVEL_CNT;                       //�����趨��ʱ�ж�����
        TDR06 = (uint16_t)(33333 - ((PWM_PERIOD * gss_pwm_adjust)/100)); //����ռ�ձ�+�����ڶ���ɾ��
        TS0 |= _0040_TAU_CH6_START_TRG_ON;                 //������ʱ�����ɽ�TCR����װ��TDR��ֵ�������¼���

        RED_LED_PIN = PTC_OUT_OFF;                         //�� 
    }
    else
    {
        bflg_pwm_level = PWM_HIGH_LEVEL;
        
        //TDR06 = PWM_HIGH_LEVEL_CNT;  
        TDR06 =  (uint16_t)((PWM_PERIOD * gss_pwm_adjust)/100);
        TS0 |= _0040_TAU_CH6_START_TRG_ON;  

        RED_LED_PIN = PTC_OUT_ON;             //��     
    } 
}
/**********************************END OF THE FILE*************************************************/
