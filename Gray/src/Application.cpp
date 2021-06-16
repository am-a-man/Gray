#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include<stdlib.h>
#include<iostream>
#include<string>
#include<fstream>
#include<sstream>
#include<tuple>
#define ASSERT(x) if (!(x)) __debugbreak(); // the underscores (__) in the beginning show that this is platform specific
                                            // and it will only run on visual studio and not on other platforms like gcc/g++


#define GLCall(x) GLClearErrors();\
    x;\
    ASSERT(GLCheckErrors(#x, __FILE__, __LINE__)) // these two __FILE__ and __LINE__ are not platform specific they are supported by all platforms
                                                  // https://www.tutorialspoint.com/what-are-file-line-and-function-in-cplusplus


static void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

static bool GLCheckErrors(const char* function, const char* file, const int line)
{
    int i = 0;
    if(GLenum error = glGetError())
    {
        std::cout << "error caught: " << error <<" at line: "<<line<<" in function: "<<function<<" in file: "<<file<< std::endl;
        return false;
    }
    return true;

}


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
    GLCall(unsigned int customShader = glCreateShader(shaderType));
    const char* src = source.c_str();// this can also be written as "&source[0]" this is what I think I would be using most of the time
    GLCall(glShaderSource(customShader, 1, &src, NULL));
    GLCall(glCompileShader(customShader));
    int result;
    GLCall(glGetShaderiv(customShader, GL_COMPILE_STATUS, &result));

    //TODO : include error hadling: we are going to check what the compile status of glCompileShader is 
    // now since we can check that glCompileShader's return type is void 
    // we are going to use 
    if (result == 0)
    {
        int length;
        GLCall(glGetShaderiv(customShader, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char));
        GLCall(glGetShaderInfoLog(customShader, length, &length, message));
        std::cout << "failed to compile " << ((shaderType == GL_VERTEX_SHADER) ? "VERTEX" : "FRAGMENT") << " shader\n";
        std::cout << "some error occoured: " <<message<< std::endl;
        GLCall(glDeleteShader(customShader));
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
    
    GLCall(unsigned int program = glCreateProgram());
    GLCall(unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader));
    GLCall(unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader));
    
    GLCall(glAttachShader(program, vs)); // this is the program created by glCreateProgram() in the above lines of code
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    GLCall(glDeleteShader(fs));
    GLCall(glDeleteShader(vs));
    return 1;
}


int main(int argc, char** argv)
{
    GLFWwindow* window;

   

    /* Initialize the library */
    if (!glfwInit())
        return -1;
   


    /* start : enbale the below program to use the program in core profile , gl version = 4.6 */

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    /* end: enable to use the program in core profile */





    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(20);
    std::cout << glGetString(GL_VERSION) << std::endl;

    if (glewInit() == GLEW_OK)
        std::cout << "Glew is working, all gl function pointers are loaded\n";
    else
        std::cout << "errors in Glew\n";
    
    unsigned int buffer;

    unsigned int vao;
    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));


    GLCall(glGenBuffers(1, &buffer));
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




    GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float),&positions,GL_STATIC_DRAW));
    GLCall(glEnableVertexAttribArray(0));  // this number is  the default vertex array object given in compatibility profile
                                           // whereas this is not an object in core profile
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
    
    
//    std::string vertexShader = vertexSource();
//    std::string fragmentShader = fragmentSource();



    /* start: adding index buffers */
    unsigned int ibo; // ibo : index buffer object
    GLCall(glGenBuffers(1, &ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(float), &indices, GL_STATIC_DRAW));

    /* end: adding index buffers*/


    auto [vertexShader, fragmentShader] = parseShader("res/shaders/basic.shader"); // this will need the relative working directory
    //and the working directory is the one where the executable is present, but the Visual studio debugger sets the working directory to "$(ProjectDir)" which is in the Gray working file 


    //std::cout << vertexShader << fragmentShader;
    int shader = createShader(vertexShader, fragmentShader);
    GLCall(glUseProgram(shader));





    int location = glGetUniformLocation(shader, "u_Color");
    ASSERT(location != -1);



    /*start : unbinding everything : because in our actual program kwith many elements we have to bind the state everytime for each element*/
    GLCall(glUseProgram(0));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    GLCall(glBindVertexArray(0));
    /* end : unbinding everything*/


        /*when we actually apply this we define vertex as struct so ultimately we have to give in size of struct in offset*/
    /* Loop until the user closes the window */

    while (!glfwWindowShouldClose(window))
    {
        GLCall(glClear(GL_COLOR_BUFFER_BIT));
        GLCall(glBindVertexArray(vao));

       /* ASSERT(GLCheckErrors());*/
        /* Render here */

        /*start : binding everything : because in our actual program kwith many elements we have to bind the state everytime for each element*/

        GLCall(glUseProgram(shader));
        
            /* start : Setting uniform : this is not a part of binding process but we need a bound shader to use a uniform */
        
        GLCall(glUniform4f(location, float((rand()+26)%123)/123, float((rand() + 26) % 123) / 123, float((rand() + 26) % 123) / 123, 1.0));
        
            /* end : setting uniform */
        
        //GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer)); /* do not need if using vertex array in core profile */
            /* start: setting vertex attributes */
        //GLCall(glEnableVertexAttribArray(0));  // this number is  the default vertex array object given in compatibility profile
                                                // whereas this is not an object in core profile
                                                 /* do not need if using vertex array in core profile */

        //GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));  /* do not need if using vertex array in core profile */
            /* end : setting vertex attributes    */  


        /*
        
        The reason why above vertex array method works:
            1. we are linking the vertex buffer to the vertex array object, question is which line of code is actually doing this
                1.  when we bind a vertex array and we bind a buffer, nothing actually links the two
                2.  when we specify the vertexAttribPointer we are saying that index 0 of this vertex array is going to actually be bound to the currently bound glArrayBuffer
                3.  if we want to bind a different buffer and then call glVertexAttribPointer with index 1after enabling it, then I would be saying that inside the array Buffer, the index 1 is pointing to a different vertex buffer


        */
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

        /* end : binding everything*/
         
        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return 0;
}