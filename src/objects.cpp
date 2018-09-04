#include "objects.h"
old::Model::Model(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices) {
  this->vertices = std::vector<float>(vertices.begin(), vertices.end());
  this->indices = std::vector<unsigned int>(indices.begin(), indices.end());
}
struct old::Model old::get_triangle() {
  return old::Model({
    .5, -.5, 0.,
    1., -.5, 0.,
    .75,  .5, 0.
  }, {
    0, 1, 2
  });
}
struct old::Model old::get_2nd_triangle() {
  return old::Model({
    -.5, -.5, 0.,
    .5, -.5, 0.,
    0.,  .5, 0.
  }, {});
}

struct old::Model old::get_square() {
  return old::Model({
    -50., 0., -50.,
    50., 0., -50.,
    50., 0., 50.,
    -50., 0., 50.
  }, {
    0, 1, 3,
    1, 2, 3
  });
}
