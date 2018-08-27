#include "Shader.h"
#include <iostream>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar * geometryPath)
{
    ID = glCreateProgram();
    Shaderid vertexShader = loadShader(GL_VERTEX_SHADER, read_shader(vertexPath).c_str());
    Shaderid fragmentShader = loadShader(GL_FRAGMENT_SHADER, read_shader(fragmentPath).c_str());
    Shaderid geometryShader;
    GLint success;
    char infoLog[512];
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    if(geometryPath) {
      geometryShader = loadShader(GL_GEOMETRY_SHADER, read_shader(geometryPath).c_str());
      glAttachShader(ID, geometryShader);
    }
    glLinkProgram(ID);
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      throw std::runtime_error(infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if(geometryPath)
      glDeleteShader(geometryShader);
}

Shader::Shaderid Shader::loadShader(int shadertype, char const * shaderSrc) {
    Shaderid shader = glCreateShader(shadertype);
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::string shadertypename;
        switch(shadertype) {
        case GL_VERTEX_SHADER:
          shadertypename = "Vertex Shader";
          break;
        case GL_GEOMETRY_SHADER:
          shadertypename = "Geometry Shader";
          break;
        case GL_FRAGMENT_SHADER:
          shadertypename = "Fragment Shader";
        }
        throw std::runtime_error(shadertypename + ": " + std::string(infoLog));
    }
    return shader;
}

std::string Shader::read_shader(char const * shaderfilename) {
    std::ifstream input(shaderfilename, std::ifstream::in);
    if(!input.is_open())
      throw std::runtime_error("Cannot open file: " + std::string(shaderfilename));
    std::string shaderdata = "";
    static int const bufsiz = 256;
    char c;
    while(input.good()) {
      input.get(c);
      shaderdata += c;
    }
    return shaderdata;
}

void Shader::use() 
{ 
    glUseProgram(ID);
}  

void Shader::setBool(const std::string &name, bool value) const
{         
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
}

void Shader::setInt(const std::string &name, int value) const
{ 
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setFloat(const std::string &name, float value) const
{ 
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1., glm::value_ptr(vec)); 
}

void Shader::setMat4(const std::string &name, glm::mat4 mtx) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1., GL_FALSE, glm::value_ptr(mtx)); 
}
