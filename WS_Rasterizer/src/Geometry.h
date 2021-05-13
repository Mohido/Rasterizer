#pragma once

namespace geometry {

	class Matrix3 {

	};

	class Matrix4 {

	};

	template<class T>
	class Vector2 {
		
	public:
		T x, y;
		Vector2(T x, T y) : x(x), y(y) {}
	};
	template<class T>
	class Vector3 {
		
	public:
		T x;
		T y;
		T z;
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vector3() { this->x = 0; this->y = 0; this->z = 0;}
	};
}
