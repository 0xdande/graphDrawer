#ifndef LOGIC_H
#define LOGIC_H

#include <qqml.h>
#include <QObject>
#include <QString>
#include <tuple>
#include <unordered_set>
#include <vector>
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
  Q_INVOKABLE void HandleRelease(uint8_t id);
  Q_INVOKABLE void HandleDelete(uint8_t id);

 private:
  std::vector<Vertice*> adjacency_list_;
  uint8_t vertice_radius_;
};

#endif  // LOGIC_H
