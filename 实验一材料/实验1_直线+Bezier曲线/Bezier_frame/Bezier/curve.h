#ifndef CURVE_H
#define CURVE_H
#include<stdlib.h>
#include "common.h"


/*=====================================================================
��άֱ������ϵ�е�����
������е��������ɶ���ֱ����β�������Ƶõ���
ͬ�����ȵ����ߣ���������ֱ��Խ�࣬Խ�̣�����Խ����
=====================================================================*/
class Curve{

public:

	//���캯��
	Curve(wcPt3D *pPoints,int iPntNum){
		m_pPoints=pPoints;
		m_iPntNum=iPntNum;
	}
	Curve(){
		m_iPntNum=0;
		m_pPoints=0;
	}

	//���ص���
	int GetPntNum(){
		return m_iPntNum;
	}

	//���ص㼯
	wcPt3D* GetPoints(){
		return m_pPoints;
	}

	//���õ㼯
	void SetPoints(wcPt3D *pPoints,int iPntNum){
		m_pPoints=pPoints;
		m_iPntNum=iPntNum;
	}

private:

	wcPt3D *m_pPoints;//���ߵ㼯�����������㹹��һ��ֱ��
	int m_iPntNum;//��ĸ���

};


/*=====================================================================
bezier������,�̳���curve��.
��һ����Ƶ㣬�����������ߵ���״
=====================================================================*/
class BezierCurve:public Curve{

//��Ա����
public:

	//���캯��
	BezierCurve(wcPt3D *pCtrlPoints,int iCtrlPntNum,wcPt3D *pPoints,int iPntNum):Curve(pPoints,iPntNum){//���캯��
		m_pCtrlPoints=pCtrlPoints;
		m_iCtrlPntNum=iCtrlPntNum;
	}
	BezierCurve():Curve(){
		m_iCtrlPntNum=0;
		m_pCtrlPoints=0;
	}
	
	//���ؿ��Ƶ���
	int GetCtrlPntNum(){
		return m_iCtrlPntNum;
	}

	//���ؿ��Ƶ㼯
	wcPt3D* GetCtrlPoints(){
		return m_pCtrlPoints;
	}

	//���ÿ��Ƶ㼯
	void SetCtrlPoints(wcPt3D *pCtrlPoints,int iCtrlPntNum){
		m_pCtrlPoints=pCtrlPoints;
		m_iCtrlPntNum=iCtrlPntNum;
	}

	//����bezier����
	void bezier(wcPt3D *ctrlPts,int nCtrlPts,wcPt3D *bezCurvePts,int nBezCurvePts);

private:

	//����n!/(k!(n-k)!),�洢��c[k],����k=0,1,...,n
	void binomiaCoeffs(int n,float *c);

	//Bezier������,���ΪB(0),�յ�ΪB(1),�ú��������B(u)�����꣬����0<=u<=1;
	void computeBezPt(float u,wcPt3D *bezPt,int nCtrlPts,wcPt3D *ctrlPts,float *C);
	

//��Ա����
private:

	wcPt3D *m_pCtrlPoints;//���Ƶ㼯
	int m_iCtrlPntNum;//���Ƶ����,���ܳ���30
};

#endif