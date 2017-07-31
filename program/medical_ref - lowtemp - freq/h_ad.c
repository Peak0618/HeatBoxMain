#ifndef _AD_C_
#define _AD_C_
//------------------------------------------------------------------------------
//包含文件声明
#include "h_type_define.h"
#include "h_ad.h"
#include "h_ad_tab.h"
#include "h_rom_para2.h"
#include "h_key_disp.h"
#include "h_protect.h"
#include "m_key_operation.h"
//------------------------------------------------------------------------------
//函数声明
void ad_val_deal(void);                //ad值处理程序，在AD转换程序中调用

void ad_temp_calc(void);              //ad温度计算程序，在主循环程序中调用

int16_t get_temp1(uint16_t lus_ad);     //根据ad值查表得到温度值1

int16_t get_temp2(uint16_t lus_ad);     //根据ad值查表得到温度值2

int16_t get_temp3(uint16_t lus_ad);     //根据ad值查表得到温度值3

int16_t get_temp4(uint16_t lus_ad);     //根据ad值查表得到温度值4

int16_t get_temp5(uint16_t lus_ad);     //根据ad值查表得到温度值5

int16_t get_temp6(uint16_t lus_ad);     //根据ad值查表得到温度值6


//------------------------------------------------------------------------------
//变量定义
flag_type flg_adc;
//----------------------------------------------------------
#define   AD_SUM_INDEX   6    //求平均移位值
#define   AD_SUM_CNT     64   //求平均个数

#define   AD_VAL_MIN     5    //AD下限值
#define   AD_VAL_MAX     1000 //AD上限值

//----------------------------------------------------------
uint16_t  gus_ad_val;             //从 ADCR转换结果寄存器中获取的值
uint16_t  gus_ad_buffer[12];      //只在求瞬时电压时用过
uint32_t  gul_ad_sum_buffer[12];  //12个元素，没个元素(通道)都是64次累加
uint16_t  gus_ad_aver_buffer[12];
uint8_t   guc_ad_index;
uint8_t   guc_ad_cnt;
//----------------------------------------------------------
uint8_t   guc_ad_fault_cnt[12];         //ad故障次数计数器
uint8_t   guc_ad_fault_release_cnt[12]; //ad故障释放次数计数器

//----------------------------------------------------------
int16_t   gss_TA;   //主传感器           放大10倍，其他不变
int16_t   gss_THW;  //环温传感器  ----->血液的热交换温度
int16_t   gss_TC;   //冷凝被加温箱的替代
int16_t   gss_TE;   //热交换被加温箱的替代

//血液柜使用
int16_t   gss_TU;   //上温传感器
int16_t   gss_TD;   //下温传感器

//加温箱使用
int16_t gss_TDIS;   //显示传感器 放大10
int16_t gss_TTEST;  //测试传感器 放大10


int16_t   gss_VAC;  //电源电压
uint8_t   guc_12V;  //12V电压

int16_t   gss_Tbat1;     //锂电池温度1
int16_t   gss_Tbat2;     //锂电池温度2

uint8_t   guc_bat_Vin;   //电池充电电压
uint8_t   guc_bat_Vout;  //电池放电电压
uint8_t   guc_bat_Vout_instant;  //电池放电电压瞬时值


uint32_t  gul_bat_Vin;   //peak计算充电电流时使用
uint32_t  gul_bat_Vout;  //peak计算充电电流时使用
int16_t   gss_bat_I;     //电池充电电流

int16_t   gss_adg_Temp[10];
int16_t   gss_adg_Temp2;

uint8_t   guc_ad_calc_cnt;

//ad_convert_deal

//------------------------------------------------------------------------------
//程序内容
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能:  

函数位置: --------------ad_convert_deal() 中,这个函数又在1ms定时中----ok 
********************************************************************************/
void ad_val_deal(void)   //ad值处理程序，在AD转换程序中调用  peak 1ms中 
{
    //------------------------------------------------------
    //ad值累计
    gus_ad_buffer[guc_ad_index] = gus_ad_val;
    gul_ad_sum_buffer[guc_ad_index] += gus_ad_val;
    //------------------------------------------------------
    
    //ad值求平均
    if (guc_ad_cnt == 0)
    {
        gus_ad_aver_buffer[guc_ad_index] = (uint16_t) (gul_ad_sum_buffer[guc_ad_index] >> AD_SUM_INDEX);
        gul_ad_sum_buffer[guc_ad_index] = 0;
    }
    
    //------------------------------------------------------
    //ad故障判断与故障解除
    if ((gus_ad_val < AD_VAL_MIN) || (gus_ad_val > AD_VAL_MAX))
    {
        guc_ad_fault_release_cnt[guc_ad_index] = 0;
        //--------------------------------------------------
        guc_ad_fault_cnt[guc_ad_index]++;
        if (guc_ad_fault_cnt[guc_ad_index] >= 100)
        {
            guc_ad_fault_cnt[guc_ad_index] = 0;
            
            switch (guc_ad_index)
            {
                case 0:
                    if ((bflg_TTEST_fault == 0) && (ram_para[num_comp2_THW_shield] == 0))
                    {
                        bflg_TTEST_fault = 1;    //测试传感器故障
                        gus_trip_code1 |= TTSET_S_CODE;
                    }
                    break;
                case 1:    
                    
                    break;
                case 2:    //peak 冷凝 --现改为上温 --待
                    if ((bflg_TC_fault == 0) && (ram_para[num_comp2_TC_shield] == 0))
                    {
                        bflg_TC_fault = 1;
                        //gus_trip_code1 |= 0x0080;
                    }
                    break;
                case 3:
                    if ((bflg_TDIS_fault == 0) && (ram_para[num_comp2_TE_shield] == 0))
                    {
                        bflg_TDIS_fault = 1;       //显示传感器故障
                        gus_trip_code1 |= TDIS_S_CODE;
                    }
                    break;
                    
                
                    break;
                case 4:
                    
                    break;
                case 5:
                    
                    break;
                case 6:
                    
                    break;
                case 7:
                    
                    break;
                case 8:
                    if ((bflg_TA_fault == 0) && (ram_para[num_comp2_PT100_shield] == 0))
                    {
                        bflg_TA_fault = 1;
                        //gus_trip_code1 |= 0x0020;
                    }
                    break;
                case 9:
                    if (bflg_Tbat2_fault == 0)
                    {
                    	  bflg_Tbat2_fault = 1;
                    }
                    break;
                case 10:
                    if (bflg_Tbat1_fault == 0)
                    {
                    	  bflg_Tbat1_fault = 1;
                    }
                    break;
                case 11:
                    
                    break;
            }
        }
    }
    else
    {
        guc_ad_fault_cnt[guc_ad_index] = 0;
        //--------------------------------------------------
        guc_ad_fault_release_cnt[guc_ad_index]++;
        if (guc_ad_fault_release_cnt[guc_ad_index] >= 100)
        {
            guc_ad_fault_release_cnt[guc_ad_index] = 0;
            
            switch (guc_ad_index)
            {
                case 0:
                    bflg_TTEST_fault = 0;     // 测试传感器
                    gus_trip_code1 &= ~TTSET_S_CODE;
                    break;
                case 1:
                    
                    break;
                case 2:
                    bflg_TC_fault = 0;
                    //gus_trip_code1 &= ~0x0080;
                    break;
                case 3:
                    bflg_TDIS_fault = 0;      //显示传感器
                    gus_trip_code1 &= ~TDIS_S_CODE;
                    break;
                case 4:
                    
                    break;
                case 5:
                    
                    break;
                case 6:
                    
                    break;
                case 7:
                    
                    break;
                case 8:
                    bflg_TA_fault = 0;
                    //gus_trip_code1 &= ~0x0020;
                    break;
                case 9:
                    bflg_Tbat2_fault = 0;
                    break;
                case 10:
                    bflg_Tbat1_fault = 0;
                    break;
                case 11:
                    
                    break;
            }
        }
    }
    //------------------------------------------------------
    //索引累加
    guc_ad_index++;
    if (guc_ad_index >= 12)              //peak 12个通道都采集完一遍
    {
        guc_ad_index = 0;
        //------------------------------
        if (guc_ad_cnt == 0)
        {
            bflg_allow_ad_calc = 1;     //置允许ad计算标志, 即置1时 ad_temp_calc() 才能执行；又清0；
        }
        //------------------------------
        guc_ad_cnt++;
        if (guc_ad_cnt >= AD_SUM_CNT)   //12路AD都采集完64次后才允许计算；
        {
            guc_ad_cnt = 0;
        }
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 对不同通道的AD值，进行计算得到对应的温度值、电压值、电流值等；

运行条件: bflg_allow_ad_calc == 1 时；

函数位置: 主循环中     ---------ok
********************************************************************************/
void ad_temp_calc(void)  //ad温度计算程序，在主循环程序中调用
{
    int32_t lsl_tmp;
    int16_t lss_TA = gss_TA,lss_TD = gss_TD,lss_TU = gss_TU,lss_TDIS = gss_TDIS,lss_TTEST = gss_TTEST;
    //----------------------------------
    //gss_THW = get_temp1(gus_ad_aver_buffer[0]) + gss_factory_CA;     //添加环温校准
             
    gss_TD = get_temp5(gus_ad_aver_buffer[1]); 
    gss_TD = (((lss_TD*3) + gss_TD)>> 2);        
    gss_TU = get_temp5(gus_ad_aver_buffer[2]); 
    gss_TU = (((lss_TU*3) + gss_TU)>> 2);        
    
    gss_TDIS = get_temp6(gus_ad_aver_buffer[3] + gss_factory_parm_value[DISP_FAC_Ct2]);   //显示传感器温度 加温箱    
    gss_TDIS = (((lss_TDIS*3) + gss_TDIS)>> 2);
    gss_TTEST = get_temp6(gus_ad_aver_buffer[0] + gss_factory_parm_value[DISP_FAC_Ct1]);  //测试传感器温度 加温箱 
    gss_TTEST = (((lss_TTEST*3) + gss_TTEST)>> 2);

    //gss_TA = get_temp3(gus_ad_aver_buffer[8]) + ram_para[num_comp2_PT100_ref];
    gss_TA = get_temp3(gus_ad_aver_buffer[8]) ;              
    gss_TA = (((lss_TA*3) + gss_TA)>> 2);        
    
    gss_Tbat2 = get_temp4(gus_ad_aver_buffer[9]);
    gss_Tbat1 = get_temp4(gus_ad_aver_buffer[10]);
    
    //------------------------------------------------------------------------------------------------------
    //电源电压计算（单位：1V） peak--220v ---和时间分压计算相差36倍，因为变压器的匝数比，现未知；
    lsl_tmp = gus_ad_aver_buffer[4];
    //lsl_tmp *= 185;
    lsl_tmp *= 180;
    lsl_tmp >>= 9;
    
    //gss_VAC = ((int16_t) (lsl_tmp)) + ram_para[num_comp2_VAC_ref];    //原来的
    gss_VAC = ((int16_t) (lsl_tmp)) + gss_factory_parm_value[DISP_FAC_Ct3];   //添加电压校准
    
    //------------------------------------------------------------------------------------------------------
    //12V计算（单位：0.1V） 
    lsl_tmp = gus_ad_aver_buffer[5];      //根据电阻分压，5v对应1024计算(10位ad)
    //lsl_tmp *= 150;
    lsl_tmp *= 150;                       //peak 放大10倍，所以和其比较的数也要放大10倍
    lsl_tmp >>= 10;
    
    guc_12V = (uint8_t) (lsl_tmp);
    
    //------------------------------------------------------------------------------------------------------
    gss_disp_see_buffer[0] = gss_VAC;  //面板显示see参数时使用
    gss_disp_see_buffer[1] = gss_THW;
    
    //------------------------------------------------------------------------------------------------------
    //电池充电电压计算（单位：0.1V）
    lsl_tmp = gus_ad_aver_buffer[11];
    //lsl_tmp *= 224;
    lsl_tmp *= 224;                   //放大10倍
    gul_bat_Vin = lsl_tmp;            //gul_bat_Vin只在计算电流时用,没有>>10，所以是放大了1024倍
    lsl_tmp >>= 10;
    
    guc_bat_Vin = (uint8_t) (((guc_bat_Vin * 3) + ((uint8_t) (lsl_tmp))) >> 2); // peak 根木滤波: 上次数值乘以3加现在的值，最后除以4； 
    
    //------------------------------------------------------------------------------------------------------
    //电池放电电压计算（单位：0.1V）
    lsl_tmp = gus_ad_aver_buffer[7];
    //lsl_tmp *= 224;
    lsl_tmp *= 224;                   //放大10倍
    gul_bat_Vout = lsl_tmp;           //gul_bat_Vout只在计算电流时用,没有>>10，所以是放大了1024倍
    lsl_tmp >>= 10;
    
    guc_bat_Vout = (uint8_t) (((guc_bat_Vout * 3) + ((uint8_t) (lsl_tmp))) >> 2);//根木滤波
    
    //------------------------------------------------------------------------------------------------------
    lsl_tmp = gus_ad_buffer[7];                  //使用瞬时为了判断速度快
    //lsl_tmp *= 224;
    lsl_tmp *= 224;                              //放大10倍
    //gul_bat_Vout = lsl_tmp;                    //peak删除，用平均的                    
    lsl_tmp >>= 10;
    guc_bat_Vout_instant = (uint8_t) (lsl_tmp);  //peak 判断电池是否连接  
    
    //------------------------------------------------------------------------------------------------------
    //电池充电电流计算（单位：1mA）
    lsl_tmp = gul_bat_Vin;
    lsl_tmp -= gul_bat_Vout;
    lsl_tmp *= 10000;                  //1.82-->182 放大100； 上面的0.1v-->1mA放大100，所以此处为10000
    //lsl_tmp /= 182;
    lsl_tmp /= 180;
    lsl_tmp >>= 10;                    //gul_bat_Vin在上面没有除以1024，为了放大易计算，所以在此处除；
    
    gss_bat_I = ((gss_bat_I * 3) + ((int16_t) (lsl_tmp))) >> 2;   //根木滤波
    
    //------------------------------------------------------------------------------------------------------
    guc_ad_calc_cnt++;              //peak 刚开始为了稳定多采集几次再判断
    if (guc_ad_calc_cnt >= 5)
    {
    	  bflg_ad_calc_ok = 1;
    }
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 根据获取的AD值，和表格中的AD值比较，获取改AD值在表格中的索引值，根据
          索引值可以到表格中查找到对应索引值的温度值；

函数位置: ad_temp_calc() 中；

获取内容: 获取环温、冷凝的温度值(这两个公用同一个传感器)
********************************************************************************/
int16_t get_temp1(uint16_t lus_ad)      //根据ad值查表得到温度值1
{
    int16_t lss_temp_index;
    lss_temp_index = (TEMP1_TAB_LENGTH >> 1);  //peak 2分法获取中间值，缩短数据的查找速度
    
    if (lus_ad > ad_to_temp1[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp1[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad > ad_to_temp1[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp1[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP1_TAB_LENGTH)
            {
                lss_temp_index = TEMP1_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp1[lss_temp_index])
            {
                lss_temp_index--;
                break;
            }
        }
        while (lus_ad < ad_to_temp1[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    return temp1_for_ad[lss_temp_index];
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 

函数位置: ad_temp_calc() 中；

获取内容: 获取热交换器的温度值 
********************************************************************************/
int16_t get_temp2(uint16_t lus_ad)      //根据ad值查表得到温度值2
{
    int16_t lss_temp_index;
    lss_temp_index = (TEMP2_TAB_LENGTH >> 1);
    
    if (lus_ad > ad_to_temp2[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp2[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad > ad_to_temp2[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp2[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP2_TAB_LENGTH)
            {
                lss_temp_index = TEMP2_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp2[lss_temp_index])
            {
                lss_temp_index--;
                break;
            }
        }
        while (lus_ad < ad_to_temp2[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    return temp2_for_ad[lss_temp_index];
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 

函数位置: ad_temp_calc() 中；

获取内容: 获取PT100的温度值 
********************************************************************************/
int16_t get_temp3(uint16_t lus_ad)      //根据ad值查表得到温度值3
{
    int16_t lss_delt;
    
    int16_t lss_temp_index;
    lss_temp_index = (TEMP3_TAB_LENGTH >> 1);
    
    if (lus_ad > ad_to_temp3[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp3[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad > ad_to_temp3[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp3[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP3_TAB_LENGTH)
            {
                lss_temp_index = TEMP3_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp3[lss_temp_index])
            {
                lss_temp_index--;                            //peak 为了可以共同使用下面的公式
                break;
            }
        }
        while (lus_ad < ad_to_temp3[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    lss_delt = lus_ad;
    lss_delt -= ad_to_temp3[lss_temp_index];
    lss_delt *= 10;      //因为pt100在下面放大了10倍，所以此处求小数部分的值也要放大10倍
    lss_delt >>= 2;      //除以4，因为PT100建表中大多间隔都是4个刻度值，也可参考temp5的处理方法
    
    //return temp3_for_ad[lss_temp_index];
    return (temp3_for_ad[lss_temp_index] * 10 - lss_delt);
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 

函数位置: ad_temp_calc() 中；

获取内容: 获取电池的温度值 
********************************************************************************/
int16_t get_temp4(uint16_t lus_ad)      //根据ad值查表得到温度值4
{
    int16_t lss_temp_index;
    lss_temp_index = (TEMP4_TAB_LENGTH >> 1);
    
    if (lus_ad > ad_to_temp4[lss_temp_index])
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp4[lss_temp_index])
            {
                //lss_temp_index++;
                break;
            }
        }
        while (lus_ad > ad_to_temp4[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp4[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP4_TAB_LENGTH)
            {
                lss_temp_index = TEMP4_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp4[lss_temp_index])
            {
                lss_temp_index--;
                break;
            }
        }
        while (lus_ad < ad_to_temp4[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    return temp4_for_ad[lss_temp_index];
}
//------------------------------------------------------------------------------
/*******************************************************************************
函数功能: 上下温

函数位置: ad_temp_calc() 中；

获取内容: 获取上下温的温度值 
********************************************************************************/
int16_t get_temp5(uint16_t lus_ad)        //根据ad值查表得到温度值5
{
    int16_t lss_delt;
     
    int16_t lss_temp_index;
    lss_temp_index = (TEMP5_TAB_LENGTH >> 1);  //peak 2分法获取中间值，缩短数据的查找速度
    
    if (lus_ad > ad_to_temp5[lss_temp_index])  //AD值越大，温度越小
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp5[lss_temp_index])
            {
                //lss_temp_index++;  原注
                break;
            }
        }
        while (lus_ad > ad_to_temp5[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp5[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP5_TAB_LENGTH)
            {
                lss_temp_index = TEMP5_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp5[lss_temp_index])
            {
                lss_temp_index--;  //原未注                 //peak 为了可以和上面那一部分共同使用下面的公式
                break;
            }
        }
        while (lus_ad < ad_to_temp5[lss_temp_index]);
    }
    //查表温度高1℃，在次减一个指针，温度降1℃
    /*lss_temp_index--;
    if (lss_temp_index < 0)
    {
        lss_temp_index = 0;
    }*/
    
    //lss_delt = lus_ad;
    //lss_delt -= ad_to_temp5[lss_temp_index];

    lss_delt = ad_to_temp5[lss_temp_index] - lus_ad;
    lss_delt *= 10;                                  
    //lss_delt >>= 2;
    lss_delt /= (ad_to_temp5[lss_temp_index] - ad_to_temp5[lss_temp_index + 1]);   //index 越小，AD值越大，温度越小
    
    return (temp5_for_ad[lss_temp_index] * 10 + lss_delt);                         //都是相对比采集的AD值大的那个元素，因为AD值小，温度增大，所以+
}

/*******************************************************************************
函数功能: 加温箱对应的传感器采集的温度

函数位置: ad_temp_calc() 中；

获取内容: 获取加温箱传感器温度
********************************************************************************/
int16_t get_temp6(uint16_t lus_ad)        //根据ad值查表得到温度值6
{
    int16_t lss_delt;
     
    int16_t lss_temp_index;
    lss_temp_index = (TEMP6_TAB_LENGTH >> 1);  
    
    if (lus_ad > ad_to_temp6[lss_temp_index])  //AD值越大，温度越小
    {
        do
        {
            lss_temp_index--;
            if (lss_temp_index < 0)
            {
                lss_temp_index = 0;
                break;
            }
            //------------------------------------
            if (lus_ad < ad_to_temp6[lss_temp_index])
            {
                //lss_temp_index++;  原注
                break;
            }
        }
        while (lus_ad > ad_to_temp6[lss_temp_index]);
    }
    else if (lus_ad < ad_to_temp6[lss_temp_index])
    {
        do
        {
            lss_temp_index++;
            if (lss_temp_index >= TEMP6_TAB_LENGTH)
            {
                lss_temp_index = TEMP6_TAB_LENGTH;
                break;
            }
            //------------------------------------
            if (lus_ad > ad_to_temp6[lss_temp_index])
            {
                lss_temp_index--;  //原未注                 //peak 为了可以和上面那一部分共同使用下面的公式
                break;
            }
        }
        while (lus_ad < ad_to_temp6[lss_temp_index]);
    }

    lss_delt = ad_to_temp6[lss_temp_index] - lus_ad;
    lss_delt *= 10;                                  
    //lss_delt >>= 2;
    lss_delt /= (ad_to_temp6[lss_temp_index] - ad_to_temp6[lss_temp_index + 1]);   //index 越小，AD值越大，温度越小
    
    return (temp6_for_ad[lss_temp_index] * 10 + lss_delt);                         //都是相对比采集的AD值大的那个元素，因为AD值小，温度增大，所以+
}

//------------------------------------------------------------------------------
#endif
