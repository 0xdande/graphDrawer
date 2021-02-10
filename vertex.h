#ifndef VERTEX_H
#define VERTEX_H
#include <qdebug.h>
#include <qobject.h>

#include <cstdint>
#include <vector>

#include "vertice.h"
class Vertex {
public:
  Vertex(int id, Vertice vertice, bool is_active,
         std::vector<Vertex *> connected);
  Vertex();

  int id();
  void SetID(int id);
  Vertice vertice();
  void SetVertice(Vertice vertice);
  int is_active();
  void SetActive(bool is_active);
  std::vector<Vertex *> connected() { return connected_; }

  uint8_t id_;
  Vertice vertice_;
  bool is_active_;
  std::vector<Vertex *> connected_;
};

#endif // VERTEX_H
