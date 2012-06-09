#include "SimpleModel.h"

SimpleModel::SimpleModel() {
	vertices = new float[12];

	float theVertices[] = {
		// FRONT
		 0.5f, 0.5f, -1.0f,
		 -0.5f, 0.5f, -1.0f,
		 0.5f, -0.5f, -1.0f,
		 -0.5f, -0.5f, -1.0f,

	};

	for (int i = 0; i<sizeof(theVertices) / sizeof(float); i++) {
		vertices[i] = theVertices[i];
	}
}

float * SimpleModel::getVertices() {
	return vertices;
}

SimpleModel::~SimpleModel() {
	delete vertices;
}

/*
	float normals[72] = {
			// FRONT
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			0.0f, 0.0f,  1.0f,
			// BACK
			0.0f, 0.0f,  -1.0f,
			0.0f, 0.0f,  -1.0f,
			0.0f, 0.0f,  -1.0f,
			0.0f, 0.0f,  -1.0f,
			// LEFT
			-1.0f, 0.0f,  0.0f,
			-1.0f, 0.0f,  0.0f,
			-1.0f, 0.0f,  0.0f,
			-1.0f, 0.0f,  0.0f,
			// RIGHT
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			1.0f, 0.0f,  0.0f,
			// TOP
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			0.0f, 1.0f,  0.0f,
			// BOTTOM
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
			0.0f, -1.0f,  0.0f,
		};
	float vertices[] = {
		// FRONT
		-25.0f,-25.0f, 25.0f,
		 25.0f,-25.0f, 25.0f,
		-25.0f, 25.0f, 25.0f,
		 25.0f, 25.0f, 25.0f,
		// BACK
		-25.0f,-25.0f,-25.0f,
		-25.0f, 25.0f, -25.0f,
		 25.0f, -25.0f,-25.0f,
		 25.0f, 25.0f, -25.0f,
		// LEFT
		-25.0f,-25.0f, 25.0f,
		-25.0f, 25.0f, 25.0f,
		-25.0f,-25.0f,-25.0f,
		-25.0f, 25.0f,-25.0f,
		 // RIGHT
		 25.0f,-25.0f,-25.0f,
		 25.0f, 25.0f,-25.0f,
		 25.0f,-25.0f, 25.0f,
		 25.0f, 25.0f, 25.0f,
		 // TOP
		-25.0f, 25.0f, 25.0f,
		 25.0f, 25.0f, 25.0f,
		-25.0f, 25.0f, -25.0f,
		 25.0f, 25.0f, -25.0f,
		 // BOTTOM
		-25.0f, -25.0f, 25.0f,
		-25.0f, -25.0f, -25.0f,
		 25.0f, -25.0f, 25.0f,
		 25.0f, -25.0f, -25.0f,
	};
*/

