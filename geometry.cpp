#include "geometry.h"
Geometry::Geometry() {}

std::tuple<double, double, double> Geometry::LineEquation(Vertice a,
                                                          Vertice b) {
  return std::make_tuple(a.y() - b.y(), b.x() - a.x(),
                         a.x() * b.y() - a.y() * b.y());
}
bool Geometry::DistanceToLine(Vertice point,
                              std::tuple<double, double, double> line) {
  return abs(std::get<0>(line) * point.x() + std::get<1>(line) * point.y() +
             std::get<2>(line)) /
             std::sqrt(std::get<0>(line) * std::get<0>(line) +
                       std::get<1>(line) * std::get<1>(line)) <= 5;
}
