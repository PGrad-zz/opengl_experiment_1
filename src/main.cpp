#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include "objects.h"
#include "Model.h"
#include "Shader.h"

typedef unsigned int VBOid;
typedef unsigned int Shaderid;

class GLApp {
public:
  GLApp() {
    try { 
      glfwSetup();
      initWindow();
      initGLAD();
      model[3][3] = 1;
      model = glm::translate(model, glm::vec3(0., 0., 0.));
      view = glm::lookAt(glm::vec3(0., 20., 10.),
                         glm::vec3(0., 10., 0.),
                         glm::vec3(0., 1., 0.));
      projection = glm::perspective(glm::radians(60.f), ((float) screenWidth) / screenHeight, .1f, 100.f);
      Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
      shader.use();
      Model crysis_model("models/nanosuit.obj");
      mainloop(crysis_model, shader);
    } catch(std::runtime_error e) {
      std::cerr << e.what() << std::endl;
    }
  }
  ~GLApp() {
    glfwTerminate();
  }
private:
  GLFWwindow * window;
  old::Model tri_model = old::get_triangle();
  old::Model square_model = old::get_square();
  glm::mat4 model;
  glm::mat4 view;
  glm::mat4 projection;
  int const screenWidth = 800, screenHeight = 600;
  
  void glfwSetup() {
    if(!glfwInit())
      throw std::runtime_error("Failed to init GLFW.");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  }
  
  void initWindow() {
    window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
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

  unsigned int setupVAO(old::Model model) {
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

  void setUniforms(Shader const & shader) {
      glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(glGetUniformLocation(shader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(glGetUniformLocation(shader.ID, "proj"), 1, GL_FALSE, glm::value_ptr(projection)); 
  }

  void mainloop(Model & model, Shader & shader) {
    while(!glfwWindowShouldClose(window)) {
      register_input(GLFW_KEY_ESCAPE, GLFW_PRESS,
        [] (GLFWwindow * window) {
          glfwSetWindowShouldClose(window, true);
      });
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);

      setUniforms(shader);
      glUseProgram(shader.ID);
      model.Draw(shader);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  }

  void register_input(int key, int action, void (*handler)(GLFWwindow * window)) {
    if(glfwGetKey(window, key) == action)
      handler(window);
  }
};

int main() {
  GLApp app;
  return 0;
}
