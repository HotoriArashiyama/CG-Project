#pragma once
#ifndef __SHADER_H__
#define __SHADER_H__
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <assert.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	Shader() { Init = false; Hdl = 0; };
	Shader(const char* vSrc, const char* fSrc);
	~Shader();
	static GLuint initShader(const char* Src, GLenum type);
	static GLuint genShaders(const char* vSrc, const char* fSrc);
	void Initialize(const char* vSrc, const char* fSrc);
	GLuint getHdl() { return Hdl; }
	void glmSetMat4(const std::string& name, const glm::mat4& mat) const;
	void glmSetVec3(const std::string& name, const glm::vec3& val) const;
	void glmSetVec4(const std::string& name, const glm::vec4& val) const;
	void glmSetInt(const std::string& name, int val) const;
	void glmSetFloat(const std::string& name, float val) const;

private:
	bool Init;
	GLuint Hdl;
};

Shader::Shader(const char* vSrc, const char* fSrc){
	Hdl = genShaders(vSrc, fSrc);
	Init = true;
}

Shader::~Shader(){
}

void Shader::Initialize(const char* vSrc, const char* fSrc) {
	if (Init) return;
	Hdl = genShaders(vSrc, fSrc);
	Init = true;
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

// Reference: https://learnopengl.com
void Shader::glmSetMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(Hdl, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

// Reference: https://learnopengl.com
void Shader::glmSetVec3(const std::string& name, const glm::vec3& val) const {
	glUniform3fv(glGetUniformLocation(Hdl, name.c_str()), 1, &val[0]);
}

// Reference: https://learnopengl.com
void Shader::glmSetVec4(const std::string& name, const glm::vec4& val) const {
	glUniform4fv(glGetUniformLocation(Hdl, name.c_str()), 1, &val[0]);
}

// Reference: https://learnopengl.com
void Shader::glmSetInt(const std::string& name, int val) const {
	glUniform1i(glGetUniformLocation(Hdl, name.c_str()), val);
}

// Reference: https://learnopengl.com
void Shader::glmSetFloat(const std::string& name, float val) const {
	glUniform1f(glGetUniformLocation(Hdl, name.c_str()), val);
}

#endif
