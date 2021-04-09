#include "logic.h"

#include <qdebug.h>

#include <QMatrix4x4>
#include <QRandomGenerator>
#include <chrono>
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <stack>
#include <thread>

#include "geometry.h"
#include "util.hpp"
Logic::Logic() {}

QVector3D Logic::HandleClick(int a, int b) {
  if (!last_hold) {
    auto clicked = NewVertice(a, b);
    if (HandlingSubfunc(clicked)) {
      return {0, 0, 0};
    }
    for (const auto &x : adjacency_list_) {
      for (auto &d : x->connected_) {
        auto xfrom = x->vertice().x();
        auto yfrom = x->vertice().y();
        auto xto = d.first->vertice().x();
        auto yto = d.first->vertice().y();

        if (std::abs((xfrom + xto) / 2 +
                     (std::min(yfrom, yto) / std::max(yto, yfrom)) * 20 - a) <=
                40 &&
            std::abs((yfrom + yto) / 2 +
                     (1 - (std::min(yto, yfrom) / std::max(yto, yfrom))) * 20 -
                     b) <= 40) {
          return {1, x->id(), d.first->id()};
        }

        if (Geometry::DistanceToSegment(d.first->vertice(), x->vertice(),
                                        clicked) <= 10) {
          if ((std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{d.first, x}) ==
               selected_edges_.end()) &&
              (std::find(selected_edges_.begin(), selected_edges_.end(),
                         std::pair<Vertex *, Vertex *>{x, d.first}) ==
               selected_edges_.end())) {
            selected_edges_.push_back({d.first, x});
            selected_edges_.push_back({x, d.first});
          } else {
            selected_edges_.erase(
                std::find(selected_edges_.begin(), selected_edges_.end(),
                          std::pair<Vertex *, Vertex *>{d.first, x}));
            selected_edges_.erase(
                std::find(selected_edges_.begin(), selected_edges_.end(),
                          std::pair<Vertex *, Vertex *>{x, d.first}));
          }
          return {0, 0, 0};
        }
      }
    }
    auto a = new Vertex(adjacency_list_.size() + 1, clicked, false, {});
    this->adjacency_list_.push_back(a);
    return {0, 0, 0};
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
                         [&](std::pair<Vertex *, int> el) {
                           return el.first->id() == d->id();
                         }),
          t->connected_.end());
    }
    delete d;
  }
  this->selected_.clear();
}

void Logic::HandleDeleteEdge() {
  std::for_each(selected_edges_.begin(), selected_edges_.end(),
                [](std::pair<Vertex *, Vertex *> el) {
                  auto a = std::find_if(el.first->connected_.begin(),
                                        el.first->connected_.end(),
                                        [&](std::pair<Vertex *, int> search) {
                                          return (el.second == search.first);
                                        });

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
    auto &from_edges = selected_[0]->connected_;
    auto &to_edges = selected_[1]->connected_;
    if (std::find_if(from_edges.begin(), from_edges.end(),
                     [&](std::pair<Vertex *, int> search) {
                       return selected_[1] == search.first;
                     }) == from_edges.end() &&
        std::find_if(to_edges.begin(), to_edges.end(),
                     [&](std::pair<Vertex *, int> search) {
                       return selected_[0] == search.first;
                     }) == to_edges.end()) {
      from_edges.push_back({selected_[1], 1});
      to_edges.push_back({selected_[0], 1});
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

std::vector<QMatrix4x4> Logic::DrawVerticesAPI() {
  std::vector<QMatrix4x4> to_draw;
  for (const auto &x : adjacency_list()) {
    to_draw.push_back(QMatrix4x4(
        static_cast<float>(x->id()), static_cast<float>(x->is_active_),
        static_cast<float>(x->vertice().x()),
        static_cast<float>(x->vertice().y()), .0f, .0f, .0f, .0f, .0f, .0f, .0f,
        .0f, .0f, .0f, .0f, .0f));
  }
  return to_draw;
}

std::vector<QMatrix4x4> Logic::DrawEdgesAPI() {
  std::vector<QMatrix4x4> to_draw;
  for (const auto &x : adjacency_list_) {
    for (const auto &y : x->connected()) {
      if (std::find_if(
              selected_edges_.begin(), selected_edges_.end(),

              [&](std::pair<Vertex *, Vertex *> search) {
                return ((search == std::pair<Vertex *, Vertex *>{x, y.first}) ||
                        (search == std::pair<Vertex *, Vertex *>{y.first, x}));
              }) != selected_edges_.end()) {
        //              std::pair<Vertex *, Vertex *>{x, y}) !=
        //              selected_edges_.end()) {
        to_draw.push_back(
            QMatrix4x4(static_cast<float>(-x->vertice().x()),
                       static_cast<float>(-x->vertice().y()),
                       static_cast<float>(-y.first->vertice().x()),
                       static_cast<float>(-y.first->vertice().y()), y.second,
                       .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f, .0f));
      } else {
        to_draw.push_back(QMatrix4x4(static_cast<float>(x->vertice().x()),
                                     static_cast<float>(x->vertice().y()),
                                     static_cast<float>(y.first->vertice().x()),
                                     static_cast<float>(y.first->vertice().y()),
                                     y.second, .0f, .0f, .0f, .0f, .0f, .0f,
                                     .0f, .0f, .0f, .0f, .0f));
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
                        [&](std::pair<Vertex *, int> el) {
                          file << '{' << el.first->id() - 1 << ',' << el.second
                               << '}' << ',';
                        });
          file << '{'
               << el->connected_[el->connected_.size() - 1].first->id() - 1
               << ',' << el->connected_[el->connected_.size() - 1].second << '}'
               << '}' << '}' << ',';
        } else {
          file << "}},";
        }
      });

  auto el = adjacency_list_[adjacency_list_.size() - 1];
  file << '{' << el->id_ - 1 << ',' << '{' << el->vertice_.x() << ','
       << el->vertice_.y() << '}' << ',' << '{';
  if (el->connected_.size() != 0) {
    std::for_each(el->connected_.begin(), el->connected_.end() - 1,
                  [&](std::pair<Vertex *, int> el) {
                    file << '{' << el.first->id() - 1 << ',' << el.second << '}'
                         << ',';
                  });
    file << '{' << '{'
         << el->connected_[el->connected_.size() - 1].first->id() - 1 << ','
         << el->connected_[el->connected_.size() - 1].second << '}' << '}';
  } else {
    file << "}}";
  }
  file << '}';
  file.close();
}
// ERROR WHILE SEEKING EMPTY VECTOR TO BE FIXED fixed ez
void Logic::Deserialize(QString filepath) {
  HandleDeleteAll();
  /*
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
        std::vector<std::pair<Vertex *, int>>{},
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
  */
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

/// Just copying graph to DESTROY

std::vector<Vertex *> Logic::CopyGraph() {
  std::vector<Vertex *> to_destroy;
  for_each(adjacency_list_.begin(), adjacency_list_.end(), [&](Vertex *x) {
    auto to_push = new Vertex{
        x->id(),
        x->vertice(),
        false,
        std::vector<std::pair<Vertex *, int>>{},
    };
    to_destroy.push_back(to_push);
  });
  for (int i = 0; i < adjacency_list_.size(); i++) {
    std::for_each(adjacency_list_[i]->connected_.begin(),
                  adjacency_list_[i]->connected_.end(),
                  [&](std::pair<Vertex *, int> x) {
                    to_destroy[i]->connected_.push_back(
                        {to_destroy[x.first->id() - 1], x.second});
                  });
  }
  return to_destroy;
}

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
      if (!visited[x.first->id()]) stack.push(x.first->id());
  }
}

void Logic::EulersPath(std::vector<Vertex *> tmp_adjacency) {
  std::vector<int> vertices;
  int odds = 0;
  int start = 0;
  for (const auto &x : adjacency_list_) {
    if (x->connected_.size() % 2) {
      start = x->id();
      odds++;
    }
  }

  if (odds == 0) {
    start = QRandomGenerator::global()->generate() % adjacency_list_.size() + 1;
  }
  qDebug() << odds;
  if (odds != 2 && odds != 0) {
    exit(0xdeadbeef);
  }
  std::stack<int> stack;
  stack.push(start);
  auto s = stack.top() - 1;
  while (!stack.empty()) {
    auto from = adjacency_list_[s];
    adjacency_list_[s]->SetActive(true);
    selected_.push_back(adjacency_list_[s]);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    if (!from->connected_.empty()) {
      stack.push(from->id_);
      auto to = adjacency_list_[s]->connected_[0].first;
      from->connected_.erase(from->connected_.begin());
      for (auto it = to->connected_.begin(); it != to->connected_.end(); it++) {
        if ((*it).first->id() == from->id()) {
          to->connected_.erase(it);
          break;
        }
      }
      adjacency_list_[s]->SetActive(false);
      selected_.pop_back();
      s = to->id() - 1;
    } else {
      vertices.push_back(s);
      s = stack.top() - 1;
      stack.pop();
    }
  }
  selected_.clear();
  for (auto &x : adjacency_list_) {
    x->SetActive(false);
  }
  //  adjacency_list_ = tmp_adjacency;
  std::reverse(vertices.begin(), vertices.end());
  for (int i = 1; i < vertices.size(); i++) {
    selected_.push_back(adjacency_list_[vertices[i]]);
    selected_.push_back(adjacency_list_[vertices[i - 1]]);
    HandleConnection();
    qDebug() << vertices[i] << vertices[i - 1];
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  qDebug() << vertices;
}

int Logic::EulersPathAPI() {
  int e = 0;
  for (const auto &x : adjacency_list_) {
    e += x->connected_.size();
  }

  std::thread a(&Logic::EulersPath, this, CopyGraph());
  a.detach();
  qDebug() << e;
  return e / 2;
}

std::vector<int> Logic::DjikstraAlgo(int source, int to) {
  std::vector<int> parents(adjacency_list_.size());
  std::vector<unsigned long long> dist(adjacency_list_.size(), INT64_MAX);
  std::set<std::pair<int, unsigned long long>> node_path;
  auto tmp = CopyGraph();
  dist[source] = 0;
  node_path.insert(std::pair<int, unsigned long long>(0, source));

  while (!node_path.empty()) {
    std::pair<int, unsigned long long> top = *node_path.begin();
    node_path.erase(node_path.begin());

    int source = top.second;

    for (auto &x : adjacency_list_[source]->connected_) {
      int adj_node = x.first->id() - 1;
      int path_adjnode = x.second;

      if (dist[adj_node] > path_adjnode + dist[source]) {
        parents[adj_node] = source;
        if (dist[adj_node] != INT64_MAX) {
          node_path.erase(node_path.find(
              std::pair<int, unsigned long long>(dist[adj_node], adj_node)));
        }
        dist[adj_node] = path_adjnode + dist[source];
        node_path.insert(
            std::pair<int, unsigned long long>(dist[adj_node], adj_node));
      }
    }
  }
  for (auto &x : adjacency_list_) {
    x->SetActive(false);
  }

  for (auto &x : adjacency_list_) {
    x->connected_.clear();
  }
  selected_.clear();

  std::vector<int> path;
  for (int v = to; v != source; v = parents[v]) path.push_back(v);
  path.push_back(source);
  std::reverse(path.begin(), path.end());

  for (int i = 0; i < adjacency_list_.size(); i++)
    qDebug() << "Source Node(" << source + 1 << ") -> Destination Node(" << i
             << ") : " << dist[i] << '\n';
  for (int i = 1; i < path.size(); i++) {
    selected_.push_back(adjacency_list_[path[i]]);
    selected_.push_back(adjacency_list_[path[i - 1]]);
    HandleConnection();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  adjacency_list_ = tmp;
  qDebug() << path;
  return path;
}

bool Logic::DjikstraAPI() {
  if (selected_.size() == 2) {
    std::thread a(&Logic::DjikstraAlgo, this, selected_[0]->id() - 1,
                  selected_[1]->id() - 1);
    selected_.clear();
    a.detach();
    return true;
  } else {
    selected_.clear();
    return false;
  }
  selected_.clear();
}

void Logic::KruskalAlgo() {
  std::vector<std::vector<int>> prices(
      adjacency_list_.size(), std::vector<int>(adjacency_list_.size(), 0));
  std::vector<std::pair<int, std::pair<int, int>>> g;
  for (const auto &x : adjacency_list_) {
    for (const auto &y : x->connected_) {
      prices[x->id_ - 1][y.first->id_ - 1] = y.second;
      prices[y.first->id_ - 1][x->id_ - 1] = y.second;
      g.push_back({y.second, {x->id() - 1, y.first->id() - 1}});
    }
  }
  int cost = 0;
  std::vector<std::pair<int, int>> res;
  std::sort(g.begin(), g.end());
  std::vector<int> tree_id(adjacency_list_.size());
  for (int i = 0; i < adjacency_list_.size(); ++i) tree_id[i] = i;
  for (int i = 0; i < g.size(); ++i) {
    int a = g[i].second.first, b = g[i].second.second, l = g[i].first;
    if (tree_id[a] != tree_id[b]) {
      cost += l;
      res.push_back(std::make_pair(a, b));
      int old_id = tree_id[b], new_id = tree_id[a];
      for (int j = 0; j < adjacency_list_.size(); ++j)
        if (tree_id[j] == old_id) tree_id[j] = new_id;
    }
  }

  for (auto &x : adjacency_list_) {
    x->SetActive(false);
  }

  for (auto &x : adjacency_list_) {
    x->connected_.clear();
  }

  selected_.clear();

  for (int i = 0; i < res.size(); i++) {
    selected_.push_back(adjacency_list_[res[i].first]);
    selected_.push_back(adjacency_list_[res[i].second]);
    HandleConnection();
    ChangePrice(res[i].first + 1, res[i].second + 1,
                prices[res[i].first][res[i].second]);

    //    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  }
  qDebug() << res;
}

void Logic::ChangePrice(int from, int to, int price) {
  for (auto &x : adjacency_list_[from - 1]->connected_) {
    if (x.first->id() == to) {
      x.second = price;
    }
  }
  for (auto &x : adjacency_list_[to - 1]->connected_) {
    if (x.first->id() == from) {
      x.second = price;
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
