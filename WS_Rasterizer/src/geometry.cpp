//#define DEBUG // Note: during debuging mode don't compile the whole project... Only compile the specific file
#include "geometry.h"

#ifdef DEBUG
	#include <iostream>
#else
	#pragma once
#endif // DEBUG





namespace geometry {

	/*
	

	template<class T>
	Matrix_4x4<T> Matrix_4x4<T>::operator*(const Matrix_4x4<T>& M) {
		T s[4][4];
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				s[i][j] = this->m_matrix[i][0] * M.m_matrix[0][j] +
						  this->m_matrix[i][1] * M.m_matrix[1][j] +
						  this->m_matrix[i][2] * M.m_matrix[2][j] +
						  this->m_matrix[i][3] * M.m_matrix[3][j];
			}
		}

		return Matrix_4x4<T>( s[0][0], s[0][1], s[0][2], s[0][3],
							  s[1][0], s[1][1], s[1][2], s[1][3],
							  s[2][0], s[2][1], s[2][2], s[2][3],
							  s[3][0], s[3][1], s[3][2], s[3][3]);
	}
		*/
//=============================== Vector_3D funciton definitions ======================================
	//Kept showing me unresolved linking error so I moved it to the inside the struct definition
	//template <class T>
	//Vector_3D<T> Vector_3D<T>::operator*(const Matrix_4x4<T>& M) {
	//	T x_ = this->x * M.m_matrix[0][0] + this->y * M.m_matrix[1][0] + this->z * M.m_matrix[2][0] + M.m_matrix[3][0];
	//	T y_ = this->x * M.m_matrix[0][1] + this->y * M.m_matrix[1][1] + this->z * M.m_matrix[2][1] + M.m_matrix[3][1];
	//	T z_ = this->x * M.m_matrix[0][2] + this->y * M.m_matrix[1][2] + this->z * M.m_matrix[2][2] + M.m_matrix[3][2];
	//	T w_ = this->x * M.m_matrix[0][3] + this->y * M.m_matrix[1][3] + this->z * M.m_matrix[2][3] + M.m_matrix[3][3];

	//	if (w_ == 0) return Vector_3D<T>(); // returns the 0 vector if division by 0 accured
	//	return Vector_3D<T>(x_ / w_, y_ / w_, z_ / w_);
	//}



}


#ifdef DEBUG
	int main() {
		std::cout << "Debuging Mode" << std::endl;
		geometry::Matrix_4x4<double> v(3,4, 1, 2, 5,8,9,6, 7, 10,11,12,13,14,15,16);
		std::cout << v.toString() << std::endl;
		
		geometry::Matrix_4x4<double> inv = v.inverse();
		std::cout << " inversed: " << std::endl;
		std::cout << inv.toString() << std::endl;


		geometry::Matrix_4x4<double> ind = v * inv;
		std::cout << "Multiplication result: " << std::endl;
		std::cout << ind.toString() << std::endl;
		return 0;
	}


#endif // Debug mode
