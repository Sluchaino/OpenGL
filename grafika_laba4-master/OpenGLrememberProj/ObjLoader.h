#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <windows.h>
#include <GL/GL.h>
#include <list>
#include <vector>
#include <random>
struct ObjVertex
{
	double x, y, z, w;

	ObjVertex():x(0), y(0), z(0), w(1)
	{		
	}

	inline double * _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};

struct ObjTexCord
{
	double u, v, w;
	
	ObjTexCord():u(0),v(0),w(0)
	{
	}
	inline double * _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};
struct ObjNormal
{
	double x, y, z;
	inline double * _ptr()
	{
		return reinterpret_cast<double*>(this);
	}
};



struct ObjFace
{
	unsigned int VertexCount;

	std::vector<ObjVertex> vertex;
	std::vector<ObjTexCord> texCoord;
	std::vector<ObjNormal> normal;
};


struct ObjFile
{
	double Now,Move, nowPos, maxPos;
	double Limit;
	int col;
	double maxX;
	double maxZ;
	double minX;
	double minZ;
	ObjFile()
	{

	}
	ObjFile(double maxPos, double move)
	{
		Limit = maxPos;
		Move = move;
		Now = 0;
		nowPos = 0;
		col = 0;
	}
	ObjFile(double limit, double MaxX, double MinX, double MaxZ, double MinZ, double move)
	{
		col = 0;
		Now = 0;
		nowPos = 0;
		Limit = limit;
		maxX = MaxX;
		minX = MinX;
		maxZ = MaxZ;
		minZ = MinZ;
		Move = move;
	}
	ObjFile(double limit, double MaxX, double MinX, double MaxZ, double MinZ, double move, double MaxPos)
	{
		col = 0;
		Now = 0;
		nowPos = 0;
		Limit = limit;
		maxX = MaxX;
		minX = MinX;
		maxZ = MaxZ;
		minZ = MinZ;
		Move = move;
		maxPos = MaxPos;
	}
	~ObjFile()
	{
		glDeleteLists(listId, 1);
	}
	std::list<ObjFace> Faces;
	int listId = -1;
	inline void Car1()
	{
		std::random_device rd;
		std::mt19937 gen(rd());

		// Определяем диапазон случайных чисел
		int min_value = -Limit;
		int max_value = Limit;
		std::uniform_int_distribution<> distribution(min_value, max_value);

		// Генерируем случайное число
		double random_number = distribution(gen);
		Now = random_number;
		maxZ -= Now;
		minZ -= Now;
	}
	inline void RenderModel(int mode)
	{
		double temp;
		double temp1;
		for (std::list<ObjFace>::iterator it = Faces.begin(); it != Faces.end(); ++it)
		{
			glBegin(mode);
			bool f_n = (it->normal.size() != 0);
			bool f_t = (it->texCoord.size() != 0);
			auto it_n = it->normal.begin();
			auto it_t = it->texCoord.begin();

			auto j = it->vertex.begin();
			auto j1 = it->vertex.end();

			for (; j != it->vertex.end(); ++j)
			{

				if (f_n)
				{
					glNormal3dv((it_n++)->_ptr());

				}

				if (f_t)
				{
					glTexCoord2dv((it_t++)->_ptr());
				}
				temp = j->z + Now;
				temp1 = j->x + 4 + nowPos;
				glVertex3d(temp1, j->y , temp);
			}
			glEnd();
		}
	}
	inline void DrawObj(GLenum mode = 0x0009) //GL_POLYGON
	{
		glCallList(listId);
	}
	inline void MoveRight()
	{
		if (Now < Limit)
		{
			Now += Move;
			maxZ -= Move;
			minZ -= Move;
		}
		if (Now >= Limit)
			Now = Limit;
	}
	inline void MoveLeft()
	{
		if (Now > -Limit)
		{
			Now -= Move;
			maxZ += Move;
			minZ += Move;
		}
		if (Now <= -Limit)
			Now = -Limit;
	}
	inline bool MoveF()
	{
		nowPos += Move;
		maxX += Move;
		minX += Move;

		if (nowPos >= maxPos)
		{
			maxX -= nowPos;
			minX -= nowPos;
			maxZ += Now;
			minZ += Now;
			Car1();
			nowPos = 0;
			++col;
			if (col % 3 == 0 && col <= 27)
				Move += 0.1;
			return true;
		}
		return false;
	}
	inline void MovePlane()
	{
		nowPos += Move;
		if (nowPos >= Limit)
		{
			nowPos = 0;
			++col;
			if (col % 2 == 0)
				Move += 0.4;
		}
	}
	inline bool Stolk(ObjFile car)
	{
		if ((car.maxX <= maxX && car.maxX>= minX || car.minX <= maxX && car.minX >= minX) && (maxZ >= car.maxZ  && minZ <= car.maxZ || maxZ >= car.minZ && minZ <= car.minZ ||  maxZ >= car.minZ && minZ >= car.minZ && maxZ <= car.maxZ && minZ <= car.maxZ))
		{
			return true;
		}
		return false;
	}
	//ObjFile(const ObjFile)
	
};

int loadModel(char *filename, ObjFile *file);

#endif