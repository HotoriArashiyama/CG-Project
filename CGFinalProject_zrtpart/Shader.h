#pragma once
#ifndef __SHADER_H__
#define __SHADER_H__
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <assert.h>
#pragma comment(lib, "glew32.lib")
#include <gl/glew.h>
#include <gl/glut.h>

class Shader
{
public:
	Shader(const char* vSrc, const char* fSrc);
	~Shader();
	static GLuint initShader(const char* Src, GLenum type);
	static GLuint genShaders(const char* vSrc, const char* fSrc);
	GLuint getHdl() { return Hdl; }

private:
	GLuint Hdl;
};

Shader::Shader(const char* vSrc, const char* fSrc){
	Hdl = genShaders(vSrc, fSrc);
}

Shader::~Shader(){
}

GLuint Shader::initShader(const char* Src, GLenum type) {
	GLuint hdlShdr = glCreateShader(type);
	glShaderSource(hdlShdr, 1, &Src, NULL);
	glCompileShader(hdlShdr);

	GLint flagCompile;
	glGetShaderiv(hdlShdr, GL_COMPILE_STATUS, &flagCompile);
	assert(flagCompile);

	return hdlShdr;
}

GLuint Shader::genShaders(const char* vSrc, const char* fSrc) {
	GLuint vtxShdrHdl = initShader(vSrc, GL_VERTEX_SHADER);
	GLuint fragShdrHdl = initShader(fSrc, GL_FRAGMENT_SHADER);

	GLuint shdrHdl = glCreateProgram();
	glAttachShader(shdrHdl, vtxShdrHdl);
	glAttachShader(shdrHdl, fragShdrHdl);
	glLinkProgram(shdrHdl);

	GLint flagCompile;
	glGetShaderiv(shdrHdl, GL_COMPILE_STATUS, &flagCompile);
	assert(flagCompile);

	glDeleteShader(vtxShdrHdl);
	glDeleteShader(fragShdrHdl);

	return shdrHdl;
}

#endif
