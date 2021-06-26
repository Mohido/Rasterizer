#pragma once

#ifndef CAMERA
#define CAMERA

#include "geometry.h"


namespace cameras {

	enum FillingMode {
		FILL, OVERSCAN
	};


	/*A simple Pihnole camera*/
	class PinholeCamera {
	private: // used the m_variableName style for this particular class just to break the routine
		float m_focalLength, m_filmWidth, m_filmHeight, m_nearClipping, m_displayWidth,m_displayHeight;
		float m_filmAspectRatio, m_displayAspectRatio;
		geometry::Vector_3D<float> m_origin;
		geometry::Matrix_4x4<float> m_transformation; // camera to world

	public:
		//TODO: constructor with focal length and film size, and another with angle of view
		PinholeCamera(const geometry::Vector_3D<float>& origin, const geometry::Matrix_4x4<float>& transformation,
						const float& focalLength, const float& filmWidth, const float& filmHeight,
						const float& nearClipping,
						const int& displayWidth, const int& displayHeight)
		: m_origin(origin), m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight) 
				, m_nearClipping(nearClipping), m_displayWidth(displayWidth), m_displayHeight(displayHeight) 
		{
			this->m_filmAspectRatio = filmWidth / filmHeight;
			this->m_displayAspectRatio = displayWidth / (float)displayHeight;
		}

		PinholeCamera(const geometry::Vector_3D<float>& origin,
			const float& focalLength, const float& filmWidth, const float& filmHeight,
			const float& nearClipping,
			const int& displayWidth, const int& displayHeight)
		: m_origin(origin), m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight)
				, m_nearClipping(nearClipping), m_displayWidth(displayWidth), m_displayHeight(displayHeight)
		{
			this->m_transformation = geometry::Matrix_4x4<float>();
			this->m_filmAspectRatio = filmWidth / filmHeight;
			this->m_displayAspectRatio = displayWidth / (float)displayHeight;
		}

	};

}
#endif // !CAMERA

