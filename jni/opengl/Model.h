#ifndef AR_OPENGL_MODEL
#define AR_OPENGL_MODEL

#include <stdio.h>
#include <stdlib.h>

class Renderer;

class Model {

public:
	Model(float [],float []);
	~Model();

	void draw(Renderer *renderer);

private:
	float *vertices;
	float *normals;
	int numberOfVertices;
};

#endif
