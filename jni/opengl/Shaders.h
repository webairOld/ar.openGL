/*
 * Shaders.h
 *
 *  Created on: Jun 10, 2012
 *      Author: seed
 */

static const char verticeShader[] =
		"attribute vec3 vPosition;	\n"
		"attribute vec3 vNormal;	\n"

		"varying vec4 fColor;	\n"
		"uniform mat4 mProjection;	\n"
		"uniform mat4 mModelView;	\n"
		"uniform vec3 vLightPosition;	\n"

		"void main()                 \n"
		"{"
		"   vec4 vModelView = mModelView * vec4(vPosition,1.0);		\n"
		"   vec4 vNormalView = mModelView * vec4(vNormal,1.0);	\n"
		"	fColor = vec4(1.0, 1.0, 1.0, 1.0);					\n"
		"	vec3 lightVector = normalize(vLightPosition - vModelView.xyz); \n"
		"   float distance = length(lightVector); 			\n"
		"   float fCosine = dot(vNormalView.xyz, normalize(lightVector));	\n"
		"	float fLamber = max(fCosine,0.1);					\n"
		"   fColor = vec4(fColor.xyz * fLamber * (1.0 / (1.0 + (0.25 * distance * distance))),1.0);			\n"
		"   gl_Position = mProjection * vModelView; \n"
		"}                           \n";

static const char fragmentShader[] =
		"precision mediump float;\n"
		"varying vec4 fColor;\n"
		"void main()\n"
		"{\n"
		"  gl_FragColor = fColor; \n"
		"} \n";
