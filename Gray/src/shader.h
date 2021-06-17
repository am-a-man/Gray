#pragma once

#include"renderer.h"
#include<string>
#include<fstream>
#include<tuple>
#include<sstream>

class shader {
private:
	std::string filepath;
	unsigned int m_rendererID;
	// we will also need a "caching system"
public:
	shader(const std::string& filename);
	~shader();
	void bind() const;  // GL does not bind shaders, it uses glUseProgram(shader) but we are going to be consistent with our api
	void unbind() const;

	// set uniforms
	void setUniform(const char* name, float f0, float f1, float f2, float f3);
private:
	int getUniformLocation(const char* name);
	std::tuple<const std::string, const std::string>parseShader();
	unsigned int compileShader(const int& shaderType, const std::string& shaderSource);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};