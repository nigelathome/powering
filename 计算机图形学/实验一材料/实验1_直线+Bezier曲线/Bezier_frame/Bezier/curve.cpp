
#include "curve.h"
#include <math.h>
#include <glut.h>

//����n!/(k!(n-k)!),�洢��c[k],����k=0,1,...,n
void BezierCurve::binomiaCoeffs(int n,float *c){

	int k,i;

	for(k=0;k<=n;k++){
		//����n!/(k!(n-k)!)
		c[k]=1;
		for(i=n;i>=k+1;i--)
			c[k]*=i;
		for(i=n-k;i>=2;i--)
			c[k]/=i;
	}

}


//Bezier������,���ΪB(0),�յ�ΪB(1),�ú��������B(u)�����꣬����0<=u<=1;
//bezPt ָ��B(u)��ָ��,
//nCtrlPts ���Ƶ����
//ctrlPts ���Ƶ��ڴ���ʼ��ַ
void BezierCurve::computeBezPt(float u,wcPt3D *bezPt,int nCtrlPts,wcPt3D *ctrlPts,float *C){

	//powf(x,k) x��k�η�
	//��Ӵ���
}


//����bezier����
//ctrlPts ���Ƶ������ڴ���ʼ��ַ
//nCtrlPts ���Ƶ����
//bezCurvePts ���ߵ������ڴ���ʼ��ַ
//nBezCurvePts ���ߵ����
void BezierCurve::bezier(wcPt3D *ctrlPts,int nCtrlPts,wcPt3D *bezCurvePts,int nBezCurvePts){

	//��Ӵ���
	
}
