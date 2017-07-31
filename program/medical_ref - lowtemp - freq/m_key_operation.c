/**************************************************************************************************** 
overview:20170329
                �����Ĳ���:
                            1��ͨ������ѡ����벻ͬ��ģʽ
                            2��ͨ�������Բ������е���
*****************************************************************************************************/

//�������ļ�
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "h_type_define.h"
#include "m_key_operation.h"
#include "h_rom_para2.h"
#include "h_main.h"
//��������
void mode_selection(void);
void key_locked_deal(void);
void key_locked_delaytime(void);   
void key_set_long_delaytime(void);
void key_set_up_long_delaytime(void);
void key_up_down_long_delaytime(void);
void key_set_down_long_delaytime(void);
void key_user_set_deal(void);
void key_user_manage_password_deal(void);
void key_user_manage_deal(void);
void key_factory_password_deal();
void key_factory_deal(void);
void key_inquire_deal(void);
void key_ic_password_deal(void);
void key_ic_register_deal(void);
void key_parm_default_val(void);


//��־����
flag_type bflg_key_operation,bflg_key_operation_save;




//��


//��������
uint8_t   guc_key_val;                      //������ֵ
uint8_t   guc_TUTD_dis_type;                //ѡ����ʾ���� �����»�������ƽ��ֵ
uint8_t   guc_key_set_long_delaytimer;      //����set����ʱ
uint8_t   guc_key_set_up_long_delaytimer;   //����set+up����ʱ
uint8_t   guc_key_up_down_long_delaytimer;  //����up+down����ʱ
uint8_t   guc_key_set_down_long_delaytimer; //����set+down����ʱ
uint8_t   guc_key_locked_delaytimer;        //�����޲���������ʱ

int8_t    gsc_user_manage_password_val;     //�û���������ֵ
int8_t    gsc_factory_password_val;         //��������ֵ
int8_t    gsc_ic_password_val;              //ic������ֵ

uint8_t   guc_disp_mode;                    //�ܵ�UIģʽ
int8_t    gsc_disp_parm_type;               //��ͬ�Ĳ�������,���еĲ����������
int16_t   gss_key_temp_value;               //��������ʱֵ,���еĲ����������

int8_t    gsc_disp_inq_mode;                //��ѯģʽ�в�ͬ����

int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];        //�û��趨����ֵ   
int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM];  //�û��������ֵ   
int16_t   gss_factory_parm_value[DISP_FAC_SUM];              //��������ֵ  


/**********************************************************************************************************************************************
��������: ģʽѡ��,ͨ������ѡ����벻ͬ��ģʽ


����λ��: ��ѭ��(��ȡ������ʱ)----------------------------------------------ok
***********************************************************************************************************************************************/
void mode_selection(void)
{    
    if (bflg_test_type == 1)                     //����ǲ��԰���ʾ��
    {
        if (guc_key_val == 0x20)                 //SET---�����µ����ü���ѪҺ�Ĳ�ͬ
        {
            bflg_test_mode = 1;                  //�ò���ģʽ��־
        } 
        else if(guc_key_val == 0x40)             //BUZZ��
        {
        	bflg_test_mode_RTC = 1;
        	//test_mode_RTC_circuit();
        }
    }
    else if(guc_disp_mode == DISP_LOCKED)
    {
        key_locked_deal();      //����ʱ��������
    }
    else if(guc_disp_mode == DISP_USER_SET)
    {
        key_user_set_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE_PASSWORD)
    {
    	key_user_manage_password_deal();
    }
    else if(guc_disp_mode == DISP_USER_MANAGE)
    {
    	key_user_manage_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY_PASSWORD)
    {
        key_factory_password_deal();
    }
    else if(guc_disp_mode == DISP_FACTORY)
    {
        key_factory_deal();
    }
    else if(guc_disp_mode == DISP_INQUIRE)
    {
        key_inquire_deal();
    }
    else if(guc_disp_mode == DISP_IC_PASSWORD)
    {
        key_ic_password_deal();
    }
    else if(guc_disp_mode == DISP_IC_REGISTER)
    {
        key_ic_register_deal();
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�°����Ĳ���


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_locked_deal(void)
{
    //�����º�ƽ��ֵ��ʾ�л�
    if(guc_key_val == KEY_UP)             //��up�л����º�������
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 1;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    
    if(guc_key_val == KEY_DOWN)            //��down�л����º�������
    {
        if(guc_TUTD_dis_type == 0)
        {
            guc_TUTD_dis_type = 2;
        }
        else
        {
            guc_TUTD_dis_type = 0;
        }
    }
    //--------------------------------------------------------------------------------------------------
    if (guc_key_val == KEY_SET)          //����set��5s����USER SET
    {
        if (bflg_key_set_long_valid == 0)           
        {
            if (bflg_key_set_long_delaytime == 0)    
            {
                bflg_key_set_long_delaytime = 1;     
                guc_key_set_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_long_valid = 0;
            
            guc_disp_mode = DISP_USER_SET;
            gsc_disp_parm_type = DISP_USER_SET_TS;
            gss_key_temp_value = 0;
            bflg_parm_save = 0;
            bflg_parm_dis_form = 0;
        }
    }
    //--------------------------------------------------------------------------------------------------
    else if(guc_key_val == KEY_SET_AND_UP)  //�����趨+�ϵ� 5s���빤������
    {
        if (bflg_key_set_up_long_valid == 0)           
        {
            if (bflg_key_set_up_long_delaytime == 0)    
            {
                bflg_key_set_up_long_delaytime = 1;     
                guc_key_set_up_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_up_long_valid = 0;
            
            guc_disp_mode = DISP_FACTORY_PASSWORD;       
            gsc_factory_password_val = 0;       
        }
    }
    //--------------------------------------------------------------------------------------------------
    else if(guc_key_val == KEY_UP_AND_DOWN)  //�����ϵ�+�µ� 5s�����ѯģʽ
    {
        if (bflg_key_up_down_long_valid == 0)           
        {
            if (bflg_key_up_down_long_delaytime == 0)    
            {
                bflg_key_up_down_long_delaytime = 1;     
                guc_key_up_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_up_down_long_valid = 0;
            
            guc_disp_mode = DISP_INQUIRE;   
            gsc_disp_inq_mode = 0;
            bflg_key_inq_val = 0;
        }
    }    
    //--------------------------------------------------------------------------------------------------    
    else if(guc_key_val == KEY_SET_AND_DOWN)  //��������+�µ� 5s����IC����ģʽ
    {
        if (bflg_key_set_down_long_valid == 0)           
        {
            if (bflg_key_set_down_long_delaytime == 0)    
            {
                bflg_key_set_down_long_delaytime = 1;     
                guc_key_set_down_long_delaytimer = 0;
            }
        }
        else                                     
        {
            bflg_key_set_down_long_valid = 0;
            
            //guc_disp_mode = DISP_IC_PASSWORD;     //��ʱδ������
            gsc_ic_password_val = 0;
        }
    }
    else
    {
        bflg_key_set_long_delaytime = 0;     
        guc_key_set_long_delaytimer = 0;

        bflg_key_set_up_long_delaytime = 0;     
        guc_key_set_up_long_delaytimer = 0;

        bflg_key_up_down_long_delaytime = 0;     
        guc_key_up_down_long_delaytimer = 0;

        bflg_key_set_down_long_delaytime = 0;     
        guc_key_set_down_long_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
��������: 20s ����û�ж�����������״̬


����λ��:  100ms -----------------------------------------ok  
***********************************************************************************************************************************************/
void key_locked_delaytime(void)   
{
    if (guc_key_val == KEY_NONE)
    {
        guc_key_locked_delaytimer++;
        if (guc_key_locked_delaytimer >= 200)    //200*100ms=20s
        {
            guc_key_locked_delaytimer = 0;
            guc_disp_mode = DISP_LOCKED;         
        }
    }
    else
    {
        guc_key_locked_delaytimer = 0;
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���set����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_long_delaytime(void)
{
    if (bflg_key_set_long_delaytime == 1)
    {
        guc_key_set_long_delaytimer++;
        if (guc_key_set_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_long_delaytimer = 0;
            bflg_key_set_long_delaytime = 0;
            
            bflg_key_set_long_valid = 1;       
        }
    }
}
/**********************************************************************************************************************************************
��������: ����ģʽ�³���set+up����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_up_long_delaytime(void)
{
    if (bflg_key_set_up_long_delaytime == 1)
    {
        guc_key_set_up_long_delaytimer++;
        if (guc_key_set_up_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_up_long_delaytimer = 0;
            bflg_key_set_up_long_delaytime = 0;
            
            bflg_key_set_up_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���up+down����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_up_down_long_delaytime(void)
{
    if (bflg_key_up_down_long_delaytime == 1)
    {
        guc_key_up_down_long_delaytimer++;
        if (guc_key_up_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_up_down_long_delaytimer = 0;
            bflg_key_up_down_long_delaytime = 0;
            
            bflg_key_up_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: ����ģʽ�³���set+down����ʱ


����λ��: 100ms��ʱ��----------------------------------------------ok
***********************************************************************************************************************************************/
void key_set_down_long_delaytime(void)
{
    if (bflg_key_set_down_long_delaytime == 1)
    {
        guc_key_set_down_long_delaytimer++;
        if (guc_key_set_down_long_delaytimer >= 50)  //50*100 = 5s
        {
            guc_key_set_down_long_delaytimer = 0;
            bflg_key_set_down_long_delaytime = 0;
            
            bflg_key_set_down_long_valid = 1;       
        }
    }
}

/**********************************************************************************************************************************************
��������: �û��趨��ģʽ�°����Ĳ���

ʹ��ע�⣺����˲�ʱ��
         1��   guc_disp_mode = DISP_USER_SET;
         2��   gsc_disp_parm_type = 0;  Ĭ����ʾ�׸�����
         3��   gss_key_temp_value = 0;      ��ʱ����Ϊ0
         4��   bflg_parm_save = 0;          Ĭ�ϲ�����
         5��   bflg_parm_dis_form = 0��      Ĭ����ʾ��������

����:  �����в����Ĳ���ֵ����1�����Բ���ͳһ         
         
����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_set_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_parm_dis_form = 0;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                    if(gsc_disp_parm_type > DISP_USER_SET_TT)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_TS;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_SET_TS)
                    {
                        gsc_disp_parm_type = DISP_USER_SET_TT;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    if(gss_user_set_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_user_set_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save
                        if(gsc_disp_parm_type == DISP_USER_SET_TS)
                        {
                            ram_para[num_user_set_TS] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_TS, ram_para[num_user_set_TS]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                        {
                            ram_para[num_user_set_ALH] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_ALH, ram_para[num_user_set_ALH]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                        {
                            ram_para[num_user_set_ALL] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_ALL, ram_para[num_user_set_ALL]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                        {
                            ram_para[num_user_set_Lt] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_Lt, ram_para[num_user_set_Lt]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                        {
                            ram_para[num_user_set_dA] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_dA, ram_para[num_user_set_dA]);
                        }   
                        else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                        {
                            ram_para[num_user_set_S1] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_S1, ram_para[num_user_set_S1]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                        {
                            ram_para[num_user_set_S2] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_S2, ram_para[num_user_set_S2]);
                        }
                        else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                        {
                            ram_para[num_user_set_TT] = gss_user_set_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_user_set_TT, ram_para[num_user_set_TT]);
                        }                         
                    }  
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value++; 
                }
                else
                {
                    gss_key_temp_value++;         

                    if(gsc_disp_parm_type == DISP_USER_SET_TS)
                    {
                        if(gss_key_temp_value > 500)    //50��  magnify 10
                        {
                            gss_key_temp_value = 500;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                    {
                        if(gss_key_temp_value > 510)     //51��  magnify 10
                        {
                            gss_key_temp_value = 510;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                    {
                        if(gss_key_temp_value > (gss_user_set_parm_value[DISP_USER_SET_TS] - 10))    
                        {
                            gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS] - 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                    {
                        if(gss_key_temp_value > 30)     //min
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                    {
                        if(gss_key_temp_value > 30)      //min
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                    {
                        if(gss_key_temp_value > 1)  
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                    {
                        if(gss_key_temp_value > 999)    
                        {
                            gss_key_temp_value = 999;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                    {
                        if(gss_key_temp_value > 1)    
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_set_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value--; 
                }
                else
                {
                    gss_key_temp_value--;         

                    if(gsc_disp_parm_type == DISP_USER_SET_TS)
                    {
                        if(gss_key_temp_value < 260)    //26��  magnify 10
                        {
                            gss_key_temp_value = 260;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALH)
                    {
                        if(gss_key_temp_value < (gss_user_set_parm_value[DISP_USER_SET_TS]+10))      
                        {
                            gss_key_temp_value = gss_user_set_parm_value[DISP_USER_SET_TS]+10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_ALL)
                    {
                        if(gss_key_temp_value < 250)    
                        {
                            gss_key_temp_value = 250;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_Lt)
                    {
                        if(gss_key_temp_value < -1)     //min
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_dA)
                    {
                        if(gss_key_temp_value < 1)      //min
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S1)
                    {
                        if(gss_key_temp_value < 0)  
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_S2)
                    {
                        if(gss_key_temp_value < -1)    
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_SET_TT)
                    {
                        if(gss_key_temp_value < 0)    
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                }
            }
        }
    }
}


/**********************************************************************************************************************************************
��������: ���û��趨������û������ʱ���������

ʹ��ע��: ����˲�Ӧ
          1��  guc_disp_mode = DISP_USER_MANAGE_PASSWORD;
          2��  gsc_user_manage_password_val = 0;  

����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_user_manage_password_val++;
        if (gsc_user_manage_password_val > 99)
        {
            gsc_user_manage_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_user_manage_password_val--;
        if (gsc_user_manage_password_val < 0)
        {
            gsc_user_manage_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_user_manage_password_val == 6)                //��ʱ��Ĭ��
        {
            guc_disp_mode = DISP_USER_MANAGE; 
            gsc_disp_parm_type = 0;
            gss_key_temp_value = 0;    
            bflg_parm_save = 0;       
            bflg_parm_dis_form = 0��   
        }
    }
}


/**********************************************************************************************************************************************
��������: �û������İ�������

ʹ��ע�⣺����˲�ʱ��
         1��   guc_disp_mode = DISP_USER_MANAGE;
         2��   gsc_disp_parm_type = 0;     Ĭ����ʾ�׸�����
         3��   gss_key_temp_value = 0;     ��ʱ����Ϊ0
         4��   bflg_parm_save = 0;         Ĭ�ϲ�����
         5��   bflg_parm_dis_form = 0��     Ĭ����ʾ��������

����:  �����в����Ĳ���ֵ����1�����Բ���ͳһ    


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_user_manage_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_USER_MANAGE_TT)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_IC;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_USER_MANAGE_IC)
                    {
                        gsc_disp_parm_type = DISP_USER_MANAGE_TT;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)  //special
                    {
                         
                    }
                    else if(gss_user_manage_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_user_manage_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save_wait
                        /*
                        ram_para[num_usb_read_delaytime] = gss_user2_T1;                                                                    //!!!!!!!
                        eeprom_write(num_usb_read_delaytime, ram_para[num_usb_read_delaytime]); 
                        */
                        //eeprom_write(num_user_set_TS, gss_user_set_parm_value[DISP_USER_SET_TS]); 
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value++; 
                }
                else
                {
                    gss_key_temp_value++;         

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                    {
                        if(gss_key_temp_value > 999)    
                        {
                            gss_key_temp_value = 999;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                    {
                        // wait
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                    {
                        if(gss_key_temp_value > 60)    
                        {
                            gss_key_temp_value = 60;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                    {
                        if(gss_key_temp_value > 30)     
                        {
                            gss_key_temp_value = 30;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                    {
                        if(gss_key_temp_value > 3)     
                        {
                            gss_key_temp_value = 3;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                    {
                        if(gss_key_temp_value > 999)     
                        {
                            gss_key_temp_value = 999;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }                      
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_user_manage_parm_value[gsc_disp_parm_type];
                    gss_key_temp_value--; 
                }
                else
                {
                    gss_key_temp_value--;         

                    if(gsc_disp_parm_type == DISP_USER_MANAGE_IC)
                    {
                        if(gss_key_temp_value < 0)    
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_CL1)
                    {
                        // wait
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_Lt)
                    {
                        if(gss_key_temp_value < -1)    
                        {
                            gss_key_temp_value = -1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_dA)
                    {
                        if(gss_key_temp_value < 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_FA)
                    {
                        if(gss_key_temp_value < 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S1)
                    {
                        if(gss_key_temp_value > 1)     
                        {
                            gss_key_temp_value = 1;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_S2)
                    {
                        if(gss_key_temp_value < 0)     
                        {
                            gss_key_temp_value = 0;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_USER_MANAGE_TT)
                    {
                        if(gss_key_temp_value < 0)     
                        {
                            gss_key_temp_value = 0;
                        }
                    }                      
                }
            }
        }
    }
}


/**********************************************************************************************************************************************
��������: ��������ģʽ�µĲ���


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_factory_password_val++;
        if (gsc_factory_password_val > 99)
        {
            gsc_factory_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_factory_password_val--;
        if (gsc_factory_password_val < 0)
        {
            gsc_factory_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_factory_password_val == 6)                //��ʱ��Ĭ��
        {
            guc_disp_mode = DISP_FACTORY; 
            gsc_disp_parm_type = 0;   
            gss_key_temp_value = 0;      
            bflg_parm_save = 0;          
            bflg_parm_dis_form = 0��     
        }
    }
}


/**********************************************************************************************************************************************
��������: ����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)---------------------------------------------ok
***********************************************************************************************************************************************/
void key_factory_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
    }
    else
    {
        if(bflg_parm_dis_form == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_parm_type++;
                   
                    if(gsc_disp_parm_type > DISP_FAC_Fd)
                    {
                        gsc_disp_parm_type = DISP_FAC_r0;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_parm_type--;
                    if(gsc_disp_parm_type < DISP_FAC_r0)
                    {
                        gsc_disp_parm_type = DISP_FAC_Fd;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_parm_dis_form = 0;
                
                if(bflg_parm_save == 1)    
                {
                    bflg_parm_save = 0;

                    //have special
                    if(gss_factory_parm_value[gsc_disp_parm_type] != gss_key_temp_value) //save
                    {
                        gss_factory_parm_value[gsc_disp_parm_type] = gss_key_temp_value;
                        gss_key_temp_value = 0;

                        //save_wait
                        if(gsc_disp_parm_type == DISP_FAC_r0)
                        {
                            ram_para[num_fac_r0] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_r0, ram_para[num_fac_r0]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_r1)
                        {
                            ram_para[num_fac_r1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_r1, ram_para[num_fac_r1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                        {
                            ram_para[num_fac_Ct1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Ct1, ram_para[num_fac_Ct1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                        {
                            ram_para[num_fac_Ct2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Ct2, ram_para[num_fac_Ct2]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Cd)
                        {
                            ram_para[num_fac_Cd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Cd, ram_para[num_fac_Cd]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_T1)
                        {
                            ram_para[num_fac_T1] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_T1, ram_para[num_fac_T1]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_T2)
                        {
                            ram_para[num_fac_T2] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_T2, ram_para[num_fac_T2]);
                        }
                        else if(gsc_disp_parm_type == DISP_FAC_Fd)
                        {
                            ram_para[num_fac_Fd] = gss_factory_parm_value[gsc_disp_parm_type];                                                                    //!!!!!!!
                            eeprom_write(num_fac_Fd, ram_para[num_fac_Fd]);
                        }                        
                    }
                }
            }
            else if(guc_key_val == KEY_UP)
            {
                if(bflg_parm_save == 0)          //�ս���ʱ�Ƚ�ԭ����ֵ������ʱֵ
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];

                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1)) //��������������0.5
                    {
                        gss_key_temp_value += 5; 
                    }
                    else
                    {
                        gss_key_temp_value++; 
                    }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value += 5; 
                     }
                     else
                     {
                         gss_key_temp_value++; 
                     }

                    if(gsc_disp_parm_type == DISP_FAC_r0)
                    {
                        if(gss_key_temp_value > 100)     // 10�� magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_r1)
                    {
                        if(gss_key_temp_value > 300)    // 30�� magnify 10
                        {
                            gss_key_temp_value = 300;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                    {
                        if(gss_key_temp_value > 100)    //10�� magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                    {
                        if(gss_key_temp_value > 100)     //10�� magnify 10
                        {
                            gss_key_temp_value = 100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Cd)
                    {
                        if(gss_key_temp_value > 15)     //min
                        {
                            gss_key_temp_value = 15;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T1)
                    {
                        if(gss_key_temp_value > 10)     //min
                        {
                            gss_key_temp_value = 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T2)
                    {
                        if(gss_key_temp_value > 10)    //min  
                        {
                            gss_key_temp_value = 10;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_FAC_Fd)
                    {
                        if(gss_key_temp_value > 3)    //���� ʹ��ʱ*10 
                        {
                            gss_key_temp_value = 3;
                        }
                    }   
                }
            }
            else if(guc_key_val == KEY_DOWN)
            {
                if(bflg_parm_save == 0)
                {
                    bflg_parm_save = 1;
                    gss_key_temp_value = gss_factory_parm_value[gsc_disp_parm_type];
                    
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value -= 5; 
                     }
                     else
                     {
                         gss_key_temp_value--; 
                     }
                }
                else
                {
                    if((gsc_disp_parm_type == DISP_FAC_r0)||(gsc_disp_parm_type == DISP_FAC_r1))
                     {
                         gss_key_temp_value -= 5; 
                     }
                     else
                     {
                         gss_key_temp_value--; 
                     }

                    if(gsc_disp_parm_type == DISP_FAC_r0)
                    {
                        if(gss_key_temp_value < -100)    // -10�� magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_r1)
                    {
                        if(gss_key_temp_value < 10)    // 1�� magnify 10
                        {
                            gss_key_temp_value = 10;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct1)
                    {
                        if(gss_key_temp_value < -100)    //-10�� magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Ct2)
                    {
                        if(gss_key_temp_value < -100)     //-10�� magnify 10
                        {
                            gss_key_temp_value = -100;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_Cd)
                    {
                        if(gss_key_temp_value < 0)        //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T1)
                    {
                        if(gss_key_temp_value < 0)       //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }
                    else if(gsc_disp_parm_type == DISP_FAC_T2)
                    {
                        if(gss_key_temp_value < 0)       //min
                        {
                            gss_key_temp_value = 0;
                        }
                    }  
                    else if(gsc_disp_parm_type == DISP_FAC_Fd)
                    {
                        if(gss_key_temp_value < 1)      //����
                        {
                            gss_key_temp_value = 1;
                        }
                    }   
                }
            }
        }
    }
}



/**********************************************************************************************************************************************
��������: ����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_inquire_deal(void)
{
    if(guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;
        bflg_key_inq_val = 0;
    }
    else
    {
        if(bflg_key_inq_val == 0)
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 1;
            }
            else
            {
                if(guc_key_val == KEY_UP)
                {
                    gsc_disp_inq_mode++;
                    if(gsc_disp_inq_mode > DISP_INQ_d9)
                    {
                        gsc_disp_inq_mode = DISP_INQ_d1;
                    }
                }
                else if(guc_key_val == KEY_DOWN)
                {
                    gsc_disp_inq_mode--;
                    if(gsc_disp_inq_mode < DISP_INQ_d1)

                    {
                        gsc_disp_inq_mode = DISP_INQ_d9;
                    }
                }
            }
        }
        else
        {
            if(guc_key_val == KEY_SET)
            {
                bflg_key_inq_val = 0;
            }
        }
    }
}

/**********************************************************************************************************************************************
��������:IC����ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_password_deal(void)
{  
    if (guc_key_val == KEY_BUZ)
    {
        guc_disp_mode = DISP_LOCKED;   
    }
    else if (guc_key_val == KEY_UP)
    {          
        gsc_ic_password_val++;
        if (gsc_ic_password_val > 99)
        {
            gsc_ic_password_val = 0;
        }
    }
    else if (guc_key_val == KEY_DOWN)
    {    
        gsc_ic_password_val--;
        if (gsc_ic_password_val < 0)
        {
            gsc_ic_password_val = 99;
        }
    }
    else if (guc_key_val == KEY_SET)
    {           
        if (gsc_ic_password_val == 8)                //��ʱ��Ĭ��
        {
            guc_disp_mode = DISP_IC_REGISTER;                       
            gsc_ic_password_val = 0;                 
        }
    }
}

/**********************************************************************************************************************************************
��������:ICע��ģʽ�µİ�������


����λ��: mode_selection(����ѭ��)----------------------------------------------ok
***********************************************************************************************************************************************/
void key_ic_register_deal(void)
{
    if(guc_key_val == KEY_BUZ)                
    {
        //bflg_IC_register_state = 0;         // �˳� ICע��״̬
        //guc_temp_flash_add = 0;
        //guc_flash_add = 0;

        guc_disp_mode = DISP_LOCKED;           
    }     
}

/**********************************************************************************************************************************************
��������: �û��趨�������û��������������������Ĭ��ֵ���Ժ�Ҫ��E2��ȡ��


����λ��: ��ʼ����---------------------------------------------ok
***********************************************************************************************************************************************/
void key_parm_default_val(void)
{   
    //�û��趨����
    gss_user_set_parm_value[DISP_USER_SET_TS]  = ram_para[num_user_set_TS];  //magnify �õ�ʱ����*10��
    gss_user_set_parm_value[DISP_USER_SET_ALH] = ram_para[num_user_set_ALH]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_ALL] = ram_para[num_user_set_ALL]; //magnify
    gss_user_set_parm_value[DISP_USER_SET_Lt]  = ram_para[num_user_set_Lt];
    gss_user_set_parm_value[DISP_USER_SET_dA]  = ram_para[num_user_set_dA];
    gss_user_set_parm_value[DISP_USER_SET_S1]  = ram_para[num_user_set_S1];
    gss_user_set_parm_value[DISP_USER_SET_S2]  = ram_para[num_user_set_S2];
    gss_user_set_parm_value[DISP_USER_SET_TT]  = ram_para[num_user_set_TT];

    
    //��������
    gss_factory_parm_value[DISP_FAC_r0]  = ram_para[num_fac_r0];   //magnify  
    gss_factory_parm_value[DISP_FAC_r1]  = ram_para[num_fac_r1];   //magnify  
    gss_factory_parm_value[DISP_FAC_Ct1] = ram_para[num_fac_Ct1];  //magnify
    gss_factory_parm_value[DISP_FAC_Ct2] = ram_para[num_fac_Ct2];  //magnify
    gss_factory_parm_value[DISP_FAC_Cd]  = ram_para[num_fac_Cd]; 
    gss_factory_parm_value[DISP_FAC_T1]  = ram_para[num_fac_T1]; 
    gss_factory_parm_value[DISP_FAC_T2]  = ram_para[num_fac_T2]; 
    gss_factory_parm_value[DISP_FAC_Fd]  = ram_para[num_fac_Fd];    //���� ʹ��ʱ*10     

    /*  ѪҺ
    //�û��趨����
    gss_user_set_parm_value[DISP_USER_SET_TS]  = 360;  //magnify �õ�ʱ����*10��
    gss_user_set_parm_value[DISP_USER_SET_ALH] = 400;  //magnify
    gss_user_set_parm_value[DISP_USER_SET_ALL] = 300;  //magnify
    gss_user_set_parm_value[DISP_USER_SET_Lt]  = 2;
    gss_user_set_parm_value[DISP_USER_SET_dA]  = 5;
    gss_user_set_parm_value[DISP_USER_SET_S1]  = 1;
    gss_user_set_parm_value[DISP_USER_SET_S2]  = 30;
    gss_user_set_parm_value[DISP_USER_SET_TT]  = 0;
    
    //�û��������----������δ�ã���Ϊ��ѪҺ��ֲ�����Ա���
    gss_user_manage_parm_value[DISP_USER_MANAGE_IC]  = 8;    
    gss_user_manage_parm_value[DISP_USER_MANAGE_CL1] = 0;
    gss_user_manage_parm_value[DISP_USER_MANAGE_Lt]  = 8;   
    gss_user_manage_parm_value[DISP_USER_MANAGE_dA]  = 5;  
    gss_user_manage_parm_value[DISP_USER_MANAGE_FA]  = 3;   
    gss_user_manage_parm_value[DISP_USER_MANAGE_S1]  = 1;
    gss_user_manage_parm_value[DISP_USER_MANAGE_S2]  = 60;  
    gss_user_manage_parm_value[DISP_USER_MANAGE_TT]  = 0;

    //��������
    gss_factory_parm_value[DISP_FAC_r0]  = 10;   //magnify  
    gss_factory_parm_value[DISP_FAC_r1]  = 100;  //magnify  
    gss_factory_parm_value[DISP_FAC_Ct1] = 0;    //magnify
    gss_factory_parm_value[DISP_FAC_Ct2] = 0;    //magnify
    gss_factory_parm_value[DISP_FAC_Cd] = 0; 
    gss_factory_parm_value[DISP_FAC_T1] = 1; 
    gss_factory_parm_value[DISP_FAC_T2] = 3; 
    gss_factory_parm_value[DISP_FAC_Fd] = 1;     //���� ʹ��ʱ*10 
    */
}



/*******************************************END OF THE FILE*************************************************************/



