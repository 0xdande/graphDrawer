#include "logic.h"

#include <qdebug.h>

#include <iostream>

#include "geometry.h"
Logic::Logic() {}

void Logic::HandleClick(int a, int b) {
  qDebug() << a << ' ' << b;
  qDebug() << "Worked";
  if (!last_hold) {
    auto clicked = NewVertice(a, b);
    if (HandlingSubfunc(clicked)) {
      qDebug() << "Popalos v vershinu";
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
    qDebug() << "Adjacency List Size: " << adjacency_list_.size();
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
    if (std::abs(vl->vertice().x() - clicked.x()) <= 2 * vertice_radius_ &&
        std::abs(vl->vertice().y() - clicked.y()) <= 2 * vertice_radius_) {
      // Select Vertice and check cases
      if (std::abs(vl->vertice().x() - clicked.x()) <= vertice_radius_ &&
          std::abs(vl->vertice().y() - clicked.y()) <= vertice_radius_) {
        qDebug() << vl->vertice().x();
        qDebug() << vl->vertice().y();
        qDebug() << clicked.x();
        qDebug() << clicked.y();
        for (auto a = selected_.begin(); a != selected_.end(); a++) {
          if ((*a) == vl) {
            if ((*a)->is_active()) {
              this->selected_.erase(a);
              (*a)->SetActive(false);
              changed = true;
              break;
            }
          }
        }
        if (!changed) {
          qDebug() << selected_.size();
          vl->SetActive(true);
          this->selected_.push_back(vl);
        }
      }
      return true;
    }
  }
  return false;
}

void Logic::HandleDelete() {
  for (int i = 0; i < this->selected_.size(); i++) {
    for (int j = selected_[i]->id(); j < adjacency_list().size(); j++) {
      adjacency_list()[j]->id_--;
    }
    selected_[i]->connected_.clear();
    for (const auto &t : adjacency_list_) {
      for (auto a = t->connected_.begin(); a != t->connected_.end(); a++) {
        if ((*a) == selected_[i]) {
          t->connected_.erase(a);
          //          for (; a != t->connected_.end(); a++) {
          //            (*a)->id_--;
          //          }
        }
      }
    }
    qDebug() << this->selected_[i]->id();
    qDebug() << "id: " << this->selected_[i]->id() - 1
             << "size: " << adjacency_list().size();
    qDebug() << adjacency_list();
    adjacency_list_.erase(adjacency_list_.begin() + this->selected_[i]->id() -
                          1);
    qDebug() << adjacency_list();
  }
  this->selected_.clear();
}

Vertice Logic::NewVertice(int x, int y) { return Vertice{x, y}; }

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

std::vector<QVector4D> Logic::DrawVerticesAPI() {
  std::vector<QVector4D> to_draw;
  for (const auto &x : adjacency_list()) {
    to_draw.push_back(
        QVector4D{x->id(), x->is_active_, x->vertice().x(), x->vertice().y()});
  }
  qDebug() << to_draw;
  return to_draw;
}
