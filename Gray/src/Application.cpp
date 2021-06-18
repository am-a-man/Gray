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
#include"texture.h"




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
    window = glfwCreateWindow(720, 720, "Hello World", NULL, NULL);
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
            -0.9f,-0.9f, 0.0f , 0.0f , // 0
             0.9f,-0.9f, 1.0f , 0.0f , // 1
             0.9f, 0.9f, 1.0f , 1.0f , // 2 
            -0.9f, 0.9f, 0.0f , 1.0f , // 3
        };

   
        unsigned int indices[] = {
            0,1,2,
            2,3,0
        };


        vertexArray vao;
        vertexBuffer buffer(positions, 4 * 4 * sizeof(float));
        vertexBufferLayout layout;
        layout.push<float>(2);
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
        texture texture("res/images/logo.png");/* very important, one thing that we need to do is to tell our shader which texture slot to sample from */
                                               /* and the way we are going to do that is by using Uniforms */
                                               /* this is isn't really a integer slot, thisis basically a sampler slot */
                                               /* TODO: we need to send an integer uniform to our shader and that integer is the slot ,*/
                                               /* and in the sampler code we can use that integer/sampler to sample the texture and do what we want to do */

     
        while (!glfwWindowShouldClose(window))
        {
            render.clear();
            sh.bind();
            sh.setUniform4f("u_Color", float((rand() + 26) % 123) / 123, float((rand() + 26) % 123) / 123, float((rand() + 26) % 123) / 123, 1.0);
           

            texture.bind(0);
            sh.setUniform1i("u_Texture", 0);/* see the comments near texture variable declaration (above this while loop) */

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