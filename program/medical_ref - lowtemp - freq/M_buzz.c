#ifndef _BUZZ_C_
#define _BUZZ_C_


//-----------------------------------------------------------------------------------
//�������ļ�����
#include "h_type_define.h"
#include "r_cg_timer.h"
#include "h_protect.h"

#include "M_buzz.h"
#include "h_main.h"
//----------------------------------------------------------------------------------
//��������
void buzz_loop_tick(void);                //�̵�ѭ��
void buzz_loop_tick_delaytime(void);  //�̵����ʱ��
void buzz_one_tick(void);                 //����һ��
void buzz_one_tick_delaytime(void);   //�������ʱ��
void buzz_always_sound(void);            //����
void buzz_sound_priority(void);         //�̵�ѭ��������һ������������ȼ�
void buzz_sound_cnt(void);



//---------------------------------------------------------------------------------
//��������
flag_type flg_buzz;

//------------------------------------------------------------------------------
int16_t   gss_buzz_loop_tick_delaytime;  
int16_t   gss_buzz_one_tick_delaytime;

uint8_t   guc_buzz_sound_cnt;            //�Զ������������������͸�ֵ���ٸ���
uint8_t   guc_buzz_sound_cnt_delaytime;

/********************************
 ������
********************************/


//---------------------------------------------------------------------------------
//���庯��

/*********************************************************************************
function: ��������һ��

֮ǰ���⼰�������:         
         �̵�:  �� 1s �е���   
       
         ��೤ʱ�䲻�ܿ��ƣ�ֻ���ڼ���Ķ�ʱ�е��ã�
         
         solution:  bflg_alarm_buzz_output  �ķ�ת�ڼ�ʱ�����е���
         
 ��������: ���Ʒ������Ŀ�/�أ�
 
 ����ӿ�: bflg_alarm_buzz_output   bflg_alarm_buzz_off_delaytime

 ִ������: �б�����δ��"����ȡ��"
 ��������: �������Ŀ��أ�

 ����ӿ�: NO

 
 ����λ��: �������е���
*********************************************************************************/
void buzz_loop_tick(void)
{
    if (bflg_buzz_loop_tick_output == 1)
    {
        bflg_buzz_loop_tick_output = 0;    

        //if (bflg_alarm_buzz_off_delaytime == 0)   //δ��"����ȡ��" �������죻
        //{
            R_TAU0_Channel3_Start();                //�������: 1���б��� 2��δ����"����ȡ��"�� 
        //}		    
    }
    else
    {
        R_TAU0_Channel3_Stop();
    }	
}

/*********************************************************************************
��������: �������εĶ̵� �������ȵĿ��ƣ������趨�� 1s;

����ӿ�: bflg_buzz_short_tick_set    ��⵽�ж̵εı�־��1��(����Ӧ�ı���ʱ����1)

ִ������: bflg_buzz_short_tick_set == 1 ;
��������: bflg_buzz_short_tick_output      ���з�ת��������0


����ӿ�: bflg_buzz_short_tick_output  ��ֵ��


����λ��: 100ms �е���;   

У��:   >= 1  ---->   > 1
*********************************************************************************/
void buzz_loop_tick_delaytime(void)
{
    if (bflg_buzz_loop_tick_set == 1)    
    {
        gss_buzz_loop_tick_delaytime++;
        if (gss_buzz_loop_tick_delaytime > 1)     //�趨ʱ�� 100ms * 1 = 100 ms
        {
            //bflg_buzz_loop_tick_output = ~bflg_buzz_loop_tick_output;

            bflg_buzz_loop_tick_output = 1;
            gss_buzz_loop_tick_delaytime = 0;
        }
    }
    else
    {
        bflg_buzz_loop_tick_output = 0;
        gss_buzz_loop_tick_delaytime = 0;
    }
}


/*********************************************************************************
��������: ���Ʒ������Ŀ�/�أ�

����ӿ�: bflg_buzz_long_tick_output    

ִ������: ����"����ȡ��" 5s
��������: �������Ŀ��أ�

����ӿ�: NO


����λ��: �������е���         
*********************************************************************************/
void buzz_one_tick(void)
{
    if (bflg_buzz_one_tick_set == 1)
    {
        if (bflg_buzz_one_tick_run == 0) 
        {
            bflg_buzz_one_tick_run = 1;
            R_TAU0_Channel3_Start();
        }
    }
}


/*********************************************************************************
��������: �������εĳ��� �������ȵĿ��ƣ������趨�� 3s;

����ӿ�: bflg_buzz_long_tick_set    ��⵽�г��εı�־��1��(����ʱ����)

ִ������: bflg_buzz_long_tick_set == 1 ;
��������: bflg_buzz_long_tick_output ���з�ת��������0


����ӿ�: bflg_buzz_long_tick_output  ��ֵ��


����λ��: 100ms �е���;       
*********************************************************************************/
void buzz_one_tick_delaytime(void)
{
    if (bflg_buzz_one_tick_set == 1)	  
    {
        gss_buzz_one_tick_delaytime++;
        if (gss_buzz_one_tick_delaytime >= 5)   // 100 * 30 = 3000ms = 3s
        {
            R_TAU0_Channel3_Stop();

            bflg_buzz_one_tick_run = 0��
            bflg_buzz_one_tick_set = 0;
            gss_buzz_one_tick_delaytime = 0;
        }
    }
}

/*********************************************************************************
function: ����������

����ӿ�: ��⵽�г����־��



����ӿ�:NO

����λ��: 100ms �е���
*********************************************************************************/
void buzz_always_sound(void)
{
    if (bflg_buzz_always_sound_set == 1)
    {      
        if (bfla_buzz_always_sound_run == 0)
        {
            R_TAU0_Channel3_Start();
            bfla_buzz_always_sound_run = 1;
        }
    }
    else
    {   
        bfla_buzz_always_sound_run = 0;
        R_TAU0_Channel3_Stop();
    }
}

/*********************************************************************************
��������:���������졢���Ρ��̵ε����ȼ�˳�� 
         �ڳ���ʱ���������йرշ���������Ϊ�����������еε��������������Ῠ��

�ӿ�����: 

��������: �жϴ�ʱҪִ����һ�����ģʽ��

�ӿ����: NO

����λ��: 100ms �е���
*********************************************************************************/
void buzz_sound_priority(void)
{
    if (bflg_test_mode == 0)                      //���ڲ���ģʽ�²Ŵ����ڲ���ģʽ�²���
    {
        buzz_always_sound();                      // priority_1
        
        if (bflg_buzz_always_sound_set == 0)      //û�г���ʱ
        {   
            buzz_one_tick();                      // priority_2
                                  
            if (bflg_buzz_one_tick_set == 0)      //û�г���ʱ
            {   //debug ����ע��
                //buzz_loop_tick();               // priority_3  �������ʱע��
            }   
        } 
    }
}

/*********************************************************************************
��������:factory ָ���AT����ģʽ�еķ������죻


����λ��: 100ms��ʱ����----------ok

*********************************************************************************/
 void buzz_sound_cnt(void)
{    
    if(guc_buzz_sound_cnt > 0)
    {
        guc_buzz_sound_cnt_delaytime++;
        if(guc_buzz_sound_cnt_delaytime >= 10)  //1s
        {
            guc_buzz_sound_cnt--;
            guc_buzz_sound_cnt_delaytime = 0;
            
            bflg_buzz_one_tick_set = 1;         //��һ��
        }
    }   
}


#endif
/***************************************ENG OF THE FILE*********************************************/
