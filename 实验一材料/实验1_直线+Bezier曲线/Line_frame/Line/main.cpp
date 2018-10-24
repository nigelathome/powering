
#include <stdlib.h>
#include <stdio.h>
#include <glut.h>

#include "common.h"
#include "letter.h"
#include "line.h"

#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")

//视口大小
#define defSCREEN_WIDTH 700								
#define defSCREEN_HEIGHT 700

int g_lineNum;//直线数目
bool g_finish;//数目是否达上限
wcPt2D *g_lines;//直线端点数组

//像素颜色
static GLubyte g_iColorRed,g_iColorGreen,g_iColorBlue;

//屏幕像素数组
GLubyte g_abyScreenImage[defSCREEN_HEIGHT][defSCREEN_WIDTH][3];

unsigned short g_lineType;//线条类型
static int tp;

bool g_lbd=false;//鼠标左键是否按下

//初始化
void MyInit(void)
{
	g_lineNum=0;
	g_finish=false;
	g_lines=(wcPt2D*)malloc(100*sizeof(wcPt2D));

	g_lineType=SOLID_LINE;
	tp=0;
}

//颜色
void setColour(int r,int g,int b)
{
	g_iColorRed=r;
	g_iColorGreen=g;
	g_iColorBlue=b;
}

//像素绘制
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

//线条类型
void setLineType(unsigned short type)
{
	g_lineType=type;
	tp=0;
}

//初始化屏幕像素数组为全黑
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

//绘制学号
void DrawStudentID()
{
	int w=32,h=46;
	int x=defSCREEN_WIDTH/2-(9*w+8*8)/2,y=defSCREEN_HEIGHT/2-h/2;
	int d=w+8;
	setColour(255,255,255);
	setLineType(SOLID_LINE);

	//请修改以下代码，改成自己的学号
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
//修改图像
void ModifyScreenImage()
{
	static int i;

	//红色实线
	setColour(255,0,0);
	setLineType(SOLID_LINE);

	for(i=0;i<g_lineNum;i++){

		DDA(ROUND(g_lines[i*2].x),ROUND(g_lines[i*2].y),ROUND(g_lines[i*2+1].x),ROUND(g_lines[i*2+1].y),setPixel);
	}

}

//响应键盘按键
void Keyboard(unsigned char uchKey, int iXPos, int iYPos)
{
	if(iXPos<0||iXPos>=defSCREEN_WIDTH||iYPos<0||iYPos>=defSCREEN_HEIGHT)
		return;

	if(uchKey == 27/*Esc*/) {
		exit(0);
	}
	else if(uchKey=='c'||uchKey=='C'){//清屏
		g_lineNum=0;
		g_finish=false;
	}

	glutPostRedisplay();
}

//响应特殊按键
void Special(int iKey, int iXPos, int iYPos)
{

}

//响应鼠标按键
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

//响应按下一个鼠标按钮移动鼠标
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

//响应移动鼠标
void PassiveMotionFunc(int iXPos, int iYPos)
{
}

//相应窗口大小改变
void Reshape(int iWidth, int iHeight)
{
	glutReshapeWindow(defSCREEN_WIDTH,defSCREEN_HEIGHT);

}

//初始化
void Init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	InitScreenImage();

}

//绘制回调函数
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
	drawCNString("鼠标拖动画直线(不超过40条)");
	y-=d;
	glRasterPos2f(-1,y);
	drawCNString("'c'清屏");

	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	MyInit();
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(defSCREEN_WIDTH,defSCREEN_HEIGHT);
	glutCreateWindow("直线(2010)");
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

