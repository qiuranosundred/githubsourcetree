#ifndef __TABLE_H
#define __TABLE_H

#include "main.h"

#include "table_switch_pset.h"

#define MT_2411071


//开机用
#define Array_ROW_NUM	     	91//94//98//3
#define Array_COLUMN_NUM		268//280

//pset用
#define ARRAY_PSET_ROW_NUM 		18
#define ARRAY_PSET_COLUMN_NUM	300

//下发用
#define ARRAY_SET_ROW_NUM 		58-5
#define ARRAY_SET_COLUMN_NUM	150//141

//////////////////////////////////开机用//////////////////////////////////
extern const uint8_t Array[Array_ROW_NUM][Array_COLUMN_NUM];
extern uint16_t Array_Rows_Data_Num[Array_ROW_NUM];
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////启停用//////////////////////////////////
extern uint8_t Disable_Array[2][29];
extern uint8_t Enable_Array[2][29];
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////心跳用//////////////////////////////////
extern uint8_t Heart_Array[10];
//extern uint8_t Setup_Array[12][21];
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////pset用//////////////////////////////////
extern uint8_t (*pArray_Pset_Addr)[ARRAY_PSET_COLUMN_NUM];
extern uint16_t Array_Pset_Rows_Data_Num[ARRAY_PSET_ROW_NUM];
extern const uint8_t asdf[78][268];

//这里添pset声明
extern const uint8_t Array_Pset_1[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
extern const uint8_t Array_Pset_2[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
extern const uint8_t Array_Pset_3[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
extern const uint8_t Array_Pset_4[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
extern const uint8_t Array_Pset_5[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
extern const uint8_t Array_Pset_6[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];

//extern uint8_t Array_Pset_10[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
//extern uint8_t Array_Pset_11[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
//extern uint8_t Array_Pset_12[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
//extern uint8_t Array_Pset_13[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];


extern uint8_t Array_Set_Buffer[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];
/////////////////////////////////////////////////////////////////////////

//////////////////////////////////下发用//////////////////////////////////
extern uint8_t (*pArray_Set_Addr)[ARRAY_SET_COLUMN_NUM];//指向下发配置数组的指针 切换配置时 用该指针指向需的配置
extern uint8_t Array_Set_Rows_Data_Num[ARRAY_SET_ROW_NUM];//记录每一行的数锯长度 下发用


//这里添加配置声明
extern uint8_t Array_Pset1_Angle_120[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];
extern uint8_t Array_Pset2_Angle_360[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];
extern uint8_t Array_Pset3_6Nm[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];
extern uint8_t Array_Pset4_10Nm[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];

extern uint8_t Array_Pset11_Angle_360[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];
extern uint8_t Array_Pset11_Angle_720[ARRAY_SET_ROW_NUM][ARRAY_SET_COLUMN_NUM];


extern uint8_t Array_Pset_Buffer[ARRAY_PSET_ROW_NUM][ARRAY_PSET_COLUMN_NUM];
/////////////////////////////////////////////////////////////////////////


//////////////////////////////////上传用//////////////////////////////////

////三个数组的行数
//#define ARR_BEFORE_SHORT_PACKAGE_LINE_MAX	4
//#define ARR_BEFORE_LONG_PACKAGE_LINE_MAX	7
//#define ARR_AFTER_LONG_PACKAGE_LINE_MAX		3
//
//extern uint8_t Arr_BEFORE_SHORT_PACKAGE[ARR_BEFORE_SHORT_PACKAGE_LINE_MAX][50];
//extern uint8_t Arr_BEFORE_SHORT_PACKAGE_Len[ARR_BEFORE_SHORT_PACKAGE_LINE_MAX];
//
//extern uint8_t Arr_BEFORE_LONG_PACKAGE[ARR_BEFORE_LONG_PACKAGE_LINE_MAX][50];
//extern uint8_t Arr_BEFORE_LONG_PACKAGE_Len[ARR_BEFORE_LONG_PACKAGE_LINE_MAX];
//
//extern uint8_t Arr_AFTER_LONG_PACKAGE[ARR_AFTER_LONG_PACKAGE_LINE_MAX][50];
//extern uint8_t Arr_AFTER_LONG_PACKAGE_Len[ARR_AFTER_LONG_PACKAGE_LINE_MAX];


extern uint8_t Fixed_Answer_Array1[19];

extern uint8_t Short_Package_CMD[21];

extern uint8_t Long_Package_CMD[7][50];
extern uint8_t Long_Package_CMD1[7][50];


extern uint8_t Long_Package_CMD_Num[7];

extern uint8_t Long_Package_Answer[6][50];
extern uint8_t Long_Package_Answer_Num[6];

extern uint8_t Package_CMD11[4][29];
extern uint8_t Package_CMD_Len11[4];

extern uint8_t Package_CMD[14][50];
extern uint8_t Package_CMD_Len[14];
extern uint8_t Package_CMD_Index;
void setPackageCMDGrade(uint8_t Grade_High,uint8_t Grade_Low);
void setPackageCMDShortCheck(uint8_t *addr);
void setPackageCMDLongCheck(uint8_t *addr);
/////////////////////////////////////////////////////////////////////////

void Table_C_Init(void);						//初始化表格
void Compute_Array_Pset_Rows_Data_Num(void);	//计算每一行的有效个数 每次切换pset时要调用
void Compute_Array_Set_Rows_Data_Num(void);		//计算每一行的有效个数 每次切换配置时要调用


#endif
