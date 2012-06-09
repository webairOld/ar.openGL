#include "opengl-jni.h"

#include <android/log.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "Model.h"

//Export this lines to a seperate header file
#define  LOG_TAG    "opengl-jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

static void printGLString(const char *name, GLenum s) {
	const char *v = (const char *) glGetString(s);
	LOGI("GL %s = %s\n", name, v);
}

static void checkGlError(const char* op) {
	for (GLint error = glGetError(); error; error = glGetError()) {
		LOGI("after %s() glError (0x%x)\n", op, error);
	}
}

GLuint program;
GLuint projectionUniform;
float width;
float height;

static const char vShaderStr[] = "attribute vec4 vPosition;\n"
		"uniform mat4 Projection;\n"
		"void main()                 \n"
		"{                           \n"
		"   gl_Position =  Projection * vPosition; \n"
		"}                           \n";

static const char fShaderStr[] = "precision mediump float;\n"
		"void main()                                \n"
		"{                                          \n"
		"  gl_FragColor = vec4(0.0, 1.0, 0.0, 1.0); \n"
		"} \n";

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
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
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

bool setupGraphics(int w, int h) {
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);

	width = (float)w;
	height = (float)h;

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;
	GLint linked;

	// Load the vertex/fragment shaders
	vertexShader = loadShader(GL_VERTEX_SHADER, vShaderStr);
	fragmentShader = loadShader(GL_FRAGMENT_SHADER, fShaderStr);
	// Create the program object
	programObject = glCreateProgram();
	if (programObject == 0)
		return 0;
	glAttachShader(programObject, vertexShader);
	glAttachShader(programObject, fragmentShader);
	// Bind vPosition to attribute 0
	glBindAttribLocation(programObject, 0, "vPosition");
	// Link the program
	glLinkProgram(programObject);
	// Check the link status
	glGetProgramiv(programObject, GL_LINK_STATUS, &linked);
	if (!linked) {
		GLint infoLen = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			LOGE("Error linking program",LOG_TAG);
		}
		glDeleteProgram(programObject);
		return false;
	}
	// Store the program object
	program = programObject;
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glViewport(0, 0, w, h);
	checkGlError("glViewport");


	return true;
}

void renderFrame() {
	//glClearColor(0.8, 0.6, 1.0, 1.0f);
	//checkGlError("glClearColor");
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	checkGlError("glClear");

	// Use the program object
	glUseProgram(program);
	checkGlError("glUseProgram");

	float vertices[] = {
		// FRONT
		 1.0f, 1.0f, -7.0f,
		 -1.0f, 1.0f, -7.0f,
		 1.0f, -1.0f, -7.0f,
		 -1.0f, -1.0f, -7.0f,

	};

	float h = 4.0f * height / width;

	float near = 4;
	float far = 10;
	float right = 2.0;
	float left = -2.0;
	float bottom = -h/2;
	float top = h/2;


	projectionUniform = glGetUniformLocation(program,"Projection");
	checkGlError("glGetUniformLocation");

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

	glUniformMatrix4fv(projectionUniform,1, 0,projection);
	checkGlError("glUniformMatrix4fv");

	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

}

extern "C" {

JNIEXPORT void JNICALL Java_ch_bfh_bachelor_opengl_JNIOpenGLInterface_init(
		JNIEnv *env, jclass thiz, jint width, jint height) {

	setupGraphics(width, height);

}

JNIEXPORT void JNICALL Java_ch_bfh_bachelor_opengl_JNIOpenGLInterface_step(
		JNIEnv *env, jclass thiz) {
	renderFrame();
}

}
