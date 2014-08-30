//COMP_Shape.h: interface for the COMP_Shape class.
//
//////////////////////////////////////////////////////////////////////
#include <Afxtempl.h>

#if !defined(AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_)
#define AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//转换后设计文件中器件的线段信息
class CT{             //CT线段
public: 
    double BX;//器件线段起点x坐标
	double BY;//器件线段起点y坐标
	double EX;//器件线段终点x坐标
	double EY;//器件线段终点y坐标
};
//转换后设计文件中器件的圆弧信息
class CA{            //CA圆弧
public:
	double CX;//器件圆弧圆心x坐标
	double CY;//器件圆弧圆心y坐标
	double R;//器件圆弧圆的半径
	double SA;//器件圆弧起始角度
	double EA;//器件圆弧终止角度
};
//转换后设计文件中所有类型器件形状信息
class COMP_Shape  //器件
{
public:
	char Geometry[50];//器件封装
	char Reference[50];//器件标号
	CList<CT,CT>  comp_CT;//器件线段列表（CT为一个class）
	CList<CA,CA>  comp_CA;//器件圆弧列表（CA为一个class）
	COMP_Shape& operator =(const COMP_Shape& New)//重载"="
	{
		if (this!=&New)//左右数据不相等
		{
			comp_CT.RemoveAll();//清空线段信息
			comp_CA.RemoveAll();//清空圆弧信息
			strcpy(Geometry,New.Geometry);//将新数据的封装赋给当前变量
			strcpy(Reference,New.Reference);//将新数据的标号赋给当前变量
			POSITION pos1=New.comp_CT.GetHeadPosition();//线段列表头位置
			POSITION pos2=New.comp_CA.GetHeadPosition();//圆弧列表头位置
			while(pos1!=NULL)//如果线段头位置不为空，则
			{
				//将新数据的线段列表中的值赋给当前变量，并获得下一个位置
				comp_CT.AddTail(New.comp_CT.GetNext(pos1));		
			}
			while(pos2!=NULL)//如果圆弧头位置不为空，则
			{
				//将新数据的圆弧列表中的值赋给当前变量，并获得下一个位置
				comp_CA.AddTail(New.comp_CA.GetNext(pos2));//
			}
		}
		return *this;//返回当前变量
	}
};
//转换后设计文件中所有的器件基本信息
class COMP_List
{
public:
	char Geometry[50];//器件封装
	char Symbol[50];//器件型号
	char Reference[50];//器件标号
	double COMPX;//器件中心的X坐标
	double COMPY;//器件中心的Y坐标d
	double Angle;// 器件的旋转角度
	char Level[20];//器件所在的层
	
};
	
#endif // !defined(AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_)
