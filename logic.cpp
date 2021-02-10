#include "logic.h"

#include <qdebug.h>

#include <iostream>

#include "geometry.h"
Logic::Logic() {}

void Logic::HandleClick(int a, int b) {
  qDebug() << "----Adjacency List Start----";
  for (const auto &x : adjacency_list_) {
    qDebug() << x->id() << x->vertice().x() << x->vertice().y();
  }
  qDebug() << "----Adjacency List End----";
  if (!last_hold) {
    auto clicked = NewVertice(a, b);
    if (HandlingSubfunc(clicked)) {
      return;
    }
    for (const auto &x : adjacency_list_) {
      for (const auto &d : x->connected_) {
        qDebug() << Geometry::DistanceToSegment(d.first->vertice(),
                                                x->vertice(), clicked);
        if (Geometry::DistanceToSegment(d.first->vertice(), x->vertice(),
                                        clicked) <= 10) {
          qDebug() << "Worked";
          if ((std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{d.first, x}) ==
               selected_edges_.end()) &&
              (std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{x, d.first}) ==
               selected_edges_.end())) {
            selected_edges_.push_back({d.first, x});
            selected_edges_.push_back({x, d.first});
            auto res = std::find(d.first->connected_.begin(),
                                 d.first->connected_.end(),
                                 std::pair<Vertex *, bool>{x, true});
          }
        } else {
          selected_edges_.erase(
              std::find(selected_edges_.begin(), selected_edges_.end(),
                        std::pair<Vertex *, Vertex *>{d.first, x}));
          selected_edges_.erase(
              std::find(selected_edges_.begin(), selected_edges_.end(),
                        std::pair<Vertex *, Vertex *>{x, d.first}));
        }
        return;
      }
    }
    auto a = new Vertex(adjacency_list_.size() + 1, clicked, false, {});
    this->adjacency_list_.push_back(a);
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
      if (std::abs(vl->vertice().x() - clicked.x()) <= vertice_radius_ &&
          std::abs(vl->vertice().y() - clicked.y()) <= vertice_radius_) {
        for (auto a = selected_.begin(); a != selected_.end(); a++) {
          if ((*a) == vl) {
            if ((*a)->is_active()) {
              this->selected_.erase(a);
              vl->SetActive(false);
              changed = true;
              return true;
            }
          }
        }
        if (!changed) {
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
  for (const auto &d : selected_) {
    adjacency_list_.erase(adjacency_list_.begin() + d->id() - 1);

    for (size_t i = d->id() - 1; i < adjacency_list_.size(); i++) {
      adjacency_list_[i]->id_--;
    }

    d->connected_.clear();

    for (const auto &t : adjacency_list_) {
      t->connected_.erase(
          std::remove_if(t->connected_.begin(), t->connected_.end(),
                         [&](std::pair<Vertex *, bool> el) {
                           return el.first->id() == d->id();
                         }),
          t->connected_.end());
    }
  }
  for (const auto &x : selected_) x->SetActive(false);
  this->selected_.clear();
}

void Logic::HandleDeleteAll() {
  for (int i = 0; i < adjacency_list_.size(); i++) {
    if (std::find(selected_.begin(), selected_.end(), adjacency_list_[i]) ==
        selected_.end())
      selected_.push_back(adjacency_list_[i]);
  }
  HandleDelete();
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

bool Logic::HandleConnection() {
  if (selected_.size() != 2) {
    for (const auto &x : selected_) x->SetActive(false);
    selected_.clear();
    return false;
  } else {
    if ((std::find(selected_[0]->connected_.begin(),
                   selected_[0]->connected_.end(),
                   std::pair<Vertex *, bool>{selected_[1], false}) ==
         selected_[0]->connected_.end()) &&
        (std::find(selected_[0]->connected_.begin(),
                   selected_[0]->connected_.end(),
                   std::pair<Vertex *, bool>{selected_[1], false}) ==
         selected_[0]->connected_.end())) {
      selected_[0]->connected_.push_back({selected_[1], false});
      selected_[1]->connected_.push_back({selected_[0], false});
      for (const auto &x : selected_) x->SetActive(false);
      selected_.clear();
      return true;
    } else {
      for (const auto &x : selected_) x->SetActive(false);
      selected_.clear();
      return false;
    }
  }
}

std::vector<QVector4D> Logic::DrawVerticesAPI() {
  std::vector<QVector4D> to_draw;
  for (const auto &x : adjacency_list()) {
    to_draw.push_back(
        QVector4D{x->id(), x->is_active_, x->vertice().x(), x->vertice().y()});
  }
  return to_draw;
}

std::vector<QVector4D> Logic::DrawEdgesAPI() {
  std::vector<QVector4D> to_draw;
  for (const auto &x : adjacency_list_) {
    for (const auto &y : x->connected()) {
      to_draw.push_back(QVector4D{x->vertice().x(), x->vertice().y(),
                                  y.first->vertice().x(),
                                  y.first->vertice().y()});
    }
  }
  return to_draw;
}
