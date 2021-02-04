#ifndef LOGIC_H
#define LOGIC_H

#include <qqml.h>
#include <QObject>
#include <QString>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include "vertex.h"
#include "vertice.h"
class Logic : public QObject {
  Q_OBJECT
  Q_PROPERTY(NOTIFY onClick)
  //      userNameChanged)
  QML_ELEMENT
 public:
  Logic();
 public slots:
  Q_INVOKABLE void HandlePress(Vertice clicked);
  Q_INVOKABLE void HandleRelease(Vertice clicked);
  Q_INVOKABLE void HandleDelete();
  Q_INVOKABLE void NewVertice(Vertice clicked);
  Q_INVOKABLE bool HandlingSubfunc(Vertice clicked);

 private:
  std::vector<Vertex*> adjacency_list_;
  std::vector<Vertex*> selected_;
  std::vector<std::pair<Vertex*, Vertex*>> selected_edges_;
  uint8_t vertice_radius_;
};

#endif  // LOGIC_H
