#include "logic.h"

#include <qdebug.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include "geometry.h"
Logic::Logic() {}

void Logic::HandleClick(int a, int b) {
  if (!last_hold) {
    auto clicked = NewVertice(a, b);
    if (HandlingSubfunc(clicked)) {
      return;
    }
    for (const auto &x : adjacency_list_) {
      for (auto &d : x->connected_) {
        if (Geometry::DistanceToSegment(d->vertice(), x->vertice(), clicked) <=
            10) {
          if ((std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{d, x}) ==
               selected_edges_.end()) &&
              (std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{x, d}) ==
               selected_edges_.end())) {
            selected_edges_.push_back({d, x});
            selected_edges_.push_back({x, d});
          } else {
            selected_edges_.erase(
                std::find(selected_edges_.begin(), selected_edges_.end(),
                          std::pair<Vertex *, Vertex *>{d, x}));
            selected_edges_.erase(
                std::find(selected_edges_.begin(), selected_edges_.end(),
                          std::pair<Vertex *, Vertex *>{x, d}));
          }
          return;
        }
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
                         [&](Vertex *el) { return el->id() == d->id(); }),
          t->connected_.end());
    }
  }
  for (const auto &x : selected_) x->SetActive(false);
  this->selected_.clear();
}

void Logic::HandleDeleteEdge() {
  std::for_each(selected_edges_.begin(), selected_edges_.end(),
                [](std::pair<Vertex *, Vertex *> el) {
                  el.first->connected_.erase(
                      std::find(el.first->connected_.begin(),
                                el.first->connected_.end(), el.second));
                });
  selected_edges_.clear();
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
                   selected_[1]) == selected_[0]->connected_.end()) &&
        (std::find(selected_[1]->connected_.begin(),
                   selected_[1]->connected_.end(),
                   selected_[0]) == selected_[1]->connected_.end())) {
      selected_[0]->connected_.push_back(selected_[1]);
      selected_[1]->connected_.push_back(selected_[0]);
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
      if (std::find(selected_edges_.begin(), selected_edges_.end(),
                    std::pair<Vertex *, Vertex *>{x, y}) !=
          selected_edges_.end()) {
        to_draw.push_back(QVector4D{-x->vertice().x(), -x->vertice().y(),
                                    -y->vertice().x(), -y->vertice().y()});
      } else {
        to_draw.push_back(QVector4D{x->vertice().x(), x->vertice().y(),
                                    y->vertice().x(), y->vertice().y()});
      }
    }
  }
  return to_draw;
}

void Logic::Serialize(QString filepath) {
  if (adjacency_list_.size() == 0) {
    return;
  }
  std::ofstream file;
  file.open(filepath.toStdString());
  file << adjacency_list_.size();
  file << '{';
  std::for_each(
      adjacency_list_.begin(), adjacency_list_.end() - 1, [&](Vertex *el) {
        file << '{' << el->id_ - 1 << ',' << '{' << el->vertice_.x() << ','
             << el->vertice_.y() << '}' << ',' << '{';
        std::for_each(el->connected_.begin(), el->connected_.end() - 1,
                      [&](Vertex *el) { file << el->id() - 1 << ','; });
        file << el->connected_[el->connected_.size() - 1]->id() - 1 << '}'
             << '}' << ',';
      });
  auto el = adjacency_list_[adjacency_list_.size() - 1];
  file << '{' << el->id_ - 1 << ',' << '{' << el->vertice_.x() << ','
       << el->vertice_.y() << '}' << ',' << '{';
  std::for_each(el->connected_.begin(), el->connected_.end() - 1,
                [&](Vertex *el) { file << el->id() - 1 << ','; });
  file << el->connected_[el->connected_.size() - 1]->id() - 1 << '}' << '}';
  file.close();
}
void Logic::Deserialize(QString filepath) {
  HandleDeleteAll();
  std::ifstream file;
  std::vector<std::vector<int>> constructed;
  file.open(filepath.toStdString(), std::ios::in);
  int cr;
  file >> cr;
  qDebug() << cr;
  char ch;
  file.ignore(1);
  for (int i = 0; i < cr; i++) {
    file.ignore(1);
    int id;
    file >> id;
    qDebug() << id;
    file.ignore(2);
    int x, y;
    file >> x;
    file.ignore(1);
    file >> y;
    qDebug() << x << y;
    file.ignore(2);
    std::string bytes;
    while (file.get(ch) && ch != '}') {
      if (ch == '{') continue;
      bytes += ch;
    }
    std::vector<int> adjacency;
    if (bytes.size()) {
      std::stringstream ss(bytes);

      for (int i; ss >> i;) {
        adjacency.push_back(i + 1);
        if (ss.peek() == ',') ss.ignore();
      }
    }

    auto to_push = new Vertex{
        id + 1,
        Vertice{x, y},
        false,
        std::vector<Vertex *>{},
    };
    file.ignore(2);
    adjacency_list_.push_back(to_push);
    constructed.push_back(adjacency);
  }

  for (int i = 0; i < constructed.size(); i++) {
    for (int j = 0; j < constructed[i].size(); j++) {
      adjacency_list_[i]->connected_.push_back(adjacency_list_[j]);
    }
  }
  file.close();
}

/* 				Style of Serializing
 * {{id,{x,y},is_active,{id.'s of connected}},{id,{x,y},is_active,
 * {id.'s of connected}}...}
 * 4{{0,{291,93},{1,2,3}},{1,{126,299},{2,0,3}},{2,{391,461},{3,1,0}},{3,{129,619},{2,1,0}}
 *
 *
 */
