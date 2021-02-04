#ifndef LOGIC_H
#define LOGIC_H

#include <QtQml/qqml.h>
#include <QtQml/qqmlapplicationengine.h>
#include <QObject>
#include <QQmlComponent>
#include <QString>
#include <tuple>
#include <unordered_set>
#include <utility>
#include <vector>
#include "vertex.h"
#include "vertice.h"
class Logic : public QObject {
  Q_OBJECT
  //      userNameChanged)
  QML_ELEMENT
 public:
  Logic();
 public slots:
  Q_INVOKABLE void HandleClick(int a, int b);
  Q_INVOKABLE void HandleRelease(int a, int b);
  Q_INVOKABLE void HandleHold(int a, int b);
  Q_INVOKABLE void HandleDelete();
  Q_INVOKABLE Vertice NewVertice(uint8_t x, uint8_t y);
  Q_INVOKABLE bool HandlingSubfunc(Vertice clicked);

 private:
  std::vector<Vertex*> adjacency_list_;
  std::vector<Vertex*> selected_;
  std::vector<std::pair<Vertex*, Vertex*>> selected_edges_;
  uint8_t vertice_radius_;
  uint8_t last_hold = 0;

 public:
  QObject* canvas;
};

#endif  // LOGIC_H
