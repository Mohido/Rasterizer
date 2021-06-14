#pragma once
#ifndef Geometry
#define Geometry


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


		/*
		* Cross Product Function for 2 2d vectors. Returns the scaler of the Z axis.
		*/
		static T crossProduct (Vector_2D<T> v1, Vector_2D<T> v2) {
			return v1.x * v2.y - v1.y * v2.x;		 // determinant (Length of the new axis)
		}
		T crossProduct(Vector_2D<T> v2) {
			return this->x * v2.y - this->y * v2.x;		 // determinant (Length of the new axis)
		}

		/*
		* Minus overriding
		*/
		Vector_2D<T> operator-(Vector_2D<T> v2) {	return Vector_2D<T>(this->x - v2.x, this->y - v2.y); }

	};

	template<class T>
	struct Vector_3D {
		T x, y, z;
		Vector_3D(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector_3D() { this->x = 0; this->y = 0; this->z = 0; }
		T lengthSqr() const { return this->x*this->x + this->y*this->y + this->z*this->z; }
	};



	// Geometric Shapes - Polygons Area
	template<class T>
	struct Triangle_3D {
		Vector_3D<T> vertex_1, vertex_2, vertex_3;
		Triangle_3D(Vector_3D<T> v1, Vector_3D<T> v2, Vector_3D<T> v3)
			: vertex_1(v1), vertex_2(v2), vertex_3(v3)
		{}

		Triangle_3D() {
			this->vertex_1 = { 0 };
			this->vertex_2 = { 0 };
			this->vertex_3 = { 0 };
		}
	};

}

#endif // !Geometry

