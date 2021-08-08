#pragma once

#ifndef CAMERA
#define CAMERA

#include "geometry.h"
#include <exception>

namespace cameras {


	/*	Fill: Putting the camera image outside the screen image and scale the camera image.
		Overscan: Putting the Screen image outside the Camera image and scale the camera image.*/
	enum FillingMode {
		FILL, OVERSCAN
	};


	/*A simple Pihnole camera*/
	class PinholeCamera {
	private: // used the m_variableName style for this particular class just to break the routine
		float m_focalLength, m_filmWidth, m_filmHeight, m_nearClipping, m_displayWidth, m_displayHeight; // style >> m_display_w or m_displayWidth
		float m_filmAspectRatio, m_displayAspectRatio;
		geometry::Vector_3D<float> m_origin;
		geometry::Matrix_4x4<float> m_transformation; // camera to world
		float m_tan_w, m_tan_h;  // describing the tan values of the angle
		float m_left, m_right, m_top, m_bottom;
		FillingMode m_fillType;




	public:
		//TODO: constructor with focal length and film size, and another with angle of view
		PinholeCamera(const geometry::Matrix_4x4<float> transformation,
						const float& focalLength, const float& filmWidth, const float& filmHeight,
						const float& nearClipping,
						const int& displayWidth, const int& displayHeight,
						const FillingMode& fillType = FILL)
		: m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight) , m_transformation(transformation),
				 m_nearClipping(nearClipping), m_displayWidth(displayWidth), m_displayHeight(displayHeight), m_fillType(fillType)
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



		PinholeCamera(const float& focalLength, const float& filmWidth, const float& filmHeight,
			const float& nearClipping,
			const int& displayWidth, const int& displayHeight,
			const FillingMode& fillType = FILL)
		: m_focalLength(focalLength), m_filmWidth(filmWidth), m_filmHeight(filmHeight)
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
			// The output vectors
			std::vector<geometry::Vector_3D<float>> ret;
			ret.reserve(3);

			// The projection matrix (Camera is the origin of the world now)

			geometry::Matrix_4x4<float> transformation_inv = this->m_transformation.inverse();

			for (const geometry::Vertex_3D<float>& vert : tri.getVertices()) {
				// Point to camera transformation
				geometry::Vector_3D<float> v = mul_vectorMatrix(vert.position, transformation_inv);

				// Checking validity of the transformed vertex from world to camera
				if (v.z <= 0) throw std::exception("Devide by zero");
				
				// Projecting the point to the camera
				geometry::Vector_2D<float> sv(v.x / v.z, (v.y / v.z));

				// Transform the projected points to the NDC values By mathematically building it from Left < x < right to -1 < ndc.x < 1
				/*geometry::Vector_2D<int> ndc(	(2 * sv.x) / (m_right - m_left) - (m_right + m_left) / (m_right - m_left),
												(2 * sv.y) / (m_top - m_bottom) - (m_top + m_bottom) / (m_top - m_bottom));*/
				geometry::Vector_2D<float> ndc(	(sv.x - this->m_left) / (this->m_right - this->m_left), // may coz float errors (deviding small close numbers)
												(sv.y - this->m_top) / ( this->m_bottom - this->m_top));

				// Change the NDC point to the screen space (raster point)
				geometry::Vector_3D<float> rp(	ndc.x * this->m_displayWidth, ndc.y * this->m_displayHeight, v.z);
				// ^^^ we needed to return "z" because we changed the transformation						^^
				//geometry::Vector_3D<float> rp(sv.x, sv.y, v.z);
				// Pushing to the returned vectors
				ret.push_back(rp);
			}

			// Return the screen vertices of the projected Triange
			return geometry::Triangle_3D<float>(ret[0],ret[1],ret[2]);
			
		}
		

		void moveBackward() {
			this->m_transformation.m_matrix[3][2] += 1.0;
		}

		std::wstring toString() {
			geometry::Matrix_4x4<float> inv = m_transformation.inverse();
			std::string s = inv.toString();
			return std::wstring(s.begin(), s.end());
		}
	};

}
#endif // !CAMERA

