#ifndef GEOMETRY_H
#define GEOMETRY_H
#include <cmath>
#include <tuple>
#include "vertice.h"
class Geometry {
 public:
  Geometry();
  static std::tuple<double, double, double> LineEquation(Vertice a, Vertice b);
  static double DistanceToLine(Vertice point,
                               std::tuple<double, double, double> line);
  static double DistanceToSegment(Vertice begin_segment, Vertice end_segment,
                                  Vertice clicked);
};

#endif  // GEOMETRY_H
