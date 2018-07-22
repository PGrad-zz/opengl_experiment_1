#pragma once

#include <vector>

struct Model {
  std::vector<float> vertices;
  std::vector<unsigned int> indices;
  Model(std::initializer_list<float> vertices, std::initializer_list<unsigned int> indices);
};
struct Model get_triangle();
struct Model get_square();
