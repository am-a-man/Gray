#pragma once

//#include"renderer.h"
#include<string>
#include<fstream>
#include<tuple>
#include<sstream>
#include<unordered_map>

class shader {
private:
	std::string filepath;
	unsigned int m_rendererID;

	std::unordered_map<std::string, int> m_uniformLocationCache;
	// we will also need a "caching system"
public:
	shader(const std::string& filename);
	~shader();
	void bind() const;  // GL does not bind shaders, it uses glUseProgram(shader) but we are going to be consistent with our api
	void unbind() const;

	// set uniforms
	void setUniform4f(const char* name, float f0, float f1, float f2, float f3);
	void setUniform3f(const char* name, float f0, float f1, float f2);
	void setUniform2f(const char* name, float f0, float f1);
	void setUniform1f(const char* name, float f0);
	void setUniform1i(const char* name, int i0);
	void setUniform2i(const char* name, int i0, int i1);
private:
	int getUniformLocation(const std::string& name);
	std::tuple<const std::string, const std::string>parseShader();
	unsigned int compileShader(const int& shaderType, const std::string& shaderSource);
	unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader);
};