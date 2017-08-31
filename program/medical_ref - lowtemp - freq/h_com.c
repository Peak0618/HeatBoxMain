#ifndef _COM_C_
#define _COM_C_
//------------------------------------------------------------------------------
//�����ļ�����
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_intc.h"
#include "r_cg_serial.h"
#include "r_cg_adc.h"
#include "r_cg_timer.h"
#include "r_cg_wdt.h"
#include "r_cg_userdefine.h"

#include "h_type_define.h"
#include "h_com.h"
#include "h_ad.h"
#include "h_key_disp.h"
#include "h_rom_para2.h"
//#include "h_comp2.h"
#include "h_protect.h"
//#include "h_volt_ctrl.h"
#include "h_bat_ctrl.h"

//peak 
#include "h_main.h"
#include "m_key_operation.h"
#include "m_PTC_heat.h"
#include "m_peripheral_control.h"

//------------------------------------------------------------------------------
//��������
void com_init(void);//ͨѶ��ʼ�������ڳ�ʼ�������е���

void com_tx_init(void);  //ͨѶ���ͳ�ʼ����������ѭ�������е���

void com_rx_init(void);  //ͨѶ���ճ�ʼ����������ѭ�������е���

void com_rx_data_deal(void);  //ͨѶ�������ݴ����������ѭ�������е���

void com_tx_delaytime(void);  //ͨѶ������ʱ������1ms��ʱ�����е���

void com_rx_delaytime(void);  //ͨѶ������ʱ������1ms��ʱ�����е���

void com_rx_end_delaytime(void);   //ͨѶ���������ʱ������1ms��ʱ�����е���

void com_fault_delaytime(void);    //ͨѶ������ʱ������1s��ʱ�����е���

void com_tx_int(void);   //ͨѶ�����жϳ���

void com_rx_int(uint8_t rx_data);  //ͨѶ�����жϳ���

unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen);    //����CRCУ��ĳ���



//------------------------------------------------------------------------------
//��־����
flag_type flg_com;
flag_type flg_read_write;
//------------------------------------------------------------------------------
//��������
uint8_t   guc_com_tx_buffer[120];        //ͨѶ���ͻ�����
uint8_t   guc_com_rx_buffer[80];         //ͨѶ���ջ�����

int16_t   gss_com_tx_delaytimer;        //ͨѶ������ʱ��ʱ��
int16_t   gss_com_rx_delaytimer;        //ͨѶ������ʱ��ʱ��

int16_t   gss_com_rx_end_delaytimer;    //ͨѶ�������־
int16_t   gss_com_fault_delaytimer;     //ͨѶ������ʱ��ʱ��

uint8_t   guc_com_tx_point;   //ͨѶ��������
uint8_t   guc_com_rx_point;   //ͨѶ��������

uint8_t   guc_com_type_cnt;   //ͨѶ���ͼ�����

//peak add
uint8_t  guc_IC_access_state = 0;    // ���յ�IC����ָ��

uint16_t guc_flash_add;              // ���յ�IC��ע�����õĵ�ַ��������ʾ�ж����ſ�ע��
uint16_t guc_temp_flash_add;         // ���յ�IC��ע����ʱ����ĵ�ַ�����������ڵĵ�ַ�Ա��Ƿ����µ�IC��ע�ᣬ����ʱ���������
uint8_t  guc_IC_erasure_tx_ok_count; // ע��IC��ָ���bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����


int16_t gss_liquid_led_button;              //Һ�����Ŀ��ư�ť  ��1�� 0��
int16_t gss_liquid_ozone_disinfect_button;  //Һ����������ɱ����ť��1�� 0��
int16_t gss_pwm_adjust;                     //��PTCģ�����pwmռ�ձȵĵ���

#define   WRITE_STATE_CNT     50

#define   READ_USER_CNT       30
#define   READ_ADMIN_CNT      30

#define   LIQU_ROK_SDELAY     20   //Һ�������ճɹ�����ʱxms�ٷ���
//------------------------------------------------------------------------------
//��������
//------------------------------------------------------------------------------
/*******************************************************************************
function:  ��ʼ��ʱ(��һ��)��Ϊ�˸�����ʱ�򿪷����жϣ��������ݣ�
    

*******************************************************************************/
void com_init(void) //ͨѶ��ʼ�������ڳ�ʼ�������е���
{
    bflg_com_tx_delaytime = 1;     //peak ��ʱʱ�䵽���򿪷����жϣ��������ݣ�
    gss_com_tx_delaytimer = 3000;
    
    bflg_com_rx_delaytime = 0;
    gss_com_rx_delaytimer = 0;
    
    bflg_read_write_data = 1; //�״�д����
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��ÿһ�η�������ʱ�������ʼ����
         LED: ������ʾ��������Ҫ��ʾ������(txt)

         ��ʼ������:
           1�����͵Ĳ�ѯָ��(�̶�)
           2�����͵��趨����---->|a�� "����״̬"��"����"��"����ָʾ"��"��ѹ"��"����"
                                 |b��"�¶��趨ֵ" "���±����趨ֵ" "���±����趨ֵ"

           a---�������ذ屾������ݲɼ����жϣ�
           
           b---����ʾ�尴�����úõ�ֵ�� ��������
               ��ʾ�尴�����ú�--->���͸�����-->�����ٽ���ֵ���͸���ʾ����ʾ��  
               
********************************************************************************/
void com_tx_init(void)   //ͨѶ���ͳ�ʼ����������ѭ�������е���
{
    int8_t i;
    word_type gss_tmp;
    word_type com_crc;
    //----------------------------------------------------------LED��-----------------------------------
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if (bflg_read_write_data == 0)  //����Ƕ�����  //peak ������ֵ
        {
            guc_com_tx_buffer[0] = 0x00;     //�豸��ַ
            guc_com_tx_buffer[1] = 0x03;     //������
            
            guc_com_tx_buffer[2] = 0x00;
            guc_com_tx_buffer[3] = 0x07;     //��ʼ��ַ7
            
            guc_com_tx_buffer[4] = 0x00;
            guc_com_tx_buffer[5] = 2;        //��ַ����
            //----------------------------------------------
            com_crc.uword = CRC16(guc_com_tx_buffer, 6);
            guc_com_tx_buffer[6] = com_crc.ubyte.low;
            guc_com_tx_buffer[7] = com_crc.ubyte.high;
        }
        else   //�����д����   //peak �趨����ܡ�LED��ʾ����
        {
            guc_com_tx_buffer[0] = 0x00;     //�豸��ַ
            guc_com_tx_buffer[1] = 0x10;     //д����
            
            guc_com_tx_buffer[2] = 0x00;
            guc_com_tx_buffer[3] = 0x00;     //��ʼ��ַ0
            
            guc_com_tx_buffer[4] = 0x00;
            guc_com_tx_buffer[5] = 22;        //��ַ����  peak Ҫд�����Ĵ�����ÿ���Ĵ���ռ2���ֽڣ�7*2 = 14 ���ֽڣ�
            
            guc_com_tx_buffer[6] = 44;       //�ֽ���    peak [7]--[20] = 14 ���������

            for (i = 0; i < 12; i++)         // peak [7]~[18] = 12 ;
            {
                guc_com_tx_buffer[i + 7] = guc_led_buffer[i];    //peak ����ܼĴ���
            }           
            
            //----------------------------------------------
            if(bflg_IC_erasure_order == 1)  //����ע��ָ��,ע��IC��ָ��_��bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����
            {               
                guc_com_tx_buffer[19] = 1;
            }
            else
            {
                guc_com_tx_buffer[19] = 0;
            }
            
            if (bflg_alarm_lamp == 0)         //peak ��ʾ���ϵ���λ������ 
            {
                //guc_com_tx_buffer[19] = 0;
                guc_com_tx_buffer[20] = 0x00;
            }
            else
            {
                //guc_com_tx_buffer[19] = 0;
                guc_com_tx_buffer[20] = 0x07;     //peak ��ʾ������� "����" �������� 
            }
        

            //----------------------------------------------
            //com_crc.uword = CRC16(guc_com_tx_buffer, 21);
            //guc_com_tx_buffer[21] = com_crc.ubyte.low;
            //guc_com_tx_buffer[22] = com_crc.ubyte.high;

            com_crc.uword = CRC16(guc_com_tx_buffer, 51);  //21
            guc_com_tx_buffer[51] = com_crc.ubyte.low;     //21
            guc_com_tx_buffer[52] = com_crc.ubyte.high;    //22            
        }
    }
//---------------------------------------------------------------------Һ����------------------
    else  //�����Һ����ͨѶ
    {
        if (bflg_read_write_data == 0)  //����Ƕ�����
        {
            if (bflg_read_user_admin == 0)   //����Ƕ��û���Ϣ
            {
                guc_com_tx_buffer[0] = 0x01;      //�豸��ַ
                guc_com_tx_buffer[1] = 0x03;      //������
                
                guc_com_tx_buffer[2] = 0xA4;
                guc_com_tx_buffer[3] = 0x1F;      //��ʼ��ַ8100
                
                guc_com_tx_buffer[4] = READ_USER_CNT;
                guc_com_tx_buffer[5] = 0x00;       //��ַ����
                //------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
            else    //����Ƕ�������Ϣ
            {
                guc_com_tx_buffer[0] = 0x01;      //�豸��ַ
                guc_com_tx_buffer[1] = 0x03;      //������
                
                guc_com_tx_buffer[2] = 0xD6;
                guc_com_tx_buffer[3] = 0x1F;      //��ʼ��ַ8150
                
                guc_com_tx_buffer[4] = READ_ADMIN_CNT;
                guc_com_tx_buffer[5] = 0x00;       //��ַ����
                //------------------------------------------
                com_crc.uword = CRC16(guc_com_tx_buffer, 6);
                guc_com_tx_buffer[6] = com_crc.ubyte.low;
                guc_com_tx_buffer[7] = com_crc.ubyte.high;
            }
        }
        else   //�����д����
        {
            guc_com_tx_buffer[0] = 0x01;     //�豸��ַ
            guc_com_tx_buffer[1] = 0x10;     //д����
            
            guc_com_tx_buffer[2] = 0x40;
            guc_com_tx_buffer[3] = 0x1F;     //��ʼ��ַ10000
            
            guc_com_tx_buffer[4] = WRITE_STATE_CNT;
            guc_com_tx_buffer[5] = 0x00;    //��ַ����
            
            guc_com_tx_buffer[6] = (WRITE_STATE_CNT * 2);   //�ֽ���
            //----------------------------------------------
            {
                //debug
                //gss_TTEST = -1997;
                
                gss_tmp.sword = gss_TTEST;      //���Դ������¶�
                guc_com_tx_buffer[7] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[8] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gss_TDIS;      //��ʾ�������¶�
                guc_com_tx_buffer[9] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[10] = gss_tmp.ubyte.high;
                //------------------------------------------
                if(bflg_PTC_heat_on == 0)     //PTC����ģ��״̬
                {
                    guc_com_tx_buffer[11] = 0;
                    guc_com_tx_buffer[12] = 0;
                }
                else
                {
                    guc_com_tx_buffer[11] = 1;
                    guc_com_tx_buffer[12] = 0;
                }
                //------------------------------------------
                if(bflg_fan_on == 0)            //�ڷ��״̬
                {
                    guc_com_tx_buffer[13] = 0;
                    guc_com_tx_buffer[14] = 0;
                }
                else
                {
                    guc_com_tx_buffer[13] = 1;
                    guc_com_tx_buffer[14] = 0;
                }
                //------------------------------------------
                if(bflg_led_on == 0)            //������״̬
                {
                    guc_com_tx_buffer[15] = 0;
                    guc_com_tx_buffer[16] = 0;
                }
                else
                {
                    guc_com_tx_buffer[15] = 1;
                    guc_com_tx_buffer[16] = 0;
                }
                //------------------------------------------
                if(bflg_germicidal_lamp_on == 0)  //ɱ����״̬
                {
                    guc_com_tx_buffer[17] = 0;
                    guc_com_tx_buffer[18] = 0;
                }
                else
                {
                    guc_com_tx_buffer[17] = 1;
                    guc_com_tx_buffer[18] = 0;
                }
                //------------------------------------------
                if (DOOR1_IN_PIN == 0)                   // 0����
                {
                    guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0;                
                }
                else
                {
                    guc_com_tx_buffer[19] = 1;
                    guc_com_tx_buffer[20] = 0;
                }
               /* if (bflg_prot2_door1_pin == 0)    //����״̬
                {
                    guc_com_tx_buffer[19] = 0;
                    guc_com_tx_buffer[20] = 0;
                }
                else
                {
                    guc_com_tx_buffer[19] = 1;
                    guc_com_tx_buffer[20] = 0;
                }*/
                
                gss_tmp.sword = gus_trip_code1;   //����״̬��Ϣ1
                guc_com_tx_buffer[21] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[22] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gus_trip_code2;   //����״̬��Ϣ2
                guc_com_tx_buffer[23] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[24] = gss_tmp.ubyte.high;
                //------------------------------------------
                gss_tmp.sword = gss_VAC;          //��Դ��ѹ
                guc_com_tx_buffer[25] = gss_tmp.ubyte.low;
                guc_com_tx_buffer[26] = gss_tmp.ubyte.high;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[27] = 0;
                guc_com_tx_buffer[28] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[29] = 0;
                guc_com_tx_buffer[30] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[31] = 0;
                guc_com_tx_buffer[32] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[33] = 0;
                guc_com_tx_buffer[34] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[35] = 0;
                guc_com_tx_buffer[36] = 0;
                //------------------------------------------
                //
                guc_com_tx_buffer[37] = 0;
                guc_com_tx_buffer[38] = 0;
                //------------------------------------------
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[39] = 0;
                guc_com_tx_buffer[40] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[41] = 0;
                guc_com_tx_buffer[42] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[43] = 0;
                guc_com_tx_buffer[44] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[45] = 0;
                guc_com_tx_buffer[46] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[47] = 0;
                guc_com_tx_buffer[48] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[49] = 0;
                guc_com_tx_buffer[50] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[51] = 0;
                guc_com_tx_buffer[52] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[53] = 0;
                guc_com_tx_buffer[54] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[55] = 0;
                guc_com_tx_buffer[56] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[57] = 0;
                guc_com_tx_buffer[58] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[59] = 0;
                guc_com_tx_buffer[60] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[61] = 0;
                guc_com_tx_buffer[62] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[63] = 0;
                guc_com_tx_buffer[64] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[65] = 0;
                guc_com_tx_buffer[66] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[67] = 0;
                guc_com_tx_buffer[68] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[69] = 0;
                guc_com_tx_buffer[70] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[71] = 0;
                guc_com_tx_buffer[72] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[73] = 0;
                guc_com_tx_buffer[74] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[75] = 0;
                guc_com_tx_buffer[76] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[77] = 0;
                guc_com_tx_buffer[78] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[79] = 0;
                guc_com_tx_buffer[80] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[81] = 0;
                guc_com_tx_buffer[82] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[83] = 0;
                guc_com_tx_buffer[84] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[85] = 0;
                guc_com_tx_buffer[86] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[87] = 0;
                guc_com_tx_buffer[88] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[89] = 0;
                guc_com_tx_buffer[90] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[91] = 0;
                guc_com_tx_buffer[92] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[93] = 0;
                guc_com_tx_buffer[94] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[95] = 0;
                guc_com_tx_buffer[96] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[97] = 0;
                guc_com_tx_buffer[98] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[99] = 0;
                guc_com_tx_buffer[100] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[101] = 0;
                guc_com_tx_buffer[102] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[103] = 0;
                guc_com_tx_buffer[104] = 0;
                //------------------------------------------
                //Ԥ��
                guc_com_tx_buffer[105] = 0;
                guc_com_tx_buffer[106] = 0;
            }
            //----------------------------------------------
            com_crc.uword = CRC16(guc_com_tx_buffer, ((WRITE_STATE_CNT * 2) + 7));   //107
            guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7] = com_crc.ubyte.low;
            guc_com_tx_buffer[(WRITE_STATE_CNT * 2) + 7 + 1] = com_crc.ubyte.high;
        }
    }
    //------------------------------------------------------
    guc_com_tx_point = 0;
    bflg_com_tx_busy = 1;
    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];
}
//------------------------------------------------------------------------------
/******************************************************************************
function:  ��ÿ�ν�������ʱ�������ʼ����
           ����ʱ����Ҫ���յ�������գ�׼�����������ݣ�

******************************************************************************/
void com_rx_init(void)   //ͨѶ���ճ�ʼ����������ѭ�������е���
{
    uint8_t i;
    
    for (i = 0; i < 65; i++)  //20
    {
        guc_com_rx_buffer[i] = 0;  //����ջ�����
    }
    
    bflg_com_rx_busy = 0;
    guc_com_rx_point = 0;
}
//------------------------------------------------------------------------------
/*******************************************************************************
function : ���ճɹ��󣬶����ݽ��д��� ��

           bflg_read_write_data--- �������͸���ʾ�尴��:���Ͳ�ѯָ������趨ֵ
      ����������ѭ�����͵ķ�ʽ��

        ��Ϊ������Ҫ��ͣ���жϴ�ʱ���µİ�������������"���±����趨"��Ҫ�ı��жϵķ�ֵ�ˣ�
            ��ʾ��Ҳ��Ҫ֪����ʱ��������Щ���ϣ���������ʾ��
        ���Զ�/дҪѭ��ִ�У�
********************************************************************************/
void com_rx_data_deal(void)   //ͨѶ�������ݴ����������ѭ�������е���
{
    uint8_t i,msb_byte = 0,lsb_byte = 0;
    uint16_t lus_tmp;
    word_type com_crc;
    word_type gss_tmp;
    //------------------------------------------------------
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����  //peak ��������ֵ
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 7);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[7])
             && (com_crc.ubyte.high == guc_com_rx_buffer[8]))
            {
                bflg_com_type_ok = 1;   //ͨѶ�����ж�ok
                
                bflg_com_tx_delaytime = 1;        //�÷�����ʱ��־
                gss_com_tx_delaytimer = 10;       //������ʱ��ʱ����ֵ
                
                bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                //------------------------------------------
                //���ݴ���
                bflg_read_write_data = 1;    //��Ϊд����
                
                if (guc_key_val != guc_com_rx_buffer[3])
                {
                    bflg_read_key_delaytime = 0; //peak ����ֵ�иı䣬����Ҫ�ȴ�һ��ʱ������жϼ�ֵ������ֱ�Ӵ��� read_key_deal()
                    gss_read_key_delaytimer = 0;
                }
                guc_key_val = guc_com_rx_buffer[3];    

                //ע��״̬ʱ��ʾ��IC����ע��ĵ�ַ����ʾ��ת��Ϊ��Ƭ������ = xx/4;
                guc_flash_add = 0;
                msb_byte = guc_com_rx_buffer[4];
                lsb_byte = guc_com_rx_buffer[5];
                guc_flash_add = ((guc_flash_add | msb_byte)<<8) | lsb_byte;
                 
                //peak add ����ָ��
                if ((guc_com_rx_buffer[6] & 0x80) != 0)
                {
                      guc_IC_access_state = 1;
                }
                else
                {
                      guc_IC_access_state = 0;
                }
                //------------------------------------------
                //���԰汾��ʾ��ȷ��
                if ((guc_com_rx_buffer[6] & 0x40) != 0)
                {
                	  bflg_test_type = 1;
                }
                else
                {
                	  bflg_test_type = 0;
                }
            }
        }
        else if (guc_com_rx_buffer[1] == 0x10)    //�����д����  //peak �趨����ܡ�LED��ʾ  ����IC���ڴ˽���
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 6);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[6])        //peak ���CRCУ����ȷ
             && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
            {

               //peak add  ��
               //guc_IC_access_state = guc_com_rx_buffer[2];



            
                bflg_com_type_ok = 1;           //ͨѶ�����ж�ok      //peak ??
                
                bflg_com_tx_delaytime = 1;      //�÷�����ʱ��־      //peak ֮��ʱ�䵽���ٴ�������
                gss_com_tx_delaytimer = 10;     //������ʱ��ʱ����ֵ
                
                bflg_com_fault = 0;             //��ͨѶ���ϱ�־      //peak ?? δ��
                gss_com_fault_delaytimer = 0;   //��ͨѶ���ϼ�ʱ��
                //------------------------------------------
                //���ݴ���
                bflg_read_write_data = 0;       //��Ϊ������          //peak 
            }
        }
    }
    else  //�����Һ����ͨѶ
    {
        if ((guc_com_rx_buffer[1] == 0x03))  //����Ƕ�����
        {
            if (bflg_read_user_admin == 0)   //����Ƕ��û���Ϣ
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_USER_CNT * 2) + 3));
                if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_USER_CNT * 2) + 3])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_USER_CNT * 2) + 4]))
                {
                    bflg_com_type_ok = 1;    //ͨѶ�����ж�ok
                    
                    bflg_com_tx_delaytime = 1;         //�÷�����ʱ��־
                    gss_com_tx_delaytimer = LIQU_ROK_SDELAY;        //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;                //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;      //��ͨѶ���ϼ�ʱ��
                    //--------------------------------------
                    //���ݴ���
                    //bflg_read_write_data = 1;     //��Ϊд����
                    bflg_read_user_admin = 1;       //��Ϊ������Ա��Ϣ
                    //-----------------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                    gss_user_set_parm_value[DISP_USER_SET_TS] = gss_tmp.sword; //�õ��趨�¶� ok
                    //ram_para[num_comp2_TS] = gss_user_set_parm_value[DISP_USER_SET_TS];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                    gss_user_set_parm_value[DISP_USER_SET_ALH] = gss_tmp.sword;//�õ����±����¶� ok
                    //ram_para[num_comp2_ALH] = gss_user_set_parm_value[DISP_USER_SET_ALH];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                    gss_user_set_parm_value[DISP_USER_SET_ALL] = gss_tmp.sword;//�õ����±����¶� ok
                    //ram_para[num_comp2_ALL] = gss_user_set_parm_value[DISP_USER_SET_ALL];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[10];   //�õ�����ѡ��--δ��
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[12];   //�õ��¶ȵ�λѡ��--δ��
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                    gss_user_set_parm_value[DISP_USER_SET_Lt] = gss_tmp.sword;  //��������ʱ���� ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                    gss_user_set_parm_value[DISP_USER_SET_S1] = gss_tmp.sword;  //ɱ��ģʽ ok ��+
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                    gss_user_set_parm_value[DISP_USER_SET_S2] = gss_tmp.sword; //ɱ��ʱ������ ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                    gss_user_set_parm_value[DISP_USER_SET_dA] = gss_tmp.sword;  //���ű�����ʱʱ������ ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[21];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[22];
                    gss_liquid_led_button = gss_tmp.sword;              //���������ƿ��ư�ť ok ��+

                    if(gss_liquid_led_button == 1)
                    {
                        bflg_led_on = 1;
                    }
                    else
                    {
                        bflg_led_on = 0;
                    }
                    //led_control_deal();
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[23];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[24];
                    gss_liquid_ozone_disinfect_button = gss_tmp.sword;  //����ɱ��������ť ok ��+
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[25];   //�� ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[26];
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[27];   //�� ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[28];      
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[29];   //�� ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[30];  
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[31];   //ʱ ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[32];  
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[33];   //�� ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[34];     
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[35];   //�� ��+
                    gss_tmp.ubyte.high = guc_com_rx_buffer[36];                      
                }
            }
            else    //����Ƕ�����Ա��Ϣ
            {
                com_crc.uword = CRC16(guc_com_rx_buffer, ((READ_ADMIN_CNT * 2) + 3));
                if ((com_crc.ubyte.low == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 3])
                 && (com_crc.ubyte.high == guc_com_rx_buffer[(READ_ADMIN_CNT * 2) + 4]))
                {
                    bflg_com_type_ok = 1;    //ͨѶ�����ж�ok
                    
                    bflg_com_tx_delaytime = 1;         //�÷�����ʱ��־
                    gss_com_tx_delaytimer = LIQU_ROK_SDELAY;        //������ʱ��ʱ����ֵ
                    
                    bflg_com_fault = 0;                //��ͨѶ���ϱ�־
                    gss_com_fault_delaytimer = 0;      //��ͨѶ���ϼ�ʱ��
                    //--------------------------------------
                    //���ݴ���
                    bflg_read_write_data = 1;     //��Ϊд����
                    bflg_read_user_admin = 0;     //��Ϊ���û���Ϣ
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[3];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[4];
                    gss_factory_parm_value[DISP_FAC_r0] = gss_tmp.sword;     //�����²� ok
                    //ram_para[num_prot2_TA_first_delaytime] = gss_tmp.sword;   
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[5];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[6];
                    gss_factory_parm_value[DISP_FAC_Ct1] = gss_tmp.sword; 
                    //ram_para[num_prot2_door_delaytime] = gss_tmp.sword;     //���Դ�����У׼ֵ ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[7];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[8];
                    gss_factory_parm_value[DISP_FAC_Ct2] = gss_tmp.sword; 
                    //ram_para[num_comp2_VAC_ref] = gss_tmp.sword;      //��ʾ������У׼ֵ ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[9];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[10];
                    gss_factory_parm_value[DISP_FAC_Cd] = gss_tmp.sword; 
                    //ram_para[num_comp2_THW_ref] = gss_tmp.sword;      //����PTC������ʱʱ�� ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[11];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[12];
                    gss_factory_parm_value[DISP_FAC_T1] = gss_tmp.sword; 
                    //ram_para[num_prot2_THW_T] = gss_tmp.sword;        //����PTC����ʱ�� ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[13];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[14];
                    gss_factory_parm_value[DISP_FAC_T2] = gss_tmp.sword; 
                    //ram_para[num_comp2_prot_T] = gss_tmp.sword;       //����PTCͣ��ʱ�� ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[15];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[16];
                    gss_factory_parm_value[DISP_FAC_r1] = gss_tmp.sword; 
                    //ram_para[num_comp2_PT100_shield] = gss_tmp.sword; //��λ�л��²� ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[17];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[18];
                    gss_factory_parm_value[DISP_FAC_Ct3] = gss_tmp.sword; 
                    //ram_para[num_comp2_THW_shield] = gss_tmp.sword;   //�е�У׼ֵ ok
                    //--------------------------------------
                    gss_tmp.ubyte.low = guc_com_rx_buffer[19];
                    gss_tmp.ubyte.high = guc_com_rx_buffer[20];
                    gss_pwm_adjust  = gss_tmp.sword;                    //pwmռ�ձ�
                }
            }
        }
        else if (guc_com_rx_buffer[1] == 0x10)    //�����д����
        {
            com_crc.uword = CRC16(guc_com_rx_buffer, 6);
            if ((com_crc.ubyte.low == guc_com_rx_buffer[6])
             && (com_crc.ubyte.high == guc_com_rx_buffer[7]))
            {
                bflg_com_type_ok = 1;        //ͨѶ�����ж�ok
                
                bflg_com_tx_delaytime = 1;   //�÷�����ʱ��־
                gss_com_tx_delaytimer = LIQU_ROK_SDELAY;  //������ʱ��ʱ����ֵ
                
                bflg_com_fault = 0;               //��ͨѶ���ϱ�־
                gss_com_fault_delaytimer = 0;     //��ͨѶ���ϼ�ʱ��
                //------------------------------------------
                //���ݴ���
                bflg_read_write_data = 0;   //��Ϊ������
            }
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_rx_delaytime ��1����ʱgss_com_rx_delaytimer �趨��ʱ���
           ���Դ��ж�������գ�
*******************************************************************************/
void com_rx_delaytime(void)     //ͨѶ������ʱ������1ms��ʱ�����е���
{
    if (bflg_com_rx_delaytime == 1)
    {
        gss_com_rx_delaytimer--;
        if (gss_com_rx_delaytimer <= 0)
        {
            gss_com_rx_delaytimer = 0;
            bflg_com_rx_delaytime = 0;
            
            bflg_com_allow_rx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function:  bflg_com_tx_delaytime �˱�־��1����ʱgss_com_tx_delaytimer �趨��ʱ��
           �󣬴򿪷����жϣ������ͣ�
          
*******************************************************************************/
void com_tx_delaytime(void)     //ͨѶ������ʱ������1ms��ʱ�����е���
{
    if (bflg_com_tx_delaytime == 1)
    {
        gss_com_tx_delaytimer--;
        if (gss_com_tx_delaytimer <= 0)
        {
            gss_com_tx_delaytimer = 0;
            bflg_com_tx_delaytime = 0;
            
            bflg_com_allow_tx = 1;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
function: ��������ʱ���೤ʱ�����δ��ȷ���յ����ݣ�����û�н����꣬
          ��ֹͣ���ݵĽ��գ�Ҳ���Խ��յ����ݽ����κεĴ���
          
*******************************************************************************/
void com_rx_end_delaytime(void) //ͨѶ���������ʱ������1ms��ʱ�����е���
{
    if (bflg_com_rx_busy == 1)  //�����ǰ����æ
    {
        gss_com_rx_end_delaytimer++;
        if (gss_com_rx_end_delaytimer >= 50)
        {
            gss_com_rx_end_delaytimer = 0;
            bflg_com_rx_busy = 0;
            
            bflg_com_rx_end = 1;
            
            bflg_com_tx_delaytime = 1;
            gss_com_tx_delaytimer = 10;
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: û�жԷ�������ʱ������½�����Ӧ�Ĵ���
            

����λ��:1s��ʱ����-------------------------ok
******************************************************************************/
void com_fault_delaytime(void)     //ͨѶ������ʱ������1s��ʱ�����е���
{
    if (bflg_com_fault == 0)                //peak ����ж��˲�û��������
    {
        gss_com_fault_delaytimer++;
        if (gss_com_fault_delaytimer >= 30)  //30
        {
            gss_com_fault_delaytimer = 0;
            bflg_com_fault = 1;
        }
    }
    //----------------------------------------------
    //ͨѶ�������ж�
    if (bflg_com_type_ok == 0)               //ͨѶ��ȷ�� = 1������ȷ��Ϊ0��
    {
        guc_com_type_cnt++;                  //peak ��ʼͨѶʱ������5������
        if (guc_com_type_cnt >= 5)
        {                                   
            bflg_com_type = ~bflg_com_type;  //peak ��0 ???
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function :�ж��д���
          ����Ҫ���͵�����----> |1�����Ͷ���ʾ�����ݵ�ָ��  (8���ֽ�)
                                |2����������ʾ��д������    (23���ֽ�)
    
******************************************************************************/
void com_tx_int(void)    //ͨѶ�����жϳ���
{
    if (bflg_com_tx_busy == 1)
    {
        if (bflg_com_type == 0)    //�����LED��ʾ��ͨѶ
        {
            if (bflg_read_write_data == 0)   //����Ƕ�����
            {
                if (guc_com_tx_point < 8)
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                }
                else
                {
                    guc_com_tx_point = 0;    //�巢���ֽ�����
                    bflg_com_tx_busy = 0;    //�巢��æ��־
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                    gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 100;
                }
            }
            else    //�����д����
            {
                if (guc_com_tx_point < 53) //23
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                }
                else
                {
                    guc_com_tx_point = 0;    //�巢���ֽ�����
                    bflg_com_tx_busy = 0;    //�巢��æ��־
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //��ͨѶ������ʱ��־
                    gss_com_rx_delaytimer = 5;    //ͨѶ������ʱ��ʱ����ֵ
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 100;
                }
            }
        }
        else   //�����Һ����ͨѶ
        {
            if (bflg_read_write_data == 0)   //����Ƕ�����
            {
                if (guc_com_tx_point < 8)
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                }
                else
                {
                    guc_com_tx_point = 0;    //�巢���ֽ�����
                    bflg_com_tx_busy = 0;    //�巢��æ��־
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;    //��comͨѶ������ʱ��־
                    gss_com_rx_delaytimer = 5;    //comͨѶ������ʱ��ʱ����ֵ
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 300; //1000
                }
            }
            else    //�����д����
            {
                if (guc_com_tx_point < ((WRITE_STATE_CNT * 2) + 9))   //109
                {
                    TXD0 = guc_com_tx_buffer[guc_com_tx_point++];     //��������
                }
                else
                {
                    guc_com_tx_point = 0;     //�巢���ֽ�����
                    bflg_com_tx_busy = 0;     //�巢��æ��־
                    bflg_com_tx_ok = 1;
                    
                    bflg_com_rx_delaytime = 1;     //��ͨѶ������ʱ��־
                    gss_com_rx_delaytimer = 5;     //ͨѶ������ʱ��ʱ����ֵ
                    
                    bflg_com_tx_delaytime = 1;
                    gss_com_tx_delaytimer = 300; //1000
                }
            }
        }
    }
}
//------------------------------------------------------------------------------
/******************************************************************************
function: �жϽ��յ�����

          *****LED****
          ����յ����Ƿ������ѯ����󷵻ص�ֵ:
          ���յ�9���ֽڡ��������п���������չ�ģ����Ǵ˳����Ͳ�ѯ�趨Ϊ�̶���     
      ��ѯ2���Ĵ��������Է���ʱҲ�ǹ̶���4���ֽ�(һ���Ĵ�������2���ֽ�);
      
       ----------------------------------------   
          ����յ����Ƿ������趨����󷵻ص�ֵ:
          ���յ�8���ֽڣ��������о���8����

*******************************************************************************/
void com_rx_int(uint8_t rx_data)   //ͨѶ�����жϳ���
{
    uint8_t luc_com_rx_byte;  //�����ֽ���ʱ������
    
    gss_com_rx_end_delaytimer = 0; //���������ʱ��ʱ��
    //--------------------------------------------------
    luc_com_rx_byte = rx_data;
    
    if (bflg_com_type == 0)   //�����LED��ʾ��ͨѶ
    {
        if ((bflg_com_rx_busy == 0) && (luc_com_rx_byte == 0x00))     //����޽���æ���Ҵӵ�ַ��ȷ
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //�������æ
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if ((guc_com_rx_buffer[1] == 0x03) && (guc_com_rx_point >= 9))       //����ǲ�ѯ����ҽ����������
            {
                guc_com_rx_point = 0;   //������ֽ�����
                bflg_com_rx_busy = 0;   //�����æ��־
                bflg_com_rx_ok = 1;     //�ý��ճɹ���־
            }
            else if ((guc_com_rx_buffer[1] == 0x10) && (guc_com_rx_point >= 8))  //����Ƕ���趨����ҽ��յ��ֽ�������֡
            {
                guc_com_rx_point = 0;    //������ֽ�����
                bflg_com_rx_busy = 0;    //�����æ��־
                bflg_com_rx_ok = 1;      //�ý��ճɹ���־
            }
        }
    }
    else  //�����Һ����ͨѶ
    {
        if ((bflg_com_rx_busy == 0) && (luc_com_rx_byte == 0x01))     //����޽���æ���Ҵӵ�ַ��ȷ
        {
            bflg_com_rx_busy = 1;
            
            guc_com_rx_buffer[0] = luc_com_rx_byte;
            guc_com_rx_point = 1;
        }
        else if (bflg_com_rx_busy == 1)      //�������æ
        {
            guc_com_rx_buffer[guc_com_rx_point++] = luc_com_rx_byte;
            
            if (guc_com_rx_buffer[1] == 0x10)     //����Ƕ���趨����
            {
                if (guc_com_rx_point >= 8)   //���յ��ֽ�������֡
                {
                    guc_com_rx_point = 0;    //������ֽ�����
                    bflg_com_rx_busy = 0;    //�����æ��־
                    bflg_com_rx_ok = 1;      //�ý��ճɹ���־
                }
            }
            else if (guc_com_rx_buffer[1] == 0x03)     //����Ƕ���Ϣ
            {
                if (bflg_read_user_admin == 0)    //����Ƕ��û���Ϣ
                {
                    if (guc_com_rx_point >= ((READ_USER_CNT * 2) + 5))      //65
                    {
                        guc_com_rx_point = 0;     //������ֽ�����
                        bflg_com_rx_busy = 0;     //�����æ��־
                        bflg_com_rx_ok = 1;       //�ý��ճɹ���־
                    }
                }
                else     //����Ƕ�����Ա��Ϣ
                {
                    if (guc_com_rx_point >= ((READ_ADMIN_CNT * 2) + 5))     //65
                    {
                        guc_com_rx_point = 0;     //������ֽ�����
                        bflg_com_rx_busy = 0;     //�����æ��־
                        bflg_com_rx_ok = 1;       //�ý��ճɹ���־
                    }
                }
            }
            else     //�յ���Ԫ��[1]��Լ����������
            {
                
            }
        }
    }
}
//------------------------------------------------------------------------------
unsigned int CRC16(unsigned char *puchmsg, unsigned int usDataLen)    //����CRCУ��ĳ���
{
    unsigned int uchCRC;
    unsigned int uIndex_x, uIndex_y;
    
    uchCRC = 0xFFFF;
    
    if (usDataLen > 0)
    {
        for (uIndex_x = 0; uIndex_x <= (usDataLen - 1); uIndex_x++)
        {
            uchCRC = uchCRC ^ (unsigned int) (*puchmsg);
            puchmsg++;
            
            for (uIndex_y = 0; uIndex_y <= 7; uIndex_y++)
            {
                if ((uchCRC & 0x0001) != 0)
                {
                    uchCRC = (uchCRC >> 1) ^ 0xA001;
                }
                else
                {
                    uchCRC = uchCRC >> 1;
                }
            }
        }
    }
    else
    {
        uchCRC = 0;
    }
    
    return uchCRC;
}

//---------------------------------------------------------------------
//peak add




//------------------------------------------------------------------------------
#endif
