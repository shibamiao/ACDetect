//COMP_Shape.h: interface for the COMP_Shape class.
//
//////////////////////////////////////////////////////////////////////
#include <Afxtempl.h>

#if !defined(AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_)
#define AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//ת��������ļ����������߶���Ϣ
class CT{             //CT�߶�
public: 
    double BX;//�����߶����x����
	double BY;//�����߶����y����
	double EX;//�����߶��յ�x����
	double EY;//�����߶��յ�y����
};
//ת��������ļ���������Բ����Ϣ
class CA{            //CAԲ��
public:
	double CX;//����Բ��Բ��x����
	double CY;//����Բ��Բ��y����
	double R;//����Բ��Բ�İ뾶
	double SA;//����Բ����ʼ�Ƕ�
	double EA;//����Բ����ֹ�Ƕ�
};
//ת��������ļ�����������������״��Ϣ
class COMP_Shape  //����
{
public:
	char Geometry[50];//������װ
	char Reference[50];//�������
	CList<CT,CT>  comp_CT;//�����߶��б�CTΪһ��class��
	CList<CA,CA>  comp_CA;//����Բ���б�CAΪһ��class��
	COMP_Shape& operator =(const COMP_Shape& New)//����"="
	{
		if (this!=&New)//�������ݲ����
		{
			comp_CT.RemoveAll();//����߶���Ϣ
			comp_CA.RemoveAll();//���Բ����Ϣ
			strcpy(Geometry,New.Geometry);//�������ݵķ�װ������ǰ����
			strcpy(Reference,New.Reference);//�������ݵı�Ÿ�����ǰ����
			POSITION pos1=New.comp_CT.GetHeadPosition();//�߶��б�ͷλ��
			POSITION pos2=New.comp_CA.GetHeadPosition();//Բ���б�ͷλ��
			while(pos1!=NULL)//����߶�ͷλ�ò�Ϊ�գ���
			{
				//�������ݵ��߶��б��е�ֵ������ǰ�������������һ��λ��
				comp_CT.AddTail(New.comp_CT.GetNext(pos1));		
			}
			while(pos2!=NULL)//���Բ��ͷλ�ò�Ϊ�գ���
			{
				//�������ݵ�Բ���б��е�ֵ������ǰ�������������һ��λ��
				comp_CA.AddTail(New.comp_CA.GetNext(pos2));//
			}
		}
		return *this;//���ص�ǰ����
	}
};
//ת��������ļ������е�����������Ϣ
class COMP_List
{
public:
	char Geometry[50];//������װ
	char Symbol[50];//�����ͺ�
	char Reference[50];//�������
	double COMPX;//�������ĵ�X����
	double COMPY;//�������ĵ�Y����d
	double Angle;// ��������ת�Ƕ�
	char Level[20];//�������ڵĲ�
	
};
	
#endif // !defined(AFX_OMP_SHAPE_H__01506856_AF7E_4D6E_A126_1ED6E5683AAA__INCLUDED_)
