#include "vertex.h"
Vertex::Vertex(int id, Vertice vertice, bool is_active,
               std::vector<std::pair<Vertex *, int>> connected)
    : id_(id),
      vertice_(vertice),
      is_active_(is_active),
      connected_(connected) {}

Vertex::Vertex() {
  id_ = 0;
  vertice_ = Vertice{0, 0};
  is_active_ = false;
  connected_ = {};
}

int Vertex::id() { return id_; }
void Vertex::SetID(int id) { id_ = id; }
Vertice Vertex::vertice() { return vertice_; }
void Vertex::SetVertice(Vertice vertice) { vertice_ = vertice; }
int Vertex::is_active() { return is_active_; }
void Vertex::SetActive(bool is_active) { is_active_ = is_active; }
