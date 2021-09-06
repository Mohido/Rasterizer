#pragma once
#include <vector>
#include "geometry.h"
#include "cow.h"


bool loadMesh(std::vector<geometry::Triangle_3D<float>>& mesh) {
	if (mesh.size() > 0) return false;

	for (unsigned int i = 0; i < cow::ntris; i++) {
		// Getting the position of the tirangle vertices
		geometry::Vertex_3D<float> p1 (cow::vertices[cow::nvertices[i * 3]]);
		geometry::Vertex_3D<float> p2 (cow::vertices[cow::nvertices[i * 3 + 1]]);
		geometry::Vertex_3D<float> p3 (cow::vertices[cow::nvertices[i * 3 + 2]]);

		mesh.push_back(geometry::Triangle_3D<float>(p1,p2,p3));

	}

	return true;
}