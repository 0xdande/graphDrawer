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
  Q_INVOKABLE Vertex(int id, Vertice vertice, bool is_active,
                     std::vector<Vertex*> connected)
      : id_(id),
        vertice_(vertice),
        is_active_(is_active),
        connected_(connected) {}
  Q_INVOKABLE Vertex() {
    id_ = 0;
    vertice_ = Vertice{0, 0};
    is_active_ = false;
    connected_ = {};
  }
  uint8_t id_;
  Vertice vertice_;
  bool is_active_;
  std::vector<Vertex*> connected_;
  Q_INVOKABLE int id();
  Q_INVOKABLE void SetID(int id);
  Q_INVOKABLE Vertice vertice();
  Q_INVOKABLE void SetVertice(Vertice vertice);
  Q_INVOKABLE int is_active();
  Q_INVOKABLE void SetActive(bool is_active);
  Q_INVOKABLE std::vector<Vertex*> connected() { return connected_; }
  Q_INVOKABLE void SetConnected(std::vector<Vertex*> s) {
    connected_ = s;
    qDebug() << "wtf";
  }
};
Q_DECLARE_METATYPE(Vertex)
Q_DECLARE_METATYPE(Vertice)
using VertexPointer = Vertex*;
Q_DECLARE_METATYPE(VertexPointer)

#endif  // VERTEX_H
