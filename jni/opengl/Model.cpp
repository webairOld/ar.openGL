#include "Model.h"
#include "util/Logger.h"
//#include "modelData/bundehaus.h"
#include "modelData/Cube.h"

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Renderer.h"

/*
 *  Constructor & Destructor
 */

Model::Model(float theVertices[], float theNormals[]) {

	//TODO find out why i can't pass arrays!!!

	//WORKAROUND


	int sizeVertices = sizeof(objVerts) / sizeof(float);
	int sizeNormals = sizeof(objNormals) / sizeof(float);
	numberOfVertices = sizeVertices / 3;

	LOGI("Load models: points: %d, vertices: %d, normals: %d",sizeVertices,numberOfVertices,sizeNormals);

	vertices = new float[sizeVertices];
	normals = new float[sizeNormals];

	for (int i = 0; i<sizeVertices; i++) {
		vertices[i] = objVerts[i];
	}

	for (int i = 0; i<sizeNormals; i++) {
		normals[i] = objNormals[i];
	}
}

Model::~Model() {
	delete vertices;
	delete normals;
}

void Model::draw(Renderer *renderer) {
	glVertexAttribPointer(renderer->vectorPositionRef,3, GL_FLOAT,GL_FALSE, 0, this->vertices);
	glVertexAttribPointer(renderer->vectorNormalRef,3, GL_FLOAT,GL_FALSE, 0, this->normals);
	glDrawArrays(GL_TRIANGLES,0,this->numberOfVertices);
}
