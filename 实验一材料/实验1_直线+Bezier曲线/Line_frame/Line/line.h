
#ifndef LINE_H
#define LINE_H

//DDA�㷨����ֱ��
//(xa,ya),�����������
//(xb,yb),�յ���������
//setPixel,���ش�����
void DDA(int xa,int ya,int xb,int yb,void (*setPixel)(int x,int y));

//Bresenham�㷨����ֱ��
//����ͬDDA
void Bres(int xa,int ya,int xb,int yb,void (*setPixel)(int x,int y));

#endif