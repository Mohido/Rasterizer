#pragma once

namespace Geometry {

	class Matrix3 {

	};

	class Matrix4 {

	};

	template<class T>
	class Vector2 {
		T x,y;
		Vector2(T x, T y) : x(x), y(y) {}
	};
	template<class T>
	class Vector3 {
		T x;
		T y;
		T z;
		Vector3(T x, T y, T z) : x(x), y(y), z(z) {}
	};
}
