#ifndef VERTEX_H
#define VERTEX_H
#include <qobject.h>
#include <cstdint>
#include <vector>
#include "vertice.h"
class Vertex {
  Q_GADGET
  Q_PROPERTY(int id_ READ id WRITE SetID)
  Q_PROPERTY(int vertice_ READ vertice WRITE SetVertice)
  Q_PROPERTY(int is_active_ READ is_active WRITE SetActive)
 public:
  uint8_t id_;
  Vertice vertice_;
  bool is_active_;
  std::vector<Vertex*> connected;
  int id();
  void SetID(int id);
  Vertice vertice();
  void SetVertice(Vertice vertice);
  int is_active();
  void SetActive(bool is_active);
};
Q_DECLARE_METATYPE(Vertex)
Q_DECLARE_METATYPE(std::vector<Vertex*>)
#endif  // VERTEX_H
