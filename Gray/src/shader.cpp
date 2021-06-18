#include"shader.h"
#include<iostream>
#include<GL/glew.h>
#include"renderer.h"


shader::shader(const std::string& filename)
    :filepath{filename}
{
    
	auto [vertexShaderSource, fragmentShaderSource] = parseShader(); // this will need the relative working directory
   
        //and the working directory is the one where the executable is present, but the Visual studio debugger sets the working directory to "$(ProjectDir)" which is in the Gray working file 
    m_rendererID = createShader(vertexShaderSource, fragmentShaderSource);
    GLCall(glUseProgram(m_rendererID));
}

shader::~shader()
{
	GLCall(glDeleteProgram(m_rendererID));
}



void shader::bind() const
{
    GLCall(glUseProgram(m_rendererID));
}


void shader::unbind() const
{
    GLCall(glUseProgram(0));
    
}


int shader::getUniformLocation(const std::string& name)
{
    
    if (m_uniformLocationCache[name])
        return m_uniformLocationCache[name];

    GLCall(int location = glGetUniformLocation(m_rendererID,name.c_str()));
    if (location == -1)
    {
        std::cout << "word " << name << " not found\n";
        ASSERT(false);
    }
    m_uniformLocationCache[name] = location;
    return location;
}



void shader::setUniform4f(const char* name, float f0, float f1, float f2, float f3)
{
    GLCall(glUniform4f(getUniformLocation(name), f0, f1, f2, f3));
}



void shader::setUniform3f(const char* name, float f0, float f1, float f2)
{
    GLCall(glUniform3f(getUniformLocation(name), f0, f1, f2));

}
void shader::setUniform2f(const char* name, float f0, float f1)
{
    GLCall(glUniform2f(getUniformLocation(name), f0, f1));

}
void shader::setUniform1f(const char* name, float f0)
{
    GLCall(glUniform1f(getUniformLocation(name), f0));

}
void shader::setUniform1i(const char* name, int i0)
{
    GLCall(glUniform1i(getUniformLocation(name), i0));

}
void shader::setUniform2i(const char* name, int i0, int i1)
{
    GLCall(glUniform2i(getUniformLocation(name), i0, i1));

}


unsigned int shader::compileShader(const int& shaderType, const std::string& source)
{
    GLCall(unsigned int customShader = glCreateShader(shaderType));
    const char* src = source.c_str();// this can also be written as "&source[0]" this is what I think I would be using most of the time
    GLCall(glShaderSource(customShader, 1, &src, NULL));
    GLCall(glCompileShader(customShader));
    int result;
    GLCall(glGetShaderiv(customShader, GL_COMPILE_STATUS, &result));

    //TODO : include error handling: we are going to check what the compile status of glCompileShader is 
    // now since we can check that glCompileShader's return type is void 
    // we are going to use 
    if (result == 0)
    {
        int length;
        GLCall(glGetShaderiv(customShader, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(customShader, length, &length, message));
        std::cout << "failed to compile " << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " shader\n";
        std::cout << "some error occoured: " << message << std::endl;
        GLCall(glDeleteShader(customShader));
    }
    else
    {
        std::cout << "everything runs smoothly\n";
    }
    // error handling ends

    return customShader;
}



unsigned int shader::createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    // inside here we are going to write the code to compile vertexShader and fragmentShader
    // these strings are just meant to be actual source code

    /*
    meaning that shaders can be avilable in many forms but in this case we are going to use the actual source code
    for shaders and the two strings 'vertexShader' and 'fragmentShader' represents the actual source code
    just for simplicity sake;
    */

    /*
    even when we are using shaders that is imported from a file it has to be passed as a string and that will be
    implemented next;
    */

    /*
    1. we write a shader function whose basic purpose are:
        1.we need to provide openGL with actual shader source code/text
        2.we want openGL to compile the progream and link these two into a single shader program and
                give some kind of unique identifier for that shader back so that we can bind that shader
                and then use it
    */

    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader));

    GLCall(glAttachShader(program, vs)); // this is the program created by glCreateProgram() in the above lines of code
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    GLCall(glDeleteShader(fs));
    GLCall(glDeleteShader(vs));

    return program;
}


std::tuple<const std::string, const std::string> shader::parseShader()
{
    std::ifstream stream(filepath);
    std::string line;
    std::stringstream ss[2];

    enum class shaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    shaderType type = shaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = shaderType::VERTEX;//set mode to vertex
            }
            else if (line.find("fragment") != std::string::npos)
            {
                ;//set mode to fragment
                type = shaderType::FRAGMENT;
            }
        }
        else {
            if (type != shaderType::NONE)
            {
                ss[int(type)] << line << "\n";
            }
        }


    }
    return std::tuple(ss[0].str(), ss[1].str());


}