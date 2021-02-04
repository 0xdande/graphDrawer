#ifndef VERTEX_H
#define VERTEX_H
#include <cstdint>
#include <vector>
#include "vertice.h"
class Vertex {
 public:
  Vertex() = default;
  uint8_t id;
  Vertice vertice;
  bool is_active;
  std::vector<Vertex*> connected;
};

#endif  // VERTEX_H
