#include "logic.h"

#include <qdebug.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>

#include "geometry.h"
#include "util.hpp"
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
bool Logic::HandlingSubfunc(Vertice clicked) {
  bool changed = false;
  for (const auto &vl : adjacency_list_) {
    if (std::abs(vl->vertice().x() - clicked.x()) <= 1.5 * vertice_radius_ &&
        std::abs(vl->vertice().y() - clicked.y()) <= 1.5 * vertice_radius_) {
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
    delete d;
  }
  this->selected_.clear();
}

void Logic::HandleDeleteEdge() {
  std::for_each(selected_edges_.begin(), selected_edges_.end(),
                [](std::pair<Vertex *, Vertex *> el) {
                  auto a = std::find(el.first->connected_.begin(),
                                     el.first->connected_.end(), el.second);
                  if (a != el.first->connected_.end()) {
                    el.first->connected_.erase(a);
                  }
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
    to_draw.push_back(QVector4D{static_cast<float>(x->id()),
                                static_cast<float>(x->is_active_),
                                static_cast<float>(x->vertice().x()),
                                static_cast<float>(x->vertice().y())});
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
        to_draw.push_back(QVector4D{static_cast<float>(-x->vertice().x()),
                                    static_cast<float>(-x->vertice().y()),
                                    static_cast<float>(-y->vertice().x()),
                                    static_cast<float>(-y->vertice().y())});
      } else {
        to_draw.push_back(QVector4D{static_cast<float>(x->vertice().x()),
                                    static_cast<float>(x->vertice().y()),
                                    static_cast<float>(y->vertice().x()),
                                    static_cast<float>(y->vertice().y())});
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
        if (!el->connected_.empty()) {
          std::for_each(el->connected_.begin(), el->connected_.end() - 1,
                        [&](Vertex *el) { file << el->id() - 1 << ','; });
          file << el->connected_[el->connected_.size() - 1]->id() - 1 << '}'
               << '}' << ',';
        }
      });

  auto el = adjacency_list_[adjacency_list_.size() - 1];
  file << '{' << el->id_ - 1 << ',' << '{' << el->vertice_.x() << ','
       << el->vertice_.y() << '}' << ',' << '{';
  if (el->connected_.size() != 0) {
    std::for_each(el->connected_.begin(), el->connected_.end() - 1,
                  [&](Vertex *el) { file << el->id() - 1 << ','; });
    file << el->connected_[el->connected_.size() - 1]->id() - 1 << '}';
  }
  file << '}';
  file.close();
}
// ERROR WHILE SEEKING EMPTY VECTOR TO BE FIXED
void Logic::Deserialize(QString filepath) {
  HandleDeleteAll();
  std::ifstream file;
  std::vector<std::vector<int>> constructed;
  file.open(filepath.toStdString(), std::ios::in);
  char ch;
  int cr;
  util::ReadFileData(file, cr, 1);
  for (int i = 0; i < cr; i++) {
    int id, x, y;
    util::ReadFileData(file, 1, id, 2, x, 1, y, 2);
    std::string bytes;
    while ((util::ReadFileData(file, ch), true) && ch != '}' && ch != EOF) {
      if (ch == '{') continue;
      bytes += ch;
    }
    auto to_push = new Vertex{
        id + 1,
        Vertice{x, y},
        false,
        std::vector<Vertex *>{},
    };
    util::ReadFileData(file, 2);
    adjacency_list_.push_back(to_push);
    constructed.push_back(util::BytesToInts(bytes));
  }

  qDebug() << constructed;
  for (int i = 0; i < constructed.size(); i++) {
    for (int j = 0; j < constructed[i].size(); j++) {
      qDebug() << constructed[i][j];
      adjacency_list_[i]->connected_.push_back(
          adjacency_list_[constructed[i][j]]);  // adjacency_list_[j]);
    }
  }
  file.close();
}

void Logic::SetIDByCoords(int x, int y) {
  for (const auto &vl : adjacency_list_) {
    qDebug() << std::abs(vl->vertice().x() - x);
    qDebug() << std::abs(vl->vertice().y() - y);
    if (std::abs(vl->vertice().x() - x) <= 1.5 * vertice_radius_ &&
        std::abs(vl->vertice().y() - y) <= 1.5 * vertice_radius_) {
      for (const auto &d : adjacency_list_) {
        if (std::abs(d->vertice().x() - x) <= 1.5 * vertice_radius_ &&
            std::abs(d->vertice().y() - y) <= 1.5 * vertice_radius_ && d != vl)
          return;
      }
      vl->vertice_.SetX(std::move(x));
      vl->vertice_.SetY(std::move(y));
    }
  }
}

std::vector<Vertex *> Logic::CopyGraph() {}

void Logic::DFS(int s) {
  std::vector<bool> visited(adjacency_list_.size() + 1, false);
  std::stack<int> stack;
  stack.push(s);

  while (!stack.empty()) {
    s = stack.top();
    stack.pop();
    if (!visited[s - 1]) {
      visited[s - 1] = true;
    }
    for (const auto &x : adjacency_list_[s - 1]->connected_)
      if (!visited[x->id()]) stack.push(x->id());
  }
}

void Logic::EulersPath(std::vector<Vertex *> tmp_adjacency) {
  std::vector<int> vertices;
  int odds = 0;
  int s = 0;
  for (const auto &x : tmp_adjacency) {
    if (x->connected_.size() % 2) {
      s = x->id();
      odds++;
    }
  }

  if (odds != 2) {
    exit(0xdeadbeef);
  }

  std::stack<int> stack;
  stack.push(s);

  while (!stack.empty()) {
    s = stack.top();
    stack.pop();
    if (!tmp_adjacency[s - 1]->connected_.empty()) {
      for (const auto &x : tmp_adjacency[s - 1]->connected_) {
        stack.push(x->id());
        vertices.push_back(x->id());
        tmp_adjacency.erase(tmp_adjacency.begin() + x->id() - 1);
      }
    }
  }
}

/* 				Style of Serializing
 * {{id,{x,y},is_active,{id.'s of connected}},{id,{x,y},is_active,
 * {id.'s of connected}}...}
 * 4{{0,{291,93},{1,2,3}},{1,{126,299},{2,0,3}},{2,{391,461},{3,1,0}},{3,{129,619},{2,1,0}}
 *
 *
 */
