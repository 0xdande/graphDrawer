#ifndef LOGIC_H
#define LOGIC_H

#include <QtQml/qqml.h>
#include <QtQml/qqmlapplicationengine.h>

#include <QMatrix4x4>
#include <QObject>
#include <QQmlComponent>
#include <QString>
#include <QVariant>
#include <QVector3D>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>

#include "vertex.h"
#include "vertice.h"

typedef std::array<int, 5> TArr;

class Logic : public QObject {
  Q_OBJECT
  Q_PROPERTY(std::vector<Vertex *> adjacency_list_ READ adjacency_list WRITE
                 SetAdjacencyList)
  QML_ELEMENT
 public:
  Logic();

 public:
  /// Handlers

  Q_INVOKABLE QVector3D HandleClick(int a, int b);
  Q_INVOKABLE void HandleDelete();
  Q_INVOKABLE void HandleDeleteEdge();
  Q_INVOKABLE void HandleDeleteAll();
  Q_INVOKABLE bool HandleConnection();
  Q_INVOKABLE Vertice NewVertice(int x, int y);
  Q_INVOKABLE bool HandlingSubfunc(Vertice clicked);
  Q_INVOKABLE std::vector<Vertex *> adjacency_list() { return adjacency_list_; }
  Q_INVOKABLE void SetAdjacencyList(std::vector<Vertex *> v) {
    adjacency_list_ = v;
  }
  Q_INVOKABLE void SetIDByCoords(int x, int y);
  Q_INVOKABLE void ChangePrice(int from, int to, int price);

  void DFS(int v, int p, int timer, std::vector<int> &tin,
           std::vector<int> &fup);
  std::vector<Vertex *> CopyGraph();

  /// Drawing API

  Q_INVOKABLE std::vector<QMatrix4x4> DrawVerticesAPI();
  Q_INVOKABLE std::vector<QMatrix4x4> DrawEdgesAPI();

  // Serialize; Deserialize
  Q_INVOKABLE void Serialize(QString filepath);
  Q_INVOKABLE void Deserialize(QString filepath);

  /// Euler's Path
  void EulersPath(std::vector<Vertex *> tmp_adjacency);
  Q_INVOKABLE int EulersPathAPI();

  /// Djikstra Algo
  std::vector<int> DjikstraAlgo(int source, int to);
  Q_INVOKABLE bool DjikstraAPI();

  /// Kruskals Algo
  void KruskalAlgo();
  Q_INVOKABLE void KruskalAPI();

  // Bridges Search
  void BridgeSearch();
  Q_INVOKABLE void BridgesAPI();

 private:
  std::vector<Vertex *> adjacency_list_;
  std::vector<Vertex *> selected_;
  std::vector<bool> used;
  std::vector<std::pair<Vertex *, Vertex *>> selected_edges_;
  uint8_t vertice_radius_ = 30;
  uint8_t last_hold = 0;
  uint8_t holded_id = 0;
};

Q_DECLARE_METATYPE(std::vector<QVector4D>)
Q_DECLARE_METATYPE(std::vector<QMatrix4x4>)

#endif  // LOGIC_H
