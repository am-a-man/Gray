#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<tuple>

static std::tuple<std::string, std::string> parseShader(const std::string& filepath)
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
                ss[int(type)] << line <<"\n";
            }
        }


    }
    return std::tuple(ss[0].str(), ss[1].str());
   

}


static unsigned int compileShader(const int& shaderType, const std::string& source)
{
    unsigned int customShader = glCreateShader(shaderType);
    const char* src = source.c_str();// this can also be written as "&source[0]" this is what I think I would be using most of the time
    glShaderSource(customShader, 1, &src, NULL);
    glCompileShader(customShader);
    int result;
    glGetShaderiv(customShader, GL_COMPILE_STATUS, &result );

    //TODO : include error hadling: we are going to check what the compile status of glCompileShader is 
    // now since we can check that glCompileShader's return type is void 
    // we are going to use 
    if (result == 0)
    {
        int length;
        glGetShaderiv(customShader, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(customShader, length, &length, message);
        std::cout << "failed to compile " << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " shader\n";
        std::cout << "some error occoured: " <<message<< std::endl;
        glDeleteShader(customShader);
    }
    else 
    {
        std::cout<<"everything runs smoothly\n";
    }
    // error handling ends

    return customShader;
}


static int createShader(const std::string& vertexShader, const std::string& fragmentShader)
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
    
    unsigned int program = glCreateProgram();
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    glAttachShader(program, vs); // this is the program created by glCreateProgram() in the above lines of code
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    glDeleteShader(fs);
    glDeleteShader(vs);
    return 1;
}


int main(int argc, char** argv)
{
    GLFWwindow* window;

   

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    std::cout << glGetString(GL_VERSION) << std::endl;

    if (glewInit() == GLEW_OK)
        std::cout << "Glew is working, all gl function pointers are loaded\n";
    else
        std::cout << "errors in Glew\n";
    
    unsigned int buffer;



    glGenBuffers(1, &buffer);
    float positions[] = {
         0.5f,-0.5f, // 0
        -0.5f, 0.5f, // 1
         0.5f, 0.9f, // 2 
         0.99f,-0.5f  // 3
    };


    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };




    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6*2*sizeof(float),&positions,GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    
//    std::string vertexShader = vertexSource();
//    std::string fragmentShader = fragmentSource();



    /* start: adding index buffers */
    unsigned int ibo; // ibo : index buffer object
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(float), &indices, GL_STATIC_DRAW);

    /* end: adding index buffers*/


    auto [vertexShader, fragmentShader] = parseShader("res/shaders/basic.shader"); // this will need the relative working directory
    //and the working directory is the one where the executable is present, but the Visual studio debugger sets the working directory to "$(ProjectDir)" which is in the Gray working file 



    //std::cout << vertexShader << fragmentShader;
    int shader = createShader(vertexShader, fragmentShader);
    glUseProgram(shader);

        /*when we actually apply this we define vertex as struct so ultimately we have to give in size of struct in offset*/
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        /* Render here */
        try {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        }
        catch (const std::exception& error)
        {
            std::cout << "error";
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}