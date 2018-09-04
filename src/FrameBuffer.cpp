#include "glad.h"
#include <GLFW/glfw3.h>
#include "FrameBuffer.h"
#include "Shader.h"
#include <iostream>
FrameBuffer::FrameBuffer()
  : framebuf(0), renderTex(0) {
  glGenFramebuffers(1, &framebuf);
  glGenTextures(1, &renderTex);
}

bool FrameBuffer::loadRenderTex(int const width, int const height) {
  glBindFramebuffer(GL_FRAMEBUFFER, framebuf);
  glBindTexture(GL_TEXTURE_2D, renderTex);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  //Bind the texture to COLOR_ATTACHMENT0
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, renderTex, 0);
  
  GLenum drawBuffers[1] = {GL_COLOR_ATTACHMENT0};
  //The first buffer bound to the fragment shader is COLOR_ATTACHMENT0
  glDrawBuffers(1, drawBuffers);
  
  return glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}

GLuint FrameBuffer::getFramebuf() {
  return framebuf;
}

GLuint FrameBuffer::getRenderTex() {
  return renderTex;
}
