#include "Shader.h"
#include "Renderer.h"
#include "GL/glew.h"


//GLCall(glUseProgram(shader)); // use already linked program 
        // use uniform to color
//GLCall(int location = glGetUniformLocation(shader, "u_Color"));
//ASSERT(location != -1);
//GLCall(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath),m_Renderer_ID(0)
{
    ShaderProgramSource source = ParseShader(filepath);
    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;

    m_Renderer_ID = CreateShader(source.VertexSource, source.FragmentSource);
	
}

Shader::~Shader()
{
    GLCall(glDeleteProgram(m_Renderer_ID));
}



void Shader::Bind() const
{
    GLCall(glUseProgram(m_Renderer_ID));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];
    GLCall(int location = glGetUniformLocation(m_Renderer_ID, name.c_str()));
    if (location == -1)
    {
        std::cout << "Warning : uniform '" << name << "' doesn't exist !" << std::endl;
    }
    m_UniformLocationCache[name] = location;
    return location;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
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
        char* message = (char*)alloca(length * sizeof(char));// stack memory
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile" <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;

    }


    return id;

}



ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
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


unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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