#include <stdio.h>
#include <stdlib.h>

class SimpleModel {

public:
	SimpleModel();
	~SimpleModel();

	float * getVertices();

private:
	float *vertices;
};


