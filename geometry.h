#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>
#include <tuple>
#include "vertice.h"
class Geometry {
 public:
  Geometry();
  static std::tuple<double, double, double> LineEquation(Vertice a,
                                                         Vertice b);
  static bool DistanceToLine(Vertice point,
                             std::tuple<double, double, double> line);
};

#endif  // GEOMETRY_H
