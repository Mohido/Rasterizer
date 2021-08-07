#pragma once

#ifndef CAMERA
#define CAMERA

#include "geometry.h"


namespace cameras {


	/*	Fill: Putting the camera image outside the screen image and scale the camera image.
		Overscan: Putting the Screen image outside the Camera image and scale the camera image.*/
	enum FillingMode {
		FILL, OVERSCAN
	};


	/*A simple Pihnole camera*/
	class PinholeCamera {
	private: // used the m_variableName style for this particular class just to break the routine
		float m_focalLength, m_filmWidth, m_filmHeight, m_nearClipping, m_displayWidth, m_displayHeight;
		float m_filmAspectRatio, m_displayAspectRatio;
		geometry::Vector_3D<float> m_origin;
		geometry::Matrix_4x4<float> m_transformation; // camera to world
		float m_tan_w, m_tan_h;  // describing the tan values of the angle
		float m_left, m_right, m_top, m_bottom;
		FillingMode m_fillType;




	public:
		//TODO: constructor with focal length and film size, and another with angle of view
		PinholeCamera(const geometry::Vector_3D<float>& origin, const geometry::Matrix_4x4<float>& transformation,
						const float& focalLength, const float& filmWidth, const float& filmHeight,
						const float& nearClipping,
						const int& displayWidth, const int& displayHeight,
						const FillingMode& fillType = FILL)
		: m_origin(origin), m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight) 
				, m_nearClipping(nearClipping), m_displayWidth(displayWidth), m_displayHeight(displayHeight), m_fillType(fillType)
		{
			this->m_tan_w = (filmWidth / 2) / focalLength;
			this->m_tan_h = (filmHeight / 2) / focalLength;
			this->m_filmAspectRatio = filmWidth / filmHeight;
			this->m_displayAspectRatio = displayWidth / (float)displayHeight;

			this->m_top = nearClipping * this->m_tan_h;
			this->m_right = this->m_top * this->m_filmAspectRatio;

			float xscale = 1, yscale = 1;
			///TODO:: Check for getting the scale from the width and height directly
			switch (this->m_fillType) {
			case FILL:
			{
				if (m_filmAspectRatio > m_displayAspectRatio) // film is wider than screen
					xscale = m_displayAspectRatio / m_filmAspectRatio;
				else
					yscale = m_filmAspectRatio / m_displayAspectRatio;
				break;
			}
			case OVERSCAN:
			{
				if (m_filmAspectRatio > m_displayAspectRatio) // film is wider than screen
					yscale = m_filmAspectRatio / m_displayAspectRatio;
				else
					xscale = m_displayAspectRatio / m_filmAspectRatio;
				break;
			}
			default:
				break;
			}
			this->m_right *= xscale;
			this->m_top *= yscale;
			this->m_left = -this->m_right;
			this->m_bottom = -this->m_top;
		}



		PinholeCamera(const geometry::Vector_3D<float>& origin,
			const float& focalLength, const float& filmWidth, const float& filmHeight,
			const float& nearClipping,
			const int& displayWidth, const int& displayHeight,
			const FillingMode& fillType = FILL)
		: m_origin(origin), m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight)
				, m_nearClipping(nearClipping), m_displayWidth(displayWidth), m_displayHeight(displayHeight), m_fillType(fillType)
		{
			this->m_tan_w = (filmWidth / 2) / focalLength;
			this->m_tan_h = (filmHeight / 2) / focalLength;
			this->m_transformation = geometry::Matrix_4x4<float>();
			this->m_filmAspectRatio = filmWidth / filmHeight;
			this->m_displayAspectRatio = displayWidth / (float)displayHeight;

			this->m_top		= nearClipping	* this->m_tan_h;
			this->m_right	= this->m_top	* this->m_filmAspectRatio;

			float xscale = 1, yscale = 1;
			///TODO:: Check for getting the scale from the width and height directly
			switch (this->m_fillType) {
			case FILL:
			{
				if(m_filmAspectRatio > m_displayAspectRatio) // film is wider than screen
					xscale = m_displayAspectRatio / m_filmAspectRatio;
				else
					yscale = m_filmAspectRatio / m_displayAspectRatio;
				break;
			}
			case OVERSCAN:
			{
				if (m_filmAspectRatio > m_displayAspectRatio) // film is wider than screen
					yscale = m_filmAspectRatio / m_displayAspectRatio;
				else
					xscale = m_displayAspectRatio / m_filmAspectRatio;
				break;
			}
			default:
				break;
			}
			this->m_right *= xscale;
			this->m_top *= yscale;
			this->m_left = -this->m_right;
			this->m_bottom = -this->m_top;
		}

		


		/*
		* TODO:::
		*	Takes a triangle from the world!
			Returns a trianble which describes the projected vertices final destination on the screen. 
			And the 3d Demension is the deminsion of the z_depth of the vertices
		*/
		geometry::Triangle_3D<float> projectToCamera(geometry::Triangle_3D<float> tri){
			//TODO:: Transform the paoint to camera

			//TODO:: Project the point to camera

			//TODO:: Transform the projected points to the NDC values.

			//TODO:: Change the NDC point to the screen space

			//TODO:: Return the screen vertices of the projected Triange
		}
		


	};

}
#endif // !CAMERA

