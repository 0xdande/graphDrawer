#include "geometry.h"
Geometry::Geometry() {}

std::tuple<double, double, double> Geometry::LineEquation(Vertice a,
                                                          Vertice b) {
  return std::make_tuple(a.y() - b.y(), b.x() - a.x(),
                         a.x() * b.y() - a.y() * b.y());
}
bool Geometry::DistanceToLine(Vertice point,
                              std::tuple<double, double, double> line) {}
