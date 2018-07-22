#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "objects.h"

typedef unsigned int VBOid;
typedef unsigned int Shaderid;

class GLApp {
public:
  GLApp() {
    try { 
      glfwSetup();
      initWindow();
      initGLAD();
      unsigned int shaderProgram = initShaders();
      mainloop(shaderProgram);
    } catch(std::runtime_error e) {
      std::cerr << e.what() << std::endl;
    }
  }
  ~GLApp() {
    glfwTerminate();
  }
private:
  GLFWwindow * window;
  Model tri_model = get_triangle();
  Model square_model = get_square();
  void glfwSetup() {
    if(!glfwInit())
      throw std::runtime_error("Failed to init GLFW.");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  
  void initWindow() {
    window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
      glfwTerminate();
      throw std::runtime_error("Failed to create GLFW window.");
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  }
  void initGLAD() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
      throw std::runtime_error("Failed to initialize GLAD.");
  }
  static void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  template<typename T>
  std::size_t size_bytes(std::vector<T> collection) {
    return collection.size() * sizeof(typeid(typename std::vector<T>::value_type));
  }

  unsigned int setupVAO(Model model) {
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, size_bytes(model.vertices), model.vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_bytes(model.indices), model.indices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    return VAO;
  }

  void mainloop(unsigned int shaderProgram) {
    while(!glfwWindowShouldClose(window)) {
      register_input(GLFW_KEY_ESCAPE, GLFW_PRESS,
        [] (GLFWwindow * window) {
          glfwSetWindowShouldClose(window, true);
      });
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shaderProgram);
      setupVAO(square_model);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);
      setupVAO(tri_model);
      glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  Shaderid loadShader(int shadertype, char const * shaderSrc) {
    Shaderid shader = glCreateShader(shadertype);
    glShaderSource(shader, 1, &shaderSrc, nullptr);
    glCompileShader(shader);
    GLint success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(shader, 512, nullptr, infoLog);
      throw std::runtime_error((shadertype == GL_VERTEX_SHADER ? "Vertex Shader: " : "Fragment Shader: ") + std::string(infoLog));
    }
    return shader;
  }

  unsigned int initShaders() {
    unsigned int shaderProgram = glCreateProgram();
    Shaderid vertexShader = loadShader(GL_VERTEX_SHADER, read_shader("shaders/vertex.glsl").c_str());
    Shaderid fragmentShader = loadShader(GL_FRAGMENT_SHADER, read_shader("shaders/fragment.glsl").c_str());
    GLint success;
    char infoLog[512];
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      throw std::runtime_error(infoLog);
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return shaderProgram;
  }

  void register_input(int key, int action, void (*handler)(GLFWwindow * window)) {
    if(glfwGetKey(window, key) == action)
      handler(window);
  }
  std::string read_shader(char const * shaderfilename) {
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
};

int main() {
  GLApp app;
  return 0;
}