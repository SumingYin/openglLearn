#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "shader.h"
#include "Texture.h"
#include "VertexBufferLayout.h"









int main(void)
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
    glfwSwapInterval(1); // Buffer change time interval 
    // glewInit must call after glfw context establish
    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;
    std::cout << glGetString(GL_VERSION) << std::endl;
    {
        float positions[] = {
            -0.5f, -0.5f,0.0f,0.0f,// 0 means the first vertex ,a vertex not mean a position,but include other stuff like texture
             0.5f, -0.5f,1.0f,0.0f, // 1
             0.5f,  0.5f,1.0f,1.0f, // 2
            -0.5f,  0.5f,0.0f,1.0f,  // 3
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };
        

        //GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA)); 
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        GLCall(glEnable(GL_BLEND));

        // create vao vertex array object 
        VertexArray vao;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        vao.AddBuffer(vb, layout);


        

        // genbuffers for indices and bind ,put data to it
        IndexBuffer ib(indices, 6);

        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        Texture texture("res/textures/mario.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);// 0 means slot 0 


        
        vao.Unbind();
        shader.Unbind();
        vb.Unbind();
        ib.Unbind();

        float r = 0.0f;
        float increment = 0.05f;
        Renderer renderer;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            //GLClearError(); // clear all error before,focus on next statement area
            //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); // this statement is false
            //GLCheckError(); // show the error in the terminal,this will cause 1280 error code
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
            renderer.Draw(vao, ib, shader);
            // we use macro to reuse it 
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr)); // use macro to warp it 
            if (r > 1.0f)
                increment = -0.5f;
            else if (r < 0.0f)
                increment = 0.5f;

            r += increment;
            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
        
    } // this {} for `destructor to complete their jobs, if we don't have this {} it will make errors,glfwTerminate() is close,check errors function will breakout 
    glfwTerminate();
    return 0;
}
