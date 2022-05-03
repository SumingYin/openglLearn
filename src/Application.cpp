#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <fstream>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"



struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0,FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) 
    {
        if (line.find("#shader") != std::string::npos)
        {
            if(line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
           
        else
        {
            ss[(int)type] << line << '\n';
        }
    }
    return { ss[0].str(),ss[1].str() };
}




static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GLCall(glShaderSource(id, 1, &src, nullptr));
    GLCall(glCompileShader(id));
    // TODO:Error handling

    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char *)alloca(length * sizeof(char));// stack memory
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" :"fragment") << std::endl;
        std::cout << message << std::endl;

    }


    return id;

}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    // when we debug shaders program ,we should not delete these shaders so that we can see the errors we make properly.
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}

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
            -0.5f, -0.5f, // 0 means the first vertex ,a vertex not mean a position,but include other stuff like texture
             0.5f, -0.5f, // 1
             0.5f,  0.5f, // 2
            -0.5f,  0.5f  // 3
        };

        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };

        // create vao vertex array object 
        VertexArray vao;
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        vao.AddBuffer(vb, layout);


        

        // genbuffers for indices and bind ,put data to it
        IndexBuffer ib(indices, 6);

        ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
        std::cout << "VERTEX" << std::endl;
        std::cout << source.VertexSource << std::endl;
        std::cout << "FRAGMENT" << std::endl;
        std::cout << source.FragmentSource << std::endl;

        const unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
        GLCall(glUseProgram(shader)); // use already linked program 
        // use uniform to color
        GLCall(int location = glGetUniformLocation(shader, "u_Color"));
        ASSERT(location != -1);
        GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

        float r = 0.0f;
        float increment = 0.05f;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            GLCall(glUseProgram(shader)); // use already linked program 
            GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f)); // uniform use in each per draw different from vertex
            // draw call let graphics card to draw ,system call 
            // glDrawArrays(GL_TRIANGLES, 0, 3);
            vao.Bind();
            ib.Bind();
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr)); // function use indices to draw ,instead of glDrawArrays
            // glDrawElements will be used often.

            //GLClearError(); // clear all error before,focus on next statement area
            //glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr); // this statement is false
            //GLCheckError(); // show the error in the terminal,this will cause 1280 error code

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
        GLCall(glDeleteProgram(shader)); // delete the program we put in the graphics card
    } // this {} for `destructor to complete their jobs, if we don't have this {} it will make errors,glfwTerminate() is close,check errors function will breakout 
    glfwTerminate();
    return 0;
}
