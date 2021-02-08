#ifndef VERTEX_H
#define VERTEX_H
#include <qdebug.h>
#include <qobject.h>

#include <cstdint>
#include <vector>

#include "vertice.h"
class Vertex {
  Q_GADGET
  Q_PROPERTY(int id_ READ id WRITE SetID)
  Q_PROPERTY(Vertice vertice_ READ vertice WRITE SetVertice)
  Q_PROPERTY(int is_active_ READ is_active WRITE SetActive)
  Q_PROPERTY(std::vector<Vertex*> connected_ READ connected WRITE SetConnected)
 public:
  Vertex(int id, Vertice vertice, bool is_active,
         std::vector<Vertex*> connected)
      : id_(id),
        vertice_(vertice),
        is_active_(is_active),
        connected_(connected){};
  uint8_t id_;
  Vertice vertice_;
  bool is_active_;
  std::vector<Vertex*> connected_;
  int id();
  void SetID(int id);
  Vertice vertice();
  void SetVertice(Vertice vertice);
  int is_active();
  void SetActive(bool is_active);
  std::vector<Vertex*> connected() { return connected_; }
  void SetConnected(std::vector<Vertex*> s) {
    connected_ = s;
    qDebug() << "wtf";
  }
};
Q_DECLARE_METATYPE(Vertex)
Q_DECLARE_METATYPE(Vertice)
Q_DECLARE_METATYPE(std::vector<Vertex*>)
#endif  // VERTEX_H
