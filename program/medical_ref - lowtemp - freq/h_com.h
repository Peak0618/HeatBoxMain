#ifndef _COM_H_
#define _COM_H_
//------------------------------------------------------------------------------
//�ⲿ��������
extern void com_init(void);   //ͨѶ��ʼ�������ڳ�ʼ�������е���

extern void com_rx_init(void);     //ͨѶ���ճ�ʼ����������ѭ�������е���

extern void com_rx_data_deal(void);     //ͨѶ�������ݴ����������ѭ�������е���

extern void com_tx_init(void);     //ͨѶ�������ݴ����������ѭ�������е���

extern void com_rx_delaytime(void);     //ͨѶ������ʱ������1ms��ʱ�����е���

extern void com_tx_delaytime(void);     //ͨѶ������ʱ������1ms��ʱ�����е���

extern void com_rx_end_delaytime(void); //ͨѶ���ս�����ʱ������1ms��ʱ�����е���

extern void com_fault_delaytime(void);  //ͨѶ������ʱ������1s��ʱ�����е���

extern void com_tx_int(void); //ͨѶ�����жϳ���

extern void com_rx_int(uint8_t rx_data);//ͨѶ�����жϳ���



//------------------------------------------------------------------------------
//�ⲿ��������//
extern flag_type flg_com;

          #define   bflg_com_tx_delaytime    flg_com.bits.bit0   //ͨѶ������ʱ��־
          #define   bflg_com_rx_delaytime    flg_com.bits.bit1   //ͨѶ������ʱ��־
          #define   bflg_com_allow_tx        flg_com.bits.bit2   //ͨѶ�����ͱ�־
          #define   bflg_com_allow_rx        flg_com.bits.bit3   //ͨѶ������ձ�־
          #define   bflg_com_tx_busy         flg_com.bits.bit4   //ͨѶ����æ��־
          #define   bflg_com_rx_busy         flg_com.bits.bit5   //ͨѶ����æ��־
          #define   bflg_com_tx_ok           flg_com.bits.bit6   //ͨѶ���ͳɹ���־
          #define   bflg_com_rx_ok           flg_com.bits.bit7   //ͨѶ���ճɹ���־
          #define   bflg_com_rx_end          flg_com.bits.bit8   //ͨѶ���ս�����־
          
          #define   bflg_com_fault           flg_com.bits.bit9   //ͨѶ���ϱ�־
          
          #define   bflg_com_type            flg_com.bits.bit10  //ͨѶ���ͱ�־     //peak  0:LED  1:Һ��
          #define   bflg_com_type_ok         flg_com.bits.bit11  //ͨѶ�����ж�ok��־

          //peak add
          #define   bflg_lock_delaytimer     flg_com.bits.bit12  // ��������ʱ��־
//------------------------------------------------------------------------------
extern flag_type flg_read_write;
          #define   bflg_read_write_data     flg_read_write.bits.bit0      //��д���ݱ�־
          #define   bflg_read_user_admin     flg_read_write.bits.bit1      //���û�����Ա��־

//------------------------------------------------------------------------------
//�ⲿ���� 
extern uint8_t  guc_IC_access_state;               // ������״̬
extern uint16_t guc_flash_add;                     // ���յ�IC��ע�����õĵ�ַ
extern uint16_t guc_temp_flash_add;                // ���յ�IC��ע����ʱ����ĵ�ַ
extern uint8_t  guc_IC_erasure_tx_ok_count;        // ע��IC��ָ���bflg_com_tx_ok�����÷��Ͷ�μ���,��ֹ����
extern int16_t  gss_liquid_led_button;              //Һ�����Ŀ��ư�ť  ��1�� 0��
extern int16_t  gss_liquid_ozone_disinfect_button;  //Һ����������ɱ����ť��1�� 0��          
//------------------------------------------------------------------------------
#endif
