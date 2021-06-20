#pragma once
#ifndef Geometry
#define Geometry
#include <vector>


namespace geometry {

	// Mathematics Geometrical representations.
	template<class T>
	class Matrix_3x3 {};
	template<class T>
	class Matrix_4x4 {};


	template<class T>
	struct Vector_2D {
		T x, y;
		Vector_2D(T x, T y) : x(x), y(y) {}
		Vector_2D() : x(0), y(0) {}

		/*
		* Cross Product Function for 2 2d vectors. Returns the scaler of the Z axis.
		*/
		static T crossProduct (Vector_2D<T> v1, Vector_2D<T> v2) {
			return v1.x * v2.y - v1.y * v2.x;		 // determinant (Length of the new axis)
		}
		T crossProduct(Vector_2D<T> v2) {
			return this->x * v2.y - this->y * v2.x;		 // determinant (Length of the new axis)
		}

		T lengthSqr() const { return this->x * this->x + this->y * this->y; }
		/* Minus overriding*/
		Vector_2D<T> operator-(Vector_2D<T> v2) {	return Vector_2D<T>(this->x - v2.x, this->y - v2.y); }

	};

	template<class T>
	struct Vector_3D {
		T x, y, z;
		Vector_3D(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector_3D() { this->x = 0; this->y = 0; this->z = 0; }
		T lengthSqr() const { return this->x*this->x + this->y*this->y + this->z*this->z; }
		Vector_3D<T> operator-(Vector_3D<T> v2) {	 return Vector_3D<T>(this->x - v2.x, this->y - v2.y, this->z - v2.z); }
	};



	// Geometric Shapes - Polygons Area
	template<class T>
	class Triangle_3D {
		Vector_3D<T> vertex_1, vertex_2, vertex_3;
		float area;
	public:
		Triangle_3D(Vector_3D<T> v1, Vector_3D<T> v2, Vector_3D<T> v3)
			: vertex_1(v1), vertex_2(v2), vertex_3(v3)
		{
			// This area needs to be implemented when matrix is implemented and the determinant of matrix is implemented
			// Or the crossProduct of the Vector_3D is implemented. we can simply do as we did in teh Triangle 2d
			this->area = 0;
		}

		Triangle_3D() {
			this->vertex_1 = { 0 };
			this->vertex_2 = { 0 };
			this->vertex_3 = { 0 };
		}

		/*Getters Area*/
		float getArea() { return this->area; }
		std::vector< Vector_3D<T> > getVertices() { return { vertex_1, vertex_2, vertex_3 };}
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





}

#endif // !Geometry

