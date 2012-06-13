#ifndef AR_OPENGL_RENDERER
#define AR_OPENGL_RENDERER

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "Model.h"

class Renderer {
public:
	Renderer();
	~Renderer();

	GLuint matrixProjectionRef;
	GLuint matrixModelViewRef;
	GLuint vectorLightPositionRef;
	GLuint vectorPositionRef;
	GLuint vectorNormalRef;

	void setScreenSize(int w, int h);
	void renderFrame();


private:
	GLuint programRef;
	float width;
	float height;

	Model *model;

	static void printGLString(const char *name, GLenum s);
	static void checkGlError(const char* op);

	void setupGraphics();
	void loadModels();
	GLuint loadShader(GLenum shaderType, const char* pSource);

	double lastTime;
	void loadFile();
};
#endif
