#include "logic.h"

Logic::Logic() {}

void Logic::HandlePress(Vertice clicked) {
  for (const auto& vl : adjacency_list_) {
    if (std::abs(vl->x() - clicked.x()) <= vertice_radius_ &&
        std::abs(vl->y() - clicked.y()) <= vertice_radius_) {
    }
    if (true) {
    }
  }
}

void Logic::HandleRelease(uint8_t id) {}

void Logic::HandleDelete(uint8_t id) {}
