#include "Render.h"

#include <sstream>
#include <iostream>

#include <windows.h>
#include <GL\GL.h>
#include <GL\GLU.h>

#include "MyOGL.h"

#include "Camera.h"
#include "Light.h"
#include "Primitives.h"

#include "GUItextRectangle.h"

#include <Math.h>
#include <vector>
#define P 3.14159
double height = 1;
double z = 1;
std::vector<std::vector<double>> aaa1;
std::vector<std::vector<double>> matrixRotate = {
	{1,0,0},
	{0,1,0},
	{0,0,1},
};
double AngelY = 0;
double AngelX = 0;
bool textureMode = true;
bool lightMode = true;

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
		camDist = 15;
		fi1 = 1;
		fi2 = 1;
	}

	
	//������� ������� ������, ������ �� ����� ��������, ���������� �������
	void SetUpCamera()
	{
		//�������� �� ������� ������ ������
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
		//������� ��������� ������
		gluLookAt(pos.X(), pos.Y(), pos.Z(), lookPoint.X(), lookPoint.Y(), lookPoint.Z(), normal.X(), normal.Y(), normal.Z());
	}



}  camera;   //������� ������ ������


//����� ��� ��������� �����
class CustomLight : public Light
{
public:
	CustomLight()
	{
		//��������� ������� �����
		pos = Vector3(1, 1, 3);
	}

	
	//������ ����� � ����� ��� ���������� �����, ���������� �������
	void  DrawLightGhismo()
	{
		glDisable(GL_LIGHTING);

		
		glColor3d(0.9, 0.8, 0);
		Sphere s;
		s.pos = pos;
		s.scale = s.scale*0.08;
		s.Show();
		
		if (OpenGL::isKeyPressed('G'))
		{
			glColor3d(0, 0, 0);
			//����� �� ��������� ����� �� ����������
			glBegin(GL_LINES);
			glVertex3d(pos.X(), pos.Y(), pos.Z());
			glVertex3d(pos.X(), pos.Y(), 0);
			glEnd();

			//������ ���������
			Circle c;
			c.pos.setCoords(pos.X(), pos.Y(), 0);
			c.scale = c.scale*1.5;
			c.Show();
		}

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

void mouseEvent(OpenGL *ogl, int mX, int mY)
{
	int dx = mouseX - mX;
	int dy = mouseY - mY;
	mouseX = mX;
	mouseY = mY;

	//������ ���� ������ ��� ������� ����� ������ ����
	if (OpenGL::isKeyPressed(VK_RBUTTON))
	{
		camera.fi1 += 0.01*dx;
		camera.fi2 += -0.01*dy;
	}

	
	//������� ���� �� ���������, � ����� ��� ����
	if (OpenGL::isKeyPressed('G') && !OpenGL::isKeyPressed(VK_LBUTTON))
	{
		LPPOINT POINT = new tagPOINT();
		GetCursorPos(POINT);
		ScreenToClient(ogl->getHwnd(), POINT);
		POINT->y = ogl->getHeight() - POINT->y;

		Ray r = camera.getLookRay(POINT->x, POINT->y);

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
	}

	
}

void mouseWheelEvent(OpenGL *ogl, int delta)
{

	if (delta < 0 && camera.camDist <= 1)
		return;
	if (delta > 0 && camera.camDist >= 100)
		return;

	camera.camDist += 0.01*delta;

}
void keyDownEvent(OpenGL *ogl, int key)
{
	if (key == 'L')
	{
		lightMode = !lightMode;
	}

	if (key == 'T')
	{
		textureMode = !textureMode;
		++z;
	}
	/*matrixRotate = {
			{cos(AngelY), 0 , -sin(AngelY)},
			{0, cos(AngelX), -sin(AngelX)},
			{sin(AngelY), 0 ,cos(AngelY)},
	};*/
	if (key == 'A')
	{
		AngelY += 0.01;
		/*matrixRotate = {
			{cos(AngelY), 0 , sin(AngelY)},
			{0, 1, 0},
			{-sin(AngelY), 0 ,cos(AngelY)},
		};*/
		matrixRotate = {
  {cos(AngelY), 0, sin(AngelY)},
  {sin(AngelX) * sin(AngelY), cos(AngelX), -sin(AngelX) * cos(AngelY)},
  {-cos(AngelX) * sin(AngelY), sin(AngelX), cos(AngelX) * cos(AngelY)}
		};
	}
	if (key == 'D')
	{
		AngelY -= 0.01;
		
		/*matrixRotate = {
			{cos(AngelY), 0 , sin(AngelY)},
			{0, 1, 0},
			{-sin(AngelY), 0 ,cos(AngelY)},
		};*/
		

		matrixRotate = {
  {cos(AngelY), 0, sin(AngelY)},
  {sin(AngelX) * sin(AngelY), cos(AngelX), -sin(AngelX) * cos(AngelY)},
  {-cos(AngelX) * sin(AngelY), sin(AngelX), cos(AngelX) * cos(AngelY)}
		};
	}
	if (key == 'W')
	{
		AngelX -= 0.01;
		if (AngelY != 0.0)
			AngelY -= 0.01;
		matrixRotate = {
  {cos(AngelY), 0, sin(AngelY)},
  {sin(AngelX) * sin(AngelY), cos(AngelX), -sin(AngelX) * cos(AngelY)},
  {-cos(AngelX) * sin(AngelY), sin(AngelX), cos(AngelX) * cos(AngelY)}
		};
		/*matrixRotate = {
			{1, 0 , 0},
			{0, cos(AngelX), -sin(AngelX)},
			{0, sin(AngelX) ,cos(AngelX)},
		};*/
	}
	if (key == 'S')
	{
		AngelX += 0.01;
		/*matrixRotate = {
			{1, 0 , 0},
			{0, cos(AngelX), -sin(AngelX)},
			{0, sin(AngelX) ,cos(AngelX)},
		};*/
		matrixRotate = {
  {cos(AngelY), 0, sin(AngelY)},
  {sin(AngelX) * sin(AngelY), cos(AngelX), -sin(AngelX) * cos(AngelY)},
  {-cos(AngelX) * sin(AngelY), sin(AngelX), cos(AngelX) * cos(AngelY)}
		};
	}
	if (key == 'R')
	{
		camera.fi1 = 1;
		camera.fi2 = 1;
		camera.camDist = 15;

		light.pos = Vector3(1, 1, 3);
	}

	if (key == 'F')
	{
		light.pos = camera.pos;
	}
}

void keyUpEvent(OpenGL *ogl, int key)
{
	
}



GLuint car, stankin, texture[8];
char* names[] = { "stankin.bmp","car.bmp" };
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
	

	//������ ����������� ���������  (R G B)
	RGBTRIPLE* texarray;
	//������ ��������, (������*������*4      4, ���������   ����, �� ������� ������������ �� 4 ����� �� ������� �������� - R G B A)
	
	char* texCharArray;
	int texW, texH;
	OpenGL::LoadBMP("car.bmp", &texW, &texH, &texarray);
	OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);


	//���������� �� ��� ��������
	glGenTextures(1, &stankin);
	
	//������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	glBindTexture(GL_TEXTURE_2D, stankin);

	//��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);
	//glBindTexture(GL_TEXTURE_2D, 0);


	free(texCharArray);
	free(texarray);
	
	//OpenGL::LoadBMP("texture[0].bmp", &texW, &texH, &texarray);
	//OpenGL::RGBtoChar(texarray, texW, texH, &texCharArray);

	//
	//
	////���������� �� ��� ��������
	//glGenTextures(1, &texture[0]);
	////������ ��������, ��� ��� ����� ����������� � ���������, ����� ����������� �� ����� ��
	//glBindTexture(GL_TEXTURE_2D, texture[0]);

	////��������� �������� � �����������, � ���������� ��� ������  ��� �� �����
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texW, texH, 0, GL_RGBA, GL_UNSIGNED_BYTE, texCharArray);

	////�������� ������
	//free(texCharArray);
	//free(texarray);

	//������� ����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);


	//������ � ���� ����������� � "������"
	ogl->mainCamera = &camera;
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

	camera.fi1 = -1.3;
	camera.fi2 = 0.8;
}
std::vector<double> MultiplyMatrices(const std::vector<std::vector<double>>& matrix, const std::vector<double>& vector1) {
	int m = matrix.size();
	int n = matrix[0].size();

	// �������� ��������������� ������� ������������ ������
	std::vector<double> result(m, 0.0);

	// ������������ ������� �� ������
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			result[i] += matrix[i][j] * vector1[j];
		}
	}

	return result;
}
void Plane(std::vector<std::vector<double>> aaa, double height)
{
	for (int i = 0; i < 2; i++)
	{
		
		glBegin(GL_POLYGON);
		
		if (i == 0)
		{
			glColor3d(0, 0.5, 1);
			//glNormal3d(aaa[0][1] * aaa[1][2] - aaa[0][2] * aaa[1][1], aaa[0][2] * aaa[1][0] - aaa[0][0] * aaa[1][2], aaa[0][0] * aaa[1][1] - aaa[0][1] * aaa[1][0]);
			//glNormal3d(0, 0, -1);
		}
			
		else
		{
			glColor3d(1, 0.5, 0);
			//glNormal3d(aaa[0][1] * aaa[1][2] - aaa[0][2] * aaa[1][1], aaa[0][2] * aaa[1][0] - aaa[0][0] * aaa[1][2], aaa[0][0] * aaa[1][1] - aaa[0][1] * aaa[1][0]);
			//glNormal3d(0, 0, 1);

		}
		glColor3d(0, 0.5, 1);
		for (int z = 0; z < aaa.size(); z++)
		{
			std::vector<double> temp = MultiplyMatrices(matrixRotate, aaa[z]);
			//aaa[z] = temp;
			/*
			double temp1[] = {
				aaa[z][0] * cos(Angel) - aaa[z][2] * sin(Angel),
				aaa[z][1],
				aaa[z][0] * sin(Angel) + aaa[z][2] * cos(Angel),
			};*/
			glVertex3d(temp[0], temp[1], temp[2]);
		}
		for (int j = 0; j < aaa.size(); j++)
		{
			aaa[j][2] += height;
		}
		glEnd();
		

	}
	for (int j = 0; j < aaa.size(); j++)
	{
		aaa[j][2] -= height;
	}
}
//void Polygon(std::vector<double> v1, std::vector<double> v2, double height, double color[])
//{
//	glBegin(GL_POLYGON);
//	glColor3d(color[0], color[1], color[2]);
//	glNormal3d(v1[1] * v2[2] - v1[2] * v2[1], v1[2] * v2[0] - v1[0] * v2[2], v1[0] * v2[1] - v1[1] * v2[0]);
//	double temp1[] = {
//		v1[0] * cos(Angel + 90) - v1[1] * sin(Angel + 90),
//		v1[0] * sin(Angel + 90) + v1[1] * cos(Angel + 90),
//		v1[2]
//	};
//	double temp2[] = {
//		v2[0] * cos(Angel + P/2) - v2[1] * sin(Angel + P / 2),
//		v2[0] * sin(Angel + P / 2) + v2[1] * cos(Angel + P / 2),
//		v2[2]
//	};
//	glVertex3dv(temp1);
//	glVertex3dv(temp2);
//
//	v2[2] += height;
//	v1[2] += height;
//
//	glVertex3dv(temp2);
//	glVertex3dv(temp1);
//
//	v2[2] -= height;
//	v1[2] -= height;
//	glEnd();
//}

//void Bok(std::vector<std::vector<double>> aaa, double height)
//{
//	double color[] = { 1, 0, 0 };
//	for (int j = 0; j < aaa.size() - 1; j++)
//	{
//		color[0] = 1;
//		color[1] = 0;
//		color[2] = 0;
//		Polygon(aaa[j], aaa[j + 1], height, color);
//	}
//	color[0] = 1;
//	color[1] = 0;
//	color[2] = 0;
//	Polygon(aaa[aaa.size() - 1], aaa[0], height, color);
//}
double OnCicle(std::vector<double> v1, double x, double y)
{
	return pow(v1[0] - x, 2) + pow(v1[1] - y, 2);
}
void DrawPlane(std::vector<std::vector<double>> aaa, double height)
{
	Plane(aaa, height);
	//Bok(aaa, height);
}

void Render(OpenGL *ogl)
{
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

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

	//���� ���� �������, ��� ����������� (����������� ���������)
	glShadeModel(GL_SMOOTH);
	//===================================
	//������� ���  


	aaa1 = {

		{ 0, -5, z },
		{ 7, -1, z },
		{ 7, 1, z },
		{ 1,1, z },
		{1, 4, z},
		{-1, 4, z},
		{ -1, 1, z },
		{ -7, 1, z },
		{ -7, -1, z },
	};
	DrawPlane(aaa1, height);

	//������ ��������� ���������� ��������
	//double A[2] = { -4, -4 };
	//double B[2] = { 4, -4 };
	//double C[2] = { 4, 4 };
	//double D[2] = { -4, 4 };

	//glBindTexture(GL_TEXTURE_2D, texture[0]);

	//glColor3d(0.6, 0.6, 0.6);
	//glBegin(GL_QUADS);

	//glNormal3d(0, 0, 1);
	//glTexCoord2d(0, 0);
	//glVertex2dv(A);
	//glTexCoord2d(1, 0);
	//glVertex2dv(B);
	//glTexCoord2d(1, 1);
	//glVertex2dv(C);
	//glTexCoord2d(0, 1);
	//glVertex2dv(D);

	//glEnd();
	//����� ��������� ���������� ��������


   //��������� ������ ������

	
	glMatrixMode(GL_PROJECTION);	//������ �������� ������� ��������. 
	                                //(���� ��������� ��������, ����� �� ������������.)
	glPushMatrix();   //��������� ������� ������� ������������� (������� ��������� ������������� ��������) � ���� 				    
	glLoadIdentity();	  //��������� ��������� �������
	glOrtho(0, ogl->getWidth(), 0, ogl->getHeight(), 0, 1);	 //������� ����� ������������� ��������

	glMatrixMode(GL_MODELVIEW);		//������������� �� �����-��� �������
	glPushMatrix();			  //��������� ������� ������� � ���� (��������� ������, ����������)
	glLoadIdentity();		  //���������� �� � ������

	glDisable(GL_LIGHTING);



	GuiTextRectangle rec;		   //������� ����� ��������� ��� ������� ������ � �������� ������.
	rec.setSize(300, 150);
	rec.setPosition(10, ogl->getHeight() - 150 - 10);


	std::stringstream ss;
	ss << "T - ���/���� �������" << std::endl;
	ss << "L - ���/���� ���������" << std::endl;
	ss << "F - ���� �� ������" << std::endl;
	ss << "G - ������� ���� �� �����������" << std::endl;
	ss << "G+��� ������� ���� �� ���������" << std::endl;
	ss << "�����. �����: (" << light.pos.X() << ", " << light.pos.Y() << ", " << light.pos.Z() << ")" << std::endl;
	ss << "�����. ������: (" << camera.pos.X() << ", " << camera.pos.Y() << ", " << camera.pos.Z() << ")" << std::endl;
	ss << "��������� ������: R="  << camera.camDist << ", fi1=" << camera.fi1 << ", fi2=" << camera.fi2 << std::endl;
	
	rec.setText(ss.str().c_str());
	rec.Draw();

	glMatrixMode(GL_PROJECTION);	  //��������������� ������� �������� � �����-��� �������� �� �����.
	glPopMatrix();


	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}