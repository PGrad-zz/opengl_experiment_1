#include "objects.h"
Model::Model(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices) {
  this->vertices = std::vector<float>(vertices.begin(), vertices.end());
  this->indices = std::vector<unsigned int>(indices.begin(), indices.end());
}
struct Model get_triangle() {
  return Model({
    .5, -.5, 0.,
    1., -.5, 0.,
    .75,  .5, 0.
  }, {
    0, 1, 2
  });
}
struct Model get_2nd_triangle() {
  return Model({
    -.5, -.5, 0.,
    .5, -.5, 0.,
    0.,  .5, 0.
  }, {});
}

struct Model get_square() {
  return Model({
    -.5, -.5, 0.,
    .5, -.5, 0.,
    .5, .5, 0.,
    -.5, .5, 0.
  }, {
    0, 1, 3,
    1, 2, 3
  });
}
