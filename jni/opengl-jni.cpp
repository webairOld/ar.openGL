#include "opengl-jni.h"
#include "util/Logger.h"
#include "opengl/Renderer.h"
#include "android/asset_manager.h"



Renderer *r;

extern "C" {

JNIEXPORT void JNICALL Java_ch_bfh_bachelor_opengl_JNIOpenGLInterface_init(
		JNIEnv *env, jclass thiz, jint width, jint height) {
	r = new Renderer();
	r->setScreenSize((int)width,(int)height);
}

JNIEXPORT void JNICALL Java_ch_bfh_bachelor_opengl_JNIOpenGLInterface_step(
		JNIEnv *env, jclass thiz) {
	r->renderFrame();
}

}
