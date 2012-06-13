#include "Renderer.h"
#include "Shaders.h"
#include <android/log.h>
#include "time.h"
#include "util/Logger.h"

#include <GLES2/gl2ext.h>
#include <android/asset_manager.h>

//#include "modelData/bundehaus.h"

/*
 * Constructor & Destructor
 */

Renderer::Renderer() {
	setupGraphics();
	loadModels();
}

Renderer::~Renderer(){
	delete model;
}

void Renderer::printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);

}

void Renderer::checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}


void Renderer::setupGraphics() {
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	GLuint vShaderRef;
	GLuint fShaderRef;
	GLint linked;

	// Load the vertex/fragment shaders
	vShaderRef = loadShader(GL_VERTEX_SHADER, verticeShader);
	fShaderRef = loadShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Create the program object
	programRef = glCreateProgram();
	if (programRef == 0) {
		LOGE("setup graphics failed !!!");
		return;
	}

	glAttachShader(programRef, vShaderRef);
	glAttachShader(programRef, fShaderRef);
	// Bind vPosition to attribute 0
	glBindAttribLocation(programRef, 0, "vPosition");
	glBindAttribLocation(programRef, 1, "vNormal");
	// Link the program
	glLinkProgram(programRef);
	// Check the link status
	glGetProgramiv(programRef, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programRef, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			LOGE("Error linking program",LOG_TAG);
		}
		glDeleteProgram(programRef);
		return;
	}
	glUseProgram(programRef);
	vectorPositionRef = glGetAttribLocation(programRef, "vPosition");
	vectorNormalRef = glGetAttribLocation(programRef, "vNormal");
	vectorLightPositionRef = glGetUniformLocation(programRef,"vLightPosition");
	matrixProjectionRef = glGetUniformLocation(programRef,"mProjection");
	matrixModelViewRef = glGetUniformLocation(programRef,"mModelView");


	LOGI("pRef: %d, nRef: %d, lRef: %d, proRef: %d, modRef: %d",
			vectorPositionRef,vectorNormalRef,vectorLightPositionRef,
			matrixProjectionRef,matrixModelViewRef
	);
}

void Renderer::setScreenSize(int w, int h) {
	LOGI("set screen size w:%d h:%d",w,h);
	width=(float)w;
	height=(float)h;
	glViewport(0, 0, w, h);

	LOGI("converted size: w:%f h:%f",width,height);
	float aspect =  width / height;
	float size = 0.01f;

	LOGI("aspect: %f",aspect);

	float near =  size;
	float far = 10.0f;
	float right = size;
	float left = -size;
	float bottom = -size/aspect;
	float top = size/aspect;


	float projection[16];
	projection[0]  = (2.0 * near) / (right - left);
	projection[1]  = 0.0;
	projection[2]  = 0.0;
	projection[3] = 0.0;

	projection[4]  = 0.0;
	projection[5]  = (2.0 * near) / (top - bottom);
	projection[6]  = 0.0;
	projection[7] = 0.0;

	projection[8]  = (right + left) / (right - left);
	projection[9]  = (top + bottom) / (top - bottom);
	projection[10] = -(far + near) / (far - near);
	projection[11] = -1.0;

	projection[12]  = 0.0;
	projection[13]  = 0.0;
	projection[14] = -(2.0 * far * near) / (far - near);
	projection[15] = 0.0;


	glUniformMatrix4fv(matrixProjectionRef,1, 0,projection);
	checkGlError("glUniformMatrix4fv");

	float lightPos[] = {0.0f, 0.0f, -2.0f };
	glUniform3fv(vectorLightPositionRef,1,lightPos);

	float translation[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, -2.5f, 1.0f,
	};
	//rotate at point
	glUniformMatrix4fv(matrixModelViewRef,1, 0,translation);

	glEnableVertexAttribArray(vectorNormalRef);
	glEnableVertexAttribArray(vectorPositionRef);

}

GLuint Renderer::loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {

    	int shaderStringLength = strlen(pSource);
        glShaderSource(shader, 1, &pSource, &shaderStringLength);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                    exit(-1);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

void Renderer::loadModels() {
	//LOGI("model ref: %d",bundeshausVerts);
	model = new Model(NULL,NULL);
	/*
	int sizeVertices = sizeof(bundeshausVerts) / sizeof(float);
	int sizeNormals = sizeof(bundeshausNormals) / sizeof(float);
	int numberOfVertices = sizeVertices / 3;

	LOGI("Load models: points: %d, vertices: %d, normals: %d",sizeVertices,numberOfVertices,sizeNormals);
	*/

	//this->loadFile();
}

void Renderer::renderFrame() {
	//return;
	//LOGI("render frame");
	//
	//checkGlError("glClearColor");
	// set input data to arrays


	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glClearColor(0.0f, 0.0f,0.0f, 0.0f);

	model->draw(this);
	//calculate FPS

	 struct timespec res;
	 clock_gettime(CLOCK_MONOTONIC, &res);

	 double currentTime = 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
	 double timeDelta = currentTime-lastTime;
	 lastTime = currentTime;
	 //LOGI("FPS: %f",timeDelta);
}

void Renderer::loadFile() {
	AAssetManager* assetManager;
	AAsset* asset = AAssetManager_open(assetManager, "test.txt",AASSET_MODE_UNKNOWN);
	if (asset == NULL) {
	LOGI("COULD NOT OPEN ASSET");
	}
	// open asset as file descriptor
	off_t start, length;
	int fd = AAsset_openFileDescriptor(asset, &start, &length);
	if(fd < 0)
	{
	LOGI("COULD NOT OPEN FILE DESCRIPTOR");
	}
	AAsset_close(asset);


}
