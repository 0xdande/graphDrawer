#include "vertex.h"
int Vertex::id() { return id_; }
void Vertex::SetID(int id) { id_ = id; }
Vertice Vertex::vertice() { return vertice_; }
void Vertex::SetVertice(Vertice vertice) { vertice_ = vertice; }
int Vertex::is_active() { return is_active_; }
void Vertex::SetActive(bool is_active) { is_active_ = is_active; }
