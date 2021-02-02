#include "logic.h"
#include "geometry.h"
Logic::Logic() {}

void Logic::HandlePress(Vertice clicked) {
  for (const auto& vl : adjacency_list_) {
    if (std::abs(vl->x() - clicked.x()) <= vertice_radius_ &&
        std::abs(vl->y() - clicked.y()) <= vertice_radius_) {
      // Select Vertice and check cases
    } else if (Geometry::DistanceToLine(clicked,
                                        Geometry::LineEquation(*vl, clicked))) {
      // Select edge and (delete?)
    } else {
    }
  }
}

void Logic::HandleRelease(uint8_t id) {}

void Logic::HandleDelete(uint8_t id) {}

void Logic::NewVertice(Vertice clicked) {}
