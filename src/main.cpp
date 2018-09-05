#include "glad.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <iostream>
#include <fstream>
#include <typeinfo>
#include <memory>
#include "objects.h"
#include "Model.h"
#include "Shader.h"
#include "FrameBuffer.h"
using std::cout;
using std::endl;

typedef unsigned int VBOid;
typedef unsigned int Shaderid;

class GLApp {
public:
  GLApp() {
    try { 
      glfwSetup();
      initWindow();
      initGLAD();
      framebuf = std::make_unique<FrameBuffer>();
      if(!framebuf->loadRenderTex(screenWidth, screenHeight))
        throw std::runtime_error(std::to_string(glCheckFramebufferStatus(GL_FRAMEBUFFER)));
      model[3][3] = 1;
      model = glm::translate(model, glm::vec3(0., 0., 0.));
      camera = glm::vec3(13.f);
      view = defaultViewMatrix();
      projection = glm::perspective(glm::radians(60.f), ((float) screenWidth) / screenHeight, .1f, 100.f);
      light = glm::vec3(0.f, 50.f, 20.f);
      Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
      Shader rendertex_shader("shaders/vertex.glsl", "shaders/fragment_rendertex.glsl");
      load_foreground();
      mainloop(square_model, foreground, shader, rendertex_shader);
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
  glm::vec3 light;
  glm::vec3 camera;
  int const screenWidth = 800, screenHeight = 600;
  std::vector<Model> foreground;
  std::unique_ptr<FrameBuffer> framebuf;

  glm::mat4 defaultViewMatrix() {
      return glm::lookAt(camera,
                         glm::vec3(1., 0., 0.),
                         glm::vec3(0., 1., 0.));
  }

  void load_foreground() {
    if(foreground.size() != 0)
      return; //Nothing to load
    foreground.emplace_back("models/nanosuit/nanosuit.obj");
  }
  
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
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("proj", projection);
    shader.setVec3("lightPos", light);
    shader.setVec3("eye", camera);
  }

  void drawModels(std::vector<Model> & foreground, Shader & shader) {
      shader.setInt("ground", 0);
      for(Model & model : foreground) {
          setUniforms(shader);
          model.Draw(shader);
      }
  }

  void mainloop(old::Model & ground, std::vector<Model> & foreground, Shader & shader, Shader & rendertex_shader) {
   glm::vec3 savecam;
    while(!glfwWindowShouldClose(window)) {
      register_input(GLFW_KEY_ESCAPE, GLFW_PRESS,
        [] (GLFWwindow * window) {
          glfwSetWindowShouldClose(window, true);
      });

      glBindFramebuffer(GL_FRAMEBUFFER, framebuf->getFramebuf());
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glDisable(GL_DEPTH_TEST);
      rendertex_shader.use();
      savecam = camera;
      camera = light;
      view = defaultViewMatrix();
      
      drawModels(foreground, rendertex_shader);
      
      glBindFramebuffer(GL_FRAMEBUFFER, 0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glEnable(GL_DEPTH_TEST);
      shader.use();
      glActiveTexture(GL_TEXTURE0);
      shader.setInt("framebuf", 0);
      glBindTexture(GL_TEXTURE_2D, framebuf->getRenderTex());
      camera = savecam;
      view = defaultViewMatrix();

      shader.setInt("ground", 1);
      setUniforms(shader);
      setupVAO(ground);
      glDrawElements(GL_TRIANGLES, ground.indices.size(), GL_UNSIGNED_INT, 0);
      glBindVertexArray(0);

      drawModels(foreground, shader);

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
