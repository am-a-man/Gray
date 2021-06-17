#include <GL/glew.h>
#include <GL/glut.h>
#include <GLFW/glfw3.h>
#include<stdlib.h>
#include<iostream>

#include"indexBuffer.h"
#include"vertexBuffer.h"
#include"vertexArray.h"
#include"vertexBufferLayout.h"
#include"shader.h"
#include"renderer.h"





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




    {
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


        vertexArray vao;
        vertexBuffer buffer(positions, 4 * 2 * sizeof(float));
        vertexBufferLayout layout;
        layout.push<float>(2);

        vao.addBuffer(buffer, layout);


        /* start: adding index buffers */
        indexBuffer ibo(indices, 6);
        /* end: adding index buffers */

        shader sh("res/shaders/basic.shader");



        /*start : unbinding everything : because in our actual program kwith many elements we have to bind the state everytime for each element*/
        GLCall(glUseProgram(0));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
        GLCall(glBindVertexArray(0));
        /* end : unbinding everything*/
//        renderer render;

            /*when we actually apply this we define vertex as struct so ultimately we have to give in size of struct in offset*/
        /* Loop until the user closes the window */

        renderer render;


        while (!glfwWindowShouldClose(window))
        {
            render.clear();
            sh.bind();
            sh.setUniform("u_Color", float((rand() + 26) % 123) / 123, float((rand() + 26) % 123) / 123, float((rand() + 26) % 123) / 123, 1.0);


            /* draw call */
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
           
            render.draw(vao, ibo, sh);


            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

     
    }
    glfwTerminate();
    return 0;
}