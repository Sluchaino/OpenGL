#include "Render.h"




#include <windows.h>

#include <GL\gl.h>
#include <GL\glu.h>
#include "GL\glext.h"

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "MyShaders.h"

#include "ObjLoader.h"
#include "GUItextRectangle.h"

#include "Texture.h"

GuiTextRectangle rec;

bool textureMode = true;
bool lightMode = true;
bool upMove = false;
double beginSpeed = 2;
ObjFile Plane(93, beginSpeed), Car(2, 6,2.2,0.9,-0.9,0.2), Car1(2,5.9 - 70,1.9 - 70, 1 , -1, beginSpeed,75), Plane1(93, beginSpeed), Plane2(465, beginSpeed), Plane3(465, beginSpeed), Sky;

Texture CarTex, PlaneTex, Car1Tex, Plane1Tex, Car2Tex, Plane2Tex, Plane3Tex, SkyTex;

//��������� ������ ��� ��������� ����
#define POP glPopMatrix();
#define PUSH glPushMatrix();


ObjFile *model;

Texture texture1;
Texture sTex;
Texture rTex;
Texture tBox;

Shader s[10];  //��������� ��� ������ ��������
Shader frac;
Shader cassini;


//����� ��� ��������� ������
class CustomCamera : public Camera
{
public:
	//��������� ������
	double camDist;
	//���� �������� ������
	double fi1, fi2;

	
	//������� ������ �� ���������
	CustomCamera()
	{
		camDist = 12;
		fi1 = 0;
		fi2 = 0.55;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	virtual void SetUpCamera()
	{

		lookPoint.setCoords(0, 0, 0);

		pos.setCoords(camDist*cos(fi2)*cos(fi1),
			camDist*cos(fi2)*sin(fi1),
			camDist*sin(fi2));

		if (cos(fi2) <= 0)
			normal.setCoords(0, 0, -1);
		else
			normal.setCoords(0, 0, 1);

		LookAt();
	}

	void CustomCamera::LookAt()
	{
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ���������!
class WASDcamera :public CustomCamera
{
//public:
//		
//	float camSpeed;

	/*WASDcamera()
	{
		camSpeed = 0.4;
		pos.setCoords(5, 5, 5);
		lookPoint.setCoords(0, 0, 0);
		normal.setCoords(0, 0, 1);
	}*/

	/*virtual void SetUpCamera()
	{

		if (OpenGL::isKeyPressed('W'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*camSpeed;
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}
		if (OpenGL::isKeyPressed('S'))
		{
			Vector3 forward = (lookPoint - pos).normolize()*(-camSpeed);
			pos = pos + forward;
			lookPoint = lookPoint + forward;
			
		}

		LookAt();
	}*/
	
} WASDcam;


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(10, 106, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, 0);
		//Shader::DontUseShaders();
		//bool f1 = glIsEnabled(GL_LIGHTING);
		//glDisable(GL_LIGHTING);
		//bool f2 = glIsEnabled(GL_TEXTURE_2D);
		//glDisable(GL_TEXTURE_2D);
		//bool f3 = glIsEnabled(GL_DEPTH_TEST);
		//
		//glDisable(GL_DEPTH_TEST);
		//glColor3d(0.9, 0.8, 0);
		//Sphere s;
		//s.pos = pos;
		//s.scale = s.scale*0.08;
		//s.Show();

		//if (OpenGL::isKeyPressed('G'))
		//{
		//	glColor3d(0, 0, 0);
		//	//����� �� ��������� ����� �� ����������
		//		glBegin(GL_LINES);
		//	glVertex3d(pos.X(), pos.Y(), pos.Z());
		//	glVertex3d(pos.X(), pos.Y(), 0);
		//	glEnd();

		//	//������ ���������
		//	Circle c;
		//	c.pos.setCoords(pos.X(), pos.Y(), 0);
		//	c.scale = c.scale*1.5;
		//	c.Show();
		//}
		/*
		if (f1)
			glEnable(GL_LIGHTING);
		if (f2)
			glEnable(GL_TEXTURE_2D);
		if (f3)
			glEnable(GL_DEPTH_TEST);
			*/
	}

	void SetUpLight()
	{
		GLfloat amb[] = { 0.2, 0.2, 0.2, 0 };
		GLfloat dif[] = { 1.0, 1.0, 1.0, 0 };
		GLfloat spec[] = { .7, .7, .7, 0 };
		GLfloat position[] = { pos.X(), pos.Y(), pos.Z(), 1. };

		// ��������� ��������� �����
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		// �������������� ����������� �����
		// ������� ��������� (���������� ����)
		glLightfv(GL_LIGHT0, GL_AMBIENT, amb);
		// ��������� ������������ �����
		glLightfv(GL_LIGHT0, GL_DIFFUSE, dif);
		// ��������� ���������� ������������ �����
		glLightfv(GL_LIGHT0, GL_SPECULAR, spec);

		glEnable(GL_LIGHT0);
	}


} light;  //������� �������� �����



//������ ���������� ����
int mouseX = 0, mouseY = 0;




float offsetX = 0, offsetY = 0;
float zoom=1;
float Time = 0;
int tick_o = 0;
int tick_n = 0;

//���������� �������� ����
void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	/*int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;*/

	////������ ���� ������ ��� ������� ����� ������ ����
	/*if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}


	if (OpenGL::isKeyPressed(VK_LBUTTON))
	{
		offsetX -= 1.0*dx/ogl->getWidth()/zoom;
		offsetY += 1.0*dy/ogl->getHeight()/zoom;
	}*/


	
	//������� ���� �� ���������, � ����� ��� ����
	/*if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y,60,ogl->aspect);

		double z = light.pos.Z();

		double k = 0, x = 0, y = 0;
		if (r.direction.Z() == 0)
			k = 0;
		else
			k = (z - r.origin.Z()) / r.direction.Z();

		x = k*r.direction.X() + r.origin.X();
		y = k*r.direction.Y() + r.origin.Y();

		light.pos = Vector3(x, y, z);
	}

	if (OpenGL::isKeyPressed('G') && OpenGL::isKeyPressed(VK_LBUTTON))
	{
		light.pos = light.pos + Vector3(0, 0, 0.02*dy);
	}*/

	
}

//���������� �������� ������  ����
void mouseWheelEvent(OpenGL *ogl, int delta)
{
	/*float _tmpZ = delta*0.003;
	if (ogl->isKeyPressed('Z'))
		_tmpZ *= 10;
	zoom += 0.2*zoom*_tmpZ;


	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;*/
}

//���������� ������� ������ ����������
void keyDownEvent(OpenGL *ogl, int key)
{
	
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}

//����������� ����� ������ ��������
void initRender(OpenGL *ogl)
{

	//��������� �������

	//4 ����� �� �������� �������
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	//��������� ������ ��������� �������
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	//�������� ��������
	glEnable(GL_TEXTURE_2D);
	
	


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
	//ogl->mainCamera = &WASDcam;
	ogl->mainLight = &light;

	// ������������ �������� : �� ����� ����� ����� 1
	glEnable(GL_NORMALIZE);

	// ���������� ������������� ��� �����
	glEnable(GL_LINE_SMOOTH); 


	//   ������ ��������� ���������
	//  �������� GL_LIGHT_MODEL_TWO_SIDE - 
	//                0 -  ������� � ���������� �������� ���������(�� ���������), 
	//                1 - ������� � ���������� �������������� ������� ��������       
	//                �������������� ������� � ���������� ��������� ����������.    
	//  �������� GL_LIGHT_MODEL_AMBIENT - ������ ������� ���������, 
	//                �� ��������� �� ���������
	// �� ��������� (0.2, 0.2, 0.2, 1.0)

	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 0);

	/*
	//texture1.loadTextureFromFile("textures\\texture.bmp");   �������� �������� �� �����
	*/


	frac.VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	frac.FshaderFileName = "shaders\\frac.frag"; //��� ����� ������������ �������
	frac.LoadShaderFromFile(); //��������� ������� �� �����
	frac.Compile(); //�����������

	cassini.VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	cassini.FshaderFileName = "shaders\\cassini.frag"; //��� ����� ������������ �������
	cassini.LoadShaderFromFile(); //��������� ������� �� �����
	cassini.Compile(); //�����������
	

	s[0].VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	s[0].FshaderFileName = "shaders\\light.frag"; //��� ����� ������������ �������
	s[0].LoadShaderFromFile(); //��������� ������� �� �����
	s[0].Compile(); //�����������

	s[1].VshaderFileName = "shaders\\v.vert"; //��� ����� ���������� ������
	s[1].FshaderFileName = "shaders\\textureShader.frag"; //��� ����� ������������ �������
	s[1].LoadShaderFromFile(); //��������� ������� �� �����
	s[1].Compile(); //�����������

	
	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\PlaneTrava.obj_m", &Plane2);
	Plane2Tex.loadTextureFromFile("textures//PlaneTrava.bmp");
	Plane2Tex.bindTexture();

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\PlaneTrava.obj_m", &Plane3);
	Plane3Tex.loadTextureFromFile("textures//PlaneTrava.bmp");
	Plane3Tex.bindTexture();

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\Sky.obj_m", &Sky);
	SkyTex.loadTextureFromFile("textures//Sky.bmp");
	SkyTex.bindTexture();

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\stena.obj_m", &Car1);
	Car1Tex.loadTextureFromFile("textures//stenaT.bmp");
	Car1Tex.bindTexture();
	Car1.Car1();
	 //��� ��� ��� ������� ������ *.obj �����, ��� ��� ��� ��������� �� ���������� � ���������� �������, 
	 // ������������ �� ����� ����������, � ������������ ������ � *.obj_m
	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\Plane.obj_m", &Plane);
	PlaneTex.loadTextureFromFile("textures//Plane.bmp");
	PlaneTex.bindTexture();

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\Plane.obj_m", &Plane1);
	Plane1Tex.loadTextureFromFile("textures//Plane.bmp");
	Plane1Tex.bindTexture();

	glActiveTexture(GL_TEXTURE0);
	loadModel("models\\LowPolyCars.obj_m", &Car);
	CarTex.loadTextureFromFile("textures//Car Texture 1.bmp");
	CarTex.bindTexture();
	

	tick_n = GetTickCount();
	tick_o = tick_n;

	/*rec.setSize(300, 100);
	rec.setPosition(10, ogl->getHeight() - 100-10);
	rec.setText("T - ���/���� �������\nL - ���/���� ���������\nF - ���� �� ������\nG - ������� ���� �� �����������\nG+��� ������� ���� �� ���������",0,0,0);*/

	
}
void Render(OpenGL *ogl)
{   
	
	tick_o = tick_n;
	tick_n = GetTickCount();
	Time += (tick_n - tick_o) / 1000.0;

	/*
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 1, 0, 1, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();

	glEnable(GL_DEPTH_TEST);
	if (textureMode)
		glEnable(GL_TEXTURE_2D);

	if (lightMode)
		glEnable(GL_LIGHTING);

	//��������������
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//��������� ���������
	GLfloat amb[] = { 0.2, 0.2, 0.1, 1. };
	GLfloat dif[] = { 0.4, 0.65, 0.5, 1. };
	GLfloat spec[] = { 0.9, 0.8, 0.3, 1. };
	GLfloat sh = 0.1f * 256;

	//�������
	glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
	//��������
	glMaterialfv(GL_FRONT, GL_DIFFUSE, dif);
	//����������
	glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
	//������ �����
	glMaterialf(GL_FRONT, GL_SHININESS, sh);

	//===================================
	//������� ���  

	//





	//s[0].UseShader();

	//�������� ���������� � ������.  ��� ���� - ���� ����� uniform ���������� �� �� �����. 
	int location = glGetUniformLocationARB(s[0].program, "light_pos");
	//��� 2 - �������� �� ��������
	glUniform3fARB(location, light.pos.X(), light.pos.Y(),light.pos.Z());

	location = glGetUniformLocationARB(s[0].program, "Ia");
	glUniform3fARB(location, 0.2, 0.2, 0.2);

	location = glGetUniformLocationARB(s[0].program, "Id");
	glUniform3fARB(location, 1.0, 1.0, 1.0);

	location = glGetUniformLocationARB(s[0].program, "Is");
	glUniform3fARB(location, .7, .7, .7);


	location = glGetUniformLocationARB(s[0].program, "ma");
	glUniform3fARB(location, 0.2, 0.2, 0.1);

	location = glGetUniformLocationARB(s[0].program, "md");
	glUniform3fARB(location, 0.4, 0.65, 0.5);

	location = glGetUniformLocationARB(s[0].program, "ms");
	glUniform4fARB(location, 0.9, 0.8, 0.3, 25.6);

	location = glGetUniformLocationARB(s[0].program, "camera");
	glUniform3fARB(location, camera.pos.X(), camera.pos.Y(), camera.pos.Z());

	
	
	
	s[1].UseShader();
	int l = glGetUniformLocationARB(s[1].program, "tex");
	glUniform1iARB(l, 0);

	PUSH;
	glRotated(90, 180, 0, 1);
	glTranslated(-70, 0, 0);
	Car1Tex.bindTexture();
	Car1.RenderModel(GL_POLYGON);
	Car1.DrawObj();
	upMove = Car1.MoveF();
	if (Car1.col  % 10 == 0 && upMove == true && Car1.col > 0 && Car1.col <= 30)
	{
		Car.Move += 0.05;
		upMove = false;
	}
	POP;

	PUSH;
	glRotated(90, 90, 0, 1);
	glTranslated(-107, 0, 0);
	Plane1Tex.bindTexture();
	Plane1.RenderModel(GL_POLYGON);
	Plane1.DrawObj();
	Plane1.MovePlane();
	POP;

	PUSH;
	glRotated(90, 90, 0, 1);
	glTranslated(-230, -0.1, 0);
	Plane2Tex.bindTexture();
	Plane2.RenderModel(GL_POLYGON);
	Plane2.DrawObj();
	Plane2.MovePlane();
	POP;

	PUSH;
	glRotated(90, 90, 0, 1);
	glTranslated(-695, -0.1, 0);
	Plane3Tex.bindTexture();
	Plane3.RenderModel(GL_POLYGON);
	Plane3.DrawObj();
	Plane3.MovePlane();
	POP;

	PUSH;
	glRotated(90, 0, 0, 1);
	glTranslated(0, 75, 0);
	SkyTex.bindTexture();
	Sky.RenderModel(GL_POLYGON);
	Sky.DrawObj();
	POP;

	PUSH;
	glRotated(90, 90, 0, 1);
	glTranslated(-15,0,0);
	PlaneTex.bindTexture();
	Plane.RenderModel(GL_POLYGON);
	Plane.DrawObj();
	Plane.MovePlane();
	POP;

	

	PUSH;
	glRotated(90, 90, 0, 1);
	CarTex.bindTexture();
	Car.RenderModel(GL_POLYGON);
	Car.DrawObj();
	if (OpenGL::isKeyPressed('D'))
		Car.MoveLeft();
	if (OpenGL::isKeyPressed('A'))
		Car.MoveRight();
	if (Car.Stolk(Car1))
	{
		Car.Now = 0;
		Car.maxX = 6;
		Car.minX = 2.2;
		Car.maxZ = 0.9;
		Car.minZ = -0.9;
		Car.Move = 0.2;
		Car.nowPos = 0;

		Car1.Now = 0;
		Car1.maxX = 5.9 - 70;
		Car1.minX = 1.9 - 70;
		Car1.maxZ = 1;
		Car1.minZ = -1;
		Car1.Move = beginSpeed;
		Car1.nowPos = 0;
		Car1.col = 0;
		Car1.Car1();
		upMove = false;

		Plane.Move = beginSpeed;
		Plane.nowPos = 0;
		Plane.col = 0;

		Plane1.Move = beginSpeed;
		Plane1.nowPos = 0;
		Plane1.col = 0;

		Plane2.Move = beginSpeed;
		Plane2.nowPos = 0;
		Plane2.col = 0;

		Plane3.Move = beginSpeed;
		Plane3.nowPos = 0;
		Plane3.col = 0;
	}
	POP;

	Shader::DontUseShaders();
}   //����� ���� �������


bool gui_init = false;

//������ ���������, ��������� ����� �������� �������
void RenderGUI(OpenGL *ogl)
{
	
	Shader::DontUseShaders();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	

	glActiveTexture(GL_TEXTURE0);
	rec.Draw();


		
	Shader::DontUseShaders(); 



	
}

void resizeEvent(OpenGL *ogl, int newW, int newH)
{
	rec.setPosition(10, newH - 100 - 10);
}

