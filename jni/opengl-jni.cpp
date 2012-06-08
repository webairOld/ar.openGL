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

static const char vShaderStr[] = "attribute vec4 vPosition;\n"
		"void main()                 \n"
		"{                           \n"
		"   gl_Position = vPosition; \n"
		"}                           \n";

static const char fShaderStr[] = "precision mediump float;\n"
		"void main()                                \n"
		"{                                          \n"
		"  gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0); \n"
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

	// Use the program object
	glUseProgram(program);
	// Load the vertex data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 24);

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
