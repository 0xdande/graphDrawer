#include "logic.h"

#include <qdebug.h>

#include <iostream>

#include "geometry.h"
Logic::Logic() {}

void Logic::HandleClick(int a, int b) {
  qDebug() << a << ' ' << b;
  if (!last_hold) {
    auto clicked = NewVertice(a, b);
    if (HandlingSubfunc(clicked)) {
      return;
    }
    for (const auto &x : adjacency_list_) {
      for (const auto &d : x->connected_) {
        if (Geometry::DistanceToSegment(d->vertice(), x->vertice(), clicked) <=
            5) {
          selected_edges_.push_back({d, x});
          return;
        }
      }
    }
    auto a = new Vertex(adjacency_list_.size() + 1, clicked, false, {});
    this->adjacency_list_.push_back(a);
    qDebug() << adjacency_list_.size();
  }
}

void Logic::HandleRelease() {
  if (last_hold == true) {
    last_hold = false;
    holded_id = -1;
  }
}

bool Logic::HandlingSubfunc(Vertice clicked) {
  bool changed = false;
  for (const auto &vl : adjacency_list_) {
    if (std::abs(vl->vertice().x() - clicked.x()) <= vertice_radius_ &&
        std::abs(vl->vertice().y() - clicked.y()) <= vertice_radius_) {
      // Select Vertice and check cases

      for (auto a = selected_.begin(); a != selected_.end(); a++) {
        if ((*a) == vl) {
          if ((*a)->is_active()) this->selected_.erase(a);
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
  for (const auto &x : this->selected_) {
    x->connected_.clear();
    for (const auto &t : adjacency_list_) {
      for (auto a = t->connected_.begin(); a != t->connected_.end(); a++) {
        if ((*a.base()) == x) {
          t->connected_.erase(a);
          for (; a != t->connected_.end(); a++) {
            (*a)->id_--;
          }
        }
      }
      delete x;
    }
  }
}

Vertice Logic::NewVertice(uint8_t x, uint8_t y) { return Vertice{x, y}; }

void Logic::HandleHold(int a, int b) {
  auto clicked = NewVertice(a, b);
  bool changed = false;
  for (const auto &vl : adjacency_list_) {
    if (std::abs(vl->vertice().x() - clicked.x()) <= vertice_radius_ &&
        std::abs(vl->vertice().y() - clicked.y()) <= vertice_radius_) {
      // Select Vertice and check cases
      last_hold = true;
      holded_id = vl->id();
    }
  }
}
