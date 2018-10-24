
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>

#include "common.h"
#include "letter.h"
#include "line.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//�ӿڴ�С
#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700

int g_lineNum;//ֱ����Ŀ
bool g_finish;//��Ŀ�Ƿ������
wcPt2D *g_lines;//ֱ�߶˵�����

//������ɫ
static GLubyte g_iColorRed,g_iColorGreen,g_iColorBlue;

//��Ļ��������
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

unsigned short g_lineType;//��������
static int tp;

bool g_lbd=false;//�������Ƿ���

//��ʼ��
void MyInit(void)
{
	g_lineNum=0;
	g_finish=false;
	g_lines=(wcPt2D*)malloc(100*sizeof(wcPt2D));

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

//����ѧ��
void DrawStudentID()
{
	int w=32,h=46;
	int x=defSCREEN_WIDTH/2-(9*w+8*8)/2,y=defSCREEN_HEIGHT/2-h/2;
	int d=w+8;
	setColour(255,255,255);
	setLineType(SOLID_LINE);

	//���޸����´��룬�ĳ��Լ���ѧ��
	//s
	DDA(x,y+h/2,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h/2,setPixel);
	DDA(x,y+h,x+w,y+h,setPixel);
	DDA(x,y+h/2,x+w,y+h/2,setPixel);
	x+=d;
	//y
	DDA(x,y+h,x+w/2,y+h/2,setPixel);
	DDA(x+w,y+h,x+w/2,y+h/2,setPixel);
	DDA(x+w/2,y+h/2,x+w/2,y,setPixel);
	x+=d;
	//1
	DDA(x,y,x,y+h,setPixel);
	x+=d;
	//0
	DDA(x,y,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h,setPixel);
	DDA(x,y+h,x+w,y+h,setPixel);
	x+=d;
	//0
	DDA(x,y,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h,setPixel);
	DDA(x,y+h,x+w,y+h,setPixel);
	x+=d;
	//6
	DDA(x,y+h,x+w,y+h,setPixel);
	DDA(x,y,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h/2,setPixel);
	DDA(x,y+h/2,x+w,y+h/2,setPixel);
	x+=d;
	//2
	DDA(x,y+h,x+w,y+h,setPixel);
	DDA(x+w,y+h/2,x+w,y+h,setPixel);
	DDA(x,y+h/2,x+w,y+h/2,setPixel);
	DDA(x,y,x,y+h/2,setPixel);
	DDA(x,y,x+w,y,setPixel);
	x+=d;
	//0
	DDA(x,y,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h,setPixel);
	DDA(x,y+h,x+w,y+h,setPixel);
	x+=d;
	//8
	DDA(x,y,x,y+h,setPixel);
	DDA(x,y,x+w,y,setPixel);
	DDA(x+w,y,x+w,y+h,setPixel);
	DDA(x,y+h,x+w,y+h,setPixel);
	DDA(x,y+h/2,x+w,y+h/2,setPixel);
	x+=d;
}
//�޸�ͼ��
void ModifyScreenImage()
{
	static int i;

	//��ɫʵ��
	setColour(255,0,0);
	setLineType(SOLID_LINE);

	for(i=0;i<g_lineNum;i++){

		DDA(ROUND(g_lines[i*2].x),ROUND(g_lines[i*2].y),ROUND(g_lines[i*2+1].x),ROUND(g_lines[i*2+1].y),setPixel);
	}

}

//��Ӧ���̰���
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(iXPos<0||iXPos>=defSCREEN_WIDTH||iYPos<0||iYPos>=defSCREEN_HEIGHT)
		return;

	if(uchKey == 27/*Esc*/) {
		exit(0);
	}
	else if(uchKey=='c'||uchKey=='C'){//����
		g_lineNum=0;
		g_finish=false;
	}

	glutPostRedisplay();
}

//��Ӧ���ⰴ��
void Special(int iKey, int iXPos, int iYPos)
{

}

//��Ӧ��갴��
void Mouse(int iButton,int iState,int iXPos, int iYPos)
{
	if(iButton==GLUT_LEFT_BUTTON&&iState==GLUT_DOWN)
	{
		g_lbd=true;

		if(g_lineNum>=40)
		{
			g_finish=true;
			return;
		}

		g_lines[g_lineNum*2].x=g_lines[g_lineNum*2+1].x=iXPos;
		g_lines[g_lineNum*2].y=g_lines[g_lineNum*2+1].y=defSCREEN_HEIGHT-iYPos;

		g_lineNum++;
	}
	else if(iButton==GLUT_LEFT_BUTTON&&iState==GLUT_UP&&!g_finish)
	{
		g_lbd=false;
		{
			g_lines[g_lineNum*2-1].x=iXPos;
			g_lines[g_lineNum*2-1].y=defSCREEN_HEIGHT-iYPos;
		}
	}

}

//��Ӧ����һ����갴ť�ƶ����
void Motion(int iXPos, int iYPos)
{
	if(g_lbd)
	{
		if(!g_finish){

			g_lines[g_lineNum*2-1].x=iXPos;
			g_lines[g_lineNum*2-1].y=defSCREEN_HEIGHT-iYPos;
			glutPostRedisplay();

		}
	}

}

//��Ӧ�ƶ����
void PassiveMotionFunc(int iXPos, int iYPos)
{
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

}

//���ƻص�����
void Display() 
{	
	glClear(GL_COLOR_BUFFER_BIT);
	InitScreenImage();
	DrawStudentID();
	ModifyScreenImage();
	glRasterPos2f(-1,-1);
	glDrawPixels(defSCREEN_WIDTH,defSCREEN_HEIGHT,GL_RGB,GL_UNSIGNED_BYTE,g_abyScreenImage);
	float y=1,d=40/(float)defSCREEN_HEIGHT;
	y-=d;
	glRasterPos2f(-1,y);
	drawCNString("����϶���ֱ��(������40��)");
	y-=d;
	glRasterPos2f(-1,y);
	drawCNString("'c'����");

	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	MyInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("ֱ��(2010)");
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

