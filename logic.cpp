#include "logic.h"
#include "geometry.h"
Logic::Logic() {}

void Logic::HandlePress(Vertice clicked) {
  if (HandlingSubfunc(clicked)) {
  }
  for (const auto& x : adjacency_list_) {
    for (const auto& d : x->connected) {
      if (Geometry::DistanceToSegment(d->vertice, x->vertice, clicked) <= 5) {
        selected_edges_.push_back({d, x});
      }
    }
  }
}

void Logic::HandleRelease(Vertice clicked) {}

bool Logic::HandlingSubfunc(Vertice clicked) {
  bool changed = false;
  for (const auto& vl : adjacency_list_) {
    if (std::abs(vl->vertice.x() - clicked.x()) <= vertice_radius_ &&
        std::abs(vl->vertice.y() - clicked.y()) <= vertice_radius_) {
      // Select Vertice and check cases

      for (auto a = selected_.begin(); a != selected_.end(); a++) {
        if ((*a) == vl) {
          if ((*a)->is_active) this->selected_.erase(a);
        }
        changed = true;
        break;
      }
      if (!changed) {
        this->selected_.push_back(vl);
      }
      return true;
    }
  }
  return false;
}

void Logic::HandleDelete() {
  for (const auto& x : this->selected_) {
    x->connected.clear();
    for (const auto& t : adjacency_list_) {
      for (auto a = t->connected.begin(); a != t->connected.end(); a++) {
        if ((*a.base()) == x) {
          t->connected.erase(a);
        }
      }
    }
  }
}

void Logic::NewVertice(Vertice clicked) {}
