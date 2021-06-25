//#define DEBUG // Note: during debuging mode don't compile the whole project... Only compile the specific file
#include "geometry.h"

#ifdef DEBUG
	#include <iostream>
#else
	#pragma once
#endif // DEBUG





namespace geometry {

	/// <summary>
	/// Inverses the matrix by using Partial Pivoting gaussian elimination.
	/// </summary>
	/// <returns>The Inversed matrix</returns>
	template<class T>
	Matrix_4x4<T> Matrix_4x4<T>::inverse() {
		T result[4][4];
		T original[4][4];

		// initializing the variables.
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				original[i][j] = this->m_matrix[i][j]; // Copy of current matrix
				result[i][j] = (i == j) ? 1 : 0; // Identity matrix
			}
		}

		// Partial Pivoting guassian algorithm
		for (int row = 0; row < 4; row++) {

			// Chosing a pivot
			T p = (original[row][row] < 0) ? -original[row][row] : original[row][row]; // absolute value
			int p_row = row;
			for (int i = row + 1; i < 4; i++) {
				T temp = (original[i][row] < 0) ? -original[i][row] : original[i][row];
				if (temp > p) {
					p = temp;
					p_row = i;
				}
			}


			if (original[p_row][row] == 0) return Matrix_4x4<T>(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

			// Swap the row of the pivot with the current row
			for (int col = 0; col < 4; col++) {
				T temp = original[row][col]; // swaping main matrix
				original[row][col] = original[p_row][col];
				original[p_row][col] = temp;

				temp = result[row][col]; // swaping result
				result[row][col] = result[p_row][col];
				result[p_row][col] = temp;
			}

			// elimination process of below rows, Note that no need for pivot .. our current row is the pivot
			for (int i = row + 1; i < 4; i++) {
				T elimenator = original[i][row] / original[row][row];

				for (int col = 0; col < 4; col++) {
					original[i][col] -= (elimenator * original[row][col]);
					result[i][col] -= (elimenator * result[row][col]);
				}
			}
		}

		// Backward substitution
		for (int row = 3; row >= 0; row--) {
			T x = original[row][row];

			if (x == 0) return Matrix_4x4<T>(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);	// Cannot invert singular matrix

			// deviding the whole row by x
			for (int col = 0; col < 4; col++) {
				original[row][col] /= x;
				result[row][col] /= x;
			}

			// Elimenating the x from the upper rows
			for (int row_b = 0; row_b < row; row_b++) {
				x = original[row_b][row]; // the avlue of x from the upper row

				for (int col = 0; col < 4; col++) {
					original[row_b][col] -= x * original[row][col]; // upper row elements - the current row elem * current row x
					result[row_b][col] -= x * result[row][col];
				}
			}	
		}

		return Matrix_4x4<T>(result[0][0], result[0][1], result[0][2], result[0][3],
			result[1][0], result[1][1], result[1][2], result[1][3],
			result[2][0], result[2][1], result[2][2], result[2][3],
			result[3][0], result[3][1], result[3][2], result[3][3]);
	}



	template <class T>
	std::string Matrix_4x4<T>::toString() {
		std::string s = std::to_string(this->m_matrix[0][0]) + " " + std::to_string(this->m_matrix[0][1]) + " " + std::to_string(this->m_matrix[0][2]) + " " + std::to_string(this->m_matrix[0][3]) + "\n";
		s += std::to_string(this->m_matrix[1][0]) + " " + std::to_string(this->m_matrix[1][1]) + " " + std::to_string(this->m_matrix[1][2]) + " " + std::to_string(this->m_matrix[1][3]) + "\n";
		s += std::to_string(this->m_matrix[2][0]) + " " + std::to_string(this->m_matrix[2][1]) + " " + std::to_string(this->m_matrix[2][2]) + " " + std::to_string(this->m_matrix[2][3]) + "\n";
		s += std::to_string(this->m_matrix[3][0]) + " " + std::to_string(this->m_matrix[3][1]) + " " + std::to_string(this->m_matrix[3][2]) + " " + std::to_string(this->m_matrix[3][3]) + "\n";
		return s;
	}

}


#ifdef DEBUG

	int main() {
		std::cout << "Debuging Mode" << std::endl;
		geometry::Matrix_4x4<float> v(3,4, 1, 2, 5,8,9,6, 7, 10,11,12,13,14,15,16);
		std::cout << v.toString() << std::endl;
		
		geometry::Matrix_4x4<float> inv = v.inverse();
		std::cout << " inversed: " << std::endl;
		std::cout << inv.toString() << std::endl;

		return 0;
	}


#endif // Debug mode
