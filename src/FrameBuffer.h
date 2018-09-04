class Shader;
class FrameBuffer {
private:
  GLuint framebuf;
  GLuint renderTex;
public:
  FrameBuffer();
  bool loadRenderTex(int const width, int const height);
  GLuint getFramebuf();
  GLuint getRenderTex();
};
