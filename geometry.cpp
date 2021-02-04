#include "geometry.h"
Geometry::Geometry() {}
#define PRECISION 5
std::tuple<double, double, double> Geometry::LineEquation(Vertice a,
                                                          Vertice b) {
  return std::make_tuple(a.y() - b.y(), b.x() - a.x(),
                         a.x() * b.y() - a.y() * b.y());
}
double Geometry::DistanceToLine(Vertice point,
                                std::tuple<double, double, double> line) {
  return abs(std::get<0>(line) * point.x() + std::get<1>(line) * point.y() +
             std::get<2>(line)) /
         std::sqrt(std::get<0>(line) * std::get<0>(line) +
                   std::get<1>(line) * std::get<1>(line));
}

double Geometry::DistanceToSegment(Vertice begin_segment, Vertice end_segment,
                                   Vertice clicked) {
  double segment_length = std::sqrt((begin_segment.x() - end_segment.x()) *
                                        (begin_segment.x() - end_segment.x()) +
                                    (begin_segment.y() - end_segment.y()) *
                                        (begin_segment.y() - end_segment.y()));
  double cla = std::sqrt(
      (begin_segment.x() - clicked.x()) * (begin_segment.x() - clicked.x()) +
      (begin_segment.y() - clicked.y()) * (begin_segment.y() - clicked.y()));

  double clb = std::sqrt(
      (end_segment.x() - clicked.x()) * (end_segment.x() - clicked.x()) +
      (end_segment.y() - clicked.y()) * (end_segment.y() - clicked.y()));

  double angle1 = (clb * clb - cla * cla - segment_length * segment_length) /
                  (-2 * cla * segment_length);
  double angle2 = (cla * cla - clb * clb - segment_length * segment_length) /
                  (-2 * clb * segment_length);

  if (angle1 >= 0 && angle2 >= 0) {
    return Geometry::DistanceToLine(
        clicked, Geometry::LineEquation(begin_segment, end_segment));
  }
  if (angle1 <= 0) {
    return cla;
  }
  if (angle2 <= 0) {
    return clb;
  }

  return INT_FAST64_MAX;
}
