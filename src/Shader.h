#pragma once

#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>

class Shader
{
private:
  typedef unsigned int Shaderid;
  Shaderid loadShader(int shadertype, char const * shaderSrc);
  std::string read_shader(char const * shaderfilename);
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // use/activate the shader
    void use();
    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void setMat4(const std::string &name, glm::mat4 mtx) const;
};
