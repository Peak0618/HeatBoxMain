#ifndef _M_KEY_OPERATION_H_
#define _M_KEY_OPERATION_H_

//�ⲿ����
extern void mode_selection(void);
extern void key_locked_delaytime(void);   
extern void key_set_long_delaytime(void);
extern void key_set_up_long_delaytime(void);
extern void key_up_down_long_delaytime(void);
extern void key_set_down_long_delaytime(void);
extern void key_parm_default_val(void);



//����ֵ
#define   KEY_NONE              0x00     //�ް�������
#define   KEY_SET               0x08     //�趨
#define   KEY_UP                0x10     //�ϵ�
#define   KEY_DOWN              0x01     //�µ�
#define   KEY_BUZ               0x02     //��������
#define   KEY_LAMP              0x04     //��
#define   KEY_SET_AND_UP        0x18     //�趨+�ϵ�  ����
#define   KEY_UP_AND_DOWN       0x11     //�ϵ�+�µ�  ��ѯ
#define   KEY_SET_AND_DOWN      0x09     // IC����



//����_��
extern uint8_t   guc_disp_mode;
#define   DISP_LOCKED                  0   //����ģʽ
#define   DISP_USER_SET                1   //�û��趨��
#define   DISP_USB_TIME_SET            2   //usbʱ������
#define   DISP_USER_MANAGE_PASSWORD    3   //�û����������
#define   DISP_USER_MANAGE             4   //�û������
#define   DISP_FACTORY_PASSWORD        5   //��������ģʽ
#define   DISP_FACTORY                 6   //����ģʽ
#define   DISP_INQUIRE                 7   //��ѯģʽ
#define   DISP_IC_PASSWORD             8   //IC������ģʽ
#define   DISP_IC_REGISTER             9   //IC��ע��ģʽ

extern int8_t   gsc_disp_parm_type;
//��֧_�û��趨�����
#define   DISP_USER_SET_TS                  0   //�����¶��趨ֵ            magnify 10 ʹ��ʱ����Ŵ�10
#define   DISP_USER_SET_ALH                 1   //���±���ֵ              magnify 10
#define   DISP_USER_SET_ALL                 2   //���±���ֵ              magnify 10
#define   DISP_USER_SET_Lt                  3   //���ڵ���ʱ�ر�ʱ�� or ȡ����ʱ
#define   DISP_USER_SET_dA                  4   //���ŷ�����������ʱ
#define   DISP_USER_SET_S1                  5   //ɱ��ģʽ
#define   DISP_USER_SET_S2                  6   //���ɱ��ʱ��
#define   DISP_USER_SET_TT                  7   //�¶ȵ�λ
#define   DISP_USER_SET_SUM                 8   //�û��趨������ܸ���

//��֧_�û���������
#define   DISP_USER_MANAGE_IC               0  //ic��ע��/�û�����������޸�
#define   DISP_USER_MANAGE_CL1              1  //ic��ע��
#define   DISP_USER_MANAGE_Lt               2  //������������ʱ�ر�ʱ��orȡ���ر���ʱ
#define   DISP_USER_MANAGE_dA               3  //���ŷ�����������ʱ
#define   DISP_USER_MANAGE_FA               4  //����������
#define   DISP_USER_MANAGE_S1               5  //ɱ��ģʽ
#define   DISP_USER_MANAGE_S2               6  //���ɱ��ʱ��
#define   DISP_USER_MANAGE_TT               7  //�¶ȵ�λ
#define   DISP_USER_MANAGE_SUM              8  //������������

//��֧_��������
#define  DISP_FAC_r0                        0   //�����²�                magnify 10
#define  DISP_FAC_r1                        1   //��λ�л��²�
#define  DISP_FAC_Ct1                       2   //���Դ�����У׼ֵ            magnify 10
#define  DISP_FAC_Ct2                       3   //��ʾ������У׼ֵ            magnify 10
#define  DISP_FAC_Cd                        4   //��ʼ�ϵ�PTC������ʱ
#define  DISP_FAC_T1                        5   //����ʱPCT����ʱ��
#define  DISP_FAC_T2                        6   //����ʱPCTͣ��ʱ��
#define  DISP_FAC_Fd                        7   //������ȡ�������������ʱ��-ʹ����*10
#define  DISP_FAC_Ct3                       8   //�е��ѹУ׼ֵ----����ڼӣ�LED����û���ã�ֻ��Һ������
#define  DISP_FAC_SUM                       9   //�������������


//��֧_��ѯ����
extern int8_t  gsc_disp_inq_mode;
#define  DISP_INQ_d1                        0   //�ϴ�����
#define  DISP_INQ_d2                        1   //�´�����
#define  DISP_INQ_d3                        2   //���ƴ�����
#define  DISP_INQ_d4                        3   //��˪������
#define  DISP_INQ_d5                        4   //����������
#define  DISP_INQ_d6                        5   //���´�����
#define  DISP_INQ_d7                        6   //����ʪ��
#define  DISP_INQ_d8                        7   //�е��ѹ
#define  DISP_INQ_d9                        8   //��ص���



//�ⲿ��־
extern flag_type bflg_key_operation;
       #define   bflg_key_set_long_valid          bflg_key_operation.bits.bit0   //����set����Ч
       #define   bflg_key_set_long_delaytime      bflg_key_operation.bits.bit1   //����set����ʱ
       #define   bflg_key_set_up_long_valid       bflg_key_operation.bits.bit2   //����set+up����Ч
       #define   bflg_key_set_up_long_delaytime   bflg_key_operation.bits.bit3   //����set+up����ʱ
       #define   bflg_key_up_down_long_valid      bflg_key_operation.bits.bit4   //����up+down����Ч
       #define   bflg_key_up_down_long_delaytime  bflg_key_operation.bits.bit5   //����up+down����ʱ
       #define   bflg_key_set_down_long_valid     bflg_key_operation.bits.bit6   //����set+down����Ч
       #define   bflg_key_set_down_long_delaytime bflg_key_operation.bits.bit7   //����set+down����ʱ
       #define   bflg_parm_dis_form                bflg_key_operation.bits.bit8  //��ʾ������ʶ or ����ֵ
       
       #define   bflg_key_inq_val                 bflg_key_operation.bits.bit10  //��ѯ��������ֵ
       
extern flag_type bflg_key_operation_save;
       #define   bflg_parm_save              bflg_key_operation_save.bits.bit0   //�Ƿ�Բ�������



//��������
extern uint8_t   guc_key_val;                      //������ֵ
extern uint8_t   guc_TUTD_dis_type;                //ѡ����ʾ���� �����»�������ƽ��ֵ
extern uint8_t   guc_key_set_long_delaytimer;      //����set����ʱ
extern uint8_t   guc_key_set_up_long_delaytimer;   //����set+up����ʱ
extern uint8_t   guc_key_up_down_long_delaytimer;  //����up+down����ʱ
extern uint8_t   guc_key_set_down_long_delaytimer; //����set+down����ʱ
extern uint8_t   guc_key_locked_delaytimer;        //�����޲���������ʱ
extern uint8_t   guc_disp_mode;                    //�ܵ�UIģʽ

extern int8_t    gsc_user_manage_password_val;     //�û���������ֵ
extern int8_t    gsc_factory_password_val;         //��������ֵ
extern int8_t    gsc_ic_password_val;              //ic������ֵ
extern int8_t    gsc_disp_parm_type;               //user�趨��Ĳ�ͬ����
extern int8_t    gsc_disp_inq_mode;                //��ѯģʽ�в�ͬ����

extern int16_t   gss_key_temp_value;               //����ʱ����ʱֵ

extern int16_t   gss_user_set_parm_value[DISP_USER_SET_SUM];       
extern int16_t   gss_user_manage_parm_value[DISP_USER_MANAGE_SUM]; 
extern int16_t   gss_factory_parm_value[DISP_FAC_SUM]; 













#endif
/*********************************************END OF THE FILE********************************************************/

