/*************************************************************************************************************
overview:20170406
    ���ļ�Ϊ������Χ�ĸ��أ� 1�����    --CN29(FAN2)              P42
                             2��������  --CN17(1.2��)             P111
                             3��ɱ����  --CN17(3.4��)��CN29(FAN1) P146 �� P73 ��2��
                             
*************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "h_type_define.h"
#include "h_ad.h"
#include "m_key_operation.h"
#include "m_peripheral_control.h"
#include "m_PTC_heat.h"
#include "h_protect.h"
#include "h_main.h"
#include "h_com.h"
//----------------------------------------------------------------------------------------------------------
//��������
void fan_control_deal(void);
void fan_RL_out(void);
void led_control_deal(void);
void led_key_on_delaytime(void);
void led_RL_out(void);
void germicidal_lamp_control_deal(void);
void germicidal_lamp_delaytime(void);
void germicidal_lamp_RL_out(void);
void remote_RL_alarm_lamp(void);



//�궨��
#define   DOOR_IN_PIN              P5.2        //�ſ��������  
#define   FAN_RL_OUT_PIN           P4.2        //����������
#define   LED_RL_OUT_PIN           P11.1       //�������������
#define   GERMICIDAL_RL_OUT_PIN_1  P14.6       //ɱ�����������1
#define   GERMICIDAL_RL_OUT_PIN_2  P7.3        //ɱ�����������2

#define   DOOR_OPEN                1           
#define   DOOR_CLOSE               0  

//��־����
flag_type bflg_peripheral_control; 

//����
uint8_t guc_led_on_delaytime;                 //����������ʱ
uint8_t guc_germicidal_lamp_on_delaytime;     //ɱ���ƿ���ʱ


/*********************************************************************************************************************************************
��������: �������

����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void fan_control_deal(void)
{
    if(DOOR_IN_PIN == DOOR_OPEN)
    {
        bflg_fan_on = 0;
    }
    else
    {
        bflg_fan_on = 1;
       /*
        if(bflg_PTC_heat_on == 1)//��PTC��ص�ȥ����ֻ�Ϳ��������
        {
            bflg_fan_on = 1;
        }
        else
        {
            bflg_fan_on = 0;
        }
        */
    }
}

/*********************************************************************************************************************************************
��������: ��������������

����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void fan_RL_out(void)
{
    if(bflg_fan_on == 1)
    {
        FAN_RL_OUT_PIN = 1;
    }
    else
    {
        FAN_RL_OUT_PIN =0;
    }
}

/*********************************************************************************************************************************************
��������: �����ƿ���

����λ��: ��ѭ��(��������ʱ��)--------------------------ok
**********************************************************************************************************************************************/
void led_control_deal(void)
{
    if(gss_liquid_led_button == 1)
    {
        bflg_led_on = 1;
    }
    else
    {
        bflg_led_on = 0;
    }
        
   /* if(bflg_germicidal_lamp_on == 1)
    {
        bflg_led_on = 0;
    }
    else
    {
        if(DOOR_IN_PIN == DOOR_OPEN)   //��������ʱֻ���ſ��ض������ư�����ɱ������
        {
            bflg_led_on = 1;
        }
        else
        {
            bflg_led_on = 0;
        }
        */
        
        /*  //ע�͵��ⲿ������ʽ�������ƿ���,�Ȳ���
        if(guc_key_val == KEY_LAMP)      //�еư�������
        {
            if(bflg_led_on == 1)
            {
                bflg_led_on = 0;
                bflg_led_on_delaytime = 0;
                guc_led_on_delaytime = 0;
            }
            else
            {
                bflg_led_on = 1;
                bflg_led_on_delaytime = 1;
                guc_led_on_delaytime = 0;
            }
        }
        else
        {
            if(DOOR_IN_PIN == DOOR_OPEN)
            {
                bflg_led_on = 1;
            }
            else
            {
                if(bflg_led_on_delaytime == 0)
                {
                    bflg_led_on = 0;
                }
            }
        }
    }*/
}

/*********************************************************************************************************************************************
��������: �����ư���������ʱ(��ֻ�ǰ�����û�а���������ʱ��ر�)

����λ��: 1min��ʱ����-------------------------ok
**********************************************************************************************************************************************/
void led_key_on_delaytime(void)
{
    if(gss_user_set_parm_value[DISP_USER_SET_Lt] != -1)
    {
        if(bflg_led_on_delaytime == 1)
        {
            guc_led_on_delaytime++;
            if(guc_led_on_delaytime >= gss_user_set_parm_value[DISP_USER_SET_Lt])  //2min
            {
                guc_led_on_delaytime = 0;
                bflg_led_on_delaytime = 0;
            }
        }
    }
}

/*********************************************************************************************************************************************
��������: �������������

����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void led_RL_out(void)
{
    if(bflg_led_on == 1)
    {
        LED_RL_OUT_PIN = 1;
    }
    else
    {
        LED_RL_OUT_PIN = 0;
    }
}

/*********************************************************************************************************************************************
��������: ɱ���ƿ���

����λ��: ��ѭ��(������ʱ��)-------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_control_deal(void)
{
    if(gss_liquid_ozone_disinfect_button == 1)
    {
        bflg_germicidal_lamp_on = 1;
    }
    else
    {
        bflg_germicidal_lamp_on = 0;
    }
    /*
    if(bflg_led_on == 1)
    {
        bflg_germicidal_lamp_on = 0;
    }
    else
    {
        if(guc_key_val == KEY_LAMP)          //��ʱ�������ư������
        {
            if(bflg_germicidal_lamp_on == 1)
            {
                bflg_germicidal_lamp_on = 0;
                bflg_germicidal_lamp_on_delaytime = 0;
                guc_germicidal_lamp_on_delaytime = 0;
            }
            else
            {
                bflg_germicidal_lamp_on = 1;
                bflg_germicidal_lamp_on_delaytime = 1;
                guc_germicidal_lamp_on_delaytime = 0;
            }
        }*/
        /*
        else
        {
            if(DOOR_IN_PIN == DOOR_OPEN)
            {
                if(bflg_germicidal_lamp_on_delaytime == 0)
                {
                    bflg_germicidal_lamp_on = 0;
                }
            }
            else
            {   
                if(bflg_germicidal_lamp_on_delaytime == 0)
                {
                    bflg_germicidal_lamp_on = 1;
                    bflg_germicidal_lamp_on_delaytime = 1;
                    guc_germicidal_lamp_on_delaytime = 0;
                }
            }
        }
    }*/
}
/*********************************************************************************************************************************************
��������: ɱ������ʱ

����λ��: 1min��ʱ��--------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_delaytime(void)
{
    if(gss_user_set_parm_value[DISP_USER_SET_S2] != -1)
    {
        if(bflg_germicidal_lamp_on_delaytime == 1)
        {
            guc_germicidal_lamp_on_delaytime++;
            if(guc_germicidal_lamp_on_delaytime >= gss_user_set_parm_value[DISP_USER_SET_S2])  //min
            {
                guc_germicidal_lamp_on_delaytime = 0;
                bflg_germicidal_lamp_on_delaytime = 0;
        
                bflg_germicidal_lamp_on = 0;
            }
        }
    }
}

/*********************************************************************************************************************************************
��������: ɱ�����������

����λ��: ��ѭ��--------------------------ok
**********************************************************************************************************************************************/
void germicidal_lamp_RL_out(void)
{
    if(bflg_germicidal_lamp_on == 1)
    {
        GERMICIDAL_RL_OUT_PIN_1 = 1;
        GERMICIDAL_RL_OUT_PIN_2 = 1;
    }
    else
    {
        GERMICIDAL_RL_OUT_PIN_1 = 0;
        GERMICIDAL_RL_OUT_PIN_2 = 0;
    }
}

/**********************************************************************************************************************************************
��������: Զ�̱�����

����λ��:  ��ѭ��----------------------------------------------ok
***********************************************************************************************************************************************/
void remote_RL_alarm_lamp(void)
{
    if(flg_alarm.uword != 0)
    {
        bflg_alarm_output = 1; //ͨѶ����״̬
        RL_ALM_OUT_PIN = 1;    //��
    }
    else
    {
        bflg_alarm_output = 0;
        RL_ALM_OUT_PIN = 0;    //��
    }
}

/**********************************END OF THE FILE*************************************************/
