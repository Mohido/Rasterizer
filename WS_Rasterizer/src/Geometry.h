#pragma once
#ifndef GEOMETRY
#define GEOMETRY

#include "renderer.h"
#include <vector>
#include <string>

namespace geometry {





/*========================================== Matrices Area ====================================*/

	template<class T>
	struct Matrix_3x3 {
		T m_matrix[3][3];

		Matrix_3x3( const T& a_00, const T& a_01, const T& a_02,
					const T& a_10, const T& a_11, const T& a_12,
					const T& a_20, const T& a_21, const T& a_22 )
		{
			m_matrix[0][0] = a_00; m_matrix[0][1] = a_01; m_matrix[0][2] = a_02;
			m_matrix[1][0] = a_10; m_matrix[1][1] = a_11; m_matrix[1][2] = a_12;
			m_matrix[2][0] = a_20; m_matrix[2][1] = a_21; m_matrix[2][2] = a_22;
		}
	};



	/*Note that we use Row major for dealing with matrices here.*/
	template<class T>
	struct Matrix_4x4 {
		T m_matrix[4][4];

		Matrix_4x4( const T& a_00, const T& a_01, const T& a_02, const T& a_03,
					const T& a_10, const T& a_11, const T& a_12, const T& a_13,
					const T& a_20, const T& a_21, const T& a_22, const T& a_23,
					const T& a_30, const T& a_31, const T& a_32, const T& a_33)
		{
			m_matrix[0][0] = a_00; m_matrix[0][1] = a_01; m_matrix[0][2] = a_02; m_matrix[0][3] = a_03;
			m_matrix[1][0] = a_10; m_matrix[1][1] = a_11; m_matrix[1][2] = a_12; m_matrix[1][3] = a_13;
			m_matrix[2][0] = a_20; m_matrix[2][1] = a_21; m_matrix[2][2] = a_22; m_matrix[2][3] = a_23;
			m_matrix[3][0] = a_30; m_matrix[3][1] = a_31; m_matrix[3][2] = a_32; m_matrix[3][3] = a_33;
		}
		/* Creates an identity matrix */
		Matrix_4x4() {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_matrix[i][j] = (i == j) ? 1 : 0;
				}
			}
		}
		/* Creates a matrix from another one*/
		Matrix_4x4(const Matrix_4x4<T>& src) {
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					m_matrix[i][j] = src.m_matrix[i][j];
				}
			}
		}



		Matrix_4x4<T> inverse(); /*A function that gives back the inverse of a Matrix by using partial pivoting*/
	
		std::string toString() {
			std::string s = std::to_string(this->m_matrix[0][0]) + " " + std::to_string(this->m_matrix[0][1]) + " " + std::to_string(this->m_matrix[0][2]) + " " + std::to_string(this->m_matrix[0][3]) + "\n";
			s += std::to_string(this->m_matrix[1][0]) + " " + std::to_string(this->m_matrix[1][1]) + " " + std::to_string(this->m_matrix[1][2]) + " " + std::to_string(this->m_matrix[1][3]) + "\n";
			s += std::to_string(this->m_matrix[2][0]) + " " + std::to_string(this->m_matrix[2][1]) + " " + std::to_string(this->m_matrix[2][2]) + " " + std::to_string(this->m_matrix[2][3]) + "\n";
			s += std::to_string(this->m_matrix[3][0]) + " " + std::to_string(this->m_matrix[3][1]) + " " + std::to_string(this->m_matrix[3][2]) + " " + std::to_string(this->m_matrix[3][3]) + "\n";
			return s;
		}
		
		Matrix_4x4<T> operator*(const Matrix_4x4<T>& M);/*Multiplyting 2 matrices*/

	};





/*====================================== 2D Representation ====================================== */


	template<class T>
	struct Vector_2D {
		T x, y;
		Vector_2D(T x, T y) : x(x), y(y) {}
		Vector_2D() : x(0), y(0) {}

		/*
		* Cross Product Function for 2 2d vectors. Returns the scaler of the Z axis.
		*/
		static T crossProduct(Vector_2D<T> v1, Vector_2D<T> v2) {
			return v1.x * v2.y - v1.y * v2.x;		 // determinant (Length of the new axis)
		}
		T crossProduct(Vector_2D<T> v2) {
			return this->x * v2.y - this->y * v2.x;		 // determinant (Length of the new axis)
		}

		T lengthSqr() const { return this->x * this->x + this->y * this->y; }
		/* Minus overriding*/
		Vector_2D<T> operator-(Vector_2D<T> v2) { return Vector_2D<T>(this->x - v2.x, this->y - v2.y); }

	};


	template<class T>
	class Triangle_2D {
		Vector_2D<T> vertex_1, vertex_2, vertex_3;
		float area;
	public:
		Triangle_2D(Vector_2D<T> v1, Vector_2D<T> v2, Vector_2D<T> v3)
			: vertex_1(v1), vertex_2(v2), vertex_3(v3)
		{
			// according to the common sense, the area of a triangle is the 1/2 * parallalgram area that represents the triangle
			// So from 2 sides we can get the area of the triangle.
			Vector_2D<T> side1 = v2 - v1;
			Vector_2D<T> side2 = v3 - v1;
			// Their detirmenant is the area of the parallelgram, (CrossProduct length)
			this->area = std::abs(side2.crossProduct(side1)) / 2;
		}

		Triangle_2D() {
			this->vertex_1 = { 0 };
			this->vertex_2 = { 0 };
			this->vertex_3 = { 0 };
			this->area = 0;
		}

		/*Checks if the given point is inside the triangle or not by using the edge function*/
		bool pointInside(Vector_2D<T> point) {
			return (vertex_2 - vertex_1).crossProduct(point - vertex_1) >= 0
				&& (vertex_3 - vertex_2).crossProduct((point - vertex_2)) >= 0
				&& (vertex_1 - vertex_3).crossProduct((point - vertex_3)) >= 0;
		}

		float getArea() { return this->area; }
		std::vector< Vector_2D<T> > getVertices() { return { vertex_1, vertex_2, vertex_3 }; }
	};




/*======================================= 3D Representations =======================================*/


	template<class T>
	struct Vector_3D {
		T x, y, z;
		Vector_3D(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector_3D() { this->x = 0; this->y = 0; this->z = 0; }
		T lengthSqr() const { return this->x * this->x + this->y * this->y + this->z * this->z; }
		std::string toString() { return "[" + std::to_string(this->x) + "," + std::to_string(this->y) + "," + std::to_string(this->z) + "]\n";}


		Vector_3D<T> operator*(const Matrix_4x4<T>& M) {
			T x_ = this->x * M.m_matrix[0][0] + this->y * M.m_matrix[1][0] + this->z * M.m_matrix[2][0] + M.m_matrix[3][0];
			T y_ = this->x * M.m_matrix[0][1] + this->y * M.m_matrix[1][1] + this->z * M.m_matrix[2][1] + M.m_matrix[3][1];
			T z_ = this->x * M.m_matrix[0][2] + this->y * M.m_matrix[1][2] + this->z * M.m_matrix[2][2] + M.m_matrix[3][2];
			T w_ = this->x * M.m_matrix[0][3] + this->y * M.m_matrix[1][3] + this->z * M.m_matrix[2][3] + M.m_matrix[3][3];
			if (w_ == 0) return Vector_3D<T>(); // returns the 0 vector if division by 0 accured
			return Vector_3D<T>(x_ / w_, y_ / w_, z_ / w_);
		}
		Vector_3D<T> operator-(Vector_3D<T> v2) { return Vector_3D<T>(this->x - v2.x, this->y - v2.y, this->z - v2.z); }
	};


	template<class T>
	struct Vertex_3D {
		renderer::Color col;
		Vector_3D<T> position;
		Vertex_3D(Vector_3D<T> pos = Vector_3D<T>(), renderer::Color c = renderer::Color())
			: position(pos), col(c)
		{}
	};



	template<class T>
	class Triangle_3D {
		Vertex_3D<T> vertex_1, vertex_2, vertex_3;
		Matrix_4x4<float> transformation; // transformation matrix
		float area;
	public:
		Triangle_3D(Vector_3D<T> v1, Vector_3D<T> v2, Vector_3D<T> v3)
			: vertex_1(v1), vertex_2(v2), vertex_3(v3), area(0) // area needs to be implemented with CrossProduct of vectors
		{}

		Triangle_3D(Vertex_3D<T> v1, Vertex_3D<T> v2, Vertex_3D<T> v3)
			: vertex_1(v1), vertex_2(v2), vertex_3(v3), area(0) // area needs to be implemented with crossproduct of vectors
		{}

		Triangle_3D() 
		{
			this->vertex_1 = Vertex_3D<T>();
			this->vertex_2 = Vertex_3D<T>();
			this->vertex_3 = Vertex_3D<T>();
		}

		/*Getters Area*/
		float getArea() { return this->area; }
		std::vector< Vertex_3D<T> > getVertices() { return { vertex_1, vertex_2, vertex_3 };}
	};



//=============================== Geometry general functions ==========================================
	template <class T>
	extern Vector_3D<T> mul_vectorMatrix(const Vector_3D<T>& vec, const Matrix_4x4<T>& M) {
		T x = vec.x * M.m_matrix[0][0] + vec.y * M.m_matrix[1][0] + vec.z * M.m_matrix[2][0] + M.m_matrix[3][0];
		T y = vec.x * M.m_matrix[0][1] + vec.y * M.m_matrix[1][1] + vec.z * M.m_matrix[2][1] + M.m_matrix[3][1];
		T z = vec.x * M.m_matrix[0][2] + vec.y * M.m_matrix[1][2] + vec.z * M.m_matrix[2][2] + M.m_matrix[3][2];
		T w = vec.x * M.m_matrix[0][3] + vec.y * M.m_matrix[1][3] + vec.z * M.m_matrix[2][3] + M.m_matrix[3][3];

		if (w == 0) return Vector_3D<T>(); // returns the 0 vector if division by 0 accured
		return Vector_3D<T>(x / w, y / w, z / w);
	}
}

#endif // !Geometry

