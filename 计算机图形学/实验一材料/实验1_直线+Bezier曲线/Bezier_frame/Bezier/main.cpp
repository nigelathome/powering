
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>
#include "common.h"
#include "letter.h"
#include "line.h"
#include "curve.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//�ӿڴ�С
#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700								

//������ɫ
static GLubyte g_iColorRed,g_iColorGreen,g_iColorBlue;

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

int g_iCtrlPntNum,//bezier���ߵĿ��Ƶ����
g_iPntNum;//����bezier���ߵĵ�ĸ���
wcPt3D *g_pCtrlPoints,//���Ƶ�
*g_pPoints;//�����ϵĵ�

BezierCurve g_curve;//bezier����
bool g_bSelectPoint;//���¿ո��ѡ��һ�����Ƶ㣬�ñ�������Ϊtrue
bool g_bFinish;//����'m'�����������Ƶ��ѡ�񣬸ñ�������Ϊtrue

int iSrcX,iSrcY,iDestX,iDestY;//ֱ����㣬�յ���������

unsigned short g_lineType;//��������
static int tp;

//��ʼ��
void MyInit(void)
{
	g_iCtrlPntNum=0;
	g_iPntNum=0;

	g_pCtrlPoints=(wcPt3D*)malloc(50*sizeof(wcPt3D));

	g_pPoints=(wcPt3D*)malloc(200*sizeof(wcPt3D));

	g_curve.SetPoints(g_pPoints,g_iPntNum);
	g_curve.SetCtrlPoints(g_pCtrlPoints,g_iCtrlPntNum);

	g_bFinish=false;
	g_bSelectPoint=false;

	g_lineType=SOLID_LINE;
	tp=0;
}

//��ɫ
void setColour(int r,int g,int b)
{
	g_iColorRed=r;
	g_iColorGreen=g;
	g_iColorBlue=b;
}

//���ػ���
void setPixel(int i,int j)
{
	static unsigned short t=0x8000;
	static unsigned short t1=t;
	static int nb=8*sizeof(unsigned short);
	static int tx,ty;

	t1=t>>tp;
	tp=(tp+1)%nb;
	if((g_lineType & t1)!=0)
	{
		tx=i;
		ty=j;

		if(0<=ty&&ty<defSCREEN_HEIGHT&&0<=tx&&tx<defSCREEN_WIDTH){
			g_abyScreenImage[ty][tx][0]=g_iColorRed;
			g_abyScreenImage[ty][tx][1]=g_iColorGreen;
			g_abyScreenImage[ty][tx][2]=g_iColorBlue;
		}
	}
}

//��������
void setLineType(unsigned short type)
{
	g_lineType=type;
	tp=0;
}

//��ʼ����Ļ��������Ϊȫ��
void InitScreenImage()
{
	int iHeight,iWidth;
	for(iHeight=0;iHeight<defSCREEN_HEIGHT;iHeight++){
		for(iWidth=0;iWidth<defSCREEN_WIDTH;iWidth++){
			g_abyScreenImage[iHeight][iWidth][0]=0;
			g_abyScreenImage[iHeight][iWidth][1]=0;
			g_abyScreenImage[iHeight][iWidth][2]=0;
		}
	}
}

//�޸�ͼ��
void ModifyScreenImage()
{
	static int i;
	static int dsx,dsy;

	//��ɫ����
	setColour(255,255,255);
	setLineType(DASH_LINE);

	//���ƿ���ͼ
	for(i=0;i<g_curve.GetCtrlPntNum()-1;i++){

		//����Ϊ���������ά�ռ�����ϵ�еĵ�ת����ƽ������ϵ�ķ����ǣ����Ե��z���꣬����x��y����ת��Ϊ��ӽ����ǵ�����
		iSrcX=ROUND(g_pCtrlPoints[i].x);
		iSrcY=ROUND(g_pCtrlPoints[i].y);
		iDestX=ROUND(g_pCtrlPoints[i+1].x);
		iDestY=ROUND(g_pCtrlPoints[i+1].y);

		DDA(iSrcX,iSrcY,iDestX,iDestY,setPixel);
		//Bres(iSrcX,iSrcY,iDestX,iDestY,setPixel);
	}

	//��ɫʵ��
	setColour(255,0,0);
	setLineType(SOLID_LINE);

	//����bezier����
	for(i=0;i<g_curve.GetPntNum()-1;i++){

		iSrcX=ROUND(g_pPoints[i].x);
		iSrcY=ROUND(g_pPoints[i].y);
		iDestX=ROUND(g_pPoints[i+1].x);
		iDestY=ROUND(g_pPoints[i+1].y);

		DDA(iSrcX,iSrcY,iDestX,iDestY,setPixel);
		//Bres(iSrcX,iSrcY,iDestX,iDestY,setPixel);
	}
}

int g_f=5;//���ߵ�������Ƶ����ı�������
//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(iXPos<0||iXPos>=defSCREEN_WIDTH||iYPos<0||iYPos>=defSCREEN_HEIGHT)
		return;

	if(uchKey == 27/*Esc*/) {
		exit(0);
	}
	else if(uchKey>='1'&&uchKey<='5')
	{
		g_f=int(uchKey-'0');
		g_curve.SetPoints(g_pPoints,(g_curve.GetCtrlPntNum()-1)*g_f+1);
		g_curve.bezier(g_curve.GetCtrlPoints(),g_curve.GetCtrlPntNum(),g_curve.GetPoints(),g_curve.GetPntNum());
		glutPostRedisplay();
	}
	
}

//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴��
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{
	if(iButton==GLUT_LEFT_BUTTON&&iState==GLUT_UP)
	{
		if(g_bFinish){//�����п��Ƶ㶼��ѡ���꣬���������£�����ѡ����Ƶ㼯
			g_iCtrlPntNum=0;
			g_bFinish=false;
		}

		if(g_iCtrlPntNum>=30)//����Ŀ��Ƶ㣬�ᵼ�¼���ֵ��������������������ܱ�ʾ����������
			return;

		//�����µĿ��Ƶ�����
		g_pCtrlPoints[g_iCtrlPntNum].x=iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y=defSCREEN_HEIGHT-iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z=0;
		g_iCtrlPntNum++;//���Ƶ�����1

		g_iPntNum=(g_iCtrlPntNum-1)*g_f+1;//�����ϵĵ�������Ƶ���������
		g_curve.SetPoints(g_pPoints,g_iPntNum);//�����������ߵ㼯�͵���
		g_curve.SetCtrlPoints(g_pCtrlPoints,g_iCtrlPntNum);//�������ÿ��Ƶ㼯�͵���

		g_bSelectPoint=true;
	}
	else if(iButton==GLUT_RIGHT_BUTTON&&iState==GLUT_UP)
	{
		if(!g_bFinish){
			//���Ƶ������䣬�������������������ֵ
			g_iPntNum=(g_iCtrlPntNum-1)*g_f+1;
			g_curve.SetPoints(g_pPoints,g_iPntNum);
			g_curve.SetCtrlPoints(g_pCtrlPoints,g_iCtrlPntNum);

			g_bFinish=true;
		}
	}
	g_curve.bezier(g_curve.GetCtrlPoints(),g_curve.GetCtrlPntNum(),g_curve.GetPoints(),g_curve.GetPntNum());

}

//��Ӧ�ƶ����
void PassiveMotionFunc(int iXPos, int iYPos);

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{
	PassiveMotionFunc(iXPos, iYPos);
}

//��Ӧ�ƶ����
void PassiveMotionFunc(int iXPos, int iYPos)
{
	if(!g_bFinish){//���û����ɿ��Ƶ��ѡ��

		//ʵʱ������λ��
		g_pCtrlPoints[g_iCtrlPntNum].x=iXPos;
		g_pCtrlPoints[g_iCtrlPntNum].y=defSCREEN_HEIGHT-iYPos;
		g_pCtrlPoints[g_iCtrlPntNum].z=0;

		if(g_bSelectPoint){//����ո�ѡ����һ�����Ƶ�,��ǰ�Ĺ��λ�û����ܳ�Ϊ�µĿ��Ƶ㣬��Ӧ����ʾ����Ļ�ϣ���ʵ��"�϶�"��Ч��

			//�����һ�����Ƶ�
			g_iPntNum=g_iCtrlPntNum*g_f+1;
			g_curve.SetPoints(g_pPoints,g_iPntNum);
			g_curve.SetCtrlPoints(g_pCtrlPoints,g_iCtrlPntNum+1);
			g_bSelectPoint=false;

		}
		g_curve.bezier(g_curve.GetCtrlPoints(),g_curve.GetCtrlPntNum(),g_curve.GetPoints(),g_curve.GetPntNum());

	}
	glutPostRedisplay();

}

//��Ӧ���ڴ�С�ı�
void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);

}

//��ʼ��
void Init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}
 
//���ƻص�����
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	InitScreenImage();
	ModifyScreenImage();
	glRasterPos2f(-1,-1);
	glDrawPixels(defSCREEN_WIDTH,defSCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_abyScreenImage);
	float y=1,d=40/(float)defSCREEN_HEIGHT;
	y-=d;
	glRasterPos2f(-1,y);
	drawCNString("����������ѡ����Ƶ�(������30��)�������Ҽ�����");
	y-=d;
	glRasterPos2f(-1,y);
	drawCNString("1,2,3,4,5 ����ƽ����");

	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	MyInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("Bezier����(2010)");
	Init();
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutMainLoop();
	return 0;
}

